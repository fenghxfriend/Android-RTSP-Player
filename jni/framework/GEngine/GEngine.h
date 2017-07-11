#ifndef GEngine_h
#define GEngine_h

#include <memory>
#include <string>

#include "GEventQueue.h"

using namespace std;

class GEngine
{
public:
    static  GEngine &sharedInstance();
    void    install();
    void    unInstall();
    void    reset();
    bool    eventRunLoop(GEventData *data);
    void    swallowNetMessage(GGameNetMessage message);
    void    swallowEngineEvent(GEngineEventData event);
    
private:
    GEventQueue     mEventQueue;
    GEventData      mNewEvent;
    int				mEventDataSize;
};

#endif
