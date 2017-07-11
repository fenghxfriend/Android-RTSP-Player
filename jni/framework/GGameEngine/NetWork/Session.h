#ifndef __CLIENT_SESSION__
#define __CLIENT_SESSION__

#include <map>
#include <vector>
#include "../Event/ListenEvent.h"
#include "MsgStream.h"
#include "../../TEEEngine/base/TEE_system.h"
#include "../../TEEEngine/engine/shared/TEE_network.h"

class NetMsgBase;
class SessionHost;

class Session : public Listener<unsigned int, NetMsgBase&>
{
public:
	Session();
	virtual ~Session();

	virtual void Flush() = 0;

protected:
	MsgReadStream	m_readStream;
	MsgWriteStream	m_writeStream;

};

class ClientSession : public Session
{
public:
    ClientSession();
    ~ClientSession();
	void StartServer(int port, int weeksec, int dropsec, NETFUNC_SIGNALSTRENGTH powerfulCallBack, NETFUNC_SIGNALSTRENGTH weekCallBack);
	int Connect( NETADDR *pAddress, long tv_usec = 5000);
	void Disconnect(const char *pReason);

	int SendConnlessMsg(NetMsgBase& msg, NETADDR *pAddr = 0);
	//int BroadCastConnlessMsg(NetMsgBase& msg);

	int SendMsg(NetMsgBase& msg, int flag = NETSENDFLAG_VITAL | NETSENDFLAG_FLUSH);

	void Flush();

	void ProcessConnlessPacket(CNetChunk *pPacket);

	void ProcessServerPacket(CNetChunk *pPacket);
    
    NETSOCKET socket() ;
private:
	int FireMsg(NetMsgBase& msg, int ClientID, int Flags = NETSENDFLAG_VITAL | NETSENDFLAG_FLUSH);
protected:
	NETADDR    m_ServerAddress;
    int         m_CurState;
	CNetClient m_NetClient;

};

class ServerSession : public Session
{
public:
	typedef std::map<unsigned int, SessionHost*>	MapHost;
public:
	ServerSession();
	virtual ~ServerSession();

	void StartServer( int port, int maxClients);

	static int OnNewClientCallback(int ClientID, void* pUser);

	static int OnDelClientCallback(int ClientID, const char* pReason, void* pUser);
	void Flush();

	void ProcessConnlessPacket(CNetChunk *pPacket);

	void ProcessClientPacket(CNetChunk *pPacket);

	void Drop(int ClientID, const char *pReason);

	int SendMsg(NetMsgBase& msg, int clientID, int flag = NETSENDFLAG_VITAL | NETSENDFLAG_FLUSH);

	int BroadCastMsg(NetMsgBase& msg);

	int SendConnlessMsg( NetMsgBase& msg, NETADDR *pAddr );
    
    MapHost* GetHostList();
    
    SessionHost* GetHost(unsigned int hostID);

private:
	void NewClientCall(int clientID);
	void DelClientCall(int clientID);

	int FireMsg(NetMsgBase& msg, int ClientID, int Flags = NETSENDFLAG_VITAL | NETSENDFLAG_FLUSH);
	
	//	virtual int RealSendMessage(CMsgPacker *pMsg, int Flags, int ClientID) = 0;


protected:
	CNetServer		m_NetServer;
	MapHost			m_mapHost;
};

template<class T> 
struct MsgHandlerInfo
{
	MsgHandlerInfo( void ) :mMsgCode( NULL )
		,mMsgId( NULL ),
		mRouter( NULL )
	{

	}
	typedef std::vector< MsgHandlerInfo> VEC_HANDLE_INFO;
	static void	UnregSession(  T* pSess,  VEC_HANDLE_INFO& __vector_nw_msg_handler_info )
	{
		if ( pSess == NULL )
		{
			return;
		}
		typename VEC_HANDLE_INFO::iterator vit =								
			__vector_nw_msg_handler_info.begin();					
		for( ; vit != __vector_nw_msg_handler_info.end(); )		
		{														
			const MsgHandlerInfo& kInfo = *vit;					
			if( kInfo.mRouter == pSess )							
			{	
				pSess ->UnregListerner(								
					kInfo.mMsgId, kInfo.mMsgCode );					
				vit = __vector_nw_msg_handler_info.erase( vit );
			}													
			else												
			{	
				++vit;												
			}													
		}
	}
	Session::ListenerCode		mMsgCode;
	unsigned int					mMsgId;
	T*					mRouter;
};

struct MsgHandleLevel
{
	enum 
	{
		MHL_SUPER,
		MHL_PRIOR,
		MHL_STANDARD,
		MHL_LOW,
		MHL_STATISTIC,
		MHL_MAX
	};
};

#define DeclareMsgHandler()					\
	public:														\
	void	RegMsgHandler( Session* pSess );	\
	void	UnRegMsgHandler( Session* pSess );	\
	MsgHandlerInfo<Session>::VEC_HANDLE_INFO __vector_nw_msg_handler_info;

#define ImplMsgHandlerBegin( theClass )						\
	void	theClass::RegMsgHandler(Session* pSess )\
{	typedef theClass TheClass;									\
	UnRegMsgHandler( pSess );									\
	MsgHandlerInfo<Session> info;									\
	//const char* theClassName = #theClass;		


#define DefineMsgHandler( handleLv, msgType, eventFun )		\
	info.mRouter = pSess;									\
	info.mMsgId = msgType::GetMsgID();			\
	info.mMsgCode = pSess ->RegListerner(						\
	handleLv, info.mMsgId, this, &TheClass::eventFun );		\
	__vector_nw_msg_handler_info.push_back(info);
#define ImplMsgHandlerEnd(Class) \
} \
	void Class::UnRegMsgHandler(Session* session) \
{ \
	MsgHandlerInfo<Session>::UnregSession(session, __vector_nw_msg_handler_info); \
}

#endif