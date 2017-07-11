#include "CommandBase.h"

bool CommandFactory::isMsgRegCreator( unsigned int id )
{
	MapCreator::iterator mit = mMapCreator.find( id );
	return ( mit != mMapCreator.end() );
}

bool CommandFactory::RegisterMsg( unsigned int id, Creator fun )
{
	if( isMsgRegCreator( id ))
	{
		return false;
	}
	mMapCreator[ id ] = fun;
	return true;
}

CommandBase* CommandFactory::CreateMsg( unsigned int id )
{
	MapCreator::iterator mit = mMapCreator.find( id );
	if( mit != mMapCreator.end() )
	{
		return (*mit ->second)();
	}
	else
	{
		return NULL;
	}
}

void CommandFactory::Clear( void )
{
	mMapCreator.clear();
}

extern CommandFactory& getGlobalCommandFactory()
{
	static CommandFactory staticFactory;
	return staticFactory;
}

CommandBase::CommandBase()
{
    m_Next = NULL;
}

CommandBase::~CommandBase()
{
    
}
