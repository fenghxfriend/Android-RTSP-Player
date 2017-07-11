#include "videodef.h"
#include "androidlog.h"
#include "autofile.h"
#include "autolock.h"
#include "jnirenderer_impl.h"
#include "renderer.h"
#include "residentreg.h"

#include <sys/prctl.h>


namespace ants360 {


namespace {
// \brief use a resident registry to manage the residency validity of
// jnirenderer instances.
CResidentReg g_registry;
}

const char * const CJniRenderer::FIELD_NAME = "mNativeRenderer";
const char * const CJniRenderer::METHOD_NAME = "fireOnNewFrame";
const char * const CJniRenderer::METHOD_SIG = "()V";

CJniRenderer::CJniRenderer(JNIEnv *env, jobject jRenderer,
        jclass jcls, jfieldID jfld, jmethodID jmtd)
    : m_jvm(NULL),
    m_jRenderer(NULL),
    m_pRenderer(NULL),
    m_jcls(NULL),
    m_jfld(jfld),
    m_jmtd(jmtd),
    m_bStarted(FALSE),
    m_bStop(FALSE),
    m_bNewFrame(FALSE)
{
    USE_LOG;

	m_tVidFrame.pbyData = (unsigned char*)malloc(MAX_FRAME_BUFFER_SIZE);

    if (env == NULL || jRenderer == NULL
            || jcls == NULL || jfld == NULL || jmtd == NULL)
    {
        LOGE("invalid parameters");
        return;
    }

    if (env->GetJavaVM(&m_jvm) != 0)
    {
        LOGE("get java vm failed");
        return;
    }

    m_jRenderer = env->NewGlobalRef(jRenderer);
    m_jcls = (jclass)env->NewGlobalRef(jcls);

    m_pRenderer = new CRenderer;
    if (m_pRenderer != NULL)
    {
        LOGD("renderer instance allocated: %u", sizeof(CRenderer));
    }

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);

    pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&m_mutex,&attr);
    pthread_mutexattr_destroy(&attr);
    pthread_cond_init(&m_cond, NULL);

    start(env, jRenderer);

    // only register valid ones
    g_registry.Add(this);
}

// FIXME: when exit the application, it will sometimes prints this error log:
// E/libEGL(19049): call to OpenGL ES API with no current context (logged once per thread)
// -- noted by huangxuefeng, 2013-07-29
CJniRenderer::~CJniRenderer()
{
    USE_LOG;

    stop();

    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);

    if (m_pRenderer != NULL)
    {
        m_pRenderer->Destroy();
        delete m_pRenderer;
        m_pRenderer = NULL;
        LOGD("renderer instance freed: %u", sizeof(CRenderer));
    }

    // be sure unregister before killing
    g_registry.Remove(this);


	if(m_tVidFrame.pbyData != NULL)
	{
    	free(m_tVidFrame.pbyData);
    	m_tVidFrame.pbyData = NULL;
	}

}

bool CJniRenderer::IsValid(CJniRenderer *p)
{
    // checks residency validity
    return g_registry.Contains(p);
}

CJniRenderer * CJniRenderer::CreateJniRenderer(JNIEnv *env, jobject jRenderer)
{
    USE_LOG;
	CJniRenderer *ret = NULL;
	env->PushLocalFrame(10);
	do
	{
	    jclass jcls = env->GetObjectClass(jRenderer);
	    if (jcls == NULL)
	    {
	        LOGE("get java class failed");
	        break;
	    }

	    jfieldID jfld = env->GetFieldID(jcls, FIELD_NAME, "I");
	    if (jfld == NULL)
	    {
	        LOGE("java field '%s' is not defined", FIELD_NAME);
	        break;
	    }

	    jmethodID jmtd = env->GetMethodID(jcls, METHOD_NAME, METHOD_SIG);
	    if (jmtd == NULL)
	    {
	        LOGE("java method 'void %s()' is not defined", METHOD_NAME);
	        break;
	    }

	    jint nValue = env->GetIntField(jRenderer, jfld);
	    if (nValue != 0 && g_registry.Contains((void*)nValue))
	    {
	        LOGI("the jni renderer is already created");
	        ret = (CJniRenderer*)nValue;
			break;
	    }

	    CJniRenderer * pNew = new CJniRenderer(env, jRenderer, jcls, jfld, jmtd);
	    if (!g_registry.Contains(pNew))
	    {
	        LOGE("create jni renderer failed");
	        delete pNew;
	        ret = NULL;
			break;
	    }
	    else
	    {
	        LOGD("jnirenderer instance allocated: %u", sizeof(CJniRenderer));
	    }

	    env->SetIntField(jRenderer, jfld, (jint)pNew);
		ret = pNew;

	    LOGI("create jni renderer ok");
	}while(0);
	env->PopLocalFrame(NULL);

    return ret;
}

