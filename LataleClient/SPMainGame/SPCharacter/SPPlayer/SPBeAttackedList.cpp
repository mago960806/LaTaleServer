
#include "SPCommon.h"
#include "SPDebug.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPCommandConvert.h"
#include "SPMonsterModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPMonsterCoordPhysics.h"
#include "SPFlyCoordPhysics.h"
#include "SPGOBStatus.h"
#include "SPMonsterStatus.h"
#include "SPMonsterStatusModel.h"
#include "SPMOBCluster.h"
#include "SPMonster.h"


#include "SPBeAttackedList.h"


//////////////////////////////////////////////////////////////////////////
void SPBeAttackedObject::Clear()
{
	m_iObjectType		= TARGET_OBJECT_TYPE_MAX;
	m_pGameObject		= NULL;
	m_iShowType			= TARGET_OBJECT_SHOW_TYPE_MAX;
	m_fDeleyTime		= 0.0f;
	m_fAccmulateTime	= 0.0f;		
	m_pIconTexture		= NULL;
	ZeroMemory(&m_rtIcon, sizeof(RECT));
	m_iMaxHp			= 0;
	m_iCurHp			= 0;
	m_fHpRatio			= 0.0f;
	ZeroMemory(&m_rtGageLeft,	sizeof(RECT));
	ZeroMemory(&m_rtGageRight,	sizeof(RECT));
	m_strName.clear();
}


void SPBeAttackedObject::SetObject(SPGameObject* pObject, TARGET_OBJECT_TYPE iType) {
	m_iObjectType		= iType;
	m_pGameObject		= pObject;
	m_iShowType			= TARGET_OBJECT_SHOW_TYPE_NORMAL;
	m_fDeleyTime		= 2.0f;
	m_fAccmulateTime	= 0.0f;
}


void SPBeAttackedObject::SetShowType(TARGET_OBJECT_SHOW_TYPE iShowType)
{
	if(m_pGameObject) {
		if(m_iObjectType != TARGET_OBJECT_TYPE_MAX) {
			m_iShowType = iShowType;
			m_fDeleyTime		= 2.0f;
			m_fAccmulateTime	= 0.0f;
		}
	}
}


int SPBeAttackedObject::SetHpRatio(int iMaxHp, int iCurHp)
{
	int iHPWidth = 0;
	m_iMaxHp = iMaxHp;
	m_iCurHp = iCurHp;
	if( m_iMaxHp == 0 )
	{
		m_fHpRatio = 0.0f;
		return iHPWidth;
	}

	m_fHpRatio = (float)m_iCurHp / (float)m_iMaxHp;
	
	if(m_iObjectType < TARGET_OBJECT_TYPE_BOSS) {
		iHPWidth = (int)((float)80 * m_fHpRatio);
	}
	else {
		iHPWidth = (int)((float)356 * m_fHpRatio);
	}

	return iHPWidth;
}


void SPBeAttackedObject::SetGageRect(int iHpWidth, RECT* p_rtMobGage)
{	
	ZeroMemory(&m_rtGageLeft, sizeof(RECT));
	ZeroMemory(&m_rtGageRight, sizeof(RECT));
	
	if(p_rtMobGage == NULL)
		return;

	if(m_pGameObject == NULL)
		return;

	if(m_iShowType != TARGET_OBJECT_SHOW_TYPE_NORMAL)
		return;	
	
	if(m_iObjectType < TARGET_OBJECT_TYPE_BOSS) {
		m_rtGageRight.left = p_rtMobGage->left + 1;
		m_rtGageRight.top = p_rtMobGage->top + 1;
		m_rtGageRight.right = m_rtGageRight.left + iHpWidth;
		m_rtGageRight.bottom = m_rtGageRight.top + 8;
	}	
	else {
		if(iHpWidth < 2) {
			m_rtGageRight.left = p_rtMobGage->left + 1;
			m_rtGageRight.top = p_rtMobGage->top + 1;
			m_rtGageRight.right = m_rtGageRight.left + iHpWidth;
			m_rtGageRight.bottom = m_rtGageRight.top + 12;	
		}
		else {
			m_rtGageLeft.left = p_rtMobGage->left + 1;
			m_rtGageLeft.top = p_rtMobGage->top + 1;
			m_rtGageLeft.right = p_rtMobGage->left + (iHpWidth - 1) - 1;
			m_rtGageLeft.bottom = m_rtGageLeft.top + 12;

			m_rtGageRight.left = m_rtGageLeft.right;
			m_rtGageRight.top = p_rtMobGage->top + 1;
			m_rtGageRight.right = m_rtGageRight.left + 1;
			m_rtGageRight.bottom = m_rtGageRight.top + 12;
		}
	}
}



