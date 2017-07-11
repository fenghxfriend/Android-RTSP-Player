#ifndef GEventManager_h
#define GEventManager_h

#include <jni.h>
#include <memory>
#include <string>
#include "GEventConverter.h"

using namespace std;

class GEventManager
{
public:
    static  GEventManager &sharedInstance();
    int     eventRunLoop(JNIEnv *env, jobject javaEventData);

private:
    GEventData 		mEventData;
    GEventConverter mEventConverter;
};

#endif
