#ifndef __COMMAND_DEF__
#define __COMMAND_DEF__

#include <sys/time.h>
#include "CommandBase.h"
#include "../CommonInfo/CommonStructDef.h"
#include "../CommonInfo/NewCommonStructDef.h"

struct CommandDef
{
	enum
	{
        CommandDef_Search_Tank,
        CommandDef_Join_Tank_Server,
        CommandDef_Disconnect_Tank,
		CommandDef_Move,
        CommandDef_PushPull,
        CommandDef_Rotation,
        CommandDef_Attack,
		CommandDef_Create_Room,
        CommandDef_Disband_Room,
        CommandDef_Begin_game,
        CommandDef_Search_Room,
        CommandDef_Join_Room,
        CommandDef_Delete_Player,
        CommandDef_Exit_Game,
        CommandDef_Back_Wait,
        CommandDef_Use_Item,
        CommandDef_Aim,
        CommandDef_Request_Leave,
        CommandDef_Change_Tank_Name,
        CommandDef_Change_Tank_Password,
        
        //----------------------------------------------------------------
        //
        //add by jinli 2012-03-01
        //
        MC_C2M_UpdateModelConfiger      = 10000,
        MC_C2M_ExecUpdateModelConfiger,
        MC_C2M_ModelControl_Forword,
        MC_C2M_ModelControl_Back,
        MC_C2M_ModelControl_Stop,
        MC_C2M_ModelControl_StartTurnLeft,
        MC_C2M_ModelControl_StopTurnLeft,
        MC_C2M_ModelControl_StartTurnRight,
        MC_C2M_ModelControl_StopTurnRight,
        MC_C2M_ModelControl_CameraMoveUp,
        MC_C2M_ModelControl_CameraMoveDown,
        MC_C2M_ModelControl_CameraStop,
        MC_C2M_ModelControl_LeftLEDStart,
        MC_C2M_ModelControl_LeftLEDStop,
        MC_C2M_ModelControl_RightLEDStart,
        MC_C2M_ModelControl_RightLEDStop,
        MC_C2M_ModelControl_BehindLEDStart,
        MC_C2M_ModelControl_BehindLEDStop,
        MC_C2M_ModelControl_FrontLightOpen,
        MC_C2M_ModelControl_FrontLightClose,
        MC_C2M_ModelControl_SpeakerOpen,
        MC_C2M_ModelControl_SpeakerClose,
        
        //
        //add by jinli 2012-03-26
        //
        MC_C2M_ModelControl_OpenPDASoundToModel,
        MC_C2M_ModelControl_OpenPDASoundToModelResponse,
        MC_C2M_ModelControl_ClosePDASoundToModel,
        MC_C2M_ModelControl_ClosePDASoundToModelResponse,
        MC_C2M_ModelControl_OpenModelSoundToPDA,
        MC_C2M_ModelControl_OpenModelSoundToPDAResponse,
        MC_C2M_ModelControl_CloseModelSoundToPDA,
        MC_C2M_ModelControl_CloseModelSoundToPDAResponse,
        MC_C2M_ModelControl_OpenStreamMusic,
        MC_C2M_ModelControl_OpenStreamMusicResponse,
        MC_C2M_ModelControl_CloseStreamMusic,
        MC_C2M_ModelControl_CloseStreamMusicResponse,
        
        //
        //add by jinli 2012-03-29
        //
        MC_C2M_ModelControl_SpeakerMusicPCMData,
        
        //
        //add by jinli 2012-04-18
        //
        MC_C2M_PingPong,
        
        //
        //add by jinli 2012-04-26
        //
        MC_C2M_ModelControl_PauseStreamMusic,
        
        //
        //add by jinli 2012-06-06
        //
        MC_C2M_ModelControl_ClearProductTester,
        MC_C2M_ModelControl_ClearProductTesterResponse,
        MC_C2M_ModelControl_ClearProductTesterRestart,
        
        //
        //add by jinli 2012-05-18
        //
        MC_C2M_ModelControl_PWMController,
        MC_C2M_ModelControl_LEDController,
        
        //
        //add by jinli 2012-11-05
        //
        MC_C2M_NeedModelAdvanceInfo,
        MC_C2M_5SM3n_Forward,
        MC_C2M_5SM3n_Backward,
        MC_C2M_5SM3n_LeftTurn,
        MC_C2M_5SM3n_RightTurn,
        MC_C2M_5SM3n_StopMove,
        MC_C2M_5SM3n_StopTurn,
        
