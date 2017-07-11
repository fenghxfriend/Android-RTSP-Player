#include "GMH264Decoder.h"
#include "../../../../source/GVideoPlayer/GVideoPlayer.h"
#include <sys/prctl.h>
#include <unistd.h>

#ifndef _ANDROID_
#define _ANDROID_
#endif

#ifdef __cplusplus
extern "C"
{
#endif
void GVideoPlayer_C_OnRender( unsigned char	* data, int size, int width, int height, int first );
#ifdef __cplusplus
}
#endif
#define LOST_HANDLE 0
GMH264Decoder::GMH264Decoder()
{
	USE_LOG;
    init();
}

GMH264Decoder::~GMH264Decoder()
{
	USE_LOG;
    destory();
}

void GMH264Decoder::init()
{
	av_register_all();
	m_pQueue						= new GMReceiveH264Queue();
	mDecodeFrameDataSize			= 0;
	mDecodeFrameDataBuffer			= (unsigned char *)malloc(MAX_FRAME_BUFFER_SIZE);


    mAVCodec                        = avcodec_find_decoder(AV_CODEC_ID_H264);
    mAVCodecContext                 = avcodec_alloc_context3(mAVCodec);
    mAVFrame						= av_frame_alloc();
    avcodec_open2(mAVCodecContext, mAVCodec, NULL);
	mAVCodecContext->flags 			|= CODEC_FLAG_EMU_EDGE | CODEC_FLAG_LOW_DELAY;
	mAVCodecContext->debug 			|= FF_DEBUG_MMCO;
	mAVCodecContext->pix_fmt		= PIX_FMT_YUV420P;
	m_nLastFrameNo					= 0;
	m_bStarted						= false;
    m_bStop							= false;
	m_bNewFrame						= false;
	m_pLock							= new ants360::CLock;
	m_width							= 0;
	m_height						= 0;
	start();
}

void GMH264Decoder::destory()
{
	stop();
	delete m_pLock;
    av_free(mAVFrame);

    avcodec_close(mAVCodecContext);
	av_free(mAVCodecContext);
    if ( mDecodeFrameDataBuffer != NULL )
	{
		free(mDecodeFrameDataBuffer);
		mDecodeFrameDataBuffer = NULL;
    }
	delete m_pQueue;
	m_pQueue = NULL;
}

void GMH264Decoder::start()
{
	LOGI("GMH264Decoder::start");
    int nErr;
    ants360::CAutoLock cAutoLock(m_pLock);
    if (m_bStarted)
    {
        return;
    }

    pthread_attr_t attr;
    nErr = pthread_attr_init(&attr);
    if (nErr)
    {
        LOGE("start failed");
        return;
    }

    nErr = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    if (nErr)
    {
        LOGE("start failed");
        return;
    }

    nErr = pthread_create(&m_thread, &attr, ThreadWrapper, this);
    if (nErr)
    {
        LOGE("start failed");
        return;
    }
    else
    {
        LOGI("created decoder thread");
    }

    pthread_attr_destroy(&attr);

    m_bStop = false;

}

void GMH264Decoder::stop()
{
	LOGI("GMH264Decoder::stop");
	do
	{
		m_pLock->lock();
	    if (m_bStarted && !m_bStop)
	    {
	        m_bStop = true;
	    }
		m_pLock->unlock();
	}while(0);
	m_pQueue->stop();
    int retv = 0;
    pthread_join(m_thread, (void**)&retv);
}


//static
void* GMH264Decoder::ThreadWrapper(void *pThis)
{
    prctl(PR_SET_NAME, (unsigned long)"GMH264Decoder",0,0,0);
    GMH264Decoder * p = (GMH264Decoder*) pThis;
    int nErr = p->ThreadEntry();
    return (void *) nErr;
}

int GMH264Decoder::ThreadEntry()
{
    int nErr = 0;


    m_pLock->lock();
    LOGI("GMH264Decoder is started");
    m_bStarted = true;
	while (!m_bStop)
    {
            m_pLock->unlock();
            decodeFrame();
            m_pLock->lock();
    }
    m_bStarted = false;
    LOGI("GMH264Decoder is stopped");
	m_pLock->unlock();

    return 0;
}

