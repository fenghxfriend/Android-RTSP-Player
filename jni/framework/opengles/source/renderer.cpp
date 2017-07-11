/*******************************************************************************
 *        Module: renderer
 *          File: renderer.cpp
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
#include <stdlib.h>
#include <sys/time.h>

#include "androidlog.h"
#include "autolock.h"
#include "renderer.h"
#include "version.h"

#define YUV420P_NUM_1080P 1080*1920*3/2

namespace ants360 {


CRenderer::CRenderer()
    : m_nViewportWidth (0),
      m_nViewportHeight(0),
      m_nWidth(0),
      m_nHeight(0),
      m_bInited(FALSE),
      m_bPaused(TRUE),
      m_nDirection(0),
      m_fDegree(0.0f),
      m_nFlip(0),
      m_nFrameDirection(0),
      m_bIsFrontCamera(TRUE),
      m_kAspectRatio(kAspRatStretch),
      m_nAspectState(0),
      m_nChannel(0),
      m_bRedrawBkground(TRUE)
{
    memset(&m_tFrame, 0, sizeof(m_tFrame));
	memset(m_fTranslate, 0, sizeof(m_fTranslate));
	memset(m_fScale, 0, sizeof(m_fScale));
    m_pLock = new CLock;
}

CRenderer::~CRenderer()
{
    Destroy();
    delete m_pLock;
}

void CRenderer::Init(s32 nWidth, s32 nHeight)
{
    LOGV("channel#%d: viewport dimension: %d x %d", m_nChannel, nWidth, nHeight);
	BEGIN_AUTOLOCK(m_pLock);
    m_nViewportWidth = nWidth;
    m_nViewportHeight = nHeight;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_bRedrawBkground = TRUE;
/*
	if(m_tFrame.pbyData == NULL)
	{

		m_tFrame.pbyData = (unsigned char *)malloc(YUV420P_NUM_1080P);
			if(m_tFrame.pbyData == NULL)
			{
				LOGE("memory is not enough: error");
		        return ;
			}
	}
*/
    if (nWidth <= 0 || nHeight <= 0)
    {
        LOGE("channel#%d: error", m_nChannel);
        return ;
    }

    glViewport(0, 0, m_nViewportWidth, m_nViewportHeight);

	if(m_bInited)
	{
		LOGE("channel#%d: already inited", m_nChannel);
		m_rendFrame.destroy();
	    m_rendBackground.destroy();

	    //m_bInited = FALSE;
		//m_bPaused = TRUE;
	}

    // init background rendering
    m_bInited = m_rendBackground.init();
    if (!m_bInited)
    {
        LOGE("channel#%d: error", m_nChannel);
        return;
    }

    // init frame rendering
    m_bInited = m_rendFrame.init();
    if (!m_bInited)
    {
        LOGE("channel#%d: error", m_nChannel);
        return;
    }

    glUseProgram(0);

    m_bInited = TRUE;
	m_bPaused = FALSE;
    END_AUTOLOCK;
}

void CRenderer::Destroy()
{
    BEGIN_AUTOLOCK(m_pLock);
    if (!m_bInited)
    {
        return;
    }
/*
	if(m_tFrame.pbyData != NULL)
	{
		free(m_tFrame.pbyData);
	}
*/
/*
    m_rendFrame.destroy();
    m_rendBackground.destroy();
*/
    m_bInited = FALSE;
	m_bPaused = TRUE;

    END_AUTOLOCK;
}

