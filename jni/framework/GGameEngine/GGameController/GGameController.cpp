#include "GGameController.h"
#include "../NetWork/Session.h"
#include "../GGameCommand/CommandDef.h"
#include "../CommonInfo/MsgDef.h"
#include "../GGameClient/GGameClient.h"
#include "../../../source/GAudioManager/GAudioManager.h"

ImplMsgHandlerBegin(GGameController)
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, FreeTankMsg                                      , OnFreeTankMsg                             )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, TankCanBeConnect                                 , OnTankCanBeConnect                        )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, TankInitInfo                                     , OnTankInitInfo                            )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, TankBeAttackedMsg                                , OnBeAttacked                              )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, BeAimMsg                                         , OnBeAimMsg                                )

//
//add by jinli 2012-03-01
//
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfModelReciveUpdateConfigerData           , OnModelReciveUpdateConfigerData           )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfModelInformationBatteryPower            , OnModelInformationOfBatteryPower          )

//
//add by jinli 2012-03-26
//
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfOpenPDASoundToModelResponse             , OnOpenPDASoundToModelResponse             )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfClosePDASoundToModelResponse            , OnClosePDASoundToModelResponse            )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfOpenModelSoundToPDAResponse             , OnOpenModelSoundToPDAResponse             )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfCloseModelSoundToPDAResponse            , OnCloseModelSoundToPDAResponse            )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfOpenStreamMusicResponse                 , OnOpenStreamMusicResponse                 )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfCloseStreamMusicResponse                , OnCloseStreamMusicResponse                )

//
//add by jinli 2012-03-29
//
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfSaveSpeakerMusicPCMDataResponse         , OnSaveSpeakerMusicPCMDataResponse         )

//
//add by jinli 2012-04-14
//
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, ClientDisconnectMsg                              , OnDisConnectWithModel                     )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfOpenSpeakerResponse                     , OnOpenSpeakerResponse                     )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfCloseSpeakerResponse                    , OnCloseSpeakerResponse                    )

//
//add by jinli 2012-06-06
//
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfClearProductTesterResponse              , OnClearProductTesterResponse              )

//
//add by jinli 2012-11-05
//
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOfModelAdvanceInfo                        , OnModelAdvanceInfo                        )

//
//add by jinli 2012-12-18
//
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5HT6GroundDistance                      , On5HT6GroundDistance                      )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5HT6MCCommonUse                         , On5HT6MCCommonUse                         )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5RM6nBeAttackedByGun                    , On5RM6nBeAttackedByGun                    )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5RM6nHeadBeAttacked                     , On5RM6nHeadBeAttacked                     )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5RM6nWaistAngle                         , On5RM6nWaistAngle                         )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5RM6nReportGunStatus                    , On5RM6nReportGunStatus                    )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSEnemyAttack                       , On5SM3nSEnemyAttack                       )



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//add by jinli 2013-02-18   GWG
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGKickPlayerResponse             , On5SM3nSGWGKickPlayerResponse             )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGDisbandGameResponse            , On5SM3nSGWGDisbandGameResponse            )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGLeaveGameResponse              , On5SM3nSGWGLeaveGameResponse              )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGStartGameResponse              , On5SM3nSGWGStartGameResponse              )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGSomeBodyJoinGameWhileWaiting   , On5SM3nSGWGSomeBodyJoinGame               )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGSomeBodyLeaveGameWhileWaiting  , On5SM3nSGWGSomeBodyLeaveGame              )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGSearchGameServerResponse       , On5SM3nSGWGSearchGameServerResponse       )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGStopSearchGameServerResponse   , On5SM3nSGWGStopSearchGameServerResponse   )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGSearchedGameServerInfo         , On5SM3nSGWGSearchedGameServerInfo         )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGFinishedSearchGameServer       , On5SM3nSGWGFinishedSearchGameServer       )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGJoinSearchedGameServerResponse , On5SM3nSGWGJoinSearchedGameServerResponse )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGGameServerTeamInfo             , On5SM3nSGWGGameServerTeamInfo             )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGCreateGameResponse             , On5SM3nSGWGCreateGameResponse             )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGHostStartGame                  , On5SM3nSGWGHostStartGame                  )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGHostDisbandGame                , On5SM3nSGWGHostDisbandGame                ) 
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGLeaveGameWhileBattlingResponse , On5SM3nSGWGLeaveGameWhileBattlingResponse )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGGameServerMessage              , On5SM3nSGWGGameServerMessage              ) 
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGGameOver                       , On5SM3nSGWGGameOver                       )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGSomeBodyLeaveGameWhileBattling , On5SM3nSGWGSomebodyLeaveWhileBattling     )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGBeKickedoutByHost              , OnBeKickedoutByHost                       )
    DefineMsgHandler(MsgHandleLevel::MHL_STANDARD, MessageOf5SM3nSGWGTeamMemberIDList               , OnGWGTeamMemberIDList                     )

ImplMsgHandlerEnd(GGameController)

ImplCommandHandlerBegin(GGameController)
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, SearchTankCommand                            , OnSearchTankCommand                       )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, JionTankServerCommand                        , OnJionTankCommand                         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, DisconnectTankCommand                        , OnDisconnectTankCommand                   )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, AttackCommand                                , OnAttackCommand                           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, AimCommand                                   , OnAimCommand                              )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, MoveCommand                                  , OnMoveCommand                             )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, RotationCommand                              , OnRotationCommand                         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, PushPullCommand                              , OnPushPullCommand                         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, ChangeTankNameCommand                        , OnChangeTankNameCommand                   )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, ChangeTankPasswordCommand                    , OnChangeTankPasswordCommand               )

