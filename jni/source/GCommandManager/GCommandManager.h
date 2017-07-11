#ifndef GCommandManager_h
#define GCommandManager_h

#include <jni.h>
#include <memory>
#include <string>
#include "../GDeviceDetecter/GDeviceDetecter.h"
#include "../GDataConverter/GDataConverter.h"

using namespace std;

#define MUSIC_STREAM_SYSTEM		"StreamingSystemMusic"
#define MUSIC_STREAM_CUSTOM		"StreamingCustomMusic"

class GCommandManager
{
public:
    static GCommandManager &sharedInstance();
    friend void GCommandManager_DeviceDetecter_C(void *data, const char *ip);

public:
    void setStreamingMusicPath(JNIEnv *env, jstring sysMusicPath, jstring customMusicPath);
    const char *getStreamingFilePath(char const *fileName, int *isSystemMusic);
    void controlDevice(bool detect);
    void disconnectWithDevice();
    void customDisconnectWithDevice(JNIEnv *env, jstring reason);
    void submitConfigruation(JNIEnv *env, jstring ssid, bool channelUF, int channel, bool volumeUF, int volume, bool calibrationUF, int calibration);
    void updateConfigruation();
    void forward(int ratio);
    void backward(int ratio);
    void stop();
    void startLeftTurn(int ratio);
    void stopLeftTurn();
    void startRightTurn(int ratio);
    void stopRightTurn();
    void cameraMoveUp();
    void cameraMoveDown();
    void cameraMoveStop();
    void openLeftLED(int on, int off);
    void closeLeftLED();
    void openRightLED(int on, int off);
    void closeRightLED();
    void openTailLED(int on, int off);
    void closeTailLED();
    void openHeadLight();
    void closeHeadLight();
    void openSpeaker(bool response);
    void closeSpeaker(bool response);
    void openSpeaking(JNIEnv *env, jstring ip, bool response);
    void closeSpeaking(bool response);
    void openListening(bool response);
    void closeListening(bool response);
    void playMusic(JNIEnv *env, jstring filename, jstring ip, jint fileType, bool reset, bool response);
    void stopMusic(bool response);
    void updateSpeakerMusic(JNIEnv *env, int type, jstring ip);
    void requestAdvanceConfigruation();
    void GP5SM3nSForward(int stall);
    void GP5SM3nSBackward(int stall);
    void GP5SM3nSTurnLeft(int stall);
    void GP5SM3nSTurnRight(int stall);
    void GP5SM3nSStopMove();
    void GP5SM3nSStopTurn();
    void GP5SM3nSBeginBattle();
    void GP5SM3nSFinishBattle(bool fail);
    void GP5SM3nSFire();
    void GWGKickGamePlayer(int gameId);
    void GWGDisbandGame();
    void GWGLeaveGameWhileWaiting();
    void GWGStartGame();
    void GWGSearchGameServer();
    void GWGStopSearchGameServer();
    void GWGJoinGameServer(JNIEnv *env, jstring macaddr, int team, jstring name);
    void GWGRequestTeamInfo(int team);
    void GWGCreateGame(JNIEnv *env, jstring gamename, jstring nickname, int gametype, int gametime, int gameteam);
    void GWGLeaveGameWhileBattling();
    void GWGRequestTeamMemberIDList();

private:
    void detectedDevice(const char *ip);

private:
    GDeviceDetecter mDeviceDetecter;
    GDataConverter  mDataConverter;
    struct timeval  mStreamMusicBeginDateTime;
    struct timeval  mStreamMusicActiveDateTime;
    struct timeval  mStreamMusicDistanceTime;
    char 			mSystemMusicPath[256];
    char 			mCustomMusicPath[256];
    char			mActiveStreamingFileName[256];
    char			mActiveStreamingFilePath[256];
    bool			mGettedMusicPath;

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//get local IPAddress
//
//add by jarry.jin 2014-04-02
//
/////////////////////////////////////////////////////////////////////////////////////////////////
public:
	int localIPAddress(int index);
};

#endif
