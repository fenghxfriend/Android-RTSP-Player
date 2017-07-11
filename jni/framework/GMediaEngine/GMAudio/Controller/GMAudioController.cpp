#include "GMAudioController.h"

GMAudioController*	GMAudioController::m_pInstance = NULL;

void* GMAudioController_moduleRunLoop_C(void *data)
{
    if ( data == 0 ) return NULL;
    ( ( GMAudioController * )data )->moduleRunLoop();
	return NULL;
}

GMAudioController::GMAudioController()
{
    init();
}

GMAudioController::~GMAudioController()
{
    destory();
}

GMAudioController* GMAudioController::sharedInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new GMAudioController();
	return m_pInstance;
}

void GMAudioController::start(const char *url, GMRTSPDIDPLYFUN playcallback, GMRTSPDIDPLYFUN stopcallback, void *data)
{
    do
    {
        if ( url == 0                   ) break;
        if ( m_pRTSPClient->isRunning()    ) break;
        if ( m_pRTSPClient->isStopping()   ) break;

        init();
        mURL                = url;
        mDidPlayCallback    = playcallback;
        mDidStopCallback    = stopcallback;
        mDidCallbackData	= data;

        GMUtility::sharedInstance()->createThread( &mThreadHandle, GMAudioController_moduleRunLoop_C, this );

    } while (0);
}

void GMAudioController::stop()
{
    if ( !m_pRTSPClient->isRunning() ) return;
    if ( m_pRTSPClient->isStopping() ) return;
    m_pRTSPClient->stop();
}

GMAudioRTSPClient* GMAudioController::RTSPClientInstance()
{
    return m_pRTSPClient;
}

void GMAudioController::init()
{
    mDidPlayCallback = NULL;
    mDidStopCallback = NULL;
    mDidCallbackData = NULL;
	m_pRTSPClient = new GMAudioRTSPClient();
}

void GMAudioController::moduleRunLoop()
{
    m_pRTSPClient->start( mURL.c_str(), mDidPlayCallback, mDidStopCallback, mDidCallbackData );
}

void GMAudioController::destory()
{
    mDidPlayCallback = NULL;
    mDidStopCallback = NULL;
    mDidCallbackData = NULL;
	if(m_pRTSPClient != NULL)
		delete m_pRTSPClient;
	m_pRTSPClient = NULL;
}




