#include "SessionHost.h"
#include "MsgStream.h"
#include "MsgBase.h"
#include "../../TEEEngine/engine/TEE_message.h"
#include "Session.h"

bool NetMsgBase::Packer( MsgStream* stream )
{
	int id = getMsgRTTI()->GetMsgID();
	stream->SerializeInt(id);
	Serialize(*stream);
	return stream->Error();
}

void NetMsgBase::Serialize( MsgStream& stream )
{

}

NetMsgBase::NetMsgBase()
{
	m_host = 0;
}

NetMsgBase::~NetMsgBase()
{

}

SessionHost* NetMsgBase::GetHost() const
{
	return m_host;
}

Session* NetMsgBase::GetSessin() const
{
	return m_Session;
}


bool MsgFactory::isMsgRegCreator( unsigned int id )
{
	MapCreator::iterator mit = mMapCreator.find( id );
	return ( mit != mMapCreator.end() );
}

bool MsgFactory::RegisterMsg( unsigned int id, Creator fun )
{
	if( isMsgRegCreator( id ))
	{
		return false;
	}
	mMapCreator[ id ] = fun;
	return true;
}

NetMsgBase* MsgFactory::CreateMsg( unsigned int id )
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

void MsgFactory::Clear( void )
{
	mMapCreator.clear();
}

extern MsgFactory& getGlobalMsgFactory( void )
{
	static MsgFactory staticFactory;
	return staticFactory;
}



SimpleMsgRTTI::~SimpleMsgRTTI()
{

}

int SimpleMsgRTTI::GetMsgID() const
{
	return m_msgID;
}
