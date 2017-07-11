#ifndef GMH264Decoder_h
#define GMH264Decoder_h

extern "C"
{
#include "../../../ffmpeg/libavformat/avformat.h"
#include "../../../ffmpeg/libavcodec/avcodec.h"
#include "../../../ffmpeg/libswscale/swscale.h"
}
#include "pthread.h"
#include "../../../opengles/include/autolock.h"


typedef struct tagGMBufferNode
{

    unsigned char           *data;
    unsigned                size;
    tagGMBufferNode   *m_ptPre;
    tagGMBufferNode   *m_ptNext;
public:
	tagGMBufferNode(){memset(this , 0 ,sizeof( struct tagGMBufferNode) );	}
} GMBufferNode,*PGGMBufferNode;

class GMReceiveH264Queue
{
	public:
		GMReceiveH264Queue(): m_nSize(0),m_bStop(false)
		{
            m_pLock = new ants360::CLock;
            m_pTail = new GMBufferNode;
            m_pHead = new GMBufferNode;
            m_pHead->m_ptNext = m_pTail;
            m_pTail->m_ptPre = m_pHead;
		}

		~GMReceiveH264Queue()
		{

            clear();
            delete m_pHead;
            delete m_pTail;
			delete m_pLock;
		}

        bool push(GMBufferNode *node)
        {
            ants360::CAutoLock cAutoLock(m_pLock);
            bool ret = false;
            do
            {
                if(m_bStop || (node == NULL))
                {
                    break;
                }
                if((node->data == NULL) && (node->size == 0))
                {
                    break;
                }

                node->m_ptNext = m_pHead->m_ptNext;
                m_pHead->m_ptNext->m_ptPre = node;

                m_pHead->m_ptNext = node;
                node->m_ptPre = m_pHead;

                m_nSize++;
                ret = true;
                m_pLock->acttive();
            }while(0);
			return ret;
        }

        bool pop(GMBufferNode **node)
		{
		    ants360::CAutoLock cAutoLock(m_pLock);
		    bool ret = false;
            while(!m_bStop && m_nSize <= 0)
            {
                m_pLock->wait();
            }
            if((node != NULL) && (m_pTail->m_ptPre != NULL))
            {
                if((m_pTail->m_ptPre->data != NULL) && (m_pTail->m_ptPre->size != 0))
                {
                    *node = m_pTail->m_ptPre;
                    (*node)->m_ptPre->m_ptNext = m_pTail;
                    m_pTail->m_ptPre = (*node)->m_ptPre;

                    m_nSize--;
                    ret = true;
                }
            }
			return ret;
		}

		bool empty()
		{
		    ants360::CAutoLock cAutoLock(m_pLock);
		    bool ret = false;
            do
            {
    			if(m_nSize <= 0)
    			{
    				ret = true;
    			}
            }while(0);
			return ret;
		}

		int size()
		{
		    ants360::CAutoLock cAutoLock(m_pLock);
			return m_nSize;
		}

        void stop()
        {
            ants360::CAutoLock cAutoLock(m_pLock);
            m_bStop = true;
            m_pLock->acttive();
        }

    private:
        void clear()
        {
            ants360::CAutoLock cAutoLock(m_pLock);
            while(m_nSize > 0)
            {
                GMBufferNode* tmp = NULL;
                if(m_pTail->m_ptPre != NULL)
                {
                    if((m_pTail->m_ptPre->data != NULL) && (m_pTail->m_ptPre->size != 0))
                    {
                        tmp = m_pTail->m_ptPre;
                        tmp->m_ptPre->m_ptNext = m_pTail;
                        m_pTail->m_ptPre = tmp->m_ptPre;
                        if(tmp->data != NULL)
                        {
                            free(tmp->data);
                        }
                        delete tmp;
                        m_nSize--;
                    }
                }
            }
            return;
        }

	private:
		ants360::ILock     *m_pLock;
        GMBufferNode *m_pHead;
        GMBufferNode *m_pTail;
		int m_nSize;
        bool m_bStop;

};


class GMH264Decoder
{
public:
    GMH264Decoder();
    ~GMH264Decoder();
    bool addToQueue(GMBufferNode* &pframe);


private:
    void init();
    void destory();
    void start();
    void stop();
    void decodeFrame();

    static void *ThreadWrapper(void *pThis);
    int ThreadEntry();

private:
	struct AVCodec                 *mAVCodec;
	struct AVCodecContext          *mAVCodecContext;
	struct AVFrame                 *mAVFrame;
    int                             mDecodeResult;
    unsigned char                  *mDecodeFrameDataBuffer;
    int                             mDecodeFrameDataSize;

    ants360::ILock                 *m_pLock;
    pthread_t                       m_thread;
    bool                            m_bStarted;
    bool                            m_bStop;
    int                             m_nLastFrameNo;
    GMReceiveH264Queue             *m_pQueue;
    unsigned				        m_nFramesNumber;
    bool                            m_bNewFrame;
    int                             m_width;
    int                             m_height;
};

#endif
