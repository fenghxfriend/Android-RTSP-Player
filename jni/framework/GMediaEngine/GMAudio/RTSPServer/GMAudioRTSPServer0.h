#ifndef GMAudioRTSPServer0_h
#define GMAudioRTSPServer0_h

#include <iostream>
#include <memory>
#include <string>
#include "RTSPServer.hh"

using namespace std;

class GMAudioRecordSession;
class GMAudioFileSession;
class GMAudioRTSPServer0 : public RTSPServer
{
public:
    static GMAudioRTSPServer0* createNew(UsageEnvironment& env, Port ourport, UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds = 0);
    friend class GMAudioRTSPServer;
private:
    GMAudioRTSPServer0(UsageEnvironment& env, int ourSocket, Port ourport, UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds);
    virtual ~GMAudioRTSPServer0();
    virtual ServerMediaSession* lookupServerMediaSession(char const* streamName);

private:
    GMAudioRecordSession *mAudioRecordSession;
    GMAudioFileSession   *mAudioFileSession;
};

#endif
