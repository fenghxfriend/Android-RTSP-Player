#ifndef GMAudioController_h
#define GMAudioController_h

#include <iostream>
#include <memory>
#include <string>
#include "../RTSPClient/GMAudioRTSPClient.h"
#include "../../GMUtility/GMUtility.h"

using namespace std;

class GMAudioController
{
public:
    static GMAudioController* sharedInstance();
    void start(const char *url, GMRTSPDIDPLYFUN playcallback, GMRTSPDIDPLYFUN stopcallback, void *data);
    void stop();
    GMAudioRTSPClient* RTSPClientInstance();
    GMAudioController();
    ~GMAudioController();

public:
    friend void* GMAudioController_moduleRunLoop_C(void *data);

private:
    void init();
    void moduleRunLoop();
    void destory();

private:
    string              mURL;
    GMAudioRTSPClient*  m_pRTSPClient;
    GMRTSPDIDPLYFUN     mDidPlayCallback;
    GMRTSPDIDPLYFUN     mDidStopCallback;
    void     			*mDidCallbackData;
    GMThread            mThreadHandle;
    static GMAudioController*  m_pInstance;
    class Garbo

    {

    public:

        ~Garbo()

        {
            if (GMAudioController::m_pInstance)

            {
                delete GMAudioController::m_pInstance;
                m_pInstance = NULL;
            }
        }
    };

    static Garbo garbo;
};

#endif
