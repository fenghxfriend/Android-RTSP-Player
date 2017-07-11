#include "GTasksKiller.h"
#include "../../framework/GEngineEvent/GEngineEvent.h"

GTasksKiller &GTasksKiller::sharedInstance()
{
	static GTasksKiller instance;
	return instance;
}

void GTasksKiller::sendKillCommand(int id)
{
	GEngineEvent::sharedInstance().swallowEvent( EET_TASK_KILLER_CMD_KILL, 0, id );
}

void GTasksKiller::sendKilledMessage(int id)
{
	GEngineEvent::sharedInstance().swallowEvent( EET_TASK_KILLER_MSG_KILLED, 0, id );
}

void GTasksKiller::sendAllKilledMessage()
{
	GEngineEvent::sharedInstance().swallowEvent( EET_TASK_KILLER_MSG_ALL_KILLED, 0, 0 );
}

