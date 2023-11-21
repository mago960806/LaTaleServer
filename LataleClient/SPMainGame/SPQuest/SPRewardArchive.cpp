
#include <algorithm>

#include "SPCommon.h"
#include "SPQuestAttr.h"
//#include "SPQuestArchive.h"
//#include "SPQuestManager.h"
#include "SPEventDef.h"
//#include "SPEvent.h"
#include "SPRewardAttr.h"
#include "SPRewardArchive.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPDebug.h"


/**
* 주어진 ID의 RewardAttr 검사 <br>
*/
struct _IsEqualReward : binary_function<SPRewardAttr*, int, bool> {
	bool operator() (SPRewardAttr* pRewardAttr, int iRewardID) const
	{
		if(pRewardAttr->m_iID == iRewardID)
			return true;
		return false;
	}
} IsEqualReward;


SPRewardArchive::SPRewardArchive()
{
	DeleteAllRewardAttr();
	LoadRewardAttrLDT();
}


SPRewardArchive::~SPRewardArchive()
{
	DeleteAllRewardAttr();
}


void SPRewardArchive::DeleteAllRewardAttr()
{
	std::vector<SPRewardAttr*>::iterator iter;
	iter = m_vpRewardList.begin();
	for(; iter != m_vpRewardList.end(); ++iter) {
		delete (*iter);
	}
	m_vpRewardList.clear();
}


bool SPRewardArchive::LoadRewardAttrLDT()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_QUEST_REWARD, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}

	int iRecordCount = pLDTFile->GetItemCount();	
	int iItemID = 0;

	LDT_Field ldtFieldExp;
	LDT_Field ldtFieldEly;		
	LDT_Field ldtFieldFame;

	int iExp = 0;
	int iEly = 0;
	int iFame = 0;

	LDT_Field ldtFieldEffect;
	int iEffect = 0;
	
	LDT_Field ldtFieldItemID;
	LDT_Field ldtFieldStack;
	LDT_Field ldtFieldRare;
	int iRewardItemID = 0;
	int iStack = 0;
	int iRare = 0;
	LDT_Field ldtFieldGuildPoint;
	int iGuildPoint = 0;

	for(int i = 0; i < iRecordCount; i++) {
	
		iItemID = pLDTFile->GetPrimaryKey( i );
		if(iItemID == 0)
			break;

		SPRewardAttr* pRewardAttr = new SPRewardAttr;
		
		pLDTFile->GetFieldFromLable(iItemID, "_Reward_EXP",		ldtFieldExp);
		iExp = ldtFieldExp.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Reward_Ely",		ldtFieldEly);
		iEly = ldtFieldEly.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Reward_Fame",	ldtFieldFame);
		iFame = ldtFieldFame.uData.lValue;

		pRewardAttr->SetRewardAttr(iItemID, iExp, iEly, iFame);

		char szEffect[64];
		for(int j = 0; j < QUESTREWARD_MAX_EFFECTID; j++) {
			ZeroMemory(szEffect, 64);
			wsprintf(szEffect, "_Reward_EffectID%d", j+1);

			pLDTFile->GetFieldFromLable(iItemID, szEffect,	ldtFieldEffect);
			iEffect = ldtFieldEffect.uData.lValue;
			
			pRewardAttr->m_iEffect[j] = iEffect;
		}

		char szItemID[64];
		char szStack[64];
		char szRare[64];
		for(int j = 0; j < QUESTREWARD_MAX_ITEM; j++) {
			ZeroMemory(szItemID,	64);
			ZeroMemory(szStack,		64);
			ZeroMemory(szRare,		64);
			wsprintf(szItemID, "_Reward_ItemID%d", j+1);
			wsprintf(szStack, "_Reward_StackCount%d", j+1);
			wsprintf(szRare, "_Reward_Rare%d", j+1);
			
			pLDTFile->GetFieldFromLable(iItemID, szItemID,	ldtFieldItemID);
			iRewardItemID = ldtFieldItemID.uData.lValue;

			pLDTFile->GetFieldFromLable(iItemID, szStack,	ldtFieldStack);
			iStack = ldtFieldStack.uData.lValue;

			pLDTFile->GetFieldFromLable(iItemID, szRare,	ldtFieldRare);
			iRare = ldtFieldRare.uData.lValue;

			pRewardAttr->m_Item[j].SetQuestItem(iRewardItemID, iStack, iRare);
		}		

		pLDTFile->GetField( iItemID , 25 , ldtFieldGuildPoint );
		iGuildPoint = ldtFieldGuildPoint.uData.lValue;

		pRewardAttr->m_iGuildPoint = iGuildPoint;
		
		m_vpRewardList.push_back(pRewardAttr);
	}

	return true;
}


SPRewardAttr* SPRewardArchive::GetReward(int iRewardID)
{
	std::vector<SPRewardAttr*>::iterator iter;
	iter = std::find_if(m_vpRewardList.begin(), m_vpRewardList.end(),
		std::bind2nd(IsEqualReward, iRewardID));	

	if(iter == m_vpRewardList.end())
		return NULL;
	else 
		return (*iter);

	return NULL;
}