//////////////////////////////////////////////////////////////////////////
SPBeAttackedList::SPBeAttackedList()
{		
	m_pTexture = NULL;
	m_pBossBeAttacked = NULL;

	m_iStartX = 0;
	m_iStartY = 0;
	m_iQuickSlotSize = 1;					//Default 2줄

	char* IMAGE_FILE6 = "DATA/INTERFACE/CONCEPT/UI106.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(IMAGE_FILE6, &m_pTexture);
	SetRect(&m_rtHpSrc, 360, 377, 375, 384);
	SetRect(&m_rtGageBaseSrc, 130, 273, 212, 283);
	SetRect(&m_rtGageSrc, 130, 264, 210, 272);
	SetRect(&m_rtDeadSrc, 301, 252, 333, 284);
	SetRect(&m_rtMobSkinSrc, 327, 135, 359, 167);

	SetRect(&m_rtBossIconLineSrc,	130, 213, 190, 254);
	SetRect(&m_rtBossNameSrc,		318, 135, 320, 152);
	SetRect(&m_rtBossGageSrcLeft,	317, 156, 319, 170);
	SetRect(&m_rtBossGageSrcCenter,	321, 156, 323, 170);
	SetRect(&m_rtBossGageSrcRight,	325, 156, 327, 170);
	SetRect(&m_rtBossHpSrcLeft,		320, 171, 322, 183);
	SetRect(&m_rtBossHpSrcRight,	324, 171, 325, 183);

	SetDestRect();
	//DebugListSize();
}


SPBeAttackedList::~SPBeAttackedList()
{
	SAFE_RELEASE(m_pTexture);
	ClearAllList();
}


void SPBeAttackedList::ClearAllList()
{		
	SAFE_DELETE(m_pBossBeAttacked);

	std::vector<SPBeAttackedObject*>::iterator iter;
	iter = m_vpBeAttackedList.begin();
	for(; iter != m_vpBeAttackedList.end(); ++iter) {
		SAFE_DELETE((*iter));
	}

	m_vpBeAttackedList.clear();
}


