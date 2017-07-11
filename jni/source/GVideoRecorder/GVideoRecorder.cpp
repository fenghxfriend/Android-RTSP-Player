#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "GVideoRecorder.h"
#include "../../framework/GEngineEvent/GEngineEvent.h"

void GVideoRecorderRunLoop(void *data)
{
	if ( data ) ( ( GVideoRecorder * )data )->runLoop();
}

GVideoRecorder &GVideoRecorder::sharedInstance()
{
	static GVideoRecorder instance;
	return instance;
}

void GVideoRecorder::install()
{
	//初始化全局变量
	mRunning 				= false;
	mLiveLocked				= false;
	mRTSPClientDetectFlag 	= 0; // 1:start   0:none   -1:stop
	mSourceLock 			= mUtility.createLock();
	mLiveLock 				= mUtility.createLock();
	memset( mRecordFilePath, 0, 256 );
	initRecord( true );
}

void GVideoRecorder::unInstall()
{
	//结束后台工作
	stop( true );

	//释放资源
	mUtility.destorylock( &mSourceLock );
	mUtility.destorylock( &mLiveLock   );
}

void GVideoRecorder::start(JNIEnv* env, jstring filePath)
{
	bool result = false;

	do
	{
		//判断参数是否有效
		if ( !env || !filePath ) break;

		//取得running
		bool running = false;
		mUtility.lock( mSourceLock );
		running = mRunning;
		mUtility.unlock( mSourceLock );
		if ( running ) break;

		//取得文件路径信息
		memset( mRecordFilePath, 0, 256 );
		int length = mConverter.getStringBytes( env, filePath, mRecordFilePath, 256 );
		if ( length <= 0 ) break;

		//开始录像
		if ( !mLiveLocked ) { mLiveLocked = true; mUtility.lock( mLiveLock ); }
		if ( !mUtility.createThread( &mBackgroundWorker, GVideoRecorderRunLoop, this ) ) break;

		//设定结果flag
		result = true;

	} while(0);

	//通知上层，录像失败
	if ( !result ) startResponse( false );
}

void GVideoRecorder::stop() { stop(false); }
void GVideoRecorder::stop(bool wait)
{
	bool result = false;

	do
	{
		//取得running
		bool running = false;
		mUtility.lock( mSourceLock );
		running = mRunning;
		mUtility.unlock( mSourceLock );
		if ( !running ) break;

		//停止工作
		if ( mLiveLocked ) { mLiveLocked = false; mUtility.unlock( mLiveLock ); }

		//等待后台工作结束
		if ( wait ) mUtility.waitingThread( mBackgroundWorker );

		//设定结果flag
		result = true;

	} while(0);

	//通知上层，停止失败
	if ( !result && !wait ) stopResponse( false );
}

void GVideoRecorder::runLoop()
{
	bool RTSPClientFinishHandle = false;
	bool recordSuccess			= false;
	bool errorOccur				= false;

	while( !mUtility.trylock( mLiveLock ) )
	{
		//设定running
		mUtility.lock( mSourceLock );
		mRunning = true;
		mUtility.unlock( mSourceLock );

		//录像
		do
		{
			//创建文件
			FILE *newFile = fopen( mRecordFilePath, "w+" );
			if ( newFile ) fclose( newFile );
			else break;

			//准备录制
			if ( !prepareRecord() )
			{
				//通知上层，录像失败
				startResponse( false );

				//停止
				errorOccur = true;
				break;
			}

			//清空缓冲区
			mDataBuffer.clear();

			//通知RTSP客户端开始录像
			mUtility.lock( mSourceLock );
			mRTSPClientDetectFlag = 1;
			mUtility.unlock( mSourceLock );

			//等待RTSP客户端处理结束
			RTSPClientFinishHandle = false;
			while( true )
			{
				mUtility.lock( mSourceLock );
				RTSPClientFinishHandle = ( mRTSPClientDetectFlag == 0 );
				mUtility.unlock( mSourceLock );
				if ( RTSPClientFinishHandle ) break;
			}

			//通知上层，开始录像
			startResponse( true );

			//开始录制
			while( true )
			{
				//循环录制
				recordRunLoop();

				//判断是否停止
				if ( mUtility.trylock( mLiveLock ) )
				{
					//解锁
					mUtility.unlock( mLiveLock );

					//通知RTSP客户端停止录像
					mUtility.lock( mSourceLock );
					mRTSPClientDetectFlag = -1;
					mUtility.unlock( mSourceLock );

					//等待RTSP客户端处理结束
					RTSPClientFinishHandle = false;
					while( true )
					{
						mUtility.lock( mSourceLock );
						RTSPClientFinishHandle = ( mRTSPClientDetectFlag == 0 );
						mUtility.unlock( mSourceLock );
						if ( RTSPClientFinishHandle ) break;
					}

					//停止
					break;
				}
			}

			//结束录制
			finishRecord();

			//通知上层
			stopResponse( true );

			//设定flag
			recordSuccess = true;

		} while(0);

		//删除文件
		if ( !recordSuccess ) { if ( access( mRecordFilePath, 0 ) == 0 ) remove( mRecordFilePath ); }

		//设定running
		mUtility.lock( mSourceLock );
		mRunning = false;
		mUtility.unlock( mSourceLock );

		//判断是否发生错误
		if ( errorOccur ) break;
	}

	//解锁
	if ( !errorOccur ) mUtility.unlock( mLiveLock );
}

