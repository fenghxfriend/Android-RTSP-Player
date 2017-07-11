#ifndef _ANTS360_AUTOLOCK_H_
#define _ANTS360_AUTOLOCK_H_


#include <pthread.h>

#include "androidlog.h"
#include "ants360typedefs.h"
#include <errno.h>

namespace ants360 {

#define BEGIN_AUTOLOCK_TRACE(pLock)   {                                   \
    CAutoLockTrace cAutoLock((pMutex), __FILE__, __FUNCTION__, __LINE__);  \

#define BEGIN_AUTOLOCK(pLock)   {  \
    CAutoLock cAutoLock((pLock));  \

#define END_AUTOLOCK     }

class ILock
{
    public:
        virtual void lock() = 0;
        virtual void unlock() = 0;
        virtual void wait() = 0;
        virtual void acttive() = 0;
        virtual void acttiveall() = 0;
};

class CLock : public virtual ILock
{
    public:
        CLock()
        {
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);

            pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);

            pthread_mutex_init(&_lock,&attr);
            pthread_mutexattr_destroy(&attr);

            pthread_cond_init(&_cond, NULL);
        }
        virtual ~CLock()
        {
            pthread_mutex_destroy(&_lock);
            pthread_cond_destroy(&_cond);
        }

        inline virtual void lock()
        {
            if( 0 != pthread_mutex_lock(&_lock) )
            {
                //  if( errno == EDEADLK )
                //  {
                //      assert( false );
                //  }
            }
        }

        inline virtual void unlock()
        {
            pthread_mutex_unlock(&_lock);
        }

        inline virtual void wait()
        {
            pthread_cond_wait(&_cond, &_lock);
        }

        inline virtual void acttive()
        {
            pthread_cond_signal(&_cond);
        }

        inline virtual void acttiveall()
        {
            pthread_cond_broadcast(&_cond);
        }
    private:
        CLock( const CLock& )
        {
        }
        pthread_mutex_t _lock;
        pthread_cond_t _cond;
};


class CAutoLockTrace
{
public:
    CAutoLockTrace(ILock* pLock, LPCSTR szFile, LPCSTR szFunction, s32 nLine)
        : m_pLock(NULL),
          m_szFile(szFile),
          m_szFunction(szFunction)
    {
        m_pLock = pLock;
        m_pLock->lock();
        if ( errno == EDEADLK)
        {
            ALOG(LOG_VERBOSE,  szFile, "[%s@%d] lock acquisition failed", szFunction, nLine);
        }
        else
        {
            ALOG(LOG_VERBOSE,  szFile, "[%s@%d] lock acquired", szFunction, nLine);
        }
    }

    ~CAutoLockTrace()
    {
        m_pLock->unlock();
        ALOG(LOG_VERBOSE,  m_szFile, "[%s] lock released", m_szFunction);
    }

private:
    ILock*           m_pLock;
    LPCSTR           m_szFile;
    LPCSTR           m_szFunction;
};

class CAutoLock
{
public:
    CAutoLock(ILock* pLock)
        :m_pLock(NULL)
    {
        m_pLock = pLock;
        m_pLock->lock();
    }

    ~CAutoLock()
    {
        m_pLock->unlock();
    }
private:
    ILock* m_pLock;
};


} // namespace ants360


#endif // _ANTS360_AUTOLOCK_H_