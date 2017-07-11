#ifndef GMAudioFileSession_h
#define GMAudioFileSession_h

#include <iostream>
#include <memory>
#include <string>
#include "../../../live555/liveMedia/include/liveMedia.hh"
#include "../../../live555/BasicUsageEnvironment/include/BasicUsageEnvironment.hh"
#include "../../../live555/groupsock/include/GroupsockHelper.hh"
#include "../../../live555/liveMedia/include/OnDemandServerMediaSubsession.hh"
#include "../../../live555/liveMedia/include/FileServerMediaSubsession.hh"

using namespace std;

class GMAudioFileSession : public FileServerMediaSubsession
{
public:
    static GMAudioFileSession *createNew(UsageEnvironment& env, char const* fileName, bool isSystemMusic, Boolean reuseFirstSource, Boolean convertToULaw = False);
    bool parseAudioFileDuration(char const* fileName, bool isSystemMusic);

protected:
    GMAudioFileSession(UsageEnvironment& env, char const* fileName, Boolean reuseFirstSource, Boolean convertToULaw, bool isSystemMusic);
    virtual ~GMAudioFileSession();
    
protected:
    virtual void seekStreamSource(FramedSource* inputSource, double seekNPT, double streamDuration);
    virtual void setStreamSourceScale(FramedSource* inputSource, float scale);
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
    virtual void closeStreamSource(FramedSource *inputSource);
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);
    virtual void testScaleFactor(float& scale);
    virtual float duration() const;

protected:
    Boolean         fConvertToULaw;
    unsigned char   fAudioFormat;
    unsigned char   fBitsPerSample;
    unsigned        fSamplingFrequency;
    unsigned        fNumChannels;
    float           fFileDuration;
    bool			mIsSystemMusic;
};

#endif
