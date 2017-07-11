#ifndef GAudioIOEngine_h
#define GAudioIOEngine_h

#include <jni.h>
#include <memory>
#include <string>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

using namespace std;

class GAudioIOEngine
{
public:
	void 		install();
	void 		unInstall();
	SLEngineItf	engine();
	SLObjectItf	mixOutput();

private:
	void createEngine();
	void destoryEngine();

private:
	SLObjectItf 					mEngineObject;
	SLEngineItf 					mEngineInterface;
	SLObjectItf 					mMixOutputObject;
	SLEnvironmentalReverbItf		mMixOutputInterface;
	bool							mChangedEnvSetting;
	SLEnvironmentalReverbSettings	mOrigEnvSetting;
};

#endif
