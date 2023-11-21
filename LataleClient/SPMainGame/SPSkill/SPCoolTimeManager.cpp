
#include <hash_map>
#include "SPCoolTimeManager.h"

using namespace stdext;

SPCoolTimeManager* SPCoolTimeManager::m_pkInstance = NULL;
//------------------------------------------------------------------------------------
SPCoolTimeManager* SPCoolTimeManager::GetInstance()
{
	if( m_pkInstance == NULL )
		m_pkInstance = new SPCoolTimeManager;

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPCoolTimeManager::Release()
{
	if( m_pkInstance )
	{
		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPCoolTimeManager::SPCoolTimeManager()
{
}

//------------------------------------------------------------------------------------
SPCoolTimeManager::~SPCoolTimeManager()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPCoolTimeManager::Clear()
{
	m_hmCoolTimeList.clear();
}

//------------------------------------------------------------------------------------
void SPCoolTimeManager::Process(float fElapsedTime)
{
	hash_map< int, COOLTIME_INFO >::iterator iter = m_hmCoolTimeList.begin();
	while( iter != m_hmCoolTimeList.end() )
	{
		iter->second.fAccmulateTime += fElapsedTime;
		if( iter->second.fAccmulateTime > iter->second.fCoolTime )
		{
			iter = m_hmCoolTimeList.erase(iter);
			continue;
		}

		++iter;
	}
}

//------------------------------------------------------------------------------------
bool SPCoolTimeManager::AddCoolTime(int iID, const COOLTIME_INFO& Src)
{
	//if( IsExistCoolTime(iID) )
	//	return false;

	if( Src.fCoolTime <= 0.0f )
		return false;

	COOLTIME_INFO* pCoolTimeInfo = GetCoolTime(iID);
	if( pCoolTimeInfo )
	{
		pCoolTimeInfo->fAccmulateTime = 0.0f;
		pCoolTimeInfo->fCoolTime = Src.fCoolTime;
	}
	else
		m_hmCoolTimeList.insert(hash_map< int, COOLTIME_INFO >::value_type(iID, Src));

	return true;
}

//------------------------------------------------------------------------------------
bool SPCoolTimeManager::IsExistCoolTime(int iID)
{
	hash_map< int, COOLTIME_INFO >::iterator iter = m_hmCoolTimeList.find(iID);
	if( iter == m_hmCoolTimeList.end() )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
COOLTIME_INFO* SPCoolTimeManager::GetCoolTime(int iID)
{
	hash_map< int, COOLTIME_INFO >::iterator iter = m_hmCoolTimeList.find(iID);
	if( iter == m_hmCoolTimeList.end() )
		return NULL;

	return &iter->second;
}