#ifndef GEngineEventDefine_h
#define GEngineEventDefine_h

typedef enum
{
	EET_DDET_ERR 					= 100000,
	EET_VIDEO_PLY 					= 100001,
	EET_AUDIO_FILE_PLAYER_START		= 100002,
	EET_AUDIO_FILE_PLAYER_STOP		= 100003,
	EET_AUDIO_FILE_RECORD_START		= 100004,
	EET_AUDIO_FILE_RECORD_STOP		= 100005,
	EET_AUDIO_FILE_SAVE_FINISH		= 100006,
	EET_VIDEO_RECORD_RESPONSE_START	= 100007,
	EET_VIDEO_RECORD_RESPONSE_STOP	= 100008,
	EET_TASK_KILLER_CMD_KILL		= 100009,
	EET_TASK_KILLER_MSG_KILLED		= 100010,
	EET_TASK_KILLER_MSG_ALL_KILLED	= 100011,
	EET_VIDEO_CHANGEMODE_FINISH		= 100012,
	EET_VIDEO_CHANGEMODE_STOPPED	= 1000000,

} GEngineEventType;

typedef struct
{
	GEngineEventType 	type;
	int					audioFilePlayerStartAction;
	int					audioFilePlayerStopAction;
	int					audioFileRecorderStartAction;
	int					audioFileRecorderStopAction;
	int					audioFileSaveAction;
	int					audioFileSaveResult;
	int					videoRecordStartResponse;
	int					videoRecordStopResponse;
	int					tasksKillerWillKillID;
	int					tasksKillerKilledID;
} GEngineEventData;

#endif
