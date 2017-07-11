#include "GMH264MediaSink.h"
#include "../../../../source/GVideoRecorder/GVideoRecorder.h"
#include "../../../../source/GFramework.h"

static const uint8_t prefix_GMH264[] = {0x00, 0x00, 0x00, 0x01};
#define GMH264MediaSink_HDSIZE (sizeof(prefix_GMH264))

#ifdef __cplusplus
extern "C"
{
#endif
void GVideoPlayer_C_OnRender( unsigned char	* data, int size, int width, int height, int first );
#ifdef __cplusplus
}
#endif

void GMH264GetNextFrameFunc(void *data)
{
	if ( data ) ( ( GMH264MediaSink * )data )->getNextFrame();
}

void GMH264MediaSink::getNextFrame()
{
    continuePlaying();
}

GMH264MediaSink::GMH264MediaSink(UsageEnvironment& env
	,char const* sPropParameterSetsStr1
	,char const* sPropParameterSetsStr2
	,char const* sPropParameterSetsStr3) :
	MediaSink(env),
	m_pfGetNextFrameTask(NULL),
	m_bHaveWrittenFirstFrame(false),
	m_bStop(false),
	m_bHaveHeader(false)
{

	USE_LOG;
	m_SPropParameterSetsStr[0] = sPropParameterSetsStr1;
	m_SPropParameterSetsStr[1] = sPropParameterSetsStr2;
	m_SPropParameterSetsStr[2] = sPropParameterSetsStr3;

	gettimeofday(&m_tlastPresentationTime, NULL);

	m_pH264Decoder = new GMH264Decoder();

    m_nBufferSize = MAX_H264_FRAME_SIZE;
	m_pbBuffer = new unsigned char[MAX_H264_FRAME_SIZE];

	gettimeofday(&m_tNextReceiveTime, NULL);


}

GMH264MediaSink::~GMH264MediaSink()
{
	USE_LOG;
	stop();
	if ( m_pfGetNextFrameTask != NULL)
	{
		envir().taskScheduler().unscheduleDelayedTask( m_pfGetNextFrameTask );
		m_pfGetNextFrameTask = NULL;
	}
	if(m_pbBuffer != NULL)
	{
		delete[] m_pbBuffer;
		m_pbBuffer = NULL;
	}

	if(m_pH264Decoder != NULL)
	{
		delete m_pH264Decoder;
		m_pH264Decoder = NULL;
	}
}

GMH264MediaSink * GMH264MediaSink::createNew(UsageEnvironment& env
	, char const* sPropParameterSetsStr1
	, char const* sPropParameterSetsStr2
	, char const* sPropParameterSetsStr3)
{
    return new GMH264MediaSink(env, sPropParameterSetsStr1, sPropParameterSetsStr2, sPropParameterSetsStr3);
}

void GMH264MediaSink::stop()
{
	LOGI( "GMH264MediaSink::stop" );
	m_bStop = true;
}
/*
void GMH264MediaSink::dealData(unsigned char * data, unsigned dataSize, struct timeval presentationTime)
{
	// define the start code :
	unsigned char start_code[4] = {0x00, 0x00, 0x00, 0x01};
	static bool bFirstFrame = true;
	// Add to our decoder queue:
	if ( (data != NULL) && (dataSize > 0))
	{
		//a new frame is comming, post the last frame to decode queue
		if(m_tlastPresentationTime.tv_usec != presentationTime.tv_usec || m_tlastPresentationTime.tv_sec != presentationTime.tv_sec)
		{
			// add the last frame to the decode queue:
			if((m_pH264Decoder != NULL) && (m_ptH264Frame->size > 0) && !bFirstFrame)
			{
				m_pH264Decoder->addToQueue(m_ptH264Frame->data, m_ptH264Frame->size);
			}
			else
			{
				if(!bFirstFrame)
				{
					LOGE("add the last frame to the decode queue failed! The decoder is %p or data size is %d!",m_pH264Decoder, m_ptH264Frame->size);
				}
				else
				{
					bFirstFrame = false;
				}
			}

			// Write the start code to the buffer in front:
			// Reset the buffer size
			m_ptH264Frame->size = 0;
			memcpy(m_ptH264Frame->data, start_code, 4);
			m_ptH264Frame->size += 4;

			// Write the h264 data to the buffer:
			if((m_ptH264Frame->size + dataSize) < MAX_H264_FRAME_SIZE)
			{
				memcpy(m_ptH264Frame->data + m_ptH264Frame->size, data, dataSize);
				m_ptH264Frame->size += dataSize;
			}
			else
			{
				LOGE("The frame buffer is not enough!");
				m_ptH264Frame->size = 0;
			}

			//save the frame flag:
			m_tlastPresentationTime = presentationTime;
		}
		// receive some data which is owned by the frame saved before (m_tlastPresentationTime):
		else
		{
			// Write the h264 data to the buffer:
			if((m_ptH264Frame->size + dataSize) < MAX_H264_FRAME_SIZE)
			{
				memcpy(m_ptH264Frame->data + m_ptH264Frame->size, data, dataSize);
				m_ptH264Frame->size += dataSize;
			}
			else
			{
				LOGE("The buffer is not enough!");
				m_ptH264Frame->size = 0;
			}

		}

	}

}
*/

