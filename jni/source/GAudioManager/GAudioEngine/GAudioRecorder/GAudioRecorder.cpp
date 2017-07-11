#include "GAudioRecorder.h"

void GAudioRecorderQueueCallback(SLAndroidSimpleBufferQueueItf queue, void *data)
{ if ( queue && data ) ( ( ( GAudioRecorder * )data )->audioQueueCallback( queue ) ); }

void GAudioRecorder::install(SLEngineItf engine)
{
	//初始化全局变量
	mRecorderObject			= 0;
	mRecorderInterface		= 0;
	mRecorderBufferQueue	= 0;
	mRunning				= false;
	mInstalled				= false;

	//创建锁
	mLock = mUtility.createLock();

	//清空队列
	mAudioQueue.clear();

	//初始化缓冲区
	memset( mAudioQueueBuffer, 0, GAUDIORECORDERBUFFERSIZE );

	//创建录音器
	createRecorder( engine );
}

void GAudioRecorder::unInstall()
{
	//销毁录音器
	destoryRecorder();

	//初始化缓冲区
	memset( mAudioQueueBuffer, 0, GAUDIORECORDERBUFFERSIZE );

	//清空队列
    mAudioQueue.clear();

	//销毁锁
	mUtility.destorylock( &mLock );

	//初始化全局变量
	mRunning 	= false;
	mInstalled 	= false;

}

bool GAudioRecorder::start()
{
	bool result = false;

	do
	{
		//判断是否安装完成
		if ( !mInstalled ) break;

		//清空队列
	    mAudioQueue.clear();

		//启动
		mUtility.lock  ( mLock );
		mRunning = true;
		mUtility.unlock( mLock );

		//设定结果flag
		result = true;

	} while(false);

	return result;
}

bool GAudioRecorder::stop()
{
	bool result = false;

	do
	{
		//判断是否安装完成
		if ( !mInstalled ) break;

		//停止
		mUtility.lock  ( mLock );
		mRunning = false;
		mUtility.unlock( mLock );

		//清空队列
	    mAudioQueue.clear();

		//设定结果flag
		result = true;

	} while(false);

	return result;
}

int GAudioRecorder::read(void *buffer, int size)
{
	int result = 0;

	do
	{
		//判断参数是否有效
		if ( !buffer || size <= 0 ) break;

		//从队列中读数据
		result = mAudioQueue.read( ( char * )buffer, size );

	} while(false);

	return result;
}

void GAudioRecorder::audioQueueCallback(SLAndroidSimpleBufferQueueItf queue)
{
	do
	{
		if ( !queue ) break;

		//取得当前状态
		bool running = false;
		mUtility.lock  ( mLock );
		running = mRunning;
		mUtility.unlock( mLock );

		//保存录制的数据
		if ( running ) mAudioQueue.write( mAudioQueueBuffer, GAUDIORECORDERBUFFERSIZE );

		//清空队列
		( *queue )->Clear( queue );

		//继续录制数据
		( *queue )->Enqueue( queue, mAudioQueueBuffer, GAUDIORECORDERBUFFERSIZE );

	} while(0);
}

void GAudioRecorder::createRecorder(SLEngineItf engine)
{
	SLresult result  = SL_RESULT_UNKNOWN_ERROR;
	bool 	 result0 = false;

	do
	{
		//配置音频源
	    SLDataLocator_IODevice localDevice = { SL_DATALOCATOR_IODEVICE, SL_IODEVICE_AUDIOINPUT, SL_DEFAULTDEVICEID_AUDIOINPUT, NULL };
	    SLDataSource 		   audioSource = { &localDevice, NULL };

	    //配置音频sink
	    SLDataLocator_AndroidSimpleBufferQueue localBufferQueue = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2 };
	    SLDataFormat_PCM 					   audioFormat 	 	= { SL_DATAFORMAT_PCM			, 1							  ,
	    															 SL_SAMPLINGRATE_22_05		,
	    															 SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16 ,
	    															 SL_SPEAKER_FRONT_CENTER	, SL_BYTEORDER_LITTLEENDIAN };
	    SLDataSink 							   audioSink 		= { &localBufferQueue, &audioFormat };

    	//创建录音器
	    SLInterfaceID recorderIds[3] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE };
	    SLboolean 	  recorderReq[3] = { SL_BOOLEAN_TRUE 				 };
    	result = ( *engine )->CreateAudioRecorder( engine, &mRecorderObject, &audioSource, &audioSink, 1, recorderIds, recorderReq );
    	if ( SL_RESULT_SUCCESS != result ) break;

    	//实现录音器
    	result = ( *mRecorderObject )->Realize( mRecorderObject, SL_BOOLEAN_FALSE );
    	if ( SL_RESULT_SUCCESS != result ) break;

    	//取得录音器接口
    	result = ( *mRecorderObject )->GetInterface( mRecorderObject, SL_IID_RECORD, &mRecorderInterface );
    	if ( SL_RESULT_SUCCESS != result ) break;

    	//取得录音器录制队列
    	result = ( *mRecorderObject )->GetInterface( mRecorderObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &mRecorderBufferQueue );
    	if ( SL_RESULT_SUCCESS != result ) break;

    	//注册录制队列回调函数
    	result = ( *mRecorderBufferQueue )->RegisterCallback( mRecorderBufferQueue, GAudioRecorderQueueCallback, this );
    	if ( SL_RESULT_SUCCESS != result ) break;
    	//清空队列
    	result = ( *mRecorderBufferQueue )->Clear( mRecorderBufferQueue );
    	if ( SL_RESULT_SUCCESS != result ) break;

    	//填充队列
    	result = ( *mRecorderBufferQueue )->Enqueue( mRecorderBufferQueue, mAudioQueueBuffer, GAUDIORECORDERBUFFERSIZE );
    	if ( SL_RESULT_SUCCESS != result ) break;

    	//启动录音器
    	result = ( *mRecorderInterface )->SetRecordState( mRecorderInterface, SL_RECORDSTATE_RECORDING );
    	if ( SL_RESULT_SUCCESS != result ) break;

    	//设定flag
    	result0 	= true;
    	mInstalled 	= true;

	} while(0);

	if ( !result0 ) destoryRecorder();
}

void GAudioRecorder::destoryRecorder()
{
	//销毁录音器
    if ( mInstalled )
    {
    	//停止录制
    	( *( mRecorderInterface ) )->SetRecordState( mRecorderInterface, SL_RECORDSTATE_STOPPED );

    	//销毁
        ( *( mRecorderObject ) )->Destroy( mRecorderObject );
    }

	//初始化全局变量
	mRecorderObject			= 0;
	mRecorderInterface		= 0;
	mRecorderBufferQueue	= 0;
}