bool GMH264Decoder::addToQueue(GMBufferNode* &pframe)
{
	bool ret = false;
	do
	{
		if( pframe == NULL || pframe == NULL || pframe->size <= 0 )
		{
			LOGE("frame not invalid! ");
			break;
		}
		if(!m_pQueue->push(pframe))
		{
			LOGE("push failed! ");
			if(pframe->data != NULL)
			{
				free(pframe->data);
				pframe->data = NULL;
			}
			delete pframe;
			break;
		}
		ret = true;
	}while(0);
	return ret;
}

void GMH264Decoder::decodeFrame()
{
	//USE_LOG;
	do
	{
		GMBufferNode *pframe = NULL ;

		if(!m_pQueue->pop(&pframe))
		{
			LOGW("pop failed! ");
			break ;
		}
	    if (pframe == NULL || pframe->data == NULL || pframe->size <= 0 )
	    {
	    	LOGE("frame is invalid! ");
			break ;
	    }
	    do
	    {
	        AVPacket packet;
	        av_init_packet( &packet );
	        packet.data = pframe->data;
	        packet.size = pframe->size;

	        mDecodeResult = 0;

			int err = avcodec_decode_video2(mAVCodecContext, mAVFrame, &mDecodeResult, &packet);

	        if ( mDecodeResult == 1 )
	        {
	        	//LOGI("decode buffer frame secess, frame size is %d",pframe->size);
				int width = mAVCodecContext->width;
				int height = mAVCodecContext->height;
				LOGD("the frame is %dX%d, frame rate is %d/%d",width,height, mAVCodecContext->time_base.num,mAVCodecContext->time_base.den);
				mDecodeFrameDataSize = avpicture_get_size(mAVCodecContext->pix_fmt, width, height);
#if LOST_HANDLE
				if( (mAVFrame->coded_picture_number > (m_nLastFrameNo + 1)) && !mAVFrame->key_frame)
				{
					LOGI("lost some frame!");
				}
				else
				{
					if(mDecodeFrameDataSize <= MAX_FRAME_BUFFER_SIZE)
					{
						m_nLastFrameNo = mAVFrame->coded_picture_number;
						/*LOGI("frame information:key_frame@%d,pict_type@%d,coded_picture_number@%d,display_picture_number@%d,pts@%lld"
							,mAVFrame->key_frame,mAVFrame->pict_type,mAVFrame->coded_picture_number,mAVFrame->display_picture_number,mAVFrame->pts);*/
						avpicture_layout((AVPicture*)mAVFrame,mAVCodecContext->pix_fmt,width,height,mDecodeFrameDataBuffer,MAX_FRAME_BUFFER_SIZE);
						LOGI("this frame is the first frame: %s",(((m_width == width) && (m_height == height)) ? "false" : "true" ));
						GVideoPlayer_C_OnRender(mDecodeFrameDataBuffer, mDecodeFrameDataSize, width, height, (((m_width == width) && (m_height == height)) ? 0 : 1 ));
					    m_width = width;
					    m_height = height;
					}
					else
					{
						LOGE("Decode buffer is not enough!");
					}
				}
#else
				if(mDecodeFrameDataSize <= MAX_FRAME_BUFFER_SIZE)
				{
					m_nLastFrameNo = mAVFrame->coded_picture_number;
					/*LOGI("frame information:key_frame@%d,pict_type@%d,coded_picture_number@%d,display_picture_number@%d,pts@%lld"
						,mAVFrame->key_frame,mAVFrame->pict_type,mAVFrame->coded_picture_number,mAVFrame->display_picture_number,mAVFrame->pts);*/
					avpicture_layout((AVPicture*)mAVFrame,mAVCodecContext->pix_fmt,width,height,mDecodeFrameDataBuffer,MAX_FRAME_BUFFER_SIZE);
                    if((m_width != width) || (m_height != height))
                    {
					    LOGI("this frame is the first frame!");
                    }
					GVideoPlayer_C_OnRender(mDecodeFrameDataBuffer, mDecodeFrameDataSize, width, height, (((m_width == width) && (m_height == height)) ? 0 : 1 ));
					m_width = width;
					m_height = height;
				}
				else
				{
					LOGE("Decode buffer is not enough!");
				}
#endif
	        }
			else
			{
				//LOGE("decode buffer frame failed, frame size is %d, errno is %d",pframe->size,err);
			}
			av_free_packet(&packet);

	    }while (0);


		if(pframe->data != NULL)
		{
			free(pframe->data);
			pframe->data = NULL;
		}
		delete pframe;
	}while(0);
    return ;
}
