#include "SPCommon.h"
#include "SPUtil.h"

#include <algorithm>

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
#include "SPGiftArchive.h"

/**
* 주어진 Key의 GiftAttr 검사 <br>
*/
struct _IsEqualGift : binary_function<SPGiftAttr*, INT64, bool> {
	bool operator() (SPGiftAttr* pGiftAttr, INT64 iKey) const
	{
		if(pGiftAttr->m_iKey == iKey)
			return true;
		return false;
	}
} IsEqualGift;


SPGiftArchive::SPGiftArchive()
{
}


SPGiftArchive::~SPGiftArchive()
{
	DeleteAllGift();
}


void SPGiftArchive::DeleteAllGift()
{
	std::vector<SPGiftAttr*>::iterator iter;
	iter = m_vpGift.begin();
	for(; iter != m_vpGift.end(); ++iter) {
		delete (*iter);
		//SAFE_DELETE((*iter));
	}

	m_vpGift.clear();
}


int SPGiftArchive::AddGift(SPGiftAttr* pGiftAttr)
{
	if(pGiftAttr == NULL)		return -1;

	m_vpGift.push_back(pGiftAttr);

	int iSize = static_cast<int>(m_vpGift.size());

	return iSize;
}


int	SPGiftArchive::GetSize()
{
	return static_cast<int>(m_vpGift.size());
}


SPGiftAttr* SPGiftArchive::GetGiftAttr(int iIndex)
{
	int iSize = static_cast<int>(m_vpGift.size());

	if(iSize == 0)
		return NULL;

	if(iIndex < 0 || iIndex >= iSize)
		return NULL;

	return m_vpGift.at(iIndex);
}


bool SPGiftArchive::DeleteGift(INT64 iKey)
{
	std::vector<SPGiftAttr*>::iterator iter;
	iter = std::find_if(m_vpGift.begin(), m_vpGift.end(),
		std::bind2nd(IsEqualGift, iKey));	

	if(iter != m_vpGift.end())
	{
		SAFE_DELETE( *iter );
		m_vpGift.erase(iter);		
	}
	else {
		return false;
	}

	//return GetSize();
	return true;
}