void GMH264MediaSink::dealData(unsigned char * data, unsigned dataSize, struct timeval presentationTime)
{
	// define the start code :
	unsigned char start_code[4] = {0x00, 0x00, 0x00, 0x01};
	// Add to our decoder queue:
	if ( (data != NULL) && (dataSize > 0))
	{
		//a new frame is comming, post it to decode queue
 		GMBufferNode *ptH264Frame = new GMBufferNode;
	    if(ptH264Frame == NULL)
		{
			return;
		}
	    ptH264Frame->data = (unsigned char*)malloc(dataSize + 4);
	    if(ptH264Frame->data == NULL)
		{
			delete ptH264Frame;
			return;
	    }

		// Write the start code to the buffer in front:
		// Reset the buffer size
		ptH264Frame->size = 0;
		memcpy(ptH264Frame->data, start_code, 4);
		ptH264Frame->size += 4;

		// Write the h264 data to the buffer:
		memcpy(ptH264Frame->data + ptH264Frame->size, data, dataSize);
		ptH264Frame->size += dataSize;

		if(m_pH264Decoder != NULL)
		{
			m_pH264Decoder->addToQueue(ptH264Frame);
		}
		else
		{
			LOGE("add the last frame to the decode queue failed! The decoder is null!");
			return;
		}
	}

}

void GMH264MediaSink::afterGettingFrame1(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime, unsigned durationInMicroseconds)
{
	if (numTruncatedBytes > 0)
	{
	    LOGE("GMH264MediaSink::afterGettingFrame(): \
			The input frame data was too large for\
			our buffer size (%d).  %d bytes of \
			trailing data was dropped!\
			Correct this by increasing \
			the \"bufferSize\" parameter in\
			the \"createNew()\" call to at least %d"
	            , MAX_H264_FRAME_SIZE,numTruncatedBytes,MAX_H264_FRAME_SIZE + numTruncatedBytes );
	}

	if (m_bStop)
	{
		if ( m_pfGetNextFrameTask != NULL)
		{
			envir().taskScheduler().unscheduleDelayedTask( m_pfGetNextFrameTask );
			m_pfGetNextFrameTask = NULL;
		}
		// The output file has closed.  Handle this the same way as if the input source had closed:
		if (fSource != NULL)
		{
			fSource->stopGettingFrames();
		}
		onSourceClosure();
		return;
	}

	if(m_pbBuffer != NULL)
	{
		dealData(m_pbBuffer, frameSize, presentationTime);
	}

	// Then try getting the next frame:
	// Figure out the time at which the next packet should be sent, based
	// on the duration of the payload that we just read:
	m_tNextReceiveTime.tv_usec += durationInMicroseconds;
	m_tNextReceiveTime.tv_sec += m_tNextReceiveTime.tv_usec/1000000;
	m_tNextReceiveTime.tv_usec %= 1000000;

	struct timeval timeNow;
	gettimeofday(&timeNow, NULL);
	int secsDiff = m_tNextReceiveTime.tv_sec - timeNow.tv_sec;
	int64_t uSecondsToGo = secsDiff*1000000 + (m_tNextReceiveTime.tv_usec - timeNow.tv_usec);
	if (uSecondsToGo < 0 || secsDiff < 0)
	{
		// sanity check: Make sure that the time-to-delay is non-negative:
		uSecondsToGo = 0;
	}

	// Delay this amount of time:
	if(m_pfGetNextFrameTask == NULL)
	{
		if(m_tlastPresentationTime.tv_usec == presentationTime.tv_usec && m_tlastPresentationTime.tv_sec == presentationTime.tv_sec)
		{
			getNextFrame();
		}
		else
		{
			m_pfGetNextFrameTask = envir().taskScheduler().scheduleDelayedTask(uSecondsToGo, (TaskFunc*)GMH264GetNextFrameFunc, this);
		}
	}
	else
	{
		envir().taskScheduler().rescheduleDelayedTask(m_pfGetNextFrameTask, uSecondsToGo, GMH264GetNextFrameFunc, this );
	}
}


void GMH264MediaSink::afterGettingFrame(unsigned frameSize,  unsigned numTruncatedBytes,  struct timeval presentationTime, unsigned durationInMicroseconds)
{

	if (!m_bHaveWrittenFirstFrame)
	{
		// If we have NAL units encoded in "sprop parameter strings", prepend these to the file:
		for (unsigned j = 0; j < 3; ++j)
		{
			unsigned numSPropRecords;
			struct timeval preTime;
			SPropRecord* sPropRecords = parseSPropParameterSets(m_SPropParameterSetsStr[j], numSPropRecords);
			for (unsigned i = 0; i < numSPropRecords; ++i)
			{
				gettimeofday(&preTime, NULL);
				dealData(sPropRecords[i].sPropBytes, sPropRecords[i].sPropLength, preTime);
			}
			delete[] sPropRecords;
		}
		m_bHaveWrittenFirstFrame = true; // for next time
	}

	// Call the parent class to complete the normal file write with the input data:
	afterGettingFrame1(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

void GMH264MediaSink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime, unsigned durationInMicroseconds)
{
    GMH264MediaSink *sink = ( GMH264MediaSink * )clientData;
    sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

Boolean GMH264MediaSink::continuePlaying()
{
	gettimeofday(&m_tNextReceiveTime, NULL);

	if (fSource == NULL || m_pbBuffer == NULL) return false;

	fSource->getNextFrame(m_pbBuffer, m_nBufferSize,
			afterGettingFrame, this,
			onSourceClosure, this);

	return true;
}


