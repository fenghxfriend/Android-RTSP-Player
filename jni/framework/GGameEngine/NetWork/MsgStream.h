#ifndef __MSG_STREAM__
#define __MSG_STREAM__

#include "../../TEEEngine/engine/shared/TEE_packer.h"

class MsgStream
{
public:
	virtual void SerializeInt(int& i) = 0;
	virtual void SerializeString(char* str, int size = 0) = 0;
	virtual void SerializeRaw(void* data, int size) = 0;
	virtual bool Error() const = 0;
};

class MsgWriteStream : public MsgStream
{
public:
	void Reset();
	virtual void SerializeInt(int& i);
	virtual void SerializeString(char* str, int size = 0);
	virtual void SerializeRaw(void* data, int size);
	virtual bool Error() const;

	int Size() const;
	const unsigned char *Data() const;


private:
	CPacker m_packer;
};

class MsgReadStream : public MsgStream
{
public:
	void Reset(const void *pData, int Size);
	virtual void SerializeInt(int& i);
	virtual void SerializeString(char* str, int size = 0);
	virtual void SerializeRaw(void* data, int size);
	virtual bool Error() const;
private:
	CUnpacker m_packer;
};

#endif