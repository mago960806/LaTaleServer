#include "SPCommon.h"
#include "SPUtil.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPCheckManager.h"

#include "SPQuestAttr.h"

#include "SPDebug.h"

SPQuestAttr::SPQuestAttr() 
{
	Clear();
}


SPQuestAttr::~SPQuestAttr()
{
	Clear();
}


void SPQuestAttr::Clear()
{
	m_iQuestID = 0;
	m_strQuestName = "";
	m_strQuestName.clear();	
	
	m_strTalk1 = "";
	m_strTalk1.clear();
	m_strTalk2 = "";
	m_strTalk2.clear();
	m_strPurPose = "";
	m_strPurPose.clear();

	m_iRewordNpcID = 0;

	m_iVisible = 0;

	int i = 0;
	int j = 0;

	for(i = 0; i < QUEST_NPC_MAX; i++) {
		m_QuestNpc[i].Clear();
	}

	for(i = 0; i < QUEST_MAX_REQUIRE; i++) {
		m_Require[i].Clear();
	}

	for(i = 0; i < QUEST_MAX_ACQUISITION_EFFECTID; i++) {
		m_iAcquisitionEffect[i] = 0;
	}

	for(i = 0; i < QUEST_MAX_ACQUISITION_ITEM; i++) {
		m_AcquisitionItem[i].Clear();
	}

	for(i = 0; i < QUEST_MAX_MISSION; i++) {
		m_Mission[i].Clear();
	}

	for(i = 0; i < QUEST_MAX_CONSUMPTION_ITEM; i++) {
		m_ConsumeItem[i].Clear();
	}

	//for(i = 0; i < QUEST_MAX_REWARD_EFFECTID; i++) {
	//	m_iRewordEffect[i] = 0;
	//}	

	//for(i = 0; i < QUEST_MAX_REWARD_ITEM; i++) {
	//	m_RewordItem[i].Clear();
	//}

	for(i = 0; i < QUEST_MAX_RANK; i++) {
		for(j = 0; j < QUEST_MAX_RANK_REQUIRE; j++) {
			m_RewordRequire[i][j].Clear();
		}
	}

	for(i = 0; i < QUEST_MAX_RANK; i++) {
		m_RewordRank[i].Clear();
	}
	
	m_iUiShow = QUEST_UI_SHOW_ABLE;				// 기본이 무조건 보이는것으로...
	m_iRewordCount = 0;

	m_strMsgAccept = "";
	m_strMsgAccept.clear();
	m_iImageAccept = 0;
	m_strMsgReword = "";
	m_strMsgReword.clear();
	m_iImageReword = 0;
	
	m_iCityName = CITY_NAME_BELOS;

	m_eAcceptType	=	(QUEST_ACCEPTTYPE)0;
	m_iActCharID	=	0;
	m_iCurrentAccept  = (QUEST_ACCEPTTYPE)0;
}


void SPQuestAttr::operator=(const SPQuestAttr Src)
{
	Clear();
	m_iQuestID		= Src.m_iQuestID;
	m_strQuestName	= Src.m_strQuestName;	
	
	m_strTalk1		= Src.m_strTalk1;
	m_strTalk2		= Src.m_strTalk2;
	m_strPurPose	= Src.m_strPurPose;

	m_iRewordNpcID	= Src.m_iRewordNpcID;

	m_iVisible		= Src.m_iVisible;	

	int i = 0;
	int j = 0;

	for(i = 0; i < QUEST_NPC_MAX; i++) {
		m_QuestNpc[i] = Src.m_QuestNpc[i];
	}

	for(i = 0; i < QUEST_MAX_REQUIRE; i++) {
		m_Require[i] = Src.m_Require[i];		
	}

	for(i = 0; i < QUEST_MAX_ACQUISITION_EFFECTID; i++) {
		m_iAcquisitionEffect[i] = Src.m_iAcquisitionEffect[i];
	}

	for(i = 0; i < QUEST_MAX_ACQUISITION_ITEM; i++) {
		m_AcquisitionItem[i] = Src.m_AcquisitionItem[i];
	}

	for(i = 0; i < QUEST_MAX_MISSION; i++) {
		m_Mission[i] = Src.m_Mission[i];
	}

	for(i = 0; i < QUEST_MAX_CONSUMPTION_ITEM; i++) {
		m_ConsumeItem[i] = Src.m_ConsumeItem[i];
	}

	//for(i = 0; i < QUEST_MAX_REWARD_EFFECTID; i++) {
	//	m_iRewordEffect[i] = Src.m_iRewordEffect[i];
	//}

	//for(i = 0; i < QUEST_MAX_REWARD_ITEM; i++) {
	//	m_RewordItem[i] = Src.m_RewordItem[i];
	//}

	for(i = 0; i < QUEST_MAX_RANK; i++) {
		for(j = 0; j < QUEST_MAX_RANK_REQUIRE; j++) {
			m_RewordRequire[i][j] = Src.m_RewordRequire[i][j];
		}
	}

	for(i = 0; i < QUEST_MAX_RANK; i++) {
		m_RewordRank[i] = Src.m_RewordRank[i];
	}

	m_iUiShow		= Src.m_iUiShow;
	m_iRewordCount  = Src.m_iRewordCount;

	m_strMsgAccept	= Src.m_strMsgAccept;
	m_iImageAccept	= Src.m_iImageAccept;
	m_strMsgReword	= Src.m_strMsgReword;
	m_iImageReword	= Src.m_iImageReword;
	
	m_iCityName		= Src.m_iCityName;

	m_eAcceptType		=	Src.m_eAcceptType;
	m_iActCharID		=	Src.m_iActCharID;
	m_iCurrentAccept	=	Src.m_iCurrentAccept;
}


