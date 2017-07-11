#ifndef TankGame_IGReadNetStream_h
#define TankGame_IGReadNetStream_h

#include "../AbstractClass/IGNetStreamBase.h"

class IGReadNetStream :public IGNetStreamBase
{
public:
	void reset(const void *piData, int piSize);
	virtual void serializeInteger(int &piValue);
	virtual void serializeString(char *piValue, int piSize = 0);
	virtual void serializeData(void *piValue, int piSize = 0);
	virtual bool error() const;
private:
	CUnpacker _unPacker;
};

#endif
