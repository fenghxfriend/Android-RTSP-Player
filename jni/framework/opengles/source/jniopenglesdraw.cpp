/*******************************************************************************
 *        Module: com_ants360_opengles_Renderer
 *          File: com_ants360_opengles_Renderer.cpp
 * Functionality: jni stub for com.ants360.opengles.Renderer.
 *       Related: GLES2
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2013 360ANTS, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2013-07-17  v1.0        huangxuefeng  created
 ******************************************************************************/
#include <jni.h>
#include <pthread.h>
#include <stdio.h>

#include "androidlog.h"
#include "autolock.h"
#include "jnirenderer.h"
#include "jnirenderer_impl.h"

#include "ants360typedefs.h"
#include "version.h"

using namespace ants360;

namespace ants360 {

namespace { // anonymous

void jni_Draw(JNIEnv *env, jobject thiz)
{
    CJniRenderer* pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        return;
    }
    pPlayer->Draw();
}

void jni_Init(JNIEnv *env, jobject thiz, jint nWidth, jint nHeight)
{
    USE_LOG;
    LOGV("%d x %d", nWidth, nHeight);
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        return;
    }
    pPlayer->Init(nWidth, nHeight);
}

void jni_Create(JNIEnv *env, jobject thiz)
{
    USE_LOG;
    CJniRenderer::CreateJniRenderer(env, thiz);
}

void jni_Destroy(JNIEnv *env, jobject thiz)
{
    USE_LOG;
    CJniRenderer * pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        LOGE("jni renderer is null");
        return;
    }
    CJniRenderer::DestroyJniRenderer(pPlayer);
}

void jni_setDirection(JNIEnv *env, jobject thiz, jint nDirection)
{
    LOGV("direction = %d", nDirection);
    if (nDirection >= 0 && nDirection < 4)
    {
        CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
        if (pPlayer == NULL)
        {
            return ;
        }
        return pPlayer->SetDirection(nDirection);
    }
}

jint jni_getDirection(JNIEnv *env, jobject thiz)
{
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        return 0;
    }
    return pPlayer->GetDirection();
}

void jni_setRotate(JNIEnv *env, jobject thiz, jfloat fDegree)
{
    LOGV("rotate: degree = %f", fDegree);
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        return ;
    }
    return pPlayer->SetRotate(fDegree);
}

jfloat jni_getRotate(JNIEnv *env, jobject thiz)
{
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);

    if (pPlayer == NULL)
    {
        return 0;
    }
    return pPlayer->GetRotate();

}

void jni_setTranslate(JNIEnv *env, jobject thiz, jfloat fTranslateX, jfloat fTranslateY, jfloat fTranslateZ)
{
    LOGV("translate: x,y,z = %f,%f,%f,", fTranslateX, fTranslateY, fTranslateZ);
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        return ;
    }
    return pPlayer->SetTranslate(fTranslateX, fTranslateY, fTranslateZ);
}

jfloat jni_getTranslate(JNIEnv *env, jobject thiz, jint nTranslateEnum)
{
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
	if(nTranslateEnum >= kDirectX && nTranslateEnum <= kDirectZ)
	{
	    if (pPlayer == NULL)
        {
            return 0;
        }
        return pPlayer->GetTranslate(nTranslateEnum);
	}

}

void jni_setScale(JNIEnv *env, jobject thiz, jfloat fScaleX, jfloat fScaleY, jfloat fScaleZ)
{
	LOGV("scale: x,y,z = %f,%f,%f,", fScaleX, fScaleY, fScaleZ);
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
    	LOGE("player is null!");
        return ;
    }
    return pPlayer->SetScale(fScaleX, fScaleY, fScaleZ);

}

jfloat jni_getScale(JNIEnv *env, jobject thiz, jint nScaleEnum)
{
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
	if(nScaleEnum >= kDirectX && nScaleEnum <= kDirectZ)
	{
	    if (pPlayer == NULL)
	    {
	        return 0;
	    }
	    return pPlayer->GetScale(nScaleEnum);
	}
}

