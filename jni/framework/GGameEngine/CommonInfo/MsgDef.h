#ifndef __MESSAGE__DEF__
#define __MESSAGE__DEF__

#include <sys/time.h>
#include "../NetWork/MsgBase.h"
#include "MsgIdDef.h"
#include "CommonStructDef.h"
#include "NewCommonStructDef.h"

class ConnectAcceptMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SM_CONNECT_CONNECTED)
};

//system msg
class ClientConnectMsg : public NetMsgBase
{
	DeclareMessage(MsgID_SM_NEW_CLIENT_CONNECT)
};

class ClientDisconnectMsg : public NetMsgBase
{
	DeclareMessage(MsgID_SM_DEL_CLIENT_CONNECT)
};

//CG msg
class BrowseGameServerMsg : public NetMsgBase
{
	DeclareMessage(MsgID_CG_BROWSE_SERVER_INFO)
};

class CreateRoomMsg : public NetMsgBase
{
	DeclareMessage(MsgID_CG_CREATE_ROOM)
public:
    GameRoomInfo info;
    BasePlayerData data;
    char macAddress[32];
    virtual void Serialize(MsgStream& stream);
};

class DisbandRoomMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CG_DISBAND_ROOM)
public:
};

class JoinRoomMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CG_JOIN_ROOM)
public:
    int teamID;
    BasePlayerData data;
    char macAddress[32];
    virtual void Serialize(MsgStream& stream);
};

class RequestLeaveGameMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CG_REQUEST_LEAVE_GAME)
};

class BeginGameMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CG_BEGIN_GAME)
};

class DeletePlayerMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CG_DELETE_PLAYER)
public:
    int playerID;
    virtual void Serialize(MsgStream& stream);
};

class ExitGameMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CG_EXIT_GAME)
};

class BackWaitMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CG_BACK_WAIT)
};

class UseItemMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CG_USE_ITEM)
public:
    int itemID;
    int hp;
    float maxTime;
    //float loopTime;
    virtual void Serialize(MsgStream& stream);
};

//GC msg
class ConnectSuccess : public NetMsgBase
{
	DeclareMessage(MsgID_SC_CONNECT_SUCCESS)
};

class ServerInfoMsg : public NetMsgBase
{
	DeclareMessage(MsgID_SC_SERVER_INFO)
public:
	virtual void Serialize( MsgStream& stream );
	GameRoomInfo m_info;
};

class CreateRoomSuccessMsg : public NetMsgBase
{
  DeclareMessage(MsgID_SC_CREATEROOM_SUCCESS)
public:
    GameRoomInfo info;
    int          playerID;
    virtual void Serialize( MsgStream& stream );
};

class RoomDisbandMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_ROOM_DISBAND)
};

class JoinRoomAnswerMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_JOIN_ROOM_ANSWER)
public:
    GameRoomInfo info;
    int          playerID;
    virtual void Serialize( MsgStream& stream );
};

class PlayerJoinRoomMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_PLAYER_JOIN_ROOM)
public:
    WaitingRoomPlayerData playerData;
    virtual void Serialize( MsgStream& stream );
};

class GameStartMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_GAME_START)
public:
    
};

class BeHittedMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_BE_HITTED)
public:
    BeAttackData info;
    virtual void Serialize( MsgStream& stream );
};

class HitMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_HIT)
public:
    AttackData info;
    virtual void Serialize( MsgStream& stream );
};

class PlayerLeaveGameMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_PLAYER_LEAVE_GAME)
public:
    int playerID;
    virtual void Serialize( MsgStream& stream );
};

class PlayerDeadMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_PLAYER_DEAD)
public:
    int playerID;
    virtual void Serialize( MsgStream& stream );
};

class TeamFailMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_TEAM_FAIL)
public:
    int playerID;
    virtual void Serialize( MsgStream& stream );
};

class NormalEndMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_NORMAL_END)
public:
    NormalEndData info;
    virtual void Serialize( MsgStream& stream );
};

class UpdateRoomMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_UPDATE_ROOM_INFO)
public:
    GameRoomInfo info;
    virtual void Serialize( MsgStream& stream );
};

class PlayerUseItemMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_PLAYER_USE_ITEM)
public:
    int playerID;
    UseItemData data;
	virtual void Serialize(MsgStream& stream);
};

class PlayerUpdateHpMsg : public NetMsgBase
{
    DeclareMessage(MsgID_SC_PLAYER_UPDATE_HP)
public:
    int playerID;
    int hp;
    virtual void Serialize(MsgStream& stream);
};

class PlayerStopUseItem : public NetMsgBase
{
    DeclareMessage(MsgID_SC_PLAYER_STOP_ITEM)
public:
    int playerID;
    int reason;
	virtual void Serialize(MsgStream& stream);
};

//CT msg
class SearchTankServer : public NetMsgBase
{
	DeclareMessage(MsgID_CT_SEARCH_SERVER)
};

class RequestControlTank : public NetMsgBase
{
    DeclareMessage(MsgID_CT_REQUEST_CONTROL_TANK)
public:
    char password[32];
    virtual void Serialize(MsgStream& stream);
};

class AttackMsg : public NetMsgBase
{
	DeclareMessage(MsgID_CT_ATTACK_MSG)
public:
	int type;
    unsigned int ledParameter;
	virtual void Serialize(MsgStream& stream);
};

class MoveMsg : public NetMsgBase
{
  DeclareMessage(MsgID_CT_MOVE_MSG)
public:
    int direction;
    int speed;
    virtual void Serialize(MsgStream& stream);
};

class PushPullMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CT_PUSH_PULL_MSG)
public:
    int state;
    int speed;
    virtual void Serialize(MsgStream& stream);
};

class ChangePassword : public NetMsgBase
{
    DeclareMessage(MsgID_CT_CHANGE_PASSWORD)
public:
    char password[32];
    virtual void Serialize(MsgStream& stream);
};

class ChangeTankName : public NetMsgBase
{
    DeclareMessage(MsgID_CT_CHANGE_TANK_NAME)
public:
    char name[32];
    virtual void Serialize(MsgStream& stream);
};

class RationMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CT_RATATION_MSG)
public:
    int state;
    int speed;
    virtual void Serialize(MsgStream& stream);
};

class ToTankGameStartMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CT_NOTIFACTION_GAME_START)
public:
    int playerID;
    int roomID;
    virtual void Serialize(MsgStream& stream);
};

class ToTankGameEndMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CT_NOTIFACTION_GAME_END)
};

class AimHandleMsg : public NetMsgBase
{
    DeclareMessage(MsgID_CT_AIM)
};


//TC msg
class FreeTankMsg : public NetMsgBase
{
    DeclareMessage(MsgID_TC_FREE_TANK_INFO)
public:
    TankServerInfo info;
    virtual void Serialize(MsgStream& stream);
};

class TankCanBeConnect : public NetMsgBase
{
	DeclareMessage(MsgID_TC_CAN_BE_CONNECT)
public:
    int reason;
    virtual void Serialize(MsgStream& stream);
};

class TankInitInfo : public NetMsgBase
{
	DeclareMessage(MsgID_TC_TANK_BASE_INFO)
public:
	TankBaseInfo info;
	virtual void Serialize(MsgStream& stream);
};

class TankBeAttackedMsg : public NetMsgBase
{
    DeclareMessage(MsgID_TC_BE_ATTACKED)
public:
    int playerID;
    int attackType;
    int direction;
    virtual void Serialize(MsgStream& stream);
};

class BeAimMsg : public NetMsgBase
{
    DeclareMessage(MsgID_TC_BE_AIM)
public:
    int playerID;
    int direction;
    virtual void Serialize(MsgStream& stream);
};

