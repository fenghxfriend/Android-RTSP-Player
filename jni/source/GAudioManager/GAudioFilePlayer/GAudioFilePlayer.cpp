#include <fcntl.h>
#include "GAudioFilePlayer.h"
#include "../GAudioEngine/GAudioEngine.h"
#include "../../../framework/GEngineEvent/GEngineEvent.h"

void GAudioFilePlayerCallback( void *buffer, int *size, int maxsize, void *data )
{ if ( data ) ( ( GAudioFilePlayer * )data )->playerCallback( buffer, size, maxsize ); }

void GAudioFilePlayer::install(JNIEnv* env, jobject assetManager)
{
	//初始化全局变量
	mRunning			= false;
	mAssetManager 		= 0;
	mAAsset				= 0;
	mCustomFile			= 0;
	mActionSeed	  		= 0;
	mPlayingAssetFile 	= false;
	memset( mFilePath, 0, 256 );

	//创建锁
	mLock = mUtility.createLock();

	//保存AssetManager
	mAssetManager = AAssetManager_fromJava(env, assetManager);
}

void GAudioFilePlayer::unInstall()
{
	//停止播放
	stop( false );

	//初始化全局变量
	mUtility.lock( mLock );
	mRunning = false;
	mUtility.unlock( mLock );
	mAssetManager 		= 0;
	mAAsset				= 0;
	mCustomFile			= 0;
	mActionSeed	  		= 0;
	mPlayingAssetFile 	= false;
	memset( mFilePath, 0, 256 );

	//销毁锁
	mUtility.destorylock( &mLock );
}

int GAudioFilePlayer::playAssetFile (JNIEnv* env, jstring path) { return play( env, path, true  ); }
int GAudioFilePlayer::playCustomFile(JNIEnv* env, jstring path) { return play( env, path, false ); }
int GAudioFilePlayer::play			(JNIEnv* env, jstring path, bool asset)
{
	int result = 0;

	do
	{
		//判断状态
		bool isRunning = false;
		mUtility.lock( mLock );
		isRunning = mRunning;
		if ( !isRunning ) mRunning = true;
		mUtility.unlock( mLock );
		if ( isRunning ) break;

		//取得文件路径信息
		memset( mFilePath, 0, 256 );
		int length = mConverter.getStringBytes( env, path, mFilePath, 256 );
		if ( length <= 0 ) break;

		//打开文件
		if ( asset )
		{
			mAAsset = AAssetManager_open( mAssetManager, mFilePath, AASSET_MODE_UNKNOWN );
			if ( !mAAsset ) break;
		}
		else
		{
			//判断文件是否存在
			if ( access( mFilePath, 0 ) != 0 ) break;

			//打开文件
			mCustomFile = fopen( mFilePath, "rb+" );
			if ( !mCustomFile ) break;
		}
		mPlayingAssetFile = asset;

		//开始回放
		if ( !GAudioEngine::sharedInstance().startPlayback( GAudioFilePlayerCallback, this ) ) break;

		//设定结果flag
		mActionSeed++;
		result = mActionSeed;

		//生成事件
		GEngineEvent::sharedInstance().swallowEvent( EET_AUDIO_FILE_PLAYER_START, 0, mActionSeed );

	} while(0);

	//判断是否成功
	if ( result <= 0 )
	{
		//关闭文件
		if ( mPlayingAssetFile ) AAsset_close( mAAsset );
		if ( mCustomFile 	   ) fclose( mCustomFile );

		//初始化全局变量
		mUtility.lock( mLock );
		mRunning = false;
		mUtility.unlock( mLock );
		mAAsset				= 0;
		mCustomFile			= 0;
		mPlayingAssetFile 	= false;
		memset( mFilePath, 0, 256 );
	}

	return result;
}

void GAudioFilePlayer::stop() { stop( true, true ); }
void GAudioFilePlayer::stop(bool notifyUser) { stop( notifyUser, true ); }
void GAudioFilePlayer::stop(bool notifyUser, bool stopPlayer)
{
	do
	{
		//判断状态
		bool isRunning = false;
		mUtility.lock( mLock );
		isRunning = mRunning;
		if ( isRunning ) mRunning = false;
		mUtility.unlock( mLock );
		if ( !isRunning ) break;

		//停止回放
		if ( stopPlayer ) GAudioEngine::sharedInstance().stopPlayback();

		//关闭文件
		if ( mPlayingAssetFile ) AAsset_close( mAAsset );
		if ( mCustomFile 	   ) fclose( mCustomFile );

		//初始化全局变量
		mAAsset				= 0;
		mCustomFile			= 0;
		mPlayingAssetFile 	= false;
		memset( mFilePath, 0, 256 );

		//生成事件
		if ( notifyUser ) GEngineEvent::sharedInstance().swallowEvent( EET_AUDIO_FILE_PLAYER_STOP, 0, mActionSeed );

	} while(0);
}

void GAudioFilePlayer::playerCallback(void *buffer, int *size, int maxsize)
{
	if ( mPlayingAssetFile ) ( *size ) = AAsset_read( mAAsset, buffer, maxsize  			);
	else 					 ( *size ) = fread		( buffer , 1     , maxsize, mCustomFile );

	//判断是否结束
	if ( ( *size ) <= 0 ) stop( true, false );

}

