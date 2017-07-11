/*******************************************************************************
 *        Module: renderbackground
 *          File: renderbackground.cpp
 * Functionality: draw black background onto opengl context.
 *       Related: GLES2
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2013 360ANTS, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2014-01-19  v1.1        huangxuefeng  created
 ******************************************************************************/
#include "androidlog.h"
#include "renderbackground.h"
#include "shader.h"
#include "version.h"

namespace ants360 {


CRenderBackground::CRenderBackground()
    : m_glProgram(0)
    , m_glVertexShader(0)
    , m_glFragmentShader(0)
    , m_glAttributes(0)
    , m_glVertexBuffer(0)
    , m_glIndexBuffer(0)
{ }

CRenderBackground::~CRenderBackground()
{
    //destroy();
}

bool CRenderBackground::init()
{
    const GLchar vShaderStr[] =
        "attribute vec4 position;                                         \n"
        "void main(void)                                                  \n"
        "{                                                                \n"
        "    gl_Position = position;                                      \n"
        "}                                                                \n";

    const GLchar fShaderStr[] =
        "precision mediump float;                                         \n"
        "void main(void)                                                  \n"
        "{                                                                \n"
        "    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);                     \n"
        "}                                                                \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    if (!vertexShader)
    {
        return false;
    }

    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);
    if (!fragmentShader)
    {
        glDeleteShader(vertexShader);
        return false;
    }

    programObject = glCreateProgram();
    if (!programObject)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
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
            LOGE("%s", infoLog);
            delete [] infoLog;
        }

        glDetachShader(programObject, vertexShader);
        glDetachShader(programObject, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(programObject);
        return false;
    }

    m_glProgram = programObject;
    m_glVertexShader = vertexShader;
    m_glFragmentShader = fragmentShader;

    glUseProgram(programObject);
    m_glAttributes = glGetAttribLocation(programObject, "position");
    glGenBuffers(1, &m_glVertexBuffer);
    glGenBuffers(1, &m_glIndexBuffer);

    return true;
}

void CRenderBackground::destroy()
{
    if (!m_glProgram || !glIsProgram(m_glProgram))
    {
    	m_glProgram = 0;
        return;
    }

    glUseProgram(m_glProgram);
    if (m_glAttributes)
    {
        glDisableVertexAttribArray(m_glAttributes);
        m_glAttributes = 0;
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

void CRenderBackground::draw()
{
    const GLfloat vertice[] =
    {
        //       position
        // x        y       z
        -1.0f,    1.0f,   0.0f,
         1.0f,    1.0f,   0.0f,
         1.0f,   -1.0f,   0.0f,
        -1.0f,   -1.0f,   0.0f,
    };
    const GLushort indices[] = {0, 1, 2, 3};
    glUseProgram(m_glProgram);
    glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), vertice, GL_STATIC_DRAW);
    glVertexAttribPointer(m_glAttributes, 3, GL_FLOAT,
            GL_FALSE, 3 * sizeof(GLfloat), (const GLvoid*)0);
    glEnableVertexAttribArray(m_glAttributes);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, 0);
}


} // namespace ants360
