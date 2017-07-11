#include "GCameraManager.h"
#include "../GCommandManager/GCommandManager.h"

GCameraManager &GCameraManager::sharedInstance()
{
	static GCameraManager instance;
	return instance;
}

void GCameraManager::install()
{
	gettimeofday( &mActiveDateTime, 0 );
	mLastActionDateTime    = mActiveDateTime;
	mLastTakePhotoDateTime = mActiveDateTime;
}

int  GCameraManager::moveUp()
{
	if ( isActionValid() )
	{
		GCommandManager::sharedInstance().cameraMoveUp();
		return 0;
	}

	return -1;
}

int	 GCameraManager::moveDown()
{
	if ( isActionValid() )
	{
		GCommandManager::sharedInstance().cameraMoveDown();
		return 0;
	}

	return -1;
}

void GCameraManager::stop()
{
	GCommandManager::sharedInstance().cameraMoveStop();
}

void GCameraManager::unInstall()
{

}

bool GCameraManager::isActionValid()
{
	bool result = false;

	gettimeofday( &mActiveDateTime, 0 );

	if ( ( ( mActiveDateTime.tv_sec - mLastActionDateTime.tv_sec ) * 1000000
			+ ( mActiveDateTime.tv_usec - mLastActionDateTime.tv_usec ) ) >= GCameraManagerActionInterval )
	{
		mLastActionDateTime = mActiveDateTime;
		result = true;
	}

	return result;
}

int GCameraManager::currentCanTakePhoto()
{
	int result = -1;

	gettimeofday( &mActiveDateTime, 0 );

	if ( ( ( mActiveDateTime.tv_sec - mLastTakePhotoDateTime.tv_sec ) * 1000000
			+ ( mActiveDateTime.tv_usec - mLastTakePhotoDateTime.tv_usec ) ) >= GCameraManagerTakePhotoInterval )
	{
		mLastTakePhotoDateTime = mActiveDateTime;
		result = 0;
	}

	return result;
}

