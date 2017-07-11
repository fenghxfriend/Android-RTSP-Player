#ifndef GAudioFilePlayer_h
#define GAudioFilePlayer_h

#include <memory>
#include <string>
#include <jni.h>
#include <assert.h>
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "../../GDataConverter/GDataConverter.h"
#include "../GAudioUtility/GAudioUtility.h"

using namespace std;

class GAudioFilePlayer
{
public:
	void 	install(JNIEnv* env, jobject assetManager);
	void 	unInstall();
	int		playAssetFile(JNIEnv* env, jstring path);
	int		playCustomFile(JNIEnv* env, jstring path);
	void	stop();
	friend 	void GAudioFilePlayerCallback( void *buffer, int *size, int maxsize, void *data );

private:
	int		play(JNIEnv* env, jstring path, bool asset);
	void	playerCallback(void *buffer, int *size, int maxsize);
	void	stop(bool notifyUser);
	void	stop(bool notifyUser, bool stopPlayer);

private:
	bool			mRunning;
	GAudioUtility	mUtility;
	GAMULock		mLock;
	GDataConverter	mConverter;
	int				mActionSeed;
	AAssetManager	*mAssetManager;
	AAsset			*mAAsset;
	FILE 			*mCustomFile;
	bool			mPlayingAssetFile;
	char			mFilePath[256];
};

#endif
