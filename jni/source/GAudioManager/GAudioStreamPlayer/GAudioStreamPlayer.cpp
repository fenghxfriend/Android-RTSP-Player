#include "GAudioStreamPlayer.h"
#include "../GAudioEngine/GAudioEngine.h"
#include "../../../framework/GMediaEngine/GMAudio/GMAudio.h"
#include "../../../framework/GGameEngine/CommonInfo/MsgDef.h"
#include "../../../framework/GGameEngine/GGameClient/GGameClient.h"

void GAudioStreamPlayerCallback( void *buffer, int *size, int maxsize, void *data )
{ if ( data ) ( ( GAudioStreamPlayer * )data )->playerCallback( buffer, size, maxsize ); }
void GAudioStreamPlayerStartCallback(void *data)
{ if ( data ) ( ( GAudioStreamPlayer * )data )->RTSPStart(); }
void GAudioStreamPlayerStopCallback (void *data)
{ if ( data ) ( ( GAudioStreamPlayer * )data )->RTSPStop();  }

void GAudioStreamPlayer::install()
{
	//初始化全局变量
	mRunning  = false;
	mSleeping = false;
	memset( mRTSPAddress, 0, 100 );

	//清空队列
	mAudioQueue.clear();

	//创建锁
	mLock = mUtility.createLock();
}

void GAudioStreamPlayer::unInstall()
{
	//停止
	stop();

	//清空队列
	mAudioQueue.clear();

	//初始化全局变量
	mRunning  = false;
	mSleeping = false;
	memset( mRTSPAddress, 0, 100 );

	//销毁锁
	mUtility.destorylock( &mLock );
}

void GAudioStreamPlayer::start()
{
	do
	{
		//判断状态
		bool running = false;
		mUtility.lock  ( mLock );
		running  = mRunning;
		if ( !running )
		{
			mRunning  = true;
			mSleeping = false;
		}
		mUtility.unlock( mLock );
		if ( running ) break;

		//清空缓冲区
		mAudioQueue.clear();

		//启动RTSP客户端
		GMAudioClientStart( mRTSPAddress, ( void * )GAudioStreamPlayerStartCallback, ( void * )GAudioStreamPlayerStopCallback, this );

	} while(0);
}

void GAudioStreamPlayer::stop()
{
	do
	{
		//判断状态
		bool running = false;
		mUtility.lock  ( mLock );
		running = mRunning;
		mUtility.unlock( mLock );
		if ( !running ) break;

		//停止RTSP客户端
		GMAudioClientStop();

	} while(0);
}

void GAudioStreamPlayer::setRTSPIPAddress(const char *ip)
{
	memset ( mRTSPAddress, 0 , 100 );
	sprintf( mRTSPAddress, "rtsp://%s:18126/ToyAudioServer", ip );
}

void GAudioStreamPlayer::RTSPStart()
{
	//启动播放器
	GAudioEngine::sharedInstance().startPlayback( GAudioStreamPlayerCallback, this );

	//通知上层
	int result = 1;
	GGameClient::sharedInstance().swallowMessage( MUIM_M2C_OpenModelSoundToPDAResponse, &result, sizeof(int) );
}

void GAudioStreamPlayer::RTSPStop()
{
	//停止播放器
	GAudioEngine::sharedInstance().stopPlayback();

	//设定状态
	mUtility.lock  ( mLock );
	mRunning  = false;
	mSleeping = false;
	mUtility.unlock( mLock );

	//清空队列
	mAudioQueue.clear();

	//通知上层
	int result = 1;
	GGameClient::sharedInstance().swallowMessage( MUIM_M2C_CloseModelSoundToPDAResponse, &result, sizeof(int) );
}

void GAudioStreamPlayer::write(void *data, int size)
{
	do
	{
		//判断状态
		bool running = false;
		mUtility.lock  ( mLock );
		running = mRunning;
		mUtility.unlock( mLock );
		if ( !running ) break;

		//写入队列
		mAudioQueue.write( ( char * )data, size );

		//判断是否唤醒
		if ( mSleeping ) GAudioEngine::sharedInstance().wakePlayer();

	} while(0);
}

void GAudioStreamPlayer::playerCallback(void *buffer, int *size, int maxsize)
{
	//读取数据
	(*size) = mAudioQueue.read( ( char * )buffer, maxsize );

	//判断是否睡眠
	if ( (*size) <= 0 )
	{
		GAudioEngine::sharedInstance().sleepPlayer();
		mSleeping = true;
	}
}
