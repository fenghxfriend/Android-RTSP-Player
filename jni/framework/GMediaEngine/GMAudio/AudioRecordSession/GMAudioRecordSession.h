#ifndef GMAudioRecordSession_h
#define GMAudioRecordSession_h

#include <iostream>
#include <memory>
#include <string>
#include "../../../live555/liveMedia/include/liveMedia.hh"
#include "../../../live555/BasicUsageEnvironment/include/BasicUsageEnvironment.hh"
#include "../../../live555/groupsock/include/GroupsockHelper.hh"
#include "../../../live555/liveMedia/include/OnDemandServerMediaSubsession.hh"
#include "../../../live555/liveMedia/include/FileServerMediaSubsession.hh"
#include "../AudioRecordFrame/GMAudioRecordFrame.h"

using namespace std;

class GMAudioRecordSession : public OnDemandServerMediaSubsession
{
public:
    static GMAudioRecordSession* createNew(UsageEnvironment& env);
    
private:
    GMAudioRecordSession(UsageEnvironment& env);
    virtual ~GMAudioRecordSession();
    
protected:
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);

private:
    unsigned            mEstimatedKbps;
    GMAudioRecordFrame  *mFrameSource;
};

#endif
