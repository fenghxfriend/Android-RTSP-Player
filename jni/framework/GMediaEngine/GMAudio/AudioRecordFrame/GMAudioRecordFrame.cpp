#include "GMAudioRecordFrame.h"
#include "../RTSPServer/GMAudioRTSPServer.h"
#include "../AudioRecorder/GMAudioRecorder.h"

GMAudioRecordFrame::GMAudioRecordFrame(UsageEnvironment& env): FramedSource(env)
{
    mAudioRecorder.install();
    mAudioRecorder.start();

    //初始化全局变量
    mActiveFrameDuration    = 0;
    mFrameBytes             = ( ( 16 /*BitsPerSample*/ ) / 8) * ( 1/*NumChannels*/ );
    mSampleingPeriod        = 1e6/( (double)22050 /*SamplingFrequency*/ );
    unsigned _1400bytes     = (1400 * 8) / ( ( 1/*NumChannels*/ ) * ( 16 /*BitsPerSample*/ ) );
    unsigned _20msc         = (unsigned)(0.02 * ( 22050 /*SamplingFrequency*/ ) );
    mNetMaxNumber           = _20msc < _1400bytes ? _20msc : _1400bytes;
}

GMAudioRecordFrame::~GMAudioRecordFrame()
{
    mAudioRecorder.stop();
    mAudioRecorder.unInstall();
}

void GMAudioRecordFrame::doGetNextFrame()
{
	//解析本次读取数据量
    mSendMaxNumber = fMaxSize / mFrameBytes;
    if (mNetMaxNumber < mSendMaxNumber) mSendMaxNumber = mNetMaxNumber;

	//读取数据
    mActualReadSize = mAudioRecorder.read( (void*)fTo, mSendMaxNumber * mFrameBytes );
	fFrameSize 		= mActualReadSize;
    fMaxSize 		= mActualReadSize;

    //解析时间信息
    gettimeofday( &fPresentationTime, 0 );
    int timeinc = fFrameSize * 1000 / ( 1/*NumChannels*/ ) /( ( 22050 /*SamplingFrequency*/ ) /1000) /2;
    while (fPresentationTime.tv_usec < timeinc)
    {
        fPresentationTime.tv_sec -= 1;
        timeinc -= 1000000;
    }
    fPresentationTime.tv_usec -= timeinc;

    //取帧结束
    afterGetting(this);
}

