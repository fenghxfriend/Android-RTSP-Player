/*******************************************************************************
 *        Module: shader
 *          File: shader.h
 * Functionality: tools for opengl shader programs.
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
#ifndef _ANTS360_SHADER_H_
#define _ANTS360_SHADER_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace ants360 {


/*******************************************************************************
 函数名      : LoadShader
 功能        : 加载渲染器
 算法实现    : 无
 引用全局变量: 无
 输入参数说明: type      - shader 类型
                           GL_VERTEX_SHADER   - vertex shader
                           GL_FRAGMENT_SHADER - fragment shader
               shaderSrc - shader 的源代码
 返回值说明  : 0    - 加载渲染器失败
               其它 - 加载渲染器成功
 -------------------------------------------------------------------------------
 修改记录:
 日期        版本  修改人  走读人  修改内容
 2013/07/12  1.0   谢志钢          创建
*******************************************************************************/
GLuint LoadShader(GLenum type, const char *shaderSrc);


} // namespace ants360

#endif // _ANTS360_SHADER_H_