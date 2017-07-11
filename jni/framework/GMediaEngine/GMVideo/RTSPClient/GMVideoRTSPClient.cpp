#include "GMVideoRTSPClient.h"
#include "../Controller/GMVideoController.h"
#include "../H264MediaSink/GMH264MediaSink.h"
#include "../../../../source/GVideoRecorder/GVideoRecorder.h"
#include "../../../../source/GFramework.h"

#define RECEIVE_SINK_BUFFER_SIZE 100000
#define SOCKET_INPUT_BUFFER_SIZE 100000
#define REQUEST_STREAMING_OVER_TCP False
#define DEFAULT_SESSION_TIMEOUT 60
#define PLAY_REAL_TIME 0

char g_exitCode = 0;
void shutdown()
{
	LOGI("shutdown the rtsp stream!");
	g_exitCode = 1;
}
void continueAfterDESCRIBE(RTSPClient* clientData, int resultCode, char* resultString)
{
	LOGI("continueAfterDESCRIBE resultCode@%d, resultString@%s",resultCode,"");
	GMVideoRTSPClient* rtsp = (GMVideoRTSPClient*)clientData;
	if( rtsp == NULL )
	{
		LOGE("GMVideoRTSPClient is NULL");
		return;
	}
	UsageEnvironment& env = rtsp->envir();
	if (resultCode != 0)
	{
		delete[] resultString;
		LOGE("DESCRIBE failed!");
		GMVideoController::sharedInstance()->setShutDownReason(REASON_LIVE555BYE);
		shutdown();
		return;
	}
	char* sdpDescription = resultString;

	LOGI("Create a media session object from this SDP description");
	rtsp->m_session = MediaSession::createNew(env, sdpDescription);
	delete[] sdpDescription;
	if (rtsp->m_session == NULL)
	{
		LOGE("create session failed!");
		GMVideoController::sharedInstance()->setShutDownReason(REASON_LIVE555BYE);
		shutdown();
		return;
	}
	else if (!rtsp->m_session->hasSubsessions())
	{
		LOGE("no subsession!");
		GMVideoController::sharedInstance()->setShutDownReason(REASON_LIVE555BYE);
		shutdown();
		return;
	}

	LOGI("Then, setup the \"RTPSource\"s for the session:");
	MediaSubsessionIterator iter(*(rtsp->m_session));
	MediaSubsession *subsession;
	Boolean madeProgress = false;
	while ((subsession = iter.next()) != NULL)
	{
		if (!subsession->initiate())
		{
			LOGE("Unable to create receiver for \"%s/%s\" subsession: %s",subsession->mediumName()
				,subsession->codecName()
				,env.getResultMsg());
		}
		else
		{
			LOGI("Created receiver for \"%s/%s\" subsession (",subsession->mediumName(),subsession->codecName());
			if (subsession->rtcpIsMuxed())
			{
				LOGI("client port %d", subsession->clientPortNum());
			}
			else
			{
				LOGI("client ports %d-%d)",subsession->clientPortNum(),subsession->clientPortNum()+1);
			}
			madeProgress = True;
#if PLAY_REAL_TIME
			if (subsession->rtpSource() != NULL)
			{
				// Because we're saving the incoming data, rather than playing
				// it in real time, allow an especially large time threshold
				// (1 second) for reordering misordered incoming packets:
				unsigned const thresh = 1000000; // 1 second
				subsession->rtpSource()->setPacketReorderingThresholdTime(thresh);
				// Set the RTP source's OS socket buffer size as appropriate - either if we were explicitly asked (using -B),
				// or if the desired FileSink buffer size happens to be larger than the current OS socket buffer size.
				// (The latter case is a heuristic, on the assumption that if the user asked for a large FileSink buffer size,
				// then the input data rate may be large enough to justify increasing the OS socket buffer size also.)
				int socketNum = subsession->rtpSource()->RTPgs()->socketNum();
				unsigned curBufferSize = getReceiveBufferSize(env, socketNum);
				if (SOCKET_INPUT_BUFFER_SIZE > 0 || RECEIVE_SINK_BUFFER_SIZE > curBufferSize)
				{
					unsigned newBufferSize = SOCKET_INPUT_BUFFER_SIZE > 0 ? SOCKET_INPUT_BUFFER_SIZE : RECEIVE_SINK_BUFFER_SIZE;
					newBufferSize = setReceiveBufferTo(env, socketNum, newBufferSize);
					if (SOCKET_INPUT_BUFFER_SIZE > 0)
					{
						// The user explicitly asked for the new socket buffer size; announce it:
						LOGI(<< "Changed socket receive buffer size for the \"%s/%s\" subsession from %d to %d bytes"
						,subsession->mediumName(),subsession->codecName(),curBufferSize,newBufferSize);
					}
				}
			}
#endif
	    }
	}

	if (!madeProgress)
	{
		LOGE("create receivers for session failed!");
		GMVideoController::sharedInstance()->setShutDownReason(REASON_LIVE555BYE);
		shutdown();
		return;
	}

	LOGI("Perform additional 'setup' on each subsession, before playing them:");
	rtsp->setupStreams();
}

