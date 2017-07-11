#ifndef GMVideoRTSPClient_h
#define GMVideoRTSPClient_h

#include <iostream>
#include <memory>
#include <string>
#include "../../../live555/liveMedia/include/liveMedia.hh"
#include "../../../live555/BasicUsageEnvironment/include/BasicUsageEnvironment.hh"
#include "../../../live555/groupsock/include/GroupsockHelper.hh"
#include "../H264MediaSink/GMH264MediaSink.h"
#include "../../GMAudio/PCMMediaSink/GMPCMMediaSink.h"
#include "../../GMUtility/GMUtility.h"
typedef enum {
    REASON_NONE,
    REASON_USER,
    REASON_LIVE555BYE,
    REASON_UNKNOWN,
    REASON_MAX
}EMShutdown;
using namespace std;
extern char g_exitCode;
extern void shutdown();
class GMVideoRTSPClient: public RTSPClient
{

public:
    static GMVideoRTSPClient* createNew(GMFUN_555_EVENT on555Event
		, void *delegate
		, UsageEnvironment& env
        , char const* rtspURL
        , const char *username = ""
        , const char * password = ""
        , int verbosityLevel = 0
        , char const* applicationName = NULL
        , portNumBits tunnelOverHTTPPortNum = 0
        , int socketNumToServer = -1);

    void start();
    void stop();

protected:
    GMVideoRTSPClient(GMFUN_555_EVENT on555Event
		, void *delegate
		, UsageEnvironment& env
        , char const* rtspURL
        , const char *username = ""
        , const char * password = ""
        , int verbosityLevel = 0
        , char const* applicationName = NULL
        , portNumBits tunnelOverHTTPPortNum = 0
        , int socketNumToServer = -1);
    virtual ~GMVideoRTSPClient();




private:
    friend void subsessionAfterPlaying(void* clientData);
    friend void subsessionByeHandler(void* clientData);
    friend void sessionAfterPlaying(void* clientData);
    friend void sessionTimerHandler(void* clientData);
    friend void checkSessionTimeoutBrokenServer(void* clientData);

    friend void continueAfterOPTIONS(RTSPClient*, int resultCode, char* resultString);
    friend void continueAfterPLAY(RTSPClient* clientData, int resultCode, char* resultString);
    friend void continueAfterSETUP(RTSPClient* clientData, int resultCode, char* resultString);
    friend void continueAfterDESCRIBE(RTSPClient* clientData, int resultCode, char* resultString);
    friend void continueAfterOPTIONS(RTSPClient*, int resultCode, char* resultString);
    static void *ThreadWrapper(void *pThis);
    int ThreadEntry();

private:
    void init();
    void destroy();
    void createRec();
    void setupStreams();
    void shutdowm();


private:
    GMFUN_555_EVENT     	 m_On555Callback;
    void                    *m_OnDelegate;

    bool                     m_bPlayContinuously;
    Authenticator           *m_authenticator;

    bool                     m_bMadeProgress;
    int                      m_sessionTimeoutParameter;

    MediaSession            *m_session;
    MediaSubsession         *m_subsession;
    MediaSubsessionIterator *m_subsessionIter;

    TaskToken                m_sessionTimerTask;
    double                   m_duration;
    bool		             m_bKeepAlive;
    TaskToken 		         m_sessionTimeoutBrokenServerTask;
    bool                     m_bAlreadyShuttingDown;
    bool                     m_bHeartBeat;

};

#endif
