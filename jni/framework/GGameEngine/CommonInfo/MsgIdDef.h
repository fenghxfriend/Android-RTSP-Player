#ifndef __MSG_ID_DEF__
#define __MSG_ID_DEF__

enum _enum_MSG_ID_DEF
{
    MsgID_SM_CONNECT_CONNECTED,

	//system msg
	MsgID_SM_NEW_CLIENT_CONNECT,
	MsgID_SM_DEL_CLIENT_CONNECT,

	//client to tank msg
	MsgID_CT_SEARCH_SERVER,
    MsgID_CT_REQUEST_CONTROL_TANK,
    MsgID_CT_ATTACK_MSG,
    MsgID_CT_MOVE_MSG,
    MsgID_CT_PUSH_PULL_MSG,
    MsgID_CT_RATATION_MSG,
    MsgID_CT_NOTIFACTION_GAME_START,
    MsgID_CT_NOTIFACTION_GAME_END,
    MsgID_CT_AIM,
    MsgID_CT_CHANGE_PASSWORD,
    MsgID_CT_CHANGE_TANK_NAME,
        
	//tank to client msg
    MsgID_TC_FREE_TANK_INFO,
	MsgID_TC_CAN_BE_CONNECT,
	MsgID_TC_TANK_BASE_INFO,
    MsgID_TC_BE_ATTACKED,
    MsgID_TC_BE_AIM,
    MsgID_TC_REPLY_SET,

	//client to game msg
	MsgID_CG_BROWSE_SERVER_INFO = 100,
	MsgID_CG_CREATE_ROOM,
    MsgID_CG_DISBAND_ROOM,
    MsgID_CG_JOIN_ROOM,
    MsgID_CG_BEGIN_GAME,
    MsgID_CG_DELETE_PLAYER,
    MsgID_CG_EXIT_GAME,
    MsgID_CG_BACK_WAIT,
    MsgID_CG_USE_ITEM,
    MsgID_CG_REQUEST_LEAVE_GAME,

	//server to client msg
	MsgID_SC_SERVER_INFO = 200,
	MsgID_SC_CONNECT_SUCCESS,
    MsgID_SC_CREATEROOM_SUCCESS,
    MsgID_SC_ROOM_DISBAND,
    MsgID_SC_JOIN_ROOM_ANSWER,
    MsgID_SC_PLAYER_JOIN_ROOM,
    MsgID_SC_GAME_START,
    MsgID_SC_BE_HITTED,
    MsgID_SC_HIT,
    MsgID_SC_PLAYER_LEAVE_GAME,
    MsgID_SC_PLAYER_DEAD,
    MsgID_SC_TEAM_FAIL,
    MsgID_SC_NORMAL_END,
    MsgID_SC_UPDATE_ROOM_INFO,
    MsgID_SC_PLAYER_USE_ITEM,
    MsgID_SC_PLAYER_UPDATE_HP,
    MsgID_SC_PLAYER_STOP_ITEM,
    
    //----------------------------------------------------------------
    //
    //add by jinli 2012-03-01
    //
    MM_C2M_UpdateModelConfiger              = 10000,
    MM_C2M_ModelReciveUpdateConfigerData,
    MM_C2M_ExecUpdateModelConfiger,
    MM_C2M_ModelControl_Forword,
    MM_C2M_ModelControl_Back,
    MM_C2M_ModelControl_Stop,
    MM_C2M_ModelControl_StartTurnLeft,
    MM_C2M_ModelControl_StopTurnLeft,
    MM_C2M_ModelControl_StartTurnRight,
    MM_C2M_ModelControl_StopTurnRight,
    MM_C2M_ModelControl_CameraMoveUp,
    MM_C2M_ModelControl_CameraMoveDown,
    MM_C2M_ModelControl_CameraStop,
    MM_C2M_ModelControl_LeftLEDStart,
    MM_C2M_ModelControl_LeftLEDStop,
    MM_C2M_ModelControl_RightLEDStart,
    MM_C2M_ModelControl_RightLEDStop,
    MM_C2M_ModelControl_BehindLEDStart,
    MM_C2M_ModelControl_BehindLEDStop,
    MM_C2M_ModelControl_FrontLightOpen,
    MM_C2M_ModelControl_FrontLightClose,
    MM_C2M_ModelControl_SpeakerOpen,
    MM_C2M_ModelControl_SpeakerClose,
    MM_C2M_ModelInformation_BatteryPower,
    
