#ifndef GMAudioConverter_h
#define GMAudioConverter_h

#include <iostream>
#include <stdint.h>
extern "C"
{
#include "../../../ffmpeg/libavformat/avformat.h"
#include "../../../ffmpeg/libavcodec/avcodec.h"
#include "../../../ffmpeg/libswscale/swscale.h"
#include "../../../ffmpeg/libavutil/opt.h"
#include "../../../ffmpeg/libavutil/channel_layout.h"
#include "../../../ffmpeg/libavutil/samplefmt.h"
#include "../../../ffmpeg/libswresample/swresample.h"
}
#include "../../GMUtility/GMUtility.h"
#ifndef  AVCODEC_MAX_AUDIO_FRAME_SIZE
#define  AVCODEC_MAX_AUDIO_FRAME_SIZE  192000
#endif
#define  GMAudioFrameMaxSize           ( AVCODEC_MAX_AUDIO_FRAME_SIZE * 2 )
#define  GMAudioFrameBufferMax         5
#define  GMAudioResampleSampleRate     22050
#define  GMAudioResampleChannels       1
#define  GMAudioResampleFormat         AV_SAMPLE_FMT_S16
#define  GMAudioFrameBytes             2

using namespace std;

typedef struct
{
    char *data;
    int   frames;
    int   usedFrames;
    bool  idle;

    void init( bool alloc = false )
    {
        frames      = 0;
        usedFrames  = 0;
        idle        = true;
        if ( alloc ) data = ( char * )malloc( GMAudioFrameMaxSize );
        memset( data, 0, GMAudioFrameMaxSize );
    }

    short *offset( int value )
    {
        return ( short * )( data + value );
    }

    void destory()
    {
        if ( data != NULL ) { free( data ); data = NULL; }
        frames  = 0;
        idle    = true;
    }

} GMAudioFrame;

class GMAudioConverter
{
public:
    GMAudioConverter();
    ~GMAudioConverter();
    bool   loadFile(const char *path);
    void   readData(void *buffer, unsigned &actualframes, unsigned maxframes);
    void   seek(double time);
    double duration();
    bool   isEOF();
    static float  mediaDuration(char const* fileName);
    void   unloadFile();

public:
    friend void* GMAudioConverter_BackgroundWorkerTask(void *data);

private:
    void init(bool init = false);
    void destory();
    void backgroundWorkerTask();
    int  readBufferData(char *buffer, int from, int to, int lastFrames, bool lastIdle, bool carelast = true);

private:
    AVFormatContext *mFormatContext;
    AVStream        *mAudioStream;
    SwrContext      *mSwrContext;
    AVCodecContext  *mCodecContext;


    GMAudioFrame    mAudioFrames[GMAudioFrameBufferMax + 1];
    GMThread        mBackgroundWorker;
    bool            mLoadedFile;
    int             mWillReadBufferIndex;
    bool            mEOF;
    int             mAudioStreamIndex;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//state machine
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    void createStateMachine();
    void destoryStateMachine();

private:
    void keepAlive();
    void kill();
    void startWork();
    void stopWork();
    bool isWorking();
    bool isAlive();
    bool isStopWork();
    void waitingForWork();
    void setIsWorking(bool value);
    void setEndOfFile(bool value, bool lock = true);
    bool isEndOfFile(bool lock = true);

private:
    GMUtility   mUtility;
    GMLock      mLiveLock;
    GMLock      mTaskLock;
    GMLock      mSourceLock;
    bool        mStopWorkWhileWorking;
    bool        mIsWorking;
    bool        mIsEndOfFile;
    bool		mNewTaskComming;
};

#endif
