#ifndef __NEW_COMMON_STRUCT_DEF__
#define __NEW_COMMON_STRUCT_DEF__

#include "../../TEEEngine/base/TEE_system.h"

struct TankServerInfo {
	int state;
	char name[32];
    char password[32];
	NETADDR address;
};

struct TankBaseInfo
{
    int     activeWifiChannel;
    char    modelName[100];
    char    macAddress[100];
    char    modelType[100];
    int     activeSpeakerVolume;
    int     steerCalibration;
};

struct _5RM6nEnemyAttackInfo
{
    int teamID;
    int gunID;
    int userID;
    int remark;
};

struct _5HT6M2CCustomData
{
    unsigned char data[128];
    int dataSize;
    int messageID;
    int remark;
};

struct _5RM6nGunStatus
{
    bool left;
    bool right;
    int remark;
};

struct _5SM3nSEnemyAttack
{
    bool group;
    int remark;
};

struct ModelAdvanceInfo
{
    int     wifiType;
    int     speakerVolume[32][2];
    int     wifiChannel[32][2];
    int     speakerVolumeLength;
    int     wifiChannelLength;
    int     remark;
};

struct BeAttackData
{
    int type;
    int damage;
    int curHp;
    int direction;
    //int curMp;
    int attacker;
};

struct AttackData
{
    int type;
    int damage;
    int curHp;
    int baseHp;
    char name[32];
};

struct NormalEndData
{
    int dismissFlag;
    int time_UP;
};

struct BasePlayerData
{
    char name[32];
    int lvl;
    int atk;
    int def;
    float hitRate;
    
    int baseHp;
    
    int     gameNumber;
    float   winRate;
};

struct WaitingRoomPlayerData
{
    int teamID;
    int playerID;
    char name[32];
    int lvl;
    int gameNumber;
    int winRate;
};

struct UseItemData
{
    int itemID;
    int curHp;
};

////////////////////////////////////////// GWG //////////////////////////////////////////
struct _5SM3nSGWGKickPlayerResponseData
{
    int gameID;
    int remark;
};

struct _5SM3nSGWGDisbandGameResponseData
{
    int remark;
};

struct _5SM3nSGWGLeaveGameResponseData
{
    int remark;
};

struct _5SM3nSGWGStartGameResponseData
{
    int remark;
};

struct _5SM3nSGWGSomeBodyJoinGameData
{
    int gameID;
    int teamID;
    char name[32];
    int remark;
};

struct _5SM3nSGWGSomeBodyLeaveGameData
{
    int gameID;
};

struct _5SM3nSGWGSearchGameServerResponseData
{
    int remark;
};

struct _5SM3nSGWGStopSearchGameServerResponse
{
    int remark;
};

struct _5SM3nSGWGSearchedGameServerInfo
{
    char macAddr[32];
    int  gameType;
    int  gameTimeLength;
    char gameName[32];
    char hostName[32];
    int  redTeamMemberNumber;
    int  whiteTeamMemberNumber;
    int  blueTeamMemberNumber;
    int  remark;
};

struct _5SM3nSGWGFinishedSearchGameServer
{
    int remark;
};

struct _5SM3nSGWGJoinSearchedGameServerResponse
{
    int remark;
};

struct _5SM3nSGWGGameServerTeamInfo
{
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
};

struct _5SM3nSGWGCreateGameResponse
{
    int gameID;
    int remark;
};

struct _5SM3nSGWGHostStartGame
{
    int remark;
};

struct _5SM3nSGWGHostDisbandGame
{
    int remark;
};

struct _5SM3nSGWGLeaveGameWhileBattlingResponseData
{
    int remark;
};

struct _5SM3nSGWGGameServerMessageData
{
    int  message;
    char nickName[32];
    int  remark;
};

struct _5SM3nSGWGGameOverData
{
    int selfStatus;
    int teamStatus;
    int continueWaiting;
};

struct _5SM3nSGWGSomebodyLeaveGameWhileBattling
{
    int  gameID;
    char nickName[32];
};

struct _5SM3nSGWGTeamMemberIDList
{
    int white[8];
    int red[8];
    int blue[8];
};

#endif