void CJniRenderer::DestroyJniRenderer(CJniRenderer* &p)
{
    USE_LOG;

    if (p == NULL || !g_registry.Contains(p))
    {
        LOGE("invalid parameters");
        return;
    }

    JNIEnv *env = NULL;
    if (p->m_jvm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE("get JNIEnv failed");
        return;
    }
	env->PushLocalFrame(10);
	do
	{
	    jint nValue = env->GetIntField(p->m_jRenderer, p->m_jfld);
	    if (nValue != (jint)p)
	    {
	        LOGE("java field is changed improperly!");
	    }

	    p->m_pRenderer->Destroy();

	    env->SetIntField(p->m_jRenderer, p->m_jfld, 0);
	    delete p;
	    LOGD("jnirenderer instance freed: %u", sizeof(CJniRenderer));

	    LOGI("destroy jni renderer ok");
	}while(0);
	env->PopLocalFrame(NULL);
    p = NULL;
}

CJniRenderer * CJniRenderer::GetJniRenderer(JNIEnv *env, jobject jRenderer)
{
	CJniRenderer* ret = NULL;
	env->PushLocalFrame(10);
	do
	{
	    if (env == NULL || jRenderer == NULL)
	    {
	        LOGE("invalid parameters");
			break;
	    }

	    jclass jcls = env->GetObjectClass(jRenderer);
	    if (jcls == NULL)
	    {
	        LOGE("get java class failed");
	        break;
	    }

	    jfieldID jfld = env->GetFieldID(jcls, FIELD_NAME, "I");
	    if (jfld == NULL)
	    {
	        LOGE("java java field '%s' failed", FIELD_NAME);
	        break;
	    }

	    jint nValue = env->GetIntField(jRenderer, jfld);
	    if (nValue == 0
	            || !g_registry.Contains((CJniRenderer*)nValue))
	    {
	        //LOGE("get jni renderer from java object failed");
	        //return NULL;
	        LOGI("try to get a new jni renderer");
	        ret = CreateJniRenderer(env, jRenderer);
	    }
		else
		{
			ret = (CJniRenderer*)nValue;
		}

	}while(0);

	env->PopLocalFrame(NULL);
    return ret;
}

void CJniRenderer::start(JNIEnv *env, jobject jRenderer)
{
    int nErr;
    pthread_mutex_lock(&m_mutex);
	do{
	    if (m_bStarted)
	    {
	        break;
	    }

	    pthread_attr_t attr;
	    nErr = pthread_attr_init(&attr);
	    if (nErr)
	    {
	        LOGE("start failed");
	        break;
	    }

	    nErr = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	    if (nErr)
	    {
	        LOGE("start failed");
	        break;
	    }

	    nErr = pthread_create(&m_thread, &attr, ThreadWrapper, this);
	    if (nErr)
	    {
	        LOGE("start failed");
	        break;
	    }
	    else
	    {
	        LOGI("created jni thread");
	    }

	    pthread_attr_destroy(&attr);

	    m_bStop = FALSE;
	}while(0);
    pthread_mutex_unlock(&m_mutex);;
}