void continueAfterOPTIONS(RTSPClient* clientData, int resultCode, char* resultString) {
	LOGI("continueAfterOPTIONS resultCode@%d, resultString@%s",resultCode,resultString);
	GMVideoRTSPClient* rtsp = (GMVideoRTSPClient*)clientData;
	delete[] resultString;
	if (resultCode != 0)
	{
		LOGE(" \"OPTIONS\" request failed!");
		GMVideoController::sharedInstance()->setShutDownReason(REASON_LIVE555BYE);
		shutdown();
		return;
	}
	else
	{
		LOGI(" \"OPTIONS\" request sucess!");
		if(rtsp->m_bHeartBeat)
		{
			return;
		}
		else
		{
			rtsp->m_bHeartBeat = true;
			rtsp->sendDescribeCommand(continueAfterDESCRIBE, rtsp->m_authenticator);
		}
	}
}
void continueAfterSETUP(RTSPClient* clientData, int resultCode, char* resultString)
{
	LOGI("continueAfterSETUP resultCode@%d, resultString@%s",resultCode,resultString);
	GMVideoRTSPClient* rtsp = (GMVideoRTSPClient*)clientData;
	if( rtsp == NULL )	{
		LOGE("GMVideoRTSPClient is NULL");
		return;
	}
	UsageEnvironment& env = rtsp->envir();
	if (resultCode == 0)
	{
		LOGI("Setup \"%s/%s\"subsession (",rtsp->m_subsession->mediumName(),rtsp->m_subsession->codecName());
		if (rtsp->m_subsession->rtcpIsMuxed())
		{
			LOGI("client port %d",rtsp->m_subsession->clientPortNum());
		}
		else
		{
			LOGI("client ports %d-%d",rtsp->m_subsession->clientPortNum(),rtsp->m_subsession->clientPortNum()+1);
		}
		rtsp->m_bMadeProgress = True;
	}
	else
	{
		LOGE("Failed to setup %s\"/%s\" subsession: %s",rtsp->m_subsession->mediumName(),rtsp->m_subsession->codecName(),resultString);
	}
	delete[] resultString;

	rtsp->m_sessionTimeoutParameter = rtsp->sessionTimeoutParameter();

	LOGI("Set up the next subsession, if any");
	rtsp->setupStreams();
}

