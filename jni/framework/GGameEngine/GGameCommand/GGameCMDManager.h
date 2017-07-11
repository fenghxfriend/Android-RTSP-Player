#ifndef GGameCMDManager_h
#define GGameCMDManager_h

#include <list>
#include "../Event/Delegate.h"
#include "../NetWork/Session.h"
#include "../GGameUtility/GGameUtility.h"

class CommandBase;
class GGameCMDManager : public Listener< unsigned int, CommandBase& >
{
public:
    static  GGameCMDManager &sharedInstance();
    void    install();
    void    unInstall();
    void    sendCommand(CommandBase &command);
    void    flush(bool all = false);
    void    clear();
    
private:
    CommandBase     *m_First_Command;
    CommandBase     *m_Last_Command;
    GGameUtility    mUtility;
    GGameLock       mSourceLock;
    CommandBase     *mNewCommand;
    CommandBase     *mActiveCommand;
};

#define DeclareCommandHandler()					\
public:														\
void	RegCommandHandler( GGameCMDManager* pSess );	\
void	UnRegCommandHandler( GGameCMDManager* pSess );	\
MsgHandlerInfo<GGameCMDManager>::VEC_HANDLE_INFO __vector_nw_Command_handler_info;



#define ImplCommandHandlerBegin( theClass )						\
void	theClass::RegCommandHandler(GGameCMDManager* pSess )\
{	typedef theClass TheClass;									\
UnRegCommandHandler( pSess );									\
MsgHandlerInfo<GGameCMDManager> info;

    
    
    
#define DefineCommandHandler( handleLv, msgType, eventFun )		\
info.mRouter = pSess;									\
info.mMsgId = msgType::GetMsgID();			\
info.mMsgCode = pSess ->RegListerner(						\
handleLv, info.mMsgId, this, &TheClass::eventFun );		\
__vector_nw_Command_handler_info.push_back(info);
    
    

#define ImplCommandHandlerEnd(Class) \
} \
void Class::UnRegCommandHandler(GGameCMDManager* session) \
{ \
MsgHandlerInfo<GGameCMDManager>::UnregSession(session, __vector_nw_Command_handler_info); \
}





#endif