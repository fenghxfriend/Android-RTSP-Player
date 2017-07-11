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
 * 2014-01-19  v1.0        huangxuefeng  created
 ******************************************************************************/
#include <stdlib.h>

#include "androidlog.h"
#include "geometry.h"
#include "renderframe.h"
#include "shader.h"
#include "version.h"

namespace ants360 {


// template points map:
//
//              A (y)
//              :
//      |       :               (s)
// -- T0(P0)----+---- T1(P1) ----->
//      |       :        |
//      |       :        |      (x)
// - - -|- - - -+- - - - + - - - ->
//      |       :        |
//      |       :        |
//    T3(P3) ---+---  T2(P2)
//      |       :
//      |
//      V (t)
//
// position(x, y, z) and texturecoord(s, t)
const GLfloat vertice[] =
{
    //       position          |    texture
    // x        y       z      |     s      t
    -1.0f,    1.0f,   0.0f, /* | */ 0.0f,  0.0f,
     1.0f,    1.0f,   0.0f, /* | */ 1.0f,  0.0f,
     1.0f,   -1.0f,   0.0f, /* | */ 1.0f,  1.0f,
    -1.0f,   -1.0f,   0.0f, /* | */ 0.0f,  1.0f,
};

typedef Dimension<s32> dimension_t;
typedef Dimension<GLfloat> dimensionf_t;
typedef Point<s32> point_t;
typedef Point<GLfloat> pointf_t;
typedef Quadrangle<GLfloat> quadranglef_t;

void calc_interest_of_aspect(const dimension_t & view, const dimension_t & frame,
        quadranglef_t & interest)
{
    if (!get_viewport_interest(view, frame, interest))
    {
        interest.p[0] = pointf_t(0.0f,  0.0f);
        interest.p[1] = pointf_t(1.0f,  0.0f);
        interest.p[2] = pointf_t(1.0f,  1.0f);
        interest.p[3] = pointf_t(0.0f,  1.0f);
    }
}

void calc_interest_of_aspect2(const dimension_t & view, const dimension_t & frame,
        quadranglef_t & interest)
{
    if (!get_viewport_interest2(view, frame, interest))
    {
        interest.p[0] = pointf_t(-1.0f,  1.0f);
        interest.p[1] = pointf_t( 1.0f,  1.0f);
        interest.p[2] = pointf_t( 1.0f, -1.0f);
        interest.p[3] = pointf_t(-1.0f, -1.0f);
    }
}

// huangxuefeng 2013-11-09 -- added aspect ratio parameters
void flip_and_rotate(s32 nChannel, GLfloat* dst, s32 flip, s32 dir,
        const dimension_t & view, const dimension_t & frame)
{
    // flip horizontally
    s32 index_1[4];
    if (flip & 1)
    {
        index_1[0] = 1;
        index_1[1] = 0;
        index_1[2] = 3;
        index_1[3] = 2;
    }
    else
    {
        index_1[0] = 0;
        index_1[1] = 1;
        index_1[2] = 2;
        index_1[3] = 3;
    }

    // flip vertically
    s32 index_2[4];
    if (flip & 2)
    {
        index_2[0] = index_1[3];
        index_2[1] = index_1[2];
        index_2[2] = index_1[1];
        index_2[3] = index_1[0];
    }
    else
    {
        index_2[0] = index_1[0];
        index_2[1] = index_1[1];
        index_2[2] = index_1[2];
        index_2[3] = index_1[3];
    }

    // rotate
    s32 index_3[4];
    index_3[0] = index_2[(0 + dir) % 4];
    index_3[1] = index_2[(1 + dir) % 4];
    index_3[2] = index_2[(2 + dir) % 4];
    index_3[3] = index_2[(3 + dir) % 4];

    // output
    const int nStep = 5;
    GLfloat * px = dst + 3;
    GLfloat * py = dst + 4;

    // calculate aspect keeping mappings
    quadranglef_t interest;
    calc_interest_of_aspect(view, frame, interest);
    px[0 * nStep] = interest.p[index_3[0]].x;
    py[0 * nStep] = interest.p[index_3[0]].y;
    px[1 * nStep] = interest.p[index_3[1]].x;
    py[1 * nStep] = interest.p[index_3[1]].y;
    px[2 * nStep] = interest.p[index_3[2]].x;
    py[2 * nStep] = interest.p[index_3[2]].y;
    px[3 * nStep] = interest.p[index_3[3]].x;
    py[3 * nStep] = interest.p[index_3[3]].y;

    LOGV("channel#%d: dir: %d, view:%dx%d, frame:%dx%d",
            nChannel, dir,
            view.width, view.height, frame.width, frame.height);
}

// huangxuefeng 2014-02-18 -- added aspect ratio parameters
void flip_and_rotate2(s32 nChannel, GLfloat* dst, s32 flip, s32 dir,
        const dimension_t & view, const dimension_t & frame)
{
    // flip horizontally
    s32 index_1[4];
    if (flip & 1)
    {
        index_1[0] = 1;
        index_1[1] = 0;
        index_1[2] = 3;
        index_1[3] = 2;
    }
    else
    {
        index_1[0] = 0;
        index_1[1] = 1;
        index_1[2] = 2;
        index_1[3] = 3;
    }

    // flip vertically
    s32 index_2[4];
    if (flip & 2)
    {
        index_2[0] = index_1[3];
        index_2[1] = index_1[2];
        index_2[2] = index_1[1];
        index_2[3] = index_1[0];
    }
    else
    {
        index_2[0] = index_1[0];
        index_2[1] = index_1[1];
        index_2[2] = index_1[2];
        index_2[3] = index_1[3];
    }

    // rotate
    s32 index_3[4];
    index_3[0] = index_2[(0 + dir) % 4];
    index_3[1] = index_2[(1 + dir) % 4];
    index_3[2] = index_2[(2 + dir) % 4];
    index_3[3] = index_2[(3 + dir) % 4];

    // output
    const int nStep = 5;
    GLfloat * px = dst + 0;
    GLfloat * py = dst + 1;

    // calculate aspect keeping mappings
    quadranglef_t interest;
    calc_interest_of_aspect2(view, frame, interest);
    px[0 * nStep] = interest.p[index_3[0]].x;
    py[0 * nStep] = interest.p[index_3[0]].y;
    px[1 * nStep] = interest.p[index_3[1]].x;
    py[1 * nStep] = interest.p[index_3[1]].y;
    px[2 * nStep] = interest.p[index_3[2]].x;
    py[2 * nStep] = interest.p[index_3[2]].y;
    px[3 * nStep] = interest.p[index_3[3]].x;
    py[3 * nStep] = interest.p[index_3[3]].y;

    LOGV("channel#%d: dir: %d, view:%dx%d, frame:%dx%d",
            nChannel, dir,
            view.width, view.height, frame.width, frame.height);
}

void flip_and_rotate(s32 nChannel, GLfloat* dst, s32 flip, s32 dir)
{
    // flip horizontally
    s32 index_1[4];
    if (flip & 1)
    {
        index_1[0] = 3;
        index_1[1] = 2;
        index_1[2] = 1;
        index_1[3] = 0;
    }
    else
    {
        index_1[0] = 0;
        index_1[1] = 1;
        index_1[2] = 2;
        index_1[3] = 3;
    }

    // flip vertically
    s32 index_2[4];
    if (flip & 2)
    {
        index_2[0] = index_1[1];
        index_2[1] = index_1[0];
        index_2[2] = index_1[3];
        index_2[3] = index_1[2];
    }
    else
    {
        index_2[0] = index_1[0];
        index_2[1] = index_1[1];
        index_2[2] = index_1[2];
        index_2[3] = index_1[3];
    }

    // rotate
    s32 index_3[4];
    index_3[0] = index_2[(0 + dir) % 4];
    index_3[1] = index_2[(1 + dir) % 4];
    index_3[2] = index_2[(2 + dir) % 4];
    index_3[3] = index_2[(3 + dir) % 4];

    // output
    const int nStep = 5;
    const GLfloat * psx = vertice + 3;
    const GLfloat * psy = vertice + 4;
    GLfloat * px = dst + 3;
    GLfloat * py = dst + 4;
    px[0 * nStep] = psx[index_3[0] * nStep];
    py[0 * nStep] = psy[index_3[0] * nStep];
    px[1 * nStep] = psx[index_3[1] * nStep];
    py[1 * nStep] = psy[index_3[1] * nStep];
    px[2 * nStep] = psx[index_3[2] * nStep];
    py[2 * nStep] = psy[index_3[2] * nStep];
    px[3 * nStep] = psx[index_3[3] * nStep];
    py[3 * nStep] = psy[index_3[3] * nStep];

    LOGV("channel#%d: dir: %d", nChannel, dir);
}

// huangxuefeng 2013-11-08 -- added aspect ratio parameters
void flip_and_rotate_with_compensation(s32 nChannel,
        GLfloat* dst, s32 flip, s32 dir, s32 dirCompensate,
        BOOL32 isFrontCamera,
        const dimension_t & view, const dimension_t & frame)
{
    s32 real_dir = isFrontCamera ?
        dir - dirCompensate: // - is for front camera.
        dir + dirCompensate; // + is for rear camera.
    if (real_dir < 0)
    {
        real_dir += 4;
    }
    else if (real_dir >= 4)
    {
        real_dir -= 4;
    }
#ifdef SUPRESS_FLIP
    flip_and_rotate(nChannel, dst, 0, real_dir, view, frame);
#else
    flip_and_rotate(nChannel, dst, flip, real_dir, view, frame);
#endif // SUPRESS_FLIP
}

// huangxuefeng 2013-11-08 -- added aspect ratio parameters
void flip_and_rotate_with_compensation2(s32 nChannel,
        GLfloat* dst, s32 flip, s32 dir, s32 dirCompensate,
        BOOL32 isFrontCamera,
        const dimension_t & view, const dimension_t & frame)
{
    s32 real_dir = isFrontCamera ?
        dir - dirCompensate: // - is for front camera.
        dir + dirCompensate; // + is for rear camera.
    if (real_dir < 0)
    {
        real_dir += 4;
    }
    else if (real_dir >= 4)
    {
        real_dir -= 4;
    }
#ifdef SUPRESS_FLIP
    flip_and_rotate2(nChannel, dst, 0, real_dir, view, frame);
#else
    flip_and_rotate2(nChannel, dst, flip, real_dir, view, frame);
#endif // SUPRESS_FLIP
}

void flip_and_rotate_with_compensation(s32 nChannel,
        GLfloat* dst, s32 flip, s32 dir, s32 dirCompensate,
        BOOL32 isFrontCamera)
{
    s32 real_dir = isFrontCamera ?
        dir - dirCompensate: // - is for front camera.
        dir + dirCompensate; // + is for rear camera.
    if (real_dir < 0)
    {
        real_dir += 4;
    }
    else if (real_dir >= 4)
    {
        real_dir -= 4;
    }
#ifdef SUPRESS_FLIP
    flip_and_rotate(nChannel, dst, 0, real_dir);
#else
    flip_and_rotate(nChannel, dst, flip, real_dir);
#endif // SUPRESS_FLIP
}

CRenderFrame::CRenderFrame()
    : m_viewWidth(0), m_viewHeight(0)
    , m_channel(0)
    , m_frameWidth(0), m_frameHeight(0)
    , m_frameDir(0), m_isFrontCam(false)
    , m_dir(0)
    , m_flip(0)
    , m_degree(0.0f)
    , m_aspectState(0)
    , m_kAspectRatio(kAspRatStretch)
    , m_dirty(true)
    , m_glProgram(0)
    , m_glVertexShader(0)
    , m_glFragmentShader(0)
    , m_glYTextureID(0)
    , m_glUTextureID(0)
    , m_glVTextureID(0)
    , m_glVertexBuffer(0)
    , m_glIndexBuffer(0)
{
    memset(m_translate, 0, sizeof(m_translate));
	memset(m_scale, 0, sizeof(m_scale));
    memset(m_glUniforms, 0, sizeof(m_glUniforms));
    memset(m_glAttributes, 0, sizeof(m_glAttributes));
	memset(m_scaleMatrix, 0, sizeof(m_scaleMatrix));
	memset(m_translateMatrix, 0, sizeof(m_translateMatrix));
}

CRenderFrame::~CRenderFrame()
{
    //destroy();
}

bool CRenderFrame::init()
{
    // should reset the frame dimension here,
    // otherwise, it would not load frame data properly after a leave.
    // -- huangxuefeng, 2014-03-05
    m_frameWidth = 0;
    m_frameHeight = 0;
	GLfloat scaleMatrix[4 * 4] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
    };
	memcpy(m_scaleMatrix, scaleMatrix, sizeof(scaleMatrix));
    GLfloat translateMatrix[4 * 4] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
    };
	memcpy(m_translateMatrix, translateMatrix, sizeof(translateMatrix));

    const GLchar vShaderStr[] =
		"uniform mat4 translate;                                \n"
	    "uniform mat4 scale;                               \n"
		"uniform float degree;                                  \n"
        "attribute vec4 position;                         		\n"
        "attribute vec2 TexCoordIn;                       		\n"
        "varying vec2 TexCoordOut;                        		\n"
        "mat4 rotation = mat4(0.0, 0.0, 0.0, 0.0,               \n"
        "                     0.0, 0.0, 0.0, 0.0,               \n"
        "                     0.0, 0.0, 1.0, 0.0,               \n"
        "                     0.0, 0.0, 0.0, 1.0);              \n"
        "void main(void)                                  		\n"
        "{                                                		\n"
        "    float radian = radians(degree);                    \n"
        "    mat4 transMatrix = scale;                      \n"
        "    rotation[0][0] = cos(radian);                      \n"
        "    rotation[0][1] = -sin(radian);                     \n"
        "    rotation[1][0] = sin(radian);                      \n"
        "    rotation[1][1] = cos(radian);                      \n"
        "    transMatrix = transMatrix * rotation;              \n"
        "    transMatrix = translate * transMatrix;            \n"
        "    gl_Position = transMatrix * position;              \n"
        "    TexCoordOut = TexCoordIn;                   		\n"
        "}                                               		\n";
    const GLchar fShaderStr[] =
        "precision mediump float;                                     \n"
        "varying lowp vec2 TexCoordOut;                               \n"
        "uniform sampler2D yTexture;                                  \n"
        "uniform sampler2D uTexture;                                  \n"
        "uniform sampler2D vTexture;                                  \n"
        "void main(void)                                              \n"
        "{                                                            \n"
        "    mediump vec4 yuv;                                        \n"
        "    vec4 rgba;                                               \n"
        "    yuv = vec4(texture2D(yTexture, TexCoordOut).r - 0.0625,  \n"
        "               texture2D(uTexture, TexCoordOut).r - 0.5000,  \n"
        "               texture2D(vTexture, TexCoordOut).r - 0.5000,  \n"
        "               1.0);                                         \n"
        "    rgba = mat4(1.164,    1.164,    1.164,    0.0,           \n"
        "                0.0,     -0.392,    2.018,    0.0,           \n"
        "                1.59,    -0.813,    0.0,      0.0,           \n"
        "                0.0,      0.0,      0.0,      1.0) * yuv;    \n"
        "    gl_FragColor = rgba;                                     \n"
        "}                                                            \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    if (!vertexShader)
    {
        LOGE("channel#%d: error", m_channel);
        return false;
    }

    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);
    if (!fragmentShader)
    {
        glDeleteShader(vertexShader);
        LOGE("channel#%d: error", m_channel);
        return false;
    }

    programObject = glCreateProgram();
    if (!programObject)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        LOGE("channel#%d: error", m_channel);
        return false;
    }

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char * infoLog = new char[infoLen];
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            LOGE("channel#%d: %s", m_channel, infoLog);
            delete [] infoLog;
        }

        glDetachShader(programObject, vertexShader);
        glDetachShader(programObject, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(programObject);
        LOGE("channel#%d: error", m_channel);
        return false;
    }

    m_glProgram = programObject;
    m_glVertexShader = vertexShader;
    m_glFragmentShader = fragmentShader;

    glUseProgram(programObject);

	m_glUniforms[UNIFORM_TRANSLATE] = glGetUniformLocation(programObject, "translate");
	m_glUniforms[UNIFORM_SCALE] = glGetUniformLocation(programObject, "scale");
	m_glUniforms[UNIFORM_DEGREE] = glGetUniformLocation(programObject, "degree");
    m_glUniforms[UNIFORM_YTEXTURE] = glGetUniformLocation(programObject, "yTexture");
    m_glUniforms[UNIFORM_UTEXTURE] = glGetUniformLocation(programObject, "uTexture");
    m_glUniforms[UNIFORM_VTEXTURE] = glGetUniformLocation(programObject, "vTexture");

    glUniform1i(m_glUniforms[UNIFORM_YTEXTURE], 0);
    glUniform1i(m_glUniforms[UNIFORM_UTEXTURE], 1);
    glUniform1i(m_glUniforms[UNIFORM_VTEXTURE], 2);
    m_glAttributes[ATTRIB_VERTEX]   = glGetAttribLocation(programObject, "position");
    m_glAttributes[ATTRIB_TEXCOORD] = glGetAttribLocation(programObject, "TexCoordIn");
    glGenBuffers(1, &(m_glVertexBuffer));
    glGenBuffers(1, &(m_glIndexBuffer));

    return true;
}

