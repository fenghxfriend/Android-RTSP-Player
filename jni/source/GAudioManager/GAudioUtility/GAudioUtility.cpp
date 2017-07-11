#include "GAudioUtility.h"

GAMULock GAudioUtility::createLock()
{
    pthread_mutex_t *lock = ( pthread_mutex_t * )malloc( sizeof( pthread_mutex_t ) );
    pthread_mutex_init( lock, 0 );
    return ( GAMULock )lock;
}

void GAudioUtility::lock( GAMULock lock )
{
    if ( lock == NULL ) return;
    pthread_mutex_lock( ( pthread_mutex_t * )lock );
}

bool GAudioUtility::trylock( GAMULock lock )
{
    if ( lock == NULL ) return false;
    return ( pthread_mutex_trylock( ( pthread_mutex_t * )lock ) == 0 ? true : false );
}

void GAudioUtility::unlock( GAMULock lock )
{
    if ( lock == NULL ) return;
    pthread_mutex_unlock( ( pthread_mutex_t * )lock );
}

void GAudioUtility::destorylock( GAMULock *lock )
{
    if ( lock == NULL || ( *lock ) == NULL ) return;
    pthread_mutex_destroy( ( pthread_mutex_t * )( *lock ) );
    free( *lock );
    ( *lock ) = NULL;
}

bool GAudioUtility::createThread( GAMUThread *handle, GAMUThreadTask task, void *data )
{
    bool result = false;
    if ( handle != NULL && task != NULL && pthread_create( handle, NULL, ( void *( * )( void * ) )task, data ) == 0 ) result = true;
    return result;
}

void GAudioUtility::waitingThread( GAMUThread handle )
{
    pthread_join(handle, 0);
}
