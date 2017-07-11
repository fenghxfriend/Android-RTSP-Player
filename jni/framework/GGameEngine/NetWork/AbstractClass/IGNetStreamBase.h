#ifndef TankGame_IGNetStreamBase_h
#define TankGame_IGNetStreamBase_h

#include <string.h>
#include "../../../TEEEngine/base/TEE_system.h"
#include "../../../TEEEngine/engine/shared/TEE_packer.h"

class IGNetStreamBase 
{
public:
	virtual void serializeInteger(int &piValue) = 0;
	virtual void serializeString(char *piValue, int piSize = 0) = 0;
	virtual void serializeData(void *piValue, int piSize = 0) = 0;
	virtual bool error() const = 0;
};

#endif
