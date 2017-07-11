#ifndef GAudioFileRecorder_h
#define GAudioFileRecorder_h

#include <jni.h>
#include <sys/types.h>
#include <memory>
#include <string>
#include "../GAudioUtility/GAudioUtility.h"
#include "../../GDataConverter/GDataConverter.h"

using namespace std;

class GAudioFileRecorder
{
public:
	void 	install();
	void 	unInstall();
	int		start(JNIEnv* env, jstring path);
	void	stop();
	friend	void GAudioFileRecorderRunLoop(void *data);

private:
	void runLoop();

private:
	GDataConverter	mConverter;
	GAudioUtility	mUtility;
	GAMULock		mLock;
	int				mActionSeed;
	bool			mRunning;
	char			mAudioQueueBuffer[256];
	char			mFilePath[256];
	GAMUThread		mThread;
};

#endif
