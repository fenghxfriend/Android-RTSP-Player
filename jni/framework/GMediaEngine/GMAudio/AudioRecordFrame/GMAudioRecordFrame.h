#ifndef GMAudioRecordFrame_h
#define GMAudioRecordFrame_h

#include <iostream>
#include <memory>
#include <string>
#include "../../../live555/liveMedia/include/liveMedia.hh"
#include "../../../live555/BasicUsageEnvironment/include/BasicUsageEnvironment.hh"
#include "../../../live555/groupsock/include/GroupsockHelper.hh"
#include "../AudioRecorder/GMAudioRecorder.h"

using namespace std;

class GMAudioRecordFrame : public FramedSource
{
public:
    GMAudioRecordFrame(UsageEnvironment& env);
    virtual ~GMAudioRecordFrame();
    
protected:
    virtual void doGetNextFrame();
    
private:
    GMAudioRecorder 	mAudioRecorder;
    unsigned int        mSendMaxNumber;
    unsigned int        mNetMaxNumber;
    unsigned int        mFrameBytes;
    double              mSampleingPeriod;
    unsigned            mActiveFrameDuration;
    unsigned            mFrameDurationCount;
    int					mActualReadSize;
};

#endif