        //
        //add by jinli 2012-11-29
        //
        MC_C2M_5HT6_Forward,
        MC_C2M_5HT6_Backward,
        MC_C2M_5HT6_TurnLeft,
        MC_C2M_5HT6_TurnRight,
        MC_C2M_5HT6_FlyLeft,
        MC_C2M_5HT6_FlyRight,
        MC_C2M_5HT6_GoUp,
        MC_C2M_5HT6_GoDown,
        MC_C2M_5HT6_CommonUse,
        MC_C2M_5HT6_IPAddress,
        MC_C2M_5HT6_StopMove,
        MC_C2M_5HT6_StopSelfTurn,
        MC_C2M_5HT6_StopFlyTurn,
        MC_C2M_5HT6_StopUpDown,
        
        //
        //add by jinli 2012-12-07
        //
        MC_C2M_5RM6n_Forward,
        MC_C2M_5RM6n_Backward,
        MC_C2M_5RM6n_StopMove,
        MC_C2M_5RM6n_TurnLeft,
        MC_C2M_5RM6n_TurnRight,
        MC_C2M_5RM6n_StopTurn,
        MC_C2M_5RM6n_WaistTurnLeft,
        MC_C2M_5RM6n_WaistTurnRight,
        MC_C2M_5RM6n_WaistStopTurn,
        MC_C2M_5RM6n_LeftHandAttack,
        MC_C2M_5RM6n_RightHandAttack,
        MC_C2M_5RM6n_LeftGunFire,
        MC_C2M_5RM6n_RightGunFire,
        MC_C2M_5RM6n_StartBattle,
        MC_C2M_5RM6n_FinishedBattle,
        
        //
        //add by jinli 2012-12-07
        //
        MC_C2M_5SM3nS_BeginBattle,
        MC_C2M_5SM3nS_FinishBattle,
        MC_C2M_5SM3nS_Fire,

        //----------------------------------------------------------------
        //
        //add by jinli 2013-02-18   履带战车对战
        //
        //----------------------------------------------------------------
        MC_C2M_5SM3nS_GWG_KickPlayer,
        MC_C2M_5SM3nS_GWG_DisbandGame,
        MC_C2M_5SM3nS_GWG_LeaveGame,
        MC_C2M_5SM3nS_GWG_StartGame,
        MC_M2C_5SM3nS_GWG_SearchGameServer,
        MC_M2C_5SM3nS_GWG_StopSearchGameServer,
        MC_M2C_5SM3nS_GWG_JoinSearchedGameServer,
        MC_M2C_5SM3nS_GWG_RequestGameServerTeamInfo,
        MC_M2C_5SM3nS_GWG_CreateGame,
        MM_C2M_5SM3nS_GWG_LeaveGameWhileBattling,
        MM_C2M_5SM3nS_GWG_RequestTeamIDList,
        
	};
};

class SearchTankCommand : public CommandBase
{
    DeclareCommand(CommandDef::CommandDef_Search_Tank);
};

class DisconnectTankCommand : public CommandBase
{
    DeclareCommand(CommandDef::CommandDef_Disconnect_Tank);
    CommandBase& operator =(CommandBase& base )
    {
        isCustom    = static_cast<DisconnectTankCommand&>(base).isCustom;
        remark      = static_cast<DisconnectTankCommand&>(base).remark;
        strcpy(reason, static_cast<DisconnectTankCommand&>(base).reason);
        return *this;
    }
public:
    int     isCustom;
    int     remark;
    char    reason[32];
};

class JionTankServerCommand : public CommandBase
{
    DeclareCommand(CommandDef::CommandDef_Join_Tank_Server);
    CommandBase& operator =(CommandBase& base )
    {
        tankID = static_cast<JionTankServerCommand&>(base).tankID;
        strcpy(password, static_cast<JionTankServerCommand&>(base).password);
        strcpy(ip, static_cast<JionTankServerCommand&>(base).ip);
        return *this;
    }
public:
    int  tankID;
    char password[32];
    char ip[16];
};

class MoveCommand : public CommandBase
{
public:
	DeclareCommand(CommandDef::CommandDef_Move)
    enum
	{
		stop,
		Left,
		Up,
		Right,
		Down,
	};
    CommandBase& operator =(CommandBase& base )
    {
        state = static_cast<MoveCommand&>(base).state;
        speed = static_cast<MoveCommand&>(base).speed;
        return *this;
    }
	unsigned int state;
    int speed;
};