void SPQuestAttr::Copy(SPQuestAttr* pSrc)
{
	Clear();
	m_iQuestID		= pSrc->m_iQuestID;
	m_strQuestName	= pSrc->m_strQuestName;	

	m_strTalk1		= pSrc->m_strTalk1;
	m_strTalk2		= pSrc->m_strTalk2;
	m_strPurPose	= pSrc->m_strPurPose;

	m_iRewordNpcID	= pSrc->m_iRewordNpcID;

	m_iVisible		= pSrc->m_iVisible;	

	int i = 0;
	int j = 0;

	for(i = 0; i < QUEST_NPC_MAX; i++) {
		m_QuestNpc[i] = pSrc->m_QuestNpc[i];
	}

	for(i = 0; i < QUEST_MAX_REQUIRE; i++) {
		m_Require[i] = pSrc->m_Require[i];		
	}

	for(i = 0; i < QUEST_MAX_ACQUISITION_EFFECTID; i++) {
		m_iAcquisitionEffect[i] = pSrc->m_iAcquisitionEffect[i];
	}

	for(i = 0; i < QUEST_MAX_ACQUISITION_ITEM; i++) {
		m_AcquisitionItem[i] = pSrc->m_AcquisitionItem[i];
	}

	for(i = 0; i < QUEST_MAX_MISSION; i++) {
		m_Mission[i] = pSrc->m_Mission[i];
	}

	for(i = 0; i < QUEST_MAX_CONSUMPTION_ITEM; i++) {
		m_ConsumeItem[i] = pSrc->m_ConsumeItem[i];
	}

	//for(i = 0; i < QUEST_MAX_REWARD_EFFECTID; i++) {
	//	m_iRewordEffect[i] = pSrc->m_iRewordEffect[i];
	//}

	//for(i = 0; i < QUEST_MAX_REWARD_ITEM; i++) {
	//	m_RewordItem[i] = pSrc->m_RewordItem[i];
	//}

	for(i = 0; i < QUEST_MAX_RANK; i++) {
		for(j = 0; j < QUEST_MAX_RANK_REQUIRE; j++) {
			m_RewordRequire[i][j] = pSrc->m_RewordRequire[i][j];
		}
	}

	for(i = 0; i < QUEST_MAX_RANK; i++) {
		m_RewordRank[i] = pSrc->m_RewordRank[i];
	}

	m_iUiShow		= pSrc->m_iUiShow;
	m_iRewordCount  = pSrc->m_iRewordCount;

	m_strMsgAccept	= pSrc->m_strMsgAccept;
	m_iImageAccept	= pSrc->m_iImageAccept;
	m_strMsgReword	= pSrc->m_strMsgReword;
	m_iImageReword	= pSrc->m_iImageReword;
	m_iCityName		= pSrc->m_iCityName;

	m_eAcceptType		=	pSrc->m_eAcceptType;
	m_iActCharID		=	pSrc->m_iActCharID;
	m_iCurrentAccept	=	pSrc->m_iCurrentAccept;
}


bool SPQuestAttr::SetQuestAttr(int iQuestID, std::string strName, int iVisible)
{
	Clear();
	m_iQuestID = iQuestID;
	if(m_iQuestID == 0)			return false;

	m_strQuestName = strName;	
	m_iVisible = iVisible;
	
	return true;
}


bool SPQuestAttr::SetQuestAttrInfo(std::string strTalk1, std::string strTalk2, 
					  std::string strPurPose, int iRewordNpcID)
{
	if(m_iQuestID == 0)			return false;	
	
	m_strTalk1 = strTalk1;
	m_strTalk2 = strTalk2;
	m_strPurPose = strPurPose;
	
	m_iRewordNpcID = iRewordNpcID;

	return true;
}


