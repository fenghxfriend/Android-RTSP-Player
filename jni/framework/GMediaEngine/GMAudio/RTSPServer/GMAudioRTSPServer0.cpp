#include "../../../live555/liveMedia/include/liveMedia.hh"
#include "../../../live555/BasicUsageEnvironment/include/BasicUsageEnvironment.hh"
#include "../../../live555/groupsock/include/GroupsockHelper.hh"
#include "../../../live555/liveMedia/include/OnDemandServerMediaSubsession.hh"
#include "../../../live555/liveMedia/include/FileServerMediaSubsession.hh"
#include "GMAudioRTSPServer0.h"
#include "GMAudioRTSPServer.h"
#include "../AudioFileSession/GMAudioFileSession.h"
#include "../AudioRecordSession/GMAudioRecordSession.h"
#include "../../../../source/GCommandManager/GCommandManager.h"

#ifdef __cplusplus
extern "C"
{
#endif
const char *GMGetAudioFilePath(char const *fileName, int *isSystemMusic)
{
	return GCommandManager::sharedInstance().getStreamingFilePath( fileName, isSystemMusic );
}
#ifdef __cplusplus
}
#endif

GMAudioRTSPServer0 *GMAudioRTSPServer0::createNew(UsageEnvironment& env, Port ourPort, UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds)
{
    int ourSocket = -1;
    do
    {
        int ourSocket = setUpOurSocket(env, ourPort);
        if (ourSocket == -1) break;
        return new GMAudioRTSPServer0(env, ourSocket, ourPort, authDatabase, reclamationTestSeconds);
    } while (0);
    
    if (ourSocket != -1) ::closeSocket(ourSocket);
    return NULL;
}

GMAudioRTSPServer0::GMAudioRTSPServer0(UsageEnvironment& env, int ourSocket, Port ourPort, UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds)
: RTSPServer(env, ourSocket, ourPort, authDatabase, reclamationTestSeconds)
{
    mAudioRecordSession = NULL;
    mAudioFileSession   = NULL;
}

GMAudioRTSPServer0::~GMAudioRTSPServer0()
{
    if (mAudioRecordSession != NULL)
    {
        Medium::close( mAudioRecordSession );
    }
    if ( mAudioFileSession != NULL )
    {
        Medium::close(mAudioFileSession);
    }
    mAudioRecordSession = NULL;
    mAudioFileSession   = NULL;
}

ServerMediaSession *GMAudioRTSPServer0::lookupServerMediaSession(char const* streamName)
{
    ServerMediaSession* _serverSession = NULL;
    if (strcmp(streamName, "IosAudioServer") == 0)
    {
        _serverSession = RTSPServer::lookupServerMediaSession(streamName);
        if (_serverSession == NULL)
        {
            _serverSession      = ServerMediaSession::createNew(envir(), streamName, NULL, GMAudioServerDescription, False);
            addServerMediaSession(_serverSession);
            mAudioRecordSession = GMAudioRecordSession::createNew(envir());
            _serverSession->addSubsession(mAudioRecordSession);
        }
    }
    else
    {
        _serverSession = RTSPServer::lookupServerMediaSession(streamName);
        if (_serverSession != NULL)
        {
            RTSPServer::removeServerMediaSession(_serverSession);
        }

        _serverSession = ServerMediaSession::createNew(envir(), streamName, NULL, GMAudioServerDescription, False);
        addServerMediaSession(_serverSession);

        int systemMusicFlag = 0;
        const char* filePath = GMGetAudioFilePath( streamName, &systemMusicFlag );
        mAudioFileSession    = GMAudioFileSession::createNew(envir(), filePath, ( systemMusicFlag > 0 ), False);
        _serverSession->addSubsession(mAudioFileSession);
    }

    return _serverSession;
}

//ServerMediaSession *GMAudioRTSPServer0::lookupServerMediaSession(char const* streamName)
//{
//    ServerMediaSession* _serverSession = NULL;
//    if (strcmp(streamName, "IosAudioServer") == 0)
//    {
//        _serverSession = RTSPServer::lookupServerMediaSession(streamName);
//        if (_serverSession != NULL) RTSPServer::removeServerMediaSession(_serverSession);
//        _serverSession      = ServerMediaSession::createNew(envir(), streamName, NULL, GMAudioServerDescription, False);
//        addServerMediaSession(_serverSession);
//        mAudioRecordSession = GMAudioRecordSession::createNew(envir());
//        _serverSession->addSubsession(mAudioRecordSession);
//    }
//    else
//    {
//        _serverSession = RTSPServer::lookupServerMediaSession(streamName);
//        if (_serverSession != NULL)
//        {
//            RTSPServer::removeServerMediaSession(_serverSession);
//        }
//
//        _serverSession = ServerMediaSession::createNew(envir(), streamName, NULL, GMAudioServerDescription, False);
//        addServerMediaSession(_serverSession);
//
//        const char* filePath = GMGetAudioFilePath(streamName);
//        mAudioFileSession    = GMAudioFileSession::createNew(envir(), filePath, False);
//        _serverSession->addSubsession(mAudioFileSession);
//    }
//
//    return _serverSession;
//}

