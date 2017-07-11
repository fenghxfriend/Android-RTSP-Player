#include "../../../live555/liveMedia/include/liveMedia.hh"
#include "../../../live555/BasicUsageEnvironment/include/BasicUsageEnvironment.hh"
#include "../../../live555/groupsock/include/GroupsockHelper.hh"
#include "../../../live555/liveMedia/include/OnDemandServerMediaSubsession.hh"
#include "../../../live555/liveMedia/include/FileServerMediaSubsession.hh"
#include "GMAudioRTSPServer.h"
#include "GMAudioRTSPServer0.h"
#include "../AudioFileSession/GMAudioFileSession.h"
#define GMAudioRTSPServer_StopScheduleInterval  50000
void GMAudioRTSPServer_stopServer   (void *data)
{
    if ( data == NULL ) return;
    ( ( GMAudioRTSPServer * )data )->stopServer(data);
}

void* GMAudioRTSPServer_serverRunLoop(void *data)
{
    if ( data == NULL ) return NULL;
    ( ( GMAudioRTSPServer * )data )->serverRunLoop();
	return NULL;
}

GMAudioRTSPServer& GMAudioRTSPServer::sharedInstance()
{
    static GMAudioRTSPServer instance;
    return instance;
}

void GMAudioRTSPServer::install()
{
    mUsageEnvironment   = NULL;
    mRTSPServer         = NULL;
    mStopTaskToken      = NULL;
    mServerPort         = 0;
    mServerExit         = 0;
    mSourceLock         = mUtility.createLock();
    setRunning (false);
    setStopping(false);
}

bool GMAudioRTSPServer::start()
{
    bool result = false;
    do
    {
        if ( isRunning()  ) break;
        if ( isStopping() ) break;

        GMThread taskHandle;
        if ( !mUtility.createThread(&taskHandle, GMAudioRTSPServer_serverRunLoop, this) ) break;
        result = true;

    } while (0);
    return result;
}

void GMAudioRTSPServer::serverRunLoop()
{
    do
    {
        if ( isRunning() ) break;

        TaskScheduler    *_scheduler    = BasicTaskScheduler::createNew();
        if ( _scheduler == NULL ) break;

        mUsageEnvironment               = BasicUsageEnvironment::createNew( *_scheduler );
        if ( mUsageEnvironment == NULL ) break;

        portNumBits      _portList[]    = { 18126, 18162, 18621, 18612, 11826, 11862, 11682, 11628, 11268, 11286 };
        unsigned         _portListCount = sizeof(_portList) / sizeof(portNumBits);
        for (unsigned i = 0; i < _portListCount; ++i)
        {
            mRTSPServer = GMAudioRTSPServer0::createNew(*mUsageEnvironment, Port(_portList[i]), NULL);
            if ( mRTSPServer != NULL )
            {
                mServerPort = _portList[i];
                break;
            }
        }
        if ( mRTSPServer == NULL ) break;

        mStopTaskToken = mUsageEnvironment->taskScheduler().scheduleDelayedTask( GMAudioRTSPServer_StopScheduleInterval, GMAudioRTSPServer_stopServer, this);

        setRunning (true);
        setStopping(false);

        mServerExit = 0;
        mUsageEnvironment->taskScheduler().doEventLoop( &mServerExit );
        mUsageEnvironment->reclaim();
        delete _scheduler;

    } while (0);

    setRunning (false);
    setStopping(false);
}

void GMAudioRTSPServer::stopServer(void *data)
{
    if ( !isStopping() )
    {
        mUsageEnvironment->taskScheduler().rescheduleDelayedTask(mStopTaskToken, GMAudioRTSPServer_StopScheduleInterval, GMAudioRTSPServer_stopServer, data);
    }
    else
    {
        mUsageEnvironment->taskScheduler().unscheduleDelayedTask(mStopTaskToken);
        mServerExit = 1;
    }
}

void GMAudioRTSPServer::stop()
{
    if ( !isRunning() ) return;
    if ( isStopping() ) return;
    setStopping(true);
}

unsigned short GMAudioRTSPServer::serverPort()
{
    if ( !isRunning() || isStopping()) return 0;
    return mServerPort;
}

bool GMAudioRTSPServer::parseAudioFileDuration(char const* fileName, int isSystemMusic)
{
    bool result = true;

    do
    {
        if ( fileName == NULL         ) break;
        if ( access(fileName, 0) != 0 ) break;
        if ( mRTSPServer == NULL      ) break;

        GMAudioFileSession *audioFileSession = ( ( GMAudioRTSPServer0 * )mRTSPServer )->mAudioFileSession;
        if ( audioFileSession == NULL ) break;
        result = audioFileSession->parseAudioFileDuration(fileName, ( isSystemMusic > 0 ) );

    } while (0);

    return result;
}

void GMAudioRTSPServer::unInstall()
{
    setStopping(true);
    usleep(1000 * 1000);
    setRunning (false);
    setStopping(false);
    mUsageEnvironment   = NULL;
    mRTSPServer         = NULL;
    mStopTaskToken      = NULL;
    mServerPort         = 0;
    mServerExit         = 0;
    mUtility.destorylock(&mSourceLock);
}

void GMAudioRTSPServer::setRunning(bool value)
{
    mUtility.lock(mSourceLock);
    mIsRunning = value;
    mUtility.unlock(mSourceLock);
}

bool GMAudioRTSPServer::isRunning()
{
    bool result = false;
    mUtility.lock(mSourceLock);
    result = mIsRunning;
    mUtility.unlock(mSourceLock);
    return result;
}

void GMAudioRTSPServer::setStopping(bool value)
{
    mUtility.lock(mSourceLock);
    mIsStopping = value;
    mUtility.unlock(mSourceLock);
}

bool GMAudioRTSPServer::isStopping()
{
    bool result = false;
    mUtility.lock(mSourceLock);
    result = mIsStopping;
    mUtility.unlock(mSourceLock);
    return result;
}


