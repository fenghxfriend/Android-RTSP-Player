#include "IGWriteNetStream.h"

void IGWriteNetStream::serializeInteger(int &piValue)
{
	_packer.AddInt(piValue);
}

void IGWriteNetStream::serializeString(char *piValue, int piSize)
{
	_packer.AddString(piValue, piSize);
}

void IGWriteNetStream::serializeData(void *piValue, int piSize)
{
	_packer.AddRaw(piValue, piSize);
}

bool IGWriteNetStream::error() const
{
	return _packer.Error();
}

void IGWriteNetStream::reset()
{
	_packer.Reset();
}

int IGWriteNetStream::size() const
{
	return _packer.Size();
}

const unsigned char * IGWriteNetStream::data() const
{
	return _packer.Data();
}