#include "SessionHost.h"

ExtraDataHost::~ExtraDataHost()
{
    VecData::iterator vit = m_vecData.begin();
	for( ; vit != m_vecData.end(); ++vit )
	{
		ExtraData* pData = *vit;
        delete pData;
	}
    m_vecData.clear();
}

bool ExtraDataHost::AddExtraData( ExtraData* data )
{
	if (GetExtraData(data->GetID()))
	{
		return false;
	}
	m_vecData.push_back(data);
	return true;
}

void ExtraDataHost::RemoveExtraData( EXTRA_ID id )
{
	VecData::iterator vit = m_vecData.begin();
	for( ; vit != m_vecData.end(); ++vit )
	{
		ExtraData* pData = *vit;
		if( pData ->GetID() == id )
		{
			m_vecData.erase( vit );
			return;
		}
	}
}

ExtraData* ExtraDataHost::GetExtraData( EXTRA_ID id )
{
	for (VecData::size_type i = 0 ; i < m_vecData.size(); ++i)
	{
		if (m_vecData[i]->GetID() == id)
		{
			return m_vecData[i];
		}
	}
	return NULL;
}
