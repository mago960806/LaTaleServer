
#pragma once

#include <windows.h>

#include <vector>

class CFixPool
{
private:
	typedef std::vector<BYTE *> VPoint;

	DWORD	m_dwEleSize;			// 每个单元大小
	DWORD	m_dwBaseEles;			// 基本单元个数
	BYTE*	m_pBaseAddr;			// 基本池开始位置

	DWORD	m_dwCurEles;			// 当前单元个数
	DWORD	m_dwMaxEles;			// 最大单元个数

	VPoint	m_vFreeBase;			// 基本空闲池
	VPoint	m_vFreeExt;				// 扩展空闲池

public:
	CFixPool(DWORD dwEleSize,DWORD dwBaseEles);
	~CFixPool(void);

	BYTE*	NewEle(void);
	void	DelEle(BYTE *pEle);
};