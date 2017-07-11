#include <android/bitmap.h>
#include "GVideoPlayer.h"
#include "../../framework/GMediaEngine/GMVideo/GMVideo.h"
#include "../GDataConverter/GDataConverter.h"
#include "../../framework/GEngineEvent/GEngineEvent.h"
#include "../GVideoRecorder/GVideoRecorder.h"
#include "../GFramework.h"

#ifdef __cplusplus
extern "C"
{
#endif
//void GVideoPlayer_C_OnCanvas(unsigned char* data, int size, int width, int height, int first) { GVideoPlayer::sharedInstance().onCanvas(data, size, width, height, first); }
void GVideoPlayer_C_OnRender(unsigned char* data, int size, int width, int height, int first)
{
	GVideoPlayer::sharedInstance()->onRender(data, size, width, height, first);
}
#ifdef __cplusplus
}
#endif

void GVideoPlayer_C_OnModuleEnter( void *delegate )
{
	if ( delegate )
		( ( GVideoPlayer * )delegate )->onModuleEnter();
}
void GVideoPlayer_C_OnModuleExit ( void *delegate )
{
	if ( delegate )
		( ( GVideoPlayer * )delegate )->onModuleExit ();
}
void GVideoPlayer_C_OnVideoEvent ( void *delegate, bool start , bool success, const char *message )
{
	if ( delegate )
		( ( GVideoPlayer * )delegate )->onVideoEvent ( start , success, message	);
}

void GVideoPlayer_C_On555Event ( void *delegate, int code, const char *message )
{
	if ( delegate )
		( ( GVideoPlayer * )delegate )->on555Event ( code , message	);
}

GVideoPlayer* GVideoPlayer::m_pInstance = NULL;
GVideoPlayer* GVideoPlayer::sharedInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new GVideoPlayer();
	return m_pInstance;
}

GVideoPlayer::GVideoPlayer():
	mRunning(false),
    m_jvm(NULL),
    m_jNativeMediaFramework(NULL),
    m_jNativeMediaFrameworkfld(NULL),
    m_jOnMessagemtd(NULL),
	m_pRenderer(NULL),
	mLock(NULL)
{
	init();
}

GVideoPlayer::~GVideoPlayer()
{
	destory();
}

void GVideoPlayer::init()
{
	mLock = GMUtility::sharedInstance()->createLock();
}

void GVideoPlayer::destory()
{
	GMUtility::sharedInstance()->destorylock( &mLock );
    JNIEnv *env = NULL;
    if (m_jvm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE("get JNIEnv failed");
        return;
    }

    if (m_jNativeMediaFramework != NULL)
    {
        env->DeleteGlobalRef(m_jNativeMediaFramework);
        m_jNativeMediaFramework = NULL;

    }
	m_jOnMessagemtd = NULL;
	m_jNativeMediaFrameworkfld = NULL;
}

jboolean GVideoPlayer::classInit (JNIEnv* env, jclass klass)
{
	m_jNativeMediaFrameworkfld = env->GetFieldID (klass, "mNativeMediaFramework", "I");
	m_jOnMessagemtd = env->GetMethodID (klass, "fireOnMessage", "(ILjava/lang/String;)V");

	if (!m_jNativeMediaFrameworkfld || !m_jOnMessagemtd) {
		LOGE("Failed to The calling class does not implement all necessary interface methods");
		return JNI_FALSE;
	}

    if (env->GetJavaVM(&m_jvm) != 0)
    {
        LOGE("get java vm failed");
        return JNI_FALSE;
    }

	return JNI_TRUE;
}
void GVideoPlayer::install ( JNIEnv *env, jobject obj )
{
	GMBindModuleEvent( (void *)GVideoPlayer_C_OnModuleEnter, (void *)GVideoPlayer_C_OnModuleExit, this );
	m_jNativeMediaFramework = (jobject)env->NewGlobalRef(obj);

	return;
}

