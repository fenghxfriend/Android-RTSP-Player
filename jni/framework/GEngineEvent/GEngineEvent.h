#ifndef GEngineEvent_h
#define GEngineEvent_h

#include <memory>
#include <string>
#include "GEngineEventDefine.h"

using namespace std;

class GEngineEvent
{
public:
    static  GEngineEvent &sharedInstance();
    void    swallowEvent(GEngineEventType type, void *data, int size);
};

#endif