void SPBeAttackedList::SetDestRect()
{
	m_iStartX = 0;
	m_iStartY = 0;
	
	if(m_iQuickSlotSize == 0) {
		m_iStartX = 197;
		m_iStartY = 80;
		
		if(m_pBossBeAttacked) {
			SetRect(&m_rtBossIconLineDest, m_iStartX + 3, m_iStartY + 1, m_iStartX + 3 + 60, m_iStartY + 1 + 41);
			SetRect(&m_rtBossNameDest, m_iStartX + 44, m_iStartY + 7, m_iStartX + 44 + 100, m_iStartY + 7 + 14);
			SetRect(&m_rtBossIconDest, m_iStartX + 9, m_iStartY + 6, m_iStartX + 9 + MOB_ICON_WIDTH, m_iStartY + 6 + MOB_ICON_HEIGHT);
			SetRect(&m_rtBossGageDestLeft, m_iStartX + 47, m_iStartY + 24, m_iStartX + 47 + 2, m_iStartY + 24 + 14); 
			SetRect(&m_rtBossGageDestCenter, m_iStartX + 49, m_iStartY + 24, m_iStartX + 49 + 352, m_iStartY + 24 + 14);
			SetRect(&m_rtBossGageDestRight, m_iStartX + 401, m_iStartY + 24, m_iStartX + 401 + 2, m_iStartY + 24 + 14);
			m_iStartY += 43;
		}
	}	
	else if(m_iQuickSlotSize == 1) {
		m_iStartX = 197;
		m_iStartY = 106;
		
		if(m_pBossBeAttacked) {
			SetRect(&m_rtBossIconLineDest, m_iStartX + 3, m_iStartY + 1, m_iStartX + 3 + 60, m_iStartY + 1 + 41);
			SetRect(&m_rtBossNameDest, m_iStartX + 44, m_iStartY + 7, m_iStartX + 44 + 100, m_iStartY + 7 + 14);
			SetRect(&m_rtBossIconDest, m_iStartX + 9, m_iStartY + 6, m_iStartX + 9 + MOB_ICON_WIDTH, m_iStartY + 6 + MOB_ICON_HEIGHT);
			SetRect(&m_rtBossGageDestLeft, m_iStartX + 47, m_iStartY + 24, m_iStartX + 47 + 2, m_iStartY + 24 + 14); 
			SetRect(&m_rtBossGageDestCenter, m_iStartX + 49, m_iStartY + 24, m_iStartX + 49 + 352, m_iStartY + 24 + 14);
			SetRect(&m_rtBossGageDestRight, m_iStartX + 401, m_iStartY + 24, m_iStartX + 401 + 2, m_iStartY + 24 + 14);			
			m_iStartY += 43;
		}
	}

	SetRect(&m_rtMobIconDest[TARGET_LIST_3], m_iStartX + 48,	m_iStartY + 6,	m_iStartX + 48 + MOB_ICON_WIDTH,	m_iStartY + 6 + MOB_ICON_HEIGHT);
	SetRect(&m_rtMobIconDest[TARGET_LIST_4], m_iStartX + 171,	m_iStartY + 6,	m_iStartX + 171 + MOB_ICON_WIDTH,	m_iStartY + 6 + MOB_ICON_HEIGHT);
	SetRect(&m_rtMobIconDest[TARGET_LIST_5], m_iStartX + 294,	m_iStartY + 6,	m_iStartX + 294 + MOB_ICON_WIDTH,	m_iStartY + 6 + MOB_ICON_HEIGHT);
	SetRect(&m_rtMobIconDest[TARGET_LIST_0], m_iStartX + 56,	m_iStartY + 23,	m_iStartX + 56 + MOB_ICON_WIDTH,	m_iStartY + 23 + MOB_ICON_HEIGHT);
	SetRect(&m_rtMobIconDest[TARGET_LIST_1], m_iStartX + 179,	m_iStartY + 23,	m_iStartX + 179 + MOB_ICON_WIDTH,	m_iStartY + 23 + MOB_ICON_HEIGHT);
	SetRect(&m_rtMobIconDest[TARGET_LIST_2], m_iStartX + 302,	m_iStartY + 23,	m_iStartX + 302 + MOB_ICON_WIDTH,	m_iStartY + 23 + MOB_ICON_HEIGHT);

	SetRect(&m_rtMobGageDest[TARGET_LIST_3], m_iStartX + 74,	m_iStartY + 11, m_iStartX + 74 + 82,	m_iStartY + 11 + 10);
	SetRect(&m_rtMobGageDest[TARGET_LIST_4], m_iStartX + 197,	m_iStartY + 11, m_iStartX + 197 + 82,	m_iStartY + 11 + 10);
	SetRect(&m_rtMobGageDest[TARGET_LIST_5], m_iStartX + 320,	m_iStartY + 11, m_iStartX + 320 + 82,	m_iStartY + 11 + 10);
	SetRect(&m_rtMobGageDest[TARGET_LIST_0], m_iStartX + 82,	m_iStartY + 28, m_iStartX + 82 + 82,	m_iStartY + 28 + 10);
	SetRect(&m_rtMobGageDest[TARGET_LIST_1], m_iStartX + 205,	m_iStartY + 28, m_iStartX + 205 + 82,	m_iStartY + 28 + 10);
	SetRect(&m_rtMobGageDest[TARGET_LIST_2], m_iStartX + 328,	m_iStartY + 28, m_iStartX + 328 + 82,	m_iStartY + 28 + 10);

	SetRect(&m_rtMobHpDest[TARGET_LIST_3],	m_iStartX + 77,		m_iStartY + 8,	m_iStartX + 77 + 15,	m_iStartY + 8 + 7);
	SetRect(&m_rtMobHpDest[TARGET_LIST_4],	m_iStartX + 200,	m_iStartY + 8,	m_iStartX + 200 + 15,	m_iStartY + 8 + 7);
	SetRect(&m_rtMobHpDest[TARGET_LIST_5],	m_iStartX + 323,	m_iStartY + 8,	m_iStartX + 323 + 15,	m_iStartY + 8 + 7);
	SetRect(&m_rtMobHpDest[TARGET_LIST_0],	m_iStartX + 85,		m_iStartY + 25,	m_iStartX + 85 + 15,	m_iStartY + 25 + 7);
	SetRect(&m_rtMobHpDest[TARGET_LIST_1],	m_iStartX + 208,	m_iStartY + 25,	m_iStartX + 208 + 15,	m_iStartY + 25 + 7);
	SetRect(&m_rtMobHpDest[TARGET_LIST_2],	m_iStartX + 331,	m_iStartY + 25,	m_iStartX + 331 + 15,	m_iStartY + 25 + 7);
}


