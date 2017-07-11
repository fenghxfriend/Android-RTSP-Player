#include "SessionHost.h"
#include "MsgBase.h"
#include "Session.h"
#include "../CommonInfo/MsgDef.h"
#include "../../TEEEngine/engine/shared/TEE_config.h"

Session::Session()
{
    
}

Session::~Session()
{

}



ClientSession::ClientSession()
{
//    g_Config.m_Debug = 1;
//    dbg_logger_stdout();
    
}
ClientSession::~ClientSession()
{
    
}


int ClientSession::FireMsg( NetMsgBase& msg, int ClientID, int Flags /*= NETSENDFLAG_VITAL | NETSENDFLAG_FLUSH*/ )
{
	m_writeStream.Reset();
	if(msg.Packer(&m_writeStream))
		return -1;

	CNetChunk Packet;
	mem_zero(&Packet, sizeof(CNetChunk));

	Packet.m_ClientID = ClientID;
	Packet.m_pData = m_writeStream.Data();
	Packet.m_DataSize = m_writeStream.Size();

	Packet.m_Flags |= Flags;

	m_NetClient.Send(&Packet);
	return 0;
}

void ClientSession::StartServer(int port, int weeksec, int dropsec, NETFUNC_SIGNALSTRENGTH powerfulCallBack, NETFUNC_SIGNALSTRENGTH weekCallBack)
{
	NETADDR BindAddr;
	mem_zero(&BindAddr, sizeof(BindAddr));
	BindAddr.type = NETTYPE_ALL;
    BindAddr.port = 8301;
	if(!m_NetClient.Open(BindAddr, 0))
	{
		return;
	}
	m_ServerAddress.port = port;
    m_CurState = NETSTATE_OFFLINE;
    
    m_NetClient.setNetSignalStateCallBack(weeksec, dropsec, powerfulCallBack, weekCallBack);
}

int  ClientSession::Connect( NETADDR *pAddress, long tv_usec )
{
	mem_copy(&m_ServerAddress, pAddress, sizeof(NETADDR));
	m_NetClient.Connect(&m_ServerAddress);
	return 0;
}


void ClientSession::Disconnect( const char *pReason )
{
	m_NetClient.Disconnect(pReason);
}


int ClientSession::SendConnlessMsg( NetMsgBase& msg, NETADDR *pAddr )
{
	m_writeStream.Reset();
	if(msg.Packer(&m_writeStream))
		return -1;

	CNetChunk Packet;
	mem_zero(&Packet, sizeof(CNetChunk));

	Packet.m_ClientID = -1;
	if (pAddr == 0)
	{
		Packet.m_Address.type = NETTYPE_IPV4 | NETTYPE_LINK_BROADCAST;
		Packet.m_Address.port = m_ServerAddress.port;
	}
	else
	{
		Packet.m_Address = *pAddr;
	}
	Packet.m_pData = m_writeStream.Data();
	Packet.m_DataSize = m_writeStream.Size();

	Packet.m_Flags = NETSENDFLAG_CONNLESS;

	return m_NetClient.Send(&Packet);
}


//int ClientSession::BroadCastConnlessMsg( NetMsgBase& msg )
//{
//	m_writeStream.Reset();
//	if(msg.Packer(&m_writeStream))
//		return -1;
//
//	CNetChunk Packet;
//	mem_zero(&Packet, sizeof(CNetChunk));
//
//	Packet.m_ClientID = -1;
//	Packet.m_Address.type = NETTYPE_IPV4 | NETTYPE_LINK_BROADCAST;
//	Packet.m_Address.port = 8300;
//	Packet.m_pData = m_writeStream.Data();
//	Packet.m_DataSize = m_writeStream.Size();
//
//	Packet.m_Flags = NETSENDFLAG_CONNLESS;
//
//	return m_NetClient.Send(&Packet);
//}

int ClientSession::SendMsg( NetMsgBase& msg, int flag /*= NETSENDFLAG_VITAL | NETSENDFLAG_FLUSH*/ )
{
	return FireMsg(msg, 0, flag);
}

