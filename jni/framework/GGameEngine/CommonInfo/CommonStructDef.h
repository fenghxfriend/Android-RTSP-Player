#ifndef __COMMON_STRUCT_DEF__
#define __COMMON_STRUCT_DEF__

#include "../../TEEEngine/base/TEE_system.h"
#include "../NetWork/SessionHost.h"
#include "../NetWork/Session.h"
#include <map>

#define OFFLINE_PLAYER_DATA_CACHE_TIME  60
#define ATTACK_LOCK_TIME 0.5
#define TARGET_LOCK_TIME 15

struct GameRoomInfo
{
	char    name[32];
    char    password[32];
    int     isNeedPassword;
    
    int     teamNumber;
    int     warType;
    int     maxPlayerNumber;
    int     warTime;
    
	int     serverState;
    
    int     curPlayerNumber;
    
    int     roomID;
    
    void Serialize( MsgStream& stream )
    {
        stream.SerializeString(name);
        stream.SerializeString(password);
        stream.SerializeInt(isNeedPassword);
        stream.SerializeInt(teamNumber);
        stream.SerializeInt(warType);
        stream.SerializeInt(maxPlayerNumber);
        stream.SerializeInt(warTime);
        stream.SerializeInt(serverState);
        stream.SerializeInt(curPlayerNumber);
        stream.SerializeInt(roomID);
    }
};

class SkillBase
{
public:
    SkillBase()
    {
        
    }
    virtual ~SkillBase()
    {
        
    }
    virtual int update(float time) = 0;
};

class HaloSkill : public SkillBase
{
public:
    HaloSkill()
    {
        
    }
    virtual ~HaloSkill()
    {
        
    }
    virtual int update(float time)
    {
        return 1;
    }
};


class StateSkill : public SkillBase
{
public:
    StateSkill(float maxTime)
    {
        mLastTime = maxTime;
    }
    virtual ~StateSkill()
    {
        
    }
    
    
    virtual float GetStateMaxTime() = 0;
    
    virtual int update(float time)
    {
        mLastTime -= time;
        if (mLastTime > 0) {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    
protected: 
    float mLastTime;
};


enum Enum_Player_State
{
    Enum_Player_State_Normal,
    Enum_Player_State_ADDHp,
};
class TankPlayerData : public ExtraData
{
public:
    TankPlayerData(ServerSession* session) : ExtraData("tankdata")
    {
        mLogicSession = session;
        deadFlag = 0;
        cacheTime = 0;
        masterFlag = 0;
        curState = 0;
        InitAttack();
    }
    
    virtual ~TankPlayerData()
    {
    }
    
    void Release()
    {
        observers.clear();
        target->UpdateObserver(this, 1);
        delete this;
    }
    
    void GameBeginInit()
    {
        hp = baseHp;
        deadFlag = 0;
        leaveGameFlag = 0;
        observers.clear();
        observers[playerID] = this;
    }
    
    void InitAttack()
    {
        target = 0;
        attLockTime = 0;
        targetLockTime = 0;
    }
    
    void AttackTo(TankPlayerData* beAttacker)
    {
        attLockTime = ATTACK_LOCK_TIME;
        target = beAttacker;
        targetLockTime = TARGET_LOCK_TIME;
        beAttacker->UpdateObserver(this);
    }
    
    void UpdateObserver(TankPlayerData* beAttacker, int isLeave = 0)
    {
        if (isLeave == 1) {
            //observers.erase(beAttacker->playerID);
        }
        else
        {
            std::map<int, TankPlayerData*>::iterator it = observers.find(beAttacker->playerID);
            if (it == observers.end()) {
                observers[beAttacker->playerID] = beAttacker;
            }
        }
    }
    
    void UpdateHp(int hpVariable);
    
    void DecideIsDead();
    
    void BreakCurState(int reason);
    
    int Update( float time)
    {
        if (targetLockTime > 0) {
            attLockTime -= time;
            targetLockTime -= time;
            if (targetLockTime <= 0) {
                target = 0;
                return 1;
            }
        }
        if (curState) {
            int ret = curState->update(time);
            if (ret == 0) {
                BreakCurState(0);
                delete curState;
                curState = 0;
            }
        }
        return 0;
    }
    int     teamID;
    int     playerID;
    char    name[32];
    int     playerLvl;
    int     gameNumber;
    float   winRate;
    
    int     lvl;
    int     atk;
    int     def;
    float   hitRate;
    
    //    float   escapeRate;
    int     baseHp;
    int      hp;
    
    
    int     masterFlag;
    int     deadFlag;
    
    TankPlayerData*     target;
    std::map<int, TankPlayerData*> observers;
    float   targetLockTime;
    float   attLockTime;
    
    float   cacheTime;
    
    char    macAddress[32];
    int leaveGameFlag;

    ServerSession* mLogicSession;
    
    StateSkill*     curState;
};


class BloodBottleSkill : public StateSkill
{
public:
    BloodBottleSkill(int hpVar,float maxSkillTime) :     StateSkill(maxSkillTime)
    {
        mLoopTime = 0.2f;
        elapsedTime = 0;
        elapsedTimeCounter = 0;
        elapsedHpCounter = 0;
        mMaxHp  = hpVar;
        mMaxSkillTime = maxSkillTime;
    }
    virtual ~BloodBottleSkill()
    {
        
    }

    virtual float GetStateMaxTime()
    {
        return mMaxSkillTime;
    }
    
    void SetOwner(TankPlayerData* player)
    {
        mOwner = player;
    }
    
    virtual int update(float time)
    {
        elapsedTime += time;
        while(elapsedTime >= mLoopTime) {
            elapsedTime -= mLoopTime;
            elapsedTimeCounter += mLoopTime;
            int hpVariable = elapsedTimeCounter * mMaxHp / mMaxSkillTime - elapsedHpCounter;
            elapsedHpCounter += hpVariable;
            mOwner->UpdateHp(hpVariable);
        }
        return StateSkill::update(time);
    }
private:
    TankPlayerData* mOwner;
    int   mMaxHp;
    float mLoopTime;
    float elapsedTime;
    float mMaxSkillTime;
    
    float elapsedTimeCounter;
    int   elapsedHpCounter;
};

#define Server_Name_Max 32

#endif