void SPBeAttackedList::SetQuickSlotSize(int iSize)
{
	m_iQuickSlotSize = iSize;
	SetDestRect();
}


bool SPBeAttackedList::AddObject(SPGameObject* pObject, TARGET_OBJECT_TYPE iType)
{
	if(pObject == NULL)
		return false;

	if(iType == TARGET_OBJECT_TYPE_BOSS) {
		return false;
	}

	//추가되는 몹이 보스인 경우
	if(m_pBossBeAttacked && m_pBossBeAttacked->m_pGameObject) {
		if(m_pBossBeAttacked->m_pGameObject->GetGUID() == pObject->GetGUID()) {
			return false;
		}
	}
	
	//기존의 동일 오브젝트가 있다면 삭제
	std::vector<SPBeAttackedObject*>::iterator ExistIter;
	ExistIter = m_vpBeAttackedList.begin();
	int iIndex = 0;
	for(; ExistIter != m_vpBeAttackedList.end(); ++ExistIter, iIndex++) {
		if((*ExistIter)->m_pGameObject->GetGUID() == pObject->GetGUID()) {
			SAFE_DELETE((*ExistIter));
			m_vpBeAttackedList.erase(ExistIter);
			break;
		}
	}	
	
	//오브젝트 추가
	SPBeAttackedObject* pBeAttackedObject = new SPBeAttackedObject;	
	int iSize = static_cast<int>(m_vpBeAttackedList.size());

	if(iSize >= MAX_VIEW_LIST) {
		std::vector<SPBeAttackedObject*>::iterator iter = m_vpBeAttackedList.begin();
		SAFE_DELETE((*iter));
		m_vpBeAttackedList.erase(iter);
	}
	
	pBeAttackedObject->SetObject(pObject, iType);

	if(iType == TARGET_OBJECT_TYPE_MONSTER) {
		SPMonster* pMonster = static_cast<SPMonster*>(pObject);		
		SPMOBUIInfo* pMobUiInfo = g_pMOBCluster->GetMOBUIInfo(pMonster->GetClassID());
		
		if(pMobUiInfo == NULL) {
			SAFE_DELETE(pBeAttackedObject);
			return false;
		}

		if(pMobUiInfo->m_iMOBIcon) {
			std::string strPath = g_pResourceManager->GetGlobalFileName(pMobUiInfo->m_iMOBIcon);
			if(false == g_pVideo->GetTextureMgr()->LoadTexture(strPath.c_str(), &pBeAttackedObject->m_pIconTexture)) {
				SAFE_DELETE(pBeAttackedObject);
				return false;
			}
		}		

		pBeAttackedObject->m_rtIcon.left = (pMobUiInfo->m_iMOBIconIndex-1)%16*32;
		pBeAttackedObject->m_rtIcon.top = (pMobUiInfo->m_iMOBIconIndex-1)/16*32;
		pBeAttackedObject->m_rtIcon.right = pBeAttackedObject->m_rtIcon.left + 32;
		pBeAttackedObject->m_rtIcon.bottom = pBeAttackedObject->m_rtIcon.top + 32;		
	}
	else if(iType == TARGET_OBJECT_TYPE_PLAYER) {

	}	

	m_vpBeAttackedList.push_back(pBeAttackedObject);

	//DebugListSize();

	return true;
}


