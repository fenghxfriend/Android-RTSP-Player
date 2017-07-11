#include "MsgDef.h"
#include "../NetWork/MsgStream.h"
#include "../../TEEEngine/base/TEE_system.h"

RegMessage(ConnectAcceptMsg);
RegMessage(ClientConnectMsg);
RegMessage(ClientDisconnectMsg);
RegMessage(CreateRoomMsg);

void CreateRoomMsg::Serialize( MsgStream& stream )
{
    info.Serialize(stream);
    stream.SerializeString(data.name);
    stream.SerializeInt(data.lvl);
    stream.SerializeInt(data.atk);
    stream.SerializeInt(data.def);
    stream.SerializeInt(data.baseHp);
    stream.SerializeInt(data.gameNumber);
    stream.SerializeRaw(&data.hitRate, sizeof(float));
    stream.SerializeRaw(&data.winRate, sizeof(float));
    stream.SerializeString(macAddress);
}

RegMessage(ConnectSuccess);
RegMessage(BrowseGameServerMsg);

RegMessage(ServerInfoMsg);
void ServerInfoMsg::Serialize( MsgStream& stream )
{
	stream.SerializeString(m_info.name);
    stream.SerializeString(m_info.password);
	stream.SerializeInt(m_info.isNeedPassword);
    stream.SerializeInt(m_info.warType);
    stream.SerializeInt(m_info.curPlayerNumber);
    stream.SerializeInt(m_info.maxPlayerNumber);
    stream.SerializeInt(m_info.warTime);
    stream.SerializeInt(m_info.serverState);
    stream.SerializeInt(m_info.teamNumber);
}

RegMessage(SearchTankServer);
RegMessage(RequestControlTank);
void RequestControlTank::Serialize( MsgStream& stream )
{
	stream.SerializeString(password);
    
}

RegMessage(FreeTankMsg);
void FreeTankMsg::Serialize( MsgStream& stream )
{
	stream.SerializeString(info.name);
	stream.SerializeInt(info.state);

}

RegMessage(TankCanBeConnect);
void TankCanBeConnect::Serialize( MsgStream& stream )
{
	stream.SerializeInt(reason);
    
}

RegMessage(CreateRoomSuccessMsg);
void CreateRoomSuccessMsg::Serialize( MsgStream& stream )
{
    stream.SerializeInt(playerID);
    info.Serialize(stream);
}

RegMessage(DeletePlayerMsg);
void DeletePlayerMsg::Serialize(MsgStream& stream)
{
    stream.SerializeInt(playerID);
}
RegMessage(TankInitInfo);
RegMessage(DisbandRoomMsg);
RegMessage(RoomDisbandMsg);
RegMessage(JoinRoomMsg);
RegMessage(ExitGameMsg);
void JoinRoomMsg::Serialize(MsgStream& stream)
{
    stream.SerializeInt(teamID);
    stream.SerializeString(data.name);
    stream.SerializeInt(data.lvl);
    stream.SerializeInt(data.atk);
    stream.SerializeInt(data.def);
    stream.SerializeInt(data.baseHp);
    stream.SerializeInt(data.gameNumber);
    stream.SerializeRaw(&data.hitRate, sizeof(float));
    stream.SerializeRaw(&data.winRate, sizeof(float));
    stream.SerializeString(macAddress);
}
RegMessage(JoinRoomAnswerMsg);
void JoinRoomAnswerMsg::Serialize( MsgStream& stream )
{
    stream.SerializeInt(playerID);
    info.Serialize(stream);
}
RegMessage(PlayerJoinRoomMsg);
void PlayerJoinRoomMsg::Serialize( MsgStream& stream )
{
    stream.SerializeString(playerData.name);
    stream.SerializeInt(playerData.teamID);
    stream.SerializeInt(playerData.playerID);
    stream.SerializeInt(playerData.lvl);
    stream.SerializeInt(playerData.gameNumber);
    stream.SerializeRaw(&playerData.winRate,sizeof(float));
}
void TankInitInfo::Serialize( MsgStream& stream )
{
    stream.SerializeInt(info.activeWifiChannel);
    stream.SerializeString(info.modelName);
    stream.SerializeString(info.macAddress);
    stream.SerializeString(info.modelType);
    stream.SerializeInt(info.activeSpeakerVolume);
    stream.SerializeInt(info.steerCalibration);
}

