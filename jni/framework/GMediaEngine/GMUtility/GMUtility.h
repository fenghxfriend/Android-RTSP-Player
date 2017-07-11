#ifndef GMUtility_h
#define GMUtility_h

#include <iostream>
#include <stdint.h>
#include <pthread.h>
#include "videodef.h"
#include "ants360typedefs.h"
#include "androidlog.h"
#include "autofile.h"
#include "autolock.h"

using namespace std;

typedef void* GMLock;
typedef void* GMCondition;
typedef pthread_t GMThread;
typedef void*( * GMThreadTask 		)( void * 							);
typedef void ( * GMFUN_MODULE_EVENT	)( void * 							);
typedef void ( * GMFUN_MEDIA_EVENT 	)( void *, bool, bool, const char *	);

typedef void ( * GMFUN_555_EVENT 	)( void *, int, const char * );

#define ADDRESS_MAX_LEN 1024
#define USERNAME_MAX_LEN 128
#define PASSWORD_MAX_LEN 64

class GMUtility
{
public:
    static GMUtility* sharedInstance();
    GMLock       createLock     ( void         );
    void         lock           ( GMLock lock  );
    bool         trylock        ( GMLock lock  );
    void         unlock         ( GMLock lock  );
    void         destorylock    ( GMLock *lock );


    GMCondition createCondition(void);
    void     	signal      	( GMCondition cond  );
    void     	broadcast      	( GMCondition cond  );
    void     	wait      	( GMCondition cond, GMLock lock   );
    void     	destoryCondition	( GMCondition *cond );


public:
    bool         createThread   ( GMThread *handle, GMThreadTask task, void *data );
    void         waitingThread  ( GMThread handle);
private:
    static GMUtility*   m_pInstance;
    class Garbo
    {

    public:

        ~Garbo()

        {
            if (GMUtility::m_pInstance)

            {
                delete GMUtility::m_pInstance;
                m_pInstance = NULL;
            }
        }
    };

    static Garbo garbo;
};

#endif