void CRenderFrame::destroy()
{
    if (!m_glProgram || !glIsProgram(m_glProgram))
    {
    	m_glProgram = 0;
        return;
    }

    glUseProgram(m_glProgram);
    if (m_glAttributes[ATTRIB_VERTEX] != 0)
    {
        glDisableVertexAttribArray(m_glAttributes[ATTRIB_VERTEX]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &(m_glVertexBuffer));
        m_glAttributes[ATTRIB_VERTEX] = 0;
    }
    if (m_glAttributes[ATTRIB_TEXCOORD] != 0)
    {
        glDisableVertexAttribArray(m_glAttributes[ATTRIB_TEXCOORD]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &(m_glIndexBuffer));
        m_glAttributes[ATTRIB_TEXCOORD] = 0;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    if (m_glYTextureID)
    {
        glDeleteTextures(1, &(m_glYTextureID));
        m_glYTextureID = 0;
    }
    if (m_glUTextureID)
    {
        glDeleteTextures(1, &(m_glUTextureID));
        m_glUTextureID = 0;
    }
    if (m_glVTextureID)
    {
        glDeleteTextures(1, &(m_glVTextureID));
        m_glVTextureID = 0;
    }
    if (m_glVertexBuffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_glVertexBuffer);
        m_glVertexBuffer = 0;
    }
    if (m_glIndexBuffer)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_glIndexBuffer);
        m_glIndexBuffer = 0;
    }
    if (m_glVertexShader)
    {
        glDetachShader(m_glProgram, m_glVertexShader);
        glDeleteShader(m_glVertexShader);
        m_glVertexShader = 0;
    }
    if (m_glFragmentShader)
    {
        glDetachShader(m_glProgram, m_glFragmentShader);
        glDeleteShader(m_glFragmentShader);
        m_glFragmentShader = 0;
    }
    glUseProgram(0);
    glDeleteProgram(m_glProgram);
    m_glProgram = 0;
}

