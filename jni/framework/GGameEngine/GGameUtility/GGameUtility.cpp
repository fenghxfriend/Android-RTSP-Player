#include "GGameUtility.h"

GGameLock GGameUtility::createLock()
{
    pthread_mutex_t *lock = ( pthread_mutex_t * )malloc( sizeof( pthread_mutex_t ) );
    pthread_mutex_init( lock, 0 );
    return ( GGameLock )lock;
}

void GGameUtility::lock( GGameLock lock )
{
    if ( lock == NULL ) return;
    pthread_mutex_lock( ( pthread_mutex_t * )lock );
}

bool GGameUtility::trylock( GGameLock lock )
{
    if ( lock == NULL ) return false;
    return ( pthread_mutex_trylock( ( pthread_mutex_t * )lock ) == 0 ? true : false );
}

void GGameUtility::unlock( GGameLock lock )
{
    if ( lock == NULL ) return;
    pthread_mutex_unlock( ( pthread_mutex_t * )lock );
}

void GGameUtility::destorylock( GGameLock *lock )
{
    if ( lock == NULL || ( *lock ) == NULL ) return;
    pthread_mutex_destroy( ( pthread_mutex_t * )( *lock ) );
    free( *lock );
    ( *lock ) = NULL;
}

bool GGameUtility::createThread( GGameThread *handle, GGameThreadTask task, void *data )
{
    bool result = false;
    if ( handle != NULL && task != NULL && pthread_create( handle, NULL, ( void *( * )( void * ) )task, data ) == 0 ) result = true;
    return result;
}

void GGameUtility::waitingThread( GGameThread handle )
{
    pthread_join(handle, 0);
}
