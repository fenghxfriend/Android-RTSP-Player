#include "GEngineEvent.h"
#include "../GEngine/GEngine.h"

GEngineEvent &GEngineEvent::sharedInstance()
{
	static GEngineEvent instance;
	return instance;
}

void GEngineEvent::swallowEvent(GEngineEventType type, void *data, int size)
{
	GEngineEventData eventData;
	eventData.type = type;

	if ( EET_DDET_ERR == type )
	{
		//...
	}
	else if ( EET_VIDEO_PLY == type )
	{
		//...
	}
	else if ( EET_AUDIO_FILE_PLAYER_START == type )
	{
		eventData.audioFilePlayerStartAction = size;
	}
	else if ( EET_AUDIO_FILE_PLAYER_STOP == type )
	{
		eventData.audioFilePlayerStopAction = size;
	}
	else if ( EET_AUDIO_FILE_RECORD_START == type )
	{
		eventData.audioFileRecorderStartAction = size;
	}
	else if ( EET_AUDIO_FILE_RECORD_STOP == type )
	{
		eventData.audioFileRecorderStopAction = size;
	}
	else if ( EET_AUDIO_FILE_SAVE_FINISH == type )
	{
		eventData.audioFileSaveAction = ( *( ( int * )data ) );
		eventData.audioFileSaveResult = size;
	}
	else if ( EET_VIDEO_RECORD_RESPONSE_START == type )
	{
		eventData.videoRecordStartResponse = size;
	}
	else if ( EET_VIDEO_RECORD_RESPONSE_STOP == type )
	{
		eventData.videoRecordStopResponse = size;
	}
	else if ( EET_TASK_KILLER_CMD_KILL == type )
	{
		eventData.tasksKillerWillKillID = size;
	}
	else if ( EET_TASK_KILLER_MSG_KILLED == type )
	{
		eventData.tasksKillerKilledID = size;
	}
	else if ( EET_VIDEO_CHANGEMODE_FINISH == type )
	{
		//...
	}
	else if ( EET_VIDEO_CHANGEMODE_STOPPED == type )
	{
		//...
	}

	GEngine::sharedInstance().swallowEngineEvent( eventData );
}