//
//add by jinli 2012-03-01
//
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfUpdateModelConfiger                 , OnUpdateModelConfigerCommand              )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfExecUpdateModelConfiger             , OnExecUpdateModelConfigerCommand          )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlForword                 , OnModelControlForwordCommand              )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlBack                    , OnModelControlBackCommand                 )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStop                    , OnModelControlStopCommand                 )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStartTurnLeft           , OnModelControlStartTurnLeftCommand        )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStopTurnLeft            , OnModelControlStopTurnLeftCommand         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStartTurnRight          , OnModelControlStartTurnRightCommand       )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStopTurnRight           , OnModelControlStopTurnRightCommand        )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlCameraMoveUp            , OnModelControlCameraMoveUpCommand         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlCameraMoveDown          , OnModelControlCameraMoveDownCommand       )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlCameraStop              , OnModelControlCameraStopCommand           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStartLeftLED            , OnModelControlStartLeftLEDCommand         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStopLeftLED             , OnModelControlStopLeftLEDCommand          )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStartRightLED           , OnModelControlStartRightLEDCommand        )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStopRightLED            , OnModelControlStopRightLEDCommand         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStartBehindLED          , OnModelControlStartBehindLEDCommand       )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlStopBehindLED           , OnModelControlStartStopBehindLEDCommand   )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlOpenFrontLight          , OnModelControlStartOpenFrontLightCommand  )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlCloseFrontLight         , OnModelControlStartCloseFrontLightCommand )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlOpenSpeaker             , OnModelControlOpenSpeakerCommand          )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlCloseSpeaker            , OnModelControlCloseSpeakerCommand         )

//
//add by jinli 2012-03-26
//
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlOpenPDASoundToModel     , OnOpenPDASoundToModel                     )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlClosePDASoundToModel    , OnClosePDASoundToModel                    )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlOpenModelSoundToPDA     , OnOpenModelSoundToPDA                     )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlCloseModelSoundToPDA    , OnCloseModelSoundToPDA                    )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlOpenStreamMusic         , OnOpenStreamMusic                         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlCloseStreamMusic        , OnCloseStreamMusic                        )

//
//add by jinli 2012-03-29
//
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfSpeakerMusicPCMData                 , OnUploadSpeakerMusicPCMData               )

//
//add by jinli 2012-04-18
//
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfPingPong                            , OnPingPongCommand                         )

//
//add by jinli 2012-04-26
//
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfModelControlPauseStreamMusic        , OnPauseStreamMusic                        )

//
//add by jinli 2012-05-18
//
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfPWMController                       , OnPWMController                           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfLEDController                       , OnLEDController                           )

//
//add by jinli 2012-06-06
//
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfClearProductTester                  , OnClearProductTester                      )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfClearProductTesterRestart           , OnClearProductTesterRestart               )

//
//add by jinli 2012-11-05
//
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOfNeedModelAdvanceInfo                , OnNeedModelAdvanceInfo                    )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nForward                        , On5SM3nForward                            )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nBackward                       , On5SM3nBackward                           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nLeftTurn                       , On5SM3nLeftTurn                           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nRightTurn                      , On5SM3nRightTurn                          )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nStopMove                       , On5SM3nRightStopMove                      )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nStopTurn                       , On5SM3nRightStopTurn                      )

//
//add by jinli 2012-12-18
//
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6Forward                         , On5HT6Forward                             )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6Backward                        , On5HT6Backward                            )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6TurnLeft                        , On5HT6TurnLeft                            )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6TurnRight                       , On5HT6TurnRight                           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6FlyLeft                         , On5HT6FlyLeft                             )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6FlyRight                        , On5HT6FlyRight                            )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6GoUp                            , On5HT6GoUp                                )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6GoDown                          , On5HT6GoDown                              )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6CMCommonUse                     , On5HT6CMCommonUse                         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6IPAddress                       , On5HT6IPAddress                           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6StopMove                        , On5HT6StopMove                            )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6StopSelfTurn                    , On5HT6StopSelfTurn                        )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6StopFlyTurn                     , On5HT6StopFlyTurn                         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5HT6StopUpDown                      , On5HT6StopUpDown                          )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nForward                        , On5RM6nForward                            )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nBackward                       , On5RM6nBackward                           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nStopMove                       , On5RM6nStopMove                           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nTurnLeft                       , On5RM6nTurnLeft                           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nTurnRight                      , On5RM6nTurnRight                          )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nStopTurn                       , On5RM6nStopTurn                           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nWaistTurnLeft                  , On5RM6nWaistTurnLeft                      )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nWaistTurnRight                 , On5RM6nWaistTurnRight                     )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nWaistStopTurn                  , On5RM6nWaistStopTurn                      )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nLeftHandAttack                 , On5RM6nLeftHandAttack                     )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nRightHandAttack                , On5RM6nRightHandAttack                    )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nLeftGunFire                    , On5RM6nLeftGunFire                        )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nRightGunFire                   , On5RM6nRightGunFire                       )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nStartBattle                    , On5RM6nStartBattle                        )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5RM6nFinishedBattle                 , On5RM6nFinishedBattle                     )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSBeginBattle                   , On5SM3nSBeginBattle                       )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSFinishBattle                  , On5SM3nSFinishBattle                      )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSFire                          , On5SM3nSFire                              )

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//add by jinli 2013-02-18   GWG
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGKickPlayer                 , On5SM3nSGWGKickPlayer                     )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGDisbandGame                , On5SM3nSGWGDisbandGame                    )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGLeaveGame                  , On5SM3nSGWGLeaveGame                      )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGStartGame                  , On5SM3nSGWGStartGame                      )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGSearchGameServer           , On5SM3nSGWGSearchGameServer               )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGStopSearchGameServer       , On5SM3nSGWGStopSearchGameServer           )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGJoinSearchedGameServer     , On5SM3nSGWGJoinSearchedGameServer         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGRequestGameServerTeamInfo  , On5SM3nSGWGRequestGameServerTeamInfo      )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGLeaveGameWhileBattling     , On5SM3nSGWGLeaveGameWhileBattling         )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGCreateGame                 , On5SM3nSGWGCreateGame                     )
    DefineCommandHandler(MsgHandleLevel::MHL_STANDARD, CommandOf5SM3nSGWGRequestTeamIDList          , OnGWGRequestTeamMemberIDList              )

ImplCommandHandlerEnd(GGameController)

//
//add by jinli 2012-09-06
//
void netSignalStrengthPowerfulCallBack()
{
    GGameController::sharedInstance().netSignalStrengthPowerful();
}

void netSignalStrengthWeekCallBack()
{
    GGameController::sharedInstance().netSignalStrengthWeek();
}

void GGameController::netSignalStrengthPowerful()
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_NetSignalPowerful, 0, 0);
}

