#ifndef _ANTS360_JNIRENDERER_IMPL_H_
#define _ANTS360_JNIRENDERER_IMPL_H_

#include <jni.h>
#include <pthread.h>

#include "renderer.h"

namespace ants360 {

namespace
    {
        enum KeyRendererIndex
        {
            kRendererMain,    // the main renderer
            kRendererAux,     // the auxiliary renderer
            kRendererPreview, // the preview renderer for video capture
            kRendererMax,     // the max index should not exceed
        };
    }
class CJniRenderer
{
public:
    static CJniRenderer * CreateJniRenderer(JNIEnv *env, jobject jRenderer);
    static CJniRenderer * GetJniRenderer(JNIEnv *env, jobject jRenderer);
    static void DestroyJniRenderer(CJniRenderer* &p);
    static bool IsValid(CJniRenderer *p);
    void Draw();
    void Init(int nWidth, int nHeight);

    inline void SetPause(BOOL32 isPause);
    inline bool GetPause();
    inline void SetRotate(float fDegree);
    inline float GetRotate();
    inline void SetTranslate(float fTranslateX, float fTranslateY, float fTranslateZ);
    inline float GetTranslate(int nTranslateEnum);
    inline void SetScale(float fScaleX, float fScaleY, float fScaleZ);
    inline float GetScale(int nScaleEnum);
    inline void SetDirection(int nDir);
    inline int GetDirection();
    inline void SetFlip(int nFlip);
    inline int GetFlip();
    inline bool IsEmptyFrame();

    void SetFrame(unsigned char *pbyData, int nWidth, int nHeight);
    void SetFrame(unsigned char *pbyData, int nWidth, int nHeight,
            int nFrameDirection,      // recommendation for display orientation
            bool bIsFrontCamera,      // whether is captured from front camera
            unsigned long long uTimestamp); // the timestamp of capture this frame
    void SetChannel(int nChannel);
    /*!
     * \brief   set the aspect state which indicates how to keep aspect ratio.
     * \param   nState binary format:
     *           _0 - keep viewport dimension
     *           _1 - transpose viewport dimension
     *           0_ - keep frame dimension
     *           1_ - transpose frame dimension
     * \author  huangxuefeng
     * \date    2013-11-15
     */
    inline void SetAspectState(int nState);
    inline int GetAspectState();
    void Refresh();
    /*!
     * \brief   whether keep the aspect ratio or not.
     * \author  xie zhigang
     * \date    2013-11-08
     */
    inline void KeepAspectRatio(EMAspectRatio mode);
    inline void QuerySettings(TRendererSettings* settings);
    inline void UpdateSettings(const TRendererSettings & settings);

private:
    CJniRenderer(JNIEnv *env, jobject jRenderer,
            jclass jcls, jfieldID jfld, jmethodID jmtd);
    ~CJniRenderer();
    void start(JNIEnv *env, jobject jRenderer);
    void stop();
    static void *ThreadWrapper(void *pThis);
    int ThreadEntry();

    static const char * const FIELD_NAME;
    static const char * const METHOD_NAME;
    static const char * const METHOD_SIG;

    JavaVM          *m_jvm;
    jobject          m_jRenderer;
    CRenderer       *m_pRenderer;
    jclass           m_jcls;
    jfieldID         m_jfld;
    jmethodID        m_jmtd;
    pthread_mutex_t  m_mutex;
    pthread_cond_t   m_cond;
    pthread_t        m_thread;
    bool             m_bStarted;
    bool             m_bStop;
    bool             m_bNewFrame;
    TVidFrame        m_tVidFrame;

};


/*
 * inline member functions implementation
 */

inline void CJniRenderer::SetPause(BOOL32 isPause)
{
    m_pRenderer->SetPause(isPause);
}

inline bool CJniRenderer::GetPause()
{
    return m_pRenderer->GetPause();
}

inline void CJniRenderer::SetRotate(float fDegree)
{
    m_pRenderer->SetRotate(fDegree);
}

inline float CJniRenderer::GetRotate()
{
    return m_pRenderer->GetRotate();
}

inline void CJniRenderer::SetTranslate(float fTranslateX, float fTranslateY, float fTranslateZ)
{
    m_pRenderer->SetTranslate(fTranslateX, fTranslateY, fTranslateZ);
}

inline float CJniRenderer::GetTranslate(int nTranslateEnum)
{
    return m_pRenderer->GetTranslate((ants360::EMDirect)nTranslateEnum);
}

inline void CJniRenderer::SetScale(float fScaleX, float fScaleY, float fScaleZ)
{
    m_pRenderer->SetScale(fScaleX, fScaleY, fScaleZ);
}

inline float CJniRenderer::GetScale(int nScaleEnum)
{
    return m_pRenderer->GetScale((ants360::EMDirect)nScaleEnum);
}

inline void CJniRenderer::SetDirection(int nDir)
{
    m_pRenderer->SetDirection(nDir);
}

inline int CJniRenderer::GetDirection()
{
    return m_pRenderer->GetDirection();
}

inline void CJniRenderer::SetFlip(int nFlip)
{
    m_pRenderer->SetFlip(nFlip);
}

inline int CJniRenderer::GetFlip()
{
    return m_pRenderer->GetFlip();
}

// huangxuefeng 2013-11-08
inline void CJniRenderer::KeepAspectRatio(EMAspectRatio mode)
{
    m_pRenderer->KeepAspectRatio(mode);
}

// huangxuefeng 2013-11-15
inline void CJniRenderer::SetAspectState(int nState)
{
    m_pRenderer->SetAspectState(nState);
}

// huangxuefeng 2013-11-15
inline int CJniRenderer::GetAspectState()
{
    return m_pRenderer->GetAspectState();
}

// huangxuefeng 2013-11-18
inline void CJniRenderer::QuerySettings(TRendererSettings* settings)
{
    m_pRenderer->QuerySettings(settings);
}

// huangxuefeng 2013-11-18
inline void CJniRenderer::UpdateSettings(const TRendererSettings & settings)
{
    m_pRenderer->UpdateSettings(settings);
}

// huangxuefeng 2013-12-19
inline bool CJniRenderer::IsEmptyFrame()
{
    return m_pRenderer->IsEmptyFrame();
}


} // namespace ants360

#endif // _ANTS360_JNIRENDERER_IMPL_H_