bool SPBeAttackedList::AddBossObject(SPGameObject* pObject, TARGET_OBJECT_TYPE iType /*= TARGET_OBJECT_TYPE_BOSS*/)
{
	if(m_pBossBeAttacked) {
		if(m_pBossBeAttacked->m_pGameObject->GetGUID() == pObject->GetGUID()) {
			return false;
		}
		//SAFE_DELETE(m_pBossBeAttacked);
	}

	m_pBossBeAttacked = new SPBeAttackedObject;
	m_pBossBeAttacked->SetObject(pObject, iType);	

	SPMonster* pMonster = static_cast<SPMonster*>(pObject);		
	SPMOBUIInfo* pMobUiInfo = g_pMOBCluster->GetMOBUIInfo(pMonster->GetClassID());
	if(pMobUiInfo == NULL) {
		SAFE_DELETE(m_pBossBeAttacked);
		return false;
	}
 
	if(pMobUiInfo->m_iMOBIcon) {
		std::string strPath = g_pResourceManager->GetGlobalFileName(pMobUiInfo->m_iMOBIcon);
		if(false == g_pVideo->GetTextureMgr()->LoadTexture(strPath.c_str(), &m_pBossBeAttacked->m_pIconTexture)) {
			SAFE_DELETE(m_pBossBeAttacked);
			return false;
		}
	}

	m_pBossBeAttacked->m_strName.clear();
	m_pBossBeAttacked->m_strName = pMobUiInfo->m_strMOBName;

	m_pBossBeAttacked->m_rtIcon.left = (pMobUiInfo->m_iMOBIconIndex-1)%16*32;
	m_pBossBeAttacked->m_rtIcon.top = (pMobUiInfo->m_iMOBIconIndex-1)/16*32;
	m_pBossBeAttacked->m_rtIcon.right = m_pBossBeAttacked->m_rtIcon.left + 32;
	m_pBossBeAttacked->m_rtIcon.bottom = m_pBossBeAttacked->m_rtIcon.top + 32;

	SetDestRect();

	if(pMonster->GetGobStatus() && pMonster->GetGobStatus()->GetGOBName()) {		
		
				
		SIZE szRet = {0, 0};
		szRet = g_pVideo->GetFont(FONT_12_BOLD)->GetSize(m_pBossBeAttacked->m_strName.c_str());
		int iNameSize = szRet.cx;
		int iStartX = m_rtBossNameDest.left;
		int iStartY = m_rtBossNameDest.top;
		SetRect(&m_rtBossNameDest, iStartX, iStartY, iStartX + iNameSize + 32, iStartY + 14);
	}
	
	return true;
}


bool SPBeAttackedList::DelObject(GU_ID iId)
{
	if(m_pBossBeAttacked) {
		if(m_pBossBeAttacked->m_pGameObject->GetGUID() == iId) {
			SAFE_DELETE(m_pBossBeAttacked);
			SetDestRect();
		}
	}
	
	std::vector<SPBeAttackedObject*>::iterator ExistIter;
	ExistIter = m_vpBeAttackedList.begin();
	int iIndex = 0;
	for(; ExistIter != m_vpBeAttackedList.end(); ++ExistIter, iIndex++) {
		if((*ExistIter)->m_pGameObject->GetGUID() == iId) {
			SAFE_DELETE((*ExistIter));
			m_vpBeAttackedList.erase(ExistIter);
			break;
		}
	}

	return true;
}


bool SPBeAttackedList::SetDead(GU_ID iId)
{
	if(m_pBossBeAttacked) {
		if(m_pBossBeAttacked->m_pGameObject->GetGUID() == iId) {
			if(m_pBossBeAttacked->m_iShowType == TARGET_OBJECT_SHOW_TYPE_NORMAL) {
				m_pBossBeAttacked->SetShowType(TARGET_OBJECT_SHOW_TYPE_DEAD);
			}
		}
	}
	
	if(m_vpBeAttackedList.empty())
		return false;
	
	SPBeAttackedObject* pObject = FindObject(iId);
	if(pObject && pObject->m_iShowType == TARGET_OBJECT_SHOW_TYPE_NORMAL) {
		pObject->SetShowType(TARGET_OBJECT_SHOW_TYPE_DEAD);
	}
	
	return false;
}


