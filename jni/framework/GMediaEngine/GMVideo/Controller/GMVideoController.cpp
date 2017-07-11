#include "GMVideoController.h"
#include "../../../../source/GFramework.h"
#include <sys/prctl.h>

extern char g_exitCode;
GMVideoController*	GMVideoController::m_pInstance = NULL;

GMVideoController::GMVideoController():
	m_OnModuleEnter(NULL),
	m_OnModuleExit(NULL),
	m_OnModuleDelegate(NULL),
	m_bStarted(false),
    m_bStop(true),
    m_emReason(REASON_NONE)
{
	init();
}

GMVideoController::~GMVideoController()
{
	m_OnModuleEnter		= NULL;
	m_OnModuleExit		= NULL;
	m_OnModuleDelegate	= NULL;
	destory();
}

GMVideoController*	GMVideoController::sharedInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new GMVideoController();
	return m_pInstance;
}

void GMVideoController::init()
{
	m_pLock = new ants360::CLock;
}

void GMVideoController::destory()
{
	if( m_pLock!= NULL )
		delete m_pLock;
	m_pLock = NULL;
}

void GMVideoController::bindModuleEvent( GMFUN_MODULE_EVENT onEnter, GMFUN_MODULE_EVENT onExit, void * delegate )
{
	m_OnModuleEnter = onEnter;
	m_OnModuleExit = onExit;
	m_OnModuleDelegate = delegate;
}

bool GMVideoController::isSameParam(const char* address, const char* username, const char* password)
{
	USE_LOG;
	bool ret = true;
	do
	{
		ants360::CAutoLock cAutoLock(m_pLock);
		if( strcmp(address,m_address)
			|| strcmp(username,m_username)
			|| strcmp(password,m_password) )
		{
			ret = false;
		}
	}while(0);
	return ret;
}

bool GMVideoController::setParam(const char* address, const char* username, const char* password)
{
	USE_LOG;
	bool ret = true;
	do
	{
		ants360::CAutoLock cAutoLock(m_pLock);
		if ( address != NULL && strlen( address ) > 0 )
		{
			strncpy(m_address, address, sizeof(m_address));
		}
		else
		{
			ret = false;
			break;
		}
		if ( username != NULL && password != NULL && strlen( username ) >= 0 && strlen( password ) >= 0 )
		{
			strncpy(m_username, username, sizeof(m_username));
			strncpy(m_password, password, sizeof(m_password));
		}
		else
		{
			ret = false;
			break;
		}
	}while(0);
	return ret;
}

void GMVideoController::setShutDownReason(EMShutdown reason)
{
	ants360::CAutoLock cAutoLock(m_pLock);
	m_emReason = reason;
}

EMShutdown GMVideoController::getShutDownReason()
{
	ants360::CAutoLock cAutoLock(m_pLock);
	return m_emReason;
}

bool GMVideoController::isPlay()
{
	USE_LOG;
	bool ret = false;
	do
	{
		ants360::CAutoLock cAutoLock(m_pLock);
		if (m_bStarted && !m_bStop)
	    {
	    	ret = true;
	        break;
	    }
	}while(0);
	return ret;
}

void GMVideoController::start( const char *address, const char *username, const char * password, GMFUN_MEDIA_EVENT onVideoEvent, GMFUN_555_EVENT on555Event, void *delegate )
{
	const char *fError = NULL;
	m_OnVideoCallback 	= onVideoEvent;
    m_On555Callback		= on555Event;
    m_OnDelegate		= delegate;

    do
    {
		if(isSameParam(address, username, password) && isPlay())
		{
			fError = "m_pRTSPClient is playing";
			break;
		}

		LOGI( "start( %s, %s, %s, ... )", address, username, password );


		if(!setParam(address, username, password))
		{
			fError = "address or username or password is invalid!";
			break;
		}
		if(!GMUtility::sharedInstance()->createThread(&m_ThreadHandle, ThreadWrapper, this))
		{
			fError = "create Thread failed";
			break;
		}
    } while (0);

    if ( fError != NULL && onVideoEvent != NULL )
    {
    	LOGE( "start( ... ) >> ERROR >> %s \n", fError );
    	onVideoEvent( delegate, true, false, fError );
    }
}

