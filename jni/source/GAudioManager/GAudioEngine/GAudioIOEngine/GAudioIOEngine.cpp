#include "GAudioIOEngine.h"


#include <android/log.h>
#define printf(...) __android_log_print( ANDROID_LOG_VERBOSE, "GMediaFramework", __VA_ARGS__ );

void GAudioIOEngine::install()
{
	createEngine();
}

void GAudioIOEngine::unInstall()
{
	destoryEngine();
}

SLEngineItf GAudioIOEngine::engine()
{
	return mEngineInterface;
}

SLObjectItf GAudioIOEngine::mixOutput()
{
	return mMixOutputObject;
}

void GAudioIOEngine::createEngine()
{
	SLresult result	 = SL_RESULT_UNKNOWN_ERROR;
	bool 	 result0 = false;
	printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 001 \n" );

	do
	{
		//��ʼ��ȫ�ֱ���
		mEngineObject	 	= 0;
		mEngineInterface 	= 0;
		mMixOutputObject	= 0;
		mMixOutputInterface = 0;
//		mOrigEnvSetting	 	= 0;
		mChangedEnvSetting 	= false;

		//��������
		printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 002 \n" );
	    result = slCreateEngine( &mEngineObject, 0, 0, 0, 0, 0 );
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 003 \n" );
	    if ( SL_RESULT_SUCCESS != result ) break;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 004 \n" );

	    //ʵ������
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 005 \n" );
	    result = ( *mEngineObject )->Realize( mEngineObject, SL_BOOLEAN_FALSE );
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 006 \n" );
	    if ( SL_RESULT_SUCCESS != result ) break;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 007 \n" );

	    //ȡ������ӿ�
	    result = ( *mEngineObject )->GetInterface( mEngineObject, SL_IID_ENGINE, &mEngineInterface );
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 008 \n" );
	    if ( SL_RESULT_SUCCESS != result ) break;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 009 \n" );

	    //��������
	    SLInterfaceID 	mixOutputIds[1]	= { SL_IID_ENVIRONMENTALREVERB 	};
	    SLboolean 		mixOutputReq[1]	= { SL_BOOLEAN_FALSE 			};
	    result = ( *mEngineInterface )->CreateOutputMix( mEngineInterface, &mMixOutputObject, 1, mixOutputIds, mixOutputReq );
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 010 \n" );
	    if ( SL_RESULT_SUCCESS != result ) break;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 011 \n" );

	    //ʵ�ֻ���
	    result = ( *mMixOutputObject )->Realize( mMixOutputObject, SL_BOOLEAN_FALSE );
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 012 \n" );
	    if ( SL_RESULT_SUCCESS != result ) break;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 013 \n" );

	    //ȡ�û����ӿ�
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 014\n" );
	    result = ( *mMixOutputObject )->GetInterface( mMixOutputObject, SL_IID_ENVIRONMENTALREVERB, &mMixOutputInterface );
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 015\n" );
	    if (SL_RESULT_SUCCESS == result)
	    {
	    	printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 016 \n" );
	    	//ȡ����Ƶȫ������
	    	( *mMixOutputInterface )->GetEnvironmentalReverbProperties( mMixOutputInterface, &mOrigEnvSetting );
	    	printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 017 \n" );

	    	//�޸���Ƶȫ������
	    	SLEnvironmentalReverbSettings mixOutputSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
	    	printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 018 \n" );
	        result = ( *mMixOutputInterface )->SetEnvironmentalReverbProperties( mMixOutputInterface, &mixOutputSettings );
	        printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 019 \n" );
	        mChangedEnvSetting = (SL_RESULT_SUCCESS == result);
	    }
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::createEngine >> 020\n" );

	    //�趨���flag
	    result0 = true;

	} while(0);

	if ( !result0 ) destoryEngine();
}

void GAudioIOEngine::destoryEngine()
{
	printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::destoryEngine >> 001 \n" );
	//��ٻ�������
    if ( mMixOutputObject ) ( *mMixOutputObject )->Destroy( mMixOutputObject );

    printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::destoryEngine >> 002\n" );

	//�������
	if ( mEngineObject )
	{
		printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::destoryEngine >> 003 mChangedEnvSetting = %s  mMixOutputInterface = %s\n", ( mChangedEnvSetting ? "YES" : "NO" ), (  mMixOutputInterface == NULL ? " NULL" : "NOT NULL"                ) );

		//��ԭ��Ƶȫ������
		if ( mChangedEnvSetting ) ( *mMixOutputInterface )->SetEnvironmentalReverbProperties( mMixOutputInterface, &mOrigEnvSetting );

		printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::destoryEngine >> 004 \n" );

		//�������
		( *mEngineObject )->Destroy( mEngineObject );

		printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::destoryEngine >> 005 \n" );
	}

	printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::destoryEngine >> 006 \n" );

	//��ʼ��ȫ�ֱ���
	mEngineObject	 	= 0;
	mEngineInterface 	= 0;
	mMixOutputObject	= 0;
	mMixOutputInterface = 0;
//	mOrigEnvSetting	 	= 0;
	mChangedEnvSetting 	= false;

	printf( "@@@@@@@@@@@@@@@@@@ GAudioIOEngine::destoryEngine >> 007 \n" );
}

