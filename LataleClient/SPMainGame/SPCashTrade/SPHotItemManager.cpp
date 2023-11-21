#include "SPCommon.h"
#include "SPUtil.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "Packet.h"

////
//#include "SPRenderModelDEF.h"
//#include "SPRenderModel.h"
//#include "SPGOBClusterDef.h"
//#include "SPAvatarModelUnitDef.h"
//#include "SPGOBModel.h"
//#include "SPGOBModelUnit.h"
//#include "SPAvatarModelUnit.h"
//#include "SPGOBCluster.h"
//#include "SPGameObject.h"
//#include "SPGOBStatus.h"
//#include "SPGOBCoordPhysics.h"
//#include "SPGOBModelUnit.h"			
//#include "SPPlayerEquipInfo.h"
//#include "SPGOBStatus.h"			
//#include "SPPlayerInvenArchive.h"	
//#include "SPPlayerStatusModel.h"	
//#include "SPPlayerEquipInfo.h"
//#include "SPItem.h"
//#include "SPItemAttr.h"
//#include "SPItemStatus.h"
//#include "SPItemCluster.h"
//#include "SPSkill.h"
//#include "SPSkillArchive.h"
//#include "SPMotionStatus.h"
//#include "SPSkillManager.h"
//#include "SPPlayer.h"
//#include "SPGOBManager.h"

#include "SPCheckManager.h"

#include "SPHotItemInfo.h"
#include "SPHotItemManager.h"
//#include "SPCashTradeManager.h"

#include "SPDebug.h"


SPHotItemManager::SPHotItemManager()
: m_bListShow (false)
{
	//[2008/4/15] - Test
	m_bListShow = true;
}


SPHotItemManager::~SPHotItemManager()
{
	DeleteAllHotItem();
}


void SPHotItemManager::DeleteAllHotItem()
{
	std::vector<SPHotItemInfo*>::iterator	 iter = m_vpHotItemList.begin();

	for(; iter != m_vpHotItemList.end(); ++iter) {
		delete (*iter);
	}
	m_vpHotItemList.clear();
}


bool SPHotItemManager::IsShow()
{
	if(m_bListShow && GetHotItemSize())
		return true;

	return false;
}


bool SPHotItemManager::AddHotItem(CASHSHOP_HOT_INFO* pInfo)
{
	if(pInfo) {
		SPHotItemInfo* pHotItemInfo = new SPHotItemInfo(pInfo);		
		
		if(pHotItemInfo->IsShow()) {
			m_vpHotItemList.push_back(pHotItemInfo);
			return true;
		}
		else {
			SAFE_DELETE(pHotItemInfo);
			return false;
		}
	}

	return false;
}


int SPHotItemManager::GetHotItemSize()
{
	int iSize = static_cast<int>(m_vpHotItemList.size());
	return iSize;
}


SPHotItemInfo* SPHotItemManager::GetHotItem(int iIndex)
{
	if(iIndex < 0 || iIndex >= GetHotItemSize())
		return NULL;

	return m_vpHotItemList.at(iIndex);
}



