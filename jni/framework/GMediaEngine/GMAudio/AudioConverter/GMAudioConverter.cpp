#include <pthread.h>
#include <fcntl.h>
#include "GMAudioConverter.h"

void* GMAudioConverter_BackgroundWorkerTask(void *data);

GMAudioConverter::GMAudioConverter() { init(true); }
GMAudioConverter::~GMAudioConverter() { destory(); }

bool GMAudioConverter::loadFile(const char *path)
{
    bool result = false;

    do
    {
        if ( path == NULL ) break;
        if ( access(path, 0) != 0 ) break;
        if ( isWorking() ) break;

        init();

        if ( avformat_open_input(&mFormatContext, path, NULL, NULL) != 0 ) break;
        if ( avformat_find_stream_info(mFormatContext, NULL) < 0 ) break;
        for (unsigned int i = 0; i < mFormatContext->nb_streams; i++)
        {
            if (mFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
            {
                mAudioStream        = mFormatContext->streams[i];
                mAudioStreamIndex   = i;
                break;
            }
        }
        if ( mAudioStream == NULL ) break;

        mCodecContext = mAudioStream->codec;
        mCodecContext->codec = avcodec_find_decoder(mCodecContext->codec_id);
        if ( mCodecContext->codec == NULL ) { mCodecContext = NULL; break; }
        if ( avcodec_open2( mCodecContext, mCodecContext->codec, NULL ) != 0 ) { mCodecContext = NULL; break; }

        mSwrContext = swr_alloc_set_opts(mSwrContext,
                                         av_get_default_channel_layout(GMAudioResampleChannels), GMAudioResampleFormat, GMAudioResampleSampleRate,
                                         av_get_default_channel_layout(mCodecContext->channels), mCodecContext->sample_fmt, mCodecContext->sample_rate,
                                         0, NULL);
        if ( mSwrContext == NULL ) break;
        if ( swr_init( mSwrContext ) < 0 ) break;

        startWork();

        mLoadedFile = true;
        mEOF        = false;
        result      = true;

    } while (0);

    if ( !result ) init();

    return result;
}

void GMAudioConverter::readData(void *buffer, unsigned &actualframes, unsigned maxframes)
{
    if ( !mLoadedFile || !buffer ) { actualframes = 0; return; }

    mUtility.lock( mSourceLock );

    if ( !mAudioFrames[mWillReadBufferIndex].idle )
    {
        int  _toIdx      = mWillReadBufferIndex;
        int  _framecount = 0;
        int  _lastframes = 0;
        int  _buffframes = 0;
        int  _readBytes  = 0;
        bool _lastIdle   = false;
        while ( true )
        {
            if ( mAudioFrames[_toIdx].idle ) break;

            _buffframes = mAudioFrames[_toIdx].frames - mAudioFrames[_toIdx].usedFrames;

            if ( ( _framecount + _buffframes ) < maxframes )
            {
                _framecount += _buffframes;
            }
            else
            {
                if ( ( _framecount + _buffframes ) == maxframes ) _lastIdle = true;
                _lastframes = maxframes - _framecount;
                _framecount += _lastframes;
                break;
            }
            _toIdx++;
            if ( _toIdx > GMAudioFrameBufferMax ) _toIdx = 1;
        }

        if ( _toIdx >= mWillReadBufferIndex )
        {
            readBufferData(( char * )buffer, mWillReadBufferIndex, _toIdx, _lastframes, _lastIdle);
        }
        else
        {
            _readBytes = readBufferData(( char * )buffer, mWillReadBufferIndex, GMAudioFrameBufferMax, 0, false, false);
            readBufferData((char *)(&(((char *)buffer)[_readBytes])), 1, _toIdx, _lastframes, _lastIdle);
        }

        actualframes = _framecount;

        if ( _lastIdle )
        {
            mWillReadBufferIndex = _toIdx + 1;
            if ( mWillReadBufferIndex > GMAudioFrameBufferMax ) mWillReadBufferIndex = 1;
        }
        else
        {
            mWillReadBufferIndex = _toIdx;
        }
    }
    else
    {
        actualframes = 0;

        if ( isEndOfFile(false) )
        {
            mEOF = true;
        }
    }

    mUtility.unlock( mSourceLock );
}

int GMAudioConverter::readBufferData(char *buffer, int from, int to, int lastFrames, bool lastIdle, bool carelast)
{
    int readTotalBytes  = 0;
    int usedBytes       = 0;
    int validBytes      = 0;
    int bufferIdx       = 0;
    int lastBytes       = lastFrames * GMAudioFrameBytes;

    do
    {
        if ( from == to )
        {
            usedBytes  = mAudioFrames[to].usedFrames * GMAudioFrameBytes;
            validBytes = ( mAudioFrames[to].frames - mAudioFrames[to].usedFrames ) * GMAudioFrameBytes;

            if ( carelast )
            {
                memcpy( buffer, &( (mAudioFrames[to].data)[usedBytes] ), lastBytes );

                readTotalBytes = lastBytes;

                if ( lastIdle ) mAudioFrames[to].init();
                else mAudioFrames[to].usedFrames += lastFrames;
            }
            else
            {
                memcpy( buffer, &( (mAudioFrames[to].data)[usedBytes] ), validBytes );

                readTotalBytes = validBytes;

                mAudioFrames[to].init();
            }

            break;
        }

        to = carelast ? to : ( to + 1 );
        for (int i = from; i < to; i++)
        {
            usedBytes  = mAudioFrames[i].usedFrames * GMAudioFrameBytes;
            validBytes = ( mAudioFrames[i].frames - mAudioFrames[i].usedFrames ) * GMAudioFrameBytes;

            memcpy( &( buffer[bufferIdx] ), &( (mAudioFrames[i].data)[usedBytes] ), validBytes );

            readTotalBytes += validBytes;

            bufferIdx += validBytes;

            mAudioFrames[i].init();
        }

        if ( !carelast ) break;

        usedBytes  = mAudioFrames[to].usedFrames * GMAudioFrameBytes;
        validBytes = ( mAudioFrames[to].frames - mAudioFrames[to].usedFrames ) * GMAudioFrameBytes;
        memcpy( &( buffer[bufferIdx] ), &( (mAudioFrames[to].data)[usedBytes] ), lastBytes );

        readTotalBytes += lastBytes;

        if ( lastIdle ) mAudioFrames[to].init();
        else mAudioFrames[to].usedFrames += lastFrames;

    } while (0);

    return readTotalBytes;
}

void GMAudioConverter::seek(double time)
{
    if ( !mLoadedFile ) return;

    long long _seekTime = time;

    mUtility.lock  ( mSourceLock );

    _seekTime = av_rescale_q(_seekTime, AV_TIME_BASE_Q, mFormatContext->streams[mAudioStreamIndex]->time_base) * AV_TIME_BASE;
    av_seek_frame(mFormatContext, mAudioStreamIndex, _seekTime, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_ANY);

    mUtility.unlock( mSourceLock );
}

double GMAudioConverter::duration()
{
    if ( !mLoadedFile ) return 0.0;

    double _duration = 0.0;
    mUtility.lock  ( mSourceLock );
    _duration = ( mFormatContext->duration / AV_TIME_BASE );
    mUtility.unlock( mSourceLock );

    return _duration;
}

bool GMAudioConverter::isEOF()
{
    return mEOF;
}

float GMAudioConverter::mediaDuration(char const* fileName)
{
    float result = 0.0;

    AVFormatContext *_formatContext = NULL;

    do
    {
        if ( fileName == NULL ) break;
        if ( access(fileName, 0) != 0 ) break;

        av_register_all();

        if ( avformat_open_input(&_formatContext, fileName, NULL, NULL) != 0 ) break;
        if ( avformat_find_stream_info(_formatContext, NULL) < 0 ) break;

        result = _formatContext->duration / AV_TIME_BASE;

    } while (0);

    if ( _formatContext != NULL ) avformat_close_input(&_formatContext);

    return result;
}

void GMAudioConverter::unloadFile()
{
    stopWork();
    init();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//private functions
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GMAudioConverter::init(bool init)
{
    for (int i = 0; i <= GMAudioFrameBufferMax; i++)
    {
        mAudioFrames[i].init(init);
    }

    if ( !init )
    {
        if ( mSwrContext    != NULL ) swr_free            ( &mSwrContext    );
        if ( mCodecContext  != NULL ) avcodec_close       ( mCodecContext   );
        if ( mFormatContext != NULL ) avformat_close_input( &mFormatContext );

        mFormatContext          = NULL;
        mAudioStream            = NULL;
        mCodecContext           = NULL;
        mSwrContext             = NULL;
        mLoadedFile             = false;
        mWillReadBufferIndex    = 1;
        mEOF                    = true;
    }
    else
    {
        mFormatContext          = NULL;
        mAudioStream            = NULL;
        mCodecContext           = NULL;
        mSwrContext             = NULL;
        mLoadedFile             = false;
        mWillReadBufferIndex    = 1;
        mEOF                    = true;

        av_register_all();

        createStateMachine();

        mUtility.createThread( &mBackgroundWorker, GMAudioConverter_BackgroundWorkerTask, this );
    }
}

void GMAudioConverter::destory()
{
    kill();
    startWork();
    mUtility.waitingThread( mBackgroundWorker );

    init();

    destoryStateMachine();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//background worker
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GMAudioConverter::backgroundWorkerTask()
{
    int           _activeBufferIndex                  = 1;
    bool          _activeBufferIdle                   = false;
    int           _decodeFrameSuccess                 = 0;
    int           _outputSamplesNumber                = 0;
    bool          _readFrameSuccess                   = false;
    AVFrame       *_audioFrame                        = avcodec_alloc_frame();
    unsigned char *_audioOutBufferArray[SWR_CH_MAX]   = { ( unsigned char * )( mAudioFrames[0].data ), NULL };
    AVPacket      _audioPacket;

    while (true)
    {
        waitingForWork();
        setIsWorking( true );

        if ( !isAlive()   ) break;
        if ( isStopWork() ) { setIsWorking( false ); continue; }

        _activeBufferIndex      = 1;
        _activeBufferIdle       = false;
        _decodeFrameSuccess     = 0;
        _outputSamplesNumber    = 0;
        av_init_packet( &_audioPacket );
        while (true)
        {
            mUtility.lock( mSourceLock );
            _activeBufferIdle = mAudioFrames[_activeBufferIndex].idle;
            mUtility.unlock( mSourceLock );
            if ( !_activeBufferIdle )
            {
                if ( !isAlive() || isStopWork()  ) break;
                continue;
            }

            _readFrameSuccess = false;
            mUtility.lock  ( mSourceLock );
            if ( av_read_frame(mFormatContext, &_audioPacket) == 0 ) _readFrameSuccess = true;
            mUtility.unlock( mSourceLock );
            if ( !_readFrameSuccess ) break;
            if ( _audioPacket.stream_index != mAudioStream->index  ) continue;

            _decodeFrameSuccess = 0;
            avcodec_decode_audio4( mCodecContext, _audioFrame, &_decodeFrameSuccess, &_audioPacket );
            if ( !_decodeFrameSuccess ) continue;

            _outputSamplesNumber = swr_convert(mSwrContext, _audioOutBufferArray, GMAudioResampleSampleRate, (const uint8_t**)_audioFrame->extended_data, _audioFrame->nb_samples);
            if ( _outputSamplesNumber < 0 ) continue;

            mUtility.lock( mSourceLock );
            mAudioFrames[_activeBufferIndex].init();
            mAudioFrames[_activeBufferIndex].frames = _outputSamplesNumber;
            mAudioFrames[_activeBufferIndex].idle   = false;
            memcpy( mAudioFrames[_activeBufferIndex].data, mAudioFrames[0].data, _outputSamplesNumber * GMAudioFrameBytes );
            mUtility.unlock( mSourceLock );

            _activeBufferIndex++;
            if ( _activeBufferIndex > GMAudioFrameBufferMax ) _activeBufferIndex = 1;

            av_free_packet( &_audioPacket );
        }

        setEndOfFile(true);

        setIsWorking( false );

        if ( !isAlive() ) break;
    }

    setIsWorking( false );

    avcodec_free_frame( &_audioFrame );
}

void* GMAudioConverter_BackgroundWorkerTask(void *data)
{
    ( ( GMAudioConverter * )data )->backgroundWorkerTask();
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//state machine
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GMAudioConverter::createStateMachine()
{
    mLiveLock               = mUtility.createLock();
    mTaskLock               = mUtility.createLock();
    mSourceLock             = mUtility.createLock();
    mStopWorkWhileWorking   = false;
    mIsWorking              = false;
    mIsEndOfFile            = true;
    mNewTaskComming			= false;

    keepAlive();
}

void GMAudioConverter::keepAlive()
{
    mUtility.lock  ( mLiveLock );
}

void GMAudioConverter::kill()
{
    mUtility.unlock( mLiveLock );
}

void GMAudioConverter::startWork()
{
    if ( isWorking() ) return;

    mUtility.lock  ( mSourceLock );
    mStopWorkWhileWorking   = false;
    mIsEndOfFile            = false;
    mUtility.unlock( mSourceLock );

    //设定任务flag
    mUtility.lock  ( mTaskLock );
    mNewTaskComming = true;
    mUtility.unlock( mTaskLock );
}

void GMAudioConverter::stopWork()
{
    if ( !isWorking() ) return;

    mUtility.lock  ( mSourceLock );
    mStopWorkWhileWorking = true;
    mUtility.unlock( mSourceLock );
}

bool GMAudioConverter::isAlive()
{
    if ( mUtility.trylock( mLiveLock ) )
    {
        mUtility.unlock( mLiveLock );
        return false;
    }

    return true;
}

bool GMAudioConverter::isWorking()
{
    bool result = false;

    mUtility.lock  ( mSourceLock );
    result = mIsWorking;
    mUtility.unlock( mSourceLock );

    return result;
}

void GMAudioConverter::setIsWorking(bool value)
{
    mUtility.lock  ( mSourceLock );
    mIsWorking = value;
    mUtility.unlock( mSourceLock );
}

bool GMAudioConverter::isStopWork()
{
    bool result = false;

    mUtility.lock  ( mSourceLock );
    result = mStopWorkWhileWorking;
    mUtility.unlock( mSourceLock );

    return result;
}

void GMAudioConverter::waitingForWork()
{
	bool haveTask = false;

	while( true )
	{
		mUtility.lock  ( mTaskLock );
		haveTask = mNewTaskComming;
		mNewTaskComming = false;
		mUtility.unlock( mTaskLock );
		if ( haveTask ) break;
		usleep( 100 );
	}
}

void GMAudioConverter::setEndOfFile(bool value, bool lock)
{
    if ( lock ) mUtility.lock  ( mSourceLock );
    mIsEndOfFile = value;
    if ( lock ) mUtility.unlock( mSourceLock );
}

bool GMAudioConverter::isEndOfFile(bool lock)
{
    bool result = true;

    if ( lock ) mUtility.lock  ( mSourceLock );
    result = mIsEndOfFile;
    if ( lock ) mUtility.unlock( mSourceLock );

    return result;
}

void GMAudioConverter::destoryStateMachine()
{
    kill();
    mStopWorkWhileWorking   = false;
    mIsWorking              = false;
    mUtility.destorylock( &mSourceLock );
    mUtility.destorylock( &mTaskLock   );
    mUtility.destorylock( &mLiveLock   );
}