void CJniRenderer::stop()
{
    pthread_mutex_lock(&m_mutex);
    if (m_bStarted && !m_bStop)
    {
        m_bStop = TRUE;
        pthread_cond_signal(&m_cond);
    }
    pthread_mutex_unlock(&m_mutex);

    int retv = 0;
    pthread_join(m_thread, (void**)&retv);

    if (!g_registry.Contains(this))
    {
        return;
    }

    JNIEnv *env = NULL;
    if (m_jvm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE("get JNIEnv failed");
        return;
    }

    if (m_jRenderer != NULL)
    {
        env->DeleteGlobalRef(m_jRenderer);
        m_jRenderer = NULL;
        env->DeleteGlobalRef(m_jcls);
        m_jcls = NULL;
        m_jfld = NULL;
    }
}

//static
void* CJniRenderer::ThreadWrapper(void *pThis)
{
#ifndef _ANDROID_
    prctl(PR_SET_NAME, "CJniRenderer");
#else
    prctl(PR_SET_NAME, (unsigned long)"CJniRenderer",0,0,0);
#endif
    CJniRenderer * p = (CJniRenderer*) pThis;
    int nErr = p->ThreadEntry();
    return (void *) nErr;
}

int CJniRenderer::ThreadEntry()
{
    int nErr = 0;

    if (m_jvm == NULL)
    {
        LOGE("no java vm");
        return 1;
    }

    JNIEnv *env = NULL;

    // different jni.h defines AttachCurrentThread(...) that have different
    // formats, it needs a preprocessor to choose which is a better usage.
    // -- huangxuefeng on 2013/07/29
#ifdef JNI_H_ // use standard Android API jni.h
    nErr = m_jvm->AttachCurrentThread(&env, NULL);
#else // _JAVASOFT_JNI_H_ : use different version other than Android API std.
    nErr = m_jvm->AttachCurrentThread((void**)&env, NULL);
#endif // JNI_H_

    if (nErr < 0)
    {
        LOGE("attach thread to java vm failed.");
        return 1;
    }

    pthread_mutex_lock(&m_mutex);
    LOGI("jni renderer is started");
    m_bStarted = TRUE;
    while (!m_bStop)
    {
        while (!m_bStop && !m_bNewFrame)
        {
            pthread_cond_wait(&m_cond, &m_mutex);
        }
        if (!m_bStop)
        {
            pthread_mutex_unlock(&m_mutex);
            env->CallVoidMethod(m_jRenderer, m_jmtd); // [java] fireOnNewFrame()
            pthread_mutex_lock(&m_mutex);

            m_bNewFrame = FALSE;
        }
    }
    m_bStarted = FALSE;
    LOGI("jni renderer is stopped");
    pthread_mutex_unlock(&m_mutex);;

    nErr = m_jvm->DetachCurrentThread();
    return 0;
}

void CJniRenderer::SetFrame(unsigned char *pbyData, int nWidth, int nHeight)
{
	if(pbyData == NULL || nWidth <=0 || nHeight <= 0)
	{
	    LOGE("SetFrame Frame is invalid.");
		return;
	}
	if(m_pRenderer->GetPause())
	{
	    LOGE("Paused,do not Render.");
		return;
	}
    int dataSize = nWidth * nHeight * 3 / 2;
    memcpy(m_tVidFrame.pbyData, pbyData, dataSize);
    m_tVidFrame.nWidth = nWidth;
    m_tVidFrame.nHeight = nHeight;

    m_pRenderer->SetFrame(m_tVidFrame);

    pthread_mutex_lock(&m_mutex);
    m_bNewFrame = TRUE;
    pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);

//    // save one remote frame for test
//    {
//        static int nFrameIndex = 0;
//        if (nFrameIndex % 10 == 0)
//        {
//            char strPath[128];
//            sprintf(strPath, "/sdcard/DCIM/r_%dx%d.yuv", nWidth, nHeight);
//            CAutoFile fileYuv(strPath, "wa");
//            fileYuv.write(pbyData, nWidth * nHeight * 1.5);
//            LOGD("saved file %s", strPath);
//        }
//        ++ nFrameIndex;
//    }
}

