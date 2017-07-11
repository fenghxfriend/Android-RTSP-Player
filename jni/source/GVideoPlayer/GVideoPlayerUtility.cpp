#include "GVideoPlayerUtility.h"

GVPULock GVideoPlayerUtility::createLock()
{
    pthread_mutex_t *lock = ( pthread_mutex_t * )malloc( sizeof( pthread_mutex_t ) );
    pthread_mutex_init( lock, NULL );
    return ( GVPULock )lock;
}

void GVideoPlayerUtility::lock( GVPULock lock )
{
    if ( lock == NULL ) return;
    pthread_mutex_lock( ( pthread_mutex_t * )lock );
}

bool GVideoPlayerUtility::trylock( GVPULock lock )
{
    if ( lock == NULL ) return false;
    return ( pthread_mutex_trylock( ( pthread_mutex_t * )lock ) == 0 ? true : false );
}

void GVideoPlayerUtility::unlock( GVPULock lock )
{
    if ( lock == NULL ) return;
    pthread_mutex_unlock( ( pthread_mutex_t * )lock );
}

void GVideoPlayerUtility::destorylock( GVPULock *lock )
{
    if ( lock == NULL || ( *lock ) == NULL ) return;
    pthread_mutex_destroy( ( pthread_mutex_t * )( *lock ) );
    free( *lock );
    ( *lock ) = NULL;
}

GVPUCondition GVideoPlayerUtility::createCondition(void)
{
    pthread_cond_t *cond = ( pthread_cond_t * )malloc( sizeof( pthread_cond_t ) );
    pthread_cond_init(cond, NULL);
    return ( GVPUCondition )cond;
}

void GVideoPlayerUtility::signal      	( GVPUCondition cond  )
{
	if( cond == NULL ) return;
	pthread_cond_signal( ( pthread_cond_t * )cond );
}

void GVideoPlayerUtility::broadcast      	( GVPUCondition cond  )
{
	if( cond == NULL ) return;
	pthread_cond_broadcast( ( pthread_cond_t * )cond );
}

void GVideoPlayerUtility::wait      	( GVPUCondition cond, GVPULock lock   )
{
	if( cond == NULL || lock == NULL ) return;
	pthread_cond_wait( ( pthread_cond_t * )cond, ( pthread_mutex_t * )lock );
}

void GVideoPlayerUtility::destoryCondition	( GVPUCondition *cond )
{
    if ( cond == NULL || ( *cond ) == NULL ) return;
    pthread_cond_destroy( ( pthread_cond_t * )( *cond ) );
    free( *cond );
    ( *cond ) = NULL;
}

bool GVideoPlayerUtility::createThread( GVPUThread *handle, GVPUThreadTask task, void *data )
{
    bool result = false;
    if ( handle != NULL && task != NULL && pthread_create( handle, NULL, ( void *( * )( void * ) )task, data ) == 0 ) result = true;
    return result;
}

void GVideoPlayerUtility::waitingThread( GVPUThread handle )
{
    pthread_join(handle, 0);
}
