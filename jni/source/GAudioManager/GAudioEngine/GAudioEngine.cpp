#include "GAudioEngine.h"

GAudioEngine &GAudioEngine::sharedInstance()
{
	static GAudioEngine instance;
	return instance;
}

void GAudioEngine::install()
{
	//��װ����
	mEngine.install();

	//��װ¼����
	mRecorder.install( mEngine.engine() );

	//��װ������
	mPlayer.install( mEngine.engine(), mEngine.mixOutput() );
}

void GAudioEngine::install0()
{
	//��װ����
	mEngine.install();

	//��װ������
	mPlayer.install( mEngine.engine(), mEngine.mixOutput() );
}

void GAudioEngine::unInstall0()
{
	//ж�ز�����
	mPlayer.unInstall();

	//ж������
	mEngine.unInstall();
}

void GAudioEngine::unInstall()
{
	//ж�ز�����
	mPlayer.unInstall();

	//ж��¼����
	mRecorder.unInstall();

	//ж������
	mEngine.unInstall();
}

bool GAudioEngine::startPlayback(GAUDIOPLAYERFUN callback, void *data)
{
	return mPlayer.start( callback, data );
}

void GAudioEngine::sleepPlayer()
{
	mPlayer.sleep();
}

void GAudioEngine::wakePlayer()
{
	mPlayer.wake();
}

bool GAudioEngine::stopPlayback()
{
	return mPlayer.stop();
}

bool GAudioEngine::startRecord()
{
	return mRecorder.start();
}

bool GAudioEngine::stopRecord()
{
	return mRecorder.stop();
}

int	 GAudioEngine::readRecordedData(void *buffer, int size)
{
	return mRecorder.read( buffer, size );
}