void ClientSession::Flush()
{
	m_NetClient.Update();
	CNetChunk Packet;
	while(m_NetClient.Recv(&Packet))
	{
		if(Packet.m_ClientID == -1)
			ProcessConnlessPacket(&Packet);
		else
			ProcessServerPacket(&Packet);
	}
    if (m_NetClient.State() != m_CurState)
    {
        if (m_CurState == NETSTATE_ONLINE) {
            ClientDisconnectMsg msg;
            MsgIniter::InitMsgSession(msg, this);
            FireEvent(msg.GetMsgID(), msg);
        }
        m_CurState = m_NetClient.State();
    }
}

void ClientSession::ProcessConnlessPacket( CNetChunk *pPacket )
{
	m_readStream.Reset(pPacket->m_pData, pPacket->m_DataSize);
	int msgID;
	m_readStream.SerializeInt(msgID);
	NetMsgBase* msg = getGlobalMsgFactory().CreateMsg(msgID);

	SessionHost host(pPacket->m_ClientID);
	host.SetNetAddr(pPacket->m_Address);
	MsgIniter::InitMsgHost(*msg, &host);
	msg->Serialize(m_readStream);
    MsgIniter::InitMsgSession(*msg, this);
	FireEvent(msgID, *msg);

	delete msg;
}

void ClientSession::ProcessServerPacket( CNetChunk *pPacket )
{
	m_readStream.Reset(pPacket->m_pData, pPacket->m_DataSize);
	int msgID;
	m_readStream.SerializeInt(msgID);
	NetMsgBase* msg = getGlobalMsgFactory().CreateMsg(msgID);
	msg->Serialize(m_readStream);
	//MapHost::iterator it = m_mapHost.find(pPacket->m_ClientID);
	//if (it != m_mapHost.end())
	//{
	//	MsgIniter::InitMsgHost(*msg, it->second);
	//}
    MsgIniter::InitMsgSession(*msg, this);
	FireEvent(msgID, *msg);
	delete msg;
}


NETSOCKET ClientSession::socket() 
{
    return m_NetClient.Socket();
}

/////////////////////////////////////////////////////////



int ServerSession::FireMsg( NetMsgBase& msg, int ClientID, int Flags /*= NETSENDFLAG_VITAL | NETSENDFLAG_FLUSH*/ )
{
	m_writeStream.Reset();
	if(msg.Packer(&m_writeStream))
		return -1;

	CNetChunk Packet;
	mem_zero(&Packet, sizeof(CNetChunk));

	Packet.m_ClientID = ClientID;
	Packet.m_pData = m_writeStream.Data();
	Packet.m_DataSize = m_writeStream.Size();

	Packet.m_Flags |= Flags;

	if(ClientID == -1)
	{
		// broadcast
		MapHost::iterator it = m_mapHost.begin();
		for ( ; it != m_mapHost.end() ; ++it)
		{
			Packet.m_ClientID = it->second->GetID();
			m_NetServer.Send(&Packet);
		}
	}
	else
		m_NetServer.Send(&Packet);
	return 0;
}

int ServerSession::SendConnlessMsg( NetMsgBase& msg, NETADDR *pAddr )
{
	m_writeStream.Reset();
	if(msg.Packer(&m_writeStream))
		return -1;

	CNetChunk Packet;
	mem_zero(&Packet, sizeof(CNetChunk));

	Packet.m_ClientID = -1;
	//if (pAddr == 0)
	//{
	//	Packet.m_Address.type = NETTYPE_IPV4 | NETTYPE_LINK_BROADCAST;
	//	Packet.m_Address.port = m_ServerAddress.port;
	//}
	//else
	//{
		Packet.m_Address = *pAddr;
	//}
	Packet.m_pData = m_writeStream.Data();
	Packet.m_DataSize = m_writeStream.Size();

	Packet.m_Flags = NETSENDFLAG_CONNLESS;

	return m_NetServer.Send(&Packet);
}

ServerSession::ServerSession()
{
	
}

ServerSession::~ServerSession()
{
	MapHost::iterator it = m_mapHost.begin();
	for ( ; it != m_mapHost.end() ; ++it)
	{
		delete it->second;
	}
	m_mapHost.clear();
}



void ServerSession::StartServer(int port, int maxClients)
{
	NETADDR BindAddr;
	mem_zero(&BindAddr, sizeof(BindAddr));
	BindAddr.port = port;
	BindAddr.type = NETTYPE_IPV4;
	//net_init();
	if(!m_NetServer.Open(BindAddr, maxClients, 16, 0))
	{
		return;
	}

	m_NetServer.SetCallbacks(OnNewClientCallback, OnDelClientCallback, this);
}

