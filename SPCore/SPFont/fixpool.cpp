#include "FixPool.h"
#include "DXUtil.h"

CFixPool::CFixPool(DWORD dwEleSize,DWORD dwBaseEles)
{
	m_dwEleSize		= dwEleSize;
	m_dwBaseEles	= dwBaseEles;
	m_pBaseAddr		= NULL;
	m_dwCurEles		= 0;
	m_dwMaxEles		= 0;

	m_pBaseAddr = new BYTE[dwEleSize * dwBaseEles];
	for(DWORD i = 0;i < dwBaseEles;i++)
	{
		m_vFreeBase.push_back(m_pBaseAddr + i * dwEleSize);
	}
}

CFixPool::~CFixPool(void)
{
	for(size_t n = 0;n < m_vFreeExt.size();n++)
	{
		SAFE_DELETE_ARRAY(m_vFreeExt[n]);
	}
	SAFE_DELETE_ARRAY(m_pBaseAddr);
}

BYTE* CFixPool::NewEle(void)
{
	BYTE *p = NULL;
	if(m_vFreeBase.size() > 0)
	{
		p = m_vFreeBase.back();
		m_vFreeBase.pop_back();
	}
	else if(m_vFreeExt.size() > 0)
	{
		p = m_vFreeExt.back();
		m_vFreeExt.pop_back();
	}
	else
		p = new BYTE[m_dwEleSize];

	m_dwCurEles++;
	if(m_dwCurEles > m_dwMaxEles)
		m_dwMaxEles = m_dwCurEles;

	return p;
}

void CFixPool::DelEle(BYTE *pEle)
{
	if(!pEle)
		return;

	if(m_dwCurEles <= 0)
		return;

	if(pEle >= m_pBaseAddr && pEle < (m_pBaseAddr + m_dwEleSize * m_dwBaseEles))
	{
		if((DWORD)(pEle - m_pBaseAddr) % m_dwEleSize)
			return;
		m_vFreeBase.push_back(pEle);
	}
	else
		m_vFreeExt.push_back(pEle);

	m_dwCurEles--;
}