#ifndef GEventQueue_h
#define GEventQueue_h

#include <memory>
#include <string>
#include "../GGameEngine/GGameClient/GGameClientMessage.h"
#include "../GGameEngine/GGameUtility/GGameUtility.h"
#include "../GEngineEvent/GEngineEvent.h"

using namespace std;

typedef enum
{
    GEVT_NETMSG	= 0x1 << 0,
    GEVT_ENGEVT	= 0x1 << 1,
} GEvent;

typedef struct
{
    GEvent          	event;
    GGameNetMessage 	netMessage;
    GEngineEventData 	engineEvent;
} GEventData;

class GEventQueue
{
public:
	void write(char *data  , int size   );
	int  read (char *buffer, int maxSize);
	void clear();

public:
	GEventQueue();
	GEventQueue(int capacity);
    ~GEventQueue();
    
private:
    void write0(char *data, int size, bool lock);
    void init(int capacity);
    void destory();
    void expandCapacity();
    
private:
    GGameUtility    mUtility;
    GGameLock       mLock;
    int				mDataBytes;
    int				mDataPtrOffset;
    char			*mBufferPtr;
    int				mBufferCapacity;
    int				mBufferExpandMargin;
};

#endif






