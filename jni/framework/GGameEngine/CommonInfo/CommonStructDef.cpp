#include "CommonStructDef.h"
#include "MsgDef.h"

void TankPlayerData::UpdateHp(int hpVariable)
{
    hp += hpVariable;
    PlayerUpdateHpMsg msg;
    msg.playerID = playerID;
    msg.hp = hpVariable;
    std::map<int, TankPlayerData*>::iterator it = observers.begin();
    for (; it != observers.end(); ++it) {
        mLogicSession->SendMsg(msg, it->first);
    }
}

void TankPlayerData::DecideIsDead()
{
    if (hp <= 0)
    {
        deadFlag = 1;
        PlayerDeadMsg msg;
        msg.playerID = playerID;
        mLogicSession->BroadCastMsg(msg);
    }
}

void TankPlayerData::BreakCurState(int reason)
{
    PlayerStopUseItem msg;
    msg.playerID = playerID;
    msg.reason = reason;
    std::map<int, TankPlayerData*>::iterator it = observers.begin();
    for (; it != observers.end(); ++it) {
        msg.playerID = playerID;
        mLogicSession->SendMsg(msg, it->first);
    }
}