void CJniRenderer::SetFrame(unsigned char *pbyData, int nWidth, int nHeight,
        int nFrameDirection, bool bIsFrontCamera,
        unsigned long long uTimestamp)
{
	if(pbyData == NULL || nWidth <=0 || nHeight <= 0)
	{
	    LOGE("SetFrame Frame is invalid.");
		return;
	}
	if(m_pRenderer->GetPause())
	{
	    LOGE("Paused,do not Render.");
		return;
	}
    int dataSize = nWidth * nHeight * 3 / 2;
    memcpy(m_tVidFrame.pbyData, pbyData, dataSize);
    m_tVidFrame.nWidth = nWidth;
    m_tVidFrame.nHeight = nHeight;
    m_tVidFrame.uTimestamp = uTimestamp;
    m_pRenderer->SetFrame(m_tVidFrame, nFrameDirection, bIsFrontCamera);

    pthread_mutex_lock(&m_mutex);
    m_bNewFrame = TRUE;
    pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);;
}

void CJniRenderer::Draw()
{
    m_pRenderer->Draw();
}

void CJniRenderer::Init(int nWidth, int nHeight)
{
    m_pRenderer->Init(nWidth, nHeight);
}

void CJniRenderer::SetChannel(int nChannel)
{
    USE_LOG;
    LOGD("channel#%d", nChannel);
/*
    CVidGlobalData *pVidShared = CVidGlobalData::Instance();
    CJniRenderer *pChangedOtherRenderer = NULL;
    BEGIN_AUTOLOCK(&pVidShared->m_DecPlylock);
    if (pVidShared->m_pRenderer[nChannel] == this)
    {
        return;
    }

    for (int i=0; i<kRendererMax; ++i)
    {
        if (i == nChannel)
        {
            // change channel
            CJniRenderer *pRenderer = (CJniRenderer*)pVidShared->m_pRenderer[i];
            if (pRenderer != this)
            {
                // before apply change, swap frame at first.
                if (CJniRenderer::IsValid(pRenderer))
                {
                    LOGD("swap channel frames.");
                    m_pRenderer->SwapFrame(*pRenderer->m_pRenderer);
                    pChangedOtherRenderer = pRenderer;
                }
                pVidShared->m_pRenderer[i] = this;
            }
        }
        else if (this == pVidShared->m_pRenderer[i])
        {
            pVidShared->m_pRenderer[i] = NULL;
        }
    }
    m_pRenderer->SetChannel(nChannel);

    // configuration
    // TODO (xiezhigang 2013-12-16) configure channel-buffers better
    TRendererSettings tSettings;
    m_pRenderer->QuerySettings(&tSettings);
    TVidFrame tFrame = {0};
    switch (nChannel)
    {
    case kRendererAux:
        tSettings.kAspectRatio = kAspRatPadding;
        tSettings.nFlip = 0;
        tFrame.pbyData = pVidShared->m_tVidDecSecond.picture_bufsrc;
        tFrame.nWidth  = pVidShared->m_tVidDecSecond.sourceWidth;
        tFrame.nHeight = pVidShared->m_tVidDecSecond.sourceHeight;
        m_pRenderer->SetFrame(tFrame);
        m_bNewFrame = TRUE;
        break;
    case kRendererMain:
        tSettings.kAspectRatio = kAspRatPadding;
        tSettings.nFlip = 0;
        tFrame.pbyData = pVidShared->m_tVidDecFirst.picture_bufsrc;
        tFrame.nWidth  = pVidShared->m_tVidDecFirst.sourceWidth;
        tFrame.nHeight = pVidShared->m_tVidDecFirst.sourceHeight;
        m_pRenderer->SetFrame(tFrame);
        m_bNewFrame = TRUE;
        break;
    case kRendererPreview:
        tSettings.kAspectRatio = kAspRatTrim;
        tSettings.nFlip = 1;
        break;
    }
    LOGV("frame-data[0]:%d", (tFrame.pbyData?tFrame.pbyData[0]:1));
    m_pRenderer->UpdateSettings(tSettings);

    END_AUTOLOCK;

    Refresh();
    if (pChangedOtherRenderer)
    {
        pChangedOtherRenderer->Refresh();
    }
    */
}

void CJniRenderer::Refresh()
{
    m_bNewFrame = TRUE;
    pthread_cond_signal(&m_cond);
}


} // namespace ants360

