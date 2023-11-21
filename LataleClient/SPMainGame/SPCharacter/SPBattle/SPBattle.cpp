
#include <windows.h>

#include "SPGOBClusterDef.h"
#include "SPMonsterModelUnitDef.h"

#include "SPBattle.h"

//////////////////////////////////////////////////////////////////////////
tagBATTLE::tagBATTLE()
: m_nIndex(0)
{
	m_rtRange.left = m_rtRange.top = m_rtRange.right = m_rtRange.bottom = 0;
}

tagBATTLE& tagBATTLE::operator=(const tagBATTLE &src)
{
	m_nIndex = src.m_nIndex;
	m_rtRange.left = src.m_rtRange.left;
	m_rtRange.top = src.m_rtRange.top;
	m_rtRange.right = src.m_rtRange.right;
	m_rtRange.bottom = src.m_rtRange.bottom;
	return *this;
}

//////////////////////////////////////////////////////////////////////////

tagATTACKINFO& tagATTACKINFO::operator=(const tagATTACKINFO &src)
{
	tagBATTLE::operator=( src );
	m_AttackType = src.m_AttackType;
	return *this;
}

tagBEATTACKEDINFO& tagBEATTACKEDINFO::operator=(const tagBEATTACKEDINFO &src)
{
	tagBATTLE::operator=( src );
	m_fAccumulatedTickTime = src.m_fAccumulatedTickTime;
	return *this;
}

tagAPCBEATTACKEDINFO& tagAPCBEATTACKEDINFO::operator=(const tagAPCBEATTACKEDINFO &src)
{
	tagBEATTACKEDINFO::operator=( src );
	m_iBeAttackedIndex = src.m_iBeAttackedIndex;
	return *this;
}

//////////////////////////////////////////////////////////////////////////

SPAttack::SPAttack()
: m_uiAniIndex(0),
m_nAttackPointNum(0),
m_fCallTickTime(0) ,
m_fAccumulateTime(0),
m_iAttackType(0)
{
}

SPAttack::SPAttack(unsigned int index) 
: m_uiAniIndex(index),
m_nAttackPointNum(0),
m_fCallTickTime(0),
m_fAccumulateTime(0),
m_iAttackType(0)
{
}

void	SPAttack::Clear()	
{
	m_uiAniIndex=0;
	m_nAttackPointNum=0;
	m_fCallTickTime=-1.0f;
	m_fAccumulateTime=0.0f;
	m_iAttackType = 0;
	m_AttackInfo.clear();
}

SPAttack& SPAttack::operator=(const SPAttack &src)
{
	m_uiAniIndex		= src.m_uiAniIndex;
	m_nAttackPointNum	= src.m_nAttackPointNum;
	m_fCallTickTime		= src.m_fCallTickTime;
	m_fAccumulateTime	= src.m_fAccumulateTime;
	m_iAttackType		= src.m_iAttackType;
	m_AttackInfo		= src.m_AttackInfo;

	return *this;
}

#ifdef _DEBUG
void	SPAttack::Dump()
{
	char buf[MAX_PATH];
	sprintf(buf,"\tAniIndex=[%d] nPoints=[%d] callTickTime=[%f] accumulateTime=[%f]\n",	m_uiAniIndex,m_nAttackPointNum,m_fCallTickTime,m_fAccumulateTime);
	OutputDebugStr(buf);

	std::vector< ATTACKINFO >::const_iterator pos;
	pos = m_AttackInfo.begin();

	ATTACKINFO* pAttackInfo = NULL;
	while(pos!= m_AttackInfo.end())
	{
		pAttackInfo = (ATTACKINFO*)&(*pos);
		if(pAttackInfo)
		{
			sprintf(buf,"\t\t%d [%d,%d,%d,%d] %d\n",
						pAttackInfo->m_nIndex,
						pAttackInfo->m_rtRange.left,
						pAttackInfo->m_rtRange.top,
						pAttackInfo->m_rtRange.right,
						pAttackInfo->m_rtRange.bottom,
						pAttackInfo->m_AttackType);
			OutputDebugStr(buf);
		}
		++pos;
	}
}

#endif

//////////////////////////////////////////////////////////////////////////

SPCharacterAttack& SPCharacterAttack::operator=(const SPCharacterAttack &src)
{		
	SPAttack::operator=( src );	
	return *this;
}

//////////////////////////////////////////////////////////////////////////

SPMonsterAttack& SPMonsterAttack::operator=(const SPMonsterAttack &src)
{
	SPAttack::operator=( src );
	return *this;
}

//////////////////////////////////////////////////////////////////////////

