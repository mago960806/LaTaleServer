#include "SPCommon.h"

#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"

#include "SPTeamArchive.h"

using namespace std;

//------------------------------------------------------------------------------------
SPTeamArchive::SPTeamArchive()
: m_bTeamActive(false)
, m_iLootingRule(0)
, m_bMaster(false)
{
}

//------------------------------------------------------------------------------------
SPTeamArchive::~SPTeamArchive()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPTeamArchive::Clear()
{
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		delete (*iter);
		++iter;
	}
	m_vpTeamList.clear();

	m_bTeamActive = false;
	m_bMaster = false;
	m_iLootingRule = 0;
	m_strActCharName.clear();
}

//------------------------------------------------------------------------------------
bool SPTeamArchive::GetActive()
{
	return m_bTeamActive;
}

//------------------------------------------------------------------------------------
int SPTeamArchive::GetTeamCount()
{
	return (int)m_vpTeamList.size();
}

//------------------------------------------------------------------------------------
vector< TEAM_MEMBER* >* SPTeamArchive::GetTeamList()
{
	return &m_vpTeamList;
}

//------------------------------------------------------------------------------------

GU_ID SPTeamArchive::GetCharGUID(const char* pszCharName)
{
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		if( strcmp((*iter)->szCharName, pszCharName) == 0 )
			return (*iter)->CharGUID;

		++iter;
	}

	return 0;
}


//------------------------------------------------------------------------------------

const char* SPTeamArchive::GetCharName(const GU_ID& CharID)
{
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		if( (*iter)->CharGUID == CharID )
			return (*iter)->szCharName;

		++iter;
	}

	return NULL;
}


//------------------------------------------------------------------------------------
const char* SPTeamArchive::GetCharName(const INT32& CharDBKey)
{
#if !defined(_CHANNEL_TEAM_VERSION_)
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		if( (*iter)->iCharID == CharDBKey )
			return (*iter)->szCharName;

		++iter;
	}
#endif

	return NULL;
}

//------------------------------------------------------------------------------------
bool SPTeamArchive::EnableInvite()
{
	return ((int)m_vpTeamList.size() < MAX_TEAM_NUM );
}

//------------------------------------------------------------------------------------
bool SPTeamArchive::InsertTeamPlayer(const TEAM_MEMBER& MemberInfo)
{
	TEAM_MEMBER* pMember;
	pMember = new TEAM_MEMBER;

	memcpy(pMember, &MemberInfo, sizeof(TEAM_MEMBER));
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	if( pMember->byGrade )
		m_vpTeamList.insert(iter, pMember);
	else
		m_vpTeamList.push_back(pMember);

	m_bTeamActive = true;
	return true;
}

//------------------------------------------------------------------------------------

void SPTeamArchive::EraseTeamPlayer(const GU_ID& CharGUID)
{
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		if( (*iter)->CharGUID == CharGUID )
		{
			delete (*iter);
			m_vpTeamList.erase(iter);
			return;
		}

		++iter;
	}
}


//------------------------------------------------------------------------------------
void SPTeamArchive::EraseTeamPlayer(const INT32& CharDBKey)
{
#if !defined(_CHANNEL_TEAM_VERSION_)
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		if( (*iter)->iCharID == CharDBKey )
		{
			delete (*iter);
			m_vpTeamList.erase(iter);
			return;
		}

		++iter;
	}
#endif
}


//------------------------------------------------------------------------------------
void SPTeamArchive::UpdateTeamPlayer(const TEAM_MEMBER& MemberInfo)
{
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
#if !defined(_CHANNEL_TEAM_VERSION_)
		if( (*iter)->iCharID == MemberInfo.iCharID )
#else
		if( (*iter)->CharGUID == MemberInfo.CharGUID )
#endif
		{
			memcpy(*iter, &MemberInfo, sizeof(TEAM_MEMBER));
			break;
		}

		++iter;
	}
}

//------------------------------------------------------------------------------------
void SPTeamArchive::SetActCharName(const char* pszCharName)
{
	m_strActCharName.clear();
	if( pszCharName == NULL )
		return;

	m_strActCharName = pszCharName;
}

