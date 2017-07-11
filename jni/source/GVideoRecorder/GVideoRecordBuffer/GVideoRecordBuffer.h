#ifndef GVideoRecordBuffer_h
#define GVideoRecordBuffer_h

#include <memory>
#include <string>
#include "../GVideoRecordUtility/GVideoRecordUtility.h"

#define GVideoRecordPacketMaxSize	51200 // 51200 = 50 * 1024; [ 50KB ]

using namespace std;

class GVideoRecordBuffer
{
public:
	void writePacket(char *data, int size );
	int  readPacket (char *buffer 		  );
	void clear();

public:
	GVideoRecordBuffer();
	GVideoRecordBuffer(int capacity);
    ~GVideoRecordBuffer();
    
private:
    void write0(char *data, int size, bool beginLock, bool endUnLock);
    int  read0(char *buffer, int size, bool beginLock, bool endUnLock);
    void init(int capacity);
    void destory();
    void expandCapacity();
    
private:
    GVideoRecordUtility	mUtility;
    GVIDEORCDLock     	mLock;
    int				  	mDataBytes;
    int				  	mDataPtrOffset;
    char			  	*mBufferPtr;
    int				  	mBufferCapacity;
    int				  	mBufferExpandMargin;
    int					mIntTypeLength;
};

#endif

