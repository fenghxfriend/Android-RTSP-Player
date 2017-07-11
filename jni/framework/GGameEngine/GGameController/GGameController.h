#ifndef GGameController_h
#define GGameController_h

#include "../NetWork/Session.h"
#include "../GGameCommand/GGameCMDManager.h"
#include "../CommonInfo/CommonStructDef.h"
#include "../CommonInfo/NewCommonStructDef.h"

class ClientLogic;
class SearchTankCommand;
class JionTankServerCommand;
class DisconnectTankCommand;
class AttackCommand;
class AimCommand;
class MoveCommand;
class PushPullCommand;
class RotationCommand;

class ChangeTankNameCommand;
class ChangeTankPasswordCommand;

class FreeTankMsg;
class TankBeAttackedMsg;
class BeAimMsg;

class TankCanBeConnect;
class TankInitInfo;

//--------------------------------------------------------------------
//
//add by jinli 2012-03-01
//
class CommandOfUpdateModelConfiger;
class MessageOfModelReciveUpdateConfigerData;
class CommandOfExecUpdateModelConfiger;
class CommandOfModelControlForword;
class CommandOfModelControlBack;
class CommandOfModelControlStop;
class CommandOfModelControlStartTurnLeft;
class CommandOfModelControlStopTurnLeft;
class CommandOfModelControlStartTurnRight;
class CommandOfModelControlStopTurnRight;
class CommandOfModelControlCameraMoveUp;
class CommandOfModelControlCameraMoveDown;
class CommandOfModelControlCameraStop;
class CommandOfModelControlStartLeftLED;
class CommandOfModelControlStopLeftLED;
class CommandOfModelControlStartRightLED;
class CommandOfModelControlStopRightLED;
class CommandOfModelControlStartBehindLED;
class CommandOfModelControlStopBehindLED;
class CommandOfModelControlOpenFrontLight;
class CommandOfModelControlCloseFrontLight;
class CommandOfModelControlOpenSpeaker;
class CommandOfModelControlCloseSpeaker;
class MessageOfModelInformationBatteryPower;

//
//add by jinli 2012-03-26
//
class MessageOfOpenPDASoundToModelResponse;
class MessageOfClosePDASoundToModelResponse;
class MessageOfOpenModelSoundToPDAResponse;
class MessageOfCloseModelSoundToPDAResponse;
class MessageOfOpenStreamMusicResponse;
class MessageOfCloseStreamMusicResponse;
class CommandOfModelControlOpenPDASoundToModel;
class CommandOfModelControlClosePDASoundToModel;
class CommandOfModelControlOpenModelSoundToPDA;
class CommandOfModelControlCloseModelSoundToPDA;
class CommandOfModelControlOpenStreamMusic;
class CommandOfModelControlCloseStreamMusic;

//
//add by jinli 2012-03-29
//
class MessageOfSaveSpeakerMusicPCMDataResponse;
class CommandOfSpeakerMusicPCMData;

//
//add by jinli 2012-04-14
//
class ClientDisconnectMsg;
class MessageOfOpenSpeakerResponse;
class MessageOfCloseSpeakerResponse;

//
//add by jinli 2012-04-18
//
class CommandOfPingPong;

//
//add by jinli 2012-04-18
//
class CommandOfModelControlPauseStreamMusic;

//
//add by jinli 2012-05-18
//
class CommandOfPWMController;
class CommandOfLEDController;

//
//add by jinli 2012-06-06
//
class CommandOfClearProductTester;
class MessageOfClearProductTesterResponse;
class CommandOfClearProductTesterRestart;

//
//add by jinli 2012-11-05
//
class CommandOfNeedModelAdvanceInfo;
class MessageOfModelAdvanceInfo;
class CommandOf5SM3nForward;
class CommandOf5SM3nBackward;
class CommandOf5SM3nLeftTurn;
class CommandOf5SM3nRightTurn;
class CommandOf5SM3nStopMove;
class CommandOf5SM3nStopTurn;

