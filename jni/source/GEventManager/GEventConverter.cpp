#include <jni.h>
#include "GEventConverter.h"
#include "../../framework/GGameEngine/CommonInfo/MsgIdDef.h"
#include "../GAudioManager/GAudioManager.h"
#include "../GVideoPlayer/GVideoPlayer.h"

bool GEventConverter::convert(JNIEnv *env, jobject javaEventData, GEventData &cEventData)
{
	bool result 	  = false;
	int  stringLength = 0;

	do
	{
		if ( !env || !javaEventData ) break;
		if ( GEVT_NETMSG == cEventData.event ) //网络消息
		{
			if ( !mDataConverter.setIntField( env, javaEventData, "message", cEventData.netMessage.message ) ) break;
			if ( UI_Control_MsgID_Tank_Init_Successful 					== cEventData.netMessage.message )
		    {
				if ( !mDataConverter.setIntField			( env, javaEventData, "GM_InitSuccessful_activeWifiChannel"	 						, cEventData.netMessage.UI_Control_MsgID_Tank_Init_Successful_activeWifiChannel 			) ) break;
				if ( !mDataConverter.setIntField			( env, javaEventData, "GM_InitSuccessful_activeSpeakerVolume"						, cEventData.netMessage.UI_Control_MsgID_Tank_Init_Successful_activeSpeakerVolume 			) ) break;
				if ( !mDataConverter.setIntField			( env, javaEventData, "GM_InitSuccessful_steerCalibration"	 						, cEventData.netMessage.UI_Control_MsgID_Tank_Init_Successful_steerCalibration 	    		) ) break;
				stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_InitSuccessful_modelName", cEventData.netMessage.UI_Control_MsgID_Tank_Init_Successful_modelName, 100 );
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_InitSuccessful_modelName_length", stringLength ) ) break;
				stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_InitSuccessful_macAddress", cEventData.netMessage.UI_Control_MsgID_Tank_Init_Successful_macAddress, 100 );
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_InitSuccessful_macAddress_length", stringLength ) ) break;
				stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_InitSuccessful_modelType", cEventData.netMessage.UI_Control_MsgID_Tank_Init_Successful_modelType, 100 );
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_InitSuccessful_modelType_length", stringLength ) ) break;
		    }
		    else if ( MUIM_M2C_ModelReciveUpdateConfigerData 			== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_ModelReciveUpdateConfigerData_flag"						, cEventData.netMessage.MUIM_M2C_ModelReciveUpdateConfigerData_flag 						) ) break;
		    }
		    else if ( MUIM_M2C_OpenPDASoundToModelResponse 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_OpenPDASoundToModelResponse_remark"						, cEventData.netMessage.MUIM_M2C_OpenPDASoundToModelResponse_remark 						) ) break;
		    }
		    else if ( MUIM_M2C_ClosePDASoundToModelResponse 			== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_ClosePDASoundToModelResponse_remark"						, cEventData.netMessage.MUIM_M2C_ClosePDASoundToModelResponse_remark 						) ) break;
		    }
		    else if ( MUIM_M2C_OpenModelSoundToPDAResponse 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_OpenModelSoundToPDAResponse_remark"						, cEventData.netMessage.MUIM_M2C_OpenModelSoundToPDAResponse_remark 						) ) break;
		    }
		    else if ( MUIM_M2C_CloseModelSoundToPDAResponse 			== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_CloseModelSoundToPDAResponse_remark"						, cEventData.netMessage.MUIM_M2C_CloseModelSoundToPDAResponse_remark 						) ) break;
		    }
		    else if ( MUIM_M2C_OpenStreamMusicResponse 					== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_OpenStreamMusicResponse_remark"							, cEventData.netMessage.MUIM_M2C_OpenStreamMusicResponse_remark 							) ) break;
		    }
		    else if ( MUIM_M2C_CloseStreamMusicResponse 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_CloseStreamMusicResponse_remark"							, cEventData.netMessage.MUIM_M2C_CloseStreamMusicResponse_remark 							) ) break;
		    }
		    else if ( MUIM_M2C_SaveSpeakerMusicPCMDataResponse 			== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_SaveSpeakerMusicPCMDataResponse_isSuccess"				, cEventData.netMessage.MUIM_M2C_SaveSpeakerMusicPCMDataResponse_isSuccess					) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_SaveSpeakerMusicPCMDataResponse_action"					, GAudioManager::sharedInstance().hornUpdateAction()										) ) break;
		    }
		    else if ( MUIM_M2C_OpenSpeakerResponse 						== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_OpenSpeakerResponse_remark"								, cEventData.netMessage.MUIM_M2C_OpenSpeakerResponse_remark									) ) break;
		    }
		    else if ( MUIM_M2C_CloseSpeakerResponse 					== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_CloseSpeakerResponse_remark"								, cEventData.netMessage.MUIM_M2C_CloseSpeakerResponse_remark								) ) break;
		    }
		    else if ( MUIM_M2C_ModelAdvanceInfo 						== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_AdvanceInfo_wifiType"										, cEventData.netMessage.MUIM_M2C_ModelAdvanceInfo_wifiType									) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_AdvanceInfo_speakerVolumeLength"							, cEventData.netMessage.MUIM_M2C_ModelAdvanceInfo_speakerVolumeLength						) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_AdvanceInfo_wifiChannelLength"							, cEventData.netMessage.MUIM_M2C_ModelAdvanceInfo_wifiChannelLength							) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_AdvanceInfo_remark"										, cEventData.netMessage.MUIM_M2C_ModelAdvanceInfo_remark									) ) break;
		    	if ( !mDataConverter.set2DInt2ArrayField	( env, javaEventData, "GM_AdvanceInfo_speakerVolume"								, cEventData.netMessage.MUIM_M2C_ModelAdvanceInfo_speakerVolume						, 32 	) ) break;
		    	if ( !mDataConverter.set2DInt2ArrayField	( env, javaEventData, "GM_AdvanceInfo_wifiChannel"									, cEventData.netMessage.MUIM_M2C_ModelAdvanceInfo_wifiChannel						, 32 	) ) break;
		    }
		    else if ( MUIM_M2C_5SM3nS_EnemyAttack 						== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_M2C_5SM3nS_EnemyAttack_group"								, cEventData.netMessage.MUIM_M2C_5SM3nS_EnemyAttack_group									) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_M2C_5SM3nS_EnemyAttack_remark"							, cEventData.netMessage.MUIM_M2C_5SM3nS_EnemyAttack_remark									) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_KickPlayerResponse 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_KickPlayerResponse_gameID"						, cEventData.netMessage.MUIM_5SM3nS_GWG_KickPlayerResponse_gameID							) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_KickPlayerResponse_remark"						, cEventData.netMessage.MUIM_5SM3nS_GWG_KickPlayerResponse_remark							) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_DisbandGameResponse 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_DisbandGameResponse_remark"					, cEventData.netMessage.MUIM_5SM3nS_GWG_DisbandGameResponse_remark							) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_LeaveGameResponse 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_LeaveGameResponse_remark"						, cEventData.netMessage.MUIM_5SM3nS_GWG_LeaveGameResponse_remark							) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_StartGameResponse 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_StartGameResponse_remark"						, cEventData.netMessage.MUIM_5SM3nS_GWG_StartGameResponse_remark							) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_SomeBodyJoinGame 					== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SomeBodyJoinGame_gameID"						, cEventData.netMessage.MUIM_5SM3nS_GWG_SomeBodyJoinGame_gameID								) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SomeBodyJoinGame_teamID"						, cEventData.netMessage.MUIM_5SM3nS_GWG_SomeBodyJoinGame_teamID								) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SomeBodyJoinGame_remark"						, cEventData.netMessage.MUIM_5SM3nS_GWG_SomeBodyJoinGame_remark								) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_SomeBodyJoinGame_name", cEventData.netMessage.MUIM_5SM3nS_GWG_SomeBodyJoinGame_name, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_SomeBodyJoinGame_name_length", stringLength ) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_SomeBodyLeaveGame 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SomeBodyLeaveGame_gameID"						, cEventData.netMessage.MUIM_5SM3nS_GWG_SomeBodyLeaveGame_gameID							) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_SearchGameServerResponse 			== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SearchGameServerResponse_remark"				, cEventData.netMessage.MUIM_5SM3nS_GWG_SearchGameServerResponse_remark						) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_StopSearchGameServerResponse 		== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_StopSearchGameServerResponse_remark"			, cEventData.netMessage.MUIM_5SM3nS_GWG_StopSearchGameServerResponse_remark					) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_SearchedGameServerInfo 			== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_gameType"				, cEventData.netMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_gameType						) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_gameTimeLength"			, cEventData.netMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_gameTimeLength				) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_redTeamMemberNumber"	, cEventData.netMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_redTeamMemberNumber			) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_whiteTeamMemberNumber"	, cEventData.netMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_whiteTeamMemberNumber		) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_blueTeamMemberNumber"	, cEventData.netMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_blueTeamMemberNumber			) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_remark"					, cEventData.netMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_remark						) ) break;

		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_macAddr", cEventData.netMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_macAddr, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_macAddr_length", stringLength ) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_gameName", cEventData.netMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_gameName, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_gameName_length", stringLength ) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_hostName", cEventData.netMessage.MUIM_5SM3nS_GWG_SearchedGameServerInfo_hostName, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_SearchedGameServerInfo_hostName_length", stringLength ) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_FinishedSearchGameServer 			== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_FinishedSearchGameServer_remark"				, cEventData.netMessage.MUIM_5SM3nS_GWG_FinishedSearchGameServer_remark						) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_JoinSearchedGameServerResponse	== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_JoinSearchedGameServerResponse_remark"			, cEventData.netMessage.MUIM_5SM3nS_GWG_JoinSearchedGameServerResponse_remark				) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_GameServerTeamInfo 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_teamType"					, cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_teamType							) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_teamMemberNumber"			, cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_teamMemberNumber					) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_remark"						, cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_remark							) ) break;
		    	if ( !mDataConverter.setIntArrayField 		( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_gameID"						, cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_gameID					, 8 	) ) break;
		    	if ( !mDataConverter.setIntArrayField 		( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_isHost"						, cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_isHost					, 8 	) ) break;

		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName0", cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName0, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName0_length", stringLength ) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName1", cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName1, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName1_length", stringLength ) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName2", cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName2, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName2_length", stringLength ) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName3", cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName3, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName3_length", stringLength ) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName4", cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName4, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName4_length", stringLength ) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName5", cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName5, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName5_length", stringLength ) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName6", cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName6, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName6_length", stringLength ) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName7", cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerTeamInfo_nickName7, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_GameServerTeamInfo_nickName7_length", stringLength ) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_CreateGameResponse 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_CreateGameResponse_gameID"						, cEventData.netMessage.MUIM_5SM3nS_GWG_CreateGameResponse_gameID							) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_CreateGameResponse_remark"						, cEventData.netMessage.MUIM_5SM3nS_GWG_CreateGameResponse_remark							) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_HostStartGame 					== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_HostStartGame_remark"							, cEventData.netMessage.MUIM_5SM3nS_GWG_HostStartGame_remark								) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_HostDisbandGame 					== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_HostDisbandGame_remark"						, cEventData.netMessage.MUIM_5SM3nS_GWG_HostDisbandGame_remark								) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_LeaveGameWhileBattlingResponse 	== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_LeaveGameWhileBattlingResponse_remark"			, cEventData.netMessage.MUIM_5SM3nS_GWG_LeaveGameWhileBattlingResponse_remark				) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_GameServerMessage 				== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_GameServerMessage_message"						, cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerMessage_message							) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_GameServerMessage_remark"						, cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerMessage_remark							) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_GameServerMessage_nickName", cEventData.netMessage.MUIM_5SM3nS_GWG_GameServerMessage_nickName, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_GameServerMessage_nickName_length", stringLength ) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_GameOver 							== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_GameOver_selfStatus"							, cEventData.netMessage.MUIM_5SM3nS_GWG_GameOver_selfStatus									) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_GameOver_teamStatus"							, cEventData.netMessage.MUIM_5SM3nS_GWG_GameOver_teamStatus									) ) break;
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_GameOver_continueWaiting"						, cEventData.netMessage.MUIM_5SM3nS_GWG_GameOver_continueWaiting							) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling 	== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntField			( env, javaEventData, "GM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling_gameID"			, cEventData.netMessage.MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling_gameID				) ) break;
		    	stringLength = mDataConverter.setByteArrayField( env, javaEventData, "GM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling_nickName", cEventData.netMessage.MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling_nickName, 32 );
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling_nickName_length", stringLength ) ) break;
		    }
		    else if ( MUIM_5SM3nS_GWG_TeamMemberIDList 					== cEventData.netMessage.message )
		    {
		    	if ( !mDataConverter.setIntArrayField 		( env, javaEventData, "GM_5SM3nS_GWG_TeamMemberIDList_white"						, cEventData.netMessage.MUIM_5SM3nS_GWG_TeamMemberIDList_white						, 8 	) ) break;
		    	if ( !mDataConverter.setIntArrayField 		( env, javaEventData, "GM_5SM3nS_GWG_TeamMemberIDList_red"							, cEventData.netMessage.MUIM_5SM3nS_GWG_TeamMemberIDList_red						, 8 	) ) break;
		    	if ( !mDataConverter.setIntArrayField 		( env, javaEventData, "GM_5SM3nS_GWG_TeamMemberIDList_blue"							, cEventData.netMessage.MUIM_5SM3nS_GWG_TeamMemberIDList_blue						, 8 	) ) break;
		    }
		    else if ( MUIM_M2C_NetSignalPowerful 						== cEventData.netMessage.message )
		    {

		    }
		    else if ( MUIM_M2C_NetSignalWeek 							== cEventData.netMessage.message )
		    {

		    }
		    else if ( MUIM_M2C_DisConnectModel 							== cEventData.netMessage.message )
		    {

		    }
		    else if ( MUIM_5SM3nS_GWG_BeKickedoutByHost 				== cEventData.netMessage.message )
		    {

		    }
		    else
		    {
		    	break;
		    }
		}
		else if ( GEVT_ENGEVT == cEventData.event ) //事件
		{
			if ( !mDataConverter.setIntField( env, javaEventData, "message", cEventData.engineEvent.type ) ) break;
			if ( EET_DDET_ERR == cEventData.engineEvent.type )
			{

			}
			else if ( EET_VIDEO_PLY == cEventData.engineEvent.type )
			{

			}
			else if ( EET_AUDIO_FILE_PLAYER_START == cEventData.engineEvent.type )
			{
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_AudioManager_PlayFile_Start_Action", cEventData.engineEvent.audioFilePlayerStartAction ) ) break;
			}
			else if ( EET_AUDIO_FILE_PLAYER_STOP == cEventData.engineEvent.type )
			{
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_AudioManager_PlayFile_Stop_Action", cEventData.engineEvent.audioFilePlayerStopAction ) ) break;
			}
			else if ( EET_AUDIO_FILE_RECORD_START == cEventData.engineEvent.type )
			{
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_AudioManager_RecordFile_Start_Action", cEventData.engineEvent.audioFileRecorderStartAction ) ) break;
			}
			else if ( EET_AUDIO_FILE_RECORD_STOP == cEventData.engineEvent.type )
			{
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_AudioManager_RecordFile_Stop_Action", cEventData.engineEvent.audioFileRecorderStopAction ) ) break;
			}
			else if ( EET_AUDIO_FILE_SAVE_FINISH == cEventData.engineEvent.type )
			{
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_AudioManager_SaveFile_FinishAction", cEventData.engineEvent.audioFileSaveAction ) ) break;
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_AudioManager_SaveFile_FinishResult", cEventData.engineEvent.audioFileSaveResult ) ) break;
			}
			else if ( EET_VIDEO_RECORD_RESPONSE_START == cEventData.engineEvent.type )
			{
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_VideoRecorder_StartResponse", cEventData.engineEvent.videoRecordStartResponse ) ) break;
			}
			else if ( EET_VIDEO_RECORD_RESPONSE_STOP == cEventData.engineEvent.type )
			{
				if ( !mDataConverter.setIntField( env, javaEventData, "GM_VideoRecorder_StopResponse", cEventData.engineEvent.videoRecordStopResponse ) ) break;
			}
		    else if ( EET_TASK_KILLER_CMD_KILL 			== cEventData.engineEvent.type )
		    {
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_TasksKillerWillKillID", cEventData.engineEvent.tasksKillerWillKillID ) ) break;
		    }
		    else if ( EET_TASK_KILLER_MSG_KILLED 		== cEventData.engineEvent.type )
		    {
		    	if ( !mDataConverter.setIntField( env, javaEventData, "GM_TasksKillerKilledID", cEventData.engineEvent.tasksKillerKilledID ) ) break;
		    }
		    else if ( EET_TASK_KILLER_MSG_ALL_KILLED	== cEventData.engineEvent.type )
		    {

		    }
		    else if ( EET_VIDEO_CHANGEMODE_FINISH	== cEventData.engineEvent.type )
		    {

		    }
		    else if ( EET_VIDEO_CHANGEMODE_STOPPED	== cEventData.engineEvent.type )
		    {
		    	//GVideoPlayer::sharedInstance().onDidStoppedWhileChangingMode();
		    }
			else
			{
				break;
			}
		}
		else
		{
			break;
		}

		result = true;

	} while(0);

	return result;
}