void GVideoPlayer::unInstall( JNIEnv *env )
{
	if(m_jNativeMediaFramework != NULL)
		env->DeleteGlobalRef(m_jNativeMediaFramework);
	m_jNativeMediaFramework = NULL;
}

bool GVideoPlayer::isRuning()
{
	bool fResult = false;
	GMUtility::sharedInstance()->lock  ( mLock );
	fResult = mRunning;
	GMUtility::sharedInstance()->unlock( mLock );
	return fResult;
}

void GVideoPlayer::setRunning( bool v )
{
	GMUtility::sharedInstance()->lock  ( mLock );
	mRunning = v;
	GMUtility::sharedInstance()->unlock( mLock );
}

jboolean GVideoPlayer::bindRenderer( JNIEnv *env, jobject obj, jobject renderer )
{
	jboolean ret = JNI_FALSE;
	do
	{
		if ( env == NULL )
		{
			LOGE("env is null!");
			break;
		}
		if ( obj == NULL )
		{
			LOGE("obj is null!");
			break;
		}
		if ( renderer == NULL )
		{
			LOGE("renderer is null!");
			break;
		}

		ants360::CJniRenderer* jniRender = ants360::CJniRenderer::GetJniRenderer(env, renderer);
		if ( jniRender == NULL )
		{
			LOGE("jniRender is null!");
			break;
		}
		if ( m_pRenderer != NULL && m_pRenderer != jniRender )
		{
			LOGI("m_pRenderer is not equal to jniRender, so destory it!");
			ants360::CJniRenderer::DestroyJniRenderer(m_pRenderer);
		}

		m_pRenderer = jniRender;
		ret = JNI_TRUE;

	} while ( 0 );
	return ret;
}

void GVideoPlayer::mediaPlay( JNIEnv *env, jobject obj, jstring  address, jstring name, jstring password)
{
	const char   *	fError 			= NULL;
	char			fAddress[ADDRESS_MAX_LEN]	= { 0 };
	char			fUserName[USERNAME_MAX_LEN]	= { 0 };
	char			fPassword[PASSWORD_MAX_LEN]	= { 0 };
	GMediaMessage 	fMessage;

	do
	{
		if ( env == NULL || obj == NULL )
		{
			fError = "system error";
			break;
		}

		//����RTSP��ַ
		if ( ( address == NULL )
			|| ( GDataConverter::sharedInstance()->getStringBytes( env, address, fAddress, sizeof( fAddress ) ) <= 0 ))
		{
			fError = "address is null";
			break;
		}

		//�����û�������
		if ( (name == NULL)
			||  (GDataConverter::sharedInstance()->getStringBytes( env, name    , fUserName, sizeof( fUserName ) ) <= 0))
		{
				strncpy( fUserName, "", sizeof(fUserName) );
		}

		if ( (password == NULL)
			|| ( GDataConverter::sharedInstance()->getStringBytes( env, password, fPassword, sizeof( fPassword ) ) <= 0 ))
		{
				strncpy( fPassword, "", sizeof(fPassword) );
		}

		if ( isRuning() )
		{
			fError = "playing now";
			break;
		}

		//����Ƶ
		LOGI( "mediaPlay(...) >> GMVideoStart( %s, %s, %s, ... ); \n", fAddress, ( strlen( fUserName ) > 0 ? fUserName : "null" ), ( strlen( fPassword ) > 0 ? fPassword : "null" ) );
		GMVideoStart( fAddress, fUserName, fPassword, (void *)GVideoPlayer_C_OnVideoEvent, (void *)GVideoPlayer_C_On555Event, this );

	} while( 0 );

	//������Ϣд����Ϣ����
	if ( fError != NULL )
	{

		LOGI( "mediaPlay( ... ) >> %s \n", fError );
		onVideoEvent( true, false, fError );
	}
}