RegMessage(AttackMsg);

void AttackMsg::Serialize(MsgStream& stream  )
{
	stream.SerializeInt(type);
	stream.SerializeRaw(&ledParameter,sizeof(unsigned int));
}

RegMessage(MoveMsg);
void MoveMsg::Serialize( MsgStream& stream )
{
    stream.SerializeInt(direction);
    stream.SerializeInt(speed);
}

RegMessage(PushPullMsg);
void PushPullMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(state);
    stream.SerializeInt(speed);
}

RegMessage(RationMsg);
void RationMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(state);
    stream.SerializeInt(speed);
}

RegMessage(ToTankGameStartMsg);
void ToTankGameStartMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(playerID);
    stream.SerializeInt(roomID);
}
RegMessage(ToTankGameEndMsg);

RegMessage(BeginGameMsg);
RegMessage(GameStartMsg);
RegMessage(TankBeAttackedMsg);

void TankBeAttackedMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(playerID);
    stream.SerializeInt(attackType);
    stream.SerializeInt(direction);
}

RegMessage(BeHittedMsg);
RegMessage(HitMsg);

RegMessage(PlayerLeaveGameMsg);

void PlayerLeaveGameMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(playerID);
}

void BeHittedMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(info.type);
    stream.SerializeInt(info.damage);
    stream.SerializeInt(info.curHp);
    stream.SerializeInt(info.direction);
    //stream.SerializeInt(curMp);
    stream.SerializeInt(info.attacker);
}

void HitMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(info.type);
    stream.SerializeInt(info.damage);
    stream.SerializeInt(info.curHp);
    stream.SerializeInt(info.baseHp);
    stream.SerializeString(info.name);
}

RegMessage(PlayerDeadMsg);
void PlayerDeadMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(playerID);
}

RegMessage(TeamFailMsg);
void TeamFailMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(playerID);
}

RegMessage(NormalEndMsg);
void NormalEndMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(info.time_UP);
    stream.SerializeInt(info.dismissFlag);
}

RegMessage(BackWaitMsg);
RegMessage(UpdateRoomMsg);
void UpdateRoomMsg::Serialize(MsgStream &stream)
{
    info.Serialize(stream);
}

RegMessage(UseItemMsg);
void UseItemMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(itemID);
    stream.SerializeInt(hp);
    stream.SerializeRaw(&maxTime, sizeof(float));
    //stream.SerializeRaw(&loopTime, sizeof(float));
}

RegMessage(PlayerUseItemMsg);
void PlayerUseItemMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(playerID);
    stream.SerializeInt(data.itemID);
    stream.SerializeInt(data.curHp);
}

RegMessage(PlayerUpdateHpMsg);
void PlayerUpdateHpMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(playerID);
    stream.SerializeInt(hp);
}

RegMessage(PlayerStopUseItem);
void PlayerStopUseItem::Serialize(MsgStream &stream)
{
    stream.SerializeInt(playerID);
    stream.SerializeInt(reason);
}

RegMessage(AimHandleMsg);
RegMessage(BeAimMsg);
void BeAimMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(playerID);
    stream.SerializeInt(direction);
}

RegMessage(ChangePassword);
void ChangePassword::Serialize(MsgStream &stream)
{
    stream.SerializeString(password);
}

RegMessage(ChangeTankName);
void ChangeTankName::Serialize(MsgStream &stream)
{
    stream.SerializeString(name);
}

RegMessage(ReplyTankSetMsg);
void ReplyTankSetMsg::Serialize(MsgStream &stream)
{
    stream.SerializeInt(success);
}

RegMessage(RequestLeaveGameMsg);


