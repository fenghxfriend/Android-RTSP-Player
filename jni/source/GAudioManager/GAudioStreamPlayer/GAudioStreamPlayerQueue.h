#ifndef GAudioStreamPlayerQueue_h
#define GAudioStreamPlayerQueue_h

#include <memory>
#include <string>
#include "../GAudioUtility/GAudioUtility.h"

using namespace std;

class GAudioStreamPlayerQueue
{
public:
	void write(char *data  , int size   );
	int  read (char *buffer, int maxSize);
	void clear();

public:
	GAudioStreamPlayerQueue();
	GAudioStreamPlayerQueue(int capacity);
    ~GAudioStreamPlayerQueue();
    
private:
    void write0(char *data, int size, bool lock);
    void init(int capacity);
    void destory();
    void expandCapacity();
    
private:
    GAudioUtility 	mUtility;
    GAMULock   		mLock;
    int				mDataBytes;
    int				mDataPtrOffset;
    char			*mBufferPtr;
    int				mBufferCapacity;
    int				mBufferExpandMargin;
};

#endif

