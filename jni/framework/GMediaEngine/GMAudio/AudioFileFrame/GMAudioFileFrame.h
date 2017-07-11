#ifndef GMAudioFileFrame_h
#define GMAudioFileFrame_h

#include <iostream>
#include <memory>
#include <string>
#include "../../../live555/liveMedia/include/liveMedia.hh"
#include "../../../live555/groupsock/include/GroupsockHelper.hh"
#include "../../../live555/liveMedia/include/AudioInputDevice.hh"
#include "../../../live555/BasicUsageEnvironment/include/BasicUsageEnvironment.hh"
#include "../AudioConverter/GMAudioConverter.h"
#include "../AudioConverter/GMAudioXConverter.h"

using namespace std;

class GMAudioFileFrame : public AudioInputDevice
{
public:
    static GMAudioFileFrame* createNew(UsageEnvironment& env, char const* fileName, bool isSystemMusic);
    static float parseAudioFileDuration(char const* fileName, bool isSystemMusic);
    
public:
    unsigned        numPCMBytes() const;
    void            setScaleFactor(int scale);
    void            seekToDuration(double seekNPT, double streamDuration);
    double          duration();
    unsigned char   getAudioFormat();
    
protected:
    GMAudioFileFrame(UsageEnvironment& env, char const* fileName, bool isSystemMusic);
    virtual ~GMAudioFileFrame();
    
private:
    virtual void    doGetNextFrame();
    virtual Boolean setInputPort(int portIndex);
    virtual double  getAverageLevel() const;
    
private:
    GMAudioConverter    mAudioConverter;
    GMAudioXConverter	mAudioXConverter;
    unsigned char       mAudioFormat;
    double              mScaleFactor;
    unsigned int        mSendMaxNumber;
    unsigned int        mNetMaxNumber;
    unsigned int        mFrameBytes;
    double              mSampleingPeriod;
    unsigned            mActiveFrameDuration;
    unsigned            mFrameDurationCount;
    bool				mSystemMusic;
};

#endif