//
//add by jinli 2012-03-01
//
RegMessage(MessageOfUpdateModelConfiger);
void MessageOfUpdateModelConfiger::Serialize(MsgStream &stream)
{
    stream.SerializeString(modelName);
    stream.SerializeString(modelPassword);
    stream.SerializeInt(wifiChannel);
    stream.SerializeInt(passwordIsNull);
    stream.SerializeInt(updateFlagOfmodelName);
    stream.SerializeInt(updateFlagOfmodelPassword);
    stream.SerializeInt(updateFlagOfwifiChannel);
    stream.SerializeString(wifiCountry);
    stream.SerializeInt(isUpdateWifiCountry);
    stream.SerializeInt(speakerVolume);
    stream.SerializeInt(modelOf5BM2SteerCalibration);
}
RegMessage(MessageOfModelReciveUpdateConfigerData);
void MessageOfModelReciveUpdateConfigerData::Serialize(MsgStream &stream)
{
    stream.SerializeInt(flag);
}
RegMessage(MessageOfExecUpdateModelConfiger);
void MessageOfExecUpdateModelConfiger::Serialize(MsgStream &stream)
{
    stream.SerializeInt(flag);
}
RegMessage(MessageOfModelControllerForword);
void MessageOfModelControllerForword::Serialize(MsgStream &stream)
{
    stream.SerializeInt(ratio);
}
RegMessage(MessageOfModelControllerBack);
void MessageOfModelControllerBack::Serialize(MsgStream &stream)
{
    stream.SerializeInt(ratio);
}
RegMessage(MessageOfModelControllerStop);
RegMessage(MessageOfModelControllerStartTurnLeft);
void MessageOfModelControllerStartTurnLeft::Serialize(MsgStream &stream)
{
    stream.SerializeInt(ratio);
}
RegMessage(MessageOfModelControllerStopTurnLeft);
RegMessage(MessageOfModelControllerStartTurnRight);
void MessageOfModelControllerStartTurnRight::Serialize(MsgStream &stream)
{
    stream.SerializeInt(ratio);
}
RegMessage(MessageOfModelControllerStopTurnRight);
RegMessage(MessageOfModelControllerCameraMoveUp);
RegMessage(MessageOfModelControllerCameraMoveDown);
RegMessage(MessageOfModelControllerCameraStop);
RegMessage(MessageOfModelControllerStartLeftLED);
void MessageOfModelControllerStartLeftLED::Serialize(MsgStream &stream)
{
    stream.SerializeInt(onDuration);
    stream.SerializeInt(offDuration);
}
RegMessage(MessageOfModelControllerStopLeftLED);
RegMessage(MessageOfModelControllerStartRightLED);
void MessageOfModelControllerStartRightLED::Serialize(MsgStream &stream)
{
    stream.SerializeInt(onDuration);
    stream.SerializeInt(offDuration);
}
RegMessage(MessageOfModelControllerStopRightLED);
RegMessage(MessageOfModelControllerStartBehindLED);
void MessageOfModelControllerStartBehindLED::Serialize(MsgStream &stream)
{
    stream.SerializeInt(onDuration);
    stream.SerializeInt(offDuration);
}
RegMessage(MessageOfModelControllerStopBehindLED);
RegMessage(MessageOfModelControllerOpenFrontLight);
RegMessage(MessageOfModelControllerCloseFrontLight);
RegMessage(MessageOfModelControllerOpenSpeaker);
void MessageOfModelControllerOpenSpeaker::Serialize(MsgStream &stream)
{
    stream.SerializeInt(isDefault);
    stream.SerializeString(filename);
    stream.SerializeInt(remark);
}
RegMessage(MessageOfModelControllerCloseSpeaker);
void MessageOfModelControllerCloseSpeaker::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfModelInformationBatteryPower);
void MessageOfModelInformationBatteryPower::Serialize(MsgStream &stream)
{
    stream.SerializeString(power);
}