//
//add by jinli 2012-12-18
//
class CommandOf5HT6Forward;
class CommandOf5HT6Backward;
class CommandOf5HT6TurnLeft;
class CommandOf5HT6TurnRight;
class CommandOf5HT6FlyLeft;
class CommandOf5HT6FlyRight;
class CommandOf5HT6GoUp;
class CommandOf5HT6GoDown;
class CommandOf5HT6CMCommonUse;
class CommandOf5HT6IPAddress;
class CommandOf5HT6StopMove;
class CommandOf5HT6StopSelfTurn;
class CommandOf5HT6StopFlyTurn;
class CommandOf5HT6StopUpDown;
class CommandOf5RM6nForward;
class CommandOf5RM6nBackward;
class CommandOf5RM6nStopMove;
class CommandOf5RM6nTurnLeft;
class CommandOf5RM6nTurnRight;
class CommandOf5RM6nStopTurn;
class CommandOf5RM6nWaistTurnLeft;
class CommandOf5RM6nWaistTurnRight;
class CommandOf5RM6nWaistStopTurn;
class CommandOf5RM6nLeftHandAttack;
class CommandOf5RM6nRightHandAttack;
class CommandOf5RM6nLeftGunFire;
class CommandOf5RM6nRightGunFire;
class CommandOf5RM6nStartBattle;
class CommandOf5RM6nFinishedBattle;
class CommandOf5SM3nSBeginBattle;
class CommandOf5SM3nSFinishBattle;
class CommandOf5SM3nSFire;
class MessageOf5HT6GroundDistance;
class MessageOf5HT6MCCommonUse;
class MessageOf5RM6nBeAttackedByGun;
class MessageOf5RM6nHeadBeAttacked;
class MessageOf5RM6nWaistAngle;
class MessageOf5RM6nReportGunStatus;
class MessageOf5SM3nSEnemyAttack;

////////////////////////////////////////////////// GWG //////////////////////////////////////////////////
class CommandOf5SM3nSGWGKickPlayer;
class CommandOf5SM3nSGWGDisbandGame;
class CommandOf5SM3nSGWGLeaveGame;
class CommandOf5SM3nSGWGStartGame;
class CommandOf5SM3nSGWGSearchGameServer;
class CommandOf5SM3nSGWGStopSearchGameServer;
class CommandOf5SM3nSGWGJoinSearchedGameServer;
class CommandOf5SM3nSGWGRequestGameServerTeamInfo;
class CommandOf5SM3nSGWGCreateGame;
class CommandOf5SM3nSGWGLeaveGameWhileBattling;
class MessageOf5SM3nSGWGKickPlayerResponse;
class MessageOf5SM3nSGWGDisbandGameResponse;
class MessageOf5SM3nSGWGLeaveGameResponse;
class MessageOf5SM3nSGWGStartGameResponse;
class MessageOf5SM3nSGWGSomeBodyJoinGameWhileWaiting;
class MessageOf5SM3nSGWGSomeBodyLeaveGameWhileWaiting;
class MessageOf5SM3nSGWGSearchGameServerResponse;
class MessageOf5SM3nSGWGStopSearchGameServerResponse;
class MessageOf5SM3nSGWGSearchedGameServerInfo;
class MessageOf5SM3nSGWGFinishedSearchGameServer;
class MessageOf5SM3nSGWGJoinSearchedGameServerResponse;
class MessageOf5SM3nSGWGGameServerTeamInfo;
class MessageOf5SM3nSGWGCreateGameResponse;
class MessageOf5SM3nSGWGHostStartGame;
class MessageOf5SM3nSGWGHostDisbandGame;
class MessageOf5SM3nSGWGLeaveGameWhileBattlingResponse;
class MessageOf5SM3nSGWGGameServerMessage;
class MessageOf5SM3nSGWGGameOver;
class MessageOf5SM3nSGWGSomeBodyLeaveGameWhileBattling;
class MessageOf5SM3nSGWGBeKickedoutByHost;
class CommandOf5SM3nSGWGRequestTeamIDList;
class MessageOf5SM3nSGWGTeamMemberIDList;

