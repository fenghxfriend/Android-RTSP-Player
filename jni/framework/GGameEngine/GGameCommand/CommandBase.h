#ifndef __COMMAND_BASE__
#define __COMMAND_BASE__

#include "../NetWork/MsgBase.h"

class CommandBase
{
public:
	CommandBase();
	virtual ~CommandBase();
	virtual const SimpleMsgRTTI* getMsgRTTI() const = 0;
    
    
    virtual CommandBase& operator =(CommandBase& base )
    {
        m_Next = base.m_Next;
        return *this;
    }
    
public:
    CommandBase* m_Next;
};

class CommandFactory
{
public:
	typedef CommandBase*	(*Creator)( void );
	typedef std::map< unsigned int, Creator> MapCreator;
    
	bool isMsgRegCreator( unsigned int id );
	bool RegisterMsg(unsigned int id, Creator fun );
    
	CommandBase* CreateMsg(unsigned int id);
    
	void	Clear( void );
protected:
	MapCreator mMapCreator;
};

extern CommandFactory&	getGlobalCommandFactory( void );

template< class T >
class CommandRegT 
{
public:
	CommandRegT()
	{
		
	}
    
    static int Reg()
    {
        getGlobalCommandFactory().RegisterMsg( T::GetMsgID(), &CommandRegT<T>::createMsg );
        return 0;
    }
    
	static CommandBase*	createMsg( void )
	{
		return static_cast<CommandBase*>(new T);
	}
};

#define DeclareCommand(ID) \
public: \
static int GetMsgID()\
{ \
return ID; \
}\
static const SimpleMsgRTTI scMsgRtti; \
const SimpleMsgRTTI* getMsgRTTI() const {return &scMsgRtti;}

#define RegCommand(className) const SimpleMsgRTTI className::scMsgRtti(className::GetMsgID(), CommandRegT<className>::Reg())

#endif

