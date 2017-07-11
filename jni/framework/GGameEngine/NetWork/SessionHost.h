#ifndef __SESSION_HOST__
#define __SESSION_HOST__

#include <vector>
#include "../../TEEEngine/base/TEE_system.h"

typedef const char* EXTRA_ID;
class ExtraData
{
public:

	ExtraData(EXTRA_ID id)
	{
		m_id = id;
	}

    virtual ~ExtraData()
    {
        
    }
    
	EXTRA_ID GetID()
	{
		return m_id;
	}
private:
	EXTRA_ID m_id;
};

class ExtraDataHost
{
public:
    virtual ~ExtraDataHost();
	bool AddExtraData(ExtraData* data);
	void RemoveExtraData(EXTRA_ID id);

	ExtraData* GetExtraData(EXTRA_ID id);

protected:
	typedef std::vector<ExtraData*> VecData;
	VecData	m_vecData;
};

typedef int HostID;
class SessionHost : public ExtraDataHost
{
public:
	SessionHost(HostID id)
	{
		m_id = id;
	}
	HostID GetID()
	{
		return m_id;
	}

	NETADDR* GetNetAddr()
	{
		return &m_Address;
	}

	void SetNetAddr( NETADDR& addr )
	{
		mem_copy(&m_Address, &addr, sizeof(NETADDR));
	}

private:
	NETADDR m_Address;
	HostID	m_id;
};

#endif