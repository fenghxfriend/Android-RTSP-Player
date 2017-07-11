#ifndef GGameCommand_h
#define GGameCommand_h

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class GGameCommand
{
public:
    static  GGameCommand& sharedInstance();
    
    ///////////////////////////////////////////////////// Common /////////////////////////////////////////////////////
    void    controlDevice(const char *ip);
    void    disconnectWithDevice();
    void    customDisconnectWithDevice(const char *reason, int remark);
    void    cacheConfigruation(const char *ssid, bool channelUF, int channel, bool volumeUF, int volume, bool calibrationUF, int calibration);
    void    updateConfigruation(bool flag);
    void    forward(int ratio);
    void    backward(int ratio);
    void    stop();
    void    startLeftTurn(int ratio);
    void    stopLeftTurn();
    void    startRightTurn(int ratio);
    void    stopRightTurn();
    void    cameraMoveUp();
    void    cameraMoveDown();
    void    cameraMoveStop();
    void    openLeftLED(int on, int off);
    void    closeLeftLED();
    void    openRightLED(int on, int off);
    void    closeRightLED();
    void    openTailLED(int on, int off);
    void    closeTailLED();
    void    openHeadLight();
    void    closeHeadLight();
    void    openSpeaker(const char *filename, bool response);
    void    closeSpeaker(bool response);
    void    openSpeaking(const char *ip, int port, int cachetimes, bool response);
    void    closeSpeaking(bool response);
    void    openListening(bool response);
    void    closeListening(bool response);
    void    playMusic(const char *filename, const char *ip, int cachetimes, struct timeval distance, int port, bool response);
    void    stopMusic(bool response);
    void    updateSpeakerMusic(int target, const char *ip);
    void    requestAdvanceConfigruation();
    
    ///////////////////////////////////////////////////// 5SM3nS /////////////////////////////////////////////////////
    void    GP5SM3nSForward(int stall);
    void    GP5SM3nSBackward(int stall);
    void    GP5SM3nSTurnLeft(int stall);
    void    GP5SM3nSTurnRight(int stall);
    void    GP5SM3nSStopMove();
    void    GP5SM3nSStopTurn();
    void    GP5SM3nSBeginBattle();
    void    GP5SM3nSFinishBattle(bool fail);
    void    GP5SM3nSFire();

    ///////////////////////////////////////////////////// 5HT6 /////////////////////////////////////////////////////
    void    GP5HT6Forward();
    void    GP5HT6Backward();
    void    GP5HT6TurnLeft();
    void    GP5HT6TurnRight();
    void    GP5HT6FlyLeft();
    void    GP5HT6FlyRight();
    void    GP5HT6GoUp();
    void    GP5HT6GoDown();
    void    GP5HT6StopMove();
    void    GP5HT6StopSelfTurn();
    void    GP5HT6StopFlyTurn();
    void    GP5HT6StopUpDown();

    ///////////////////////////////////////////////////// 5RM6n /////////////////////////////////////////////////////
    void    GP5RM6nForward(int stall);
    void    GP5RM6nBackward(int stall);
    void    GP5RM6nStopMove();
    void    GP5RM6nTurnLeft(int stall);
    void    GP5RM6nTurnRight(int stall);
    void    GP5RM6nStopTurn();
    void    GP5RM6nWaistTurnLeft();
    void    GP5RM6nWaistTurnRight();
    void    GP5RM6nWaistStopTurn();
    void    GP5RM6nLeftHandAttack();
    void    GP5RM6nRightHandAttack();
    void    GP5RM6nLeftGunFire();
    void    GP5RM6nRightGunFire();
    void    GP5RM6nStartBattle(int group);
    void    GP5RM6nFinishedBattle(bool fail);

    ///////////////////////////////////////////////////// GWG /////////////////////////////////////////////////////
    void    GWGKickGamePlayer(int gameID);
    void    GWGDisbandGame();
    void    GWGLeaveGameWhileWaiting();
    void    GWGStartGame();
    void    GWGSearchGameServer();
    void    GWGStopSearchGameServer();
    void    GWGJoinGameServer(const char *macaddr, int team, const char *name);
    void    GWGRequestTeamInfo(int teamID);
    void    GWGCreateGame(const char *gamename, const char *nickname, int gametype, int gametime, int gameteam);
    void    GWGLeaveGameWhileBattling();
    void    GWGRequestTeamMemberIDList();
};

#endif
