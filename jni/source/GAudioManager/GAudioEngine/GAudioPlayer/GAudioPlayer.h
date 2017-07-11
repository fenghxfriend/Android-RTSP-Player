#ifndef GAudioPlayer_h
#define GAudioPlayer_h

#include <jni.h>
#include <memory>
#include <string>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

using namespace std;

typedef void ( *GAUDIOPLAYERFUN )( void *buffer, int *size, int maxsize, void *data );
#define GAUDIOPLAYERBUFFERSIZE	256

class GAudioPlayer
{
public:
	void install(SLEngineItf engine, SLObjectItf mixoutput);
	void unInstall();
	bool start(GAUDIOPLAYERFUN callback, void *data);
	void sleep();
	void wake();
	bool stop();
	friend void GAudioPlayerQueueCallback(SLAndroidSimpleBufferQueueItf queue, void *data);

private:
	void createPlayer(SLEngineItf engine, SLObjectItf mixoutput);
	void destoryPlayer();
	void audioQueueCallback(SLAndroidSimpleBufferQueueItf queue);

private:
	SLObjectItf 				  	mPlayerObject;
	SLPlayItf   					mPlayerInterface;
	SLAndroidSimpleBufferQueueItf 	mPlayerBufferQueue;
	bool							mRunning;
	char							mAudioQueueBuffer[GAUDIOPLAYERBUFFERSIZE];
	int								mAudioQueueDataSize;
	GAUDIOPLAYERFUN					mAudioQueueCallback;
	void							*mAudioQueueCallbackData;
	bool							mSleeping;
};

#endif