void GGameController::netSignalStrengthWeek()
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_NetSignalWeek, 0, 0);
}
//--------------------------------------------------------------------

GGameController::GGameController(ClientLogic* clientLogic)
{
    mLogic = clientLogic;
}

GGameController::~GGameController()
{
    UnInit();
}

void GGameController::Init()
{
    m_TankControlSession = new ClientSession;
    m_TankControlSession->StartServer(8200, 1, 20, netSignalStrengthPowerfulCallBack, netSignalStrengthWeekCallBack);
    
    RegMsgHandler(m_TankControlSession);
    RegCommandHandler( &( GGameCMDManager::sharedInstance() ) );
}

void GGameController::OnSearchTankCommand(SearchTankCommand& command)
{
    m_TanksInfo.clear();
	SearchTankServer msg;
    NETADDR pAddr;
    pAddr.type = NETTYPE_IPV4 | NETTYPE_LINK_BROADCAST;
    pAddr.port = 8200;
	m_TankControlSession->SendConnlessMsg(msg, &pAddr);
    pAddr.type = NETTYPE_IPV4 | NETTYPE_LINK_BROADCAST;
    pAddr.port = 8201;
	m_TankControlSession->SendConnlessMsg(msg, &pAddr);
}

void GGameController::OnJionTankCommand(JionTankServerCommand& command)
{
    NETADDR _address;
    _address.type = NETTYPE_IPV4;
    net_addr_from_str(&_address,command.ip);
    _address.port = 8200;    
    m_TankControlSession->Connect(&_address);
}
void GGameController::OnDisconnectTankCommand(DisconnectTankCommand& command)
{
    if (!command.isCustom) 
    {
        m_TankControlSession->Disconnect("");
    }
    else
    {
        MessageOfCustomDisconnect msg;
        str_copy(msg.reason, command.reason, 32);
        msg.remark = command.remark;
        m_TankControlSession->SendMsg(msg);
    }
}

void GGameController::OnAttackCommand(AttackCommand& command)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::BATTLE_GAME_STATE) {
//        AttackMsg msg;
//        msg.type = command.type;
//        msg.ledParameter = command.ledParameter;
//        m_TankControlSession->SendMsg(msg);
//    }
}

