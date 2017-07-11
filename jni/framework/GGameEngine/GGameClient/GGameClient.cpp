#include "../../TEEEngine/base/TEE_system.h"
#include "../GGameCommand/GGameCMDManager.h"
#include "../CommonInfo/MsgIdDef.h"
#include "../GGameController/GGameController.h"
#include "../CommonInfo/NewCommonStructDef.h"
#include "../../GEngine/GEngine.h"
#include "GGameClient.h"

void GGameClient_ModuleRunLoop_C(void *data)
{
    if ( data == NULL ) return;
    ( ( GGameClient * )data )->runLoop();
}

GGameClient& GGameClient::sharedInstance()
{
    static GGameClient instance;
    return instance;
}

void GGameClient::install()
{
    mSourceLock = mUtility.createLock();
    mLiveLock   = mUtility.createLock();

    mUtility.lock(mLiveLock);
    
	CNetBase::Init();
    
    GGameController::sharedInstance().Init();
    
    mUtility.createThread(&mModuleHandler, GGameClient_ModuleRunLoop_C, this);
}

void GGameClient::swallowMessage(int msgID, void* data, int size)
{
	unpackMessage(msgID, data, size);
}

void GGameClient::unInstall()
{
    mUtility.unlock(mLiveLock);
    mUtility.waitingThread(mModuleHandler);
    
    GGameController::sharedInstance().UnInit();
        
    mUtility.destorylock( &mSourceLock );
    mUtility.destorylock( &mLiveLock   );
}

void GGameClient::runLoop()
{
	while( !mUtility.trylock(mLiveLock) )
	{
        GGameController::sharedInstance().session()->Flush();
        GGameCMDManager::sharedInstance().flush();
        net_socket_read_wait(GGameController::sharedInstance().session()->socket(), 5);
	}

    GGameCMDManager::sharedInstance().flush(true);
    
    mUtility.unlock(mLiveLock);
}