class ReplyTankSetMsg : public NetMsgBase
{
    DeclareMessage(MsgID_TC_REPLY_SET)
public:
    int success;
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-03-01
//
class MessageOfUpdateModelConfiger : public NetMsgBase
{
    DeclareMessage(MM_C2M_UpdateModelConfiger)
public:
    char modelName[100];
    char modelPassword[100];
    int  wifiChannel;
    int  passwordIsNull;
    int  updateFlagOfmodelName;
    int  updateFlagOfmodelPassword;
    int  updateFlagOfwifiChannel;
    char wifiCountry[200];
    int  isUpdateWifiCountry;
    int  speakerVolume;
    int  modelOf5BM2SteerCalibration;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelReciveUpdateConfigerData : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelReciveUpdateConfigerData)
public:
    int flag;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfExecUpdateModelConfiger : public NetMsgBase
{
    DeclareMessage(MM_C2M_ExecUpdateModelConfiger)
public:
    int flag;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelControllerForword : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_Forword)
public:
    int ratio;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelControllerBack : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_Back)
public:
    int ratio;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelControllerStop : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_Stop)
};
class MessageOfModelControllerStartTurnLeft : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_StartTurnLeft)
public:
    int ratio;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelControllerStopTurnLeft : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_StopTurnLeft)
};
class MessageOfModelControllerStartTurnRight : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_StartTurnRight)
public:
    int ratio;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelControllerStopTurnRight : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_StopTurnRight)
};
class MessageOfModelControllerCameraMoveUp : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_CameraMoveUp)
};
class MessageOfModelControllerCameraMoveDown : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_CameraMoveDown)
};
class MessageOfModelControllerCameraStop : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_CameraStop)
};
class MessageOfModelControllerStartLeftLED : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_LeftLEDStart)
public:
    int onDuration;
    int offDuration;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelControllerStopLeftLED : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_LeftLEDStop)
};
class MessageOfModelControllerStartRightLED : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_RightLEDStart)
public:
    int onDuration;
    int offDuration;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelControllerStopRightLED : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_RightLEDStop)
};
class MessageOfModelControllerStartBehindLED : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_BehindLEDStart)
public:
    int onDuration;
    int offDuration;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelControllerStopBehindLED : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_BehindLEDStop)
};
class MessageOfModelControllerOpenFrontLight : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_FrontLightOpen)
};
class MessageOfModelControllerCloseFrontLight : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_FrontLightClose)
};
class MessageOfModelControllerOpenSpeaker : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_SpeakerOpen)
public:
    int isDefault;
    char filename[100];
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelControllerCloseSpeaker : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelControl_SpeakerClose)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelInformationBatteryPower : public NetMsgBase
{
    DeclareMessage(MM_C2M_ModelInformation_BatteryPower)
public:
    char power[4];
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-03-26
//
class MessageOfOpenPDASoundToModel : public NetMsgBase
{
    DeclareMessage(MM_C2M_OpenPDASoundToModel)
public:
    int     remark;
    NETADDR PDAAddress;
    int     cacheTimes;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfOpenPDASoundToModelResponse : public NetMsgBase
{
    DeclareMessage(MM_M2C_OpenPDASoundToModelResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfClosePDASoundToModel : public NetMsgBase
{
    DeclareMessage(MM_C2M_ClosePDASoundToModel)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfClosePDASoundToModelResponse : public NetMsgBase
{
    DeclareMessage(MM_C2M_ClosePDASoundToModelResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfOpenModelSoundToPDA : public NetMsgBase
{
    DeclareMessage(MM_C2M_OpenModelSoundToPDA)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfOpenModelSoundToPDAResponse : public NetMsgBase
{
    DeclareMessage(MM_C2M_OpenModelSoundToPDAResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfCloseModelSoundToPDA : public NetMsgBase
{
    DeclareMessage(MM_C2M_CloseModelSoundToPDA)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfCloseModelSoundToPDAResponse : public NetMsgBase
{
    DeclareMessage(MM_C2M_CloseModelSoundToPDAResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfOpenStreamMusic : public NetMsgBase
{
    DeclareMessage(MM_C2M_OpenStreamMusic)
public:
    char    fileName[64];
    NETADDR NetAddress;
    int     cacheTimes;
    int     remark;
    int     seconds;
    int     microSeconds;
    struct timeval timeDistance;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfOpenStreamMusicResponse : public NetMsgBase
{
    DeclareMessage(MM_C2M_OpenStreamMusicResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOfCloseStreamMusic : public NetMsgBase
{
    DeclareMessage(MM_C2M_CloseStreamMusic)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfCloseStreamMusicResponse : public NetMsgBase
{
    DeclareMessage(MM_C2M_CloseStreamMusicResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-03-29
//
class MessageOfSpeakerMusicPCMData : public NetMsgBase
{
    DeclareMessage(MM_C2M_SpeakerMusicPCMData)
public:
    NETADDR NetAddress;
    int     remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfSaveSpeakerMusicPCMDataResponse : public NetMsgBase
{
    DeclareMessage(MM_C2M_SaveSpeakerMusicPCMDataResponse)
public:
    int     isSuccess;
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-04-14
//
class MessageOfOpenSpeakerResponse : public NetMsgBase
{
    DeclareMessage(MM_C2M_OpenSpeakerResponse)
public:
    int     remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfCloseSpeakerResponse : public NetMsgBase
{
    DeclareMessage(MM_C2M_CloseSpeakerResponse)
public:
    int     remark;
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-04-18
//
class MessageOfCustomDisconnect : public NetMsgBase
{
    DeclareMessage(MM_C2M_CustomDisconnect)
public:
    char    reason[32];
    int     remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfPingPong : public NetMsgBase
{
    DeclareMessage(MM_C2M_PingPong)
public:
    struct timeval time;
    int     remark;
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-04-26
//
class MessageOfPauseStreamMusic : public NetMsgBase
{
    DeclareMessage(MM_C2M_PauseStreamMusic)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfPauseStreamMusicResponse : public NetMsgBase
{
    DeclareMessage(MM_C2M_PauseStreamMusicResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-05-18
//
class MessageOfPWMController : public NetMsgBase
{
    DeclareMessage(MM_C2M_PWMController)
public:
    int count;
    int pwmId[10];
    int pwmValue[10];
    virtual void Serialize(MsgStream& stream);
};
class MessageOfLEDController : public NetMsgBase
{
    DeclareMessage(MM_C2M_LEDController)
public:
    int count;
    int ledId[10];
    int onDuration[10];
    int offDuration[10];
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-06-06
//
class MessageOfClearProductTester : public NetMsgBase
{
    DeclareMessage(MM_C2M_ClearProductTester)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfClearProductTesterResponse : public NetMsgBase
{
    DeclareMessage(MM_C2M_ClearProductTesterResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfClearProductTesterRestart : public NetMsgBase
{
    DeclareMessage(MM_C2M_ClearProductTesterRestart)
public:
    int remark;
    virtual void Serialize(MsgStream& stream); 
};

//
//add by jinli 2012-11-05
//
class MessageOfNeedModelAdvanceInfo : public NetMsgBase
{
    DeclareMessage(MM_C2M_NeedModelAdvanceInfo)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOfModelAdvanceInfo : public NetMsgBase
{
    DeclareMessage(MM_M2C_ModelAdvanceInfo)
public:
    ModelAdvanceInfo info;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5SM3nForward : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3n_Forward)
public:
    int stall;
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5SM3nBackward : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3n_Backward)
public:
    int stall;
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5SM3nLeftTurn : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3n_LeftTurn)
public:
    int stall;
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5SM3nRightTurn : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3n_RightTurn)
public:
    int stall;
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5SM3nStopMove : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3n_StopMove)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5SM3nStopTurn : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3n_StopTurn)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-11-29
//
class MessageOf5HT6Forward : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_Forward)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6Backward : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_Backward)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6TurnLeft : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_TurnLeft)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6TurnRight : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_TurnRight)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6FlyLeft : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_FlyLeft)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6FlyRight : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_FlyRight)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6GoUp : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_GoUp)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6GoDown : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_GoDown)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6GroundDistance : public NetMsgBase
{
    DeclareMessage(MM_M2C_5HT6_GroundDistance)
public:
    int distance;
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6MCCommonUse : public NetMsgBase
{
    DeclareMessage(MM_M2C_5HT6_CommonUse)
public:
    unsigned char data[128];
    int dataSize;
    int messageID;
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6CMCommonUse : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_CommonUse)
public:
    unsigned char data[128];
    int dataSize;
    int messageID;
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6IPAddress : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_IPAddress)
public:
    NETADDR NetAddress;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5HT6StopMove : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_StopMove)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6StopSelfTurn : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_StopSelfTurn)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6StopFlyTurn : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_StopFlyTurn)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};
class MessageOf5HT6StopUpDown : public NetMsgBase
{
    DeclareMessage(MM_C2M_5HT6_StopUpDown)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-12-07
//
class MessageOf5RM6nForward : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_Forward)
public:
    int stall;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nBackward : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_Backward)
public:
    int stall;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nStopMove : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_StopMove)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nTurnLeft : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_TurnLeft)
public:
    int stall;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nTurnRight : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_TurnRight)
public:
    int stall;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nStopTurn : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_StopTurn)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nWaistTurnLeft : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_WaistTurnLeft)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nWaistTurnRight : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_WaistTurnRight)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nWaistStopTurn : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_WaistStopTurn)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nLeftHandAttack : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_LeftHandAttack)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nRightHandAttack : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_RightHandAttack)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nLeftGunFire : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_LeftGunFire)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nRightGunFire : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_RightGunFire)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nBeAttackedByGun : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_BeAttackedByGun)
public:
    int teamID;
    int gunID;
    int userID;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nHeadBeAttacked : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_HeadBeAttacked)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nWaistAngle : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_WaistAngle)
public:
    int angle;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nStartBattle : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_StartBattle)