class RotationCommand : public CommandBase
{
public:
    DeclareCommand(CommandDef::CommandDef_Rotation)
    CommandBase& operator =(CommandBase& base )
    {
        state = static_cast<RotationCommand&>(base).state;
        speed = static_cast<RotationCommand&>(base).speed;
        return *this;
    }
	unsigned int state;
    int speed;
};

class PushPullCommand : public CommandBase
{
public:
    DeclareCommand(CommandDef::CommandDef_PushPull)
    CommandBase& operator =(CommandBase& base )
    {
        state = static_cast<PushPullCommand&>(base).state;
        speed = static_cast<RotationCommand&>(base).speed;
        return *this;
    }
    int state;
    int speed;
};
class AttackCommand : public CommandBase
{
  public:
    DeclareCommand(CommandDef::CommandDef_Attack)
    CommandBase& operator =(CommandBase& base )
    {
        type = static_cast<AttackCommand&>(base).type;
        ledParameter = static_cast<AttackCommand&>(base).ledParameter;
        return *this;
    }
    int type;
    int ledParameter;
};

class DisbandRoomCommand : public CommandBase
{
    DeclareCommand(CommandDef::CommandDef_Disband_Room)
};

class NewGameCommand : public CommandBase
{
    DeclareCommand(CommandDef::CommandDef_Begin_game)
};

class CreateRoomCommand : public CommandBase
{
public:
	DeclareCommand(CommandDef::CommandDef_Create_Room)
	GameRoomInfo info;
    BasePlayerData data;
    char macAddress[32];
    CommandBase& operator =(CommandBase& base )
    {
        info = static_cast<CreateRoomCommand&>(base).info;
        data = static_cast<CreateRoomCommand&>(base).data;
        
        strcpy(macAddress, static_cast<CreateRoomCommand&>(base).macAddress);
        return *this;
    }
};

class SerachGameRoomCommand : public CommandBase
{
public:  
    DeclareCommand(CommandDef::CommandDef_Search_Room)
};

class JoinRoomCommand : public CommandBase
{
public:  
    DeclareCommand(CommandDef::CommandDef_Join_Room)
    
    CommandBase& operator =(CommandBase& base )
    {
        serverID = static_cast<JoinRoomCommand&>(base).serverID;
        teamID = static_cast<JoinRoomCommand&>(base).teamID;
        strcpy(password, static_cast<JoinRoomCommand&>(base).password);
        data = static_cast<JoinRoomCommand&>(base).data;
        
        strcpy(macAddress, static_cast<JoinRoomCommand&>(base).macAddress);
        return *this;
    }
    int serverID;
    int teamID;
    char password[32];
    char macAddress[32];
    BasePlayerData data;
};

class DeletePlayerCommand : public CommandBase
{
public:  
    DeclareCommand(CommandDef::CommandDef_Delete_Player) 
    CommandBase& operator =(CommandBase& base )
    {
        playerID = static_cast<DeletePlayerCommand&>(base).playerID;
        return *this;
    }
    int playerID;
};

class ExitGameCommand : public CommandBase
{
public:
    DeclareCommand(CommandDef::CommandDef_Exit_Game) 
};

class BackToWaitCommand : public CommandBase
{
public:
    DeclareCommand(CommandDef::CommandDef_Back_Wait) 
};

class UseItemCommand : public CommandBase
{
public:
    DeclareCommand(CommandDef::CommandDef_Use_Item)
    CommandBase& operator =(CommandBase& base )
    {
        itemId = static_cast<UseItemCommand&>(base).itemId;
        hp = static_cast<UseItemCommand&>(base).hp;
        maxTime = static_cast<UseItemCommand&>(base).maxTime;
        return *this;
    }
    int itemId;
    int hp;
    float maxTime;
};

class AimCommand : public CommandBase
{
public:
    DeclareCommand(CommandDef::CommandDef_Aim)
};

class RequestLeaveGameCommand : public CommandBase
{
public:
    DeclareCommand(CommandDef::CommandDef_Request_Leave)
};

class ChangeTankNameCommand : public CommandBase
{
public:
    DeclareCommand(CommandDef::CommandDef_Change_Tank_Name)
    CommandBase& operator =(CommandBase& base )
    {
        strcpy(name, static_cast<ChangeTankNameCommand&>(base).name);
        return *this;
    }
    char name[32];
};

class ChangeTankPasswordCommand : public CommandBase
{
public:
    DeclareCommand(CommandDef::CommandDef_Change_Tank_Password) 
    CommandBase& operator =(CommandBase& base )
    {
        strcpy(password, static_cast<ChangeTankPasswordCommand&>(base).password);
        return *this;
    }
    char password[32];
};

