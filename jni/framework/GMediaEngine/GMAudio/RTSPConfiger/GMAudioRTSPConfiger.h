#ifndef GMAudioRTSPConfiger_h
#define GMAudioRTSPConfiger_h

#include <iostream>
#include <memory>
#include <string>
#include "../../../live555/liveMedia/include/liveMedia.hh"
#include "../../../live555/BasicUsageEnvironment/include/BasicUsageEnvironment.hh"
#include "../../../live555/groupsock/include/GroupsockHelper.hh"
#include "../PCMMediaSink/GMPCMMediaSink.h"
#include "../../GMUtility/GMUtility.h"

using namespace std;

typedef void (*GMRTSPDIDPLYFUN)(void *);
class GMAudioRTSPClient;
class GMAudioRTSPConfiger
{
public:
    void init();
    bool isStopping();
    void setStopping(bool value);
    bool isRunning();
    void setRunning(bool value);
    GMAudioRTSPConfiger();
    ~GMAudioRTSPConfiger();

/*
public:
    friend class GMAudioRTSPClient;
    
private:
*/

public:
    string                  m_URL;
    TaskScheduler           *m_taskScheduler;
	UsageEnvironment        *m_usageEnvironment;
	RTSPClient              *m_RTSPClient;
    MediaSession            *m_mediaSession;
    MediaSubsession         *m_mediaSubsession;
    MediaSubsessionIterator *m_mediaSubsessionIterator;
    GMPCMMediaSink          *m_PCMMediaSink;
    GMRTSPDIDPLYFUN         m_didPlayCallback;
    GMRTSPDIDPLYFUN         m_didStopCallback;
    void					*m_didCallbackData;
    TaskToken               m_tearDownTask;
    char                    m_RTSPExit;
    bool                    m_tearDown;
    bool                    m_stopping;
    bool                    m_running;
    
private:
    GMUtility               mUtility;
    GMLock                  mSourceLock;
};

#endif
