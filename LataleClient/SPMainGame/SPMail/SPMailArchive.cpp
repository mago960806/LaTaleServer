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

#include "SPMailAttr.h"
#include "SPMailArchive.h"


/**
* 주어진 ID의 MailAttr 검사 <br>
*/
struct _IsEqualMail : binary_function<SPMailAttr*, UINT64, bool> {
	bool operator() (SPMailAttr* pMailAttr, UINT64 iId) const
	{
		if(pMailAttr->m_pHeader->uiMailID == iId)
			return true;
		return false;
	}
} IsEqualMail;

/*
* 내림차순 소트 <br>
*/
struct _SortDescendMailID : public binary_function<SPMailAttr*, SPMailAttr*, bool>
{
	bool operator() ( SPMailAttr* pLeftMail, SPMailAttr* pRightMail) const
	{
		if(pLeftMail->m_pHeader->uiMailID > pRightMail->m_pHeader->uiMailID){
			return true;
		}
		return false;
	}
} SortDescendMailID;


SPMailArchive::SPMailArchive()
{
}


SPMailArchive::~SPMailArchive()
{
	DeleteAllMail();
}


int SPMailArchive::AddMail(SPMailAttr* pMail)
{
	if(pMail) {
		//[2006/5/8]	ID 중복 체크
		if(FindMail(pMail->m_pHeader->uiMailID)) {
			return -1;
		}

		m_vpMail.push_back(pMail);
		return static_cast<int>(m_vpMail.size());
	}

	return -1;
}


int SPMailArchive::AddNewMail(SPMailAttr* pMail)
{
	if(pMail) {
		std::vector<SPMailAttr*>::iterator iter = m_vpMail.begin();
		m_vpMail.insert(iter, pMail);
		return static_cast<int>(m_vpMail.size());
	}
	return -1;
}


void SPMailArchive::DeleteAllMail()
{
	std::vector<SPMailAttr*>::iterator iter;
	iter = m_vpMail.begin();
	for(; iter != m_vpMail.end(); ++iter) {
		delete (*iter);
	}

	m_vpMail.clear();
}


int SPMailArchive::GetSize()
{
	return static_cast<int>(m_vpMail.size());
}

SPMailAttr* SPMailArchive::GetMailAttr(int iIndex)
{
	int iSize = static_cast<int>(m_vpMail.size());

	if(iSize == 0)
		return NULL;

	if(iIndex < 0 || iIndex >= iSize)
		return NULL;

	return m_vpMail.at(iIndex);
}


SPMailAttr* SPMailArchive::FindMail(UINT64 iMailID)
{
	std::vector<SPMailAttr*>::iterator iter;
	iter = std::find_if(m_vpMail.begin(), m_vpMail.end(),
		std::bind2nd(IsEqualMail, iMailID));

	if(iter != m_vpMail.end()) {
		return (*iter);
	}
	else {
		return NULL;
	}
}


bool SPMailArchive::ClearMailHeader(UINT64 iMailID)
{
	std::vector<SPMailAttr*>::iterator iter;
	iter = std::find_if(m_vpMail.begin(), m_vpMail.end(),
		std::bind2nd(IsEqualMail, iMailID));

	if(iter != m_vpMail.end()) {
		(*iter)->m_pHeader->uiItemID = 0;
		(*iter)->m_pHeader->uiItemCount = 0;
		(*iter)->m_pHeader->uiItemNo = 0;
		(*iter)->m_pHeader->uiEly = 0;

		return true;
	}
	else {
		return false;
	}
}


bool SPMailArchive::DeleteMail(UINT64 iMailID)
{
	std::vector<SPMailAttr*>::iterator iter;
	iter = std::find_if(m_vpMail.begin(), m_vpMail.end(),
		std::bind2nd(IsEqualMail, iMailID));	

	if(iter != m_vpMail.end())
	{
		SAFE_DELETE( *iter );
		m_vpMail.erase(iter);		
	}
	else {
		return false;
	}
	
	return true;
}


bool SPMailArchive::SetSort()
{
	std::sort(m_vpMail.begin(), m_vpMail.end(), SortDescendMailID);	
	return true;
}


bool SPMailArchive::CheckReadMail()
{
	std::vector<SPMailAttr*>::iterator iter;
	iter = m_vpMail.begin();

	for(; iter != m_vpMail.end(); ++iter) {
		if((*iter)->m_pHeader->uiReadFlag == false)
			return false;
	}

	return true;
}