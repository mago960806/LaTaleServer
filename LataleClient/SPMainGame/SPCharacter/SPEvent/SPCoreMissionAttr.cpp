#include "SPCommon.h"
#include "SPUtil.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"

#include "SPEventDEF.h"
#include "SPCoreMissionAttr.h"

#include "SPCheckManager.h"

#include "SPDebug.h"


SPCoreMissionAttr::SPCoreMissionAttr()
{
	Clear();
}


SPCoreMissionAttr::~SPCoreMissionAttr()
{
	Clear();
}


void SPCoreMissionAttr::Clear()
{
	m_iID		 = 0;
	m_iType		 = CORE_MISSION_TYPE_NULL;
	m_iSavePos	 = -1;
	for(int i = 0; i < MAX_CORE_EVENT_REQUIRE; i++)	 {
		m_Require[i].Clear();
	}	
}


bool SPCoreMissionAttr::CheckRequire()
{
	int iRet = -1;

	bool bRet = true;
	for(int i = 0; i < MAX_CORE_EVENT_REQUIRE; i++) {
		if( g_pCheckManager->CheckCondition(m_Require[i].m_iRequireStat, 
			m_Require[i].m_iRequireStatValue, 
			m_Require[i].m_iValue1,
			m_Require[i].m_iValue2) == false)
		{				
			//실패 메시지 출력
			if(m_iType == CORE_MISSION_TYPE_QUEST){
				if(g_pResourceManager->GetGlobalString(3007001)){
					std::string strMsg = g_pResourceManager->GetGlobalString(3007001);
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str());
				}
			}
			else if(m_iType == CORE_MISSION_TYPE_ITEM){
				if(g_pResourceManager->GetGlobalString(3007002)){
					std::string strMsg = g_pResourceManager->GetGlobalString(3007002);
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str());
				}				
			}
			return false;
			//continue;
		}
		else {
			//bCheck[j] = true;
			//iRet = i;
			return true;
		}
	}
	
	DXUTOutputDebugString("CoreMission[%d] CheckRequire Find Fail [%d]\n", m_iID, iRet);
	return false;
}


void SPCoreMissionAttr::SetSavePos(int iIndex)
{
	int iPos = iIndex - 1;
	if(iPos < 0)
		m_iSavePos = -1;
	else 
		m_iSavePos = iPos;
}


