#include "IGReadNetStream.h"

void IGReadNetStream::reset(const void *piData, int piSize)
{
	_unPacker.Reset(piData, piSize);
}

void IGReadNetStream::serializeInteger(int &piValue)
{
	piValue = _unPacker.GetInt();
}

void IGReadNetStream::serializeString(char *piValue, int piSize)
{
    strcpy(piValue, _unPacker.GetString());
}

void IGReadNetStream::serializeData(void *piValue, int piSize)
{
	mem_copy(piValue, _unPacker.GetRaw(piSize), piSize);
}

bool IGReadNetStream::error() const
{
	return _unPacker.Error();
}