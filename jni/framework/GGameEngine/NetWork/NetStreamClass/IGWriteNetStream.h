#ifndef TankGame_IGWriteNetStream_h
#define TankGame_IGWriteNetStream_h

#include "../AbstractClass/IGNetStreamBase.h"

class IGWriteNetStream
{
public:
	void reset();
	virtual void serializeInteger(int &piValue);
	virtual void serializeString(char *piValue, int piSize = 0);
	virtual void serializeData(void *piValue, int piSize = 0);
	virtual bool error() const;
	int size() const;
	const unsigned char *data() const;
    
private:
	CPacker _packer;
};

#endif