int ServerSession::OnNewClientCallback( int ClientID, void *pUser )
{
	reinterpret_cast<ServerSession*>(pUser)->NewClientCall(ClientID);
	return 0;
}

int ServerSession::OnDelClientCallback( int ClientID, const char *pReason, void *pUser )
{
	reinterpret_cast<ServerSession*>(pUser)->DelClientCall(ClientID);
	return 0;
}

void ServerSession::Flush()
{
	m_NetServer.Update();
	CNetChunk Packet;
	while(m_NetServer.Recv(&Packet))
	{
		if(Packet.m_ClientID == -1)
			ProcessConnlessPacket(&Packet);
		else
			ProcessClientPacket(&Packet);
	}
}

void ServerSession::ProcessConnlessPacket( CNetChunk *pPacket )
{
	m_readStream.Reset(pPacket->m_pData, pPacket->m_DataSize);
	int msgID;
	m_readStream.SerializeInt(msgID);
	NetMsgBase* msg = getGlobalMsgFactory().CreateMsg(msgID);

	MapHost::iterator it = m_mapHost.find(pPacket->m_ClientID);
	if (it != m_mapHost.end())
	{
		MsgIniter::InitMsgHost(*msg, it->second);
		msg->Serialize(m_readStream);
        MsgIniter::InitMsgSession(*msg, this);
		FireEvent(msgID, *msg);
	}
	else
	{
		SessionHost host(pPacket->m_ClientID);
		host.SetNetAddr(pPacket->m_Address);
		MsgIniter::InitMsgHost(*msg, &host);
		msg->Serialize(m_readStream);
        MsgIniter::InitMsgSession(*msg, this);
		FireEvent(msgID, *msg);
	}
	delete msg;
}

void ServerSession::ProcessClientPacket( CNetChunk *pPacket )
{
	m_readStream.Reset(pPacket->m_pData, pPacket->m_DataSize);
	int msgID;
	m_readStream.SerializeInt(msgID);
	NetMsgBase* msg = getGlobalMsgFactory().CreateMsg(msgID);

	MapHost::iterator it = m_mapHost.find(pPacket->m_ClientID);
	if (it != m_mapHost.end())
	{
		MsgIniter::InitMsgHost(*msg, it->second);
	}

	msg->Serialize(m_readStream);
    MsgIniter::InitMsgSession(*msg, this);
	FireEvent(msgID, *msg);
	delete msg;
}

void ServerSession::Drop( int ClientID, const char *pReason )
{
	m_NetServer.Drop(ClientID, pReason);
}

ServerSession::MapHost* ServerSession::GetHostList()
{
    return &m_mapHost;
}

SessionHost* ServerSession::GetHost(unsigned int hostID)
{
    MapHost::iterator it = m_mapHost.find(hostID);
    if (it != m_mapHost.end()) {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

int ServerSession::SendMsg( NetMsgBase& msg, int clientID, int flag /*= NETSENDFLAG_VITAL | NETSENDFLAG_FLUSH*/ )
{
	return FireMsg(msg, clientID, flag);
}

int ServerSession::BroadCastMsg( NetMsgBase& msg )
{
	return FireMsg(msg, -1);
}

void ServerSession::NewClientCall( int clientID )
{
	SessionHost* host = new SessionHost(clientID);
	m_mapHost.insert(MapHost::value_type(clientID,host));
	ClientConnectMsg msg;
	MsgIniter::InitMsgHost(msg, host);
    MsgIniter::InitMsgSession(msg, this);
	FireEvent(msg.GetMsgID(), msg);
}

void ServerSession::DelClientCall( int clientID )
{
	MapHost::iterator it = m_mapHost.find(clientID);
	if (it != m_mapHost.end())
	{
        SessionHost* host = it->second;
        m_mapHost.erase(it);
		ClientDisconnectMsg msg;
		MsgIniter::InitMsgHost(msg, host);
        MsgIniter::InitMsgSession(msg, this);
		FireEvent(msg.GetMsgID(), msg);
        
		delete host;
		
	}

}




