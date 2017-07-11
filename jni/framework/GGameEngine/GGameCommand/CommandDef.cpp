#include "CommandDef.h"

RegCommand(SearchTankCommand);
RegCommand(JionTankServerCommand);
RegCommand(DisconnectTankCommand);
RegCommand(MoveCommand);
RegCommand(RotationCommand);
RegCommand(PushPullCommand);
RegCommand(AttackCommand);
RegCommand(NewGameCommand);
RegCommand(CreateRoomCommand);
RegCommand(SerachGameRoomCommand);
RegCommand(DisbandRoomCommand);
RegCommand(JoinRoomCommand);
RegCommand(DeletePlayerCommand);
RegCommand(ExitGameCommand);
RegCommand(BackToWaitCommand);
RegCommand(UseItemCommand);
RegCommand(AimCommand);
RegCommand(RequestLeaveGameCommand);
RegCommand(ChangeTankNameCommand);
RegCommand(ChangeTankPasswordCommand);

//
//add by jinli 2012-03-01
//
RegCommand(CommandOfUpdateModelConfiger);
RegCommand(CommandOfExecUpdateModelConfiger);
RegCommand(CommandOfModelControlForword);
RegCommand(CommandOfModelControlBack);
RegCommand(CommandOfModelControlStop);
RegCommand(CommandOfModelControlStartTurnLeft);
RegCommand(CommandOfModelControlStopTurnLeft);
RegCommand(CommandOfModelControlStartTurnRight);
RegCommand(CommandOfModelControlStopTurnRight);
RegCommand(CommandOfModelControlCameraMoveUp);
RegCommand(CommandOfModelControlCameraMoveDown);
RegCommand(CommandOfModelControlCameraStop);
RegCommand(CommandOfModelControlStartLeftLED);
RegCommand(CommandOfModelControlStopLeftLED);
RegCommand(CommandOfModelControlStartRightLED);
RegCommand(CommandOfModelControlStopRightLED);
RegCommand(CommandOfModelControlStartBehindLED);
RegCommand(CommandOfModelControlStopBehindLED);
RegCommand(CommandOfModelControlOpenFrontLight);
RegCommand(CommandOfModelControlCloseFrontLight);
RegCommand(CommandOfModelControlOpenSpeaker);
RegCommand(CommandOfModelControlCloseSpeaker);

//
//add by jinli 2012-03-26
//
RegCommand(CommandOfModelControlOpenPDASoundToModel);
RegCommand(CommandOfModelControlClosePDASoundToModel);
RegCommand(CommandOfModelControlOpenModelSoundToPDA);
RegCommand(CommandOfModelControlCloseModelSoundToPDA);
RegCommand(CommandOfModelControlOpenStreamMusic);
RegCommand(CommandOfModelControlCloseStreamMusic);

//
//add by jinli 2012-03-29
//
RegCommand(CommandOfSpeakerMusicPCMData);

//
//add by jinli 2012-04-18
//
RegCommand(CommandOfPingPong);

//
//add by jinli 2012-04-26
//
RegCommand(CommandOfModelControlPauseStreamMusic);


//
//add by jinli 2012-05-18
//
RegCommand(CommandOfPWMController);
RegCommand(CommandOfLEDController);

//
//add by jinli 2012-06-06
//
RegCommand(CommandOfClearProductTester);
RegCommand(CommandOfClearProductTesterRestart);

//
//add by jinli 2012-11-05
//
RegCommand(CommandOfNeedModelAdvanceInfo);
RegCommand(CommandOf5SM3nForward);
RegCommand(CommandOf5SM3nBackward);
RegCommand(CommandOf5SM3nLeftTurn);
RegCommand(CommandOf5SM3nRightTurn);
RegCommand(CommandOf5SM3nStopMove);
RegCommand(CommandOf5SM3nStopTurn);

//
//add by jinli 2012-11-29
//
RegCommand(CommandOf5HT6Forward);
RegCommand(CommandOf5HT6Backward);
RegCommand(CommandOf5HT6TurnLeft);
RegCommand(CommandOf5HT6TurnRight);
RegCommand(CommandOf5HT6FlyLeft);
RegCommand(CommandOf5HT6FlyRight);
RegCommand(CommandOf5HT6GoUp);
RegCommand(CommandOf5HT6GoDown);
RegCommand(CommandOf5HT6CMCommonUse);
RegCommand(CommandOf5HT6IPAddress);
RegCommand(CommandOf5HT6StopMove);
RegCommand(CommandOf5HT6StopSelfTurn);
RegCommand(CommandOf5HT6StopFlyTurn);
RegCommand(CommandOf5HT6StopUpDown);

//
//add by jinli 2012-12-07
//
RegCommand(CommandOf5RM6nForward);
RegCommand(CommandOf5RM6nBackward);
RegCommand(CommandOf5RM6nStopMove);
RegCommand(CommandOf5RM6nTurnLeft);
RegCommand(CommandOf5RM6nTurnRight);
RegCommand(CommandOf5RM6nStopTurn);
RegCommand(CommandOf5RM6nWaistTurnLeft);
RegCommand(CommandOf5RM6nWaistTurnRight);
RegCommand(CommandOf5RM6nWaistStopTurn);
RegCommand(CommandOf5RM6nLeftHandAttack);
RegCommand(CommandOf5RM6nRightHandAttack);
RegCommand(CommandOf5RM6nLeftGunFire);
RegCommand(CommandOf5RM6nRightGunFire);
RegCommand(CommandOf5RM6nStartBattle);
RegCommand(CommandOf5RM6nFinishedBattle);

//
//add by jinli 2012-12-07
//
RegCommand(CommandOf5SM3nSBeginBattle);
RegCommand(CommandOf5SM3nSFinishBattle);
RegCommand(CommandOf5SM3nSFire);

//----------------------------------------------------------------
//
//add by jinli 2013-02-18   履带战车对战
//
//----------------------------------------------------------------
RegCommand(CommandOf5SM3nSGWGKickPlayer);
RegCommand(CommandOf5SM3nSGWGDisbandGame);
RegCommand(CommandOf5SM3nSGWGLeaveGame);
RegCommand(CommandOf5SM3nSGWGStartGame);

RegCommand(CommandOf5SM3nSGWGSearchGameServer);
RegCommand(CommandOf5SM3nSGWGStopSearchGameServer);
RegCommand(CommandOf5SM3nSGWGJoinSearchedGameServer);
RegCommand(CommandOf5SM3nSGWGRequestGameServerTeamInfo);

RegCommand(CommandOf5SM3nSGWGCreateGame);
RegCommand(CommandOf5SM3nSGWGLeaveGameWhileBattling);
RegCommand(CommandOf5SM3nSGWGRequestTeamIDList);