void sessionTimerHandler(void* clientData) {
	GMVideoRTSPClient* rtsp = (GMVideoRTSPClient*)clientData;
	rtsp->m_sessionTimerTask = NULL;
	sessionAfterPlaying(rtsp);
}
void checkSessionTimeoutBrokenServer(void* clientData)
{
	GMVideoRTSPClient* rtsp = (GMVideoRTSPClient*)clientData;
	if( rtsp == NULL )
	{
		LOGE("GMVideoRTSPClient is NULL");
		return;
	}
	UsageEnvironment& env = rtsp->envir();
	if (rtsp->m_sessionTimeoutBrokenServerTask != NULL && rtsp->m_authenticator != NULL)
	{
		rtsp->sendOptionsCommand(continueAfterOPTIONS, rtsp->m_authenticator);
	}
	unsigned sessionTimeout = rtsp->m_sessionTimeoutParameter == 0 ? DEFAULT_SESSION_TIMEOUT : rtsp->m_sessionTimeoutParameter;
	unsigned secondsUntilNextKeepAlive = sessionTimeout <= 5 ? 1 : sessionTimeout - 5;
	LOGI("Send an \"OPTIONS\" request, starting with the second call, the interval is %d seconds!",secondsUntilNextKeepAlive);
	if(rtsp->m_sessionTimeoutBrokenServerTask == NULL)
	{
		rtsp->m_sessionTimeoutBrokenServerTask = env.taskScheduler().scheduleDelayedTask(secondsUntilNextKeepAlive*1000000, (TaskFunc*)checkSessionTimeoutBrokenServer, rtsp);
	}
	else
	{
		env.taskScheduler().rescheduleDelayedTask(rtsp->m_sessionTimeoutBrokenServerTask, secondsUntilNextKeepAlive*1000000, (TaskFunc*)checkSessionTimeoutBrokenServer, rtsp );
	}
}
void continueAfterPLAY(RTSPClient* clientData, int resultCode, char* resultString)
{
	LOGI("continueAfterPLAY resultCode@%d, resultString@%s",resultCode,resultString);
	GMVideoRTSPClient* rtsp = (GMVideoRTSPClient*)clientData;
	if( rtsp == NULL )
	{
		LOGE("GMVideoRTSPClient is NULL");
		return;
	}

	UsageEnvironment& env = rtsp->envir();
	Boolean success = False;

	do
	{
		if (resultCode != 0)
		{
			LOGE("Failed to start playing session: %s",resultString);
			break;
		}

		// Set a timer to be handled at the end of the stream's expected duration (if the stream does not already signal its end
		// using a RTCP "BYE").  This is optional.  If, instead, you want to keep the stream active - e.g., so you can later
		// 'seek' back within it and do another RTSP "PLAY" - then you can omit this code.
		// (Alternatively, if you don't want to receive the entire stream, you could set this timer for some shorter value.)
		if (rtsp->m_duration > 0)
		{
			unsigned const delaySlop = 2; // number of seconds extra to delay, after the stream's expected duration.  (This is optional.)
			rtsp->m_duration += delaySlop;
			unsigned uSecsToDelay = (unsigned)(rtsp->m_duration * 1000000);
			rtsp->m_sessionTimerTask = env.taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)sessionTimerHandler, rtsp);
		}

		LOGI("Started playing session");
		if (rtsp->m_duration > 0)
		{
			LOGI(" (for up to %f seconds)",rtsp->m_duration);
		}
		success = True;
	} while (0);
	delete[] resultString;

	if (!success)
	{
		LOGE(" An unrecoverable error occurred with this stream.");
		GMVideoController::sharedInstance()->setShutDownReason(REASON_LIVE555BYE);
		shutdown();
	}
	else
	{
		rtsp->m_sessionTimeoutBrokenServerTask = NULL;
		checkSessionTimeoutBrokenServer(rtsp);
	}
}

void subsessionAfterPlaying(void* clientData)
{
	LOGI("subsessionAfterPlaying medianame@%s",((MediaSubsession*)clientData)->mediumName());
	//LOGI("Begin by closing this media subsession's stream");
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	GMVideoRTSPClient* rtsp = (GMVideoRTSPClient*)subsession->miscPtr;
	if( rtsp == NULL )	{
		LOGE("GMVideoRTSPClient is NULL");
		return;
	}
	UsageEnvironment& env = rtsp->envir();

	Medium::close(subsession->sink);
	subsession->sink = NULL;

	LOGI("Next, check whether *all* subsessions' streams have now been closed");
	MediaSession& session = subsession->parentSession();
	MediaSubsessionIterator iter(session);
	while ((subsession = iter.next()) != NULL)
	{
		if (subsession->sink != NULL)
		{
			LOGI("this subsession is still active");
			return;
		}

	}

	LOGI("All subsessions' streams have now been closed");
	sessionAfterPlaying(rtsp);
}

void subsessionByeHandler(void* clientData)
{
	LOGI("subsessionByeHandler medianame@%s",((MediaSubsession*)clientData)->mediumName());
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	subsessionAfterPlaying(subsession);
}