void GVideoPlayer::mediaStop()
{
	const char 	 *fError = NULL;
	GMediaMessage fMessage;

	do
	{
		if ( !isRuning() )
		{
			fError = "already stopped";
			break;
		}
		//�ر���Ƶ
		GMVideoStop( ( void * )GVideoPlayer_C_OnVideoEvent, this );
	} while( 0 );

	//������Ϣд����Ϣ����
	if ( fError != NULL )
	{
		LOGI( "mediaStop() >> %s \n", fError );
		onVideoEvent( false, false, fError );
	}
}

void GVideoPlayer::onModuleEnter()
{
	LOGI( "onModuleEnter()\n" );
	setRunning( true );
}

void GVideoPlayer::onModuleExit()
{
	LOGI( "onModuleExit()\n" );
	setRunning( false );
}

void GVideoPlayer::onRender( void * data, int size, int width , int height, int first )
{
	if ( first > 0 )
	{
		GMediaMessage fMessage;
		fMessage.id = EVENT_CODE_FIRST_FRAME(width, height);
		sprintf( fMessage.message, "%s", "" );
		fMessage.size = strlen( fMessage.message );
		sendMessage( &fMessage );
		onVideoEvent( true , true, NULL );
	}

	if(data == NULL || width == 0 || height ==0)
	{
		return;
	}

	if(m_pRenderer != NULL)
	{
		if(ants360::CJniRenderer::IsValid(m_pRenderer))
		{
			m_pRenderer->SetFrame((unsigned char*)data, width, height);
		}
		else
		{
			LOGE("renderer is invalid: %p", m_pRenderer);
		}
	}
	else
	{
		LOGE("renderer is NULL");
	}

	return;
}

void GVideoPlayer::onVideoEvent( bool start , bool success, const char *message )
{
	GMediaMessage fMessage;
	LOGD( "onVideoEvent( ... ) >> start = %s  success = %s  message = %s\n", ( start ? "true" : "false" ), ( success ? "true" : "false" ), ( message != NULL ? message : "null" ) );
	fMessage.id = ( EVENT_CODE_TYPE_VIDEO | ( start ? EVENT_CODE_ACTION_START : EVENT_CODE_ACTION_STOP ) | ( success ? EVENT_CODE_VALUE_SUCCESS : EVENT_CODE_VALUE_FAILED ) );
	sprintf( fMessage.message, "%s", ( message != NULL ? message : "" ) );
	fMessage.size = strlen( fMessage.message );
	sendMessage( &fMessage );
}

void GVideoPlayer::on555Event(  int code, const char *message )
{
	GMediaMessage fMessage;

	LOGD( "on555Event( ... ) >> code = %d message = %s\n", code,  ( message != NULL ? message : "null" ) );

	fMessage.id = ( EVENT_CODE_TYPE_555 | code );
	sprintf( fMessage.message, "%s", ( message != NULL ? message : "" ) );
	fMessage.size = strlen( fMessage.message );
	sendMessage( &fMessage );
}

void GVideoPlayer::sendMessage(GMediaMessage *message)
{
	do
	{
		if ( !message || !m_jvm || !m_jNativeMediaFramework || !m_jOnMessagemtd ) break;
		JNIEnv *env = NULL;
		bool isAttacked = false;
		int status = (m_jvm)->GetEnv((void **) &env, JNI_VERSION_1_4);
		if(status != JNI_OK)
		{

			status = m_jvm->AttachCurrentThread( &env, NULL );
			if(status  != JNI_OK)
			{
				LOGE("failed to attach current thread");
				break;
			}
			isAttacked = true;
		}

		env->PushLocalFrame(10);
		jstring jnistr = GDataConverter::sharedInstance()->StringToJString(env, message->message);
		env->CallVoidMethod( m_jNativeMediaFramework, m_jOnMessagemtd, message->id, jnistr );
		env->DeleteLocalRef(jnistr);
		env->PopLocalFrame(NULL);

		if(isAttacked)
		{
			m_jvm->DetachCurrentThread();
		}
	} while ( false );
}

