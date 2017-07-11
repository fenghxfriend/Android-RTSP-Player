#include "GGameCommand.h"
#include "GGameCMDManager.h"
#include "CommandDef.h"

GGameCommand& GGameCommand::sharedInstance()
{
    static GGameCommand instance;
    return instance;
}

///////////////////////////////////////////////////// Common /////////////////////////////////////////////////////
void GGameCommand::controlDevice(const char *ip)
{
    if ( ip == NULL ) return;
    
    JionTankServerCommand command;
    command.tankID = -1;
    strcpy(command.password, "");
    strcpy(command.ip, ip);

    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::disconnectWithDevice()
{
    DisconnectTankCommand command;
    command.isCustom = 0;

    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::customDisconnectWithDevice(const char *reason, int remark)
{
    DisconnectTankCommand command;
    command.isCustom    = 1;
    command.remark      = remark;
    if ( reason != NULL ) strcpy(command.reason, reason);
    else strcpy(command.reason, "");
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::cacheConfigruation(const char *ssid, bool channelUF, int channel, bool volumeUF, int volume, bool calibrationUF, int calibration)
{
    CommandOfUpdateModelConfiger command;
    
    if ( ssid != NULL )
    {
        strcpy(command.modelName, ssid);
        command.updateFlagOfmodelName = 1;
    }
    else
    {
        command.updateFlagOfmodelName = 0;
    }
    if ( channelUF )
    {
        command.wifiChannel             = channel;
        command.updateFlagOfwifiChannel = 1;
    }
    else
    {
        command.updateFlagOfwifiChannel = 0;
    }
    
    if ( volumeUF )
    {
        command.speakerVolume = volume;
    }
    else
    {
        command.speakerVolume = -1000;
    }
    if ( calibrationUF )
    {
        command.modelOf5BM2SteerCalibration = calibration;
    }
    else
    {
        command.modelOf5BM2SteerCalibration = -1000;
    }
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::updateConfigruation(bool flag)
{
    CommandOfExecUpdateModelConfiger command;
    command.flag = flag ? 1 : 0;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::forward(int ratio)
{
    CommandOfModelControlForword command;
    command.ratio = ratio;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::backward(int ratio)
{
    CommandOfModelControlBack command;
    command.ratio = ratio;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::stop()
{
    CommandOfModelControlStop command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::startLeftTurn(int ratio)
{
    CommandOfModelControlStartTurnLeft command;
    command.ratio = ratio;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::stopLeftTurn()
{
    CommandOfModelControlStopTurnLeft command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::startRightTurn(int ratio)
{
    CommandOfModelControlStartTurnRight command;
    command.ratio = ratio;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::stopRightTurn()
{
    CommandOfModelControlStopTurnRight command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::cameraMoveUp()
{
    CommandOfModelControlCameraMoveUp command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::cameraMoveDown()
{
    CommandOfModelControlCameraMoveDown command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::cameraMoveStop()
{
    CommandOfModelControlCameraStop command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::openLeftLED(int on, int off)
{
    CommandOfModelControlStartLeftLED command;
    command.onDuration  = on;
    command.offDuration = off;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::closeLeftLED()
{
    CommandOfModelControlStopLeftLED command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::openRightLED(int on, int off)
{
    CommandOfModelControlStartRightLED command;
    command.onDuration  = on;
    command.offDuration = off;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::closeRightLED()
{
    CommandOfModelControlStopRightLED command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::openTailLED(int on, int off)
{
    CommandOfModelControlStartBehindLED command;
    command.onDuration  = on;
    command.offDuration = off;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::closeTailLED()
{
    CommandOfModelControlStopBehindLED command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::openHeadLight()
{
    CommandOfModelControlOpenFrontLight command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::closeHeadLight()
{
    CommandOfModelControlCloseFrontLight command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::openSpeaker(const char *filename, bool response)
{
    if ( filename == NULL ) return;
    
    CommandOfModelControlOpenSpeaker command;
    strcpy(command.filename, filename);
    command.remark = response ? 1 : 0;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::closeSpeaker(bool response)
{
    CommandOfModelControlCloseSpeaker command;
    command.remark = response ? 1 : 0;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::openSpeaking(const char *ip, int port, int cachetimes, bool response)
{
    CommandOfModelControlOpenPDASoundToModel command;
    
    NETADDR __NETADDR;
    if ( ip != NULL ) net_addr_from_str( &__NETADDR, ip );
    else net_addr_from_str( &__NETADDR, "" );
    __NETADDR.type = NETTYPE_IPV4;
    __NETADDR.port = port;
    
    memcpy(&command.PDAAddress, &__NETADDR, sizeof(NETADDR));
    command.cacheTimes = cachetimes;
    command.remark = response ? 1 : 0;

    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::closeSpeaking(bool response)
{
    CommandOfModelControlClosePDASoundToModel command;
    command.remark = response ? 1 : 0;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::openListening(bool response)
{
    CommandOfModelControlOpenModelSoundToPDA command;
    command.remark = response ? 1 : 0;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::closeListening(bool response)
{
    CommandOfModelControlCloseModelSoundToPDA command;
    command.remark = response ? 1 : 0;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::playMusic(const char *filename, const char *ip, int cachetimes, struct timeval distance, int port, bool response)
{
    if ( filename == NULL ) return;
    
    CommandOfModelControlOpenStreamMusic command;
    
    NETADDR __NETADDR;
    if ( ip != NULL ) net_addr_from_str( &__NETADDR, ip );
    else net_addr_from_str( &__NETADDR, "" );
    __NETADDR.type = NETTYPE_IPV4;
    __NETADDR.port = port;
    memcpy( &command.NetAddress, &__NETADDR, sizeof(NETADDR) );
    
    str_copy(command.fileName, filename, 64);
    command.cacheTimes      = cachetimes;
    command.seconds         = 0;
    command.microSeconds    = 0;
    command.timeDistance    = distance;
    command.remark          = response ? 1 : 0;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::stopMusic(bool response)
{
    CommandOfModelControlCloseStreamMusic command;
    command.remark = response ? 1 : 0;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::updateSpeakerMusic(int target, const char *ip)
{
    CommandOfSpeakerMusicPCMData command;
    
    NETADDR __NETADDR;
    if ( ip != NULL ) net_addr_from_str( &__NETADDR, ip );
    else net_addr_from_str( &__NETADDR, "" );
    __NETADDR.type = NETTYPE_IPV4;
    memcpy( &command.NetAddress, &__NETADDR, sizeof(NETADDR) );
    command.remark = target;
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::requestAdvanceConfigruation()
{
    CommandOfNeedModelAdvanceInfo command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}


///////////////////////////////////////////////////// 5SM3nS /////////////////////////////////////////////////////
void GGameCommand::GP5SM3nSForward(int stall)
{
    CommandOf5SM3nForward command;
    command.stall   = stall;
    command.remark  = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GP5SM3nSBackward(int stall)
{
    CommandOf5SM3nBackward command;
    command.stall   = stall;
    command.remark  = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GP5SM3nSTurnLeft(int stall)
{
    CommandOf5SM3nLeftTurn command;
    command.stall   = stall;
    command.remark  = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GP5SM3nSTurnRight(int stall)
{
    CommandOf5SM3nRightTurn command;
    command.stall   = stall;
    command.remark  = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GP5SM3nSStopMove()
{
    CommandOf5SM3nStopMove command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GP5SM3nSStopTurn()
{
    CommandOf5SM3nStopTurn command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GP5SM3nSBeginBattle()
{
    CommandOf5SM3nSBeginBattle command;
    command.group = 0;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GP5SM3nSFinishBattle(bool fail)
{
    CommandOf5SM3nSFinishBattle command;
    command.remark = fail ? 1 : 0;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GP5SM3nSFire()
{
    CommandOf5SM3nSFire command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

///////////////////////////////////////////////////// 5HT6 /////////////////////////////////////////////////////
void GGameCommand::GP5HT6Forward()
{
    CommandOf5HT6Forward command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6Backward()
{
    CommandOf5HT6Backward command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6TurnLeft()
{
    CommandOf5HT6TurnLeft command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6TurnRight()
{
    CommandOf5HT6TurnRight command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6FlyLeft()
{
    CommandOf5HT6FlyLeft command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6FlyRight()
{
    CommandOf5HT6FlyRight command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6GoUp()
{
    CommandOf5HT6GoUp command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6GoDown()
{
    CommandOf5HT6GoDown command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6StopMove()
{
    CommandOf5HT6StopMove command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6StopSelfTurn()
{
    CommandOf5HT6StopSelfTurn command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6StopFlyTurn()
{
    CommandOf5HT6StopFlyTurn command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5HT6StopUpDown()
{
    CommandOf5HT6StopUpDown command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

///////////////////////////////////////////////////// 5RM6n /////////////////////////////////////////////////////
void GGameCommand::GP5RM6nForward(int stall)
{
    CommandOf5RM6nForward command;
    command.stall = stall;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nBackward(int stall)
{
    CommandOf5RM6nBackward command;
    command.stall = stall;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nStopMove()
{
    CommandOf5RM6nStopMove command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nTurnLeft(int stall)
{
    CommandOf5RM6nTurnLeft command;
    command.stall = stall;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nTurnRight(int stall)
{
    CommandOf5RM6nTurnRight command;
    command.stall = stall;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nStopTurn()
{
    CommandOf5RM6nStopTurn command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nWaistTurnLeft()
{
    CommandOf5RM6nWaistTurnLeft command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nWaistTurnRight()
{
    CommandOf5RM6nWaistTurnRight command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nWaistStopTurn()
{
    CommandOf5RM6nWaistStopTurn command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nLeftHandAttack()
{
    CommandOf5RM6nLeftHandAttack command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nRightHandAttack()
{
    CommandOf5RM6nRightHandAttack command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nLeftGunFire()
{
    CommandOf5RM6nLeftGunFire command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nRightGunFire()
{
    CommandOf5RM6nRightGunFire command;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nStartBattle(int group)
{
    CommandOf5RM6nStartBattle command;
    command.groupID = group;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
void GGameCommand::GP5RM6nFinishedBattle(bool fail)
{
    CommandOf5RM6nFinishedBattle command;
    command.status = fail ? 0 : 1;
    command.remark = 1;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

///////////////////////////////////////////////////// GWG /////////////////////////////////////////////////////
void GGameCommand::GWGKickGamePlayer(int gameID)
{
    CommandOf5SM3nSGWGKickPlayer command;
    command.gameID = gameID;
    command.remark = 0;
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GWGDisbandGame()
{
    CommandOf5SM3nSGWGDisbandGame command;
    command.remark = 0;
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GWGLeaveGameWhileWaiting()
{
    CommandOf5SM3nSGWGLeaveGame command;
    command.remark = 0;
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GWGStartGame()
{
    CommandOf5SM3nSGWGStartGame command;
    command.remark = 0;
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GWGSearchGameServer()
{
    CommandOf5SM3nSGWGSearchGameServer command;
    command.remark = 0;
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GWGStopSearchGameServer()
{
    CommandOf5SM3nSGWGStopSearchGameServer command;
    command.remark = 0;
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GWGJoinGameServer(const char *macaddr, int team, const char *name)
{
    if ( macaddr == NULL || name == NULL ) return;
    
    CommandOf5SM3nSGWGJoinSearchedGameServer command;
    strcpy( command.macAddr , macaddr);
    strcpy( command.nickName, name   );
    command.teamID = team;
    command.remark = 0;
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GWGRequestTeamInfo(int teamID)
{
    CommandOf5SM3nSGWGRequestGameServerTeamInfo command;
    command.teamID = teamID;
    command.remark = 0;
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GWGCreateGame(const char *gamename, const char *nickname, int gametype, int gametime, int gameteam)
{
    if ( gamename == NULL || nickname == NULL ) return;
    
    CommandOf5SM3nSGWGCreateGame command;
    
    strcpy( command.gameName, gamename );
    strcpy( command.nickName, nickname );
    command.gameType = gametype;
    command.gameTime = gametime;
    command.gameTeam = gameteam;
    command.remark   = 0;
    
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GWGLeaveGameWhileBattling()
{
    CommandOf5SM3nSGWGLeaveGameWhileBattling command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}

void GGameCommand::GWGRequestTeamMemberIDList()
{
    CommandOf5SM3nSGWGRequestTeamIDList command;
    GGameCMDManager::sharedInstance().sendCommand(command);
}
