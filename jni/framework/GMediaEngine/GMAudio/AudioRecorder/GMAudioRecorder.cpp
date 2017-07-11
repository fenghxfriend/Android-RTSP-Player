#include "GMAudioRecorder.h"
#include "../../../../source/GAudioManager/GAudioManager.h"

void GMAudioRecorder::install()
{

}

void GMAudioRecorder::start()
{
    
}

int GMAudioRecorder::read(void *buffer, int maxSize)
{
	return GAudioManager::sharedInstance().readStream( buffer, maxSize );
}

void GMAudioRecorder::stop()
{
    
}

void GMAudioRecorder::unInstall()
{

}
