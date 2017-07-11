#include "../../../live555/liveMedia/include/uLawAudioFilter.hh"
#include "../../../live555/liveMedia/include/SimpleRTPSink.hh"
#include "GMAudioFileSession.h"
#include "../AudioFileFrame/GMAudioFileFrame.h"

GMAudioFileSession* GMAudioFileSession::createNew(UsageEnvironment& env, char const* fileName, bool isSystemMusic, Boolean reuseFirstSource, Boolean convertToULaw)
{
    return new GMAudioFileSession(env, fileName, reuseFirstSource, convertToULaw, isSystemMusic);
}

GMAudioFileSession::GMAudioFileSession(UsageEnvironment& env, char const* fileName, Boolean reuseFirstSource, Boolean convertToULaw, bool isSystemMusic)
: FileServerMediaSubsession(env, fileName, reuseFirstSource), fConvertToULaw(convertToULaw), mIsSystemMusic( isSystemMusic )
{
}

GMAudioFileSession::~GMAudioFileSession()
{
    
}

void GMAudioFileSession::seekStreamSource(FramedSource* inputSource, double seekNPT, double streamDuration)
{
    GMAudioFileFrame *fileFrameSource = NULL;
    if (fBitsPerSample == 16)
    {
        fileFrameSource = ( GMAudioFileFrame * )(((FramedFilter*)inputSource)->inputSource());
    }
    else
    {
        fileFrameSource = ( GMAudioFileFrame * )inputSource;
    }
    
    fileFrameSource->seekToDuration(seekNPT, streamDuration);
}

void GMAudioFileSession::setStreamSourceScale(FramedSource* inputSource, float scale)
{
    int iScale = (int)scale;
    GMAudioFileFrame *fileFrameSource = NULL;
    if (fBitsPerSample == 16)
    {
        fileFrameSource = ( GMAudioFileFrame * )(((FramedFilter*)inputSource)->inputSource());
    }
    else
    {
        fileFrameSource = ( GMAudioFileFrame * )inputSource;
    }
    
    fileFrameSource->setScaleFactor(iScale);
}

FramedSource* GMAudioFileSession::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate)
{
    FramedSource* resultSource = NULL;
    do
    {
        GMAudioFileFrame *fileFrameSource = GMAudioFileFrame::createNew(envir(), fFileName, mIsSystemMusic);
        
        if (fileFrameSource == NULL) break;
        fAudioFormat    = fileFrameSource->getAudioFormat();
        fBitsPerSample  = fileFrameSource->bitsPerSample();
        if ( !(fBitsPerSample == 4 || fBitsPerSample == 8 || fBitsPerSample == 16) ) break;
        
        fSamplingFrequency      = fileFrameSource->samplingFrequency();
        fNumChannels            = fileFrameSource->numChannels();
        unsigned bitsPerSecond  = fSamplingFrequency * fBitsPerSample * fNumChannels;
        fFileDuration           = fileFrameSource->duration();
        
        if (fBitsPerSample == 16)
        {
            if (fConvertToULaw)
            {
                resultSource = uLawFromPCMAudioSource::createNew(envir(), fileFrameSource, 1);
                bitsPerSecond /= 2;
            }
            else
            {
                resultSource = EndianSwap16::createNew(envir(), fileFrameSource);
            }
        }
        else
        {
            resultSource = fileFrameSource;
        }
        
        estBitrate = (bitsPerSecond + 500) / 1000;
        return resultSource;
        
    } while (0);
    
    Medium::close(resultSource);
    return NULL;
}

RTPSink* GMAudioFileSession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic,FramedSource* )
{
    do
    {
        char const      *mimeType;
        unsigned char   payloadFormatCode;
        if (fAudioFormat == 0x01)
        {
            if (fBitsPerSample == 16)
            {
                if (fConvertToULaw)
                {
                    mimeType = "PCMU";
                    if (fSamplingFrequency == 8000 && fNumChannels == 1)
                    {
                        payloadFormatCode = 0;
                    }
                    else
                    {
                        payloadFormatCode = rtpPayloadTypeIfDynamic;
                    }
                }
                else
                {
                    mimeType = "L16";
                    if (fSamplingFrequency == 44100 && fNumChannels == 2)
                    {
                        payloadFormatCode = 10;
                    }
                    else if (fSamplingFrequency == 44100 && fNumChannels == 1)
                    {
                        payloadFormatCode = 11;
                    }
                    else
                    {
                        payloadFormatCode = rtpPayloadTypeIfDynamic;
                    }
                }
            }
            else
            {
                mimeType = "L8";
                payloadFormatCode = rtpPayloadTypeIfDynamic;
            }
        }
        else if (fAudioFormat == 0x07)
        {
            mimeType = "PCMU";
            if (fSamplingFrequency == 8000 && fNumChannels == 1)
            {
                payloadFormatCode = 0;
            }
            else
            {
                payloadFormatCode = rtpPayloadTypeIfDynamic;
            }
        }
        else if (fAudioFormat == 0x06)
        {
            mimeType = "PCMA";
            if (fSamplingFrequency == 8000 && fNumChannels == 1)
            {
                payloadFormatCode = 8;
            }
            else
            {
                payloadFormatCode = rtpPayloadTypeIfDynamic;
            }
        }
        else if (fAudioFormat == 0x11)
        {
            mimeType = "DVI4";
            payloadFormatCode = rtpPayloadTypeIfDynamic;
            if (fNumChannels == 1)
            {
                if (fSamplingFrequency == 8000)
                {
                    payloadFormatCode = 5;
                }
                else if (fSamplingFrequency == 16000)
                {
                    payloadFormatCode = 6;
                }
                else if (fSamplingFrequency == 11025)
                {
                    payloadFormatCode = 16;
                }
                else if (fSamplingFrequency == 22050)
                {
                    payloadFormatCode = 17;
                }
            }
        }
        else
        {
            break;
        }
        
        return SimpleRTPSink::createNew(envir(), rtpGroupsock, payloadFormatCode, fSamplingFrequency, "audio", mimeType, fNumChannels);

    } while (0);
    
    return NULL;
}

void GMAudioFileSession::closeStreamSource(FramedSource *inputSource)
{
    Medium::close(inputSource);
}

void GMAudioFileSession::testScaleFactor(float& scale)
{
    if (fFileDuration <= 0.0)
    {
        scale = 1;
    }
    else
    {
        int iScale = scale < 0.0 ? (int)(scale - 0.5) : (int)(scale + 0.5);
        if (iScale == 0) iScale = 1;
        scale = (float)iScale;
    }
}

float GMAudioFileSession::duration() const
{
    return fFileDuration;
}

bool GMAudioFileSession::parseAudioFileDuration(char const* fileName, bool isSystemMusic)
{
    bool result = false;
    
    fFileDuration = GMAudioFileFrame::parseAudioFileDuration(fileName, isSystemMusic);
    if (fFileDuration > 0.0f)
    {
        result = true;
    }
    
    return result;
}
