#include "GAudioPlayer.h"

void GAudioPlayerQueueCallback(SLAndroidSimpleBufferQueueItf queue, void *data)
{ if ( queue && data ) ( ( ( GAudioPlayer * )data )->audioQueueCallback( queue ) ); }

void GAudioPlayer::install(SLEngineItf engine, SLObjectItf mixoutput)
{
	//��ʼ��ȫ�ֱ���
	mSleeping = false;

	//����������
	createPlayer( engine, mixoutput );
}

void GAudioPlayer::unInstall()
{
	//��ٲ�����
	destoryPlayer();
}

bool GAudioPlayer::start(GAUDIOPLAYERFUN callback, void *data)
{
	bool result = false;

	do
	{
		//�жϰ�װ�Ƿ�ɹ�
		if ( !mPlayerObject ) break;

		//�жϲ����Ƿ���Ч
		if ( !callback ) break;

		//�жϵ�ǰ�Ƿ���������
		if ( mRunning ) break;

		//�������
		mAudioQueueCallback 	= callback;
		mAudioQueueCallbackData	= data;

		//��ջ�����
		SLresult flag = ( *mPlayerBufferQueue )->Clear( mPlayerBufferQueue );
		if ( SL_RESULT_SUCCESS != flag ) break;

		//����
		flag = ( *mPlayerInterface )->SetPlayState( mPlayerInterface, SL_PLAYSTATE_PLAYING );
		if ( SL_RESULT_SUCCESS != flag ) break;
		audioQueueCallback( mPlayerBufferQueue );

		//�趨flag
		mRunning = true;
		result 	 = true;

	} while(false);

	return result;
}

bool GAudioPlayer::stop()
{
	bool result = false;

	do
	{
		//�жϰ�װ�Ƿ�ɹ�
		if ( !mPlayerObject ) break;

		//�жϵ�ǰ�Ƿ���������
		if ( !mRunning ) break;

		//ֹͣ
		SLresult flag = ( *mPlayerInterface )->SetPlayState( mPlayerInterface, SL_PLAYSTATE_STOPPED );
		if ( SL_RESULT_SUCCESS != flag ) break;

		//��ջ�����
		flag = ( *mPlayerBufferQueue )->Clear( mPlayerBufferQueue );
		if ( SL_RESULT_SUCCESS != flag ) break;

		//�趨flag
		mAudioQueueDataSize		= 0;
		mAudioQueueCallback		= 0;
		mAudioQueueCallbackData	= 0;
		mRunning 				= false;
		result 	 				= true;

	} while(false);

	return result;
}


#include <android/log.h>
#define printf(...) __android_log_print( ANDROID_LOG_VERBOSE, "GMediaFramework", __VA_ARGS__ );


