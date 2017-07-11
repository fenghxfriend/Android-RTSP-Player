#ifndef GVideoPlayer_h
#define GVideoPlayer_h

#include <jni.h>
#include <memory>
#include <string>
#include "../../framework/GMediaEngine/GMUtility/GMUtility.h"
#include "../GDataConverter/GDataConverter.h"

#include <pthread.h>
#include <stdio.h>
#include "videodef.h"
#include "ants360typedefs.h"
#include "androidlog.h"
#include "autofile.h"
#include "autolock.h"
#include "jnirenderer.h"
#include "jnirenderer_impl.h"


#define MESSAGE_MAX_SIZE 256

using namespace std;

class GMediaMessage
{
public:
    int	 id;
    char message[MESSAGE_MAX_SIZE];
    int  size;

	GMediaMessage():
        id(-1),
		size(0)
	{
		memset( message, 0, sizeof( message ) );
	};
    ~GMediaMessage(){};


} ;

class GVideoPlayer
{
public:
    GVideoPlayer();
    ~GVideoPlayer();
    static  GVideoPlayer* sharedInstance();
    jboolean classInit (JNIEnv* env, jclass klass);
    void install  ( JNIEnv *env, jobject obj);
    void unInstall( JNIEnv *env );

public:
    friend void GVideoPlayer_C_OnModuleEnter( void 			* delegate 										     	   );
    friend void GVideoPlayer_C_OnModuleExit ( void 			* delegate 										     	   );
    friend void GVideoPlayer_C_OnVideoEvent ( void 			* delegate, bool start , bool success, const char *message );
    friend  void GVideoPlayer_C_On555Event ( void *delegate, int code, const char *message );

public:
	void 		mediaPlay				( JNIEnv *env, jobject obj, jstring  address	, jstring name			, jstring 	password );
	jboolean 	bindRenderer				( JNIEnv *env, jobject obj, jobject  renderer );
	void   		mediaStop				();

private:
	void 		onModuleEnter			();
	void 		onModuleExit 			();
	void 		onVideoEvent 			( bool start , bool success, const char *message );

	void 		on555Event					(  int code, const char *message );

public:
	void 		onRender				( void * data, int 	   size, int   width , int height, int first );

private:
	void		init					();
	void		destory					();
	bool		isRuning				();
	void		setRunning				( bool v );

private:
	void		sendMessage			(GMediaMessage *message);

private:

    JavaVM*                 m_jvm;
    jobject					m_jNativeMediaFramework;
    jfieldID                m_jNativeMediaFrameworkfld;
    jmethodID				m_jOnMessagemtd;

    ants360::CJniRenderer*  m_pRenderer;
	bool					mRunning;

    GMLock	    			mLock;
    static GVideoPlayer*    m_pInstance;


    class Garbo

    {

    public:

        ~Garbo()

        {
            if (GVideoPlayer::m_pInstance)

            {
                delete GVideoPlayer::m_pInstance;
                m_pInstance = NULL;
            }
        }
    };

    static Garbo garbo;
};

#endif