void GGameClient::unpackMessage(int msgID, void* data, int size)
{
	//该函数有多线程在访问
	GGameNetMessage newNetMessage;
    newNetMessage.message = msgID;
    
    if ( UI_Control_MsgID_Tank_Init_Successful == msgID )
    {
        struct TankBaseInfo *info = ( struct TankBaseInfo * )data;
        newNetMessage.UI_Control_MsgID_Tank_Init_Successful_activeWifiChannel   = info->activeWifiChannel;
        newNetMessage.UI_Control_MsgID_Tank_Init_Successful_activeSpeakerVolume = info->activeSpeakerVolume;
        newNetMessage.UI_Control_MsgID_Tank_Init_Successful_steerCalibration    = info->steerCalibration;
        memcpy(newNetMessage.UI_Control_MsgID_Tank_Init_Successful_modelName    , info->modelName   , 100);
        memcpy(newNetMessage.UI_Control_MsgID_Tank_Init_Successful_macAddress   , info->macAddress  , 100);
        memcpy(newNetMessage.UI_Control_MsgID_Tank_Init_Successful_modelType    , info->modelType   , 100);
    }
    else if ( MUIM_M2C_ModelReciveUpdateConfigerData == msgID )
    {
        newNetMessage.MUIM_M2C_ModelReciveUpdateConfigerData_flag   =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_ModelInformation_BatteryPower == msgID )
    {
        memcpy(newNetMessage.MUIM_M2C_ModelInformation_BatteryPower_power, data, size);
    }
    else if ( MUIM_M2C_OpenPDASoundToModelResponse == msgID )
    {
        newNetMessage.MUIM_M2C_OpenPDASoundToModelResponse_remark   =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_ClosePDASoundToModelResponse == msgID )
    {
        newNetMessage.MUIM_M2C_ClosePDASoundToModelResponse_remark =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_OpenModelSoundToPDAResponse == msgID )
    {
        newNetMessage.MUIM_M2C_OpenModelSoundToPDAResponse_remark =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_CloseModelSoundToPDAResponse == msgID )
    {
        newNetMessage.MUIM_M2C_CloseModelSoundToPDAResponse_remark =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_OpenStreamMusicResponse == msgID )
    {
        newNetMessage.MUIM_M2C_OpenStreamMusicResponse_remark =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_CloseStreamMusicResponse == msgID )
    {
        newNetMessage.MUIM_M2C_CloseStreamMusicResponse_remark =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_SaveSpeakerMusicPCMDataResponse == msgID )
    {
        newNetMessage.MUIM_M2C_SaveSpeakerMusicPCMDataResponse_isSuccess =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_OpenSpeakerResponse == msgID )
    {
        newNetMessage.MUIM_M2C_OpenSpeakerResponse_remark =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_CloseSpeakerResponse == msgID )
    {
        newNetMessage.MUIM_M2C_CloseSpeakerResponse_remark =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_ClearProductTesterResponse == msgID )
    {
        newNetMessage.MUIM_M2C_ClearProductTesterResponse_remark =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_ModelAdvanceInfo == msgID )
    {
        struct ModelAdvanceInfo *info                                       = ( struct ModelAdvanceInfo * )data;
        newNetMessage.MUIM_M2C_ModelAdvanceInfo_wifiType            =  info->wifiType;
        newNetMessage.MUIM_M2C_ModelAdvanceInfo_speakerVolumeLength =  info->speakerVolumeLength;
        newNetMessage.MUIM_M2C_ModelAdvanceInfo_wifiChannelLength   =  info->wifiChannelLength;
        newNetMessage.MUIM_M2C_ModelAdvanceInfo_remark              =  info->remark;
        memcpy(newNetMessage.MUIM_M2C_ModelAdvanceInfo_speakerVolume,  info->speakerVolume, 32 * 2 * sizeof(int));
        memcpy(newNetMessage.MUIM_M2C_ModelAdvanceInfo_wifiChannel  ,  info->wifiChannel  , 32 * 2 * sizeof(int));
    }
    else if ( MUIM_M2C_5HT6_GroundDistance == msgID )
    {
        newNetMessage.MUIM_M2C_5HT6_GroundDistance_distance =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_5HT6_CommonUse == msgID )
    {
        struct _5HT6M2CCustomData *info                             = ( struct _5HT6M2CCustomData * )data;
        newNetMessage.MUIM_M2C_5HT6_CommonUse_dataSize      =  info->dataSize;
        newNetMessage.MUIM_M2C_5HT6_CommonUse_messageID     =  info->messageID;
        newNetMessage.MUIM_M2C_5HT6_CommonUse_remark        =  info->remark;
        memcpy( newNetMessage.MUIM_M2C_5HT6_CommonUse_data,    info->data, 128 );
    }
    else if ( MUIM_M2C_5RM6n_BeAttackedByGun == msgID )
    {
        struct _5RM6nEnemyAttackInfo *info                          = ( struct _5RM6nEnemyAttackInfo * )data;
        newNetMessage.MUIM_M2C_5RM6n_BeAttackedByGun_teamID =  info->teamID;
        newNetMessage.MUIM_M2C_5RM6n_BeAttackedByGun_gunID  =  info->gunID;
        newNetMessage.MUIM_M2C_5RM6n_BeAttackedByGun_userID =  info->userID;
        newNetMessage.MUIM_M2C_5RM6n_BeAttackedByGun_remark =  info->remark;
    }
    else if ( MUIM_M2C_5RM6n_HeadBeAttacked == msgID )
    {
        newNetMessage.MUIM_M2C_5RM6n_HeadBeAttacked_remark  =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_5RM6n_WaistAngle == msgID )
    {
        newNetMessage.MUIM_M2C_5RM6n_WaistAngle_angle       =  ( *( (int *)data ) );
    }
    else if ( MUIM_M2C_5RM6n_ReportGunStatus == msgID )
    {
        struct _5RM6nGunStatus *info                                = ( struct _5RM6nGunStatus * )data;
        newNetMessage.MUIM_M2C_5RM6n_ReportGunStatus_left   =  info->left;
        newNetMessage.MUIM_M2C_5RM6n_ReportGunStatus_right  =  info->right;
        newNetMessage.MUIM_M2C_5RM6n_ReportGunStatus_remark =  info->remark;
    }
    else if ( MUIM_M2C_5SM3nS_EnemyAttack == msgID )
    {
        struct _5SM3nSEnemyAttack *info                             = ( struct _5SM3nSEnemyAttack * )data;
        newNetMessage.MUIM_M2C_5SM3nS_EnemyAttack_group     =  info->group;
        newNetMessage.MUIM_M2C_5SM3nS_EnemyAttack_remark    =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_KickPlayerResponse == msgID )
    {
        struct _5SM3nSGWGKickPlayerResponseData *info                   = ( struct _5SM3nSGWGKickPlayerResponseData * )data;
        newNetMessage.MUIM_5SM3nS_GWG_KickPlayerResponse_gameID =  info->gameID;
        newNetMessage.MUIM_5SM3nS_GWG_KickPlayerResponse_remark =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_DisbandGameResponse == msgID )
    {
        struct _5SM3nSGWGDisbandGameResponseData *info                      = ( struct _5SM3nSGWGDisbandGameResponseData * )data;
        newNetMessage.MUIM_5SM3nS_GWG_DisbandGameResponse_remark    =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_LeaveGameResponse == msgID )
    {
        struct _5SM3nSGWGLeaveGameResponseData *info                    = ( struct _5SM3nSGWGLeaveGameResponseData * )data;
        newNetMessage.MUIM_5SM3nS_GWG_LeaveGameResponse_remark  =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_StartGameResponse == msgID )
    {
        struct _5SM3nSGWGStartGameResponseData *info                    = ( struct _5SM3nSGWGStartGameResponseData * )data;
        newNetMessage.MUIM_5SM3nS_GWG_StartGameResponse_remark  =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_SomeBodyJoinGame == msgID )
    {
        struct _5SM3nSGWGSomeBodyJoinGameData *info                     = ( struct _5SM3nSGWGSomeBodyJoinGameData * )data;
        newNetMessage.MUIM_5SM3nS_GWG_SomeBodyJoinGame_gameID   =  info->gameID;
        newNetMessage.MUIM_5SM3nS_GWG_SomeBodyJoinGame_teamID   =  info->teamID;
        newNetMessage.MUIM_5SM3nS_GWG_SomeBodyJoinGame_remark   =  info->remark;
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_SomeBodyJoinGame_name, info->name, 32 );
    }
    else if ( MUIM_5SM3nS_GWG_SomeBodyLeaveGame == msgID )
    {
        struct _5SM3nSGWGSomeBodyLeaveGameData *info                    = ( struct _5SM3nSGWGSomeBodyLeaveGameData * )data;
        newNetMessage.MUIM_5SM3nS_GWG_SomeBodyLeaveGame_gameID  =  info->gameID;
    }
    else if ( MUIM_5SM3nS_GWG_SearchGameServerResponse == msgID )
    {
        struct _5SM3nSGWGSearchGameServerResponseData *info                     = ( struct _5SM3nSGWGSearchGameServerResponseData * )data;
        newNetMessage.MUIM_5SM3nS_GWG_SearchGameServerResponse_remark   =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_StopSearchGameServerResponse == msgID )
    {
        struct _5SM3nSGWGStopSearchGameServerResponse *info                         = ( struct _5SM3nSGWGStopSearchGameServerResponse * )data;
        newNetMessage.MUIM_5SM3nS_GWG_StopSearchGameServerResponse_remark   =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_SearchedGameServerInfo == msgID )
    {
        struct _5SM3nSGWGSearchedGameServerInfo *info                                       = ( struct _5SM3nSGWGSearchedGameServerInfo * )data;
        newNetMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_gameType               =  info->gameType;
        newNetMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_gameTimeLength         =  info->gameTimeLength;
        newNetMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_redTeamMemberNumber    =  info->redTeamMemberNumber;
        newNetMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_whiteTeamMemberNumber  =  info->whiteTeamMemberNumber;
        newNetMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_blueTeamMemberNumber   =  info->blueTeamMemberNumber;
        newNetMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_remark                 =  info->remark;
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_macAddr , info->macAddr , 32 );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_gameName, info->gameName, 32 );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_hostName, info->hostName, 32 );
    }
    else if ( MUIM_5SM3nS_GWG_FinishedSearchGameServer == msgID )
    {
        struct _5SM3nSGWGFinishedSearchGameServer *info                         = ( struct _5SM3nSGWGFinishedSearchGameServer * )data;
        newNetMessage.MUIM_5SM3nS_GWG_FinishedSearchGameServer_remark   =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_JoinSearchedGameServerResponse == msgID )
    {
        struct _5SM3nSGWGJoinSearchedGameServerResponse *info                       = ( struct _5SM3nSGWGJoinSearchedGameServerResponse * )data;
        newNetMessage.MUIM_5SM3nS_GWG_JoinSearchedGameServerResponse_remark =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_GameServerTeamInfo == msgID )
    {
        struct _5SM3nSGWGGameServerTeamInfo *info                                   = ( struct _5SM3nSGWGGameServerTeamInfo * )data;
        newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_teamType           =  info->teamType;
        newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_teamMemberNumber   =  info->teamMemberNumber;
        newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_remark             =  info->remark;
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_gameID   , info->gameID   , 8 * sizeof(int) );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName0, info->nickName0, 32 );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName1, info->nickName1, 32 );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName2, info->nickName2, 32 );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName3, info->nickName3, 32 );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName4, info->nickName4, 32 );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName5, info->nickName5, 32 );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName6, info->nickName6, 32 );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName7, info->nickName7, 32 );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_isHost   , info->isHost   , 8 * sizeof(int) );
    }
    else if ( MUIM_5SM3nS_GWG_CreateGameResponse == msgID )
    {
        struct _5SM3nSGWGCreateGameResponse *info                       = ( struct _5SM3nSGWGCreateGameResponse * )data;
        newNetMessage.MUIM_5SM3nS_GWG_CreateGameResponse_gameID =  info->gameID;
        newNetMessage.MUIM_5SM3nS_GWG_CreateGameResponse_remark =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_HostStartGame == msgID )
    {
        struct _5SM3nSGWGHostStartGame *info                        = ( struct _5SM3nSGWGHostStartGame * )data;
        newNetMessage.MUIM_5SM3nS_GWG_HostStartGame_remark  =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_HostDisbandGame == msgID )
    {
        struct _5SM3nSGWGHostDisbandGame *info                          = ( struct _5SM3nSGWGHostDisbandGame * )data;
        newNetMessage.MUIM_5SM3nS_GWG_HostDisbandGame_remark    =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_LeaveGameWhileBattlingResponse == msgID )
    {
        struct _5SM3nSGWGLeaveGameWhileBattlingResponseData *info                   = ( struct _5SM3nSGWGLeaveGameWhileBattlingResponseData * )data;
        newNetMessage.MUIM_5SM3nS_GWG_LeaveGameWhileBattlingResponse_remark =  info->remark;
    }
    else if ( MUIM_5SM3nS_GWG_GameServerMessage == msgID )
    {
        struct _5SM3nSGWGGameServerMessageData *info                    = ( struct _5SM3nSGWGGameServerMessageData * )data;
        newNetMessage.MUIM_5SM3nS_GWG_GameServerMessage_message =  info->message;
        newNetMessage.MUIM_5SM3nS_GWG_GameServerMessage_remark  =  info->remark;
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_GameServerMessage_nickName, info->nickName, 32 );
    }
    else if ( MUIM_5SM3nS_GWG_GameOver == msgID )
    {
        struct _5SM3nSGWGGameOverData *info                             = ( struct _5SM3nSGWGGameOverData * )data;
        newNetMessage.MUIM_5SM3nS_GWG_GameOver_selfStatus       =  info->selfStatus;
        newNetMessage.MUIM_5SM3nS_GWG_GameOver_teamStatus       =  info->teamStatus;
        newNetMessage.MUIM_5SM3nS_GWG_GameOver_continueWaiting  =  info->continueWaiting;
    }
    else if ( MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling == msgID )
    {
        struct _5SM3nSGWGSomebodyLeaveGameWhileBattling *info                        = ( struct _5SM3nSGWGSomebodyLeaveGameWhileBattling * )data;
        newNetMessage.MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling_gameID  =  info->gameID;
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling_nickName, info->nickName, 32 );
    }
    else if ( MUIM_5SM3nS_GWG_TeamMemberIDList == msgID )
    {
        struct _5SM3nSGWGTeamMemberIDList *info = ( struct _5SM3nSGWGTeamMemberIDList * )data;
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_TeamMemberIDList_white, info->white, 8 * sizeof(int) );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_TeamMemberIDList_red  , info->red  , 8 * sizeof(int) );
        memcpy( newNetMessage.MUIM_5SM3nS_GWG_TeamMemberIDList_blue , info->blue  , 8 * sizeof(int) );
    }
    /*
    else
    {
        //MUIM_M2C_NetSignalPowerful
        //MUIM_M2C_NetSignalWeek
        //MUIM_M2C_DisConnectModel
        //MUIM_5SM3nS_GWG_BeKickedoutByHost
    }
    */
    
    GEngine::sharedInstance().swallowNetMessage(newNetMessage);
}