    //
    //add by jinli 2012-03-26
    //
    MM_C2M_OpenPDASoundToModel,
    MM_M2C_OpenPDASoundToModelResponse,
    MM_C2M_ClosePDASoundToModel,
    MM_C2M_ClosePDASoundToModelResponse,
    MM_C2M_OpenModelSoundToPDA,
    MM_C2M_OpenModelSoundToPDAResponse,
    MM_C2M_CloseModelSoundToPDA,
    MM_C2M_CloseModelSoundToPDAResponse,
    MM_C2M_OpenStreamMusic,
    MM_C2M_OpenStreamMusicResponse,
    MM_C2M_CloseStreamMusic,
    MM_C2M_CloseStreamMusicResponse,
    
    //
    //add by jinli 2012-03-29
    //
    MM_C2M_SpeakerMusicPCMData,
    MM_C2M_SaveSpeakerMusicPCMDataResponse,
    
    //
    //add by jinli 2012-04-14
    //
    MM_C2M_OpenSpeakerResponse,
    MM_C2M_CloseSpeakerResponse,
    
    //
    //add by jinli 2012-04-18
    //
    MM_C2M_CustomDisconnect,
    MM_C2M_PingPong,
    
    //
    //add by jinli 2012-04-26
    //
    MM_C2M_PauseStreamMusic,
    MM_C2M_PauseStreamMusicResponse,
    
    //
    //add by jinli 2012-06-06
    //
    MM_C2M_ClearProductTester,
    MM_C2M_ClearProductTesterResponse,
    MM_C2M_ClearProductTesterRestart,
    
    //
    //add by jinli 2012-11-05
    //
    MM_C2M_NeedModelAdvanceInfo,
    MM_M2C_ModelAdvanceInfo,
    MM_C2M_5SM3n_Forward,
    MM_C2M_5SM3n_Backward,
    MM_C2M_5SM3n_LeftTurn,
    MM_C2M_5SM3n_RightTurn,
    MM_C2M_5SM3n_StopMove,
    MM_C2M_5SM3n_StopTurn,
    
    //
    //add by jinli 2012-11-29
    //
    MM_C2M_5HT6_Forward,
    MM_C2M_5HT6_Backward,
    MM_C2M_5HT6_TurnLeft,
    MM_C2M_5HT6_TurnRight,
    MM_C2M_5HT6_FlyLeft,
    MM_C2M_5HT6_FlyRight,
    MM_C2M_5HT6_GoUp,
    MM_C2M_5HT6_GoDown,
    MM_M2C_5HT6_GroundDistance,
    MM_M2C_5HT6_CommonUse,
    MM_C2M_5HT6_CommonUse,
    MM_C2M_5HT6_IPAddress,
    MM_C2M_5HT6_StopMove,
	MM_C2M_5HT6_StopSelfTurn,
	MM_C2M_5HT6_StopFlyTurn,
	MM_C2M_5HT6_StopUpDown,
    
