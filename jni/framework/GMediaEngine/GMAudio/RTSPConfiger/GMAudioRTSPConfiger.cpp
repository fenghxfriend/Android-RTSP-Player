#include "GMAudioRTSPConfiger.h"

void GMAudioRTSPConfiger::init()
{
    m_taskScheduler             = NULL;
    m_usageEnvironment          = NULL;
    m_RTSPClient                = NULL;
    m_mediaSession              = NULL;
    m_mediaSubsession           = NULL;
    m_mediaSubsessionIterator   = NULL;
    m_PCMMediaSink              = NULL;
    m_didPlayCallback           = NULL;
    m_didStopCallback           = NULL;
    m_didCallbackData			= NULL;
    m_tearDownTask              = NULL;
    m_tearDown                  = false;
    m_RTSPExit                  = 0;
    
    mUtility.lock(mSourceLock);
    m_stopping                  = false;
    m_running                   = false;
    mUtility.unlock(mSourceLock);
}

bool GMAudioRTSPConfiger::isStopping()
{
    bool result = false;
    mUtility.lock(mSourceLock);
    result = m_stopping;
    mUtility.unlock(mSourceLock);
    return result;
}

void GMAudioRTSPConfiger::setStopping(bool value)
{
    mUtility.lock(mSourceLock);
    m_stopping = value;
    mUtility.unlock(mSourceLock);
}

bool GMAudioRTSPConfiger::isRunning()
{
    bool result = false;
    mUtility.lock(mSourceLock);
    result = m_running;
    mUtility.unlock(mSourceLock);
    return result;
}

void GMAudioRTSPConfiger::setRunning(bool value)
{
    mUtility.lock(mSourceLock);
    m_running = value;
    mUtility.unlock(mSourceLock);
}

GMAudioRTSPConfiger::GMAudioRTSPConfiger()
{
    mSourceLock = mUtility.createLock();
    init();
}

GMAudioRTSPConfiger::~GMAudioRTSPConfiger()
{
    init();
    mUtility.destorylock(&mSourceLock);
}
