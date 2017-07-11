#ifndef GMH264MediaSink_h
#define GMH264MediaSink_h

#include <iostream>
#include <memory>
#include <string>
#include "../../../live555/liveMedia/include/liveMedia.hh"
#include "../../../live555/BasicUsageEnvironment/include/BasicUsageEnvironment.hh"
#include "../../../live555/groupsock/include/GroupsockHelper.hh"
#include "../H264Decoder/GMH264Decoder.h"
#include "../../../../source/GVideoPlayer/GVideoPlayer.h"

using namespace std;


class GMH264MediaSink : public MediaSink
{
public:
    static GMH264MediaSink *createNew(UsageEnvironment& env
    , char const* sPropParameterSetsStr1
	, char const* sPropParameterSetsStr2 = NULL
	, char const* sPropParameterSetsStr3 = NULL);

    virtual void dealData(unsigned char * data, unsigned dataSize, struct timeval presentationTime);

protected:
    GMH264MediaSink(UsageEnvironment& env
	,char const* sPropParameterSetsStr1
	,char const* sPropParameterSetsStr2
	,char const* sPropParameterSetsStr3);
    virtual ~GMH264MediaSink();

protected:
    virtual Boolean continuePlaying();
protected:

    void stop();

    static void afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
                                  struct timeval presentationTime, unsigned durationInMicroseconds);
    virtual void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime,
                           unsigned durationInMicroseconds);
    virtual void afterGettingFrame1(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime,
                           unsigned durationInMicroseconds);



    void getNextFrame();

    friend void GMH264GetNextFrameFunc(void *data);


protected:
    bool                    m_bStop;
    char const             *m_SPropParameterSetsStr[3];
    bool                    m_bHaveWrittenFirstFrame;
    bool                    m_bHaveHeader;
    struct timeval          m_tlastPresentationTime;
    struct timeval          m_tNextReceiveTime;

    unsigned char          *m_pbBuffer;
    int                     m_nBufferSize;

    GMH264Decoder          *m_pH264Decoder;
    TaskToken               m_pfGetNextFrameTask;

};

#endif
