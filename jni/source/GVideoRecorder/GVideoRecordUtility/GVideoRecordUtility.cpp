#include "GVideoRecordUtility.h"

GVIDEORCDLock GVideoRecordUtility::createLock()
{
    pthread_mutex_t *lock = ( pthread_mutex_t * )malloc( sizeof( pthread_mutex_t ) );
    pthread_mutex_init( lock, 0 );
    return ( GVIDEORCDLock )lock;
}

void GVideoRecordUtility::lock( GVIDEORCDLock lock )
{
    if ( lock == NULL ) return;
    pthread_mutex_lock( ( pthread_mutex_t * )lock );
}

bool GVideoRecordUtility::trylock( GVIDEORCDLock lock )
{
    if ( lock == NULL ) return false;
    return ( pthread_mutex_trylock( ( pthread_mutex_t * )lock ) == 0 ? true : false );
}

void GVideoRecordUtility::unlock( GVIDEORCDLock lock )
{
    if ( lock == NULL ) return;
    pthread_mutex_unlock( ( pthread_mutex_t * )lock );
}

void GVideoRecordUtility::destorylock( GVIDEORCDLock *lock )
{
    if ( lock == NULL || ( *lock ) == NULL ) return;
    pthread_mutex_destroy( ( pthread_mutex_t * )( *lock ) );
    free( *lock );
    ( *lock ) = NULL;
}

bool GVideoRecordUtility::createThread( GVIDEORCDThread *handle, GVIDEORCDThreadTask task, void *data )
{
    bool result = false;
    if ( handle != NULL && task != NULL && pthread_create( handle, NULL, ( void *( * )( void * ) )task, data ) == 0 ) result = true;
    return result;
}

void GVideoRecordUtility::waitingThread( GVIDEORCDThread handle )
{
    pthread_join(handle, 0);
}