void CRenderFrame::set_view(int width, int height)
{
    if (m_viewWidth != width || m_viewHeight != height)
    {
        m_viewWidth = width;
        m_viewHeight = height;
        m_dirty = true;
    }
}

void CRenderFrame::set_direction(int dir)
{
    if (m_dir != dir)
    {
        m_dir = dir;
        m_dirty = true;
    }
}
void CRenderFrame::set_channel(int channel)
{
    if (m_channel != channel)
    {
        m_channel = channel;
        m_dirty = true;
    }
}

void CRenderFrame::set_frameDir(int frameDir)
{
    if (m_frameDir != frameDir)
    {
        m_frameDir = frameDir;
        m_dirty = true;
    }
}

void CRenderFrame::set_frontCamera(bool front)
{
    if (m_isFrontCam != front)
    {
        m_isFrontCam = front;
        m_dirty = true;
    }
}

void CRenderFrame::set_flip(int flip)
{
    if (m_flip != flip)
    {
        m_flip = flip;
        m_dirty = true;
    }
}

void CRenderFrame::set_scale(float *scale)
{
	if (m_scale[kDirectX]!= scale[kDirectX]
		|| m_scale[kDirectY]!= scale[kDirectY]
		|| m_scale[kDirectZ]!= scale[kDirectZ])
    {
        memcpy(m_scale, scale, sizeof(float)*kDirectEnd);
        m_dirty = true;
    }
}