//
//add by jinli 2012-03-01
//
class CommandOfUpdateModelConfiger : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_UpdateModelConfiger) 
    CommandBase& operator =(CommandBase& base )
    {
        strcpy(modelName, static_cast<CommandOfUpdateModelConfiger&>(base).modelName);
        strcpy(modelPassword, static_cast<CommandOfUpdateModelConfiger&>(base).modelPassword);
        wifiChannel = static_cast<CommandOfUpdateModelConfiger&>(base).wifiChannel;
        passwordIsNull = static_cast<CommandOfUpdateModelConfiger&>(base).passwordIsNull;
        updateFlagOfmodelName = static_cast<CommandOfUpdateModelConfiger&>(base).updateFlagOfmodelName;
        updateFlagOfmodelPassword = static_cast<CommandOfUpdateModelConfiger&>(base).updateFlagOfmodelPassword;
        updateFlagOfwifiChannel = static_cast<CommandOfUpdateModelConfiger&>(base).updateFlagOfwifiChannel;
        strcpy(wifiCountry, static_cast<CommandOfUpdateModelConfiger&>(base).wifiCountry);
        isUpdateWifiCountry = static_cast<CommandOfUpdateModelConfiger&>(base).isUpdateWifiCountry;
        speakerVolume = static_cast<CommandOfUpdateModelConfiger&>(base).speakerVolume;
        modelOf5BM2SteerCalibration = static_cast<CommandOfUpdateModelConfiger&>(base).modelOf5BM2SteerCalibration;
        
        return *this;
    }
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
};
class CommandOfExecUpdateModelConfiger : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ExecUpdateModelConfiger) 
    CommandBase& operator =(CommandBase& base )
    {
        flag = static_cast<CommandOfExecUpdateModelConfiger&>(base).flag;
        return *this;
    }
    int flag;
};
class CommandOfModelControlForword : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_Forword) 
    CommandBase& operator =(CommandBase& base )
    {
        ratio = static_cast<CommandOfModelControlForword&>(base).ratio;
        return *this;
    }
    int ratio;
};
class CommandOfModelControlBack : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_Back) 
    CommandBase& operator =(CommandBase& base )
    {
        ratio = static_cast<CommandOfModelControlBack&>(base).ratio;
        return *this;
    }
    int ratio;
};
class CommandOfModelControlStop : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_Stop) 
};
class CommandOfModelControlStartTurnLeft : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_StartTurnLeft) 
    CommandBase& operator =(CommandBase& base )
    {
        ratio = static_cast<CommandOfModelControlStartTurnLeft&>(base).ratio;
        return *this;
    }
    int ratio;
};
class CommandOfModelControlStopTurnLeft : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_StopTurnLeft) 
};
class CommandOfModelControlStartTurnRight : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_StartTurnRight) 
    CommandBase& operator =(CommandBase& base )
    {
        ratio = static_cast<CommandOfModelControlStartTurnRight&>(base).ratio;
        return *this;
    }
    int ratio;
};
class CommandOfModelControlStopTurnRight : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_StopTurnRight) 
};
class CommandOfModelControlCameraMoveUp : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_CameraMoveUp) 
};
class CommandOfModelControlCameraMoveDown : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_CameraMoveDown) 
};
class CommandOfModelControlCameraStop : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_CameraStop) 
};
class CommandOfModelControlStartLeftLED : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_LeftLEDStart) 
    CommandBase& operator =(CommandBase& base )
    {
        onDuration = static_cast<CommandOfModelControlStartLeftLED&>(base).onDuration;
        offDuration = static_cast<CommandOfModelControlStartLeftLED&>(base).offDuration;
        return *this;
    }
    int onDuration;
    int offDuration;
};
class CommandOfModelControlStopLeftLED : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_LeftLEDStop) 
};
class CommandOfModelControlStartRightLED : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_RightLEDStart) 
    CommandBase& operator =(CommandBase& base )
    {
        onDuration = static_cast<CommandOfModelControlStartRightLED&>(base).onDuration;
        offDuration = static_cast<CommandOfModelControlStartRightLED&>(base).offDuration;
        return *this;
    }
    int onDuration;
    int offDuration;
};
class CommandOfModelControlStopRightLED : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_RightLEDStop) 
};
class CommandOfModelControlStartBehindLED : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_BehindLEDStart) 
    CommandBase& operator =(CommandBase& base )
    {
        onDuration = static_cast<CommandOfModelControlStartBehindLED&>(base).onDuration;
        offDuration = static_cast<CommandOfModelControlStartBehindLED&>(base).offDuration;
        return *this;
    }
    int onDuration;
    int offDuration;
};
class CommandOfModelControlStopBehindLED : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_BehindLEDStop) 
};
class CommandOfModelControlOpenFrontLight : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_FrontLightOpen) 
};
class CommandOfModelControlCloseFrontLight : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_FrontLightClose) 
};
class CommandOfModelControlOpenSpeaker : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_SpeakerOpen)
    CommandBase& operator =(CommandBase& base )
    {
        isDefault = static_cast<CommandOfModelControlOpenSpeaker&>(base).isDefault;
        remark = static_cast<CommandOfModelControlOpenSpeaker&>(base).remark;
        strcpy(filename, static_cast<CommandOfModelControlOpenSpeaker&>(base).filename);
        return *this;
    }
    int isDefault;
    char filename[100];
    int remark;
};
class CommandOfModelControlCloseSpeaker : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_SpeakerClose) 
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfModelControlCloseSpeaker&>(base).remark;
        return *this;
    }
    int remark;
};

