/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "../../base/TEE_system.h"
#include "TEE_config.h"
#include "TEE_network.h"

#include <stdio.h>

CNetConnection::CNetConnection()
{

}

CNetConnection::~CNetConnection()
{

}

void CNetConnection::ResetStats()
{
	mem_zero(&m_Stats, sizeof(m_Stats));
}

void CNetConnection::Reset()
{
	m_Sequence = 0;
	m_Ack = 0;
	m_RemoteClosed = 0;

	m_State = NET_CONNSTATE_OFFLINE;
	m_LastSendTime = 0;
	m_LastRecvTime = 0;
	m_LastUpdateTime = 0;
	m_Token = -1;
	mem_zero(&m_PeerAddr, sizeof(m_PeerAddr));
    
    m_weekSec = 2;
    m_dropSec = 20;
    m_isWeekSignal = false;
    m_fineSignalCounter = 0;

	m_Buffer.Init();

	mem_zero(&m_Construct, sizeof(m_Construct));
}

const char *CNetConnection::ErrorString()
{
	return m_ErrorString;
}

void CNetConnection::SetError(const char *pString)
{
	str_copy(m_ErrorString, pString, sizeof(m_ErrorString));
}

void CNetConnection::Init(NETSOCKET Socket)
{
	Reset();
	ResetStats();

	m_Socket = Socket;
	mem_zero(m_ErrorString, sizeof(m_ErrorString));
    
    m_weekSignalCallBack = 0;
    m_weekSignalCallBack = 0;
}

void CNetConnection::AckChunks(int Ack)
{
    CNetChunkResend *pResend = 0;
    
	while((pResend = m_Buffer.First()) != 0)
    {
		if(CNetBase::IsSeqInBackroom(pResend->m_Sequence, Ack))
		{
            m_Buffer.PopFirst();
		}
		else
        {
            break;
        }
	}
}

void CNetConnection::SignalResend()
{
	m_Construct.m_Flags |= NET_PACKETFLAG_RESEND;
}

int CNetConnection::Flush()
{
	int NumChunks = m_Construct.m_NumChunks;
	if(!NumChunks && !m_Construct.m_Flags)
	{
	    return 0;
	}

	// send of the packets
	m_Construct.m_Ack = m_Ack;
	CNetBase::SendPacket(m_Socket, &m_PeerAddr, &m_Construct);
    
	// update send times
	m_LastSendTime = time_get();

	// clear construct so we can start building a new package
	mem_zero(&m_Construct, sizeof(m_Construct));
    
	return NumChunks;
}

int CNetConnection::QueueChunkEx(int Flags, int DataSize, const void *pData, int Sequence)
{
	unsigned char *pChunkData;
	{
        // check if we have space for it, if not, flush the connection
        if(m_Construct.m_DataSize + DataSize + NET_MAX_CHUNKHEADERSIZE > (int)sizeof(m_Construct.m_aChunkData))
        {
            //Flush()
            //{
            int NumChunks = m_Construct.m_NumChunks;
            if(!NumChunks && !m_Construct.m_Flags)
                return 0;

            // send of the packets
            m_Construct.m_Ack = m_Ack;
            CNetBase::SendPacket(m_Socket, &m_PeerAddr, &m_Construct);

//            dbg_msg("send", "int CNetConnection::QueueChunkEx()");
            
            // update send times
            m_LastSendTime = time_get();

            // clear construct so we can start building a new package
            mem_zero(&m_Construct, sizeof(m_Construct));
            //}
        }


        // pack all the data
        CNetChunkHeader Header;
        Header.m_Flags = Flags;
        Header.m_Size = DataSize;
        Header.m_Sequence = Sequence;
        pChunkData = &m_Construct.m_aChunkData[m_Construct.m_DataSize];
        pChunkData = Header.Pack(pChunkData);
        mem_copy(pChunkData, pData, DataSize);
        pChunkData += DataSize;

        //
        m_Construct.m_NumChunks++;
        m_Construct.m_DataSize = (int)(pChunkData-m_Construct.m_aChunkData);
    }

    // set packet flags aswell
    if(Flags&NET_CHUNKFLAG_VITAL && !(Flags&NET_CHUNKFLAG_RESEND))
    {
        CNetChunkResend *pResend = m_Buffer.Allocate(sizeof(CNetChunkResend)+DataSize);
        if(pResend)
        {
            pResend->m_Sequence = Sequence;
            pResend->m_Flags = Flags;
            pResend->m_DataSize = DataSize;
            pResend->m_pData = (unsigned char *)(pResend+1);
            pResend->m_FirstSendTime = time_get();
            pResend->m_LastSendTime = pResend->m_FirstSendTime;
            mem_copy(pResend->m_pData, pData, DataSize);
        }
        else
        {
            // out of buffer
            //not thread safe, todo
            Disconnect("too weak connection (out of buffer)");
            return -1;
        }
    }

	return 0;
}