//
//add by jinli 2012-03-26
//
RegMessage(MessageOfOpenPDASoundToModel);
void MessageOfOpenPDASoundToModel::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
    stream.SerializeRaw(&PDAAddress, sizeof(NETADDR));
    stream.SerializeInt(cacheTimes);
}
RegMessage(MessageOfOpenPDASoundToModelResponse);
void MessageOfOpenPDASoundToModelResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfClosePDASoundToModel);
void MessageOfClosePDASoundToModel::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfClosePDASoundToModelResponse);
void MessageOfClosePDASoundToModelResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfOpenModelSoundToPDA);
void MessageOfOpenModelSoundToPDA::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfOpenModelSoundToPDAResponse);
void MessageOfOpenModelSoundToPDAResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfCloseModelSoundToPDA);
void MessageOfCloseModelSoundToPDA::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfCloseModelSoundToPDAResponse);
void MessageOfCloseModelSoundToPDAResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfOpenStreamMusic);
void MessageOfOpenStreamMusic::Serialize(MsgStream &stream)
{
    stream.SerializeString(fileName);
    stream.SerializeRaw(&NetAddress, sizeof(NETADDR));
    stream.SerializeInt(cacheTimes);
    stream.SerializeInt(remark);
    stream.SerializeInt(seconds);
    stream.SerializeInt(microSeconds);
    stream.SerializeRaw(&timeDistance, sizeof(struct timeval));
}
RegMessage(MessageOfOpenStreamMusicResponse);
void MessageOfOpenStreamMusicResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfCloseStreamMusic);
void MessageOfCloseStreamMusic::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfCloseStreamMusicResponse);
void MessageOfCloseStreamMusicResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

//
//add by jinli 2012-03-29
//
RegMessage(MessageOfSpeakerMusicPCMData);
void MessageOfSpeakerMusicPCMData::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
    stream.SerializeRaw(&NetAddress, sizeof(NETADDR));
}
RegMessage(MessageOfSaveSpeakerMusicPCMDataResponse);
void MessageOfSaveSpeakerMusicPCMDataResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(isSuccess);
}

//
//add by jinli 2012-04-14
//
RegMessage(MessageOfOpenSpeakerResponse);
void MessageOfOpenSpeakerResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfCloseSpeakerResponse);
void MessageOfCloseSpeakerResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

//
//add by jinli 2012-04-18
//
RegMessage(MessageOfCustomDisconnect);
void MessageOfCustomDisconnect::Serialize(MsgStream &stream)
{
    stream.SerializeString(reason);
    stream.SerializeInt(remark);
}
RegMessage(MessageOfPingPong);
void MessageOfPingPong::Serialize(MsgStream &stream)
{
    stream.SerializeRaw(&time, sizeof(time));
    stream.SerializeInt(remark);
}

//
//add by jinli 2012-04-26
//
RegMessage(MessageOfPauseStreamMusic);
void MessageOfPauseStreamMusic::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfPauseStreamMusicResponse);
void MessageOfPauseStreamMusicResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

//
//add by jinli 2012-05-18
//
RegMessage(MessageOfPWMController);
void MessageOfPWMController::Serialize(MsgStream &stream)
{
    stream.SerializeInt(count);
    stream.SerializeRaw(pwmId   , 10 * sizeof(int));
    stream.SerializeRaw(pwmValue, 10 * sizeof(int));
}
RegMessage(MessageOfLEDController);
void MessageOfLEDController::Serialize(MsgStream &stream)
{
    stream.SerializeInt(count);
    stream.SerializeRaw(ledId      , 10 * sizeof(int));
    stream.SerializeRaw(onDuration , 10 * sizeof(int));
    stream.SerializeRaw(offDuration, 10 * sizeof(int));
}

//
//add by jinli 2012-06-06
//
RegMessage(MessageOfClearProductTester);
void MessageOfClearProductTester::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfClearProductTesterResponse);
void MessageOfClearProductTesterResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfClearProductTesterRestart);
void MessageOfClearProductTesterRestart::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

//
//add by jinli 2012-11-05
//
RegMessage(MessageOfNeedModelAdvanceInfo);
void MessageOfNeedModelAdvanceInfo::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOfModelAdvanceInfo);
void MessageOfModelAdvanceInfo::Serialize(MsgStream &stream)
{
    stream.SerializeInt(info.wifiType);
    stream.SerializeRaw(info.speakerVolume, sizeof(int) * 32 * 2);
    stream.SerializeRaw(info.wifiChannel, sizeof(int) * 32 * 2);
    stream.SerializeInt(info.speakerVolumeLength);
    stream.SerializeInt(info.wifiChannelLength);
    stream.SerializeInt(info.remark);
}
RegMessage(MessageOf5SM3nForward);
void MessageOf5SM3nForward::Serialize(MsgStream &stream)
{
    stream.SerializeInt(stall);
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5SM3nBackward);
void MessageOf5SM3nBackward::Serialize(MsgStream &stream)
{
    stream.SerializeInt(stall);
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5SM3nLeftTurn);
void MessageOf5SM3nLeftTurn::Serialize(MsgStream &stream)
{
    stream.SerializeInt(stall);
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5SM3nRightTurn);
void MessageOf5SM3nRightTurn::Serialize(MsgStream &stream)
{
    stream.SerializeInt(stall);
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5SM3nStopMove);
void MessageOf5SM3nStopMove::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5SM3nStopTurn);
void MessageOf5SM3nStopTurn::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

