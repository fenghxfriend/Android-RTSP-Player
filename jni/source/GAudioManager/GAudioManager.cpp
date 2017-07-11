#include "GAudioManager.h"

GAudioManager &GAudioManager::sharedInstance()
{
	static GAudioManager instance;
	return instance;
}

void GAudioManager::install(JNIEnv* env, jobject assetManager)
{
	GAudioEngine::sharedInstance().install();
	mFilePlayer.install( env, assetManager );
	mStreamPlayer.install();
	mFileRecorder.install();
	mStreamRecorder.install();
	mHornUpdate.install( env, assetManager );
	mSaveFile.install();
	mDeleteFile.install();
}

void GAudioManager::install0()
{
	GAudioEngine::sharedInstance().install0();
	mStreamPlayer.install();
}

void GAudioManager::unInstall0()
{
	mStreamPlayer.unInstall();
	GAudioEngine::sharedInstance().unInstall0();
}

void GAudioManager::unInstall()
{
	mDeleteFile.unInstall();
	mSaveFile.unInstall();
	mHornUpdate.unInstall();
	mStreamRecorder.unInstall();
	mFileRecorder.unInstall();
	mStreamPlayer.unInstall();
	mFilePlayer.unInstall();
	GAudioEngine::sharedInstance().unInstall();
}

//delete file
int GAudioManager::deleteFile(JNIEnv* env, jstring path)
{
	return mDeleteFile.deleteFile( env, path );
}

//play file
int  GAudioManager::playAssetFile(JNIEnv* env, jstring path)
{
	return mFilePlayer.playAssetFile( env, path );
}

int	 GAudioManager::playCustomFile(JNIEnv* env, jstring path)
{
	return mFilePlayer.playCustomFile( env, path );
}

void GAudioManager::stopFilePlayer()
{
	mFilePlayer.stop();
}

//record file
int	 GAudioManager::startRecordFile(JNIEnv* env, jstring path)
{
	return mFileRecorder.start( env, path );
}

void GAudioManager::stopRecordFile()
{
	mFileRecorder.stop();
}

//horn update
int GAudioManager::updateHorn(JNIEnv* env, jstring ip, jstring path, jint target, bool asset)
{
	if ( !asset )
	{
		return mHornUpdate.updateCustomFile( env, ip, path, target );
	}
	else
	{
		return mHornUpdate.updateAssetFile( env, ip, path, target );
	}
}

int GAudioManager::hornUpdateAction()
{
	return mHornUpdate.currentAction();
}

//save file
int GAudioManager::saveFile(JNIEnv* env, jstring oldFilePath, jstring newFilePath)
{
	return mSaveFile.saveFile( env, oldFilePath, newFilePath );
}

//play stream
void GAudioManager::startPlayStream()
{
	mStreamPlayer.start();
}

void GAudioManager::writeStream(void *data, int size)
{
	mStreamPlayer.write( data, size );
}

void GAudioManager::stopPlayStream()
{
	mStreamPlayer.stop();
}

void GAudioManager::savePlayStreamAddress(const char *address)
{
	mStreamPlayer.setRTSPIPAddress( address );
}

//record stream
void GAudioManager::startRecordStream()
{
	mStreamRecorder.start();
}

int	 GAudioManager::readStream(void *buffer, int maxSize)
{
	return mStreamRecorder.read( buffer, maxSize );
}

void GAudioManager::stopRecordStream()
{
	mStreamRecorder.stop();
}