bool SPQuestAttr::SetQuestRequireStat(int iIndex, int iStat, 
									  int iStatValue, int iValue1, int iValue2)
{
	if(m_iQuestID == 0)			return false;
	
	m_Require[iIndex].SetRequire(iStat, iStatValue, iValue1, iValue2);
	
	return true;
}


bool SPQuestAttr::SetAcquisitionEffect(int iIndex, int iEffect)
{
	if(m_iQuestID == 0)			return false;
	
	m_iAcquisitionEffect[iIndex] = iEffect;

	return true;
}


bool SPQuestAttr::SetAcquisitionItem(int iIndex, int iItemID, 
									 int iStack, int iRear)
{
	if(m_iQuestID == 0)			return false;
	
	m_AcquisitionItem[iIndex].SetQuestItem(iItemID, iStack, iRear);

	return true;
}


bool SPQuestAttr::SetMission(int iIndex, MISSION_TYPE iMission, 
							 int iTypeID, int iCount, bool bShow)
{
	if(m_iQuestID == 0)			return false;
	
	m_Mission[iIndex].SetMission(iMission, iTypeID, iCount, bShow);

	return true;
}


bool SPQuestAttr::SetCunsumeItem(int iIndex, int iItemID, int iStack)
{
	if(m_iQuestID == 0)			return false;
	
	m_ConsumeItem[iIndex].SetConsumeItem(iItemID, iStack);

	return true;
}


////
//bool SPQuestAttr::SetRewordEffect(int iIndex, int iEffect)
//{
//	if(m_iQuestID == 0)			return false;
//	
//	//m_iRewordEffect[iIndex] = iEffect;
//
//	return true;
//}


////
//bool SPQuestAttr::SetRewordItem(int iIndex, int iItemID, int iStack, int iRear)
//{
//	if(m_iQuestID == 0)			return false;
//	
//	//m_RewordItem[iIndex].SetQuestItem(iItemID, iStack, iRear);
//	
//	return true;
//}


bool SPQuestAttr::SetQuestRewordStat(int i, int j, int iStat, int iStatValue, int iValue1, int iValue2)
{
	if(m_iQuestID == 0)			return false;

	m_RewordRequire[i][j].SetRequire(iStat, iStatValue, iValue1, iValue2);

	return true;
}


bool SPQuestAttr::SetNpc(int iIndex, int iIcon, int iIconIndex,
						 std::string strPlace, std::string strName)
{
	if(m_iQuestID == 0)			return false;

	m_QuestNpc[iIndex].SetQuestNpc(iIcon, iIconIndex, strPlace, strName);

	return true;
}


bool SPQuestAttr::SetNpcMessage(std::string strAccept, int iAccept, 
				   std::string strReword, int iReword)
{
	if(m_iQuestID == 0)			return false;	

	m_strMsgAccept = strAccept;
	m_iImageAccept = iAccept;
	m_strMsgReword = strReword;
	m_iImageReword = iReword;	

	return true;
}


QUEST_UI_SHOW SPQuestAttr::IsRequire()
{
	REQUIRE_TYPE iRequireType = REQUIRE_TYPE_NULL;
	int iID = 0;
	int iValue1 = 0;
	int iValue2 = 0;
	bool bResult = true;
	
	for(int i = 0; i < QUEST_MAX_REQUIRE; i++) 
	{
		//(*iter)->m_Require[i].m_iRequireStat
		iRequireType = (REQUIRE_TYPE)(m_Require[i].m_iRequireStat);
		iID = m_Require[i].m_iRequireStatValue;
		iValue1 = m_Require[i].m_iValue1;
		iValue2 = m_Require[i].m_iValue2;

		////
		//if(bResult == false) {
		//	break;
		//}			
		
		////
		//switch(iRequireType) {
		//	case REQUIRE_TYPE_STAT	:
		//		bResult = g_pCheckManager->CheckPCStatus((CHECK_STATUS)iID, iValue1, iValue2);
		//		m_Require[i].m_bOK = bResult;
		//		break;
		//	case REQUIRE_TYPE_ITEM	:
		//		bResult = g_pCheckManager->CheckPCItem(iID, iValue1, iValue2);
		//		m_Require[i].m_bOK = bResult;
		//		break;
		//	case REQUIRE_TYPE_QUEST	:
		//		bResult = g_pCheckManager->CheckPCQuest(iID, iValue1, iValue2);
		//		m_Require[i].m_bOK = bResult;
		//		break;
		//	case REQUIRE_TYPE_SKILL :
		//		bResult = g_pCheckManager->CheckPCSkill(iID, iValue1, iValue2);
		//		m_Require[i].m_bOK = bResult;
		//		break;
		//	default:
		//		break;
		//}

		m_Require[i].m_bOK = g_pCheckManager->CheckCondition(iRequireType, iID, iValue1, iValue2);
	}

	for(int i = 0; i < QUEST_MAX_REQUIRE; i++) {
		if(m_Require[i].m_bOK == false) {
			bResult = false;
			break;
		}
	}

	if(bResult == false) {
		if(m_iVisible > 0)
			m_iUiShow = QUEST_UI_SHOW_DISABLE;
		else 
			m_iUiShow = QUEST_UI_SHOW_NULL;
	}
	
	//return bResult;
	return m_iUiShow;
}


