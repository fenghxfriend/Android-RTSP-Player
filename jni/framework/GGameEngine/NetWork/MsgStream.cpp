#include "../../TEEEngine/base/TEE_system.h"
#include "MsgStream.h"
#include <string.h>

void MsgWriteStream::SerializeInt( int& i )
{
	m_packer.AddInt(i);
}

void MsgWriteStream::SerializeString( char* str, int size )
{
	m_packer.AddString(str, size);
}

void MsgWriteStream::SerializeRaw( void* data, int size )
{
	m_packer.AddRaw(data, size);
}

bool MsgWriteStream::Error() const
{
	return m_packer.Error();
}

void MsgWriteStream::Reset()
{
	m_packer.Reset();
}

int MsgWriteStream::Size() const
{
	return m_packer.Size();
}

const unsigned char * MsgWriteStream::Data() const
{
	return m_packer.Data();
}

void MsgReadStream::SerializeInt( int& i )
{
	i = m_packer.GetInt();
}

void MsgReadStream::SerializeString( char* str, int size )
{
    strcpy(str, m_packer.GetString());
}

void MsgReadStream::SerializeRaw( void* data, int size )
{
	mem_copy(data, m_packer.GetRaw(size), size);
}

bool MsgReadStream::Error() const
{
	return m_packer.Error();
}

void MsgReadStream::Reset( const void *pData, int Size )
{
	m_packer.Reset(pData, Size);
}
