#ifndef GMAudioRTSPServer_h
#define GMAudioRTSPServer_h

#include <iostream>
#include <memory>
#include <string>
#include "../../GMUtility/GMUtility.h"

#define GMAudioServerDescription         "RTSP/RTP stream from a WIS GO7007 encoder"
#define GMAudioServerSamplingFrequency   22050
#define GMAudioServerChannels            1

using namespace std;

class GMAudioRTSPServer
{
public:
    static GMAudioRTSPServer& sharedInstance();
    void install();
    bool start();
    void stop();
    unsigned short serverPort();
    void unInstall();
    bool parseAudioFileDuration(char const* fileName, int isSystemMusic);
    friend void GMAudioRTSPServer_stopServer   (void *data);
    friend void* GMAudioRTSPServer_serverRunLoop(void *data);

private:
    void setRunning(bool value);
    bool isRunning();
    void setStopping(bool value);
    bool isStopping();
    void stopServer(void *data);
    void serverRunLoop();

private:
    GMUtility           mUtility;
    GMLock              mSourceLock;
    bool                mIsRunning;
    bool                mIsStopping;
    unsigned short      mServerPort;
    void                *mStopTaskToken;
    UsageEnvironment    *mUsageEnvironment;
    RTSPServer          *mRTSPServer;
    char                mServerExit;
};

#endif
