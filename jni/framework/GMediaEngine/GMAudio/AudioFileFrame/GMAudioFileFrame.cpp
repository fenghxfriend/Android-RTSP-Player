#include "GMAudioFileFrame.h"

GMAudioFileFrame* GMAudioFileFrame::createNew(UsageEnvironment& env, char const* fileName, bool isSystemMusic)
{
    GMAudioFileFrame* result = new GMAudioFileFrame(env, fileName, isSystemMusic);
    if (result != NULL && result->bitsPerSample() == 0)
    {
        Medium::close(result);
        result = NULL;
    }
    
    return result;
}

GMAudioFileFrame::GMAudioFileFrame(UsageEnvironment& env, char const* fileName, bool isSystemMusic) : AudioInputDevice(env, 0, 0, 0, 0)
{
    mAudioFormat            = 0x01;
    mScaleFactor            = 1;
    mActiveFrameDuration    = 0;
    fSamplingFrequency      = 22050;
    fBitsPerSample          = 16;
    fNumChannels            = 1;
    mFrameBytes             = (fBitsPerSample / 8) * fNumChannels;
    mSampleingPeriod        = 1e6/(double)fSamplingFrequency;
    unsigned _1400bytes     = (1400 * 8) / (fNumChannels * fBitsPerSample);
    unsigned _20msc         = (unsigned)(0.02 * fSamplingFrequency);
    mNetMaxNumber           = _20msc < _1400bytes ? _20msc : _1400bytes;
    mSystemMusic			= isSystemMusic;

    if ( mSystemMusic ) mAudioConverter.loadFile( fileName );
    else mAudioXConverter.loadFile( fileName );
}

GMAudioFileFrame::~GMAudioFileFrame()
{

}

void GMAudioFileFrame::setScaleFactor(int scale)
{
    mScaleFactor = scale;
}

void GMAudioFileFrame::seekToDuration(double seekNPT, double streamDuration)
{
	if ( mSystemMusic ) mAudioConverter.seek( seekNPT );
	else mAudioXConverter.seek( seekNPT );
}

double GMAudioFileFrame::duration()
{
	if ( mSystemMusic ) { return mAudioConverter.duration(); }
	else { return mAudioXConverter.duration(); }
}

unsigned char GMAudioFileFrame::getAudioFormat()
{
    return mAudioFormat;
}

void GMAudioFileFrame::doGetNextFrame()
{
	if ( mSystemMusic )
	{
	    if ( mAudioConverter.isEOF() )
	    {
	        handleClosure(this);
	        return;
	    }
	}
	else
	{
	    if ( mAudioXConverter.isEOF() )
	    {
	        handleClosure(this);
	        return;
	    }
	}

    mSendMaxNumber = fMaxSize / mFrameBytes;
    if (mNetMaxNumber < mSendMaxNumber) mSendMaxNumber = mNetMaxNumber;
    fMaxSize = mSendMaxNumber * mFrameBytes;
    
    if ( mSystemMusic ) mAudioConverter.readData( (void*)fTo, fFrameSize, mSendMaxNumber );
    else mAudioXConverter.readData( (void*)fTo, fFrameSize, mSendMaxNumber );

    if (fPresentationTime.tv_sec != 0 || fPresentationTime.tv_usec != 0)
    {
        mFrameDurationCount         = fPresentationTime.tv_usec + mActiveFrameDuration;
        fPresentationTime.tv_sec    += mFrameDurationCount / 1000000;
        fPresentationTime.tv_usec   = mFrameDurationCount % 1000000;
    }
    else
    {
        gettimeofday(&fPresentationTime, NULL);
    }
    
    fDurationInMicroseconds = mActiveFrameDuration = (unsigned)(mSampleingPeriod * fFrameSize);
    
    fFrameSize *= mFrameBytes;
    
    afterGetting(this);
}

Boolean GMAudioFileFrame::setInputPort(int portIndex)
{
    return True;
}

double GMAudioFileFrame::getAverageLevel() const
{
    return 0.0;
}

float GMAudioFileFrame::parseAudioFileDuration(char const* fileName, bool isSystemMusic)
{
    if ( isSystemMusic ) { return GMAudioConverter::mediaDuration(fileName); }
    else { return GMAudioXConverter::mediaDuration(fileName); }
}







