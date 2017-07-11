#ifndef GAudioHornUpdate_h
#define GAudioHornUpdate_h

#include <jni.h>
#include <assert.h>
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <memory>
#include <string>
#include "../GAudioUtility/GAudioUtility.h"
#include "../../GDataConverter/GDataConverter.h"

using namespace std;

class GAudioHornUpdate
{
public:
	void 	install(JNIEnv* env, jobject assetManager);
	void 	unInstall();
	int		updateCustomFile(JNIEnv* env, jstring ip, jstring path, jint target);
	int		updateAssetFile(JNIEnv* env, jstring ip, jstring path, jint target);
	int		currentAction();
	friend	void GAudioHornUpdateFileServerRunLoop(void *data);

private:
	void	init();
	void	destory();
	int		update(JNIEnv* env, jstring ip, jstring path, jint target, bool asset);
	void	runLoop();
	int 	parseFileSize(AAsset *asset, FILE *file);
	int 	copyDataFromFileToBuffer();

private:
	GDataConverter	mConverter;
	GAMUThread		mThread;
	GAudioUtility	mUtility;
	GAMULock		mLock;
	int				mActionSeed;
	bool			mRunning;
	AAssetManager	*mAssetManager;
	AAsset			*mAAsset;
	FILE 			*mCustomFile;
	bool			mIsAssetFile;
	char			mFilePath[256];
	char			mLocalIPAddress[256];
	int				mUpdateTarget;
	char			mFileServerBuffer[1024];
};

#endif