class GGameController
{
public:
	DeclareMsgHandler();
	DeclareCommandHandler();
    
    static GGameController& sharedInstance();
	GGameController(ClientLogic* clientLogic);
    GGameController();
	virtual ~GGameController();
    
    ClientSession *session();    
    
    void Init();
    void UnInit();

	void OnSearchTankCommand(SearchTankCommand& command);
    void OnJionTankCommand(JionTankServerCommand& command);
    void OnDisconnectTankCommand(DisconnectTankCommand& command);
    
    void OnAttackCommand(AttackCommand& command);
    void OnAimCommand(AimCommand& command);
    void OnMoveCommand(MoveCommand& command);
    void OnPushPullCommand(PushPullCommand& command);
    void OnRotationCommand(RotationCommand& command);
    
    void OnFreeTankMsg( FreeTankMsg& info );
    void OnTankCanBeConnect(TankCanBeConnect& info);
    void OnTankInitInfo(TankInitInfo& info);
    
    void OnBeAttacked(TankBeAttackedMsg& info);
    void OnBeAimMsg(BeAimMsg& info);
    
    void OnChangeTankNameCommand(ChangeTankNameCommand& info);
    void OnChangeTankPasswordCommand(ChangeTankPasswordCommand& info);
    
    void NotifacationGameStart(int playerID, int roomID);
    void NotifacationGameEnd();
    
    //--------------------------------------------------------------------
    //
    //add by jinli 2012-03-01
    //
    void OnUpdateModelConfigerCommand(CommandOfUpdateModelConfiger& info);
    void OnModelReciveUpdateConfigerData(MessageOfModelReciveUpdateConfigerData& info);
    void OnExecUpdateModelConfigerCommand(CommandOfExecUpdateModelConfiger& info);
    void OnModelControlForwordCommand(CommandOfModelControlForword& info);
    void OnModelControlBackCommand(CommandOfModelControlBack& info);
    void OnModelControlStopCommand(CommandOfModelControlStop& info);
    void OnModelControlStartTurnLeftCommand(CommandOfModelControlStartTurnLeft& info);
    void OnModelControlStopTurnLeftCommand(CommandOfModelControlStopTurnLeft& info);
    void OnModelControlStartTurnRightCommand(CommandOfModelControlStartTurnRight& info);
    void OnModelControlStopTurnRightCommand(CommandOfModelControlStopTurnRight& info);
    void OnModelControlCameraMoveUpCommand(CommandOfModelControlCameraMoveUp& info);
    void OnModelControlCameraMoveDownCommand(CommandOfModelControlCameraMoveDown& info);
    void OnModelControlCameraStopCommand(CommandOfModelControlCameraStop& info);
    void OnModelControlStartLeftLEDCommand(CommandOfModelControlStartLeftLED& info);
    void OnModelControlStopLeftLEDCommand(CommandOfModelControlStopLeftLED& info);
    void OnModelControlStartRightLEDCommand(CommandOfModelControlStartRightLED& info);
    void OnModelControlStopRightLEDCommand(CommandOfModelControlStopRightLED& info);
    void OnModelControlStartBehindLEDCommand(CommandOfModelControlStartBehindLED& info);
    void OnModelControlStartStopBehindLEDCommand(CommandOfModelControlStopBehindLED& info);
    void OnModelControlStartOpenFrontLightCommand(CommandOfModelControlOpenFrontLight& info);
    void OnModelControlStartCloseFrontLightCommand(CommandOfModelControlCloseFrontLight& info);
    void OnModelControlOpenSpeakerCommand(CommandOfModelControlOpenSpeaker& info);
    void OnModelControlCloseSpeakerCommand(CommandOfModelControlCloseSpeaker& info);
    void OnModelInformationOfBatteryPower(MessageOfModelInformationBatteryPower& info);
    
