#ifndef GAudioDeleteFile_h
#define GAudioDeleteFile_h

#include <jni.h>
#include <memory>
#include <string>
#include "../../GDataConverter/GDataConverter.h"

using namespace std;

class GAudioDeleteFile
{
public:
	void install();
	void unInstall();
	int  deleteFile(JNIEnv* env, jstring path);

private:
	GDataConverter 	mConverter;
	char			mFilePath[256];
	int				mFilePathLength;
};

#endif
