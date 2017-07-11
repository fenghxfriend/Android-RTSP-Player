#include "GMAudioRecordSession.h"
#include "../RTSPServer/GMAudioRTSPServer.h"

GMAudioRecordSession* GMAudioRecordSession::createNew(UsageEnvironment& env)
{
    return new GMAudioRecordSession(env);
}

GMAudioRecordSession::GMAudioRecordSession(UsageEnvironment& env) : OnDemandServerMediaSubsession(env, true)
{
    mEstimatedKbps  = GMAudioServerSamplingFrequency * 16 * GMAudioServerChannels;
    mFrameSource    = NULL;
}

GMAudioRecordSession::~GMAudioRecordSession()
{
    if ( mFrameSource == NULL ) return;
//    Medium::close( mFrameSource );
}

FramedSource *GMAudioRecordSession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate)
{
    estBitrate = mEstimatedKbps;
//    if ( mFrameSource ) Medium::close( mFrameSource );
    mFrameSource = new GMAudioRecordFrame( envir() );
    return EndianSwap16::createNew(envir(), mFrameSource);
}

RTPSink* GMAudioRecordSession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource)
{
    return SimpleRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic, GMAudioServerSamplingFrequency, "audio", "L16", GMAudioServerChannels);
}