    //
    //add by jinli 2012-03-26
    //
    void OnOpenPDASoundToModelResponse(MessageOfOpenPDASoundToModelResponse& info);
    void OnClosePDASoundToModelResponse(MessageOfClosePDASoundToModelResponse& info);
    void OnOpenModelSoundToPDAResponse(MessageOfOpenModelSoundToPDAResponse& info);
    void OnCloseModelSoundToPDAResponse(MessageOfCloseModelSoundToPDAResponse& info);
    void OnOpenStreamMusicResponse(MessageOfOpenStreamMusicResponse& info);
    void OnCloseStreamMusicResponse(MessageOfCloseStreamMusicResponse& info);
    void OnOpenPDASoundToModel(CommandOfModelControlOpenPDASoundToModel& info);
    void OnClosePDASoundToModel(CommandOfModelControlClosePDASoundToModel& info);
    void OnOpenModelSoundToPDA(CommandOfModelControlOpenModelSoundToPDA& info);
    void OnCloseModelSoundToPDA(CommandOfModelControlCloseModelSoundToPDA& info);
    void OnOpenStreamMusic(CommandOfModelControlOpenStreamMusic& info);
    void OnCloseStreamMusic(CommandOfModelControlCloseStreamMusic& info);
    
    //
    //add by jinli 2012-03-29
    //
    void OnSaveSpeakerMusicPCMDataResponse(MessageOfSaveSpeakerMusicPCMDataResponse& info);
    void OnUploadSpeakerMusicPCMData(CommandOfSpeakerMusicPCMData& info);
    
    //
    //add by jinli 2012-04-14
    //
    void OnDisConnectWithModel(ClientDisconnectMsg& info);
    void OnOpenSpeakerResponse(MessageOfOpenSpeakerResponse& info);
    void OnCloseSpeakerResponse(MessageOfCloseSpeakerResponse& info);
    
    //
    //add by jinli 2012-04-18
    //
    void OnPingPongCommand(CommandOfPingPong& info);
    
    //
    //add by jinli 2012-04-26
    //
    void OnPauseStreamMusic(CommandOfModelControlPauseStreamMusic& info);
    
    //
    //add by jinli 2012-05-18
    //
    void OnPWMController(CommandOfPWMController& info);
    void OnLEDController(CommandOfLEDController& info);
    
    //
    //add by jinli 2012-06-06
    //
    void OnClearProductTester(CommandOfClearProductTester& info);
    void OnClearProductTesterResponse(MessageOfClearProductTesterResponse& info);
    void OnClearProductTesterRestart(CommandOfClearProductTesterRestart& info);

    //--------------------------------------------------------------------
    
    //
    //add by jinli 2012-09-06
    //
    void netSignalStrengthPowerful();
    void netSignalStrengthWeek();
    //--------------------------------------------------------------------
    
    //
    //add by jinli 2012-11-05
    void OnNeedModelAdvanceInfo(CommandOfNeedModelAdvanceInfo& info);
    void OnModelAdvanceInfo(MessageOfModelAdvanceInfo& info);
    void On5SM3nForward(CommandOf5SM3nForward& info);
    void On5SM3nBackward(CommandOf5SM3nBackward& info);
    void On5SM3nLeftTurn(CommandOf5SM3nLeftTurn& info);
    void On5SM3nRightTurn(CommandOf5SM3nRightTurn& info);
    void On5SM3nRightStopMove(CommandOf5SM3nStopMove& info);
    void On5SM3nRightStopTurn(CommandOf5SM3nStopTurn& info);
    
