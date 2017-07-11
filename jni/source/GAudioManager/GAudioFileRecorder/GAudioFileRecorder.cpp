#include "GAudioFileRecorder.h"
#include "../GAudioEngine/GAudioEngine.h"
#include "../../../framework/GEngineEvent/GEngineEvent.h"

void GAudioFileRecorderRunLoop(void *data) { if ( data ) ( ( GAudioFileRecorder * )data )->runLoop(); }

void GAudioFileRecorder::install()
{
	//初始化全局变量
	mRunning		= false;
	mActionSeed		= 0;
	mThread			= 0;
	memset( mAudioQueueBuffer, 0, 256 );
	memset( mFilePath		 , 0, 256 );

	//创建锁
	mLock = mUtility.createLock();
}

void GAudioFileRecorder::unInstall()
{
	//停止工作
	bool isRunning = false;
	mUtility.lock( mLock );
	isRunning = mRunning;
	mRunning = false;
	mUtility.unlock( mLock );
	if ( isRunning ) mUtility.waitingThread( mThread );

	//销毁锁
	mUtility.destorylock( &mLock );

	//初始化全局变量
	mRunning		= false;
	mActionSeed		= 0;
	mThread			= 0;
	memset( mAudioQueueBuffer, 0, 256 );
	memset( mFilePath		 , 0, 256 );
}

int	 GAudioFileRecorder::start(JNIEnv* env, jstring path)
{
	int result = 0;

	do
	{
		//判断参数是否有效
		if ( !env || !path ) break;

		//判断当前是否正在工作
		bool isRunning = false;
		mUtility.lock( mLock );
		isRunning = mRunning;
		if ( !isRunning ) mRunning = true;
		mUtility.unlock( mLock );
		if ( isRunning ) break;

		//解析文件路径信息
		memset( mFilePath, 0, 256 );
		int length = mConverter.getStringBytes( env, path, mFilePath, 256 );
		if ( length <= 0 ) break;

		//尝试打开文件
		FILE *file = fopen( mFilePath, "w+" );
		if ( file ) fclose( file );
		else break;

		//启动线程
		if ( !mUtility.createThread( &mThread, GAudioFileRecorderRunLoop, this ) ) break;

		//设定结果flag
		mActionSeed++;
		result = mActionSeed;

	} while(0);

	//判断是否成功启动
	if ( result <= 0 ) stop();

	return result;
}

void GAudioFileRecorder::stop()
{
	//设定flag
	mUtility.lock( mLock );
	mRunning = false;
	mUtility.unlock( mLock );
}

void GAudioFileRecorder::runLoop()
{
	//生成开始事件
	GEngineEvent::sharedInstance().swallowEvent( EET_AUDIO_FILE_RECORD_START, 0, mActionSeed );

	//设定状态flag
	mUtility.lock( mLock );
	mRunning = true;
	mUtility.unlock( mLock );

	//变量定义
	FILE *file 		= 0;
	int	 readsize	= 0;
	int	 savesize	= 0;
	bool isRunning 	= false;

	do
	{
		//打开文件
		file = fopen( mFilePath, "w+" );
		if ( !file ) break;

		//启动录音器
		if ( !GAudioEngine::sharedInstance().startRecord() ) break;

		//循环读取->存入数据
		while ( true )
		{
			//取得状态flag
			mUtility.lock( mLock );
			isRunning = mRunning;
			mUtility.unlock( mLock );
			if ( !isRunning ) break;

			//读录音数据
			readsize = GAudioEngine::sharedInstance().readRecordedData( mAudioQueueBuffer, 256 );
			if ( readsize <= 0 ) continue;

			//保存数据
			savesize = fwrite( mAudioQueueBuffer, 1, readsize, file );

			//判断是否正常保存
			if ( savesize != readsize ) break;
		}

		//停止录音器
		GAudioEngine::sharedInstance().stopRecord();

	} while( 0 );

	//关闭文件
	if ( file ) { fclose( file ); file = 0; }

	//设定状态flag
	mUtility.lock( mLock );
	mRunning = false;
	mUtility.unlock( mLock );

	//生成结束事件
	GEngineEvent::sharedInstance().swallowEvent( EET_AUDIO_FILE_RECORD_STOP, 0, mActionSeed );
}

