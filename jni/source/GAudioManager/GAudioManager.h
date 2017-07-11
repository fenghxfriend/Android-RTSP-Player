#ifndef GAudioManager_h
#define GAudioManager_h

#include "GAudioEngine/GAudioEngine.h"
#include "GAudioFilePlayer/GAudioFilePlayer.h"
#include "GAudioStreamPlayer/GAudioStreamPlayer.h"
#include "GAudioFileRecorder/GAudioFileRecorder.h"
#include "GAudioStreamRecorder/GAudioStreamRecorder.h"
#include "GAudioHornUpdate/GAudioHornUpdate.h"
#include "GAudioSaveFile/GAudioSaveFile.h"
#include "GAudioDeleteFile/GAudioDeleteFile.h"

using namespace std;

class GAudioManager
{
public:
    static	GAudioManager &sharedInstance();
	void 	install(JNIEnv* env, jobject assetManager);
	void 	unInstall();

public:
	void 	install0();
	void 	unInstall0();

public: //delete file
	int 	deleteFile(JNIEnv* env, jstring path);

public: //play file
	int		playAssetFile(JNIEnv* env, jstring path);
	int		playCustomFile(JNIEnv* env, jstring path);
	void	stopFilePlayer();

public: //record file
	int		startRecordFile(JNIEnv* env, jstring path);
	void	stopRecordFile();

public: //horn update
	int		updateHorn(JNIEnv* env, jstring ip, jstring path, jint target, bool asset);
	int		hornUpdateAction();

public: //save file
	int		saveFile(JNIEnv* env, jstring oldFilePath, jstring newFilePath);

public: //play stream
    void 	startPlayStream();
    void 	writeStream(void *data, int size);
    void 	stopPlayStream();
    void 	savePlayStreamAddress(const char *address);

public: //record stream
    void 	startRecordStream();
    int	 	readStream(void *buffer, int maxSize);
    void 	stopRecordStream();

private:
    GAudioFilePlayer 		mFilePlayer;
    GAudioStreamPlayer 		mStreamPlayer;
    GAudioFileRecorder 		mFileRecorder;
    GAudioStreamRecorder	mStreamRecorder;
    GAudioHornUpdate 		mHornUpdate;
    GAudioSaveFile 			mSaveFile;
    GAudioDeleteFile 		mDeleteFile;
};

#endif