SPBeAttacked::SPBeAttacked() 
:m_uiAniIndex(0)
,m_nTotalFrame(0)
{
	m_fAccumulateTime = 0.0f;
}

SPBeAttacked::SPBeAttacked(unsigned int index)
:m_uiAniIndex(index) 
,m_nTotalFrame(0)
{
	m_fAccumulateTime = 0.0f;
}

void	SPBeAttacked::Clear()	
{
	m_uiAniIndex = 0;
}

SPBeAttacked& SPBeAttacked::operator=(const SPBeAttacked &src)
{
	m_uiAniIndex		= src.m_uiAniIndex;  
	m_nTotalFrame	= src.m_nTotalFrame;	
	m_fAccumulateTime = src.m_fAccumulateTime;
	
	return *this;
}

//////////////////////////////////////////////////////////////////////////

void SPCharacterBeAttacked::Clear() 
{ 
	SPBeAttacked::Clear(); 
	m_BeAttackInfo.clear();
}

SPCharacterBeAttacked& SPCharacterBeAttacked::operator=(const SPCharacterBeAttacked &src)
{
	SPBeAttacked::operator = ( src ) ;
	m_BeAttackInfo	= src.m_BeAttackInfo;

	return *this;
}


#ifdef _DEBUG
void	SPCharacterBeAttacked::Dump()
{
	char buf[MAX_PATH];
	sprintf(buf,"\tAniIndex=[%d] nTotalFrames=[%d] fAccumulateTime=[%f]\n",m_uiAniIndex,m_nTotalFrame,m_fAccumulateTime);
	OutputDebugStr(buf);

	std::vector<APCBEATTACKEDINFO>::const_iterator pos;
	pos = m_BeAttackInfo.begin();

	APCBEATTACKEDINFO* pPCBeAttackedInfo = NULL;
	while(pos!= m_BeAttackInfo.end())
	{
		pPCBeAttackedInfo = (APCBEATTACKEDINFO*)&(*pos);
		if(pPCBeAttackedInfo)
		{
			sprintf(buf,"\t\t%d %d [%d,%d,%d,%d] %f\n",
				pPCBeAttackedInfo->m_iBeAttackedIndex,
				pPCBeAttackedInfo->m_nIndex,
				pPCBeAttackedInfo->m_rtRange.left,
				pPCBeAttackedInfo->m_rtRange.top,
				pPCBeAttackedInfo->m_rtRange.right,
				pPCBeAttackedInfo->m_rtRange.bottom,
				pPCBeAttackedInfo->m_fAccumulatedTickTime);
			OutputDebugStr(buf);
		}
		++pos;
	}
}

#endif
//////////////////////////////////////////////////////////////////////////

void SPMonsterBeAttacked::Clear()
{ 
	SPBeAttacked::Clear(); 
	m_BeAttackInfo.clear(); 
	m_nMonsterSizeIndex=MON_TINY;
};

SPMonsterBeAttacked& SPMonsterBeAttacked::operator=(const SPMonsterBeAttacked &src)
{
	SPBeAttacked::operator = ( src ) ;
	m_BeAttackInfo	= src.m_BeAttackInfo;
	m_nMonsterSizeIndex = src.m_nMonsterSizeIndex;

	return *this;
}

#ifdef _DEBUG
void	SPMonsterBeAttacked::Dump()
{
	char buf[MAX_PATH];
	sprintf(buf,"\tAniIndex=[%d] nTotalFrames=[%d] fAccumulateTime=[%f] nMonsterSizeIndex=[%d]\n",m_uiAniIndex,m_nTotalFrame,m_fAccumulateTime,m_nMonsterSizeIndex);
	OutputDebugStr(buf);

	std::vector<APCBEATTACKEDINFO>::const_iterator pos;
	pos = m_BeAttackInfo.begin();

	BEATTACKEDINFO* pBeAttackedInfo = NULL;
	while(pos!= m_BeAttackInfo.end())
	{
		pBeAttackedInfo = (BEATTACKEDINFO*)&(*pos);
		if(pBeAttackedInfo)
		{
			sprintf(buf,"\t\t%d [%d,%d,%d,%d] %f\n",
				pBeAttackedInfo->m_nIndex,
				pBeAttackedInfo->m_rtRange.left,
				pBeAttackedInfo->m_rtRange.top,
				pBeAttackedInfo->m_rtRange.right,
				pBeAttackedInfo->m_rtRange.bottom,
				pBeAttackedInfo->m_fAccumulatedTickTime);
			OutputDebugStr(buf);
		}
		++pos;
	}
}

#endif