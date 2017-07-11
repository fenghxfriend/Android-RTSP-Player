#ifndef GGameUtility_h
#define GGameUtility_h

#include <iostream>
#include <stdint.h>
#include <pthread.h>

using namespace std;

typedef void        *GGameLock;
typedef pthread_t   GGameThread;
typedef void        ( * GGameThreadTask)( void * );

class GGameUtility
{
public:
    GGameLock   createLock     ( void            );
    void        lock           ( GGameLock lock  );
    bool        trylock        ( GGameLock lock  );
    void        unlock         ( GGameLock lock  );
    void        destorylock    ( GGameLock *lock );
    
public:
    bool         createThread   ( GGameThread *handle, GGameThreadTask task, void *data );
    void         waitingThread  ( GGameThread handle);
};

#endif