void sessionAfterPlaying(void* clientData) {
	GMVideoRTSPClient* rtsp = (GMVideoRTSPClient*)clientData;
	if( rtsp == NULL )	{
		LOGE("GMVideoRTSPClient is NULL");
		return;
	}
	UsageEnvironment& env = rtsp->envir();

	if (!rtsp->m_bPlayContinuously)
	{
		LOGI("do not play again!");
		GMVideoController::sharedInstance()->setShutDownReason(REASON_LIVE555BYE);
		shutdown();
	}
	else
	{
		// We've been asked to play the stream(s) over again.
		// First, reset state from the current session:
		env.taskScheduler().unscheduleDelayedTask(rtsp->m_sessionTimerTask);
		env.taskScheduler().unscheduleDelayedTask(rtsp->m_sessionTimeoutBrokenServerTask);
		rtsp->sendDescribeCommand(continueAfterDESCRIBE, rtsp->m_authenticator);
		//rtsp->sendPlayCommand(*(rtsp->m_session), continueAfterPLAY, 0.0f, -1.0f, 1.0f, rtsp->m_authenticator);
	}
	/*
	if ( (rtsp->m_On555Callback != NULL) && (rtsp->m_OnDelegate != NULL))
	{
		const char* fError = "subsessionBye: medianame is video!";
		rtsp->m_On555Callback( rtsp->m_OnDelegate, 1000, fError );
	}*/
}

GMVideoRTSPClient* GMVideoRTSPClient::createNew(GMFUN_555_EVENT on555Event
		, void *delegate
		, UsageEnvironment& env
        , char const* rtspURL
        , const char *username
        , const char * password
        , int verbosityLevel
        , char const* applicationName
        , portNumBits tunnelOverHTTPPortNum
        , int socketNumToServer) {
  return new GMVideoRTSPClient(on555Event, delegate, env, rtspURL,username,password, verbosityLevel, applicationName, tunnelOverHTTPPortNum,socketNumToServer);
}

GMVideoRTSPClient::GMVideoRTSPClient(GMFUN_555_EVENT on555Event
		, void *delegate
		, UsageEnvironment& env
        , char const* rtspURL
        , const char *username
        , const char * password
        , int verbosityLevel
        , char const* applicationName
        , portNumBits tunnelOverHTTPPortNum
        , int socketNumToServer)
				  :	RTSPClient(env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, socketNumToServer),
    m_bPlayContinuously(false),
    m_sessionTimerTask(NULL),
    m_sessionTimeoutBrokenServerTask(NULL),
    m_duration(0.0),
    m_authenticator(NULL),
    m_session(NULL),
    m_subsession(NULL),
    m_subsessionIter(NULL),
    m_bMadeProgress(false),
    m_bKeepAlive(true),
    m_bAlreadyShuttingDown(false),
    m_bHeartBeat(false)
{
	USE_LOG;
	m_On555Callback = on555Event;
	m_OnDelegate = delegate;
	m_authenticator = new Authenticator(username,password);

	if(m_authenticator == NULL)
	{
		LOGE("create Authenticator failed");
		return;
	}
	init();
}
GMVideoRTSPClient::~GMVideoRTSPClient	()
{
	USE_LOG;
	if(m_authenticator != NULL)
		delete m_authenticator;
	m_authenticator = NULL;
	destroy();
}

void GMVideoRTSPClient::init()
{

}

void GMVideoRTSPClient::destroy()
{

}

void GMVideoRTSPClient::start()
{
	if( m_authenticator != NULL )
	{
		GMVideoRTSPClient::sendOptionsCommand(continueAfterOPTIONS, m_authenticator);
	}
}

void GMVideoRTSPClient::setupStreams() {
	USE_LOG;
	if (m_subsessionIter== NULL)
	{
		m_subsessionIter = new MediaSubsessionIterator(*m_session);
	}

	while ((m_subsession = m_subsessionIter->next()) != NULL)
	{
		LOGI(" We have another subsession left to set up:");
		if (m_subsession->clientPortNum() == 0)
		{
			LOGI(" port # was not set");
			continue;
		}

		sendSetupCommand(*m_subsession, continueAfterSETUP, False, REQUEST_STREAMING_OVER_TCP, False, m_authenticator);
		return;
	}
	LOGI(" We're done setting up subsessions.");
	if(m_subsessionIter != NULL)
	{
		delete m_subsessionIter;
	}
	if (!m_bMadeProgress)
	{
		LOGE("create sink failed!");
		GMVideoController::sharedInstance()->setShutDownReason(REASON_LIVE555BYE);
		shutdown();
		return;
	}
	createRec();

	LOGI(" We've finished setting up all of the subsessions.  Now, send a RTSP \"PLAY\" command to start the streaming:");

	sendPlayCommand(*m_session, continueAfterPLAY, 0.0f, -1.0f, 1.0f, m_authenticator);
}