//
//add by jinli 2012-03-26
//
class CommandOfModelControlOpenPDASoundToModel : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_OpenPDASoundToModel)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfModelControlOpenPDASoundToModel&>(base).remark;
        cacheTimes = static_cast<CommandOfModelControlOpenPDASoundToModel&>(base).cacheTimes;
        memcpy(&PDAAddress, &(static_cast<CommandOfModelControlOpenPDASoundToModel&>(base).PDAAddress), sizeof(NETADDR));
        return *this;
    }
    int     remark;
    int     cacheTimes;
    NETADDR PDAAddress;
};
class CommandOfModelControlClosePDASoundToModel : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_ClosePDASoundToModel)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfModelControlClosePDASoundToModel&>(base).remark;
        return *this;
    }
    int remark;
};
class CommandOfModelControlOpenModelSoundToPDA : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_OpenModelSoundToPDA)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfModelControlOpenModelSoundToPDA&>(base).remark;
        return *this;
    }
    int remark;
};
class CommandOfModelControlCloseModelSoundToPDA : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_CloseModelSoundToPDA)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfModelControlCloseModelSoundToPDA&>(base).remark;
        return *this;
    }
    int remark;
};
class CommandOfModelControlOpenStreamMusic : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_OpenStreamMusic)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfModelControlOpenStreamMusic&>(base).remark;
        cacheTimes = static_cast<CommandOfModelControlOpenStreamMusic&>(base).cacheTimes;
        seconds = static_cast<CommandOfModelControlOpenStreamMusic&>(base).seconds;
        microSeconds = static_cast<CommandOfModelControlOpenStreamMusic&>(base).microSeconds;
        strcpy(fileName, static_cast<CommandOfModelControlOpenStreamMusic&>(base).fileName);
        memcpy(&NetAddress, &(static_cast<CommandOfModelControlOpenStreamMusic&>(base).NetAddress), sizeof(NETADDR));
        memcpy(&timeDistance, &(static_cast<CommandOfModelControlOpenStreamMusic&>(base).timeDistance), sizeof(struct timeval));
        return *this;
    }
    int     remark;
    NETADDR NetAddress;
    int     cacheTimes;
    char    fileName[64];
    int     seconds;
    int     microSeconds;
    struct timeval timeDistance;
};
class CommandOfModelControlCloseStreamMusic : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_CloseStreamMusic)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfModelControlCloseStreamMusic&>(base).remark;
        return *this;
    }
    int remark;
};

//
//add by jinli 2012-03-29
//
class CommandOfSpeakerMusicPCMData : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_SpeakerMusicPCMData)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfSpeakerMusicPCMData&>(base).remark;
        memcpy(&NetAddress, &(static_cast<CommandOfSpeakerMusicPCMData&>(base).NetAddress), sizeof(NETADDR));
        return *this;
    }
    NETADDR NetAddress;
    int     remark;
};

//
//add by jinli 2012-04-18
//
class CommandOfPingPong : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_PingPong)
    CommandBase& operator =(CommandBase& base )
    {
        memcpy(&time, &(static_cast<CommandOfPingPong&>(base).time), sizeof(time));
        remark   = static_cast<CommandOfPingPong&>(base).remark;
        return *this;
    }
    timeval time;
    int     remark;
};

//
//add by jinli 2012-04-26
//
class CommandOfModelControlPauseStreamMusic : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_PauseStreamMusic)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfModelControlPauseStreamMusic&>(base).remark;
        return *this;
    }
    int remark;
};