//------------------------------------------------------------------------------------
const char* SPTeamArchive::GetActCharName()
{
	return m_strActCharName.c_str();
}

//------------------------------------------------------------------------------------
void SPTeamArchive::SetMaster(bool bMaster)
{
	m_bMaster = bMaster;
}

//------------------------------------------------------------------------------------
bool SPTeamArchive::IsMaster()
{
	return m_bMaster;
}

//------------------------------------------------------------------------------------
void SPTeamArchive::ChangeMaster(GU_ID MasterID)
{
	vector< TEAM_MEMBER* >::iterator iterMaster, iter;
	iter = m_vpTeamList.begin();
	iterMaster = m_vpTeamList.end();

	while( iter != m_vpTeamList.end() )
	{
		if( (*iter)->CharGUID == MasterID )
		{
			(*iter)->byGrade = 1;
			iterMaster = iter;
		}
		else
			(*iter)->byGrade = 0;

		++iter;
	}

	if( iterMaster != m_vpTeamList.begin() &&
		iterMaster != m_vpTeamList.end() )
	{
		TEAM_MEMBER* pTemp;
		iter = m_vpTeamList.begin();
		
		pTemp = (*iter);
		(*iter) = (*iterMaster);
		(*iterMaster) = pTemp;
	}
}

//------------------------------------------------------------------------------------
void SPTeamArchive::ChangeMaster(INT32 CharDBKey)
{
#if !defined(_CHANNEL_TEAM_VERSION_)
	vector< TEAM_MEMBER* >::iterator iterMaster, iter;
	iter = m_vpTeamList.begin();
	iterMaster = m_vpTeamList.end();

	while( iter != m_vpTeamList.end() )
	{
		if( (*iter)->iCharID == CharDBKey )
		{
			(*iter)->byGrade = 1;
			iterMaster = iter;
		}
		else
			(*iter)->byGrade = 0;

		++iter;
	}

	if( iterMaster != m_vpTeamList.begin() &&
		iterMaster != m_vpTeamList.end() )
	{
		TEAM_MEMBER* pTemp;
		iter = m_vpTeamList.begin();

		pTemp = (*iter);
		(*iter) = (*iterMaster);
		(*iterMaster) = pTemp;
	}
#endif
}

//------------------------------------------------------------------------------------
bool SPTeamArchive::IsMember(GU_ID iID)
{
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		if( (*iter)->CharGUID == iID )
			return true;

		++iter;
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPTeamArchive::IsMember( GU_ID iID , UINT16 uiServerID )
{
#if !defined(_CHANNEL_TEAM_VERSION_)
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		if( (*iter)->CharGUID == iID && (*iter)->wServerID == uiServerID )
			return true;

		++iter;
	}
#endif

	return false;
}

//------------------------------------------------------------------------------------
INT32 SPTeamArchive::GetCharDBKey(const char* pszCharName)
{
#if !defined(_CHANNEL_TEAM_VERSION_)
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		if( strcmp((*iter)->szCharName, pszCharName) == 0 )
			return (*iter)->iCharID;

		++iter;
	}
#endif

	return 0;
}

//------------------------------------------------------------------------------------
TEAM_MEMBER* SPTeamArchive::GetCharInfo( const INT32& CharDBKey )
{
#if !defined(_CHANNEL_TEAM_VERSION_)
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		if( (*iter)->iCharID == CharDBKey )
			return (*iter);

		++iter;
	}
#endif

	return NULL;
}

//------------------------------------------------------------------------------------

const char* SPTeamArchive::GetCharNameChannel( const GU_ID& CharID , UINT16 uiServerID )
{
#if !defined(_CHANNEL_TEAM_VERSION_)
	vector< TEAM_MEMBER* >::iterator iter = m_vpTeamList.begin();
	while( iter != m_vpTeamList.end() )
	{
		if( (*iter)->CharGUID == CharID && (*iter)->wServerID == uiServerID )
			return (*iter)->szCharName;

		++iter;
	}
#endif
	return NULL;
}
