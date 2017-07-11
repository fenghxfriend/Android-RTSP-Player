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
#include "GAudioSaveFile.h"
#include "../../../framework/GEngineEvent/GEngineEvent.h"

void GAudioSaveFileRunLoop(void *data) { if ( data ) ( ( GAudioSaveFile * )data )->runLoop(); }

void GAudioSaveFile::install()
{
	//初始化全局变量
	mRunning 	= false;
	mActionSeed	= 0;
	mThread		= 0;
	memset( mOldFilePath	, 0, 256  );
	memset( mNewFilePath	, 0, 256  );
	memset( mFileDateBuffer	, 0, 1024 );

	//创建锁
	mLock = mUtility.createLock();
}

void GAudioSaveFile::unInstall()
{
	//判断当前是否正在工作
	bool isRunning = false;
	mUtility.lock( mLock );
	isRunning = mRunning;
	mUtility.unlock( mLock );

	//等待工作结束
	if ( isRunning ) mUtility.waitingThread( mThread );

	//销毁锁
	mUtility.destorylock( &mLock );

	//初始化全局变量
	mRunning 	= false;
	mActionSeed	= 0;
	mThread		= 0;
	memset( mOldFilePath	, 0, 256  );
	memset( mNewFilePath	, 0, 256  );
	memset( mFileDateBuffer	, 0, 1024 );
}

int GAudioSaveFile::saveFile(JNIEnv* env, jstring oldFilePath, jstring newFilePath)
{
	int result = 0;

	do
	{
		//判断参数是否有效
		if ( !env || !oldFilePath || !newFilePath ) break;

		//判断当前是否正在工作
		bool isRunning = false;
		mUtility.lock( mLock );
		isRunning = mRunning;
		mUtility.unlock( mLock );
		if ( isRunning ) break;

		//解析文件路径信息
		memset( mOldFilePath, 0, 256 );
		memset( mNewFilePath, 0, 256 );
		int length0 = mConverter.getStringBytes( env, oldFilePath, mOldFilePath, 256 );
		int length1 = mConverter.getStringBytes( env, newFilePath, mNewFilePath, 256 );
		if ( length0 <= 0 || length1 <= 0 ) break;

		//启动线程
		if ( !mUtility.createThread( &mThread, GAudioSaveFileRunLoop, this ) ) break;

		//设定结果flag
		mActionSeed++;
		result = mActionSeed;

	} while(0);

	return result;
}

void GAudioSaveFile::runLoop()
{
	//设定Running
	mUtility.lock( mLock );
	mRunning = true;
	mUtility.unlock( mLock );

	bool result 	= false;
	FILE *oldFile 	= NULL;
	FILE *newFile 	= NULL;

	do
	{
		//判断文件是否存在
		if ( access( mOldFilePath, 0 ) != 0 ) break;

		//删除新文件
		if ( access( mNewFilePath, 0 ) == 0 ) { if ( remove( mNewFilePath ) != 0 ) break; }

		//打开文件
		oldFile = fopen( mOldFilePath, "rb+" );
		newFile = fopen( mNewFilePath, "w+" );

		//判断文件是否打开成功
		if ( !oldFile || !newFile ) break;

		//循环拷贝文件内容
		int  copyOutSize = 0;
		int  copyInSize  = 0;
		bool copyResult  = false;
		do
		{
			//从旧文件中拷贝数据至缓冲区
			copyOutSize = fread( mFileDateBuffer, 1, 1024, oldFile );
			if ( copyOutSize <= 0 ) { copyResult = true; break; }

			//从缓冲区中拷贝数据至新文件
			copyInSize = fwrite( mFileDateBuffer, 1, copyOutSize, newFile );
			if ( copyInSize != copyOutSize ) break;

		} while( true );

		//设定结果flag
		result = copyResult;

	} while(0);

	//关闭文件
	if ( oldFile ) fclose( oldFile );
	if ( newFile ) fclose( newFile );
	oldFile = 0;
	newFile = 0;

	//强制等待
	usleep( 1000 * 1000 );

	//生成事件
	int action = mActionSeed;
	GEngineEvent::sharedInstance().swallowEvent( EET_AUDIO_FILE_SAVE_FINISH, ( void * )( &action ), result ? 1 : 0 );

	//设定Running
	mUtility.lock( mLock );
	mRunning = false;
	mUtility.unlock( mLock );
}