//
//add by jinli 2012-11-29
//
RegMessage(MessageOf5HT6Forward);
void MessageOf5HT6Forward::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6Backward);
void MessageOf5HT6Backward::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6TurnLeft);
void MessageOf5HT6TurnLeft::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6TurnRight);
void MessageOf5HT6TurnRight::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6FlyLeft);
void MessageOf5HT6FlyLeft::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6FlyRight);
void MessageOf5HT6FlyRight::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6GoUp);
void MessageOf5HT6GoUp::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6GoDown);
void MessageOf5HT6GoDown::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6GroundDistance);
void MessageOf5HT6GroundDistance::Serialize(MsgStream &stream)
{
    stream.SerializeInt(distance);
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6MCCommonUse);
void MessageOf5HT6MCCommonUse::Serialize(MsgStream &stream)
{
    stream.SerializeRaw(data, 128);
    stream.SerializeInt(dataSize);
    stream.SerializeInt(messageID);
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6CMCommonUse);
void MessageOf5HT6CMCommonUse::Serialize(MsgStream &stream)
{
    stream.SerializeRaw(data, 128);
    stream.SerializeInt(dataSize);
    stream.SerializeInt(messageID);
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6IPAddress);
void MessageOf5HT6IPAddress::Serialize(MsgStream &stream)
{
    stream.SerializeRaw(&NetAddress, sizeof(NETADDR));
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5HT6StopMove);
void MessageOf5HT6StopMove::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6StopSelfTurn);
void MessageOf5HT6StopSelfTurn::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6StopFlyTurn);
void MessageOf5HT6StopFlyTurn::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}
RegMessage(MessageOf5HT6StopUpDown);
void MessageOf5HT6StopUpDown::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

//
//add by jinli 2012-12-07
//
RegMessage(MessageOf5RM6nForward);
void MessageOf5RM6nForward::Serialize(MsgStream &stream)
{
    stream.SerializeInt(stall);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nBackward);
void MessageOf5RM6nBackward::Serialize(MsgStream &stream)
{
    stream.SerializeInt(stall);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nStopMove);
void MessageOf5RM6nStopMove::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nTurnLeft);
void MessageOf5RM6nTurnLeft::Serialize(MsgStream &stream)
{
    stream.SerializeInt(stall);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nTurnRight);
void MessageOf5RM6nTurnRight::Serialize(MsgStream &stream)
{
    stream.SerializeInt(stall);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nStopTurn);
void MessageOf5RM6nStopTurn::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nWaistTurnLeft);
void MessageOf5RM6nWaistTurnLeft::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nWaistTurnRight);
void MessageOf5RM6nWaistTurnRight::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nWaistStopTurn);
void MessageOf5RM6nWaistStopTurn::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nLeftHandAttack);
void MessageOf5RM6nLeftHandAttack::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nRightHandAttack);
void MessageOf5RM6nRightHandAttack::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nLeftGunFire);
void MessageOf5RM6nLeftGunFire::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nRightGunFire);
void MessageOf5RM6nRightGunFire::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nBeAttackedByGun);
void MessageOf5RM6nBeAttackedByGun::Serialize(MsgStream &stream)
{
    stream.SerializeInt(teamID);
    stream.SerializeInt(gunID);
    stream.SerializeInt(userID);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nHeadBeAttacked);
void MessageOf5RM6nHeadBeAttacked::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nWaistAngle);
void MessageOf5RM6nWaistAngle::Serialize(MsgStream &stream)
{
    stream.SerializeInt(angle);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nStartBattle);