SPBeAttackedObject* SPBeAttackedList::FindObject(GU_ID iId)
{
	std::vector<SPBeAttackedObject*>::iterator iter;
	iter = m_vpBeAttackedList.begin();
	for(; iter != m_vpBeAttackedList.end(); ++iter) {
		if((*iter)->m_pGameObject->GetGUID() == iId)
			return (*iter);
	}

	return NULL;
}


void SPBeAttackedList::Process(float fTime)
{
	CheckObjectProcess(fTime);
	RenderProcess(fTime);
}


void SPBeAttackedList::CheckObjectProcess(float fTime)
{
	std::vector<SPBeAttackedObject*>::iterator iter;	
	int iIndex = 0;
	for(iter = m_vpBeAttackedList.begin(); iter != m_vpBeAttackedList.end(); ++iter, iIndex++) {
		(*iter)->m_fAccmulateTime += fTime;
		if((*iter)->m_fAccmulateTime > (*iter)->m_fDeleyTime) {
			(*iter)->m_fAccmulateTime -= (*iter)->m_fDeleyTime;
			//(*iter)->SetShowType(TARGET_OBJECT_TYPE_MAX);
			SAFE_DELETE((*iter));
			m_vpBeAttackedList.erase(iter);
			//DebugListSize();
			return;
		}
	}
}


void SPBeAttackedList::RenderProcess(float fTime)
{
	if(m_pBossBeAttacked) {
		if(m_pBossBeAttacked->m_pGameObject) {
			SPMonster* pMonster = static_cast<SPMonster*>(m_pBossBeAttacked->m_pGameObject);
			int iHpWidth = m_pBossBeAttacked->SetHpRatio(pMonster->GetGobStatus()->GetMaxHP(), pMonster->GetGobStatus()->GetCurHP());
			RECT rtBossGageDest;
			ZeroMemory(&rtBossGageDest, sizeof(RECT));
			rtBossGageDest.left = m_rtBossGageDestLeft.left;
			rtBossGageDest.top = m_rtBossGageDestLeft.top;
			rtBossGageDest.right = rtBossGageDest.left + 356;
			rtBossGageDest.bottom = rtBossGageDest.top + 14;
			m_pBossBeAttacked->SetGageRect(iHpWidth, &rtBossGageDest);
		}
	}

	int iSize = static_cast<int>(m_vpBeAttackedList.size());
	int iStartIndex = TARGET_LIST_MAX - iSize;
	int iIndex = 0;
	for(int i = iStartIndex, iIndex = 0; i < TARGET_LIST_MAX; i++, iIndex++) {
		if(iIndex < iSize) {
			SPBeAttackedObject* pObject = m_vpBeAttackedList.at(iIndex);
			if(pObject && pObject->m_pGameObject) {
				SPMonster* pMonster = static_cast<SPMonster*>(pObject->m_pGameObject);
				int iHPWidth = pObject->SetHpRatio(pMonster->GetGobStatus()->GetMaxHP(), pMonster->GetGobStatus()->GetCurHP());				
				pObject->SetGageRect(iHPWidth, &m_rtMobGageDest[i]);
			}
		}
	}
}