void GVideoRecorder::write(void *data, int size)
{
	if ( data && size > 0 ) mDataBuffer.writePacket( ( char * )data, size );
}

int GVideoRecorder::RTSPClientDetect()
{
	int result = 0;
	mUtility.lock( mSourceLock );
	result = mRTSPClientDetectFlag;
	mRTSPClientDetectFlag = 0;
	mUtility.unlock( mSourceLock );
	return result;
}

void GVideoRecorder::startResponse(bool success)
{
	GEngineEvent::sharedInstance().swallowEvent( EET_VIDEO_RECORD_RESPONSE_START, 0, success ? 0 : -1 );
}

void GVideoRecorder::stopResponse(bool success)
{
	GEngineEvent::sharedInstance().swallowEvent( EET_VIDEO_RECORD_RESPONSE_STOP, 0, success ? 0 : -1 );
}

//=======================================================================================================
//
//视频录制
//
//=======================================================================================================
void GVideoRecorder::initRecord(bool init)
{
	//释放资源
	if ( !init )
	{
		//关闭文件
		if ( mAVFormatContext && mAVFormatContext->pb )
		{
			avio_close( mAVFormatContext->pb );
			mAVFormatContext->pb = 0;
		}

		//释放格式上下文
		if ( mAVFormatContext )
		{
			avformat_free_context( mAVFormatContext );
			mAVFormatContext = 0;
		}
	}

	//初始化全局变量
	if ( init )
	{
		mVideoWidth  = 0;
		mVideoHeight = 0;
	}
	mAVFormatContext = 0;
	mAVOutputFormat  = 0;
	mAVStream		 = 0;

	//注册编码器
	if ( init ) av_register_all();
}

void GVideoRecorder::setVideoSize(int width, int height)
{
	mVideoWidth  = width;
	mVideoHeight = height;
}

bool GVideoRecorder::prepareRecord()
{
	bool result = false;

	do
	{
		//初始化
		initRecord( false );

		//创建格式上下文
//		mAVFormatContext = avformat_alloc_context();
		avformat_alloc_output_context2( &mAVFormatContext, 0, 0, mRecordFilePath );
		if ( !mAVFormatContext ) break;
		mAVOutputFormat = mAVFormatContext->oformat;

		//解析输出格式
//		mAVOutputFormat = av_guess_format( 0, mRecordFilePath, 0 );
//		if ( !mAVOutputFormat || mAVOutputFormat->video_codec == CODEC_ID_NONE ) break;

		//配置输出信息
//		mAVFormatContext->oformat = mAVOutputFormat;
//		sprintf( mAVFormatContext->filename, "%s", mRecordFilePath );

		//创建输出流
//		mAVStream = avformat_new_stream( mAVFormatContext, avcodec_find_encoder( CODEC_ID_H264 ) );
		mAVStream = avformat_new_stream( mAVFormatContext, 0 );
//        mAVStream->codec->codec_id 		= mAVOutputFormat->video_codec;
		mAVStream->codec->codec_id 		= CODEC_ID_H264;
        mAVStream->codec->codec_type 	= AVMEDIA_TYPE_VIDEO;
        mAVStream->codec->bit_rate 		= 400000; //unused for constant quantizer encoding.
        mAVStream->codec->width 		= mVideoWidth;
        mAVStream->codec->height 		= mVideoHeight;
        mAVStream->codec->time_base.num = 1;
        mAVStream->codec->time_base.den	= 11; //帧率
        mAVStream->codec->gop_size 		= 12;
        mAVStream->codec->pix_fmt 		= PIX_FMT_YUV420P;
		if( mAVOutputFormat->flags & AVFMT_GLOBALHEADER) mAVStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;

        //检查配置是否正确
        av_dump_format( mAVFormatContext, 0, mRecordFilePath, 1 );

        //打开文件
//	    if ( !( mAVOutputFormat->flags & AVFMT_NOFILE ) )
        if ( avio_open( &( mAVFormatContext->pb ), mRecordFilePath, AVIO_FLAG_WRITE) != 0 ) break;

        //写容器头
        if ( avformat_write_header( mAVFormatContext, 0 ) != 0 ) break;

        /*
        int ret = avformat_write_header( mAVFormatContext, 0 );
        if ( ret < 0 )
        {
        	char msg[64];
        	memset( msg, 0, 64 );
        	printf("*************************** running 004 002 return = %s \n", av_make_error_string( msg, 64, ret ) );
        	break;
        }
        */


		//设定结果flag
		result = true;

	} while(0);

	//初始化
	if ( !result ) initRecord( false );

	return result;
}

bool GVideoRecorder::recordRunLoop()
{
	bool result = true;

	//读取一个包的数据
	mActivePacketSize = mDataBuffer.readPacket( mPacketData );

	//判断是否有效
	if ( mActivePacketSize > 0 )
	{
		//创建包
	    AVPacket packet;
	    av_init_packet( &packet );
	    packet.flags 		|= AV_PKT_FLAG_KEY;
	    packet.stream_index  = mAVStream->index;
	    packet.data 		 = ( unsigned char * )mPacketData;
	    packet.size 		 = mActivePacketSize;

	    //写入包
	    if ( av_write_frame( mAVFormatContext, &packet ) < 0 ) result = false;
	}

	return result;
}

void GVideoRecorder::finishRecord()
{
	//写容器尾
	av_write_trailer( mAVFormatContext );

	//初始化
	initRecord( false );
}

