#ifndef GMVideo_h
#define GMVideo_h

#ifdef __cplusplus
extern "C"
{
#endif

	void GMBindModuleEvent( void * onEnter, void * onExit, void * delegate );
    void GMVideoStart	( const char *address, const char *username, const char *password, void *onEvent,  void *on555Event, void *delegate );
    void GMVideoStop 	( void *onEvent, void *delegate );

#ifdef __cplusplus
}
#endif

#endif