int CNetConnection::QueueChunk(int Flags, int DataSize, const void *pData)
{
	if(Flags&NET_CHUNKFLAG_VITAL)
		m_Sequence = (m_Sequence+1)%NET_MAX_SEQUENCE;

	return QueueChunkEx(Flags, DataSize, pData, m_Sequence);
}

void CNetConnection::SendControl(int ControlMsg, const void *pExtra, int ExtraSize)
{
	// send the control message
	m_LastSendTime = time_get();
	CNetBase::SendControlMsg(m_Socket, &m_PeerAddr, m_Ack, ControlMsg, pExtra, ExtraSize);
}

void CNetConnection::ResendChunk(CNetChunkResend *pResend)
{
	QueueChunkEx(pResend->m_Flags|NET_CHUNKFLAG_RESEND, pResend->m_DataSize, pResend->m_pData, pResend->m_Sequence);
	pResend->m_LastSendTime = time_get();  
}

void CNetConnection::Resend()
{
    CNetChunkResend *pResend = m_Buffer.First();
    while(pResend)
    {
        ResendChunk(pResend);
        pResend = m_Buffer.Next(pResend);
    }
}

int CNetConnection::Connect(NETADDR *pAddr)
{
	if(State() != NET_CONNSTATE_OFFLINE)
		return -1;

	// init connection
	Reset();
	m_PeerAddr = *pAddr;
	mem_zero(m_ErrorString, sizeof(m_ErrorString));
	m_State = NET_CONNSTATE_CONNECT;
	SendControl(NET_CTRLMSG_CONNECT, 0, 0);
	return 0;
}

void CNetConnection::Disconnect(const char *pReason)
{
	if(State() == NET_CONNSTATE_OFFLINE)
		return;

	if(m_RemoteClosed == 0)
	{
		if(pReason)
			SendControl(NET_CTRLMSG_CLOSE, pReason, str_length(pReason)+1);
		else
			SendControl(NET_CTRLMSG_CLOSE, 0, 0);

		m_ErrorString[0] = 0;
		if(pReason)
			str_copy(m_ErrorString, pReason, sizeof(m_ErrorString));
	}

	Reset();
}

int CNetConnection::Feed(CNetPacketConstruct *pPacket, NETADDR *pAddr)
{
	int64 Now = time_get();

	//服务端要求重发
	if(pPacket->m_Flags&NET_PACKETFLAG_RESEND)
    {
        //重发全部数据包
        Resend();
    }
    
	//判断当前数据包类型
	if(pPacket->m_Flags&NET_PACKETFLAG_CONTROL) //控制命令
	{
		int CtrlMsg = pPacket->m_aChunkData[0]; //解析具体命令类型

		if(CtrlMsg == NET_CTRLMSG_CLOSE) //服务器强行断开
		{
			if(net_addr_comp(&m_PeerAddr, pAddr) == 0)
			{
				m_State = NET_CONNSTATE_ERROR;
				m_RemoteClosed = 1;
			}
			return 0;
		}
		else
		{
			if(State() == NET_CONNSTATE_OFFLINE)    //当前自身为未连接状态
			{
				if(CtrlMsg == NET_CTRLMSG_CONNECT)  //服务器发出连接请求
				{
					//初始化
					Reset();
					m_State = NET_CONNSTATE_PENDING; //设置自身状态[准备就绪]
					m_PeerAddr = *pAddr;
					m_LastSendTime = Now;
					m_LastRecvTime = Now;
					m_LastUpdateTime = Now;
					SendControl(NET_CTRLMSG_CONNECTACCEPT, 0, 0);//发送命令，接受服务端的连接请求
				}
			}
			else if(State() == NET_CONNSTATE_CONNECT)       //自身正在连接服务器
			{
				if(CtrlMsg == NET_CTRLMSG_CONNECTACCEPT)    //服务器接受连接
				{
					m_LastRecvTime = Now;
					SendControl(NET_CTRLMSG_ACCEPT, 0, 0);
					m_State = NET_CONNSTATE_ONLINE; //成功连接服务器
				}
			}
			else if(State() == NET_CONNSTATE_PENDING)         //this is svr state only
            {
                if(CtrlMsg == NET_CTRLMSG_ACCEPT)             //so this is a cnt msg
				{
                    m_LastRecvTime = Now;
                    m_State = NET_CONNSTATE_ONLINE;
				}
            }
		}
	}

	if(State() == NET_CONNSTATE_ONLINE)
	{
        //累加器，测试稳定性
        m_fineSignalCounter++;
        
		m_LastRecvTime = Now;
		AckChunks(pPacket->m_Ack);
	}

	return 1;
}

