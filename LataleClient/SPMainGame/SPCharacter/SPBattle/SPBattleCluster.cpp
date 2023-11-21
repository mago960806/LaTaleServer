#include <windows.h>

#include <string>
#include <functional>
#include <algorithm>

#ifdef _CHARTOOL
#include <fstream>
#endif

#include "SPMonsterModelUnitDef.h"
#include "SPGOBClusterDef.h"

#include "SPBattle.h"
#include "SPBattleCluster.h"
#include "SPGameObjectDEF.h"

using namespace std;
using namespace stdext;

SPBattleCluster*	SPBattleCluster::m_pInstance=0;

SPBattleCluster* g_pBattleCluster = NULL; // by metalgeni


struct _IsEqualAPCAttack : binary_function<SPCharacterAttack, unsigned int, bool> {
	bool operator() (const SPCharacterAttack& BattleData, unsigned int index) const
	{		
		return (BattleData.m_uiAniIndex == index);
	}
} IsEqualAPCAttack;

struct _IsEqualMonsterAttack : binary_function<SPMonsterAttack, unsigned int, bool> {
	bool operator() (const SPMonsterAttack& BattleData, unsigned int index) const
	{		
		return (BattleData.m_uiAniIndex == index);
	}
} IsEqualMonsterAttack;

struct _IsEqualAPCBeAttacked : binary_function<SPCharacterBeAttacked, unsigned int, bool> {
	bool operator() (const SPCharacterBeAttacked& BattleData, unsigned int index) const
	{		
		return (BattleData.m_uiAniIndex == index);
	}
} IsEqualAPCBeAttacked;

struct _IsEqualMonsterBeAttacked : binary_function<SPMonsterBeAttacked, unsigned int, bool> {
	bool operator() (const SPMonsterBeAttacked& BattleData, unsigned int index) const
	{		
		return (BattleData.m_uiAniIndex == index);
	}
} IsEqualMonsterBeAttacked;

//////////////////////////////////////////////////////////////////////////

SPBattleCluster::SPBattleCluster()
{
	g_pBattleCluster = this;
	m_fCurrCallTickTime = -1.0f;
}

SPBattleCluster::~SPBattleCluster()
{
	Clear();
}

