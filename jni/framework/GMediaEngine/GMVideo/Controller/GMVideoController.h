#ifndef GMVideoController_h
#define GMVideoController_h

#include <iostream>
#include <memory>
#include <string>
#include "../RTSPClient/GMVideoRTSPClient.h"
#include "../../GMUtility/GMUtility.h"

using namespace std;

class GMVideoController
{
public:

    GMVideoController	();
    ~GMVideoController	();

    static GMVideoController* sharedInstance();

    void bindModuleEvent( GMFUN_MODULE_EVENT onEnter, GMFUN_MODULE_EVENT onExit, void* delegate );
    void start( const char* address	, const char* username, const char * password, GMFUN_MEDIA_EVENT onVideoEvent, GMFUN_555_EVENT on555Event, void *delegate );
    void stop( GMFUN_MEDIA_EVENT onVideoEvent, void* delegate );
    void setShutDownReason(EMShutdown reason);
    EMShutdown getShutDownReason();

private:
    void init	();
    void destory();
    bool isSameParam(const char* address, const char* username, const char* password);
    bool isPlay();
    bool setParam(const char* address, const char* username, const char* password);
    static void* ThreadWrapper(void *pThis);
    int ThreadEntry();
    void runLoop();

private:

    GMFUN_MEDIA_EVENT     	 m_OnVideoCallback;
    GMFUN_555_EVENT     	 m_On555Callback;
    void                    *m_OnDelegate;

    GMFUN_MODULE_EVENT       m_OnModuleEnter;
    GMFUN_MODULE_EVENT	     m_OnModuleExit;
    void                    *m_OnModuleDelegate;

    char                     m_address[ADDRESS_MAX_LEN];
    char                     m_username[USERNAME_MAX_LEN];
    char                     m_password[PASSWORD_MAX_LEN];

	UsageEnvironment        *m_usageEnvironment;
    BasicTaskScheduler      *m_taskScheduler;

    GMThread                 m_ThreadHandle;
    ants360::ILock          *m_pLock;

    bool                     m_bStarted;
    bool                     m_bStop;
    EMShutdown               m_emReason;

    static GMVideoController*  m_pInstance;

    class Garbo

    {

    public:

        ~Garbo()

        {
            if (GMVideoController::m_pInstance)

            {
                delete GMVideoController::m_pInstance;
                m_pInstance = NULL;
            }
        }
    };

    static Garbo garbo;
};

#endif
