#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "../../framework/GGameEngine/GGameUtility/GGameUtility.h"
#include "../../framework/GEngineEvent/GEngineEvent.h"
#include "../GAudioManager/GAudioManager.h"
#include "GDeviceDetecter.h"

typedef struct
{
    char IPAddress[32];
} GDeviceDetecterData;

void GDeviceDetecter_RunLoop_C(void *data)
{
	if ( !data )
	{
		GEngineEvent::sharedInstance().swallowEvent( EET_DDET_ERR, 0, 0 );
		return;
	}
	( ( GDeviceDetecter * )data )->runLoop();
}

void GDeviceDetecter::detect(void *callback, void *data)
{
	if ( callback && data )
	{
		mCallback = callback;
		mData	  = data;

		GGameUtility utility;
		GGameThread  threadHandle;
		utility.createThread( &threadHandle, GDeviceDetecter_RunLoop_C, this );
	}
	else
	{
		GEngineEvent::sharedInstance().swallowEvent( EET_DDET_ERR, 0, 0 );
	}
}

void GDeviceDetecter::runLoop()
{
	bool result = false;
	GDeviceDetecterData resultData;

    do
    {
        //广播地址
        struct sockaddr_in __sockaddr;
        int __addrDataSize = sizeof(__sockaddr);
        memset( &__sockaddr, 0, __addrDataSize );
        __sockaddr.sin_port = htons(58688);
        __sockaddr.sin_family = AF_INET;
        __sockaddr.sin_addr.s_addr = htons(INADDR_ANY);
        struct sockaddr *__listenAddr = (struct sockaddr *)&__sockaddr;

        //接收超时
        int timeoutLength = 1000 * 1000;
        struct timeval __litenTimeout;
        __litenTimeout.tv_sec  = 0;
        __litenTimeout.tv_usec = timeoutLength;

        //文件描述集
        fd_set __fdlist;

        //接收数据
        int __recivedDataSize = 0;
        int __recivedDataSize0 = 0;
        int __totalReciveDataSize = sizeof(GDeviceDetecterData);
        char __reciveDataBuffer[__totalReciveDataSize];
        memset( __reciveDataBuffer, 0, __totalReciveDataSize );

        //客户端地址
        struct sockaddr_in __clientAddr;
        socklen_t __clientAddrDataSize = sizeof(__clientAddr);
        struct sockaddr *__clientAddr0 = (struct sockaddr *)&__clientAddr;

        //创建套接字
        int socketObject = createSocket();
        if ( socketObject < 0 ) break;

        //绑定
        if ( bind( socketObject, __listenAddr, __addrDataSize ) != 0 ) { close(socketObject); break; }

        //循环接收数据
        for (int i = 0; i < 3; i++)
        {
            //设定文件描述集
            FD_ZERO(&__fdlist);
            FD_SET(socketObject, &__fdlist);
            __litenTimeout.tv_sec  = 0;
            __litenTimeout.tv_usec = timeoutLength;

            //监听是否有数据
            if ( select(socketObject + 1, &__fdlist, NULL, NULL,&__litenTimeout) > 0 )
            {
                //接收数据
                __recivedDataSize0 = recvfrom(socketObject, ( __reciveDataBuffer + __recivedDataSize ), ( __totalReciveDataSize - __recivedDataSize ), 0, __clientAddr0, &__clientAddrDataSize);

                //累加数据
                if ( __recivedDataSize0 > 0 )
                {
                    __recivedDataSize += __recivedDataSize0;
                    if ( __recivedDataSize >= __totalReciveDataSize )
                    {
                        memcpy(&resultData, __reciveDataBuffer, __totalReciveDataSize);
                        result = true;
                        break;
                    }
                }
            }
        }

        //关闭socket
        close(socketObject);

    } while (0);

    if ( result )
    {
    	//保存IP地址信息
    	GAudioManager::sharedInstance().savePlayStreamAddress( resultData.IPAddress );

    	//指定回调函数
    	( ( void (*)(void *, const char *) )mCallback )( mData, resultData.IPAddress );
    }
    else
    {
    	GEngineEvent::sharedInstance().swallowEvent( EET_DDET_ERR, 0, 0 );
    }
}

int GDeviceDetecter::createSocket()
{
    int handle = -1;
    bool result = false;

    do
    {
        //创建socket
        handle = socket(AF_INET, SOCK_DGRAM, 0);
        if (handle == -1)
        {
            break;
        }

        //地址重用、广播
        int __flag = 1;
        setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, &__flag, sizeof(__flag));
        //setsockopt(handle, SOL_SOCKET, SO_REUSEPORT, &__flag, sizeof(__flag));
        setsockopt(handle, SOL_SOCKET, SO_BROADCAST, &__flag, sizeof(__flag));

        //非阻塞模式
        unsigned long __nonBlocking = 1;
        ioctl(handle, FIONBIO, (unsigned long *)&__nonBlocking);

        //成功
        result = true;

    } while(0);

    //判断创建是否成功
    if ( !result && handle != -1 )
    {
        //关闭socket
        close(handle);
        handle = -1;
    }

    return handle;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//get local IPAddress
