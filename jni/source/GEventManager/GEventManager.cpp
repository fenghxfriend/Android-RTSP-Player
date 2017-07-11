#include "GEventManager.h"

GEventManager &GEventManager::sharedInstance()
{
	static GEventManager instance;
	return instance;
}

int GEventManager::eventRunLoop(JNIEnv *env, jobject javaEventData)
{
	int result = -1;

	do
	{
		if ( !GEngine::sharedInstance().eventRunLoop( &mEventData ) ) break;
		if ( mEventConverter.convert( env, javaEventData, mEventData ) ) result = 0;

	} while(0);

	return result;
}
