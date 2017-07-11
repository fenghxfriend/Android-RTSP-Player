#ifndef __MESSAGE_DEF__
#define __MESSAGE_DEF__

#include <map>

class MsgStream;
class MsgIniter;
class SessionHost;
class Session;
class SimpleMsgRTTI
{
public:
	SimpleMsgRTTI(int id, ... )
    {
        m_msgID = id;
    }
	~SimpleMsgRTTI();
	
	int GetMsgID() const;
private:
	int m_msgID;
};

class NetMsgBase
{
	friend class MsgIniter;
public:
	NetMsgBase();
	virtual ~NetMsgBase();
	//virtual int GetMsgSize()	= NULL;
	SessionHost* GetHost() const;
    Session*     GetSessin() const;
	virtual const SimpleMsgRTTI* getMsgRTTI() const = 0;
	//virtual int GetMsgID()		= 0;
	bool Packer(MsgStream* stream);
	virtual void Serialize(MsgStream& stream);

protected:
    Session*   m_Session; 
	SessionHost* m_host;
};

class MsgIniter
{
public:
	static void InitMsgHost( NetMsgBase& msg, SessionHost* host )
	{
		msg.m_host = host;
	}
    
    static void InitMsgSession( NetMsgBase& msg, Session* session )
	{
		msg.m_Session = session;
	}
};

class MsgFactory
{
public:
	typedef NetMsgBase*	(*Creator)( void );
	typedef std::map< unsigned int, Creator> MapCreator;

	bool isMsgRegCreator( unsigned int id );
	bool RegisterMsg(unsigned int id, Creator fun );

	NetMsgBase* CreateMsg(unsigned int id);

	void	Clear( void );
protected:
	MapCreator mMapCreator;
};

extern MsgFactory&	getGlobalMsgFactory( void );

template< class T >
class MsgRegT 
{
public:
	MsgRegT()
	{
		
	}
    
    static int Reg()
    {
        getGlobalMsgFactory().RegisterMsg( T::GetMsgID(), &MsgRegT<T>::createMsg );
        return 0;
    }

	static NetMsgBase*	createMsg( void )
	{
		return static_cast<NetMsgBase*>(new T);
	}
};


#define DeclareMessage(ID) \
	public: \
	static int GetMsgID()\
	{ \
		return ID; \
	}\
	static const SimpleMsgRTTI scMsgRtti; \
	const SimpleMsgRTTI* getMsgRTTI() const {return &scMsgRtti;}

#define RegMessage(className) const SimpleMsgRTTI className::scMsgRtti(className::GetMsgID(), MsgRegT<className>::Reg())
#endif