void GMVideoRTSPClient::createRec()
{
	USE_LOG;
	LOGI("Create and start \"MediaSink\"s for each subsession:");
    m_bMadeProgress = False;
    MediaSubsessionIterator iter(*m_session);
    while ((m_subsession = iter.next()) != NULL)
	{
		if (m_subsession->readSource() == NULL)
		{
			continue; // was not initiated
		}
		MediaSink* sink = NULL;
		if (strcmp(m_subsession->mediumName(), "video") == 0)
		{
			// For H.264 video stream, we use a special sink that adds 'start codes',
			// and (at the start) the SPS and PPS NAL units:
			LOGI("create GMH264MediaSink: sps_pps is @%s", m_subsession->fmtp_spropparametersets());
			sink = GMH264MediaSink::createNew( envir()
				, m_subsession->fmtp_spropvps()
				, m_subsession->fmtp_spropsps()
				, m_subsession->fmtp_sproppps());
		}
		else if (strcmp(m_subsession->mediumName(), "audio") == 0)
		{
			sink = GMPCMMediaSink::createNew ( envir(), NULL, NULL );
		}
		m_subsession->sink = sink;

		if (m_subsession->sink == NULL)
		{
			LOGE( "Failed to create FileSink for \"%s\": %s", m_subsession->mediumName(), envir().getResultMsg());
		}
		else
		{
			LOGI("createRec: %s start palying", m_subsession->mediumName());
			m_subsession->miscPtr = this;
			m_subsession->sink->startPlaying(*(m_subsession->readSource()), subsessionAfterPlaying, m_subsession);
			LOGI("Also set a handler to be called if a RTCP \"BYE\" arrives for this subsession:");
			if (m_subsession->rtcpInstance() != NULL)
			{
				m_subsession->rtcpInstance()->setByeHandler(subsessionByeHandler, m_subsession);
			}
			m_bMadeProgress = True;
		}
	}
    if (!m_bMadeProgress)
	{
		LOGE("create sink failed!");
		GMVideoController::sharedInstance()->setShutDownReason(REASON_LIVE555BYE);
		shutdown();
	}
}

void GMVideoRTSPClient::stop()
{
	USE_LOG;
	if (m_bAlreadyShuttingDown)
	{
		LOGE("the client is already shutting down.");
		return; // in case we're called after receiving a RTCP "BYE" while in the middle of a "TEARDOWN".
	}

	m_bAlreadyShuttingDown = true;


	envir().taskScheduler().unscheduleDelayedTask(m_sessionTimerTask);
	envir().taskScheduler().unscheduleDelayedTask(m_sessionTimeoutBrokenServerTask);


	LOGI("Teardown, then shutdown, any outstanding RTP/RTCP subsessions");
	if (m_session != NULL)
	{
	    Boolean someSubsessionsWereActive = False;
	    MediaSubsessionIterator iter(*m_session);
	    MediaSubsession* subsession;

	    while ((subsession = iter.next()) != NULL)
		{
			if (subsession->sink != NULL)
			{
				Medium::close(subsession->sink);
				subsession->sink = NULL;

				if (subsession->rtcpInstance() != NULL)
				{
					subsession->rtcpInstance()->setByeHandler(NULL, NULL);
					// in case the server sends a RTCP "BYE" while handling "TEARDOWN"
				}
				someSubsessionsWereActive = True;
			}
	    }

	    if (someSubsessionsWereActive)
		{
			// Send a RTSP "TEARDOWN" command, to tell the server to shutdown the stream.
			// Don't bother handling the response to the "TEARDOWN".
			sendTeardownCommand( *m_session, NULL, m_authenticator );
		}
	}
}
