#include "GAudioStreamRecorder.h"
#include "../GAudioEngine/GAudioEngine.h"

void GAudioStreamRecorder::install	() { };
void GAudioStreamRecorder::unInstall() { };

void GAudioStreamRecorder::start()
{
	//Æô¶¯Â¼ÒôÆ÷
	bool result = GAudioEngine::sharedInstance().startRecord();
}

int GAudioStreamRecorder::read(void *buffer, int maxSize)
{
	//¶ÁÈ¡Êý¾Ý
	return GAudioEngine::sharedInstance().readRecordedData( buffer, maxSize );
}

void GAudioStreamRecorder::stop()
{
	//Í£Ö¹Â¼ÒôÆ÷
	GAudioEngine::sharedInstance().stopRecord();
}