REWARD_RANK SPQuestAttr::IsRewardRequire()
{
	REQUIRE_TYPE iRequireType = REQUIRE_TYPE_NULL;
	int iID = 0;
	int iValue1 = 0;
	int iValue2 = 0;
	
	bool bCheck[QUEST_MAX_RANK_REQUIRE];
	m_iRank = REWARD_RANK_NULL;					//완료 조건 검사시 랭크 초기화... 

	int i = 0;

	for(i = 0; i < QUEST_MAX_RANK; i++) {
		for(int j = 0; j < QUEST_MAX_RANK_REQUIRE; j++) {
			bCheck[j] = false;
			
			if( g_pCheckManager->CheckCondition(m_RewordRequire[i][j].m_iRequireStat, 
				m_RewordRequire[i][j].m_iRequireStatValue, 
				m_RewordRequire[i][j].m_iValue1,
				m_RewordRequire[i][j].m_iValue2) == false)
			{				
				continue;
			}
			else {
				bCheck[j] = true;
			}
		}

		bool bOk = true;
		for(int k = 0; k < QUEST_MAX_RANK_REQUIRE; k++) {
			bOk &= bCheck[k];
		}

		if(bOk) {
			m_iRank = (REWARD_RANK)i;
			DXUTOutputDebugString("Quest[%d] IsRewardRequire [%d]\n", m_iQuestID, m_iRank);			
			return m_iRank;
		}
	}
	
	DXUTOutputDebugString("Quest[%d] IsRewardRequire Find Fail [%d]\n", m_iQuestID, m_iRank);
	return m_iRank;
}


bool SPQuestAttr::SetCheckMissionItem(bool bMsg /*= true*/)
{
	int i = 0; 
	for(i = 0; i < QUEST_MAX_MISSION; i++) {
		if(m_Mission[i].m_iMissionType == MISSION_TYPE_ITEM) {
			int iCurCount = g_pCheckManager->GetTotalPCItemCount(m_Mission[i].m_iMissionTypeID);

			m_Mission[i].m_iCurCount = iCurCount;

			if(iCurCount >= m_Mission[i].m_iMissionCount) {				//체크할값이 필요치이상인 경우
				m_Mission[i].m_bClear = true;
				m_Mission[i].m_iCurCount = m_Mission[i].m_iMissionCount;
			}
		}
	}

	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUEST , SPIM_QUEST_REFRESH, (WPARAM)m_iQuestID , (LPARAM)true );	// AJJIYA [10/25/2005]
	
	bool bResult = true;
	for(int i = 0; i < QUEST_MAX_MISSION; i++) {
		if(m_Mission[i].m_iMissionType == MISSION_TYPE_NULL)
			continue;

		if(m_Mission[i].m_bClear == false) {
			bResult = false;
			return false;
		}
	}

	//주어진 퀘스트의 모든 미션이 클리어된 상태.
	//여기는 플레이어 진행 퀘스트 목록의 아이템만이 들어올수 있다.
	if(bResult) {		
		
		if(bMsg && m_iUiShow != QUEST_UI_SHOW_COMPLETE) {
			if(g_pResourceManager->GetGlobalString(6001002) != NULL) {
				std::string strMsg = g_pResourceManager->GetGlobalString(6001002);
				char szMsg[512];
				wsprintf(szMsg, strMsg.c_str(), m_strQuestName.c_str());
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
			}			
		}

		m_iUiShow = QUEST_UI_SHOW_COMPLETE;
	}

	return true;
}


void SPQuestAttr::SetQuestUIShow(QUEST_UI_SHOW iUiShow /*= QUEST_UI_SHOW_NULL*/)
{
	m_iUiShow = iUiShow;
}


QUEST_UI_SHOW SPQuestAttr::GetQuestUIShow()
{
	return m_iUiShow;
}



