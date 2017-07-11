/*******************************************************************************
 *        Module: renderer
 *          File: renderer.h
 * Functionality: rendering video onto opengl context.
 *       Related: GLES2
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2013 360ANTS, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2013-07-12  v1.0        huangxuefeng  created
 * 2014-01-19  v1.1        huangxuefeng  aggregates render-objects
 ******************************************************************************/
#ifndef _ANTS360_RENDERER_H_
#define _ANTS360_RENDERER_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <pthread.h>

#include "ants360typedefs.h"
#include "renderframe.h"
#include "renderbackground.h"

namespace ants360 {

/*!
 * \brief   video frame used in CRenderer.
 * \author  huangxuefeng
 * \date    2013-07-25
 */
typedef struct tagVidFrame
{
    //! width of frame, zero or positive is acceptable.
    s32 nWidth;
    //! height of frame, zero or positive is acceptable.
    s32 nHeight;
    /*!
     * \brief   frame data of a continuous trunk, or null.
     *          null means an invalid frame, otherwise, it must bearing data of
     *             nWidth x nHeight x 1.5
     */
    u8 *pbyData;
    //! time-stamp of this frame is captured.
    u64 uTimestamp;
} TVidFrame;

/*!
 * \brief   numeration of directions for x-axial.
 * \author  huangxuefeng
 * \date    2013-07-23
 */
typedef enum {
    kDirEast,
    kDirNorth,
    kDirWest,
    kDirSouth,
} EMDirection;

/*!
 * \brief   numeration of flips for frame.
 * \author  huangxuefeng
 * \date    2013-07-23
 */
typedef enum {
    kFlipNone,
    kFlipHorizontal,
    kFlipVertical,
    kFlipBoth,
} EMFlip;

/*!
 * \brief   settings for rendering a frame.
 * \author  huangxuefeng
 * \date    2013-07-23
 */
typedef struct {
    //! rendering direction to layout frames.
    s32 nDirection;
    //! flip modes.
    s32 nFlip;
    //! aspect ratio mode for rendering frames.
    EMAspectRatio kAspectRatio;
    //! how to scale the frames for keep correct aspect ratio.
    s32 nAspectState;
} TRendererSettings;

/*!
 * \brief   renderer of OpenGLES for YUV420Planar frames.
 * \note    it dependents GLContext created in java by GLSurfaceView.
 *          currently, it only support rendering YUV420P frames.
 * \author  huangxuefeng
 * \date    2013-07-12
 */
class CRenderer
{
public:
    CRenderer();
    ~CRenderer();

    void Draw();
    void Init(s32 nWidth, s32 nHeight);
    void Destroy();
    void SetPause(BOOL32 bIsPause);
    void SetFrame(TVidFrame &tFrame);
    void SetFrame(TVidFrame &tFrame, s32 nDir, BOOL32 bIsFrontCamera);
    void SetRotate(float fDegree);
    void SetTranslate(float fTranslateX, float fTranslateY, float fTranslateZ);
    void SetScale(float fScaleX, float fScaleY, float fScaleZ);
    void SetDirection(s32 nDir);
    /*!
     * \brief   sets flip modes for rendering frames.
     * \param  nFlip
     *          0 - none
     *          1 - horizontal flip
     * \author huangxuefeng
     * \date   2014-03-19
     */
    void SetFlip(s32 nFlip);
    void SetAspectState(s32 nState);

    /*!
     * \brief swap frame and related settings with another renderer.
     * \author huangxuefeng
     * \date   2013-11-07
     */
    void SwapFrame(CRenderer &renderer);

    /*!
     * \brief whether keep aspect ratio or not
     * \author huangxuefeng
     * \date   2013-11-08
     */
    void KeepAspectRatio(EMAspectRatio kAspRat);

    inline BOOL32 GetPause();
    inline s32 GetFlip();
    inline s32 GetDirection();
    inline s32 GetAspectState();
    inline float GetRotate();
    inline float GetTranslate(EMDirect kTrans);
    inline float GetScale(EMDirect kScale);
    inline BOOL32 IsEmptyFrame();
    inline void SetChannel(s32 nChannel);
    inline s32  GetChannel();

    void QuerySettings(TRendererSettings* settings);
    void UpdateSettings(const TRendererSettings & settings);

private:
    s32      m_nViewportWidth;   //!< viewport width
    s32      m_nViewportHeight;  //!< viewport height
    s32      m_nWidth;           //!< display frame width
    s32      m_nHeight;          //!< display frame height
    BOOL32   m_bInited;
    BOOL32   m_bPaused;
    s32      m_nDirection;       //!< rotate the image, vertice will be changed.
    s32      m_nFrameDirection;  //!< direction of drawing frames.
    BOOL32   m_bIsFrontCamera;   //!< whether is preview for front camera.
    s32      m_nFlip;            //!< flip the image, vertice will be changed.
    EMAspectRatio m_kAspectRatio;//!< aspect ratio mode.
    s32      m_nAspectState;     //!< indicate how to keep aspect ratio.
                                 //   binary format:
                                 //    _0 - keep viewport dimension
                                 //    _1 - transpose viewport dimension
                                 //    0_ - keep frame dimension
                                 //    1_ - transpose frame dimension
    s32      m_nChannel;         //!< channel index.
    TVidFrame          m_tFrame;
    CRenderFrame       m_rendFrame;
    CRenderBackground  m_rendBackground; //!< rendering background
    BOOL32             m_bRedrawBkground;//!< whether needs redraw background
    float              m_fDegree;
    float              m_fTranslate[kDirectEnd];
    float              m_fScale[kDirectEnd];
    ILock*             m_pLock;
};

inline float CRenderer::GetRotate()
{
    return m_fDegree;
}

inline float CRenderer::GetTranslate(EMDirect kTrans)
{
    return m_fTranslate[kTrans];
}

inline float CRenderer::GetScale(EMDirect kScale)
{
    return m_fScale[kScale];
}

inline BOOL32 CRenderer::GetPause()
{
    return m_bPaused;
}

inline s32 CRenderer::GetFlip()
{
    return m_nFlip;
}

inline s32 CRenderer::GetDirection()
{
    return m_nDirection;
}

inline s32 CRenderer::GetAspectState()
{
    return m_nAspectState;
}

inline BOOL32 CRenderer::IsEmptyFrame()
{
    return m_tFrame.pbyData == 0
        || m_tFrame.nWidth == 0
        || m_tFrame.nHeight == 0;
}

inline void CRenderer::SetChannel(s32 nChannel)
{
    m_nChannel = nChannel;
    m_bRedrawBkground = TRUE; // needs redraw the background.
}

inline s32 CRenderer::GetChannel()
{
    return m_nChannel;
}


} // namespace ants360

#endif // _ANTS360_RENDERER_H_