void CRenderer::Draw()
{
    BEGIN_AUTOLOCK(m_pLock);
    if (!m_bInited)
    {
        LOGE("channel#%d: not already initialized", m_nChannel);
        return ;
    }

	if (m_bPaused)
    {
        LOGE("channel#%d: paused", m_nChannel);
        return ;
    }

    // clear for rendering
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // draw Background
#if 0
    if (m_bRedrawBkground)
    {
        m_rendBackground.draw();
        m_bRedrawBkground = FALSE; // draws background only once.
    }
#else
    // there is a bug: after swap frame of channels, the background is transperent.
    // -- huangxuefeng, 2014-03-26
    m_rendBackground.draw();
#endif

    // draw frame
    if (m_tFrame.pbyData == NULL
            || m_tFrame.nWidth == 0 || m_tFrame.nHeight == 0)
    {
        LOGW("channel#%d: gets picture buffer failed: frame@%p(%d x %d)",
                m_nChannel, m_tFrame.pbyData, m_tFrame.nWidth, m_tFrame.nHeight);
        m_rendBackground.draw(); // always draw background to clear the screen.
        return;
    }
    u8 *pbyFrame = m_tFrame.pbyData;
    m_nWidth = m_tFrame.nWidth;
    m_nHeight = m_tFrame.nHeight;
    s32 uvStride = (m_nWidth + 1) / 2;
    m_rendFrame.set_view(m_nViewportWidth, m_nViewportHeight);
    m_rendFrame.set_channel(m_nChannel);
    m_rendFrame.set_frameDir(m_nFrameDirection);
    m_rendFrame.set_direction(m_nDirection);
	m_rendFrame.set_rotate(m_fDegree);
	m_rendFrame.set_scale(m_fScale);
	m_rendFrame.set_translate(m_fTranslate);
    m_rendFrame.set_flip(m_nFlip);
    m_rendFrame.set_aspectState(m_nAspectState);
    m_rendFrame.set_aspectRatioMode(m_kAspectRatio);
    m_rendFrame.set_frontCamera(m_bIsFrontCamera);
    m_rendFrame.draw(
            pbyFrame, m_tFrame.nWidth,
            pbyFrame + m_nWidth * m_nHeight, uvStride,
            pbyFrame + m_nWidth * m_nHeight + uvStride * m_nHeight / 2, uvStride,
            m_tFrame.nWidth, m_tFrame.nHeight);

    END_AUTOLOCK;
}

void CRenderer::SetPause(BOOL32 bIsPause)
{
    LOGD("channel#%d: status: %d", m_nChannel, bIsPause);
    BEGIN_AUTOLOCK(m_pLock);
	m_bPaused = bIsPause;
    END_AUTOLOCK;
}

void CRenderer::SetFrame(TVidFrame &tFrame)
{
    LOGD("channel#%d: size: %dx%d", m_nChannel, tFrame.nWidth, tFrame.nHeight);
    BEGIN_AUTOLOCK(m_pLock);
	m_tFrame = tFrame;
	/*
	memcpy(m_tFrame.pbyData,tFrame.pbyData,tFrame.nWidth*tFrame.nHeight*3/2);
    m_tFrame.nWidth = tFrame.nWidth;
    m_tFrame.nHeight = tFrame.nHeight;
    */
    if (m_nFrameDirection != 0)
    {
        m_nFrameDirection = 0;
    }
    END_AUTOLOCK;
}

void CRenderer::SetFrame(TVidFrame &tFrame, s32 nDir, BOOL32 bIsFrontCamera)
{
    LOGD("channel#%d: camera:%s, dir:%d, size: %dx%d", m_nChannel,
            (bIsFrontCamera?"front":"rear"), nDir, tFrame.nWidth, tFrame.nHeight);
    BEGIN_AUTOLOCK(m_pLock);
    if (m_nFrameDirection != nDir
            || m_bIsFrontCamera != bIsFrontCamera
            || tFrame.nWidth != m_tFrame.nWidth
            || tFrame.nHeight != m_tFrame.nHeight)
    {
        m_bIsFrontCamera = bIsFrontCamera;
        m_nFrameDirection = nDir;
        m_bRedrawBkground = TRUE;
    }
	m_tFrame = tFrame;
	/*
    memcpy(m_tFrame.pbyData,tFrame.pbyData,tFrame.nWidth*tFrame.nHeight*3/2);
    m_tFrame.nWidth = tFrame.nWidth;
    m_tFrame.nHeight = tFrame.nHeight;
	m_tFrame.uTimestamp = tFrame.uTimestamp;
	*/
    END_AUTOLOCK;
}

