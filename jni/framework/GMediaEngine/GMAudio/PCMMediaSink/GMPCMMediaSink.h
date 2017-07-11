#ifndef GMPCMMediaSink_h
#define GMPCMMediaSink_h

#include <iostream>
#include <memory>
#include <string>
#include "../../../live555/liveMedia/include/liveMedia.hh"
#include "../../../live555/BasicUsageEnvironment/include/BasicUsageEnvironment.hh"
#include "../../../live555/groupsock/include/GroupsockHelper.hh"
#include "../AudioPlayer/GMAudioPlayer.h"

#define GMPCMDataBufferSize     	2000
#define GMPCM_TIMESTAMP_ENABLE   	1

using namespace std;

class GMPCMMediaSink : public MediaSink
{
public:
    static GMPCMMediaSink *createNew(UsageEnvironment& env, void *delegate, void *callback);
    void pause(bool v);
    void stop();
    
private:
    GMPCMMediaSink(UsageEnvironment& env, void *delegate, void *callback);
    virtual ~GMPCMMediaSink();
    static void afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
                                  struct timeval presentationTime, unsigned durationInMicroseconds);
    void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime,
                           unsigned durationInMicroseconds);
    
private:
    virtual Boolean continuePlaying();
    
private:
    unsigned char   mDataBuffer[GMPCMDataBufferSize];
    short          *mDataBuffer16Ptr;
    unsigned        mDataBufferIndex;
    unsigned        mDataBuffer16Length;
    
    GMAudioPlayer   mAudioPlayer;
    bool            mStop;
    bool            mIsRunning;
    bool			mPausing;
    bool            mAlive;
    void           *mCallbackDelegate;
    void           *mStopCallback;

#if	GMPCM_TIMESTAMP_ENABLE
private:
    friend void         GMPCMGetNextFrameTask(void *data);
    void                getNextFrame();

private:
    struct timeval      mLastPresentationTime;
    unsigned            mPresentationTime0;
    unsigned            mPresentationTime1;
    bool                mFirstFrame;
    TaskToken           mGetNextFrameTask;
    UsageEnvironment    *mUsageEnvironment;
#endif
};

#endif
