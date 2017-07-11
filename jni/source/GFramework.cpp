#include <string.h>
#include "../framework/GEngine/GEngine.h"
#include "GVideoPlayer/GVideoPlayer.h"
#include "GEventManager/GEventManager.h"
#include "GCommandManager/GCommandManager.h"
#include "GAudioManager/GAudioManager.h"
#include "GCameraManager/GCameraManager.h"
#include "GVideoRecorder/GVideoRecorder.h"
#include "GTasksKiller/GTasksKiller.h"
#include "GScreenshotManager/GScreenshotManager.h"
#include "GFramework.h"

#include "../framework/opengles/include/jnirenderer.h"

/* Static class initializer: retrieve method and field IDs */
static jboolean jni_classInit (JNIEnv* env, jclass klass)
{
	return GVideoPlayer::sharedInstance()->classInit( env, klass);
}

static void jni_install( JNIEnv *env, jobject obj )
{
	//GAudioManager::sharedInstance().install0();
	GVideoPlayer::sharedInstance()->install( env, obj );
}

static void jni_mediaPlay ( JNIEnv *env, jobject obj, jstring  address, jstring name, jstring password )
{
	GVideoPlayer::sharedInstance()->mediaPlay( env, obj, address, name, password );
}

static void jni_bindRenderer ( JNIEnv *env, jobject obj, jobject  renderer )
{
	GVideoPlayer::sharedInstance()->bindRenderer( env, obj, renderer);
}

static void jni_mediaStop ( JNIEnv *env, jobject obj )
{
	GVideoPlayer::sharedInstance()->mediaStop();
}

static void jni_unInstall ( JNIEnv *env, jobject obj )
{
	GVideoPlayer::sharedInstance()->unInstall( env );
	//GAudioManager::sharedInstance().unInstall0();
}


/* List of implemented native methods */
static JNINativeMethod gMethods[] = {
  { "classInit", "()Z", (void *) jni_classInit},
  { "install", "()V", (void *) jni_install},

  { "mediaPlay", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", (void *) jni_mediaPlay},
  { "bindRenderer", "(Ljava/lang/Object;)V", (void *) jni_bindRenderer},
  { "mediaStop", "()V", (void *) jni_mediaStop},

  { "unInstall", "()V", (void *) jni_unInstall}
};

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

int LoadGMediaFrameworkJNI(JNIEnv* env)
{
	const char * const class_path_name = "com/greenbamboo/prescholleducation/MediaFramework/GMediaFramework";
	return jniRegisterNativeMethods(env, class_path_name, gMethods, N_ELEMENTS(gMethods));
}
jint JNI_OnLoad(JavaVM * vm, void * reserved)
{
    JNIEnv * env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        goto bail;
    }

    LoadGMediaFrameworkJNI(env);
	ants360::LoadRendererJNI(env);

    result = JNI_VERSION_1_4;
bail:
    return result;
}