void GGameController::OnAimCommand(AimCommand& command)
{
    AimHandleMsg msg;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::OnMoveCommand(MoveCommand& command)
{
    MoveMsg msg;
    msg.direction = command.state;
    msg.speed = command.speed;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::OnPushPullCommand(PushPullCommand& command)
{
    PushPullMsg msg;
    msg.state = command.state;
    msg.speed = command.speed;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::OnRotationCommand(RotationCommand& command)
{
    RationMsg msg;
    msg.state = command.state;
    msg.speed = command.speed;
    m_TankControlSession->SendMsg(msg);
}


void GGameController::OnFreeTankMsg( FreeTankMsg& info )
{
    info.info.address = *(info.GetHost()->GetNetAddr());
    m_TanksInfo.push_back(info.info);
    
    int msgID = UI_Control_MsgID_ServerInfo;
    GGameClient::sharedInstance().swallowMessage(msgID,&info.info, sizeof(TankServerInfo));
}

void GGameController::OnTankCanBeConnect(TankCanBeConnect& info)
{
    if (info.reason == 0) {
        m_TankControlSession->Connect(info.GetHost()->GetNetAddr());
    }
}

void GGameController::OnTankInitInfo(TankInitInfo& info)
{
    int msgID = UI_Control_MsgID_Tank_Init_Successful;
    GGameClient::sharedInstance().swallowMessage(msgID, &info.info, sizeof(TankBaseInfo));
}

void GGameController::OnBeAttacked(TankBeAttackedMsg& info)
{
//    mLogic->m_GameLogicSession->SendMsg(info);
}

void GGameController::OnBeAimMsg(BeAimMsg& info)
{
//    mLogic->m_GameLogicSession->SendMsg(info);
}

void GGameController::NotifacationGameStart(int playerID, int roomID)
{
    ToTankGameStartMsg msg;
    msg.playerID = playerID;
    msg.roomID = roomID;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::NotifacationGameEnd()
{
    ToTankGameEndMsg msg;
    m_TankControlSession->SendMsg(msg);
}


void GGameController::OnChangeTankNameCommand(ChangeTankNameCommand& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        ChangeTankName msg;
        str_copy(msg.name, info.name, 32);
        m_TankControlSession->SendMsg(msg);
    }
}

void GGameController::OnChangeTankPasswordCommand(ChangeTankPasswordCommand& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        ChangePassword msg;
        str_copy(msg.password, info.password, 32);
        m_TankControlSession->SendMsg(msg);
    }
}

//TankServerInfo* TankControlSystem::GetTankInfo(unsigned int tank_id)
//{
//    if (tank_id < m_TanksInfo.size()) {
//        return &m_TanksInfo[tank_id];
//    }
//    return NULL;
//}

//int TankControlSystem::GetTankCount()
//{
//    int i = m_TanksInfo.size();
//    return i;
//}


//
//add by jinli 2012-03-01
//
void GGameController::OnUpdateModelConfigerCommand(CommandOfUpdateModelConfiger& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {

        MessageOfUpdateModelConfiger msg;
        str_copy(msg.modelName, info.modelName, 100);
        str_copy(msg.modelPassword, info.modelPassword, 100);
        str_copy(msg.wifiCountry, info.wifiCountry, 200);
        msg.wifiChannel = info.wifiChannel;
        msg.passwordIsNull = info.passwordIsNull;
        msg.updateFlagOfmodelName = info.updateFlagOfmodelName;
        msg.updateFlagOfmodelPassword = info.updateFlagOfmodelPassword;
        msg.updateFlagOfwifiChannel = info.updateFlagOfwifiChannel; 
        msg.isUpdateWifiCountry = info.isUpdateWifiCountry; 
        msg.speakerVolume = info.speakerVolume;
        msg.modelOf5BM2SteerCalibration = info.modelOf5BM2SteerCalibration;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelReciveUpdateConfigerData(MessageOfModelReciveUpdateConfigerData& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_ModelReciveUpdateConfigerData, &info.flag, sizeof(int));
}
void GGameController::OnExecUpdateModelConfigerCommand(CommandOfExecUpdateModelConfiger& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfExecUpdateModelConfiger msg;
        msg.flag = info.flag;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlForwordCommand(CommandOfModelControlForword& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerForword msg;
        msg.ratio = info.ratio;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlBackCommand(CommandOfModelControlBack& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerBack msg;
        msg.ratio = info.ratio;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStopCommand(CommandOfModelControlStop& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStop msg;  
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStartTurnLeftCommand(CommandOfModelControlStartTurnLeft& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStartTurnLeft msg;
        msg.ratio = info.ratio;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStopTurnLeftCommand(CommandOfModelControlStopTurnLeft& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStopTurnLeft msg;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStartTurnRightCommand(CommandOfModelControlStartTurnRight& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStartTurnRight msg;
        msg.ratio = info.ratio;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStopTurnRightCommand(CommandOfModelControlStopTurnRight& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStopTurnRight msg;   
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlCameraMoveUpCommand(CommandOfModelControlCameraMoveUp& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerCameraMoveUp msg;   
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlCameraMoveDownCommand(CommandOfModelControlCameraMoveDown& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerCameraMoveDown msg;   
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlCameraStopCommand(CommandOfModelControlCameraStop& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerCameraStop msg;   
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStartLeftLEDCommand(CommandOfModelControlStartLeftLED& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStartLeftLED msg;  
        msg.onDuration = info.onDuration;
        msg.offDuration = info.offDuration;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStopLeftLEDCommand(CommandOfModelControlStopLeftLED& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStopLeftLED msg;  
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStartRightLEDCommand(CommandOfModelControlStartRightLED& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStartRightLED msg;  
        msg.onDuration = info.onDuration;
        msg.offDuration = info.offDuration;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStopRightLEDCommand(CommandOfModelControlStopRightLED& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStopRightLED msg;  
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStartBehindLEDCommand(CommandOfModelControlStartBehindLED& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStartBehindLED msg;  
        msg.onDuration = info.onDuration;
        msg.offDuration = info.offDuration;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStartStopBehindLEDCommand(CommandOfModelControlStopBehindLED& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerStopBehindLED msg;  
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStartOpenFrontLightCommand(CommandOfModelControlOpenFrontLight& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerOpenFrontLight msg;  
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlStartCloseFrontLightCommand(CommandOfModelControlCloseFrontLight& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerCloseFrontLight msg;  
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlOpenSpeakerCommand(CommandOfModelControlOpenSpeaker& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerOpenSpeaker msg;
        msg.isDefault = info.isDefault;
        str_copy(msg.filename, info.filename, 100);
        msg.remark = info.remark;        
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelControlCloseSpeakerCommand(CommandOfModelControlCloseSpeaker& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfModelControllerCloseSpeaker msg;
        msg.remark = info.remark;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnModelInformationOfBatteryPower(MessageOfModelInformationBatteryPower& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_ModelInformation_BatteryPower, &info.power, sizeof(&info.power));
}

//
//add by jinli 2012-03-01
//
void GGameController::OnOpenPDASoundToModelResponse(MessageOfOpenPDASoundToModelResponse& info)
{
	//开始录制 ** 之所以写在这边，为了保证录制的开关是在同一个线程里进行
	GAudioManager::sharedInstance().startRecordStream();

	//通知上层
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_OpenPDASoundToModelResponse, &info.remark, sizeof(&info.remark));
}
void GGameController::OnClosePDASoundToModelResponse(MessageOfClosePDASoundToModelResponse& info)
{
	//停止录制
	GAudioManager::sharedInstance().stopRecordStream();

	//通知上层
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_ClosePDASoundToModelResponse, &info.remark, sizeof(&info.remark));
}
void GGameController::OnOpenModelSoundToPDAResponse(MessageOfOpenModelSoundToPDAResponse& info)
{
//    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_OpenModelSoundToPDAResponse, &info.remark, sizeof(&info.remark));
}
void GGameController::OnCloseModelSoundToPDAResponse(MessageOfCloseModelSoundToPDAResponse& info)
{
//    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_CloseModelSoundToPDAResponse, &info.remark, sizeof(&info.remark));
}

void GGameController::OnOpenStreamMusicResponse(MessageOfOpenStreamMusicResponse& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_OpenStreamMusicResponse, &info.remark, sizeof(&info.remark));
}
void GGameController::OnCloseStreamMusicResponse(MessageOfCloseStreamMusicResponse& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_CloseStreamMusicResponse, &info.remark, sizeof(&info.remark));
}

void GGameController::OnOpenPDASoundToModel(CommandOfModelControlOpenPDASoundToModel& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfOpenPDASoundToModel msg;
        msg.remark = info.remark;
        msg.cacheTimes = info.cacheTimes;
        memcpy(&msg.PDAAddress, &info.PDAAddress, sizeof(NETADDR));

    	m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnClosePDASoundToModel(CommandOfModelControlClosePDASoundToModel& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfClosePDASoundToModel msg;
        msg.remark = info.remark;

        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnOpenModelSoundToPDA(CommandOfModelControlOpenModelSoundToPDA& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfOpenModelSoundToPDA msg;
        msg.remark = info.remark;
        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnCloseModelSoundToPDA(CommandOfModelControlCloseModelSoundToPDA& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfCloseModelSoundToPDA msg;
        msg.remark = info.remark;
        m_TankControlSession->SendMsg(msg);
    }
}

void GGameController::OnOpenStreamMusic(CommandOfModelControlOpenStreamMusic& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfOpenStreamMusic msg;
        msg.remark = info.remark;
        str_copy(msg.fileName, info.fileName, 64);
        memcpy(&msg.NetAddress, &info.NetAddress, sizeof(NETADDR));
        msg.cacheTimes = info.cacheTimes;
        msg.seconds = info.seconds;
        msg.microSeconds = info.microSeconds;
        memcpy(&msg.timeDistance, &info.timeDistance, sizeof(struct timeval));

        m_TankControlSession->SendMsg(msg);
    }
}
void GGameController::OnCloseStreamMusic(CommandOfModelControlCloseStreamMusic& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfCloseStreamMusic msg;
        msg.remark = info.remark;
        m_TankControlSession->SendMsg(msg);
    }
}

//
//add by jinli 2012-03-29
//
void GGameController::OnSaveSpeakerMusicPCMDataResponse(MessageOfSaveSpeakerMusicPCMDataResponse& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_SaveSpeakerMusicPCMDataResponse, &info.isSuccess, sizeof(info.isSuccess));
}
void GGameController::OnUploadSpeakerMusicPCMData(CommandOfSpeakerMusicPCMData& info)
{    
    MessageOfSpeakerMusicPCMData msg;
    memcpy(&msg.NetAddress, &info.NetAddress, sizeof(NETADDR));
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}


//
//add by jinli 2012-04-14
//
void GGameController::OnDisConnectWithModel(ClientDisconnectMsg& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_DisConnectModel, NULL, 0);
}
void GGameController::OnOpenSpeakerResponse(MessageOfOpenSpeakerResponse& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_OpenSpeakerResponse, &info.remark, sizeof(&info.remark));
}
void GGameController::OnCloseSpeakerResponse(MessageOfCloseSpeakerResponse& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_CloseSpeakerResponse, &info.remark, sizeof(&info.remark));
}


//
//add by lugang
//
GGameController& GGameController::sharedInstance()
{
    static GGameController instance;
    return instance;
}

GGameController::GGameController()
{
    m_TankControlSession = 0;
}

ClientSession *GGameController::session()
{
    return m_TankControlSession;
}
void GGameController::UnInit()
{
    if (m_TankControlSession)
    {
        UnRegMsgHandler(m_TankControlSession);
        UnRegCommandHandler( &( GGameCMDManager::sharedInstance() ) );
        delete m_TankControlSession;
        m_TankControlSession = 0;
    }
}

//
//add by jinli 2012-04-18
//
void GGameController::OnPingPongCommand(CommandOfPingPong& info)
{
    MessageOfPingPong msg;
    memcpy(&msg.time, &info.time, sizeof(info.time));
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

//
//add by jinli 2012-04-26
//
void GGameController::OnPauseStreamMusic(CommandOfModelControlPauseStreamMusic& info)
{
//    if (mLogic->GetCurrentState()->GetStateID() == ServerLogicStates::NORMAL_GAME_STATE) 
    {
        MessageOfPauseStreamMusic msg;
        msg.remark = info.remark;
        m_TankControlSession->SendMsg(msg);
    }
}

//
//add by jinli 2012-05-18
//
void GGameController::OnPWMController(CommandOfPWMController& info)
{
    MessageOfPWMController msg;
    msg.count = info.count;
    memcpy(msg.pwmId, info.pwmId, 10 * sizeof(int));
    memcpy(msg.pwmValue, info.pwmValue, 10 * sizeof(int));
    m_TankControlSession->SendMsg(msg);
}
void GGameController::OnLEDController(CommandOfLEDController& info)
{
    MessageOfLEDController msg;
    msg.count = info.count;
    memcpy(msg.ledId, info.ledId, 10 * sizeof(int));
    memcpy(msg.onDuration, info.onDuration, 10 * sizeof(int));
    memcpy(msg.offDuration, info.offDuration, 10 * sizeof(int));
    m_TankControlSession->SendMsg(msg);
}

//
//add by jinli 2012-06-06
//
void GGameController::OnClearProductTester(CommandOfClearProductTester& info)
{
    MessageOfClearProductTester msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}
void GGameController::OnClearProductTesterResponse(MessageOfClearProductTesterResponse& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_ClearProductTesterResponse, &info.remark, sizeof(&info.remark));
}
void GGameController::OnClearProductTesterRestart(CommandOfClearProductTesterRestart& info)
{
    MessageOfClearProductTesterRestart msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

//
//add by jinli 2012-11-05
//
void GGameController::OnNeedModelAdvanceInfo(CommandOfNeedModelAdvanceInfo& info)
{
    MessageOfNeedModelAdvanceInfo msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}
void GGameController::OnModelAdvanceInfo(MessageOfModelAdvanceInfo& info)
{
    int msgID = MUIM_M2C_ModelAdvanceInfo;
    GGameClient::sharedInstance().swallowMessage(msgID, &info.info, sizeof(ModelAdvanceInfo));
}
void GGameController::On5SM3nForward(CommandOf5SM3nForward& info)
{
    MessageOf5SM3nForward msg;
    msg.stall = info.stall;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}
void GGameController::On5SM3nBackward(CommandOf5SM3nBackward& info)
{
    MessageOf5SM3nBackward msg;
    msg.stall = info.stall;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}
void GGameController::On5SM3nLeftTurn(CommandOf5SM3nLeftTurn& info)
{
    MessageOf5SM3nLeftTurn msg;
    msg.stall = info.stall;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}
void GGameController::On5SM3nRightTurn(CommandOf5SM3nRightTurn& info)
{
    MessageOf5SM3nRightTurn msg;
    msg.stall = info.stall;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}
void GGameController::On5SM3nRightStopMove(CommandOf5SM3nStopMove& info)
{
    MessageOf5SM3nStopMove msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);  
}
void GGameController::On5SM3nRightStopTurn(CommandOf5SM3nStopTurn& info)
{
    MessageOf5SM3nStopTurn msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

//
//add by jinli 2012-12-18
//
void GGameController::On5HT6Forward(CommandOf5HT6Forward& info)
{
    MessageOf5HT6Forward msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6Backward(CommandOf5HT6Backward& info)
{
    MessageOf5HT6Backward msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6TurnLeft(CommandOf5HT6TurnLeft& info)
{
    MessageOf5HT6TurnLeft msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6TurnRight(CommandOf5HT6TurnRight& info)
{
    MessageOf5HT6TurnRight msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6FlyLeft(CommandOf5HT6FlyLeft& info)
{
    MessageOf5HT6FlyLeft msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6FlyRight(CommandOf5HT6FlyRight& info)
{
    MessageOf5HT6FlyRight msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6GoUp(CommandOf5HT6GoUp& info)
{
    MessageOf5HT6GoUp msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6GoDown(CommandOf5HT6GoDown& info)
{
    MessageOf5HT6GoDown msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6CMCommonUse(CommandOf5HT6CMCommonUse& info)
{
    MessageOf5HT6CMCommonUse msg;
    msg.dataSize = info.dataSize;
    msg.messageID = info.messageID;
    msg.remark = info.remark;
    memcpy(&msg.data, &info.data, 128 * sizeof(char));
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6IPAddress(CommandOf5HT6IPAddress& info)
{
    MessageOf5HT6IPAddress msg;
    msg.remark = info.remark;
    memcpy(&msg.NetAddress, &info.NetAddress, 128 * sizeof(NETADDR));
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6StopMove(CommandOf5HT6StopMove& info)
{
    MessageOf5HT6StopMove msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6StopSelfTurn(CommandOf5HT6StopSelfTurn& info)
{
    MessageOf5HT6StopSelfTurn msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6StopFlyTurn(CommandOf5HT6StopFlyTurn& info)
{
    MessageOf5HT6StopFlyTurn msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6StopUpDown(CommandOf5HT6StopUpDown& info)
{
    MessageOf5HT6StopUpDown msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nForward(CommandOf5RM6nForward& info)
{
    MessageOf5RM6nForward msg;
    msg.stall = info.stall;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nBackward(CommandOf5RM6nBackward& info)
{
    MessageOf5RM6nBackward msg;
    msg.stall = info.stall;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nStopMove(CommandOf5RM6nStopMove& info)
{
    MessageOf5RM6nStopMove msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nTurnLeft(CommandOf5RM6nTurnLeft& info)
{
    MessageOf5RM6nTurnLeft msg;
    msg.stall = info.stall;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nTurnRight(CommandOf5RM6nTurnRight& info)
{
    MessageOf5RM6nTurnRight msg;
    msg.stall = info.stall;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nStopTurn(CommandOf5RM6nStopTurn& info)
{
    MessageOf5RM6nStopTurn msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nWaistTurnLeft(CommandOf5RM6nWaistTurnLeft& info)
{
    MessageOf5RM6nWaistTurnLeft msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nWaistTurnRight(CommandOf5RM6nWaistTurnRight& info)
{
    MessageOf5RM6nWaistTurnRight msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nWaistStopTurn(CommandOf5RM6nWaistStopTurn& info)
{
    MessageOf5RM6nWaistStopTurn msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nLeftHandAttack(CommandOf5RM6nLeftHandAttack& info)
{
    MessageOf5RM6nLeftHandAttack msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nRightHandAttack(CommandOf5RM6nRightHandAttack& info)
{
    MessageOf5RM6nRightHandAttack msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nLeftGunFire(CommandOf5RM6nLeftGunFire& info)
{
    MessageOf5RM6nLeftGunFire msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nRightGunFire(CommandOf5RM6nRightGunFire& info)
{
    MessageOf5RM6nRightGunFire msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nStartBattle(CommandOf5RM6nStartBattle& info)
{
    MessageOf5RM6nStartBattle msg;
    msg.groupID = info.groupID;
    msg.userID = info.userID;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5RM6nFinishedBattle(CommandOf5RM6nFinishedBattle& info)
{
    MessageOf5RM6nFinishedBattle msg;
    msg.status = info.status;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5SM3nSBeginBattle(CommandOf5SM3nSBeginBattle& info)
{
    MessageOf5SM3nSBeginBattle msg;
    msg.group = info.group;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5SM3nSFinishBattle(CommandOf5SM3nSFinishBattle& info)
{
    MessageOf5SM3nSFinishBattle msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5SM3nSFire(CommandOf5SM3nSFire& info)
{
    MessageOf5SM3nSFire msg;
    msg.remark = info.remark;
    m_TankControlSession->SendMsg(msg);
}

void GGameController::On5HT6GroundDistance(MessageOf5HT6GroundDistance& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_5HT6_GroundDistance, &(info.distance), sizeof(int));
}

void GGameController::On5HT6MCCommonUse(MessageOf5HT6MCCommonUse& info)
{
    _5HT6M2CCustomData customData;
    memcpy(customData.data, info.data, 128 * sizeof(char));
    customData.dataSize = info.dataSize;
    customData.messageID = info.messageID;
    customData.remark = info.remark;
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_5HT6_CommonUse, &customData, sizeof(_5HT6M2CCustomData));
}

void GGameController::On5RM6nBeAttackedByGun(MessageOf5RM6nBeAttackedByGun& info)
{
    _5RM6nEnemyAttackInfo attackInfo;
    attackInfo.teamID = info.teamID;
    attackInfo.gunID = info.gunID;
    attackInfo.userID = info.userID;
    attackInfo.remark = info.remark;
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_5RM6n_BeAttackedByGun, &attackInfo, sizeof(_5RM6nEnemyAttackInfo));
}

void GGameController::On5RM6nHeadBeAttacked(MessageOf5RM6nHeadBeAttacked& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_5RM6n_HeadBeAttacked, &(info.remark), sizeof(int));
}

void GGameController::On5RM6nWaistAngle(MessageOf5RM6nWaistAngle& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_5RM6n_WaistAngle, &(info.angle), sizeof(int));
}

void GGameController::On5RM6nReportGunStatus(MessageOf5RM6nReportGunStatus& info)
{
    _5RM6nGunStatus gunStatus;
    gunStatus.remark = info.remark;
    gunStatus.left = info.left;
    gunStatus.right = info.right;

    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_5RM6n_ReportGunStatus, &gunStatus, sizeof(_5RM6nGunStatus));
}

void GGameController::On5SM3nSEnemyAttack(MessageOf5SM3nSEnemyAttack& info)
{
    _5SM3nSEnemyAttack attackInfo;
    attackInfo.group = info.group;
    attackInfo.remark = info.remark;
    GGameClient::sharedInstance().swallowMessage(MUIM_M2C_5SM3nS_EnemyAttack, &attackInfo, sizeof(_5SM3nSEnemyAttack));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//add by jinli 2013-02-18   GWG
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GGameController::On5SM3nSGWGKickPlayer(CommandOf5SM3nSGWGKickPlayer& info)
{
    MessageOf5SM3nSGWGKickPlayer message;
    message.gameID = info.gameID;
    message.remark = info.remark;
    
    m_TankControlSession->SendMsg(message);
}

void GGameController::On5SM3nSGWGKickPlayerResponse(MessageOf5SM3nSGWGKickPlayerResponse& info)
{
    _5SM3nSGWGKickPlayerResponseData data;
    data.gameID = info.gameID;
    data.remark = info.remark > 0 ? 1 : 0;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_KickPlayerResponse, &data, sizeof(_5SM3nSGWGKickPlayerResponseData));
}

void GGameController::On5SM3nSGWGDisbandGame(CommandOf5SM3nSGWGDisbandGame& info)
{
    MessageOf5SM3nSGWGDisbandGame message;
    message.remark = info.remark;
    
    m_TankControlSession->SendMsg(message);
}

void GGameController::On5SM3nSGWGDisbandGameResponse(MessageOf5SM3nSGWGDisbandGameResponse& info)
{
    _5SM3nSGWGDisbandGameResponseData data;
    data.remark = info.remark > 0 ? 1 : 0;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_DisbandGameResponse, &data, sizeof(_5SM3nSGWGDisbandGameResponseData));
}

void GGameController::On5SM3nSGWGLeaveGame(CommandOf5SM3nSGWGLeaveGame& info)
{
    MessageOf5SM3nSGWGLeaveGame message;
    message.remark = info.remark;
    
    m_TankControlSession->SendMsg(message);
}

void GGameController::On5SM3nSGWGLeaveGameResponse(MessageOf5SM3nSGWGLeaveGameResponse& info)
{
    _5SM3nSGWGLeaveGameResponseData data;
    data.remark = info.remark > 0 ? 1 : 0;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_LeaveGameResponse, &data, sizeof(_5SM3nSGWGLeaveGameResponseData));
}

void GGameController::On5SM3nSGWGStartGame(CommandOf5SM3nSGWGStartGame& info)
{
    MessageOf5SM3nSGWGStartGame message;
    message.remark = info.remark;
    
    m_TankControlSession->SendMsg(message);
}

void GGameController::On5SM3nSGWGStartGameResponse(MessageOf5SM3nSGWGStartGameResponse& info)
{
    _5SM3nSGWGStartGameResponseData data;
    data.remark = info.remark > 0 ? 1 : 0;
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_StartGameResponse, &data, sizeof(_5SM3nSGWGStartGameResponseData));
}

void GGameController::On5SM3nSGWGSomeBodyJoinGame(MessageOf5SM3nSGWGSomeBodyJoinGameWhileWaiting& info)
{
    _5SM3nSGWGSomeBodyJoinGameData data;
    data.gameID = info.gameID;
    data.teamID = info.teamID;
    memcpy(data.name, info.name, 32);
    data.remark = info.remark > 0 ? 1 : 0;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_SomeBodyJoinGame, &data, sizeof(_5SM3nSGWGSomeBodyJoinGameData));
}

void GGameController::On5SM3nSGWGSomeBodyLeaveGame(MessageOf5SM3nSGWGSomeBodyLeaveGameWhileWaiting& info)
{
    _5SM3nSGWGSomeBodyLeaveGameData data;
    data.gameID = info.gameID;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_SomeBodyLeaveGame, &data, sizeof(_5SM3nSGWGSomeBodyLeaveGameData));
}


void GGameController::On5SM3nSGWGSearchGameServer(CommandOf5SM3nSGWGSearchGameServer& info)
{
    MessageOf5SM3nSGWGSearchGameServer message;
    message.remark = info.remark;
    
    m_TankControlSession->SendMsg(message);
}

void GGameController::On5SM3nSGWGStopSearchGameServer(CommandOf5SM3nSGWGStopSearchGameServer& info)
{
    MessageOf5SM3nSGWGStopSearchGameServer message;
    message.remark = info.remark;
    
    m_TankControlSession->SendMsg(message);
}

void GGameController::On5SM3nSGWGJoinSearchedGameServer(CommandOf5SM3nSGWGJoinSearchedGameServer& info)
{
    MessageOf5SM3nSGWGJoinSearchedGameServer message;
    memcpy(message.macAddr, info.macAddr, 32);
    memcpy(message.nickName, info.nickName, 32);
    message.teamID = info.teamID;
    message.remark = info.remark;
    
    m_TankControlSession->SendMsg(message);
}

void GGameController::On5SM3nSGWGRequestGameServerTeamInfo(CommandOf5SM3nSGWGRequestGameServerTeamInfo& info)
{
    MessageOf5SM3nSGWGRequestGameServerTeamInfo message;
    message.teamID = info.teamID;
    message.remark = info.remark;
    
    m_TankControlSession->SendMsg(message);
}

void GGameController::On5SM3nSGWGSearchGameServerResponse(MessageOf5SM3nSGWGSearchGameServerResponse& info)
{
    _5SM3nSGWGSearchGameServerResponseData data;
    data.remark = info.remark;

    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_SearchGameServerResponse, &data, sizeof(_5SM3nSGWGSearchGameServerResponseData));
}

void GGameController::On5SM3nSGWGStopSearchGameServerResponse(MessageOf5SM3nSGWGStopSearchGameServerResponse& info)
{
    _5SM3nSGWGStopSearchGameServerResponse data;
    data.remark = info.remark;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_StopSearchGameServerResponse, &data, sizeof(_5SM3nSGWGStopSearchGameServerResponse));
}

void GGameController::On5SM3nSGWGSearchedGameServerInfo(MessageOf5SM3nSGWGSearchedGameServerInfo& info)
{    
    _5SM3nSGWGSearchedGameServerInfo data;
    
    memcpy(data.macAddr,    info.macAddr,   32);
    memcpy(data.gameName,   info.gameName,  32);
    memcpy(data.hostName,   info.hostName,  32);
    data.gameType               = info.gameType;
    data.gameTimeLength         = info.gameTimeLength;
    data.redTeamMemberNumber    = info.redTeamMemberNumber;
    data.whiteTeamMemberNumber  = info.whiteTeamMemberNumber;
    data.blueTeamMemberNumber   = info.blueTeamMemberNumber;
    data.remark                 = info.remark;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_SearchedGameServerInfo, &data, sizeof(_5SM3nSGWGSearchedGameServerInfo));
}

void GGameController::On5SM3nSGWGFinishedSearchGameServer(MessageOf5SM3nSGWGFinishedSearchGameServer& info)
{
    _5SM3nSGWGFinishedSearchGameServer data;
    data.remark = info.remark;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_FinishedSearchGameServer, &data, sizeof(_5SM3nSGWGFinishedSearchGameServer));
}

void GGameController::On5SM3nSGWGJoinSearchedGameServerResponse(MessageOf5SM3nSGWGJoinSearchedGameServerResponse& info)
{
    _5SM3nSGWGJoinSearchedGameServerResponse data;
    data.remark = info.remark;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_JoinSearchedGameServerResponse, &data, sizeof(_5SM3nSGWGJoinSearchedGameServerResponse));
}

void GGameController::On5SM3nSGWGGameServerTeamInfo(MessageOf5SM3nSGWGGameServerTeamInfo& info)
{
    _5SM3nSGWGGameServerTeamInfo data;

    memcpy(data.gameID,    info.gameID,    8 * sizeof(int));
    memcpy(data.isHost,    info.isHost,    8 * sizeof(int));
    memcpy(data.nickName0, info.nickName0, 32);
    memcpy(data.nickName1, info.nickName1, 32);
    memcpy(data.nickName2, info.nickName2, 32);
    memcpy(data.nickName3, info.nickName3, 32);
    memcpy(data.nickName4, info.nickName4, 32);
    memcpy(data.nickName5, info.nickName5, 32);
    memcpy(data.nickName6, info.nickName6, 32);
    memcpy(data.nickName7, info.nickName7, 32);
    data.teamType               = info.teamType;
    data.teamMemberNumber       = info.teamMemberNumber;
    data.remark                 = info.remark;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_GameServerTeamInfo, &data, sizeof(_5SM3nSGWGGameServerTeamInfo));
}


void GGameController::On5SM3nSGWGCreateGame(CommandOf5SM3nSGWGCreateGame& info)
{
    MessageOf5SM3nSGWGCreateGame message;
    
    memcpy(message.gameName, info.gameName, 32);
    memcpy(message.nickName, info.nickName, 32);
    message.gameType = info.gameType;
    message.gameTime = info.gameTime;
    message.gameTeam = info.gameTeam;
    message.remark   = info.remark;
    
    m_TankControlSession->SendMsg(message);
}

void GGameController::On5SM3nSGWGCreateGameResponse(MessageOf5SM3nSGWGCreateGameResponse& info)
{
    _5SM3nSGWGCreateGameResponse data;
    data.gameID = info.gameID;
    data.remark = info.remark;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_CreateGameResponse, &data, sizeof(_5SM3nSGWGCreateGameResponse));
}


void GGameController::On5SM3nSGWGHostStartGame(MessageOf5SM3nSGWGHostStartGame& info)
{
    _5SM3nSGWGHostStartGame data;
    data.remark = info.remark;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_HostStartGame, &data, sizeof(_5SM3nSGWGHostStartGame));
}

void GGameController::On5SM3nSGWGHostDisbandGame(MessageOf5SM3nSGWGHostDisbandGame& info)
{
    _5SM3nSGWGHostDisbandGame data;
    data.remark = info.remark;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_HostDisbandGame, &data, sizeof(_5SM3nSGWGHostDisbandGame));
}

void GGameController::On5SM3nSGWGLeaveGameWhileBattling(CommandOf5SM3nSGWGLeaveGameWhileBattling& info)
{
    MessageOf5SM3nSGWGLeaveGameWhileBattling message;
    message.remark = info.remark;
    
    m_TankControlSession->SendMsg(message);
}

void GGameController::On5SM3nSGWGLeaveGameWhileBattlingResponse(MessageOf5SM3nSGWGLeaveGameWhileBattlingResponse& info)
{
    _5SM3nSGWGLeaveGameWhileBattlingResponseData data;
    data.remark = info.remark;
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_LeaveGameWhileBattlingResponse, &data, sizeof(_5SM3nSGWGLeaveGameWhileBattlingResponseData));
}

void GGameController::On5SM3nSGWGGameServerMessage(MessageOf5SM3nSGWGGameServerMessage& info)
{
    _5SM3nSGWGGameServerMessageData data;
    data.message = info.message;
    data.remark  = info.remark;
    memcpy(data.nickName, info.nickName, 32);
    
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_GameServerMessage, &data, sizeof(_5SM3nSGWGGameServerMessageData));
}

void GGameController::On5SM3nSGWGGameOver(MessageOf5SM3nSGWGGameOver& info)
{
    _5SM3nSGWGGameOverData data;
    data.selfStatus  = info.selfStatus;
    data.teamStatus  = info.teamStatus;
    data.continueWaiting = info.continueWaiting;

    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_GameOver, &data, sizeof(_5SM3nSGWGGameOverData));
}

void GGameController::On5SM3nSGWGSomebodyLeaveWhileBattling(MessageOf5SM3nSGWGSomeBodyLeaveGameWhileBattling& info)
{
    _5SM3nSGWGSomebodyLeaveGameWhileBattling data;
    data.gameID  = info.gameID;
    memcpy(data.nickName, info.nickname, 32);
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_SomeBodyLeaveGameWhileBattling, &data, sizeof(_5SM3nSGWGGameOverData));
}

void GGameController::OnBeKickedoutByHost(MessageOf5SM3nSGWGBeKickedoutByHost& info)
{
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_BeKickedoutByHost, 0, 0);
}

void GGameController::OnGWGRequestTeamMemberIDList(CommandOf5SM3nSGWGRequestTeamIDList& info)
{
    MessageOf5SM3nSGWGRequestTeamMemberIDList message;
    m_TankControlSession->SendMsg(message);
}

void GGameController::OnGWGTeamMemberIDList(MessageOf5SM3nSGWGTeamMemberIDList& info)
{
    _5SM3nSGWGTeamMemberIDList data;
    memcpy(data.white, info.teamMemberIDList0, 8 * sizeof(int));
    memcpy(data.red  , info.teamMemberIDList1, 8 * sizeof(int));
    memcpy(data.blue , info.teamMemberIDList2, 8 * sizeof(int));
    GGameClient::sharedInstance().swallowMessage(MUIM_5SM3nS_GWG_TeamMemberIDList, &data, sizeof(_5SM3nSGWGTeamMemberIDList));
}

