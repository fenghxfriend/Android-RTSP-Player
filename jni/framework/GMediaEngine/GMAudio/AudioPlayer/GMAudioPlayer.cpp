#include "GMAudioPlayer.h"
#include "../../../../source/GAudioManager/GAudioManager.h"

void GMAudioPlayer::install()
{

}

void GMAudioPlayer::start()
{
    
}

void GMAudioPlayer::feedData(void *data, unsigned size)
{
	GAudioManager::sharedInstance().writeStream( data, size );
}

void GMAudioPlayer::stop()
{
    
}

void GMAudioPlayer::unInstall()
{

}