void MessageOf5RM6nStartBattle::Serialize(MsgStream &stream)
{
    stream.SerializeInt(groupID);
    stream.SerializeInt(userID);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nFinishedBattle);
void MessageOf5RM6nFinishedBattle::Serialize(MsgStream &stream)
{
    stream.SerializeInt(status);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5RM6nReportGunStatus);
void MessageOf5RM6nReportGunStatus::Serialize(MsgStream &stream)
{
    stream.SerializeInt(left);
    stream.SerializeInt(right);
    stream.SerializeInt(remark);
}

//
//add by jinli 2012-12-07
//
RegMessage(MessageOf5SM3nSBeginBattle);
void MessageOf5SM3nSBeginBattle::Serialize(MsgStream &stream)
{
    stream.SerializeInt(group);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSFinishBattle);
void MessageOf5SM3nSFinishBattle::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSFire);
void MessageOf5SM3nSFire::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSEnemyAttack);
void MessageOf5SM3nSEnemyAttack::Serialize(MsgStream &stream)
{
    stream.SerializeInt(group);
    stream.SerializeInt(remark);
}

///////////////////////////////////////////////////////// GWG /////////////////////////////////////////////////////////
RegMessage(MessageOf5SM3nSGWGKickPlayer);
void MessageOf5SM3nSGWGKickPlayer::Serialize(MsgStream &stream)
{
    stream.SerializeInt(gameID);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGKickPlayerResponse);
void MessageOf5SM3nSGWGKickPlayerResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(gameID);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGDisbandGame);
void MessageOf5SM3nSGWGDisbandGame::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGDisbandGameResponse);
void MessageOf5SM3nSGWGDisbandGameResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGLeaveGame);
void MessageOf5SM3nSGWGLeaveGame::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGLeaveGameResponse);
void MessageOf5SM3nSGWGLeaveGameResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGStartGame);
void MessageOf5SM3nSGWGStartGame::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGStartGameResponse);
void MessageOf5SM3nSGWGStartGameResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGSomeBodyJoinGameWhileWaiting);
void MessageOf5SM3nSGWGSomeBodyJoinGameWhileWaiting::Serialize(MsgStream &stream)
{
    stream.SerializeInt(gameID);
    stream.SerializeInt(teamID);
    stream.SerializeRaw(&name, 32);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGSomeBodyLeaveGameWhileWaiting);
void MessageOf5SM3nSGWGSomeBodyLeaveGameWhileWaiting::Serialize(MsgStream &stream)
{
    stream.SerializeInt(gameID);
}

RegMessage(MessageOf5SM3nSGWGSomeBodyLeaveGameWhileBattling);
void MessageOf5SM3nSGWGSomeBodyLeaveGameWhileBattling::Serialize(MsgStream &stream)
{
    stream.SerializeInt(gameID);
    stream.SerializeRaw(&nickname, 32);
}

RegMessage(MessageOf5SM3nSGWGSearchGameServer);
void MessageOf5SM3nSGWGSearchGameServer::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGSearchGameServerResponse);
void MessageOf5SM3nSGWGSearchGameServerResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGStopSearchGameServer);
void MessageOf5SM3nSGWGStopSearchGameServer::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGStopSearchGameServerResponse);
void MessageOf5SM3nSGWGStopSearchGameServerResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGSearchedGameServerInfo);
void MessageOf5SM3nSGWGSearchedGameServerInfo::Serialize(MsgStream &stream)
{
    stream.SerializeRaw(&macAddr,  32);
    stream.SerializeRaw(&gameName, 32);
    stream.SerializeRaw(&hostName, 32);
    stream.SerializeInt(gameType);
    stream.SerializeInt(gameTimeLength);
    stream.SerializeInt(redTeamMemberNumber);
    stream.SerializeInt(whiteTeamMemberNumber);
    stream.SerializeInt(blueTeamMemberNumber);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGFinishedSearchGameServer);