    //
	//add by jinli 2012-12-07
	//
	MM_C2M_5RM6n_Forward,
	MM_C2M_5RM6n_Backward,
	MM_C2M_5RM6n_StopMove,
	MM_C2M_5RM6n_TurnLeft,
	MM_C2M_5RM6n_TurnRight,
	MM_C2M_5RM6n_StopTurn,
    MM_C2M_5RM6n_WaistTurnLeft,
    MM_C2M_5RM6n_WaistTurnRight,
    MM_C2M_5RM6n_WaistStopTurn,
    MM_C2M_5RM6n_LeftHandAttack,
    MM_C2M_5RM6n_RightHandAttack,
    MM_C2M_5RM6n_LeftGunFire,
    MM_C2M_5RM6n_RightGunFire,
    MM_C2M_5RM6n_BeAttackedByGun,
    MM_C2M_5RM6n_HeadBeAttacked,
    MM_C2M_5RM6n_WaistAngle,
    MM_C2M_5RM6n_StartBattle,
    MM_C2M_5RM6n_FinishedBattle,
    MM_C2M_5RM6n_ReportGunStatus,
    
    //
	//add by jinli 2012-12-07
	//
	MM_C2M_5SM3nS_BeginBattle,
	MM_C2M_5SM3nS_FinishBattle,
	MM_C2M_5SM3nS_Fire,
	MM_C2M_5SM3nS_EnemyAttack,
    
    ///////////////////////////////////////// GWG /////////////////////////////////////////
    MM_C2M_5SM3nS_GWG_KickPlayer,
    MM_M2C_5SM3nS_GWG_KickPlayerResponse,
    MM_C2M_5SM3nS_GWG_DisbandGame,
    MM_M2C_5SM3nS_GWG_DisbandGameResponse,
    MM_C2M_5SM3nS_GWG_LeaveGame,
    MM_M2C_5SM3nS_GWG_LeaveGameResponse,
    MM_C2M_5SM3nS_GWG_StartGame,
    MM_M2C_5SM3nS_GWG_StartGameResponse,
    MM_M2C_5SM3nS_GWG_SomeBodyJoinGameWhileWaiting,
    MM_M2C_5SM3nS_GWG_SomeBodyLeaveGameWhileWaiting,
    MM_M2C_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling,
    
    MM_M2C_5SM3nS_GWG_SearchGameServer,
    MM_M2C_5SM3nS_GWG_SearchGameServerResponse,
    MM_M2C_5SM3nS_GWG_StopSearchGameServer,
    MM_M2C_5SM3nS_GWG_StopSearchGameServerResponse,
    MM_M2C_5SM3nS_GWG_SearchedGameServerInfo,
    MM_M2C_5SM3nS_GWG_FinishedSearchGameServer,
    MM_M2C_5SM3nS_GWG_JoinSearchedGameServer,
    MM_M2C_5SM3nS_GWG_JoinSearchedGameServerResponse,
    MM_M2C_5SM3nS_GWG_RequestGameServerTeamInfo,
    MM_M2C_5SM3nS_GWG_GameServerTeamInfo,
    
    MM_C2M_5SM3nS_GWG_CreateGame,
    MM_M2C_5SM3nS_GWG_CreateGameResponse,
    
    MM_M2C_5SM3nS_GWG_HostStartGame,
    MM_M2C_5SM3nS_GWG_HostDisbandGame,
    
    MM_C2M_5SM3nS_GWG_LeaveGameWhileBattling,
    MM_M2C_5SM3nS_GWG_LeaveGameWhileBattlingResponse,
    MM_M2C_5SM3nS_GWG_GameServerMessage,
    MM_M2C_5SM3nS_GWG_GameOver,
    MM_M2C_5SM3nS_GWG_BeKickedoutByHost,
    MM_C2M_5SM3nS_GWG_RequestTeamMemberIDList,
    MM_M2C_5SM3nS_GWG_TeamMemberIDList,
    
