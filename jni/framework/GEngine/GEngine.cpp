#include "GEngine.h"
#include "../GGameEngine/GGameCommand/GGameCMDManager.h"
#include "../GGameEngine/GGameClient/GGameClient.h"


#include "../GMediaEngine/GMAudio/GMAudio.h"

GEngine &GEngine::sharedInstance()
{
    static GEngine instance;
    return instance;
}

void GEngine::install()
{
	//清空事件队列
	mEventDataSize = sizeof( GEventData );
    mEventQueue.clear();

    //安装、启动RTSP服务器
    GMAudioInstall();
    GMAudioServerStart();

    //安装网络指令管理器、客户端
    GGameCMDManager::sharedInstance().install();
    GGameClient::sharedInstance().install();
}

void GEngine::unInstall()
{
	//卸载网络指令管理器、客户端
    GGameClient::sharedInstance().unInstall();
    GGameCMDManager::sharedInstance().unInstall();

    //停止、卸载RTSP服务器
    GMAudioServerStop();
    GMAudioUnInstall();

    //清空事件队列
    mEventQueue.clear();
}

void GEngine::reset()
{
    GGameCMDManager::sharedInstance().clear();
    mEventQueue.clear();
}

bool GEngine::eventRunLoop(GEventData *data)
{
    bool result = false;
    
    do
    {
        if ( !data ) break;

        //从队列中读取数据
        if ( mEventQueue.read( ( char * )data, mEventDataSize ) == mEventDataSize ) result = true;
        
    } while (0);
    
    return result;
}

void GEngine::swallowNetMessage(GGameNetMessage message)
{
    mNewEvent.event         = GEVT_NETMSG;
    mNewEvent.netMessage	= message;
    mEventQueue.write( ( char * )( &mNewEvent ), mEventDataSize );
}

void GEngine::swallowEngineEvent(GEngineEventData event)
{
    mNewEvent.event         = GEVT_ENGEVT;
    mNewEvent.engineEvent   = event;
    mEventQueue.write( ( char * )( &mNewEvent ), mEventDataSize );
}