void SPBeAttackedList::Render(float fTime)
{	
	if(m_pBossBeAttacked && m_pTexture) {
		m_pTexture->RenderTexture(&m_rtBossNameDest,		&m_rtBossNameSrc);		
		g_pVideo->GetFont(FONT_12_BOLD)->RenderText(m_pBossBeAttacked->m_strName.c_str(), &m_rtBossNameDest, DT_CENTER | DT_VCENTER);
		//g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos(m_rtBossNameDest.left + 8, m_rtBossNameDest.top, m_pBossBeAttacked->m_strName.c_str(), 0.0f, DT_LEFT | DT_VCENTER);
		
		m_pTexture->RenderTexture(&m_rtBossGageDestLeft,	&m_rtBossGageSrcLeft);
		m_pTexture->RenderTexture(&m_rtBossGageDestCenter,	&m_rtBossGageSrcCenter);
		m_pTexture->RenderTexture(&m_rtBossGageDestRight,	&m_rtBossGageSrcRight);
		
		if(m_pBossBeAttacked->m_iShowType == TARGET_OBJECT_SHOW_TYPE_NORMAL) {
			if(m_pBossBeAttacked->m_pIconTexture) 
				m_pBossBeAttacked->m_pIconTexture->RenderTexture(&m_rtBossIconDest, &m_pBossBeAttacked->m_rtIcon);
		}
		else if(m_pBossBeAttacked->m_iShowType == TARGET_OBJECT_SHOW_TYPE_DEAD) {
			m_pTexture->RenderTexture(&m_rtBossIconDest, &m_rtDeadSrc);
		}
		
		m_pTexture->RenderTexture(&m_pBossBeAttacked->m_rtGageLeft, &m_rtBossHpSrcLeft);
		m_pTexture->RenderTexture(&m_pBossBeAttacked->m_rtGageRight, &m_rtBossHpSrcRight);
		m_pTexture->RenderTexture(&m_rtBossIconLineDest,	&m_rtBossIconLineSrc);
	}
	
	/*
	if(m_vpBeAttackedList.empty())
		return;		

	int iSize = static_cast<int>(m_vpBeAttackedList.size());
	int iStartIndex = TARGET_LIST_MAX - iSize;
	int iIndex = 0;
	
	char szTemp[16];
	for(int i = iStartIndex, iIndex = 0; i < TARGET_LIST_MAX; i++, iIndex++) {
		if(iIndex >= iSize)
			return;		
		
		if(m_vpBeAttackedList.at(iIndex)->m_iShowType == TARGET_OBJECT_SHOW_TYPE_NORMAL) {
			if(m_vpBeAttackedList.at(iIndex)->m_pIconTexture) {
				if(m_pTexture)
					m_pTexture->RenderTexture(&m_rtMobIconDest[i], &m_rtMobSkinSrc);

				g_pVideo->Flush();
				m_vpBeAttackedList.at(iIndex)->m_pIconTexture->RenderTexture(&m_rtMobIconDest[i], &m_vpBeAttackedList.at(iIndex)->m_rtIcon);
			}
		}
		else if(m_vpBeAttackedList.at(iIndex)->m_iShowType == TARGET_OBJECT_SHOW_TYPE_DEAD) {
			if(m_pTexture) {
				m_pTexture->RenderTexture(&m_rtMobIconDest[i], &m_rtDeadSrc);
			}
		}

		g_pVideo->Flush();

		if(m_pTexture) {
			m_pTexture->RenderTexture(&m_rtMobGageDest[i], &m_rtGageBaseSrc);			

			if(m_vpBeAttackedList.at(iIndex)->m_iObjectType != TARGET_OBJECT_TYPE_BOSS) {				
				m_pTexture->RenderTexture(&m_vpBeAttackedList.at(iIndex)->m_rtGageRight, &m_rtGageSrc);
			}
			
			m_pTexture->RenderTexture(&m_rtMobHpDest[i], &m_rtHpSrc);
		}
////
//#ifdef _DEBUG
//		ZeroMemory(szTemp, 16);
//		sprintf(szTemp, "%d", i);
//		g_pVideo->GetFont(FONT_12_BOLD)->RenderText(szTemp, &m_rtMobIconDest[i]);
//#endif
	}
	*/

	g_pVideo->Flush();
	
	return;
}


void SPBeAttackedList::DebugListSize()
{
#ifdef _DEBUG
	int iSize = static_cast<int>(m_vpBeAttackedList.size());
	DXUTOutputDebugString("	-------- BeAttackList(%d)\t", iSize); 
	
	int iIndex = 0;
	std::vector<SPBeAttackedObject*>::iterator iter;
	iter = m_vpBeAttackedList.begin();
	for(; iter != m_vpBeAttackedList.end(); ++iter, iIndex++) {
		DXUTOutputDebugString("Obj(%d)-[%d]  ", iIndex, (*iter)->m_pGameObject);
	}
	DXUTOutputDebugString("\n");
#endif
}