int CNetConnection::Update()
{
	int64 Now = time_get();

    //判断当前状态是否有效
	if(State() == NET_CONNSTATE_OFFLINE || State() == NET_CONNSTATE_ERROR)
    {
        return 0;
    }
    
	//判断是否弱连接
	if(State() != NET_CONNSTATE_OFFLINE 
       && State() != NET_CONNSTATE_CONNECT 
       && (Now - m_LastRecvTime) <= time_freq() * m_weekSec)
	{
        if (!m_isWeekSignal)
        {
            //清空稳定性累加器
            m_fineSignalCounter = 0;
        }
        else
        {
            //判断是否达到指定的稳定性
            if (m_fineSignalCounter >= 10)
            {
                //清空稳定性累加器
                m_fineSignalCounter = 0;
                
                //设定弱连接flag
                m_isWeekSignal = false;
                
                //执行强连接回调函数
                if (m_powefulSignalCallBack != 0) 
                {
                    m_powefulSignalCallBack();
                }
            }
        }
	}
    else
    {
        //清空稳定性累加器
        m_fineSignalCounter = 0;
        
        //判断先前状态
        if (!m_isWeekSignal) 
        {
            if(State() != NET_CONNSTATE_OFFLINE 
               && State() != NET_CONNSTATE_CONNECT 
               && (Now - m_LastRecvTime) > time_freq() * m_weekSec)
            {
                //设定弱连接flag
                m_isWeekSignal = true;
                
                //执行弱连接回调函数
                if (m_weekSignalCallBack != 0) 
                {
                    m_weekSignalCallBack();
                }
            }
        }
        else
        {
            if(State() != NET_CONNSTATE_OFFLINE 
               && State() != NET_CONNSTATE_CONNECT 
               && (Now - m_LastRecvTime) > time_freq() * m_dropSec)
            {
                //掉线
                m_State = NET_CONNSTATE_ERROR;
            }
        }
    }
    
    //判断是否弱连接状态
//    if (!m_isWeekSignal) 
    if(State() != NET_CONNSTATE_OFFLINE 
       && State() != NET_CONNSTATE_CONNECT)
    {
        //从重发队列当中取数据包
        CNetChunkResend *pResend = m_Buffer.First();
        if(pResend && (Now - pResend->m_LastSendTime > time_freq())) //存在数据包1秒以上没收到回复，重发全部待重发的数据包
        {
            Resend();
        }
    }
//    else
//    {
////        //清除所有待重发数据包
////        CNetChunkResend *pResend = 0;
////        while((pResend = m_Buffer.First()) != 0)
////        {
////            m_Buffer.PopFirst();
////        }
//        
//        m_Sequence = 0;
//        m_Ack = 0;
////        m_LastSendTime = 0;
////        m_LastRecvTime = 0;
////        m_LastUpdateTime = 0;
//        m_Buffer.Init();
//        mem_zero(&m_Construct, sizeof(m_Construct));
//    }

	//判断当前状态
	if(State() == NET_CONNSTATE_ONLINE) //在线
	{
        if(time_get() - m_LastSendTime > time_freq()/5) //200ms保活一下
        {
            if (Flush() == 0) //没有可发送的数据包
            {
                SendControl(NET_CTRLMSG_KEEPALIVE, 0, 0); //发送心跳
            }
        }
	}
	else if(State() == NET_CONNSTATE_CONNECT) //正在连接
	{
		if(time_get() - m_LastSendTime > time_freq()/2) //每隔500ms发送一次连接命令
        {
            SendControl(NET_CTRLMSG_CONNECT, 0, 0);
        }
	}
	else if(State() == NET_CONNSTATE_PENDING) //准备就绪
	{
		if(time_get() - m_LastSendTime > time_freq()/2) //每隔500ms发送一次准备就绪状态
        {
            SendControl(NET_CTRLMSG_CONNECTACCEPT, 0, 0);
        }
	}

	return 0;
}

void CNetConnection::setNetSignalStateCallBack(int weeksec, int dropsec, NETFUNC_SIGNALSTRENGTH powerfulCallBack, NETFUNC_SIGNALSTRENGTH weekCallBack)
{
    m_weekSec = weeksec < dropsec ? weeksec:dropsec;
    m_dropSec = weeksec > dropsec ? weeksec:dropsec;;
    m_isWeekSignal = false;
    m_powefulSignalCallBack = powerfulCallBack;
    m_weekSignalCallBack = weekCallBack;
}