    //**************************************************************************
    //ARM-S2C  &  ARM-C2S
    //**************************************************************************
    MM_5SM3nS_GWG_ARM_SC_GameOver,
    MM_5SM3nS_GWG_ARM_SC_HostStartGame,
    MM_5SM3nS_GWG_ARM_SC_HostDisbandGame,
    MM_5SM3nS_GWG_ARM_SC_BeKicked,
    MM_5SM3nS_GWG_ARM_SC_SomeOneLeaveGameWhileWaiting,
    MM_5SM3nS_GWG_ARM_SC_ServerMessage,
    MM_5SM3nS_GWG_ARM_SC_ServerInfo,
    MM_5SM3nS_GWG_ARM_SC_JoinGameResponse,
    MM_5SM3nS_GWG_ARM_SC_SomeOneJoinGame,
    MM_5SM3nS_GWG_ARM_SC_TeamInfo,
    MM_5SM3nS_GWG_ARM_SC_AllowPlayerLeaveWhileWaiting,
    MM_5SM3nS_GWG_ARM_SC_AllowPlayerLeaveWhileBattling,
    MM_5SM3nS_GWG_ARM_SC_SomeOneLeaveGameWhileBattling,
    MM_5SM3nS_GWG_ARM_SC_RequestNewJoinGameInformation,
    MM_5SM3nS_GWG_ARM_SC_TeamMemberIDList,
    MM_5SM3nS_GWG_ARM_CS_JoinGame,
    MM_5SM3nS_GWG_ARM_CS_LeaveGameWhileWaiting,
    MM_5SM3nS_GWG_ARM_CS_LeaveGameWhileBattling,
    MM_5SM3nS_GWG_ARM_CS_LeaveGameDirectly,
    MM_5SM3nS_GWG_ARM_CS_RequestTeamInfo,
    MM_5SM3nS_GWG_ARM_CS_SufferAttack,
    MM_5SM3nS_GWG_ARM_CS_NewJoinGameInformation,
    MM_5SM3nS_GWG_ARM_CS_RequestTeamMemberIDList,
    //************************** IR Product Test **************************
    MM_5SM3nS_GWG_IRTest_Start,
    MM_5SM3nS_GWG_IRTest_Stop,
    MM_5SM3nS_GWG_IRTest_Fire,
    MM_5SM3nS_GWG_IRTest_RevData,
    
    //----------------------------------------------------------------
    //
    //add by jinli 2012-05-18
    //
    MM_C2M_PWMController,
    MM_C2M_LEDController,
    
};

enum _Enum_UI_CONTROL_ID_DEF
{
    UI_Control_MsgID_ServerInfo = 1,
    UI_Control_MsgID_Tank_Init_Successful = 2,
    UI_Control_MsgID_RoomInfo = 3,
    UI_Control_MsgID_RoomCreateSuccess = 4,
    UI_Control_MsgID_RoomDisband = 5,
    UI_Control_MsgID_JoinRoomSuccess = 6,
    UI_Control_MsgID_PlayerjoinRoom = 7,
    UI_Control_MsgID_RoomInitInfo   = 8,
    UI_Control_MsgID_GameStart  = 9,
    UI_Control_MsgID_BeHitted = 10,
    UI_Control_MsgID_Hit = 11,
    UI_Control_MsgID_PlayerLeaveGame = 12,
    UI_Control_MsgID_DisGame = 13,
    UI_Control_MsgID_TankDead = 14,
    UI_Control_MsgID_TeamFail = 15,
    UI_Control_MsgID_GameEnd = 16,
    UI_Control_MsgID_BackWait = 17,
    UI_Control_MsgID_OwnUseItem = 18,
    UI_Control_MsgID_TargetUseItem = 19,
    UI_Control_MsgID_UpdateOwnHp = 20,
    UI_Control_MsgID_UpdateTargetHp = 21,
    UI_Control_MsgID_StopOwnItem = 22,
    UI_Control_MsgID_StopTargetItem = 23,
    UI_Control_MsgID_GameServerError = 99,
    
    //----------------------------------------------------------------
    //
    //add by jinli 2012-03-01
    //
    MUIM_M2C_ModelReciveUpdateConfigerData = 10000,
    MUIM_M2C_ModelInformation_BatteryPower,
    
    //
    //add by jinli 2012-03-01
    //
    MUIM_M2C_OpenPDASoundToModelResponse,
    MUIM_M2C_ClosePDASoundToModelResponse,
    MUIM_M2C_OpenModelSoundToPDAResponse,
    MUIM_M2C_CloseModelSoundToPDAResponse,
    MUIM_M2C_OpenStreamMusicResponse,
    MUIM_M2C_CloseStreamMusicResponse,
    
