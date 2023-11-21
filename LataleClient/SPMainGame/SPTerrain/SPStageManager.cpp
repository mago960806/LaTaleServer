#include <string>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <assert.h>
#include <string>

using namespace std;

#ifdef _LATALE_CLIENT
	#include "SPCommon.h"
#endif

#include "SPResourceDef.h"

#ifdef PLAN_TOOL
	#include "stdafx.h"
#else
	#include "SPMainGameDEF.h"
	#include "SPGameObjectDEF.h"
#endif

#include "SPUtil.h"
#include "SPFileDEF.H"

#include "SPStage.h"
#include "SPStageManager.h"


SPStageManager g_StageManager;

SPStageManager::SPStageManager() {
	SetNull();
}


SPStageManager::~SPStageManager() {
	DeleteAllStage();
	SetNull();
}


void SPStageManager::SetNull() {
	m_iCurStage = -1;
	m_iCurMap = -1;
}


void SPStageManager::DeleteAllStage() {
	std::vector<SPStage*>::iterator iter;
	
	iter = m_vpStageList.begin();
	for(; iter != m_vpStageList.end(); ++iter){
		delete (*iter);
	}
	m_vpStageList.clear();
}


int	SPStageManager::GetStageCount() {
	int iCount = static_cast<int>(m_vpStageList.size());
	return iCount;
}


int SPStageManager::SetCurStageIndex(int iStage /*= -1*/) {
	if(iStage < 0)
		return -1;
	
	m_iCurStage = iStage;
	
	return m_iCurStage;
}


int SPStageManager::SetCurGroupIndex(int iGroup /*= -1*/) {
	if(iGroup < 0)
		return -1;	

	if(m_vpStageList.empty())
		return -1;

	vector< SPStage* >::iterator iter = m_vpStageList.begin() + m_iCurStage;
	(*iter)->SetCurMapGroupIndex(iGroup);

	return iGroup;
}


int SPStageManager::SetCurMapIndex(int iMap /*= 0*/, 
									int iGroup /*= -1*/, 
									int iStage /*= -1*/) 
{
	if(-1 < iStage)
		SetCurStageIndex(iStage);	

	if(-1 < iGroup)
		SetCurGroupIndex(iGroup);

	m_iCurMap = iMap;

	return iMap;
}


int SPStageManager::GetCurStageIndex() {
	return m_iCurStage;
}


int SPStageManager::GetCurGroupIndex()
{
	if( m_iCurStage < 0 || m_iCurStage > (int)m_vpStageList.size() - 1 )
		return -1;

	vector< SPStage* >::iterator iter = m_vpStageList.begin() + m_iCurStage;
	return (*iter)->GetCurMapGroupIndex();
}


int SPStageManager::GetCurMapIndex() {
	return m_iCurMap;
}


SPStage* SPStageManager::GetCurStage(int iStage /*= -1*/) {

	if(m_vpStageList.empty()) 
		return NULL;

	if(iStage < 0)
		iStage = m_iCurStage;

	if(iStage < 0)
		return NULL;

	if(iStage < m_vpStageList.size())
	{
		vector< SPStage* >::iterator iter = m_vpStageList.begin() + iStage;
		return (*iter);
	}

	return NULL;
}


SPMapGroup* SPStageManager::GetMapGroup(int iGroup /*= -1*/,
										int iStage /*= -1*/)
{
	if(iStage < 0)
		iStage = m_iCurStage;
	
	SPStage* pkCurStage = GetCurStage(iStage);
	if( pkCurStage == NULL )
		return NULL;

	if( iGroup < 0 )
		iGroup = pkCurStage->GetCurMapGroupIndex();

	if( iGroup < 0 || iGroup > pkCurStage->m_vpMapGroup.size() - 1 )
		return NULL;

	vector< SPMapGroup* >::iterator iterMapGroup = pkCurStage->m_vpMapGroup.begin() + iGroup;
	return (*iterMapGroup);
}


