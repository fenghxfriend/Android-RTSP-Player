#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <dirent.h>
#include "GAudioHornUpdate.h"
#include "../../../framework/GEngineEvent/GEngineEvent.h"
#include "../../../framework/GGameEngine/GGameCommand/GGameCommand.h"
#include "../../../framework/GGameEngine/GGameClient/GGameClient.h"
#include "../../../framework/GGameEngine/CommonInfo/MsgIdDef.h"

void GAudioHornUpdateFileServerRunLoop(void *data) { if ( data ) ( ( GAudioHornUpdate * )data )->runLoop(); }
int GAudioHornUpdate::currentAction() { return mActionSeed; }

void GAudioHornUpdate::install(JNIEnv* env, jobject assetManager)
{
	//初始化全局变量
	mRunning		= false;
	mThread 		= 0;
	mActionSeed		= 0;
	mAssetManager	= 0;
	mAAsset			= 0;
	mCustomFile		= 0;
	mIsAssetFile	= false;
	mUpdateTarget	= 0;
	memset( mFilePath		 , 0, 256  );
	memset( mLocalIPAddress	 , 0, 256  );
	memset( mFileServerBuffer, 0, 1024 );

	//保存AssetManager
	mAssetManager = AAssetManager_fromJava(env, assetManager);

	//创建锁
	mLock = mUtility.createLock();
}

void GAudioHornUpdate::unInstall()
{
	//取得状态flag
	bool isRunning = false;
	mUtility.lock( mLock );
	isRunning = mRunning;
	mUtility.unlock( mLock );

	//等待任务结束
	if ( isRunning ) mUtility.waitingThread( mThread );

	//销毁锁
	mUtility.destorylock( &mLock );

	//初始化全局变量
	mRunning		= false;
	mThread 		= 0;
	mActionSeed		= 0;
	mAssetManager	= 0;
	mAAsset			= 0;
	mCustomFile		= 0;
	mIsAssetFile	= false;
	mUpdateTarget	= 0;
	memset( mFilePath		 , 0, 256  );
	memset( mLocalIPAddress	 , 0, 256  );
	memset( mFileServerBuffer, 0, 1024 );
}

int GAudioHornUpdate::updateCustomFile(JNIEnv* env, jstring ip, jstring path, jint target) { return update( env, ip, path, target, false ); }
int GAudioHornUpdate::updateAssetFile (JNIEnv* env, jstring ip, jstring path, jint target) { return update( env, ip, path, target, true  ); }
int GAudioHornUpdate::update(JNIEnv* env, jstring ip, jstring path, jint target, bool asset)
{
	int result = 0;

	do
	{
		//判断参数是否有效
		if ( !env || !ip || !path || target < 0 || target > 2 ) break;

		//判断当前是否正在运行
		bool isRunning = false;
		mUtility.lock( mLock );
		isRunning = mRunning;
		mUtility.unlock( mLock );
		if ( isRunning ) break;

		//保存参数信息
		memset( mLocalIPAddress, 0, 256 );
		memset( mFilePath	   , 0, 256 );
		int length0 = mConverter.getStringBytes( env, ip  , mLocalIPAddress, 256 );
		int length1 = mConverter.getStringBytes( env, path, mFilePath	   , 256 );
		if ( length0 <= 0 || length1 <= 0 ) break;
		mIsAssetFile  = asset;
		mUpdateTarget = target;

		//启动工作线程
		if ( !mUtility.createThread( &mThread, GAudioHornUpdateFileServerRunLoop, this ) ) break;

		//设定结果flag
		mActionSeed++;
		result = mActionSeed;

	} while(0);

	return result;
}

//==========================================================================================================
//
//文件服务器
//
//==========================================================================================================
int GAudioHornUpdate::parseFileSize(AAsset *asset, FILE *file)
{
	int result = 0;

	do
	{
		if ( !asset && !file ) break;
		if ( asset )
		{
			result = AAsset_getLength( asset );
		}
		else
		{
            fseek( file, 0, SEEK_END);
            result = ftell( file );
            fseek( file, 0, SEEK_SET);
		}

	} while(0);

	return result;
}

int GAudioHornUpdate::copyDataFromFileToBuffer()
{
	int result = 0;
    if ( mIsAssetFile ) result = AAsset_read( mAAsset, mFileServerBuffer, 1024 );
    else 				result = fread( mFileServerBuffer, 1, 1024, mCustomFile );
	return result;
}

void GAudioHornUpdate::runLoop()
{
	bool result 	= false;
	int  resultStep = 0;

	//设定Running
	mUtility.lock( mLock );
	mRunning = true;
	mUtility.unlock( mLock );

	do
	{
		//初始化全局变量
		mAAsset 	= 0;
		mCustomFile = 0;

		//打开文件
		if ( mIsAssetFile )
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

		//解析文件大小
		int fileSize = parseFileSize( mAAsset, mCustomFile );
		if ( fileSize <= 0 ) break;

        //变量定义
		struct sockaddr_in 	sin;
		int 				socketID, fileID, clientID, readDataBytes;
		socklen_t 			acceptLength;

        //设置网络属性
        sin.sin_family 		= AF_INET;
        sin.sin_port 		= htons(8688);
        sin.sin_addr.s_addr = htonl(INADDR_ANY);

        //创建socket
        if ( ( socketID = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) break;

        //地址、端口重用
        int _reuseFlag = 1;
        setsockopt( socketID, SOL_SOCKET, SO_REUSEADDR, &_reuseFlag, sizeof(int) );
//        setsockopt(socketID, SOL_SOCKET, SO_REUSEPORT, &_reuseFlag, sizeof(int));

        //绑定socket
        if ( bind( socketID, (struct sockaddr *)(&sin), sizeof(sin) ) < 0 ) { close(socketID); break; }

        //开始监听指定端口
        if ( listen(socketID, 5) < 0 )  { close(socketID); break; }

        //通知板端准备更新音效
        GGameCommand::sharedInstance().updateSpeakerMusic( mUpdateTarget, mLocalIPAddress );

        //等待板端连接
        clientID = accept( socketID, (struct sockaddr *)(&sin), &acceptLength );

        //判断连接是否成功
        if (clientID < 0) { close(socketID); break; }
        resultStep = 1;

        //发送当前文件大小信息
        memset( mFileServerBuffer, 0				, 1024 		  );
        memcpy( mFileServerBuffer, &fileSize		, sizeof(int) );
        write ( clientID		 , mFileServerBuffer, sizeof(int) );

        //发送数据
        while ( ( readDataBytes = copyDataFromFileToBuffer() ) > 0 )
        {
            write( clientID, mFileServerBuffer, readDataBytes );
        }

        //关闭socket
        close(socketID);

		//设定结果flag
		result = true;

	} while(0);

	//关闭文件
	if ( mIsAssetFile ) AAsset_close( mAAsset );
	if ( mCustomFile  ) fclose( mCustomFile );
	mCustomFile	= 0;
	mAAsset 	= 0;

	//判断更新结果
	if ( !result && resultStep <= 0)
	{
		//内部出错
		int updateResult = 0;
		GGameClient::sharedInstance().swallowMessage(MUIM_M2C_SaveSpeakerMusicPCMDataResponse, &updateResult, sizeof(updateResult));
	}

	//设定Running
	mUtility.lock( mLock );
	mRunning = false;
	mUtility.unlock( mLock );
}
