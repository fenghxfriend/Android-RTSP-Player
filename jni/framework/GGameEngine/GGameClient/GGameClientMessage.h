#ifndef GGameClientMessage_h
#define GGameClientMessage_h

#include "../../TEEEngine/base/TEE_system.h"

typedef struct
{
    int message;
    
    //MUIM_M2C_NetSignalPowerful
    
    //MUIM_M2C_NetSignalWeek
    
    //UI_Control_MsgID_ServerInfo
	int     UI_Control_MsgID_ServerInfo_state;
	char    UI_Control_MsgID_ServerInfo_name[32];
    char    UI_Control_MsgID_ServerInfo_password[32];
	NETADDR UI_Control_MsgID_ServerInfo_address;
    
    //UI_Control_MsgID_Tank_Init_Successful
    int     UI_Control_MsgID_Tank_Init_Successful_activeWifiChannel;
    char    UI_Control_MsgID_Tank_Init_Successful_modelName[100];
    char    UI_Control_MsgID_Tank_Init_Successful_macAddress[100];
    char    UI_Control_MsgID_Tank_Init_Successful_modelType[100];
    int     UI_Control_MsgID_Tank_Init_Successful_activeSpeakerVolume;
    int     UI_Control_MsgID_Tank_Init_Successful_steerCalibration;
    
    //MUIM_M2C_ModelReciveUpdateConfigerData
    int     MUIM_M2C_ModelReciveUpdateConfigerData_flag;

    //MUIM_M2C_ModelInformation_BatteryPower
    char    MUIM_M2C_ModelInformation_BatteryPower_power[4];
    
    //MUIM_M2C_OpenPDASoundToModelResponse
    int     MUIM_M2C_OpenPDASoundToModelResponse_remark;
    
    //MUIM_M2C_ClosePDASoundToModelResponse
    int     MUIM_M2C_ClosePDASoundToModelResponse_remark;

    //MUIM_M2C_OpenModelSoundToPDAResponse
    int     MUIM_M2C_OpenModelSoundToPDAResponse_remark;
    
    //MUIM_M2C_CloseModelSoundToPDAResponse
    int     MUIM_M2C_CloseModelSoundToPDAResponse_remark;

    //MUIM_M2C_OpenStreamMusicResponse
    int     MUIM_M2C_OpenStreamMusicResponse_remark;
    
    //MUIM_M2C_CloseStreamMusicResponse
    int     MUIM_M2C_CloseStreamMusicResponse_remark;

    //MUIM_M2C_SaveSpeakerMusicPCMDataResponse
    int     MUIM_M2C_SaveSpeakerMusicPCMDataResponse_isSuccess;

    //MUIM_M2C_DisConnectModel

    //MUIM_M2C_OpenSpeakerResponse
    int     MUIM_M2C_OpenSpeakerResponse_remark;
    
    //MUIM_M2C_CloseSpeakerResponse
    int     MUIM_M2C_CloseSpeakerResponse_remark;

    //MUIM_M2C_ClearProductTesterResponse
    int     MUIM_M2C_ClearProductTesterResponse_remark;
    
    //MUIM_M2C_ModelAdvanceInfo
    int     MUIM_M2C_ModelAdvanceInfo_wifiType;
    int     MUIM_M2C_ModelAdvanceInfo_speakerVolume[32][2];
    int     MUIM_M2C_ModelAdvanceInfo_wifiChannel[32][2];
    int     MUIM_M2C_ModelAdvanceInfo_speakerVolumeLength;
    int     MUIM_M2C_ModelAdvanceInfo_wifiChannelLength;
    int     MUIM_M2C_ModelAdvanceInfo_remark;
    
    //MUIM_M2C_5HT6_GroundDistance
    int     MUIM_M2C_5HT6_GroundDistance_distance;
    
    //MUIM_M2C_5HT6_CommonUse
    unsigned char   MUIM_M2C_5HT6_CommonUse_data[128];
    int             MUIM_M2C_5HT6_CommonUse_dataSize;
    int             MUIM_M2C_5HT6_CommonUse_messageID;
    int             MUIM_M2C_5HT6_CommonUse_remark;
    
    //MUIM_M2C_5RM6n_BeAttackedByGun
    int     MUIM_M2C_5RM6n_BeAttackedByGun_teamID;
    int     MUIM_M2C_5RM6n_BeAttackedByGun_gunID;
    int     MUIM_M2C_5RM6n_BeAttackedByGun_userID;
    int     MUIM_M2C_5RM6n_BeAttackedByGun_remark;
    
    //MUIM_M2C_5RM6n_HeadBeAttacked
    int     MUIM_M2C_5RM6n_HeadBeAttacked_remark;
    
    //MUIM_M2C_5RM6n_WaistAngle
    int     MUIM_M2C_5RM6n_WaistAngle_angle;

    //MUIM_M2C_5RM6n_ReportGunStatus
    bool    MUIM_M2C_5RM6n_ReportGunStatus_left;
    bool    MUIM_M2C_5RM6n_ReportGunStatus_right;
    int     MUIM_M2C_5RM6n_ReportGunStatus_remark;
    
    //MUIM_M2C_5SM3nS_EnemyAttack
    int     MUIM_M2C_5SM3nS_EnemyAttack_group;
    int     MUIM_M2C_5SM3nS_EnemyAttack_remark;
    
    //MUIM_5SM3nS_GWG_KickPlayerResponse
    int     MUIM_5SM3nS_GWG_KickPlayerResponse_gameID;
    int     MUIM_5SM3nS_GWG_KickPlayerResponse_remark;
    
    //MUIM_5SM3nS_GWG_DisbandGameResponse
    int     MUIM_5SM3nS_GWG_DisbandGameResponse_remark;
    
    //MUIM_5SM3nS_GWG_LeaveGameResponse
    int     MUIM_5SM3nS_GWG_LeaveGameResponse_remark;

    //MUIM_5SM3nS_GWG_StartGameResponse
    int     MUIM_5SM3nS_GWG_StartGameResponse_remark;
    
    //MUIM_5SM3nS_GWG_SomeBodyJoinGame
    int     MUIM_5SM3nS_GWG_SomeBodyJoinGame_gameID;
    int     MUIM_5SM3nS_GWG_SomeBodyJoinGame_teamID;
    char    MUIM_5SM3nS_GWG_SomeBodyJoinGame_name[32];
    int     MUIM_5SM3nS_GWG_SomeBodyJoinGame_remark;
    
    //MUIM_5SM3nS_GWG_SomeBodyLeaveGame
    int     MUIM_5SM3nS_GWG_SomeBodyLeaveGame_gameID;
    
    //MUIM_5SM3nS_GWG_SearchGameServerResponse
    int     MUIM_5SM3nS_GWG_SearchGameServerResponse_remark;

    //MUIM_5SM3nS_GWG_StopSearchGameServerResponse
    int     MUIM_5SM3nS_GWG_StopSearchGameServerResponse_remark;

    //MUIM_5SM3nS_GWG_SearchedGameServerInfo
    char    MUIM_5SM3nS_GWG_SearchedGameServerInfo_macAddr[32];
    int     MUIM_5SM3nS_GWG_SearchedGameServerInfo_gameType;
    int     MUIM_5SM3nS_GWG_SearchedGameServerInfo_gameTimeLength;
    char    MUIM_5SM3nS_GWG_SearchedGameServerInfo_gameName[32];
    char    MUIM_5SM3nS_GWG_SearchedGameServerInfo_hostName[32];
    int     MUIM_5SM3nS_GWG_SearchedGameServerInfo_redTeamMemberNumber;
    int     MUIM_5SM3nS_GWG_SearchedGameServerInfo_whiteTeamMemberNumber;
    int     MUIM_5SM3nS_GWG_SearchedGameServerInfo_blueTeamMemberNumber;
    int     MUIM_5SM3nS_GWG_SearchedGameServerInfo_remark;
    
    //MUIM_5SM3nS_GWG_FinishedSearchGameServer
    int     MUIM_5SM3nS_GWG_FinishedSearchGameServer_remark;

    //MUIM_5SM3nS_GWG_JoinSearchedGameServerResponse
    int     MUIM_5SM3nS_GWG_JoinSearchedGameServerResponse_remark;
    
    //MUIM_5SM3nS_GWG_GameServerTeamInfo
    int     MUIM_5SM3nS_GWG_GameServerTeamInfo_teamType;
    int     MUIM_5SM3nS_GWG_GameServerTeamInfo_teamMemberNumber;
    int     MUIM_5SM3nS_GWG_GameServerTeamInfo_gameID[8];
    char    MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName0[32];
    char    MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName1[32];
    char    MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName2[32];
    char    MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName3[32];
    char    MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName4[32];
    char    MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName5[32];
    char    MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName6[32];
    char    MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName7[32];
    int     MUIM_5SM3nS_GWG_GameServerTeamInfo_isHost[8];
    int     MUIM_5SM3nS_GWG_GameServerTeamInfo_remark;
    
    //MUIM_5SM3nS_GWG_CreateGameResponse
    int     MUIM_5SM3nS_GWG_CreateGameResponse_gameID;
    int     MUIM_5SM3nS_GWG_CreateGameResponse_remark;
    
    //MUIM_5SM3nS_GWG_HostStartGame
    int     MUIM_5SM3nS_GWG_HostStartGame_remark;
    
    //MUIM_5SM3nS_GWG_HostDisbandGame
    int     MUIM_5SM3nS_GWG_HostDisbandGame_remark;

    //MUIM_5SM3nS_GWG_LeaveGameWhileBattlingResponse
    int     MUIM_5SM3nS_GWG_LeaveGameWhileBattlingResponse_remark;

    //MUIM_5SM3nS_GWG_GameServerMessage
    int     MUIM_5SM3nS_GWG_GameServerMessage_message;
    char    MUIM_5SM3nS_GWG_GameServerMessage_nickName[32];
    int     MUIM_5SM3nS_GWG_GameServerMessage_remark;
    
    //MUIM_5SM3nS_GWG_GameOver
    int     MUIM_5SM3nS_GWG_GameOver_selfStatus;
    int     MUIM_5SM3nS_GWG_GameOver_teamStatus;
    int     MUIM_5SM3nS_GWG_GameOver_continueWaiting;
    
    //MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling    
    int     MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling_gameID;
    char    MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling_nickName[32];

    //MUIM_5SM3nS_GWG_BeKickedoutByHost
    
    //MUIM_5SM3nS_GWG_TeamMemberIDList
    int     MUIM_5SM3nS_GWG_TeamMemberIDList_white[8];
    int     MUIM_5SM3nS_GWG_TeamMemberIDList_red[8];
    int     MUIM_5SM3nS_GWG_TeamMemberIDList_blue[8];
    
} GGameNetMessage;

#endif
