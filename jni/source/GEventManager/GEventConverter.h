#ifndef GEventConverter_h
#define GEventConverter_h

#include <memory>
#include <string>
#include "../../framework/GEngine/GEngine.h"
#include "../GDataConverter/GDataConverter.h"

using namespace std;

class GEventConverter
{
public:
	bool convert(JNIEnv *env, jobject javaEventData, GEventData &cEventData);

private:
	GDataConverter mDataConverter;
};

#endif