void CRenderer::SetDirection(s32 nDir)
{
    BEGIN_AUTOLOCK(m_pLock);
    if (nDir != m_nDirection && nDir >= 0 && nDir < 4)
    {
        m_nDirection = nDir;
        m_bRedrawBkground = TRUE;
    }
    END_AUTOLOCK;
}

void CRenderer::SetRotate(float fDegree)
{
    BEGIN_AUTOLOCK(m_pLock);
    if (fDegree != m_fDegree)
    {
    	if(fDegree < 0.0f)
    	{
    	    fDegree += 360.0f;
    	}
		else if(fDegree > 360.0f)
		{
		    fDegree -= 360.0f;
		}
        m_fDegree = fDegree;
        m_bRedrawBkground = TRUE;
    }
    END_AUTOLOCK;
}

void CRenderer::SetTranslate(float fTranslateX, float fTranslateY, float fTranslateZ)
{
    BEGIN_AUTOLOCK(m_pLock);
    if (fTranslateX != m_fTranslate[kDirectX]|| fTranslateY != m_fTranslate[kDirectY] || fTranslateZ != m_fTranslate[kDirectZ])
    {
        m_fTranslate[kDirectX]= fTranslateX;
		m_fTranslate[kDirectY]= fTranslateY;
		m_fTranslate[kDirectZ]= fTranslateZ;
        m_bRedrawBkground = TRUE;
    }
    END_AUTOLOCK;
}

void CRenderer::SetScale(float fScaleX, float fScaleY, float fScaleZ)
{

    BEGIN_AUTOLOCK(m_pLock);
    if (fScaleX!= m_fScale[kDirectX]|| fScaleY != m_fScale[kDirectY] || fScaleZ != m_fScale[kDirectZ])
    {
        m_fScale[kDirectX]= fScaleX;
		m_fScale[kDirectY]= fScaleY;
		m_fScale[kDirectZ]= fScaleZ;
        m_bRedrawBkground = TRUE;
    }
    END_AUTOLOCK;
}


void CRenderer::SetFlip(s32 nFlip)
{
    BEGIN_AUTOLOCK(m_pLock);
    if (nFlip != m_nFlip && nFlip >= 0 && nFlip < 4)
    {
        m_nFlip = nFlip;
        m_bRedrawBkground = TRUE;
    }
    END_AUTOLOCK;
}

