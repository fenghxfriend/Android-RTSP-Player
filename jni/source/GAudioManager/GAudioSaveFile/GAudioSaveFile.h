#ifndef GAudioSaveFile_h
#define GAudioSaveFile_h

#include <jni.h>
#include <sys/types.h>
#include <memory>
#include <string>
#include "../GAudioUtility/GAudioUtility.h"
#include "../../GDataConverter/GDataConverter.h"

using namespace std;

class GAudioSaveFile
{
public:
	void 	install();
	void 	unInstall();
	int		saveFile(JNIEnv* env, jstring oldFilePath, jstring newFilePath);
	friend	void GAudioSaveFileRunLoop(void *data);

private:
	void runLoop();

private:
	bool			mRunning;
	char			mOldFilePath[256];
	char			mNewFilePath[256];
	GDataConverter	mConverter;
	GAudioUtility	mUtility;
	GAMULock		mLock;
	int				mActionSeed;
	char			mFileDateBuffer[1024];
	GAMUThread		mThread;
};

#endif