void jni_setFlip(JNIEnv *env, jobject thiz, jint nFlip)
{
    LOGV("flip = %d", nFlip);
    if (nFlip >= 0 && nFlip < 4)
    {
        CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
        if (pPlayer == NULL)
        {
            return ;
        }
        return pPlayer->SetFlip(nFlip);
    }
}

jint jni_getFlip(JNIEnv *env, jobject thiz)
{
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        return 0;
    }
    return pPlayer->GetFlip();
}

void jni_keepAspectRatio(JNIEnv *env, jobject thiz, jboolean bValue)
{
    LOGV("keep aspect: %d", bValue);
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        return ;
    }
    if (bValue)
    {
        return pPlayer->KeepAspectRatio(kAspRatTrim);
    }
    else
    {
        return pPlayer->KeepAspectRatio(kAspRatStretch);
    }
}

jint jni_getAspectState(JNIEnv *env, jobject thiz)
{
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        return 0;
    }
    return pPlayer->GetAspectState();
}

void jni_setAspectState(JNIEnv *env, jobject thiz, jint state)
{
    LOGV("aspect state: %d", state);
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        return;
    }
    return pPlayer->SetAspectState(state);
}

void jni_refresh(JNIEnv *env, jobject thiz)
{
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL)
    {
        return;
    }
    return pPlayer->Refresh();
}

void jni_setFrame(JNIEnv *env, jobject thiz, jbyteArray jframeBuff, jint jframeWidth, jint jframeheight)
{
	USE_LOG;
    CJniRenderer *pPlayer = CJniRenderer::GetJniRenderer(env, thiz);
    if (pPlayer == NULL || !ants360::CJniRenderer::IsValid(pPlayer))
    {
        return;
    }
	jbyte* frameBuff = env->GetByteArrayElements(jframeBuff,0);
	if(frameBuff == NULL || jframeheight == 0 || jframeWidth ==0)
	{
		return;
	}
 	pPlayer->SetFrame((unsigned char *) frameBuff,jframeWidth,jframeheight);
	env->ReleaseByteArrayElements(jframeBuff,frameBuff,0);
    return;
}

static JNINativeMethod gMethods[] =
{
    {"keepAspectRatio", "(Z)V", (void*) jni_keepAspectRatio},
    {"setDirection", "(I)V", (void*) jni_setDirection},
    {"getDirection", "()I", (void*) jni_getDirection},
    {"setFlip", "(I)V", (void*) jni_setFlip},
    {"getFlip", "()I", (void*) jni_getFlip},
    {"setScale", "(FFF)V", (void*) jni_setScale},
    {"getScale", "(I)F", (void*) jni_getScale},
    {"setTranslate", "(FFF)V", (void*) jni_setTranslate},
    {"getTranslate", "(I)F", (void*) jni_getTranslate},
    {"setRotate", "(F)V", (void*) jni_setRotate},
    {"getRotate", "()F", (void*) jni_getRotate},
    {"getAspectState", "()I", (void*)jni_getAspectState},
    {"setAspectState", "(I)V", (void*)jni_setAspectState},
    {"setFrame", "([BII)V", (void*)jni_setFrame},
    {"refresh", "()V", (void*)jni_refresh},
    {"_draw", "()V", (void*) jni_Draw},
    {"_init", "(II)V", (void*) jni_Init},
    {"_create", "()V", (void*) jni_Create},
    {"_destroy", "()V", (void*) jni_Destroy},
};

#ifndef NELEM
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

static int jniRegisterNativeMethods(JNIEnv* env, const char* className, const JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        char* msg;
        asprintf(&msg, "Native registration unable to find class '%s', aborting", className);
        env->FatalError(msg);
    }

    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        char* msg;
        asprintf(&msg, "RegisterNatives failed for '%s', aborting", className);
        env->FatalError(msg);
    }

    return 0;
}

} // anonymous namespace

int LoadRendererJNI(JNIEnv* env)
{
    ALOG(LOG_INFO, "VidOpenGL", "version: %u", VIDOPENGL_VERSION);

    const char * const class_path_name = "com/video/draw/PlayerRenderer";
    return jniRegisterNativeMethods(env, class_path_name, gMethods, NELEM(gMethods));
}
} // namespace ants360
