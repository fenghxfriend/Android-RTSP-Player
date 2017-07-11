#include "GMUtility.h"

GMUtility*	GMUtility::m_pInstance = NULL;
GMUtility*	GMUtility::sharedInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new GMUtility();
	return m_pInstance;
}

GMLock GMUtility::createLock()
{
    pthread_mutex_t *lock = ( pthread_mutex_t * )malloc( sizeof( pthread_mutex_t ) );
    pthread_mutex_init( lock, 0 );
    return ( GMLock )lock;
}

void GMUtility::lock( GMLock lock )
{
    if ( lock == NULL ) return;
    pthread_mutex_lock( ( pthread_mutex_t * )lock );
}

bool GMUtility::trylock( GMLock lock )
{
    if ( lock == NULL ) return false;
    return ( pthread_mutex_trylock( ( pthread_mutex_t * )lock ) == 0 ? true : false );
}

void GMUtility::unlock( GMLock lock )
{
    if ( lock == NULL ) return;
    pthread_mutex_unlock( ( pthread_mutex_t * )lock );
}

void GMUtility::destorylock( GMLock *lock )
{
    if ( lock == NULL || ( *lock ) == NULL ) return;
    pthread_mutex_destroy( ( pthread_mutex_t * )( *lock ) );
    free( *lock );
    ( *lock ) = NULL;
}

GMCondition GMUtility::createCondition(void)
{
    pthread_cond_t *cond = ( pthread_cond_t * )malloc( sizeof( pthread_cond_t ) );
    pthread_cond_init(cond, NULL);
    return ( GMCondition )cond;
}

void GMUtility::signal      	( GMCondition cond  )
{
	if( cond == NULL ) return;
	pthread_cond_signal( ( pthread_cond_t * )cond );
}

void GMUtility::broadcast      	( GMCondition cond  )
{
	if( cond == NULL ) return;
	pthread_cond_broadcast( ( pthread_cond_t * )cond );
}

void GMUtility::wait      	( GMCondition cond, GMLock lock   )
{
	if( cond == NULL || lock == NULL ) return;
	pthread_cond_wait( ( pthread_cond_t * )cond, ( pthread_mutex_t * )lock );
}

void GMUtility::destoryCondition	( GMCondition *cond )
{
    if ( cond == NULL || ( *cond ) == NULL ) return;
    pthread_cond_destroy( ( pthread_cond_t * )( *cond ) );
    free( *cond );
    ( *cond ) = NULL;
}

bool GMUtility::createThread( GMThread *pHandle, GMThreadTask task, void *data )
{
	int ret = true;
	do{
		int nErr;
		pthread_attr_t attr;
	    nErr = pthread_attr_init(&attr);
	    if (nErr)
	    {
	    	ret = false;
			break;
	    }

	    nErr = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	    if (nErr)
	    {
	        ret = false;
			break;
	    }

	    nErr = pthread_create(pHandle, &attr, ( void* ( * )( void * ) )task, data);
	    if (nErr)
	    {
	        ret = false;
			break;
	    }

	    pthread_attr_destroy(&attr);
	}while(0);

    return ret;
}

void GMUtility::waitingThread( GMThread handle )
{
    int retv = 0;
    pthread_join(handle, (void**)&retv);
}
