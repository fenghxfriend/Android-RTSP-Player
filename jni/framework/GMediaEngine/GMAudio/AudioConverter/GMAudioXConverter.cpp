#include <pthread.h>
#include <fcntl.h>
#include "GMAudioXConverter.h"
#define GMAudioXConverterFrameBytes 2

GMAudioXConverter::GMAudioXConverter() { init(); }
GMAudioXConverter::~GMAudioXConverter() { destory(); }

void GMAudioXConverter::init()
{
	mFile 				= 0;
	mFileSize			= 0;
	mFileRemainSize		= 0;
	mActiveReadSize		= 0;
	mActiveReadedSize	= 0;
	mLoaded				= false;
}

void GMAudioXConverter::destory()
{
	if ( mFile ) fclose( mFile );
	init();
}

int  GMAudioXConverter::fileSize()
{
	int result = 0;

	do
	{
		if ( !mFile ) break;
        fseek( mFile, 0, SEEK_END);
        result = ftell( mFile );
        fseek( mFile, 0, SEEK_SET);

	} while(0);

	return result;
}

bool   GMAudioXConverter::loadFile(const char *path)
{
	bool result = false;
	mLoaded = false;

	do
	{
	    if ( !path ) break;
	    if ( access(path, 0) != 0 ) break;

		//打开文件
	    mFile = fopen( path, "rb+" );
		if ( !mFile ) break;

		//解析文件大小
		mFileSize = fileSize();
		mFileRemainSize = mFileSize;
		if ( mFileSize <= 0 ) break;

		//设定结果flag
		mLoaded = true;
		result  = true;

	} while( 0 );

	if ( !result && mFile )
	{
		fclose( mFile );
		mFile = 0;
	}

	return result;
}

void   GMAudioXConverter::readData(void *buffer, unsigned &actualframes, unsigned maxframes)
{
	do
	{
		if ( !mLoaded || !buffer || maxframes <= 0 || mFileRemainSize <= 0 ) { actualframes = 0; break; }

		//计算本次读取的字节数
		mActiveReadSize = ( maxframes * GMAudioXConverterFrameBytes );
		mActiveReadSize = ( ( mActiveReadSize > mFileRemainSize ) ? mFileRemainSize : mActiveReadSize );

		//读取数据
		mActiveReadedSize = fread( buffer, 1, mActiveReadSize, mFile );
		if ( mActiveReadedSize <= 0 ) mActiveReadedSize = 0;

		//计算读取出来的帧数
		actualframes = ( mActiveReadedSize / GMAudioXConverterFrameBytes );

		//更新剩余数据量
		mFileRemainSize -= mActiveReadedSize;

	} while( 0 );
}

void   GMAudioXConverter::seek(double time)
{
	if ( !mLoaded ) return;

	//计算偏移量
	int offsetsize = ( int )( 44100.0 /* 44100.0 = 22050.0 * 2.0 */ * time );

	//重定位文件
	fseek( mFile, offsetsize, SEEK_SET );

	//重设剩余字节数
	mFileRemainSize = mFileSize - offsetsize;
	mFileRemainSize = ( ( mFileRemainSize < 0 ) ? 0 : mFileRemainSize );
}

double GMAudioXConverter::duration()
{
	return ( ( ( double )( mFileSize * 1.0 ) ) / ( 44100.0 /* 44100.0 = 22050.0 * 2.0 */ ) - 0.0 );
}

bool   GMAudioXConverter::isEOF()
{
	return ( mFileRemainSize <= 0 );
}

float  GMAudioXConverter::mediaDuration(char const* fileName)
{
	float result = 0.0;

	do
	{
	    if ( !fileName ) break;
	    if ( access(fileName, 0) != 0 ) break;

	    FILE *_file = fopen( fileName, "rb+" );
	    if ( !_file ) break;
        fseek( _file, 0, SEEK_END);
        result = ( ftell( _file ) / ( 44100.0 /* 44100.0 = 22050.0 * 2.0 */ ) - 0.0 );
        fclose( _file );

	} while(0);

	return result;
}