public:
    int groupID;
    int userID;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nFinishedBattle : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_FinishedBattle)
public:
    int status;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5RM6nReportGunStatus : public NetMsgBase
{
    DeclareMessage(MM_C2M_5RM6n_ReportGunStatus)
public:
    int left;
    int right;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

//
//add by jinli 2012-12-07
//
class MessageOf5SM3nSBeginBattle : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_BeginBattle)
public:
    int group;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSFinishBattle : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_FinishBattle)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSFire : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_Fire)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSEnemyAttack : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_EnemyAttack)
public:
    int group;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

//////////////////////////////////////////////////// GWG ////////////////////////////////////////////////////
class MessageOf5SM3nSGWGKickPlayer : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_GWG_KickPlayer)
public:
    int gameID;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGKickPlayerResponse : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_KickPlayerResponse)
public:
    int gameID;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGDisbandGame : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_GWG_DisbandGame)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGDisbandGameResponse : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_DisbandGameResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGLeaveGame : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_GWG_LeaveGame)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGLeaveGameResponse : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_LeaveGameResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGStartGame : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_GWG_StartGame)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGStartGameResponse : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_StartGameResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGSomeBodyJoinGameWhileWaiting : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_SomeBodyJoinGameWhileWaiting)
public:
    int gameID;
    int teamID;
    char name[32];
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGSomeBodyLeaveGameWhileWaiting : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_SomeBodyLeaveGameWhileWaiting)
public:
    int gameID;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGSomeBodyLeaveGameWhileBattling : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling)