// huangxuefeng 2013-11-07
void CRenderer::SwapFrame(CRenderer &renderer)
{
    BEGIN_AUTOLOCK(m_pLock);
    BEGIN_AUTOLOCK(renderer.m_pLock);

    if (renderer.m_bInited && m_bInited)
    {
        // swap state
        TVidFrame  tFrame            =  renderer.m_tFrame;
        BOOL32     bIsFrontCamera    =  renderer.m_bIsFrontCamera;
        s32        nFrameDirection   =  renderer.m_nFrameDirection;
        s32        nDirection        =  renderer.m_nDirection;
        s32        nFlip             =  renderer.m_nFlip;
        EMAspectRatio kAspectRatio   =  renderer.m_kAspectRatio;
        s32        nAspectState      =  renderer.m_nAspectState;
        s32        nChannel          =  renderer.m_nChannel;
		float      fDegree           =  renderer.m_fDegree;
		float      fScale[kDirectEnd]         =  {0.0f,0.0f,0.0f};
		float      fTranslate[kDirectEnd]     =  {0.0f,0.0f,0.0f};
		memcpy(fScale, renderer.m_fScale, sizeof(float) * kDirectEnd);
		memcpy(fTranslate, renderer.m_fTranslate, sizeof(float) * kDirectEnd);

        renderer.m_tFrame            =  m_tFrame;
        renderer.m_bIsFrontCamera    =  m_bIsFrontCamera;
        renderer.m_nFrameDirection   =  m_nFrameDirection;
        renderer.m_nDirection        =  m_nDirection;
        renderer.m_nFlip             =  m_nFlip;
        renderer.m_kAspectRatio      =  m_kAspectRatio;
        renderer.m_nAspectState      =  m_nAspectState;
        renderer.m_nChannel          =  m_nChannel;
		renderer.m_fDegree           =  m_fDegree;
		memcpy(renderer.m_fScale, fScale, sizeof(float) * kDirectEnd);
		memcpy(renderer.m_fTranslate, fTranslate, sizeof(float) * kDirectEnd);

        m_tFrame            =  tFrame;
        m_bIsFrontCamera    =  bIsFrontCamera;
        m_nFrameDirection   =  nFrameDirection;
        m_nDirection        =  nDirection;
        m_nFlip             =  nFlip;
        m_kAspectRatio      =  kAspectRatio;
        m_nAspectState      =  nAspectState;
        m_nChannel          =  nChannel;
		m_fDegree           =  fDegree;
		memcpy(m_fScale, fScale, sizeof(float) * kDirectEnd);
		memcpy(m_fTranslate, fTranslate, sizeof(float) * kDirectEnd);

        // both needs redraw background
        renderer.m_bRedrawBkground = TRUE;
        m_bRedrawBkground = TRUE;
    }

    END_AUTOLOCK;
    END_AUTOLOCK;
}

// huangxuefeng 2013-11-08
void CRenderer::KeepAspectRatio(EMAspectRatio kAspRat)
{
    BEGIN_AUTOLOCK(m_pLock);
    if (m_kAspectRatio != kAspRat)
    {
        LOGI("channel#%d: keepAsp:%d", m_nChannel, kAspRat);
        m_kAspectRatio = kAspRat;
        m_bRedrawBkground = TRUE;
    }
    END_AUTOLOCK;
}

void CRenderer::SetAspectState(s32 nState)
{
    BEGIN_AUTOLOCK(m_pLock);
    if (m_nAspectState != (nState & 3))
    {
        LOGI("channel#%d: aspState:%d", m_nChannel, nState);
        m_nAspectState = nState & 3;
        m_bRedrawBkground = TRUE;
    }
    END_AUTOLOCK;
}

void CRenderer::QuerySettings(TRendererSettings* settings)
{
    if (settings)
    {
        memset(settings, 0, sizeof(TRendererSettings));
        settings->nDirection = m_nDirection;
        settings->nFlip = m_nFlip;
        settings->kAspectRatio = m_kAspectRatio;
        settings->nAspectState = m_nAspectState;
    }
}

void CRenderer::UpdateSettings(const TRendererSettings & settings)
{
    LOGV("channel#%d, direction: %d, flip: %d, aspRat: %d, aspState: %d",
            m_nChannel, settings.nDirection, settings.nFlip,
            settings.kAspectRatio, settings.nAspectState);

    BEGIN_AUTOLOCK(m_pLock);
    BOOL32 bDirty = FALSE;

    if (m_nDirection != settings.nDirection)
    {
        m_nDirection = settings.nDirection;
        bDirty = TRUE;
    }

    if (m_nFlip != settings.nFlip)
    {
        m_nFlip = settings.nFlip % 4;
        bDirty = TRUE;
    }

    if (settings.kAspectRatio != kAspRatUnknown
            && m_kAspectRatio != settings.kAspectRatio)
    {
        m_kAspectRatio = settings.kAspectRatio;
        bDirty = TRUE;
    }

    if (m_nAspectState != settings.nAspectState)
    {
        m_nAspectState = settings.nAspectState & 3;
        bDirty = TRUE;
    }

    m_bRedrawBkground = m_bRedrawBkground || bDirty;

    END_AUTOLOCK;
}


} // namespace ants360

