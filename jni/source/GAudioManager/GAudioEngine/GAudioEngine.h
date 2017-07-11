#ifndef GAudioEngine_h
#define GAudioEngine_h

#include <jni.h>
#include <memory>
#include <string>
#include "GAudioIOEngine/GAudioIOEngine.h"
#include "GAudioPlayer/GAudioPlayer.h"
#include "GAudioRecorder/GAudioRecorder.h"

using namespace std;

class GAudioEngine
{
public:
    static 	GAudioEngine &sharedInstance();
	void   	install();
	void   	unInstall();
	bool   	startPlayback(GAUDIOPLAYERFUN callback, void *data);
	bool   	stopPlayback();
	void 	sleepPlayer();
	void 	wakePlayer();
	bool 	startRecord();
	bool 	stopRecord();
	int	 	readRecordedData(void *buffer, int size);

public:
	void 	install0();
	void 	unInstall0();

private:
	GAudioIOEngine 	mEngine;
	GAudioPlayer	mPlayer;
	GAudioRecorder	mRecorder;
};

#endif