void CRenderFrame::set_rotate(float degree)
{
    if (m_degree != degree)
    {
        m_degree = degree;
        m_dirty = true;
    }
}

void CRenderFrame::set_translate(float *translate)
{
	if (m_translate[kDirectX]!= translate[kDirectX]
		|| m_translate[kDirectY]!= translate[kDirectY]
		|| m_translate[kDirectZ]!= translate[kDirectZ])
    {
        memcpy(m_translate, translate, sizeof(float)*kDirectEnd);
        m_dirty = true;
    }
}

void CRenderFrame::set_aspectState(int astate)
{
    if (m_aspectState != astate)
    {
        LOGV("channel#%d: aspState:%d", m_channel, astate);
        m_aspectState = astate;
        m_dirty = true;
    }
}

void CRenderFrame::set_aspectRatioMode(EMAspectRatio armode)
{
    if (m_kAspectRatio != armode)
    {
        m_kAspectRatio = armode;
        m_dirty = true;
    }
}

void CRenderFrame::draw(
        const unsigned char * py, int ystride,
        const unsigned char * pu, int ustride,
        const unsigned char * pv, int vstride,
        int width, int height)
{
    const GLubyte indices[] = {0, 1, 2, 3};

    LOGD("channel#%d: draw py:%p, ys:%d, pu:%p, us:%d, pv:%p, vs:%d, size:%dx%d, view:%dx%d, aspRat:%d",
            m_channel, py, ystride, pu, ustride, pv, vstride, width, height,
            m_viewWidth, m_viewHeight, m_kAspectRatio);

    glUseProgram(m_glProgram);

    if (update_vertice(width, height) && !m_glYTextureID)
    {
        LOGD("channel#%d: load texture", m_channel);
        //
        // load texture
        //

        // Y Texture
        glGenTextures(1, &(m_glYTextureID));
        glBindTexture(GL_TEXTURE_2D, m_glYTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, ystride, height,
                0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
        // U Texture
        glGenTextures(1, &(m_glUTextureID));
        glBindTexture(GL_TEXTURE_2D, m_glUTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, ustride, height / 2,
                0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
        // V Texture
        glGenTextures(1, &(m_glVTextureID));
        glBindTexture(GL_TEXTURE_2D, m_glVTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, vstride, height / 2,
                0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
		LOGD("m_glYTextureID:%d, m_glUTextureID:%d, m_glVTextureID:%d", m_glYTextureID,m_glUTextureID,m_glVTextureID);
    }


	glUniformMatrix4fv(m_glUniforms[UNIFORM_TRANSLATE], 1, GL_FALSE, m_translateMatrix);
	glUniformMatrix4fv(m_glUniforms[UNIFORM_SCALE], 1, GL_FALSE, m_scaleMatrix);
	glUniform1f(m_glUniforms[UNIFORM_DEGREE], m_degree);
    // vertex and texture data
    glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertice), m_vertice, GL_STATIC_DRAW);
    glVertexAttribPointer(m_glAttributes[ATTRIB_VERTEX], 3, GL_FLOAT,
            GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)0);
    glEnableVertexAttribArray(m_glAttributes[ATTRIB_VERTEX]);
    glVertexAttribPointer(m_glAttributes[ATTRIB_TEXCOORD], 2, GL_FLOAT,
            GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(m_glAttributes[ATTRIB_TEXCOORD]);
    // elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // texture: Y data
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_glYTextureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ystride, height,
            GL_LUMINANCE, GL_UNSIGNED_BYTE, py);
    // texture: U data
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_glUTextureID);
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, ustride, height / 2,
            GL_LUMINANCE, GL_UNSIGNED_BYTE, pu);
    // texture: V data
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_glVTextureID);
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, vstride, height / 2,
            GL_LUMINANCE, GL_UNSIGNED_BYTE, pv);
    // rendering
    //glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, indices);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, 0);

}