public:
    int  gameID;
    char nickname[32];
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGSearchGameServer : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_SearchGameServer)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGSearchGameServerResponse : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_SearchGameServerResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGStopSearchGameServer : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_StopSearchGameServer)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGStopSearchGameServerResponse : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_StopSearchGameServerResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGSearchedGameServerInfo : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_SearchedGameServerInfo)
public:
    
    char macAddr[32];
    char gameName[32];
    char hostName[32];
    int  gameType;
    int  gameTimeLength;
    int  redTeamMemberNumber;
    int  whiteTeamMemberNumber;
    int  blueTeamMemberNumber;
    int  remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGFinishedSearchGameServer : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_FinishedSearchGameServer)
public:
    int  remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGJoinSearchedGameServer : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_JoinSearchedGameServer)
public:
    char macAddr[32];
    int  teamID;
    char nickName[32];
    int  remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGJoinSearchedGameServerResponse : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_JoinSearchedGameServerResponse)
public:
    int  remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGRequestGameServerTeamInfo : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_RequestGameServerTeamInfo)
public:
    int  teamID;
    int  remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGGameServerTeamInfo : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_GameServerTeamInfo)
public:
    
    int  teamType;
    int  teamMemberNumber;
    int  gameID[8];
    char nickName0[32];
    char nickName1[32];
    char nickName2[32];
    char nickName3[32];
    char nickName4[32];
    char nickName5[32];
    char nickName6[32];
    char nickName7[32];
    int  isHost[8];
    int  remark;
    
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGCreateGame : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_GWG_CreateGame)
public:
    
    char gameName[32];
    char nickName[32];
    int gameType;
    int gameTime;
    int gameTeam;
    int remark;
    
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGCreateGameResponse : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_CreateGameResponse)
public:
    
    int gameID;
    int remark;
    
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGHostStartGame : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_HostStartGame)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGHostDisbandGame : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_HostDisbandGame)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGLeaveGameWhileBattling : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_GWG_LeaveGameWhileBattling)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGLeaveGameWhileBattlingResponse : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_LeaveGameWhileBattlingResponse)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGGameServerMessage : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_GameServerMessage)
public:
    int  message;
    char nickName[32];
    int  remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGGameOver : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_GameOver)