//
//add by jinli 2012-05-18
//
class CommandOfPWMController : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_PWMController)
    CommandBase& operator =(CommandBase& base )
    {
        count = static_cast<CommandOfPWMController&>(base).count;
        memcpy(pwmId, &(static_cast<CommandOfPWMController&>(base).pwmId), 10 * sizeof(int));
        memcpy(pwmValue, &(static_cast<CommandOfPWMController&>(base).pwmValue), 10 * sizeof(int));
        return *this;
    }
    int count;
    int pwmId[10];
    int pwmValue[10];
};
class CommandOfLEDController : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_LEDController)
    CommandBase& operator =(CommandBase& base )
    {
        count = static_cast<CommandOfLEDController&>(base).count;
        memcpy(ledId, &(static_cast<CommandOfLEDController&>(base).ledId), 10 * sizeof(int));
        memcpy(onDuration, &(static_cast<CommandOfLEDController&>(base).onDuration), 10 * sizeof(int));
        memcpy(offDuration, &(static_cast<CommandOfLEDController&>(base).offDuration), 10 * sizeof(int));
        return *this;
    }
    int count;
    int ledId[10];
    int onDuration[10];
    int offDuration[10];
};

//
//add by jinli 2012-06-06
//
class CommandOfClearProductTester : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_ClearProductTester)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfClearProductTester&>(base).remark;
        return *this;
    }
    int remark;
};
class CommandOfClearProductTesterRestart : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_ModelControl_ClearProductTesterRestart)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfClearProductTesterRestart&>(base).remark;
        return *this;
    }
    int remark;
};

//
//add by jinli 2012-11-05
//
class CommandOfNeedModelAdvanceInfo : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_NeedModelAdvanceInfo)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOfNeedModelAdvanceInfo&>(base).remark;
        return *this;
    }
    int remark;
};
class CommandOf5SM3nForward : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3n_Forward)
    CommandBase& operator =(CommandBase& base )
    {
        stall = static_cast<CommandOf5SM3nForward&>(base).stall;
        remark = static_cast<CommandOf5SM3nForward&>(base).remark;
        return *this;
    }
    int stall;
    int remark;
};
class CommandOf5SM3nBackward : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3n_Backward)
    CommandBase& operator =(CommandBase& base )
    {
        stall = static_cast<CommandOf5SM3nBackward&>(base).stall;
        remark = static_cast<CommandOf5SM3nBackward&>(base).remark;
        return *this;
    }
    int stall;
    int remark;
};
class CommandOf5SM3nLeftTurn : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3n_LeftTurn)
    CommandBase& operator =(CommandBase& base )
    {
        stall = static_cast<CommandOf5SM3nLeftTurn&>(base).stall;
        remark = static_cast<CommandOf5SM3nLeftTurn&>(base).remark;
        return *this;
    }
    int stall;
    int remark;
};
class CommandOf5SM3nRightTurn : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3n_RightTurn)
    CommandBase& operator =(CommandBase& base )
    {
        stall = static_cast<CommandOf5SM3nRightTurn&>(base).stall;
        remark = static_cast<CommandOf5SM3nRightTurn&>(base).remark;
        return *this;
    }
    int stall;
    int remark;
};
class CommandOf5SM3nStopMove : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3n_StopMove)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5SM3nStopMove&>(base).remark;
        return *this;
    }
    int remark;
};
class CommandOf5SM3nStopTurn : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3n_StopTurn)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5SM3nStopTurn&>(base).remark;
        return *this;
    }
    int remark;
};

//
//add by jinli 2012-11-29
//
class CommandOf5HT6Forward : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_Forward)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6Forward&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6Backward : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_Backward)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6Backward&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6TurnLeft : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_TurnLeft)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6TurnLeft&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6TurnRight : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_TurnRight)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6TurnRight&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6FlyLeft : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_FlyLeft)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6FlyLeft&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6FlyRight : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_FlyRight)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6FlyRight&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6GoUp : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_GoUp)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6GoUp&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6GoDown : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_GoDown)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6GoDown&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6CMCommonUse : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_CommonUse)
    CommandBase& operator =(CommandBase& base )
    {
        memcpy(data, &(static_cast<CommandOf5HT6CMCommonUse&>(base).data), 128 * sizeof(char));
        dataSize = static_cast<CommandOf5HT6CMCommonUse&>(base).dataSize;
        messageID = static_cast<CommandOf5HT6CMCommonUse&>(base).messageID;
        remark = static_cast<CommandOf5HT6CMCommonUse&>(base).remark;
        return *this;
    }
    unsigned char data[128];
    int dataSize;
    int messageID;
    int remark;
};