    //
    //add by jinli 2012-12-18
    //
    void On5HT6Forward(CommandOf5HT6Forward& info);
    void On5HT6Backward(CommandOf5HT6Backward& info);
    void On5HT6TurnLeft(CommandOf5HT6TurnLeft& info);
    void On5HT6TurnRight(CommandOf5HT6TurnRight& info);
    void On5HT6FlyLeft(CommandOf5HT6FlyLeft& info);
    void On5HT6FlyRight(CommandOf5HT6FlyRight& info);
    void On5HT6GoUp(CommandOf5HT6GoUp& info);
    void On5HT6GoDown(CommandOf5HT6GoDown& info);
    void On5HT6CMCommonUse(CommandOf5HT6CMCommonUse& info);
    void On5HT6IPAddress(CommandOf5HT6IPAddress& info);
    void On5HT6StopMove(CommandOf5HT6StopMove& info);
    void On5HT6StopSelfTurn(CommandOf5HT6StopSelfTurn& info);
    void On5HT6StopFlyTurn(CommandOf5HT6StopFlyTurn& info);
    void On5HT6StopUpDown(CommandOf5HT6StopUpDown& info);
    void On5RM6nForward(CommandOf5RM6nForward& info);
    void On5RM6nBackward(CommandOf5RM6nBackward& info);
    void On5RM6nStopMove(CommandOf5RM6nStopMove& info);
    void On5RM6nTurnLeft(CommandOf5RM6nTurnLeft& info);
    void On5RM6nTurnRight(CommandOf5RM6nTurnRight& info);
    void On5RM6nStopTurn(CommandOf5RM6nStopTurn& info);
    void On5RM6nWaistTurnLeft(CommandOf5RM6nWaistTurnLeft& info);
    void On5RM6nWaistTurnRight(CommandOf5RM6nWaistTurnRight& info);
    void On5RM6nWaistStopTurn(CommandOf5RM6nWaistStopTurn& info);
    void On5RM6nLeftHandAttack(CommandOf5RM6nLeftHandAttack& info);
    void On5RM6nRightHandAttack(CommandOf5RM6nRightHandAttack& info);
    void On5RM6nLeftGunFire(CommandOf5RM6nLeftGunFire& info);
    void On5RM6nRightGunFire(CommandOf5RM6nRightGunFire& info);
    void On5RM6nStartBattle(CommandOf5RM6nStartBattle& info);
    void On5RM6nFinishedBattle(CommandOf5RM6nFinishedBattle& info);
    void On5SM3nSBeginBattle(CommandOf5SM3nSBeginBattle& info);
    void On5SM3nSFinishBattle(CommandOf5SM3nSFinishBattle& info);
    void On5SM3nSFire(CommandOf5SM3nSFire& info);
    void On5HT6GroundDistance(MessageOf5HT6GroundDistance& info);
    void On5HT6MCCommonUse(MessageOf5HT6MCCommonUse& info);
    void On5RM6nBeAttackedByGun(MessageOf5RM6nBeAttackedByGun& info);
    void On5RM6nHeadBeAttacked(MessageOf5RM6nHeadBeAttacked& info);
    void On5RM6nWaistAngle(MessageOf5RM6nWaistAngle& info);
    void On5RM6nReportGunStatus(MessageOf5RM6nReportGunStatus& info);
    void On5SM3nSEnemyAttack(MessageOf5SM3nSEnemyAttack& info);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //add by jinli 2013-02-18
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //C->S
    void On5SM3nSGWGKickPlayer                    (CommandOf5SM3nSGWGKickPlayer                     & info);
    void On5SM3nSGWGDisbandGame                   (CommandOf5SM3nSGWGDisbandGame                    & info);
    void On5SM3nSGWGLeaveGame                     (CommandOf5SM3nSGWGLeaveGame                      & info);
    void On5SM3nSGWGStartGame                     (CommandOf5SM3nSGWGStartGame                      & info);
    void On5SM3nSGWGSearchGameServer              (CommandOf5SM3nSGWGSearchGameServer               & info);
    void On5SM3nSGWGStopSearchGameServer          (CommandOf5SM3nSGWGStopSearchGameServer           & info);
    void On5SM3nSGWGJoinSearchedGameServer        (CommandOf5SM3nSGWGJoinSearchedGameServer         & info);
    void On5SM3nSGWGRequestGameServerTeamInfo     (CommandOf5SM3nSGWGRequestGameServerTeamInfo      & info);
    void On5SM3nSGWGLeaveGameWhileBattling        (CommandOf5SM3nSGWGLeaveGameWhileBattling         & info);
    void On5SM3nSGWGCreateGame                    (CommandOf5SM3nSGWGCreateGame                     & info);
    //S->C
    void On5SM3nSGWGKickPlayerResponse            (MessageOf5SM3nSGWGKickPlayerResponse             & info);
    void On5SM3nSGWGDisbandGameResponse           (MessageOf5SM3nSGWGDisbandGameResponse            & info);
    void On5SM3nSGWGLeaveGameResponse             (MessageOf5SM3nSGWGLeaveGameResponse              & info);
    void On5SM3nSGWGStartGameResponse             (MessageOf5SM3nSGWGStartGameResponse              & info);
    void On5SM3nSGWGSomeBodyJoinGame              (MessageOf5SM3nSGWGSomeBodyJoinGameWhileWaiting   & info);
    void On5SM3nSGWGSomeBodyLeaveGame             (MessageOf5SM3nSGWGSomeBodyLeaveGameWhileWaiting  & info);
    void On5SM3nSGWGSearchGameServerResponse      (MessageOf5SM3nSGWGSearchGameServerResponse       & info);
    void On5SM3nSGWGStopSearchGameServerResponse  (MessageOf5SM3nSGWGStopSearchGameServerResponse   & info);
    void On5SM3nSGWGSearchedGameServerInfo        (MessageOf5SM3nSGWGSearchedGameServerInfo         & info);
    void On5SM3nSGWGFinishedSearchGameServer      (MessageOf5SM3nSGWGFinishedSearchGameServer       & info);
    void On5SM3nSGWGJoinSearchedGameServerResponse(MessageOf5SM3nSGWGJoinSearchedGameServerResponse & info);
    void On5SM3nSGWGGameServerTeamInfo            (MessageOf5SM3nSGWGGameServerTeamInfo             & info);
    void On5SM3nSGWGCreateGameResponse            (MessageOf5SM3nSGWGCreateGameResponse             & info);
    void On5SM3nSGWGHostStartGame                 (MessageOf5SM3nSGWGHostStartGame                  & info);
    void On5SM3nSGWGHostDisbandGame               (MessageOf5SM3nSGWGHostDisbandGame                & info);
    void On5SM3nSGWGLeaveGameWhileBattlingResponse(MessageOf5SM3nSGWGLeaveGameWhileBattlingResponse & info);
    void On5SM3nSGWGGameServerMessage             (MessageOf5SM3nSGWGGameServerMessage              & info);
    void On5SM3nSGWGGameOver                      (MessageOf5SM3nSGWGGameOver                       & info);
    void On5SM3nSGWGSomebodyLeaveWhileBattling    (MessageOf5SM3nSGWGSomeBodyLeaveGameWhileBattling & info);
    void OnBeKickedoutByHost                      (MessageOf5SM3nSGWGBeKickedoutByHost              & info);
    void OnGWGRequestTeamMemberIDList             (CommandOf5SM3nSGWGRequestTeamIDList              & info);
    void OnGWGTeamMemberIDList                    (MessageOf5SM3nSGWGTeamMemberIDList               & info);    
    
private:
    
    ClientSession* m_TankControlSession;
    typedef std::vector<TankServerInfo> TanksInfo;
    TanksInfo m_TanksInfo;
	
    ClientLogic* mLogic;
};

#endif
