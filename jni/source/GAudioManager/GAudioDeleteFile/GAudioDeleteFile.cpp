#include <stdio.h>
#include <fcntl.h>
#include "GAudioDeleteFile.h"

void GAudioDeleteFile::install  () { };
void GAudioDeleteFile::unInstall() { };

int GAudioDeleteFile::deleteFile(JNIEnv* env, jstring path)
{
	int result = -1;

	do
	{
		if ( !env || !path ) break;

		//取得文件全路径信息
		memset( mFilePath, 0, 256 );
		mFilePathLength = mConverter.getStringBytes( env, path, mFilePath, 256 );
		if ( mFilePathLength <= 0 ) break;

		//判断文件是否存在
		if ( access( mFilePath, 0 ) != 0 ) break;

		//删除文件
		if ( remove( mFilePath ) == 0 ) result = 0;

	} while(0);

	return result;
}
