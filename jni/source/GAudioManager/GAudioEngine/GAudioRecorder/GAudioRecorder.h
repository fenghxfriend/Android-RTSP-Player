#ifndef GAudioRecorder_h
#define GAudioRecorder_h

#include <jni.h>
#include <memory>
#include <string>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "../../GAudioUtility/GAudioUtility.h"
#include "GAudioRecorderQueue.h"

using namespace std;

#define	GAUDIORECORDERBUFFERSIZE	4410 // ( ( 22050 * ( 16 / 8 ) * 1 ) / 10 )  100毫秒为一个周期

class GAudioRecorder
{
public:
	void install(SLEngineItf engine);
	void unInstall();
	bool start();
	bool stop();
	int	 read(void *buffer, int size);
	friend void GAudioRecorderQueueCallback(SLAndroidSimpleBufferQueueItf queue, void *data);

private:
	void createRecorder(SLEngineItf engine);
	void destoryRecorder();
	void audioQueueCallback(SLAndroidSimpleBufferQueueItf queue);

private:
	SLObjectItf 					mRecorderObject;
	SLRecordItf 					mRecorderInterface;
	SLAndroidSimpleBufferQueueItf 	mRecorderBufferQueue;
	char							mAudioQueueBuffer[GAUDIORECORDERBUFFERSIZE];
	GAudioUtility					mUtility;
	GAMULock						mLock;
	GAudioRecorderQueue				mAudioQueue;
	bool							mRunning;
	bool							mInstalled;
};

#endif