class CommandOf5HT6IPAddress : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_IPAddress)
    CommandBase& operator =(CommandBase& base )
    {
        memcpy(&NetAddress, &(static_cast<CommandOf5HT6IPAddress&>(base).NetAddress), sizeof(NETADDR));
        remark = static_cast<CommandOf5HT6IPAddress&>(base).remark;
        return *this;
    }
    NETADDR NetAddress;
    int remark;
};

class CommandOf5HT6StopMove : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_StopMove)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6StopMove&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6StopSelfTurn : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_StopSelfTurn)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6StopSelfTurn&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6StopFlyTurn : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_StopFlyTurn)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6StopFlyTurn&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5HT6StopUpDown : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5HT6_StopUpDown)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5HT6StopUpDown&>(base).remark;
        return *this;
    }
    int remark;
};

//
//add by jinli 2012-12-07
//
class CommandOf5RM6nForward : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_Forward)
    CommandBase& operator =(CommandBase& base )
    {
        stall = static_cast<CommandOf5RM6nForward&>(base).stall;
        remark = static_cast<CommandOf5RM6nForward&>(base).remark;
        return *this;
    }
    int stall;
    int remark;
};

class CommandOf5RM6nBackward : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_Backward)
    CommandBase& operator =(CommandBase& base )
    {
        stall = static_cast<CommandOf5RM6nBackward&>(base).stall;
        remark = static_cast<CommandOf5RM6nBackward&>(base).remark;
        return *this;
    }
    int stall;
    int remark;
};

class CommandOf5RM6nStopMove : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_StopMove)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5RM6nStopMove&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5RM6nTurnLeft : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_TurnLeft)
    CommandBase& operator =(CommandBase& base )
    {
        stall = static_cast<CommandOf5RM6nTurnLeft&>(base).stall;
        remark = static_cast<CommandOf5RM6nTurnLeft&>(base).remark;
        return *this;
    }
    int stall;
    int remark;
};

class CommandOf5RM6nTurnRight : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_TurnRight)
    CommandBase& operator =(CommandBase& base )
    {
        stall = static_cast<CommandOf5RM6nTurnRight&>(base).stall;
        remark = static_cast<CommandOf5RM6nTurnRight&>(base).remark;
        return *this;
    }
    int stall;
    int remark;
};

class CommandOf5RM6nStopTurn : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_StopTurn)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5RM6nStopTurn&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5RM6nWaistTurnLeft : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_WaistTurnLeft)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5RM6nWaistTurnLeft&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5RM6nWaistTurnRight : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_WaistTurnRight)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5RM6nWaistTurnRight&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5RM6nWaistStopTurn : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_WaistStopTurn)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5RM6nWaistStopTurn&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5RM6nLeftHandAttack : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_LeftHandAttack)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5RM6nLeftHandAttack&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5RM6nRightHandAttack : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_RightHandAttack)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5RM6nRightHandAttack&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5RM6nLeftGunFire : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_LeftGunFire)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5RM6nLeftGunFire&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5RM6nRightGunFire : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_RightGunFire)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5RM6nRightGunFire&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5RM6nStartBattle : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_StartBattle)
    CommandBase& operator =(CommandBase& base )
    {
        groupID = static_cast<CommandOf5RM6nStartBattle&>(base).groupID;
        userID = static_cast<CommandOf5RM6nStartBattle&>(base).userID;
        remark = static_cast<CommandOf5RM6nStartBattle&>(base).remark;
        return *this;
    }
    int groupID;
    int userID;
    int remark;
};

class CommandOf5RM6nFinishedBattle : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5RM6n_FinishedBattle)
    CommandBase& operator =(CommandBase& base )
    {
        status = static_cast<CommandOf5RM6nFinishedBattle&>(base).status;
        remark = static_cast<CommandOf5RM6nFinishedBattle&>(base).remark;
        return *this;
    }
    int status;
    int remark;
};

//
//add by jinli 2012-12-07
//
class CommandOf5SM3nSBeginBattle : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3nS_BeginBattle)
    CommandBase& operator =(CommandBase& base )
    {
        group = static_cast<CommandOf5SM3nSBeginBattle&>(base).group;
        remark = static_cast<CommandOf5SM3nSBeginBattle&>(base).remark;
        return *this;
    }
    int group;
    int remark;
};