void GAudioPlayer::createPlayer(SLEngineItf engine, SLObjectItf mixoutput)
{
	SLresult result	 = SL_RESULT_UNKNOWN_ERROR;
	bool 	 result0 = false;
	printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::createPlayer >> 001 \n" );
	do
	{
		//��ʼ��ȫ�ֱ���
        mPlayerObject 			= 0;
        mPlayerInterface 		= 0;
        mPlayerBufferQueue 		= 0;
        mAudioQueueDataSize		= 0;
        mAudioQueueCallback		= 0;
        mAudioQueueCallbackData	= 0;
        mRunning				= false;
        memset( mAudioQueueBuffer, 0, GAUDIOPLAYERBUFFERSIZE );

        //�жϲ����Ƿ���Ч
        if ( !engine || !mixoutput ) break;

        printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::createPlayer >> 002 \n" );

	    //������ƵԴ
	    SLDataLocator_AndroidSimpleBufferQueue 	bufferQueue = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2 };
	    SLDataFormat_PCM 						audioFormat = { SL_DATAFORMAT_PCM			, 1							  ,
	    														SL_SAMPLINGRATE_22_05		,
	    														SL_PCMSAMPLEFORMAT_FIXED_16	, SL_PCMSAMPLEFORMAT_FIXED_16 ,
	    														SL_SPEAKER_FRONT_CENTER		, SL_BYTEORDER_LITTLEENDIAN };
	    SLDataSource 							audioSource = { &bufferQueue, &audioFormat };

	    //������Ƶsink
	    SLDataLocator_OutputMix 				mixOutput 	= { SL_DATALOCATOR_OUTPUTMIX, mixoutput };
	    SLDataSink 								audioSink 	= { &mixOutput, NULL };

	    //����������
	    SLInterfaceID playerIds[3] = { SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME   };
	    SLboolean 	  playerReq[3] = { SL_BOOLEAN_TRUE	 , SL_BOOLEAN_TRUE  , SL_BOOLEAN_TRUE };
	    result = ( *engine )->CreateAudioPlayer( engine, &mPlayerObject, &audioSource, &audioSink, 3, playerIds, playerReq);
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::createPlayer >> 003 mPlayerObject = %s \n", ( mPlayerObject == NULL ? "NULL" : "NOT NULL"  ) );
	    if ( SL_RESULT_SUCCESS != result ) break;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::createPlayer >> 004 \n" );

	    //����������
	    result = ( *mPlayerObject )->Realize( mPlayerObject, SL_BOOLEAN_FALSE );
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::createPlayer >> 005 \n" );
	    if ( SL_RESULT_SUCCESS != result ) break;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::createPlayer >> 006 \n" );

	    //ȡ�ò������ӿ�
	    result = ( *mPlayerObject )->GetInterface( mPlayerObject, SL_IID_PLAY, &mPlayerInterface );
	    if ( SL_RESULT_SUCCESS != result ) break;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::createPlayer >> 007 \n" );

	    //ȡ�ò��������Ŷ���
	    result = ( *mPlayerObject )->GetInterface( mPlayerObject, SL_IID_BUFFERQUEUE, &mPlayerBufferQueue );
	    if ( SL_RESULT_SUCCESS != result ) break;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::createPlayer >> 008 \n" );

	    //ע�Ქ�Ŷ��лص�����
	    result = ( *mPlayerBufferQueue )->RegisterCallback( mPlayerBufferQueue, GAudioPlayerQueueCallback, this );
	    if ( SL_RESULT_SUCCESS != result ) break;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::createPlayer >> 009 \n" );

	    //��ʼ��ֹͣ״̬
		( *mPlayerInterface   )->SetPlayState( mPlayerInterface, SL_PLAYSTATE_STOPPED );
		( *mPlayerBufferQueue )->Clear		 ( mPlayerBufferQueue 					  );

	    //�趨���flag
	    result0 = true;
	    printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::createPlayer >> 010 \n" );

	} while(0);

	if ( !result0 ) destoryPlayer();
}

void GAudioPlayer::destoryPlayer()
{
	//printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::destoryPlayer >> 001 \n" );
    if ( mPlayerObject )
    {
    	//printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::destoryPlayer >> 002 \n" );
    	//ֹͣ
    	stop();

    	//printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::destoryPlayer >> 003\n" );
    	//��ٲ�����
        ( *mPlayerObject )->Destroy( mPlayerObject );

        //printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::destoryPlayer >> 004 \n" );

        //��ʼ��ȫ�ֱ���
        mPlayerObject 			= 0;
        mPlayerInterface 		= 0;
        mPlayerBufferQueue 		= 0;
        mAudioQueueDataSize		= 0;
        mAudioQueueCallback		= 0;
        mAudioQueueCallbackData	= 0;
        mRunning				= false;

        //printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::destoryPlayer >> 005 \n" );
        memset( mAudioQueueBuffer, 0, GAUDIOPLAYERBUFFERSIZE );

        //printf( "@@@@@@@@@@@@@@@@@@ GAudioPlayer::destoryPlayer >> 006 \n" );
    }
}

void GAudioPlayer::audioQueueCallback(SLAndroidSimpleBufferQueueItf queue)
{
	//�жϻص������Ƿ���Ч
	if ( mAudioQueueCallback )
	{
		//ִ�лص�����
		mAudioQueueCallback( mAudioQueueBuffer, &mAudioQueueDataSize, GAUDIOPLAYERBUFFERSIZE, mAudioQueueCallbackData );

		//�ж��Ƿ������
		if ( mAudioQueueDataSize > 0 && mAudioQueueDataSize <= GAUDIOPLAYERBUFFERSIZE )
		{
			( *mPlayerBufferQueue )->Enqueue( mPlayerBufferQueue, mAudioQueueBuffer, mAudioQueueDataSize );
		}
		else
		{
			//ֹͣ
			if ( !mSleeping ) stop();
		}
	}
	else
	{
		//ֹͣ
		stop();
	}
}

void GAudioPlayer::sleep()
{
	mSleeping = true;
}

void GAudioPlayer::wake()
{
	if ( !mSleeping ) return;

	mSleeping = false;
	audioQueueCallback( mPlayerBufferQueue );
}