void MessageOf5SM3nSGWGFinishedSearchGameServer::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGJoinSearchedGameServer);
void MessageOf5SM3nSGWGJoinSearchedGameServer::Serialize(MsgStream &stream)
{
    stream.SerializeRaw(&macAddr,  32);
    stream.SerializeRaw(&nickName, 32);
    stream.SerializeInt(teamID);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGJoinSearchedGameServerResponse);
void MessageOf5SM3nSGWGJoinSearchedGameServerResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGRequestGameServerTeamInfo);
void MessageOf5SM3nSGWGRequestGameServerTeamInfo::Serialize(MsgStream &stream)
{
    stream.SerializeInt(teamID);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGGameServerTeamInfo);
void MessageOf5SM3nSGWGGameServerTeamInfo::Serialize(MsgStream &stream)
{
    stream.SerializeInt(teamType);
    stream.SerializeInt(teamMemberNumber);
    stream.SerializeRaw(&gameID,    8 * sizeof(int));
    stream.SerializeRaw(&isHost,    8 * sizeof(int));
    stream.SerializeInt(remark);
    stream.SerializeRaw(&nickName0, 32);
    stream.SerializeRaw(&nickName1, 32);
    stream.SerializeRaw(&nickName2, 32);
    stream.SerializeRaw(&nickName3, 32);
    stream.SerializeRaw(&nickName4, 32);
    stream.SerializeRaw(&nickName5, 32);
    stream.SerializeRaw(&nickName6, 32);
    stream.SerializeRaw(&nickName7, 32);
}

RegMessage(MessageOf5SM3nSGWGCreateGame);
void MessageOf5SM3nSGWGCreateGame::Serialize(MsgStream &stream)
{
    stream.SerializeRaw(&gameName, 32);
    stream.SerializeRaw(&nickName, 32);
    stream.SerializeInt(gameType);
    stream.SerializeInt(gameTime);
    stream.SerializeInt(gameTeam);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGCreateGameResponse);
void MessageOf5SM3nSGWGCreateGameResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(gameID);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGHostStartGame);
void MessageOf5SM3nSGWGHostStartGame::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGHostDisbandGame);
void MessageOf5SM3nSGWGHostDisbandGame::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGLeaveGameWhileBattling);
void MessageOf5SM3nSGWGLeaveGameWhileBattling::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGLeaveGameWhileBattlingResponse);
void MessageOf5SM3nSGWGLeaveGameWhileBattlingResponse::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGGameServerMessage);
void MessageOf5SM3nSGWGGameServerMessage::Serialize(MsgStream &stream)
{
    stream.SerializeInt(message);
    stream.SerializeRaw(&nickName, 32);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWGGameOver);
void MessageOf5SM3nSGWGGameOver::Serialize(MsgStream &stream)
{
    stream.SerializeInt(selfStatus);
    stream.SerializeInt(teamStatus);
    stream.SerializeInt(continueWaiting);
}

RegMessage(MessageOf5SM3nSGWGBeKickedoutByHost);

RegMessage(MessageOf5SM3nSGWGRequestTeamMemberIDList);

RegMessage(MessageOf5SM3nSGWGTeamMemberIDList);
void MessageOf5SM3nSGWGTeamMemberIDList::Serialize(MsgStream &stream)
{
    stream.SerializeRaw(teamMemberIDList0, 8 * sizeof(int));
    stream.SerializeRaw(teamMemberIDList1, 8 * sizeof(int));
    stream.SerializeRaw(teamMemberIDList2, 8 * sizeof(int));
}

//************************** IR Product Test **************************
RegMessage(MessageOf5SM3nSGWG_IRTest_Start);
void MessageOf5SM3nSGWG_IRTest_Start::Serialize(MsgStream &stream)
{
    stream.SerializeInt(IRID);
    stream.SerializeInt(mode);
    stream.SerializeInt(hitMusic);
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWG_IRTest_Stop);
void MessageOf5SM3nSGWG_IRTest_Stop::Serialize(MsgStream &stream)
{
    stream.SerializeInt(remark);
}

RegMessage(MessageOf5SM3nSGWG_IRTest_RevData);
void MessageOf5SM3nSGWG_IRTest_RevData::Serialize(MsgStream &stream)
{
    stream.SerializeInt(IRID);
    stream.SerializeInt(remark);
}







