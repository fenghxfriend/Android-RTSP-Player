#include "GMPCMMediaSink.h"
#include "../../../../source/GFramework.h"

#if	GMPCM_TIMESTAMP_ENABLE
void GMPCMGetNextFrameTask(void *data)
{
	if ( data ) ( ( GMPCMMediaSink * )data )->getNextFrame();
}

void GMPCMMediaSink::getNextFrame()
{
	continuePlaying();
}
#endif

GMPCMMediaSink* GMPCMMediaSink::createNew(UsageEnvironment& env, void *delegate, void *callback)
{
    return new GMPCMMediaSink(env, delegate, callback);
}

GMPCMMediaSink::GMPCMMediaSink(UsageEnvironment& env, void *delegate, void *callback) : MediaSink(env)
{
    mStop               = false;
    mIsRunning          = false;
    mAlive              = true;
    mPausing			= false;
    mCallbackDelegate   = delegate;
    mStopCallback       = callback;
    mDataBuffer16Ptr    = (short *)mDataBuffer;
    
    mAudioPlayer.install();
    mAudioPlayer.start();

#if	GMPCM_TIMESTAMP_ENABLE
    mFirstFrame                     = true;
    mLastPresentationTime.tv_sec    = 0;
    mLastPresentationTime.tv_usec   = 0;
    mPresentationTime0              = 0;
    mPresentationTime1              = 0;
    mGetNextFrameTask               = NULL;
    mUsageEnvironment               = &env;
#endif
}

GMPCMMediaSink::~GMPCMMediaSink()
{
#if GMPCM_TIMESTAMP_ENABLE

    if ( mUsageEnvironment != NULL && mGetNextFrameTask != NULL )
        mUsageEnvironment->taskScheduler().unscheduleDelayedTask( mGetNextFrameTask );

    mFirstFrame                     = true;
    mLastPresentationTime.tv_sec    = 0;
    mLastPresentationTime.tv_usec   = 0;
    mPresentationTime0              = 0;
    mPresentationTime1              = 0;
    mGetNextFrameTask               = NULL;
    mUsageEnvironment               = NULL;

#endif

    mAudioPlayer.stop();
    mAudioPlayer.unInstall();
}

void GMPCMMediaSink::pause(bool v)
{
	mPausing = v;
}

void GMPCMMediaSink::stop()
{
    if ( mIsRunning )
    {
        mStop = true;
    }
    else
    {
        mStop       = false;
        mIsRunning  = false;
        mAlive      = false;
        
        if (mStopCallback != NULL)
        {
            ( ( void (*)(void *) ) mStopCallback )( mCallbackDelegate );
            //mStopCallback = NULL;
        }
    }
}

void GMPCMMediaSink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime, unsigned durationInMicroseconds)
{
    if ( !mAlive ) return;
    
    mIsRunning = true;
    
    mDataBuffer16Length = ( frameSize / 2 );
    for( mDataBufferIndex = 0; mDataBufferIndex < mDataBuffer16Length; mDataBufferIndex++ )
        mDataBuffer16Ptr[mDataBufferIndex] = ntohs(mDataBuffer16Ptr[mDataBufferIndex]);
    
    if ( !mPausing )
    {
#if	DO_DEBUG
        printf( "RTSP >> PCM >> decode >> size = %d \n", frameSize );
#endif
    	mAudioPlayer.feedData(mDataBuffer16Ptr, frameSize);
    }

    if ( !mStop )
    {
#if GMPCM_TIMESTAMP_ENABLE

        if ( !mFirstFrame )
        {
            mPresentationTime0    = presentationTime.tv_sec * 1000000 + presentationTime.tv_usec;
            mPresentationTime1    = mLastPresentationTime.tv_sec * 1000000 + mLastPresentationTime.tv_usec;
            mLastPresentationTime = presentationTime;

            if ( mPresentationTime0 > mPresentationTime1 )
            {
                if ( mGetNextFrameTask != NULL )
                    mUsageEnvironment->taskScheduler().rescheduleDelayedTask(mGetNextFrameTask, mPresentationTime0 - mPresentationTime1, GMPCMGetNextFrameTask, this );
                else
                    mGetNextFrameTask = mUsageEnvironment->taskScheduler().scheduleDelayedTask( mPresentationTime0 - mPresentationTime1, GMPCMGetNextFrameTask, this );
            }
            else
            {
                continuePlaying();
            }
        }
        else
        {
            mFirstFrame = false;
            mLastPresentationTime = presentationTime;

            //继续下一个frame
            continuePlaying();
        }

#else
        //继续下一个frame
        continuePlaying();
#endif
    }
    else
    {
        mStop       = false;
        mIsRunning  = false;
        mAlive      = false;

        if (mStopCallback != NULL)
        {
            ( ( void (*)(void *) ) mStopCallback )( mCallbackDelegate );
//            mStopCallback = NULL;
        }
    }
}

void GMPCMMediaSink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime, unsigned durationInMicroseconds)
{
    GMPCMMediaSink *sink = (GMPCMMediaSink*)clientData;
    sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

Boolean GMPCMMediaSink::continuePlaying()
{
    if (fSource)
    {
        fSource->getNextFrame(mDataBuffer, GMPCMDataBufferSize, afterGettingFrame, this, onSourceClosure, this);
        return True;
    }
    return False;
}