void SPBattleCluster::Release()
{
	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

SPBattleCluster*	SPBattleCluster::GetInstance()
{	
	if(m_pInstance==0)
		m_pInstance = new SPBattleCluster;
	return (SPBattleCluster*)m_pInstance;
}

//////////////////////////////////////////////////////////////////////////
void	SPBattleCluster::Clear()
{
	ClearCharacter();
	ClearMonster();
}

void	SPBattleCluster::ClearCharacter()
{
	m_hmCharacterAttack.clear();
	m_hmCharacterBeAttacked.clear();
}

void	SPBattleCluster::ClearMonster()
{
	hash_map< unsigned int, vector< SPMonsterAttack > >::iterator iterAttack;
	hash_map< unsigned int, vector< SPMonsterBeAttacked > >::iterator iterBeAttack;
	
	iterAttack = m_hmMonsterAttack.begin();
	while( iterAttack != m_hmMonsterAttack.end() )
	{
		iterAttack->second.clear();
		++iterAttack;
	}
	m_hmMonsterAttack.clear();
	
	iterBeAttack = m_hmMonsterBeAttacked.begin();
	while( iterBeAttack != m_hmMonsterBeAttacked.end() )
	{
		iterBeAttack->second.clear();
		++iterBeAttack;
	}
	m_hmMonsterBeAttacked.clear();
}

void	SPBattleCluster::ClearAttack()
{
	m_hmCharacterAttack.clear();

	hash_map< unsigned int, vector< SPMonsterAttack > >::iterator iterMonAttack;
	iterMonAttack = m_hmMonsterAttack.begin();
	while( iterMonAttack != m_hmMonsterAttack.end() )
	{
		iterMonAttack->second.clear();
		++iterMonAttack;
	}
	m_hmMonsterAttack.clear();
}

void	SPBattleCluster::ClearBeAttacked()
{
	m_hmCharacterBeAttacked.clear();

	hash_map< unsigned int, vector< SPMonsterBeAttacked > >::iterator iteMonBeAttack;
	iteMonBeAttack = m_hmMonsterBeAttacked.begin();
	while( iteMonBeAttack != m_hmMonsterBeAttacked.end() )
	{
		iteMonBeAttack->second.clear();
		++iteMonBeAttack;
	}
	m_hmMonsterBeAttacked.clear();
}

//////////////////////////////////////////////////////////////////////////

bool	SPBattleCluster::GetAttack(unsigned int uiIndex, SPCharacterAttack **BattleData) 
{
	hash_map< unsigned int, SPCharacterAttack >::iterator iter;
	iter = m_hmCharacterAttack.find(uiIndex);
	if( iter == m_hmCharacterAttack.end() )
	{
		m_fCurrCallTickTime = -1.0f;
		return false;
	}

	(*BattleData) = &iter->second;
	m_fCurrCallTickTime = iter->second.m_fCallTickTime;

	return true;
}

bool	SPBattleCluster::GetAttack(ANIMATION_INDEX index,
	SPMonsterAttack **BattleData, unsigned int uiClassID) 
{
	if( m_hmMonsterAttack.empty() )
		return false;

	hash_map< unsigned int, vector< SPMonsterAttack > >::iterator iterKey = m_hmMonsterAttack.find(uiClassID);
	if( iterKey == m_hmMonsterAttack.end() )
		return false;

	vector<SPMonsterAttack>::iterator iterValue = iterKey->second.end();
	iterValue = find_if(iterKey->second.begin(), iterKey->second.end(),
		std::bind2nd( IsEqualMonsterAttack, index ) );

	if( iterValue == iterKey->second.end() )
	{
		m_fCurrCallTickTime = -1.0f;
		return false;
	}

	(*BattleData) = &(*iterValue);
	m_fCurrCallTickTime = (*iterValue).m_fCallTickTime;

	return true;
}

bool	SPBattleCluster::GetAttack(unsigned int uiIndex, SPCharacterBeAttacked **BattleData) 
{
	hash_map< unsigned int, SPCharacterBeAttacked >::iterator iter;
	iter = m_hmCharacterBeAttacked.find(uiIndex);
	if( iter == m_hmCharacterBeAttacked.end() )
		return false;

	(*BattleData) = &iter->second;
	return true;
}

bool	SPBattleCluster::GetAttack(ANIMATION_INDEX index,
	SPMonsterBeAttacked **BattleData, unsigned int uiClassID) 
{
	if( m_hmMonsterBeAttacked.empty() )
		return false;

	hash_map< unsigned int, vector< SPMonsterBeAttacked > >::iterator iterKey = m_hmMonsterBeAttacked.find(uiClassID);
	if( iterKey == m_hmMonsterBeAttacked.end() )
		return false;

	vector<SPMonsterBeAttacked>::iterator iterValue = iterKey->second.end();
	iterValue = find_if(iterKey->second.begin(), iterKey->second.end(),
		std::bind2nd(IsEqualMonsterBeAttacked, index));
	if( iterValue == iterKey->second.end() )
		return false;

	(*BattleData) = &(*iterValue);
	return true;
}

bool SPBattleCluster::IsMonsterAttack(unsigned int uiClassID)
{
	if( m_hmMonsterAttack.empty() )
		return false;

	hash_map< unsigned int, vector< SPMonsterAttack > >::iterator iterKey = m_hmMonsterAttack.find(uiClassID);
	if( iterKey == m_hmMonsterAttack.end() )
		return false;

	return true;
}

bool SPBattleCluster::IsMonsterBeAttacked(unsigned int uiClassID)
{
	if( m_hmMonsterBeAttacked.empty() )
		return false;

	hash_map< unsigned int, vector< SPMonsterBeAttacked > >::iterator iterKey = m_hmMonsterBeAttacked.find(uiClassID);
	if( iterKey == m_hmMonsterBeAttacked.end() )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool	SPBattleCluster::SetAttack(unsigned int uiAniIndex, const SPCharacterAttack BattleData)
{
	hash_map< unsigned int, SPCharacterAttack >::iterator iter;
	iter = m_hmCharacterAttack.find(uiAniIndex);
	if( iter != m_hmCharacterAttack.end() )
		iter->second = BattleData;
	else
	{
		m_hmCharacterAttack.insert(
			hash_map< unsigned int, SPCharacterAttack >::value_type(uiAniIndex, BattleData));
	}
	
	return true;
}

bool	SPBattleCluster::SetAttack(const SPMonsterAttack BattleData, unsigned int uiClassID)
{
	hash_map< unsigned int, vector< SPMonsterAttack > >::iterator iterKey = m_hmMonsterAttack.find(uiClassID);
	if( iterKey == m_hmMonsterAttack.end() )
	{
		vector< SPMonsterAttack > vMonsterAttack;
		vMonsterAttack.push_back(BattleData);

		m_hmMonsterAttack.insert(
			hash_map< unsigned int, vector< SPMonsterAttack > >::value_type(uiClassID, vMonsterAttack));
	}
	else
		iterKey->second.push_back(BattleData);

	return true;
}

bool	SPBattleCluster::SetAttack(unsigned int uiAniIndex, const SPCharacterBeAttacked BattleData)
{
	hash_map< unsigned int, SPCharacterBeAttacked >::iterator iter;
	iter = m_hmCharacterBeAttacked.find(uiAniIndex);
	if( iter == m_hmCharacterBeAttacked.end() )
	{
		m_hmCharacterBeAttacked.insert(
			hash_map< unsigned int, SPCharacterBeAttacked >::value_type(uiAniIndex, BattleData));
	}
	else
		iter->second = BattleData;
	
	return true;
}

bool	SPBattleCluster::SetAttack(const SPMonsterBeAttacked BattleData, unsigned int uiClassID)
{
	hash_map< unsigned int, vector< SPMonsterBeAttacked > >::iterator iterKey = m_hmMonsterBeAttacked.find(uiClassID);

	if( iterKey == m_hmMonsterBeAttacked.end() )
	{
		vector< SPMonsterBeAttacked > vMonsterBeAttacked;
		vMonsterBeAttacked.push_back(BattleData);

		m_hmMonsterBeAttacked.insert(
			hash_map< unsigned int, vector< SPMonsterBeAttacked > >::value_type(uiClassID, vMonsterBeAttacked));
	}
	else
		iterKey->second.push_back(BattleData);
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool SPBattleCluster::IsCollision(const RECT& rcBeAttacked,
	const vector<ATTACKINFO>& vAttackPoints, const FACING eFacing)
{
	if( vAttackPoints.empty() )
		return false;

	RECT rcAttacker;
	ATTACKINFO* pAttackInfo;
	vector< ATTACKINFO >::const_iterator iter = vAttackPoints.begin();
	while( iter != vAttackPoints.end() )
	{
		pAttackInfo = (ATTACKINFO*)&(*iter);
		if( pAttackInfo )
		{
			int iWidth = pAttackInfo->m_rtRange.right - pAttackInfo->m_rtRange.left;
			if( eFacing == FACING_RIGHT )
				rcAttacker.left = pAttackInfo->m_rtRange.left;
			else
				rcAttacker.left = -pAttackInfo->m_rtRange.left - iWidth;

			rcAttacker.right = rcAttacker.left + iWidth;

			rcAttacker.top = pAttackInfo->m_rtRange.top;
			rcAttacker.bottom = pAttackInfo->m_rtRange.top;

			if( !((rcBeAttacked.left > rcAttacker.right) || (rcBeAttacked.right < rcAttacker.left) ||
				(rcBeAttacked.top > rcAttacker.bottom) || (rcBeAttacked.bottom < rcAttacker.top)) )
				return true;
		}
		
		++iter;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

OBJECT_TYPE	SPBattleCluster::GetObjectType( ANIMATION_INDEX /*index*/)
{
	/*
	if(index == ANI_NULL)			return OBJECT_NULL;

	if(index < ANI_MONSTER_BEGIN)
	{
		return OBJECT_APC;
	}
	else if(index < ANI_NPC_01)
	{
		return OBJECT_MONSTER;
	}
	else 
	{
		return OBJECT_NPC;
	}
	*/

	return OBJECT_NULL;
}

//////////////////////////////////////////////////////////////////////////

bool	SPBattleCluster::IsCharacterAttackEmpty()
{
	return m_hmCharacterAttack.empty();
}

bool	SPBattleCluster::IsMonsterAttackEmpty()
{
	return m_hmMonsterAttack.empty();
}

bool	SPBattleCluster::IsCharacterBeAttackedEmpty()
{
	return m_hmCharacterBeAttacked.empty();
}

bool	SPBattleCluster::IsMonsterBeAttackedEmpty()
{
	return m_hmMonsterBeAttacked.empty();
}

//////////////////////////////////////////////////////////////////////////

#ifdef _CHARTOOL

bool	SPBattleCluster::GetAttack( unsigned int uiIndex, SPCharacterAttack &BattleData)
{
	hash_map< unsigned int, SPCharacterAttack >::iterator iter;
	iter = m_hmCharacterAttack.find(uiIndex);
	if( iter == m_hmCharacterAttack.end() )
	{
		m_fCurrCallTickTime = -1.0f;
		return false;
	}
	
	BattleData = iter->second;
	m_fCurrCallTickTime = iter->second.m_fCallTickTime;
	return true;
}

bool	SPBattleCluster::GetAttack( ANIMATION_INDEX index, SPMonsterAttack &BattleData) 
{
	return false;
}

bool	SPBattleCluster::GetAttack( unsigned int uiIndex, SPCharacterBeAttacked &BattleData) 
{
	hash_map< unsigned int, SPCharacterBeAttacked >::iterator iter;
	iter = m_hmCharacterBeAttacked.find(uiIndex);
	if( iter == m_hmCharacterBeAttacked.end() )
		return false;
	
	SPCharacterBeAttacked	spTable(iter->second.m_uiAniIndex);
	spTable = iter->second;
	spTable.m_BeAttackInfo.clear();

	int nIndex = 0;
	vector< APCBEATTACKEDINFO >::iterator iterAttackInfo=  iter->second.m_BeAttackInfo.begin();	
	while( iterAttackInfo != iter->second.m_BeAttackInfo.end())
	{
		for (int i=nIndex; i<(*iterAttackInfo).m_nIndex; i++, nIndex++)
		{
			APCBEATTACKEDINFO acpBeAttackedInfo;
			acpBeAttackedInfo = (*iterAttackInfo);
			acpBeAttackedInfo.m_nIndex = i+1;
			spTable.m_BeAttackInfo.push_back(acpBeAttackedInfo);			
		}		
		++iterAttackInfo;
	}

	BattleData = spTable;

	return true;
}

bool	SPBattleCluster::GetAttack(ANIMATION_INDEX index, SPMonsterBeAttacked &BattleData) 
{
	return false;
}

bool SPBattleCluster::IsMonsterAttack()
{
	return !m_hmMonsterAttack.empty();
}

void SPBattleCluster::DeleteMOBAttack(ANIMATION_INDEX eIndex, unsigned int uiClassID)
{
	if( m_hmMonsterAttack.empty() )
		return;

	hash_map< unsigned int, vector< SPMonsterAttack > >::iterator iterKey = m_hmMonsterAttack.find(uiClassID);
	if( iterKey == m_hmMonsterAttack.end() )
		return;
	
	vector< SPMonsterAttack >::iterator iterValue = iterKey->second.end();
	iterValue = find_if(iterKey->second.begin(), iterKey->second.end(),
		std::bind2nd(IsEqualMonsterAttack, eIndex));
	if( iterValue == iterKey->second.end() )
		return;

	iterKey->second.erase(iterValue);
}

void SPBattleCluster::DeleteMOBAttack(unsigned int uiClassID)
{
	if( m_hmMonsterAttack.empty() )
		return;

	hash_map< unsigned int, vector< SPMonsterAttack > >::iterator iterKey = m_hmMonsterAttack.find(uiClassID);
	if( iterKey == m_hmMonsterAttack.end() )
		return;

	iterKey->second.clear();
	m_hmMonsterAttack.erase(iterKey);
}

void SPBattleCluster::DeleteMOBBeAttacked(ANIMATION_INDEX eIndex, unsigned int uiClassID)
{
	if( m_hmMonsterBeAttacked.empty() )
		return;

	hash_map< unsigned int, vector< SPMonsterBeAttacked > >::iterator iterKey = m_hmMonsterBeAttacked.find(uiClassID);
	if( iterKey == m_hmMonsterBeAttacked.end() )
		return;
	
	vector< SPMonsterBeAttacked >::iterator iterValue = iterKey->second.end();
	iterValue = find_if(iterKey->second.begin(), iterKey->second.end(),
		std::bind2nd(IsEqualMonsterBeAttacked, eIndex));
	if( iterValue == iterKey->second.end() )
		return;

	iterKey->second.erase(iterValue);
}

void SPBattleCluster::DeleteMOBBeAttacked(unsigned int uiClassID)
{
	if( m_hmMonsterBeAttacked.empty() )
		return;

	hash_map< unsigned int, vector< SPMonsterBeAttacked > >::iterator iterKey = m_hmMonsterBeAttacked.find(uiClassID);
	if( iterKey == m_hmMonsterBeAttacked.end() )
		return;

	iterKey->second.clear();
	m_hmMonsterBeAttacked.erase(iterKey);
}

void SPBattleCluster::DeleteGOBAttack(unsigned int uiAniIndex)
{
	if( m_hmCharacterAttack.empty() )
		return;

	hash_map< unsigned int, SPCharacterAttack >::iterator iter = m_hmCharacterAttack.find(uiAniIndex);
	if( iter != m_hmCharacterAttack.end() )
	{
		m_hmCharacterAttack.erase(iter);
	}
}

void SPBattleCluster::DeleteGOBBeAttacked(unsigned int uiAniIndex)
{
	if( m_hmCharacterBeAttacked.empty() )
		return;

	hash_map< unsigned int, SPCharacterBeAttacked >::iterator iter = m_hmCharacterBeAttacked.find(uiAniIndex);
	if( iter != m_hmCharacterBeAttacked.end() )
	{
		m_hmCharacterBeAttacked.erase(iter);
	}
}

bool SPBattleCluster::SaveAttack(string strFilePath)
{
	if( m_hmCharacterAttack.empty() )
		return false;

	ofstream file(strFilePath.c_str());
	if(!file)	return false;

	// Header Description
	file << "// Attack Information File : Made - " << __DATE__<< " " << __TIME__ << endl;
	file << "// Animation Index" << endl;
	file << "// { " << endl;
	file << "//\t PointNum CallTickTime" << endl;
	file << "//\t Index PosX PosY AttackType " << endl;
	file << "// \t ... " << endl;
	file << "// } " << endl << endl;
	file << "// AttackType : ATTACK_NORMAL(N), ATTACK_PIERCING(P), ATTACK_DOWN(D), ATTACK_BOTH(B) " << endl << endl;
			
	hash_map< unsigned int, SPCharacterAttack >::iterator iterKey;
	iterKey = m_hmCharacterAttack.begin();
	while( iterKey != m_hmCharacterAttack.end() )
	{
		if( iterKey->second.m_uiAniIndex == 0 )
			return false;

		//file << iterKey->second.m_uiAniIndex << endl;
		file << "ATTACK" << endl;
		file << "{" << endl;										// {
		file << "\t" << iterKey->second.m_nAttackPointNum 
			<< " " << iterKey->second.m_fCallTickTime 
			<< " " << iterKey->second.m_fAccumulateTime << endl;			// AttackPointNum, CallTickTime

		for(int i=0; i<iterKey->second.m_nAttackPointNum; i++)
		{
			file << "\t" << iterKey->second.m_AttackInfo.at(i).m_nIndex 
				<< " " << iterKey->second.m_AttackInfo.at(i).m_rtRange.left
				<< " " << iterKey->second.m_AttackInfo.at(i).m_rtRange.top ;
			switch(iterKey->second.m_AttackInfo.at(i).m_AttackType)
			{
				case ATTACK_NORMAL:
					file << " N" << endl;
					break;
				case ATTACK_PIERCING:
					file << " P" << endl;
					break;
				case ATTACK_DOWN:
					file << " D" << endl;
					break;
				case ATTACK_BOTH:
					file << " B" << endl;
					break;				
			}
		}
		file << "}" << endl;

		++iterKey;
	}
	
	return true;
}

bool SPBattleCluster::SaveBeAttacked(string strFilePath)
{
	if( m_hmCharacterBeAttacked.empty() )
		return false;

	ofstream file(strFilePath.c_str());
	if(!file)	return false;	

	// Header Description
	file << "// Be Attacked Information File : Made - " << __DATE__<< " " << __TIME__ << endl;
	file << "// Animation Index" << endl;
	file << "// { " << endl;
	file << "//\t TotalFrameNum TotalAccumulatedTime (Mob:SizeIndex)  TotalDataNum " << endl;
	file << "//\t FrameNum PosX PosY CallTickTime (APC:DefenseID) " << endl;
	file << "//\t ... " << endl;
	file << "// } " << endl << endl;
	file << "// SizeIndex : MON_TINY(T) , MON_SMALL(S), MON_MED(M), MON_BIG(B) " << endl;
	file << "// DefenseID : BEATTACKED_NORMAL(M) ,BEATTACKED_ATTACK(A), BEATTACKED_DEFENSE(D), BEATTACKED_JUMP(J), BEATTACKED_NODMG(N) " << endl << endl;

	hash_map< unsigned int, SPCharacterBeAttacked >::iterator iterKey;
	iterKey = m_hmCharacterBeAttacked.begin();

	while( iterKey != m_hmCharacterBeAttacked.end() )
	{
		if( iterKey->second.m_uiAniIndex == 0 )
			return false;

		int nCount = 0 ;		
		for(int i=0; i<iterKey->second.m_BeAttackInfo.size()-1; i++)
		{			
			vector< APCBEATTACKEDINFO > vBeAttackedInfo = iterKey->second.m_BeAttackInfo;
			if(vBeAttackedInfo.at(i).m_rtRange.left == vBeAttackedInfo.at(i+1).m_rtRange.left 
				&& vBeAttackedInfo.at(i).m_rtRange.top == vBeAttackedInfo.at(i+1).m_rtRange.top
				&& vBeAttackedInfo.at(i).m_iBeAttackedIndex == vBeAttackedInfo.at(i+1).m_iBeAttackedIndex)
				continue;
			nCount++;
		}

		nCount++; // 항상 마지막 프레임은 들어가야한다.

		//file << iterKey->second.m_uiAniIndex << endl;	// index string
		file << "BEATTACKED" << endl;
		file << "{" << endl;									// {
		file << "\t" << iterKey->second.m_nTotalFrame << " " << iterKey->second.m_fAccumulateTime<< " " << nCount<< endl;	// AttackPointNum, CallTickTime
				
		for(int i=0; i<iterKey->second.m_BeAttackInfo.size(); i++)
		{
			try
			{
				vector< APCBEATTACKEDINFO > vBeAttackedInfo = iterKey->second.m_BeAttackInfo;
				if(vBeAttackedInfo.at(i).m_rtRange.left == vBeAttackedInfo.at(i+1).m_rtRange.left 
					&& vBeAttackedInfo.at(i).m_rtRange.top == vBeAttackedInfo.at(i+1).m_rtRange.top
					&& vBeAttackedInfo.at(i).m_iBeAttackedIndex == vBeAttackedInfo.at(i+1).m_iBeAttackedIndex)
					continue;	 
			}
			catch(...)
			{;}			

			file << "\t" << iterKey->second.m_BeAttackInfo.at(i).m_nIndex 
				<< " " << iterKey->second.m_BeAttackInfo.at(i).m_rtRange.left
				<< " " << iterKey->second.m_BeAttackInfo.at(i).m_rtRange.top 
				<< " " << iterKey->second.m_BeAttackInfo.at(i).m_fAccumulatedTickTime 
				<< " " << iterKey->second.m_BeAttackInfo.at(i).m_iBeAttackedIndex;
		}
		file << "}" << endl;

		++iterKey;
	}
	
	return true;
}
#endif

#ifdef _DEBUG
void SPBattleCluster::Dump()
{

}
#endif