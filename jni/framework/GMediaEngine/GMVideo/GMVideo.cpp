#include "GMVideo.h"
#include "../GMUtility/GMUtility.h"
#include "Controller/GMVideoController.h"

void GMVideoStart	( const char *address, const char *username, const char *password, void *onVideoEvent,  void *on555Event, void *delegate )
{
    GMVideoController::sharedInstance()->start( address, username, password, ( GMFUN_MEDIA_EVENT )onVideoEvent, (GMFUN_555_EVENT)on555Event, delegate );
}

void GMVideoStop 	( void *onVideoEvent, void *delegate )
{
    GMVideoController::sharedInstance()->stop( ( GMFUN_MEDIA_EVENT )onVideoEvent, delegate );
}

void GMBindModuleEvent( void * onEnter, void * onExit, void * delegate )
{
	GMVideoController::sharedInstance()->bindModuleEvent( ( GMFUN_MODULE_EVENT )onEnter, ( GMFUN_MODULE_EVENT )onExit, delegate );
}
