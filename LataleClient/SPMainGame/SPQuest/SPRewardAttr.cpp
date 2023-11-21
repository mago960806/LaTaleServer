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
#include "SPRewardAttr.h"



SPRewardAttr::SPRewardAttr()
{
	Clear();
}


SPRewardAttr::~SPRewardAttr()
{
	Clear();
}


void SPRewardAttr::Clear()
{
	m_iID			= 0;
	m_iExp			= 0;
	m_iEly			= 0;
	m_iFame			= 0;
	
	int i = 0;

	for(i = 0; i < QUESTREWARD_MAX_EFFECTID; i++){
		m_iEffect[i] = 0;
	}
	
	for(i = 0; i < QUESTREWARD_MAX_ITEM; i++) {
		m_Item[i].Clear();
	}
}


void SPRewardAttr::SetRewardAttr(int iID, int iExp, int iEly, int iFame)
{
	m_iID			= iID;
	m_iExp			= iExp;
	m_iEly			= iEly;
	m_iFame			= iFame;
}