void GMVideoController::stop (GMFUN_MEDIA_EVENT onVideoEvent, void* delegate)
{
	USE_LOG;
	const char *fError = NULL;
	do
	{
		ants360::CAutoLock cAutoLock(m_pLock);
	    if (m_bStarted && !m_bStop)
	    {
			shutdown();
			m_emReason = REASON_USER;
	    }
		else
		{
			fError = "I'm aready stoped or not running!";
			break;
		}
	}while(0);
	if ( fError != NULL && m_OnVideoCallback!= NULL )
	{
        LOGE( "RTSP >> stop( ... ) >> ERROR >> %s \n", fError );
        onVideoEvent( delegate, false, false, fError );
	}
	GMUtility::sharedInstance()->waitingThread(m_ThreadHandle);
}

void* GMVideoController::ThreadWrapper(void *pThis)
{
    prctl(PR_SET_NAME, (unsigned long)"GMVideoController",0,0,0);
    GMVideoController * p = (GMVideoController*) pThis;
    int nErr = p->ThreadEntry();
    return (void *) nErr;
}

int GMVideoController::ThreadEntry()
{
    m_pLock->lock();
	m_bStarted = true;
	m_pLock->unlock();;
	setShutDownReason(REASON_NONE);
	LOGI( "Player Start \n" );

	if ( m_OnModuleEnter ) m_OnModuleEnter( m_OnModuleDelegate );
	runLoop();
    if ( m_OnModuleExit  ) m_OnModuleExit ( m_OnModuleDelegate  );

    LOGI( "Player Stop \n" );

    do
    {
		LOGI("the stop reason id is %d", m_emReason);
		switch(getShutDownReason())
		{
			case REASON_LIVE555BYE:
				if ( (m_On555Callback != NULL) && (m_OnDelegate != NULL))
				{
					const char* fError = "live555 failed!";
					m_On555Callback( m_OnDelegate, 1000, fError );
				}
				break;
			default:
				break;
		}
    }while(0);
    return 0;
}

void GMVideoController::runLoop()
{
USE_LOG;
	const char *fError = NULL;

    do
    {
        m_taskScheduler = BasicTaskScheduler::createNew();
		if ( m_taskScheduler == NULL )
        {
        	fError = "create BasicTaskScheduler failed";
    		break;
        }

        m_usageEnvironment = BasicUsageEnvironment::createNew(*m_taskScheduler);
		if ( m_usageEnvironment == NULL )
        {
        	fError = "create BasicUsageEnvironment failed";
    		break;
        }

		m_pLock->lock();
        GMVideoRTSPClient* client = GMVideoRTSPClient::createNew(m_On555Callback, m_OnDelegate
			, *m_usageEnvironment, m_address, m_username, m_password, 0, "GMVideoRTSPClient");
		m_pLock->unlock();
        if ( client == NULL )
        {
        	fError = "create GMVideoRTSPClient failed";
    		break;
        }

		m_pLock->lock();
		m_bStarted = true;
		m_bStop = false;
		m_pLock->unlock();

        client->start();

        m_usageEnvironment->taskScheduler().doEventLoop( &g_exitCode );

		client->stop();

		if(client!= NULL)
		{
			Medium::close(client);
		}
		if( m_usageEnvironment != NULL )
		{
			m_usageEnvironment->reclaim();
			m_usageEnvironment = NULL;
		}
		if( m_taskScheduler != NULL )
		{
			delete m_taskScheduler;
			m_taskScheduler = NULL;
		}
		m_pLock->lock();
		m_bStarted = false;
		m_bStop = true;
		m_pLock->unlock();
		g_exitCode = 0;
    } while ( 0 );

    if ( fError != NULL && m_OnVideoCallback!= NULL )
    {
        LOGE( "RTSP >> start( ... ) >> ERROR >> %s \n", fError );
    	m_OnVideoCallback( m_OnDelegate, true, false, fError );
    }
}