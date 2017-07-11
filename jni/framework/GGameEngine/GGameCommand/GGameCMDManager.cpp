#include "GGameCMDManager.h"
#include "CommandDef.h"

GGameCMDManager &GGameCMDManager::sharedInstance()
{
    static GGameCMDManager instance;
    return instance;
}

void GGameCMDManager::install()
{
    mSourceLock     = mUtility.createLock();
    m_First_Command = NULL;
    m_Last_Command  = NULL;
    mNewCommand     = NULL;
    mActiveCommand  = NULL;
}

void GGameCMDManager::unInstall()
{
    clear();
    m_First_Command = NULL;
    m_Last_Command  = NULL;
    mNewCommand     = NULL;
    mActiveCommand  = NULL;
    mUtility.destorylock( &mSourceLock );
}

void GGameCMDManager::sendCommand(CommandBase &command)
{
	mNewCommand     = getGlobalCommandFactory().CreateMsg(command.getMsgRTTI()->GetMsgID());
	*mNewCommand    = command;
    
    mUtility.lock(mSourceLock);
    if (m_First_Command != NULL)
    {
        m_Last_Command->m_Next  = mNewCommand;
        m_Last_Command          = mNewCommand;
    }
    else
    {
        m_Last_Command  = mNewCommand;
        m_First_Command = mNewCommand;
    }
    mUtility.unlock(mSourceLock);
}

void GGameCMDManager::flush(bool all)
{
	do
	{
        mActiveCommand = NULL;
        mUtility.lock(mSourceLock);
        do
        {
            if (m_First_Command == NULL) break;
            mActiveCommand  = m_First_Command;
            m_First_Command = m_First_Command->m_Next;
        } while (0);
        mUtility.unlock(mSourceLock);
        
        if ( mActiveCommand != NULL )
        {
            FireEvent(mActiveCommand->getMsgRTTI()->GetMsgID(), *mActiveCommand);
            delete mActiveCommand;
        }
        else break;
        
	}while (all) ;
}

void GGameCMDManager::clear()
{
    while ( true )
    {
        mActiveCommand = NULL;
        mUtility.lock(mSourceLock);
        
        do
        {
            if (m_First_Command == NULL) break;
            mActiveCommand  = m_First_Command;
            m_First_Command = m_First_Command->m_Next;
        } while (0);
        
        mUtility.unlock(mSourceLock);
        
        if ( mActiveCommand != NULL ) delete mActiveCommand;
        else break;
    }
}

