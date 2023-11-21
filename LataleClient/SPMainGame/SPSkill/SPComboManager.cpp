
#include <vector>
#include <map>
#include "SPSkill.h"
#include "SPComboManager.h"

using namespace std;

SPComboManager* SPComboManager::m_pkInstance = NULL;
//------------------------------------------------------------------------------------
SPComboManager* SPComboManager::GetInstance()
{
	if( m_pkInstance == NULL )
		m_pkInstance = new SPComboManager;

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPComboManager::Release()
{
	if( m_pkInstance )
	{
		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPComboManager::SPComboManager()
{
}

//------------------------------------------------------------------------------------
SPComboManager::~SPComboManager()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPComboManager::Clear()
{
	m_mComboList.clear();
}

//------------------------------------------------------------------------------------
bool SPComboManager::AddCombo(int iID, const COMBO_INFO& Src)
{
	if( IsExist(iID) )
		return false;
	
	m_mComboList.insert(make_pair(iID, Src));
	return true;
}

//------------------------------------------------------------------------------------
bool SPComboManager::AddCombo(int iID, const SKILL_COMBO& Src)
{
	if( IsExist(iID) )
		return false;

	COMBO_INFO ComboInfo;
	ComboInfo.fAccmulateTime = 0.0f;
	ComboInfo.fComboMinTime = Src.fComboMinTime;
	ComboInfo.fComboMaxTime = Src.fComboMaxTime;

	m_mComboList.insert(make_pair(iID, ComboInfo));
	return true;
}

//------------------------------------------------------------------------------------
bool SPComboManager::IsExist(int iID)
{
	map< int, COMBO_INFO >::iterator iter = m_mComboList.find(iID);
	if( iter == m_mComboList.end() )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
bool SPComboManager::CanConnection(int iID)
{
	map< int, COMBO_INFO >::iterator iter = m_mComboList.find(iID);
	if( iter == m_mComboList.end() )
		return false;

	if( (*iter).second.fAccmulateTime >= (*iter).second.fComboMinTime &&
		(*iter).second.fAccmulateTime <= (*iter).second.fComboMaxTime )
		return true;

	return false;
}

//------------------------------------------------------------------------------------
void SPComboManager::Process(float fElapsedTime)
{
	map< int, COMBO_INFO >::iterator iter = m_mComboList.begin();
	while( iter != m_mComboList.end() )
	{
		(*iter).second.fAccmulateTime += fElapsedTime;
		if( (*iter).second.fAccmulateTime > (*iter).second.fComboMaxTime )
		{
			iter = m_mComboList.erase(iter);
			continue;
		}

		++iter;
	}
}
