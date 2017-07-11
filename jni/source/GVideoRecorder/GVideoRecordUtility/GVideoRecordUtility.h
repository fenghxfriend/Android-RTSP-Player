#ifndef GVideoRecordUtility_h
#define GVideoRecordUtility_h

#include <iostream>
#include <stdint.h>
#include <pthread.h>

using namespace std;

typedef void        *GVIDEORCDLock;
typedef pthread_t   GVIDEORCDThread;
typedef void 		( * GVIDEORCDThreadTask )( void * );

class GVideoRecordUtility
{
public:
	GVIDEORCDLock   createLock     	( void          		);
    void         	lock           	( GVIDEORCDLock lock	);
    bool         	trylock        	( GVIDEORCDLock lock	);
    void         	unlock         	( GVIDEORCDLock lock	);
    void         	destorylock    	( GVIDEORCDLock *lock 	);
    
public:
    bool         	createThread 	( GVIDEORCDThread *handle, GVIDEORCDThreadTask task, void *data );
    void         	waitingThread  	( GVIDEORCDThread handle);
};

#endif
