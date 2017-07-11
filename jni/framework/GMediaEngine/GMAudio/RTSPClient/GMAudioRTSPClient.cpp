#include "GMAudioRTSPClient.h"
#include "../Controller/GMAudioController.h"
#include "../PCMMediaSink/GMPCMMediaSink.h"

#define GMAudio_RTSPCHKTEARDOWN 50000

void GMRTSPAudioContinueAfterDESCRIBE (class RTSPClient  *client, int resultCode, char *resultString);
void GMRTSPAudioContinueAfterSETUP    (class RTSPClient  *client, int resultCode, char *resultString);
void GMRTSPAudioContinueAfterPLAY     (class RTSPClient  *client, int resultCode, char *resultString);
void GMRTSPAudioSetupNextSession      (GMAudioRTSPClient *rtsp);
void GMRTSPAudioSubsessionAfterPlaying(void *clientData);
void GMRTSPAudioSubsessionAfterBye    (void *clientData);
void GMRTSPAudioTearDownTask          (void *clientData);
void GMRTSPAudioMediaSinkStopCall     (void *clientData);

GMAudioRTSPClient::GMAudioRTSPClient()
{
    init();
}

GMAudioRTSPClient::~GMAudioRTSPClient()
{
    destory();
}

void GMAudioRTSPClient::init()
{
    mConfiger.init();
}

void GMAudioRTSPClient::destory()
{
    mConfiger.init();
}

void GMAudioRTSPClient::playCallback()
{
    if ( mConfiger.m_didPlayCallback )
    {
        (mConfiger.m_didPlayCallback)( mConfiger.m_didCallbackData );
    }
}

void GMAudioRTSPClient::stopCallback()
{
    if ( mConfiger.m_didStopCallback)
    {
        (mConfiger.m_didStopCallback)( mConfiger.m_didCallbackData );
    }
}

void GMAudioRTSPClient::start(const char *url, GMRTSPDIDPLYFUN playcallback, GMRTSPDIDPLYFUN stopcallback, void *data)
{
    do
    {
        if ( url == 0               ) break;
        if ( mConfiger.isRunning()  ) break;
        if ( mConfiger.isStopping() ) break;

        mConfiger.init();

        mConfiger.m_URL                 = url;
        mConfiger.m_didPlayCallback     = playcallback;
        mConfiger.m_didStopCallback     = stopcallback;
        mConfiger.m_didCallbackData     = data;

        mConfiger.m_taskScheduler       = BasicTaskScheduler::createNew();
        if ( !mConfiger.m_taskScheduler     ) break;

        mConfiger.m_usageEnvironment    = BasicUsageEnvironment::createNew(*(mConfiger.m_taskScheduler));
        if ( !mConfiger.m_usageEnvironment  ) break;

        mConfiger.m_RTSPClient          = RTSPClient::createNew(*(mConfiger.m_usageEnvironment), mConfiger.m_URL.c_str(), 0, "GMAudioRTSPClient");
        if ( !mConfiger.m_RTSPClient        ) break;

        mConfiger.m_tearDownTask        = (mConfiger.m_usageEnvironment)->taskScheduler().scheduleDelayedTask(GMAudio_RTSPCHKTEARDOWN, GMRTSPAudioTearDownTask, this);

        mConfiger.setRunning(true);
        mConfiger.setStopping(false);

        ( mConfiger.m_RTSPClient )->sendDescribeCommand(GMRTSPAudioContinueAfterDESCRIBE, 0);

        ( mConfiger.m_usageEnvironment )->taskScheduler().doEventLoop(&(mConfiger.m_RTSPExit));

        ( mConfiger.m_usageEnvironment )->reclaim();

        mConfiger.setStopping(false);
        mConfiger.setRunning(false);

    } while (0);

    if ( mConfiger.m_taskScheduler ) delete mConfiger.m_taskScheduler;
    mConfiger.init();
}

void GMAudioRTSPClient::stop()
{
    if ( !mConfiger.isRunning() ) return;
    if ( mConfiger.isStopping() ) return;
    mConfiger.setStopping(true);
}

bool GMAudioRTSPClient::isRunning()
{
    return mConfiger.isRunning();
}

bool GMAudioRTSPClient::isStopping()
{
    return mConfiger.isStopping();
}

void GMRTSPAudioContinueAfterDESCRIBE(class RTSPClient *client, int resultCode, char *resultString)
{
    do
    {
        GMAudioRTSPClient*  rtsp        = GMAudioController::sharedInstance()->RTSPClientInstance();
        UsageEnvironment*   environment    = rtsp->mConfiger.m_usageEnvironment;
        MediaSession*       &mediaSession   = rtsp->mConfiger.m_mediaSession;

        if (resultCode != 0)
        {
            rtsp->stop();
            break;
        }

        mediaSession = MediaSession::createNew(*environment, resultString);
        delete[] resultString;

        if (mediaSession == NULL || !mediaSession->hasSubsessions())
        {
            rtsp->stop();
            break;
        }

        rtsp->mConfiger.m_mediaSubsessionIterator = new MediaSubsessionIterator(*mediaSession);
        GMRTSPAudioSetupNextSession(rtsp);

    } while (0);
}

void GMRTSPAudioSetupNextSession(GMAudioRTSPClient *rtsp)
{
    MediaSession*       &mediaSession       = rtsp->mConfiger.m_mediaSession;
    MediaSubsession*    &mediaSubsession    = rtsp->mConfiger.m_mediaSubsession;

    mediaSubsession                         = (rtsp->mConfiger.m_mediaSubsessionIterator)->next();
    if (mediaSubsession != NULL)
    {
        if (mediaSubsession->initiate())
        {
            (rtsp->mConfiger.m_RTSPClient)->sendSetupCommand(*mediaSubsession, GMRTSPAudioContinueAfterSETUP, false, false/*true*/, false);
        }
        else
        {
            GMRTSPAudioSetupNextSession(rtsp);
        }

        return;
    }

    (rtsp->mConfiger.m_RTSPClient)->sendPlayCommand(*mediaSession, GMRTSPAudioContinueAfterPLAY, 0.0f, -1.0f, 1.0f);
}

