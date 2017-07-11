/*******************************************************************************
 *        Module: renderframe
 *          File: renderframe.h
 * Functionality: rendering an i420 frame onto opengl context.
 *       Related: GLES2
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2013 360ANTS, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2014-01-19  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _ANTS360_RENDERERFRAME_H_
#define _ANTS360_RENDERERFRAME_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "ants360typedefs.h"

namespace ants360 {


/*!
 * \brief   keys for aspect ratio mode
 * \author  huangxuefeng
 * \date    2014-03-26
 */
typedef enum {
    //! unknown aspect ratio, just keep previous ratio
    kAspRatUnknown = -1,
    //! no aspect ratio is kept, stretch to fill.
    kAspRatStretch = 0,
    //! use trim margin strategy to keep aspect ratio
    kAspRatTrim,
    //! use padding margin strategy to keep aspect ratio
    kAspRatPadding,
} EMAspectRatio;

typedef enum {
    kDirectX,
    kDirectY,
    kDirectZ,
    kDirectEnd,
} EMDirect;

class CRenderFrame
{
public:
    CRenderFrame();
    ~CRenderFrame();

    bool init();
    void destroy();

    void set_view(int width, int height);
    void set_channel(int channel);
    void set_frameDir(int frameDir);
    void set_direction(int dir);
    void set_flip(int flip);
    void set_scale(float *scale);
    void set_rotate(float degree);
    void set_translate(float *translate);
    void set_aspectState(int astate);
    //! Set aspect ratio strategy for rendering frames.
    void set_aspectRatioMode(EMAspectRatio kAspRat);
    void set_frontCamera(bool front);

    void draw(
        const unsigned char * py, int ystride,
        const unsigned char * pu, int ustride,
        const unsigned char * pv, int vstride,
        int width, int height);

private:
    bool update_vertice(int frame_width, int frame_height);
    void rotate_scale_translate();
    int m_viewWidth;
    int m_viewHeight;
    int m_channel;
    int m_frameWidth;
    int m_frameHeight;
    int m_frameDir;
    bool m_isFrontCam;
    int m_dir;
    int m_flip;
    int m_aspectState;
    EMAspectRatio m_kAspectRatio;
    bool m_dirty;

    GLfloat m_degree;
    GLfloat m_scale[3];
    GLfloat m_translate[3];
    GLfloat  m_vertice[20];      //!< vertice needed by opengl to map the image.

    // uniform index
    enum {
        UNIFORM_YTEXTURE,
        UNIFORM_UTEXTURE,
        UNIFORM_VTEXTURE,
        UNIFORM_TRANSLATE,
        UNIFORM_SCALE,
        UNIFORM_DEGREE,
        NUM_UNIFORMS
    };

    // attribute index
    enum {
        ATTRIB_VERTEX,
        ATTRIB_TEXCOORD,
        NUM_ATTRIBUTES
    };

    GLuint m_glProgram;
    GLuint m_glVertexShader;
    GLuint m_glFragmentShader;

    GLuint m_glYTextureID;
    GLuint m_glUTextureID;
    GLuint m_glVTextureID;
    GLuint m_glVertexBuffer;
    GLuint m_glIndexBuffer;
    GLint  m_glUniforms[NUM_UNIFORMS];
    GLint  m_glMatrix;
    GLint  m_glAttributes[NUM_ATTRIBUTES];
    GLfloat m_scaleMatrix[4 * 4];
    GLfloat m_translateMatrix[4 * 4];
};


} // namespace ants360

#endif // _ANTS360_RENDERERFRAME_H_