//
//add by jarry.jin 2014-04-02
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void GDeviceDetecter::parseLocalIPAddress(const char *ip)
{
	mLocalIPAddress[0] = 0;
	mLocalIPAddress[1] = 0;
	mLocalIPAddress[2] = 0;
	mLocalIPAddress[3] = 0;

	do
	{
		if ( !ip ) break;

		//get local all ip list
		char fIPList[32][100];
		int  fIPNumber;
		parseLocalIPAddressList( fIPList, &fIPNumber, 32 );
		if ( fIPNumber <= 0 ) break;

		//get server ip 3 number
		char fX3IPAddress[100];
		parseIPAddress3Number( ip, fX3IPAddress, 100 );
		if ( strlen( fX3IPAddress ) <= 0 ) break;

		//filter local ip
		char fY3IPAddress[100];
		for ( int i = 0; i < fIPNumber; i++ )
		{
			parseIPAddress3Number( fIPList[i], fY3IPAddress, 100 );
			if ( strlen( fY3IPAddress ) <= 0 ) break;
			if ( strcmp( fY3IPAddress, fX3IPAddress ) == 0 )
			{
				convertIPV4Address2IntArray( fIPList[i], mLocalIPAddress );
				break;
			}
		}

	} while( 0 );
}

int GDeviceDetecter::localIPAddress(int index)
{
	int result = 0;

	do
	{
		if ( index < 0 || index > 3 ) break;
		result = mLocalIPAddress[index];
	} while( 0 );

	return result;
}

void GDeviceDetecter::parseIPAddress3Number(const char *ip, char *cip, int maxsize)
{
	do
	{
		if ( cip && maxsize > 0 ) memset ( cip, 0, maxsize );

		if ( !ip  		   ) break;
		if ( !cip 		   ) break;
		if (  maxsize <= 0 ) break;

		int  fSize  = strlen( ip );
		int  fIndex = -1;
		if ( fSize <= 0) break;
		for ( int i = fSize - 1; i >= 0; i-- )
		{
			if ( ip[i] == '.' )
			{
				fIndex = i;
				break;
			}
		}
		if ( fIndex <= 0 ) break;
		memcpy( cip, ip, fIndex );

	} while( 0 );
}

void GDeviceDetecter::parseLocalIPAddressList(char list[][100], int *num, int max)
{
	do
	{
		if (  num      ) ( *num ) = 0;
		if ( !list 	   ) break;
		if ( !num  	   ) break;
		if (  max <= 0 ) break;

		int fd, intrface;
		struct ifreq buf[16];
		struct ifconf ifc;
		if ( ( fd = socket( AF_INET, SOCK_DGRAM, 0 ) ) >= 0 )
		{
			ifc.ifc_len = sizeof(buf);
			ifc.ifc_buf = (char *)buf;
			if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))
			{
				intrface = ifc.ifc_len/sizeof(struct ifreq);
				while (intrface-- > 0)
				{
					if (!(ioctl(fd, SIOCGIFADDR, (char *)&buf[intrface])))
					{
						memset ( list[( *num )], 0, 100 );
						sprintf( list[( *num )], "%s", (inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr)) );
						( *num )++;
						if ( ( *num ) >= max ) break;
					}
				}
			}
			close(fd);
		}

	} while( 0 );
}

void GDeviceDetecter::convertIPV4Address2IntArray(const char *ip, int *iip)
{
	if ( iip ) { for ( int i = 0; i < 4; i++ ) iip[i] = 0; }

	do
	{
		if ( !ip  ) break;
		if ( !iip ) break;

		char fIP[4][100];
		int  fSize  	 = strlen( ip );
		int  fPNumer     = 0;
		int  fIPCharIdx  = 0;
		int  fIPArrayIdx = 0;

		for ( int i = 0; i < 4	  ; i++ ) memset( fIP[i], 0, 100 );
		for ( int i = 0; i < fSize; i++ )
		{
			if ( ip[i] == '.' ) fPNumer++;
		}
		if ( fPNumer != 3 ) break;

		for ( int i = 0; i < fSize; i++ )
		{
			if ( ip[i] != '.' )
			{
				fIP[fIPArrayIdx][fIPCharIdx] = ip[i];
				fIPCharIdx++;
			}
			else
			{
				fIPArrayIdx++;
				fIPCharIdx = 0;
			}
		}

		for ( int i = 0; i < 4; i++ ) iip[i] = atoi( fIP[i] );

	} while( 0 );
}
