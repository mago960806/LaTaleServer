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

#include "SPGiftAttr.h"



SPGiftAttr::SPGiftAttr() 
{
	Clear();
}


SPGiftAttr::~SPGiftAttr()
{
}


void SPGiftAttr::Clear()
{
	m_iKey			= 0;
	m_strName		= "";
	m_strName.clear();
	m_strDesc		= "";
	m_strDesc.clear();
	m_iItemID		= 0;
	m_iItemRare		= 0;
	m_iItemStack	= 0;
	m_iEly			= 0;
}


void SPGiftAttr::Init(UINT64 iKeyID) 
{
	Clear();
	m_iKey = iKeyID;
}