void CRenderFrame::rotate_scale_translate()
{
    m_scaleMatrix[0 * 4 + 0] = m_scale[kDirectX];
	m_scaleMatrix[1 * 4 + 1] = m_scale[kDirectY];
	m_scaleMatrix[2 * 4 + 2] = m_scale[kDirectZ];
	m_translateMatrix[3 * 4 + 0] = m_translate[kDirectX];
	m_translateMatrix[3 * 4 + 1] = m_translate[kDirectY];
	m_translateMatrix[3 * 4 + 2] = m_translate[kDirectZ];
}

bool CRenderFrame::update_vertice(int frame_width, int frame_height)
{
    if (m_frameWidth != frame_width || m_frameHeight != frame_height)
    {
        m_frameWidth = frame_width;
        m_frameHeight = frame_height;
        // clear texture buffers
        if(m_glYTextureID != 0)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteTextures(1, &(m_glYTextureID));
            glDeleteTextures(1, &(m_glUTextureID));
            glDeleteTextures(1, &(m_glVTextureID));
            m_glYTextureID = 0;
            m_glUTextureID = 0;
            m_glVTextureID = 0;
        }
        m_dirty = true;
    }

    if (!m_dirty)
    {
        return false;
    }

    memcpy(m_vertice, vertice, sizeof(m_vertice));

    //
    // update vertice
    //
    dimension_t viewport(m_viewWidth, m_viewHeight);
    dimension_t frame(m_frameWidth, m_frameHeight);
	LOGD("channel#%d: m_scale:X%fY%fZ%f, m_degree:%f, m_translate:X%fY%fZ%f", m_channel
		, m_scale[kDirectX], m_scale[kDirectY], m_scale[kDirectZ]
		, m_degree
		, m_translate[kDirectX], m_translate[kDirectY], m_translate[kDirectZ]);
	rotate_scale_translate();
    LOGD("channel#%d: m_kAspectRatio:%d, m_aspectState:%d", m_channel,
            m_kAspectRatio, m_aspectState);
    switch (m_kAspectRatio)
    {
    case kAspRatTrim:
        if (m_aspectState & 1)
        {
            viewport = viewport.transposeOf();
        }
        if (m_aspectState & 2)
        {
            frame = frame.transposeOf();
        }

        flip_and_rotate_with_compensation(m_channel, m_vertice, m_flip, m_dir,
                m_frameDir, m_isFrontCam, viewport, frame);
        break;
    case kAspRatPadding:
        if (m_aspectState & 1)
        {
            viewport = viewport.transposeOf();
        }
        if (m_aspectState & 2)
        {
            frame = frame.transposeOf();
        }

        flip_and_rotate_with_compensation2(m_channel, m_vertice, m_flip, m_dir,
                m_frameDir, m_isFrontCam, viewport, frame);
        break;
    default:
        flip_and_rotate_with_compensation(m_channel, m_vertice, m_flip, m_dir,
                m_frameDir, m_isFrontCam);
    }

    m_dirty = false;
    return true;
}


} // namespace ants360