public:
    int selfStatus;
    int teamStatus;
    int continueWaiting;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWGBeKickedoutByHost : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_BeKickedoutByHost)
};

class MessageOf5SM3nSGWGRequestTeamMemberIDList : public NetMsgBase
{
    DeclareMessage(MM_C2M_5SM3nS_GWG_RequestTeamMemberIDList)
};

class MessageOf5SM3nSGWGTeamMemberIDList : public NetMsgBase
{
    DeclareMessage(MM_M2C_5SM3nS_GWG_TeamMemberIDList)
public:
    int teamMemberIDList0[8];
    int teamMemberIDList1[8];
    int teamMemberIDList2[8];
    virtual void Serialize(MsgStream& stream);
};

//************************** IR Product Test **************************
class MessageOf5SM3nSGWG_IRTest_Start : public NetMsgBase
{
    DeclareMessage(MM_5SM3nS_GWG_IRTest_Start)
public:
    int IRID;
    int mode;
    int hitMusic;
    int fireMusic;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWG_IRTest_Stop : public NetMsgBase
{
    DeclareMessage(MM_5SM3nS_GWG_IRTest_Stop)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWG_IRTest_Fire : public NetMsgBase
{
    DeclareMessage(MM_5SM3nS_GWG_IRTest_Fire)
public:
    int remark;
    virtual void Serialize(MsgStream& stream);
};

class MessageOf5SM3nSGWG_IRTest_RevData : public NetMsgBase
{
    DeclareMessage(MM_5SM3nS_GWG_IRTest_RevData)
public:
    int IRID;
    int remark;
    virtual void Serialize(MsgStream& stream);
};

#endif