void GMRTSPAudioContinueAfterSETUP(class RTSPClient *client, int resultCode, char *resultString)
{
    GMAudioRTSPClient*  rtsp          = GMAudioController::sharedInstance()->RTSPClientInstance();
    UsageEnvironment*   environment       = rtsp->mConfiger.m_usageEnvironment;
    MediaSubsession*    &mediaSubsession   = rtsp->mConfiger.m_mediaSubsession;

    do
    {
        if ( resultCode != 0 ) break;

        mediaSubsession->sink = GMPCMMediaSink::createNew(*environment, rtsp, (void *)GMRTSPAudioMediaSinkStopCall);
        if (mediaSubsession->sink == NULL) break;

        rtsp->mConfiger.m_PCMMediaSink = (GMPCMMediaSink *)(mediaSubsession->sink);
        mediaSubsession->miscPtr = rtsp;

        mediaSubsession->sink->startPlaying(*(mediaSubsession->readSource()), GMRTSPAudioSubsessionAfterPlaying, mediaSubsession);

        if ( !mediaSubsession->rtcpInstance() ) break;

        mediaSubsession->rtcpInstance()->setByeHandler(GMRTSPAudioSubsessionAfterBye, mediaSubsession);

    } while (0);

    GMRTSPAudioSetupNextSession(rtsp);
}

void GMRTSPAudioContinueAfterPLAY(class RTSPClient *client, int resultCode, char *resultString)
{
    do
    {
        GMAudioRTSPClient* rtsp = GMAudioController::sharedInstance()->RTSPClientInstance();

        if (resultCode != 0)
        {
            rtsp->stop();
        }

        rtsp->playCallback();

    } while (0);
}

void GMRTSPAudioSubsessionAfterPlaying(void* clientData)
{
    MediaSubsession*    currentSubsession  = (MediaSubsession*)clientData;
    GMAudioRTSPClient   &RTSPRef           = *((GMAudioRTSPClient *)(currentSubsession->miscPtr));
    MediaSession*       &mediaSession      = RTSPRef.mConfiger.m_mediaSession;

    do
    {
        if ( currentSubsession->rtcpInstance() ) break;
        currentSubsession->rtcpInstance()->setByeHandler(NULL, NULL);
        Medium::close(currentSubsession->sink);
        currentSubsession->sink = NULL;

        bool allSubsessionClosed = true;
        MediaSubsession* subsession;
        MediaSubsessionIterator iter(*mediaSession);
        while ( (subsession = iter.next()) )
        {
            if (subsession->sink != NULL)
            {
                allSubsessionClosed = false;
                break;
            }
        }
        if (!allSubsessionClosed)
        {
            break;
        }

        RTSPRef.stop();

    } while (0);
}

void GMRTSPAudioSubsessionAfterBye(void *clientData)
{
    GMRTSPAudioSubsessionAfterPlaying(clientData);
}

void GMRTSPAudioTearDownTask(void *clientData)
{
    GMAudioRTSPClient* rtsp = (GMAudioRTSPClient *)clientData;

    if ( !rtsp->mConfiger.isStopping() )
    {
        (rtsp->mConfiger.m_usageEnvironment)->taskScheduler().rescheduleDelayedTask(rtsp->mConfiger.m_tearDownTask, GMAudio_RTSPCHKTEARDOWN, GMRTSPAudioTearDownTask, clientData);
    }
    else
    {
        rtsp->closeMediaSink();
    }
}

void GMAudioRTSPClient::closeMediaSink()
{
    if ( mConfiger.m_PCMMediaSink != NULL )
    {
        (mConfiger.m_PCMMediaSink)->stop();
    }
    else
    {
        closeStream();
    }
}

void GMRTSPAudioMediaSinkStopCall(void *clientData)
{
    GMAudioRTSPClient* rtsp = (GMAudioRTSPClient *)clientData;
    rtsp->closeStream();
}

void GMAudioRTSPClient::closeStream()
{
    if (mConfiger.m_tearDownTask)
    {
        (mConfiger.m_usageEnvironment)->taskScheduler().unscheduleDelayedTask(mConfiger.m_tearDownTask);
    }

    bool haveLiveSubsession = false;
    if (mConfiger.m_mediaSession != NULL)
    {
        MediaSubsession* subsession;
        MediaSubsessionIterator iter(*(mConfiger.m_mediaSession));
        while ((subsession = iter.next()) != NULL)
        {
            if (subsession->sink != NULL)
            {
                if (subsession->rtcpInstance() != NULL)
                {
                    subsession->rtcpInstance()->setByeHandler(NULL, NULL);
                }
                Medium::close(subsession->sink);
                subsession->sink = NULL;

                haveLiveSubsession = true;
            }
        }

        if (haveLiveSubsession)
        {
            (mConfiger.m_RTSPClient)->sendTeardownCommand(*(mConfiger.m_mediaSession), NULL, NULL);
        }
    }

    if (mConfiger.m_mediaSubsessionIterator != NULL)
    {
        delete mConfiger.m_mediaSubsessionIterator;
    }

    if (mConfiger.m_mediaSession != NULL)
    {
        Medium::close(mConfiger.m_mediaSession);
    }

    Medium::close(mConfiger.m_RTSPClient);
    mConfiger.m_RTSPExit = 1;

    stopCallback();
}