class CommandOf5SM3nSFinishBattle : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3nS_FinishBattle)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5SM3nSFinishBattle&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5SM3nSFire : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3nS_Fire)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5SM3nSFire&>(base).remark;
        return *this;
    }
    int remark;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//add by jinli 2013-02-18   履带战车对战
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CommandOf5SM3nSGWGKickPlayer : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3nS_GWG_KickPlayer)
    CommandBase& operator =(CommandBase& base )
    {
        gameID = static_cast<CommandOf5SM3nSGWGKickPlayer&>(base).gameID;
        remark = static_cast<CommandOf5SM3nSGWGKickPlayer&>(base).remark;
        return *this;
    }
    int gameID;
    int remark;
};

class CommandOf5SM3nSGWGDisbandGame : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3nS_GWG_DisbandGame)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5SM3nSGWGDisbandGame&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5SM3nSGWGLeaveGame : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3nS_GWG_LeaveGame)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5SM3nSGWGLeaveGame&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5SM3nSGWGStartGame : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_C2M_5SM3nS_GWG_StartGame)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5SM3nSGWGStartGame&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5SM3nSGWGSearchGameServer : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_M2C_5SM3nS_GWG_SearchGameServer)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5SM3nSGWGSearchGameServer&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5SM3nSGWGStopSearchGameServer : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_M2C_5SM3nS_GWG_StopSearchGameServer)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5SM3nSGWGStopSearchGameServer&>(base).remark;
        return *this;
    }
    int remark;
};

class CommandOf5SM3nSGWGJoinSearchedGameServer : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_M2C_5SM3nS_GWG_JoinSearchedGameServer)
    CommandBase& operator =(CommandBase& base )
    {
        memcpy(&macAddr , &(static_cast<CommandOf5SM3nSGWGJoinSearchedGameServer&>(base).macAddr) , 32);
        memcpy(&nickName, &(static_cast<CommandOf5SM3nSGWGJoinSearchedGameServer&>(base).nickName), 32);
        teamID = static_cast<CommandOf5SM3nSGWGJoinSearchedGameServer&>(base).teamID;
        remark = static_cast<CommandOf5SM3nSGWGJoinSearchedGameServer&>(base).remark;
        return *this;
    }
    char macAddr[32];
    char nickName[32];
    int  teamID;
    int  remark;
};

class CommandOf5SM3nSGWGRequestGameServerTeamInfo : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_M2C_5SM3nS_GWG_RequestGameServerTeamInfo)
    CommandBase& operator =(CommandBase& base )
    {
        teamID = static_cast<CommandOf5SM3nSGWGRequestGameServerTeamInfo&>(base).teamID;
        remark = static_cast<CommandOf5SM3nSGWGRequestGameServerTeamInfo&>(base).remark;
        return *this;
    }
    
    int teamID;
    int remark;
};

class CommandOf5SM3nSGWGCreateGame : public CommandBase
{
public:
    DeclareCommand(CommandDef::MC_M2C_5SM3nS_GWG_CreateGame)
    CommandBase& operator =(CommandBase& base )
    {
        memcpy(gameName, &(static_cast<CommandOf5SM3nSGWGCreateGame&>(base).gameName), 32);
        memcpy(nickName, &(static_cast<CommandOf5SM3nSGWGCreateGame&>(base).nickName), 32);
        gameType = static_cast<CommandOf5SM3nSGWGCreateGame&>(base).gameType;
        gameTime = static_cast<CommandOf5SM3nSGWGCreateGame&>(base).gameTime;
        gameTeam = static_cast<CommandOf5SM3nSGWGCreateGame&>(base).gameTeam;
        remark = static_cast<CommandOf5SM3nSGWGCreateGame&>(base).remark;
        return *this;
    }
    
    char gameName[32];
    char nickName[32];
    int  gameType;
    int  gameTime;
    int  gameTeam;
    int  remark;
};

class CommandOf5SM3nSGWGLeaveGameWhileBattling : public CommandBase
{
public:
    DeclareCommand(CommandDef::MM_C2M_5SM3nS_GWG_LeaveGameWhileBattling)
    CommandBase& operator =(CommandBase& base )
    {
        remark = static_cast<CommandOf5SM3nSGWGLeaveGameWhileBattling&>(base).remark;
        return *this;
    }
    
    int remark;
};

class CommandOf5SM3nSGWGRequestTeamIDList : public CommandBase
{
    DeclareCommand(CommandDef::MM_C2M_5SM3nS_GWG_RequestTeamIDList);
};

#endif


