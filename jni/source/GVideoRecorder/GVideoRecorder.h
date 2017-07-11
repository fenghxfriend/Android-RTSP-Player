#ifndef GVideoRecorder_h
#define GVideoRecorder_h

#include <jni.h>
#include <memory>
#include <string>

extern "C"
{
#include "../../framework/ffmpeg/libavformat/avformat.h"
#include "../../framework/ffmpeg/libavcodec/avcodec.h"
#include "../../framework/ffmpeg/libswscale/swscale.h"
#include "../../framework/ffmpeg/libavutil/opt.h"
#include "../../framework/ffmpeg/libavutil/channel_layout.h"
#include "../../framework/ffmpeg/libavutil/samplefmt.h"
#include "../../framework/ffmpeg/libswresample/swresample.h"
}
#include "GVideoRecordBuffer/GVideoRecordBuffer.h"
#include "GVideoRecordUtility/GVideoRecordUtility.h"
#include "../GDataConverter/GDataConverter.h"

using namespace std;

class GVideoRecorder
{
public:
    static	GVideoRecorder &sharedInstance();
	void 					install();
	void 					unInstall();
	void					start(JNIEnv* env, jstring filePath);
	void					stop();
	void					write(void *data, int size);
	int						RTSPClientDetect();
	void					setVideoSize(int width, int height);
	friend void				GVideoRecorderRunLoop(void *data);

private:
	void					startResponse(bool success);
	void					stopResponse(bool success);
	void					runLoop();
	void					stop(bool wait);

private:
	bool					mRunning;
	int						mRTSPClientDetectFlag; // 1:start   0:none   -1:stop
	GVideoRecordUtility		mUtility;
	GVIDEORCDLock			mSourceLock;
	GVIDEORCDLock			mLiveLock;
	GVideoRecordBuffer		mDataBuffer;
	GVIDEORCDThread			mBackgroundWorker;
	char					mRecordFilePath[256];
	GDataConverter			mConverter;
	bool					mLiveLocked;

//=======================================================================================================
//
//ÊÓÆµÂ¼ÖÆ
//
//=======================================================================================================
private:
	void					initRecord(bool init);
	bool					prepareRecord();
	bool					recordRunLoop();
	void					finishRecord();

private:
	int						mVideoWidth;
	int						mVideoHeight;
	AVFormatContext			*mAVFormatContext;
	AVOutputFormat			*mAVOutputFormat;
	AVStream				*mAVStream;
	int						mActivePacketSize;
	char					mPacketData[GVideoRecordPacketMaxSize];
};

#endif
