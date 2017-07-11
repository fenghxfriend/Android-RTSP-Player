#ifndef GAudioUtility_h
#define GAudioUtility_h

#include <iostream>
#include <stdint.h>
#include <pthread.h>

using namespace std;

typedef void        *GAMULock;
typedef pthread_t   GAMUThread;
typedef void ( * GAMUThreadTask)( void * );

class GAudioUtility
{
public:
	GAMULock	createLock	( void           );
    void     	lock      	( GAMULock lock  );
    bool    	trylock   	( GAMULock lock  );
    void      	unlock    	( GAMULock lock  );
    void     	destorylock	( GAMULock *lock );
    
public:
    bool 		createThread   	( GAMUThread *handle, GAMUThreadTask task, void *data );
    void      	waitingThread	( GAMUThread handle);
};

#endif
