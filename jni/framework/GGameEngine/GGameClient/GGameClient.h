#ifndef GGameClient_h
#define GGameClient_h

#include "../GGameUtility/GGameUtility.h"
#include "GGameClientMessage.h"

class GGameClient
{
public:
    static  GGameClient& sharedInstance();
    void    install();
    void    unInstall();
    bool    messageRunLoop();
    void    swallowMessage(int msgID, void* data, int size);
    friend void GGameClient_ModuleRunLoop_C(void *data);
    
private:    
    void runLoop();
    void unpackMessage(int msgID, void* data, int size);

private:
    GGameUtility        mUtility;
    GGameLock           mSourceLock;
    GGameLock           mLiveLock;
    GGameThread         mModuleHandler;
//    GGameNetMessage     mActiveGameNetMessage;
};

#endif




























