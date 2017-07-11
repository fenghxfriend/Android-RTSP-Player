#ifndef GAudioStreamPlayer_h
#define GAudioStreamPlayer_h

#include <jni.h>
#include <sys/types.h>
#include <memory>
#include <string>
#include "GAudioStreamPlayerQueue.h"
#include "../GAudioUtility/GAudioUtility.h"

using namespace std;

class GAudioStreamPlayer
{
public:
	void install();
	void unInstall();
    void start();
    void write(void *data, int size);
    void stop();
    void setRTSPIPAddress(const char *ip);

public:
    friend 	void GAudioStreamPlayerCallback( void *buffer, int *size, int maxsize, void *data );
	friend 	void GAudioStreamPlayerStartCallback(void *data);
	friend 	void GAudioStreamPlayerStopCallback(void *data);

private:
	void	playerCallback(void *buffer, int *size, int maxsize);
	void	RTSPStart();
	void	RTSPStop();

private:
	bool					mRunning;
	bool					mSleeping;
	GAudioStreamPlayerQueue	mAudioQueue;
	char					mRTSPAddress[100];
	GAudioUtility			mUtility;
	GAMULock				mLock;
};

#endif

