#ifndef _ANTS360_AUTOLOG_H_
#define _ANTS360_AUTOLOG_H_

#include <android/log.h>
#include <sys/time.h>

#include "ants360typedefs.h"

#define LOG_LEVEL 3
#define LOG_ENABLE 0

#ifndef android_printLog
#define android_printLog(prio, tag, fmt...) \
    __android_log_print(prio, tag, fmt)
#endif

#ifndef android_vprintLog
#define android_vprintLog(prio, cond, tag, fmt...) \
    __android_log_vprint(prio, tag, fmt)
#endif

/*
 * Basic log message macro.
 *
 * Example:
 *  ALOG(LOG_WARN, NULL, "Failed with error %d", errno);
 *
 * The second argument may be NULL or "" to indicate the "global" tag.
 */
#ifndef ALOG
#define ALOG(priority, tag, ...) \
    android_printLog(ANDROID_##priority, tag, __VA_ARGS__)
#endif


namespace ants360 {

class CAutoLog
{
public:
    CAutoLog(LPCSTR szFile, LPCSTR szFunction, s32 nLine)
        : m_szCaption(szFile),
          m_szFunction(szFunction)
    {
        ALOG(LOG_DEBUG, m_szCaption, "[%s@%d] enter", m_szFunction, nLine);
    }

    ~CAutoLog()
    {
        ALOG(LOG_DEBUG, m_szCaption, "[%s] leave", m_szFunction);
    }

    LPCSTR m_szCaption;
    LPCSTR m_szFunction;
};

class CAutoLogReportTime
{
public:
    CAutoLogReportTime(LPCSTR szFile, LPCSTR szFunction, s32 nLine)
        : m_szCaption(szFile),
          m_szFunction(szFunction)
    {
        ALOG(LOG_DEBUG, m_szCaption, "[%s@%d] enter", m_szFunction, nLine);
        gettimeofday(&begin, NULL);
    }

    ~CAutoLogReportTime()
    {
        struct timeval end;
        gettimeofday(&end, NULL);
        int span = 1000000 * ( end.tv_sec - begin.tv_sec ) + end.tv_usec - begin.tv_usec;
        ALOG(LOG_DEBUG, m_szCaption, "[%s] leave with time elapsed: %dus", m_szFunction, span);
    }

    LPCSTR m_szCaption;
    LPCSTR m_szFunction;
    struct timeval begin;
};

#define USE_LOG              (void(0))
#define USE_LOG_REPORT_TIME  (void(0))

#define LOGD(fmt, ...) (void(0))
#define LOGI(fmt, ...) (void(0))
#define LOGV(fmt, ...) (void(0))
#define LOGW(fmt, ...) (void(0))
#define LOGE(fmt, ...) ALOG(LOG_ERROR, __FILE__, "[%s@%d] " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__);

#if LOG_ENABLE  // enable logcat output
#undef USE_LOG
#define USE_LOG  ants360::CAutoLog this_log(__FILE__, __FUNCTION__, __LINE__)
#undef USE_LOG_REPORT_TIME
#define USE_LOG_REPORT_TIME  ants360::CAutoLogReportTime this_log(__FILE__, __FUNCTION__, __LINE__)

#if (LOG_LEVEL>=0)
#undef LOGE
#define LOGE(fmt, ...) ALOG(LOG_ERROR, __FILE__, "[%s@%d] " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__);
#endif

#if (LOG_LEVEL>=1)
#undef LOGW
#define LOGW(fmt, ...) ALOG(LOG_WARN,  __FILE__, "[%s@%d] " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__);
#endif

#if (LOG_LEVEL>=2)
#undef LOGV
#define LOGV(fmt, ...) ALOG(LOG_VERBOSE,  __FILE__, "[%s@%d] " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__);
#endif

#if (LOG_LEVEL>=3)
#undef LOGI
#define LOGI(fmt, ...) ALOG(LOG_INFO,  __FILE__, "[%s@%d] " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__);
#endif

#if (LOG_LEVEL>=4)
#undef LOGD
#define LOGD(fmt, ...) ALOG(LOG_DEBUG, __FILE__, "[%s@%d] " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__);
#endif

#endif

} // namespace kedacom



#endif // _ANTS360_AUTOLOG_H_