    //
    //add by jinli 2012-03-29
    //
    MUIM_M2C_SaveSpeakerMusicPCMDataResponse,
    
    //
    //add by jinli 2012-04-14
    //
    MUIM_M2C_DisConnectModel,
    MUIM_M2C_OpenSpeakerResponse,
    MUIM_M2C_CloseSpeakerResponse,
    
    //
    //add by jinli 2012-06-16
    //
    MUIM_M2C_ClearProductTesterResponse,
    
    //----------------------------------------------------------------
    
    //
    //add by jinli 2012-09-06
    //
    MUIM_M2C_NetSignalPowerful = 10013,
    MUIM_M2C_NetSignalWeek,
    
    //
    //add by jinli 2012-11-05
    //
    MUIM_M2C_ModelAdvanceInfo,
    
    //
    //add by jinli 2012-12-18
    //
    MUIM_M2C_5HT6_GroundDistance = 10016,
    MUIM_M2C_5HT6_CommonUse,
    MUIM_M2C_5RM6n_BeAttackedByGun,
    MUIM_M2C_5RM6n_HeadBeAttacked,
    MUIM_M2C_5RM6n_WaistAngle,
    MUIM_M2C_5RM6n_ReportGunStatus,
    MUIM_M2C_5SM3nS_EnemyAttack,
    
    
    
    ////////////////////////////////////////// GWG //////////////////////////////////////////
    MUIM_5SM3nS_GWG_KickPlayerResponse              = 10100,
    MUIM_5SM3nS_GWG_DisbandGameResponse             = 10101,
    MUIM_5SM3nS_GWG_LeaveGameResponse               = 10102,
    MUIM_5SM3nS_GWG_StartGameResponse               = 10103,
    MUIM_5SM3nS_GWG_SomeBodyJoinGame                = 10104,
    MUIM_5SM3nS_GWG_SomeBodyLeaveGame               = 10105,
    MUIM_5SM3nS_GWG_SearchGameServerResponse        = 10106,
    MUIM_5SM3nS_GWG_StopSearchGameServerResponse    = 10107,
    MUIM_5SM3nS_GWG_SearchedGameServerInfo          = 10108,
    MUIM_5SM3nS_GWG_FinishedSearchGameServer        = 10109,
    MUIM_5SM3nS_GWG_JoinSearchedGameServerResponse  = 10110,
    MUIM_5SM3nS_GWG_GameServerTeamInfo              = 10111,
    MUIM_5SM3nS_GWG_CreateGameResponse              = 10112,
    MUIM_5SM3nS_GWG_HostStartGame                   = 10113,
    MUIM_5SM3nS_GWG_HostDisbandGame                 = 10114,
    MUIM_5SM3nS_GWG_LeaveGameWhileBattlingResponse  = 10115,
    MUIM_5SM3nS_GWG_GameServerMessage               = 10116,
    MUIM_5SM3nS_GWG_GameOver                        = 10117,
    MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling  = 10118,
    MUIM_5SM3nS_GWG_BeKickedoutByHost               = 10119,
    MUIM_5SM3nS_GWG_TeamMemberIDList                = 10120,
};

enum _enum_MOVE_STATE_DEF
{
    MOVE_STATE_Stop = 0,
    MOVE_STATE_Up,
    MOVE_STATE_Down,
    MOVE_STATE_Left,
    MOVE_STATE_Right,
};

enum _enum_RATATION_STATE_DEF
{
    RATATION_STATE_Stop = 0,
    RATATION_STATE_Right,
    RATATION_STATE_Left,
};

enum _enum_PUSHPULL_STATE_DEF
{
    PUSHPULL_STATE_Stop = 0,
    PUSHPULL_STATE_Up,
    PUSHPULL_STATE_Down,
};

#endif

