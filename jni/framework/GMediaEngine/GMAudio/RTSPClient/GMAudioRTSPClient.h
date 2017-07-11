#ifndef GMAudioRTSPClient_h
#define GMAudioRTSPClient_h

#include "../RTSPConfiger/GMAudioRTSPConfiger.h"
#include "../../GMUtility/GMUtility.h"

using namespace std;

class GMAudioRTSPClient
{
public:
    void start(const char *url, GMRTSPDIDPLYFUN playcallback, GMRTSPDIDPLYFUN stopcallback, void *data);
    void stop();
    bool isRunning();
    bool isStopping();

    GMAudioRTSPClient();
    ~GMAudioRTSPClient();

public:
    friend void GMRTSPAudioContinueAfterDESCRIBE (class RTSPClient  *client, int resultCode, char *resultString);
    friend void GMRTSPAudioContinueAfterSETUP    (class RTSPClient  *client, int resultCode, char *resultString);
    friend void GMRTSPAudioContinueAfterPLAY     (class RTSPClient  *client, int resultCode, char *resultString);
    friend void GMRTSPAudioSetupNextSession      (GMAudioRTSPClient *rtsp);
    friend void GMRTSPAudioSubsessionAfterPlaying(void *clientData);
    friend void GMRTSPAudioSubsessionAfterBye    (void *clientData);
    friend void GMRTSPAudioTearDownTask          (void *clientData);
    friend void GMRTSPAudioMediaSinkStopCall     (void *clientData);

private:
    void init();
    void destory();
    void playCallback();
    void stopCallback();
    void closeMediaSink();
    void closeStream();

private:
    GMAudioRTSPConfiger mConfiger;
};

#endif
