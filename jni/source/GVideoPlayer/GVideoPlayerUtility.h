#ifndef GVideoPlayerUtility_h
#define GVideoPlayerUtility_h

#include <iostream>
#include <stdint.h>
#include <pthread.h>

using namespace std;

typedef void        *GVPULock;
typedef void        *GVPUCondition;
typedef pthread_t   GVPUThread;
typedef void ( * GVPUThreadTask)( void * );

class GVideoPlayerUtility
{
public:
	GVPULock	createLock	( void           );
    void     	lock      	( GVPULock lock  );
    bool    	trylock   	( GVPULock lock  );
    void      	unlock    	( GVPULock lock  );
    void     	destorylock	( GVPULock *lock );

    GVPUCondition createCondition(void);
    void     	signal      	( GVPUCondition cond  );
    void     	broadcast      	( GVPUCondition cond  );
    void     	wait      	( GVPUCondition cond, GVPULock lock   );
    void     	destoryCondition	( GVPUCondition *cond );

public:
    bool 		createThread   	( GVPUThread *handle, GVPUThreadTask task, void *data );
    void      	waitingThread	( GVPUThread handle);
};

#endif
