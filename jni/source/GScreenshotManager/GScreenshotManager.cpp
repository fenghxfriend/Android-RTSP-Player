#include "GScreenshotManager.h"

GScreenshotManager &GScreenshotManager::sharedInstance()
{
	static GScreenshotManager instance;
	return instance;
}

GScreenshotManager::GScreenshotManager()
{
	gettimeofday( &mActionDateTime, 0 );
	mActiveDateTime = mActionDateTime;
}

GScreenshotManager::~GScreenshotManager()
{

}

void GScreenshotManager::start()
{
	//记录开始时间
	gettimeofday( &mActionDateTime, 0 );
}

int  GScreenshotManager::stop()
{
	int result = -1;

	//取得当前时间
	gettimeofday( &mActiveDateTime, 0 );

	//比较时间差是否满足
	if ( ( ( mActiveDateTime.tv_sec - mActionDateTime.tv_sec ) * 1000000
			+ ( mActiveDateTime.tv_usec - mActionDateTime.tv_usec ) ) >= GScreenshotManagerActionInterval )
	{
		mActionDateTime = mActiveDateTime;
		result = 0;
	}

	return result;
}
