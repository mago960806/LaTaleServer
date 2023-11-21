#include <windows.h>
#include <vector>

#include <float.h>

#include "SPBattle.h"
#include "SPGOBClusterDef.h"
#include "SPBattleCluster.h"
#include "SPMonsterModelUnitDef.h"
#include "SPBattleLoader.h"

#ifdef _LATALE_SERVER
	#include "GlobalDefines.h"
#else 
	#include "dxutil.h"
#endif

#include "SPStream.h"
#include "SPResourceBaseDEF.h"
#include "SPResourceBase.h"
#include "SPGameObjectDEF.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////

SPBattleLoader::SPBattleLoader()
{
	m_strLineData = new char[255];	
	m_uiCharIndex = 0;
}

SPBattleLoader::~SPBattleLoader()
{
	// 2005.11.24 dhpark new & delete 검사에 의해서 수정합니다.
	delete [] m_strLineData;
}

void	SPBattleLoader::ReadMonHead(const char* pszString)
{
	m_CurAniIndex = ANI_NULL;
	for(int i=0; i<ANI_NUM; i++)
	{
		if( 0== strcmp(pszString, g_strAnimationIndex[i]) )
		{
			m_CurAniIndex = (ANIMATION_INDEX) i;
			break;
		}
	}	
}

//////////////////////////////////////////////////////////////////////////

void	SPBattleLoader::ReadCharacterAttack()
{	
	SPCharacterAttack	*spAttackTable =new SPCharacterAttack(m_uiCharIndex);	
	ReadAttackBody((SPAttack*)spAttackTable);	
	g_pBattleCluster->SetAttack(m_uiCharIndex, *spAttackTable);
	
	delete spAttackTable;
}

void	SPBattleLoader::ReadMonsterAttack(unsigned int uiClassID)
{	
	SPMonsterAttack		*spAttackTable = new SPMonsterAttack(m_CurAniIndex);
	ReadAttackBody((SPAttack*)spAttackTable);
	g_pBattleCluster->SetAttack(*spAttackTable, uiClassID);
	delete spAttackTable;
}

void SPBattleLoader::ReadMOBLayerAttack(unsigned int uiClassID)
{
	ReadMonsterAttack(uiClassID);
}

void	SPBattleLoader::ReadAttackBody(SPAttack *attack)
{	
	ReadLine();		// {	
	ReadValue( attack->m_nAttackPointNum, attack->m_fCallTickTime,
		attack->m_fAccumulateTime, attack->m_iAttackType, ReadLine());		
	for(int i=0; i<attack->m_nAttackPointNum; i++)
	{
		ATTACKINFO	attackInfo;
		ReadValue( attackInfo, ReadLine());		
		attack->m_AttackInfo.push_back(attackInfo);
	}
	ReadLine();		// }
}
//////////////////////////////////////////////////////////////////////////
void	SPBattleLoader::ReadCharacterBeAttacked()
{
	SPCharacterBeAttacked	spBeAttackedTable(m_uiCharIndex);		
	
	int nDataNum=0;
	ReadLine();		// {
	ReadValue( spBeAttackedTable.m_nTotalFrame, spBeAttackedTable.m_fAccumulateTime, nDataNum, ReadLine());

	// Read
	for(int i=0; i<nDataNum; i++)
	{
		APCBEATTACKEDINFO acpBeAttackedInfo;
		ReadValue(acpBeAttackedInfo, ReadLine());
		spBeAttackedTable.m_BeAttackInfo.push_back(acpBeAttackedInfo);		
	}	

	ReadLine();		// }
	g_pBattleCluster->SetAttack(m_uiCharIndex, spBeAttackedTable);
}

void	SPBattleLoader::ReadMonsterBeAttacked(unsigned int uiClassID)
{
	char		cMonsterSize;
	SPMonsterBeAttacked		spBeAttackedTable(m_CurAniIndex);
	
	int nDataNum=0;
	ReadLine();		// {
	ReadValue( spBeAttackedTable.m_nTotalFrame, spBeAttackedTable.m_fAccumulateTime, nDataNum, cMonsterSize, ReadLine());
	switch(cMonsterSize)
	{
		case 'T':		// Tiny
			spBeAttackedTable.m_nMonsterSizeIndex = MON_TINY;
			break;
		case 'S':		// Small
			spBeAttackedTable.m_nMonsterSizeIndex = MON_SMALL;
			break;
		case 'M':		// Medium
			spBeAttackedTable.m_nMonsterSizeIndex = MON_MED;
			break;	
		case 'B':		// Big
			spBeAttackedTable.m_nMonsterSizeIndex = MON_BIG;
			break;	
	}
	for (int i=0; i<nDataNum; i++)
	{
		APCBEATTACKEDINFO	beAttackedInfo;
		ReadValue( beAttackedInfo, spBeAttackedTable.m_nMonsterSizeIndex, ReadLine());		
		spBeAttackedTable.m_BeAttackInfo.push_back(beAttackedInfo);
	}
	ReadLine();		// }
	g_pBattleCluster->SetAttack(spBeAttackedTable, uiClassID);
}

void SPBattleLoader::ReadMOBLayerBeAttacked(unsigned int uiClassID)
{
	char		cMonsterSize;
	SPMonsterBeAttacked		spBeAttackedTable(m_CurAniIndex);

	int nDataNum=0;
	ReadLine();		// {
	ReadValue( spBeAttackedTable.m_nTotalFrame, spBeAttackedTable.m_fAccumulateTime, nDataNum, cMonsterSize, ReadLine());
	switch(cMonsterSize)
	{
	case 'T':		// Tiny
		spBeAttackedTable.m_nMonsterSizeIndex = MON_TINY;
		break;
	case 'S':		// Small
		spBeAttackedTable.m_nMonsterSizeIndex = MON_SMALL;
		break;
	case 'M':		// Medium
		spBeAttackedTable.m_nMonsterSizeIndex = MON_MED;
		break;	
	case 'B':		// Big
		spBeAttackedTable.m_nMonsterSizeIndex = MON_BIG;
		break;	
	}
	for (int i=0; i<nDataNum; i++)
	{
		APCBEATTACKEDINFO	beAttackedInfo;
		ReadValue( beAttackedInfo, true, ReadLine());		
		spBeAttackedTable.m_BeAttackInfo.push_back(beAttackedInfo);
	}
	ReadLine();		// }
	g_pBattleCluster->SetAttack(spBeAttackedTable, uiClassID);
}

//////////////////////////////////////////////////////////////////////////
bool SPBattleLoader::LoadMOBBattleTableFromFolder(string strFilePath)
{
	/*
	g_pBattleCluster->ClearMonster();
	
	string strFileName;
	unsigned int i, uiID, uiIDTemp, uiKind;

	// Monster
	uiIDTemp = 0;
	for( i = 0; i < MOB_CLASSID_NUM; i++ )
	{
		SetMOBIdentify(s_iMOBClassID[i], uiID, uiKind);
		if( uiIDTemp == uiID )
			continue;

		strFileName = strFilePath + GetMOBFileName(s_iMOBClassID[i]);
		strFileName += ".arg";
		
		LoadBattleFile(strFileName, false, true, 0, s_iMOBClassID[i]);
			
		strFileName = strFilePath + GetMOBFileName(s_iMOBClassID[i]);
		strFileName += ".drg";
		
		LoadBattleFile(strFileName, false, false, 0, s_iMOBClassID[i]);
		uiIDTemp = uiID;
	}
	*/

	return true;
}


bool SPBattleLoader::LoadBattleFile(string strFileName,
	bool bCharacter, bool bAttack, unsigned int uiCharAniIndex, unsigned int uiClassID)
{
	char szBuf[256];
	const char* pszLineBuf;

	//m_fptrAseFile = fopen(strFileName.c_str(), "rb");
	//if( m_fptrAseFile == NULL )
	//	return false;
	
	SPResourceBase::GetInstance()->GetStreamData( strFileName.c_str(), &m_pStream);
	if(m_pStream == NULL || !m_pStream->Valid()) {
		return false;
	}
	
	m_strLineData[0] = '\0';
	do
	{
		pszLineBuf = ReadLine();
		
		sprintf(szBuf, "%s", pszLineBuf);
		if( memcmp(szBuf, "//", 2) == 0 ||
			(int)strlen(szBuf) == 0 )
			continue;

		if( bCharacter )
		{
			if( strcmp(szBuf, "ATTACK") == 0 ||
				strcmp(szBuf, "BEATTACKED") == 0 )
			{
				m_uiCharIndex = uiCharAniIndex;
				bAttack ? ReadCharacterAttack() : ReadCharacterBeAttacked();
			}
		}
		else
		{
			ReadMonHead(szBuf);
			if( m_CurAniIndex != ANI_NULL )
				bAttack ? ReadMonsterAttack(uiClassID) : ReadMonsterBeAttacked(uiClassID);
		}
	}while( 0!=strcmp(pszLineBuf, "EOF") );

	//fclose(m_fptrAseFile);

	SAFE_RELEASE(m_pStream);
	return true;
}

bool SPBattleLoader::LoadBinaryBattleFile(string strFileName, bool bCharacter, bool bAttack,
	unsigned int uiCharAniIndex, unsigned int uiClassID)
{
	//m_fptrAseFile = fopen(strFileName.c_str(), "rb");
	//if( m_fptrAseFile == NULL )
	//	return false;

#ifdef _LATALE_SERVER
	m_pStream = new SPFileStream(strFileName.c_str());
#else
	SPResourceBase::GetInstance()->GetStreamData( strFileName.c_str(), &m_pStream);
#endif

	if(m_pStream == NULL || !m_pStream->Valid())
	{
		SAFE_RELEASE(m_pStream);
		return false;
	}

	if( bCharacter )
		bAttack ? LoadBinaryCharAttack(uiCharAniIndex) : LoadBinaryCharBeAttacked(uiCharAniIndex);
	else
		bAttack ? LoadBinaryMonsterAttack(uiClassID) : LoadBinaryMonsterBeAttacked(uiClassID);

	SAFE_RELEASE(m_pStream);
	//fclose(m_fptrAseFile);

	return true;
}

bool SPBattleLoader::LoadMOBLayerBattleFile(string strFileName, bool bAttack,
	unsigned int uiClassID)
{
	char szBuf[256];
	const char* pszLineBuf;

	SPResourceBase::GetInstance()->GetStreamData( strFileName.c_str(), &m_pStream);
	if(m_pStream == NULL || !m_pStream->Valid()) {
		SAFE_RELEASE(m_pStream);
		return false;
	}

	m_strLineData[0] = '\0';
	do
	{
		pszLineBuf = ReadLine();

		sprintf(szBuf, "%s", pszLineBuf);
		if( memcmp(szBuf, "//", 2) == 0 ||
			(int)strlen(szBuf) == 0 )
			continue;

		ReadMonHead(szBuf);
		if( m_CurAniIndex != ANI_NULL )
			bAttack ? ReadMOBLayerAttack(uiClassID) : ReadMOBLayerBeAttacked(uiClassID);
	}while( 0!=strcmp(pszLineBuf, "EOF") );

	SAFE_RELEASE(m_pStream);
	return true;
}

bool SPBattleLoader::LoadBinaryCharAttack(unsigned int uiCharAniIndex)
{
	SPCharacterAttack* pkAttack = NULL;
	g_pBattleCluster->GetAttack(uiCharAniIndex, &pkAttack);
	if( pkAttack )
		return true;

	SPCharacterAttack kAttackInfo;
	unsigned int uiIndex;

	m_pStream->Read(&uiIndex, sizeof(unsigned int));
	m_pStream->Read(&kAttackInfo.m_uiAniIndex, sizeof(unsigned int));
	m_pStream->Read(&kAttackInfo.m_nAttackPointNum, sizeof(int));
	m_pStream->Read(&kAttackInfo.m_fCallTickTime, sizeof(float));
	m_pStream->Read(&kAttackInfo.m_fAccumulateTime, sizeof(float));
	m_pStream->Read(&kAttackInfo.m_iAttackType, sizeof(int));

	//m_pStream->Read(&uiIndex, 1, sizeof(unsigned int), m_fptrAseFile);
	//m_pStream->Read(&kAttackInfo.m_uiAniIndex, 1, sizeof(unsigned int), m_fptrAseFile);
	//m_pStream->Read(&kAttackInfo.m_nAttackPointNum, 1, sizeof(int), m_fptrAseFile);
	//m_pStream->Read(&kAttackInfo.m_fCallTickTime, 1, sizeof(float), m_fptrAseFile);
	//m_pStream->Read(&kAttackInfo.m_fAccumulateTime, 1, sizeof(float), m_fptrAseFile);

	ATTACKINFO Info;
	int iSize, i;
	
	m_pStream->Read(&iSize, sizeof(int));
	for( i = 0; i < iSize; i++ )
	{
		m_pStream->Read(&Info.m_AttackType, sizeof(int));
		m_pStream->Read(&Info.m_nIndex, sizeof(int));
		m_pStream->Read(&Info.m_rtRange, sizeof(RECT));

		kAttackInfo.m_AttackInfo.push_back(Info);
	}

	g_pBattleCluster->SetAttack(uiCharAniIndex, kAttackInfo);

	return true;
}

bool SPBattleLoader::LoadBinaryCharBeAttacked(unsigned int uiCharAniIndex)
{
	SPCharacterBeAttacked* pkBeAttacked = NULL;
	g_pBattleCluster->GetAttack(uiCharAniIndex, &pkBeAttacked);
	if( pkBeAttacked )
		return true;

	unsigned int uiIndex;

	m_pStream->Read(&uiIndex, sizeof(unsigned int));
	SPCharacterBeAttacked kBeAttackedInfo;
	m_pStream->Read(&kBeAttackedInfo.m_uiAniIndex, sizeof(unsigned int));
	m_pStream->Read(&kBeAttackedInfo.m_nTotalFrame, sizeof(int));
	m_pStream->Read(&kBeAttackedInfo.m_fAccumulateTime, sizeof(float));

	APCBEATTACKEDINFO Info;
	int iSize, i;

	m_pStream->Read(&iSize, sizeof(int));
	for( i = 0; i < iSize; i++ )
	{
		m_pStream->Read(&Info.m_iBeAttackedIndex, sizeof(int));
		m_pStream->Read(&Info.m_fAccumulatedTickTime, sizeof(float));
		m_pStream->Read(&Info.m_nIndex, sizeof(int));
		m_pStream->Read(&Info.m_rtRange, sizeof(RECT));

		kBeAttackedInfo.m_BeAttackInfo.push_back(Info);
	}

	g_pBattleCluster->SetAttack(uiCharAniIndex, kBeAttackedInfo);
	return true;
}

bool SPBattleLoader::LoadBinaryMonsterAttack(unsigned int uiMOBClassID)
{
	if( g_pBattleCluster->IsMonsterAttack(uiMOBClassID) )
		return true;

	unsigned int uiClassID;
	int iAniNum, iSize, i, j;

	m_pStream->Read(&uiClassID, sizeof(unsigned int));
	m_pStream->Read(&iAniNum, sizeof(int));
	if( iAniNum <= 0 )
		return false;

	SPMonsterAttack kAttackInfo;
	for( i = 0; i < iAniNum; i++ )
	{
		m_pStream->Read(&kAttackInfo.m_uiAniIndex, sizeof(unsigned int));
		m_pStream->Read(&kAttackInfo.m_nAttackPointNum, sizeof(int));
		m_pStream->Read(&kAttackInfo.m_fCallTickTime, sizeof(float));
		m_pStream->Read(&kAttackInfo.m_fAccumulateTime, sizeof(float));
		m_pStream->Read(&kAttackInfo.m_iAttackType, sizeof(int));

		m_pStream->Read(&iSize, sizeof(int));
		
		kAttackInfo.m_AttackInfo.clear();
		ATTACKINFO Info;
		for( j = 0; j < iSize; j++ )
		{
			m_pStream->Read(&Info.m_AttackType, sizeof(int));
			m_pStream->Read(&Info.m_nIndex, sizeof(int));
			m_pStream->Read(&Info.m_rtRange, sizeof(RECT));

			kAttackInfo.m_AttackInfo.push_back(Info);
		}

		g_pBattleCluster->SetAttack(kAttackInfo, uiMOBClassID);
	}

	return true;
}

bool SPBattleLoader::LoadBinaryMonsterBeAttacked(unsigned int uiMOBClassID)
{
	if( g_pBattleCluster->IsMonsterBeAttacked(uiMOBClassID) )
		return true;

	unsigned int uiClassID;
	int iAniNum, iSize, i, j;
	
	m_pStream->Read(&uiClassID, sizeof(unsigned int));
	m_pStream->Read(&iAniNum, sizeof(int));
	if( iAniNum <= 0 )
		return false;

	SPMonsterBeAttacked kBeAttackedInfo;
	for( i = 0; i < iAniNum; i++ )
	{
		m_pStream->Read(&kBeAttackedInfo.m_uiAniIndex, sizeof(unsigned int));
		m_pStream->Read(&kBeAttackedInfo.m_nTotalFrame, sizeof(int));
		m_pStream->Read(&kBeAttackedInfo.m_fAccumulateTime, sizeof(float));
		m_pStream->Read(&kBeAttackedInfo.m_nMonsterSizeIndex, sizeof(MONSTER_INDEX));
		
		m_pStream->Read(&iSize, sizeof(int));

		kBeAttackedInfo.m_BeAttackInfo.clear();
		APCBEATTACKEDINFO Info;
		for( j = 0; j < iSize; j++ )
		{
			m_pStream->Read(&Info.m_iBeAttackedIndex, sizeof(int));
			m_pStream->Read(&Info.m_fAccumulatedTickTime, sizeof(float));
			m_pStream->Read(&Info.m_nIndex, sizeof(int));
			m_pStream->Read(&Info.m_rtRange, sizeof(RECT));

			kBeAttackedInfo.m_BeAttackInfo.push_back(Info);
		}

		g_pBattleCluster->SetAttack(kBeAttackedInfo, uiMOBClassID);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
char*	SPBattleLoader::ReadLine()
{
	char   ch;

	m_strLineData[0] = '\0';
	/*
	for( unsigned char counter=0; ; counter++ )
	{
		ch = fgetc( m_fptrAseFile );

		if( ch == EOF ) return "EOF";

		m_strLineData[counter] = ch;
		if( counter > 0 )
			if(	m_strLineData[counter-1] == 0x0D )
				if( m_strLineData[counter]   == 0x0A )
					break;
	}
	m_strLineData[counter] = '\0';
	*/
	
	unsigned char counter=0;
	for( counter=0; ; counter++ )
	{
		//ch = fgetc( m_fptrAseFile );
		m_pStream->Read(&ch, 1);

		//if( ch == EOF ) return "EOF";

		if(m_pStream->Eos()) 
			return "EOF";

		m_strLineData[counter] = ch;
		if( ch == '\n' )
			break;
	}
	
	if( counter > 0 )
		m_strLineData[counter - 1] = '\0';

	return m_strLineData;

}

void	SPBattleLoader::ReadValue(int &nValue, const char* strLine)
{
	sscanf( strLine, "%d", &nValue);
}

void	SPBattleLoader::ReadValue(int &nValue, float& fValue, const char* strLine)
{
	sscanf( strLine, "%d %f", &nValue, &fValue);	
}
void	SPBattleLoader::ReadValue( int &nValue, float &fValue1, float &fValue2 , int &nValue2, const char* strLine)
{
	sscanf( strLine, "%d %f %f %d", &nValue, &fValue1, &fValue2, &nValue2);	
}

void	SPBattleLoader::ReadValue(int &nValue, float& fValue, int& nValue1, char& cValue2, const char* strLine)
{
	sscanf( strLine, "%d %f %d %c", &nValue, &fValue, &nValue1, &cValue2);
}

void	SPBattleLoader::ReadValue(int &nValue, float &fValue, int &nValue2, const char* strLine)
{
	sscanf( strLine, "%d %f %d", &nValue , &fValue, &nValue2);
}

void	SPBattleLoader::ReadValue(ATTACKINFO& sValue, const char* strLine)
{		
	char	cAttackType;	

	sscanf( strLine, "%d %d %d %d %d %c", &sValue.m_nIndex, &sValue.m_rtRange.left, &sValue.m_rtRange.top, &sValue.m_rtRange.right, &sValue.m_rtRange.bottom, &cAttackType);

	switch(cAttackType)
	{
		case 'N':	// normal
			sValue.m_AttackType = ATTACK_NORMAL;
			break;
		case 'P':	// piercing
			sValue.m_AttackType = ATTACK_PIERCING;
			break;
		case 'D':	// down
			sValue.m_AttackType = ATTACK_DOWN;
			break;
		case 'B':	// piercing + down
			sValue.m_AttackType = ATTACK_BOTH;
			break;
	}
}

void	SPBattleLoader::ReadValue( APCBEATTACKEDINFO& sValue,	MONSTER_INDEX monSize, const char* strLine)
{
	sscanf( strLine, "%d %d %d %f %d", &sValue.m_nIndex, &sValue.m_rtRange.left, &sValue.m_rtRange.top, &sValue.m_fAccumulatedTickTime, &sValue.m_iBeAttackedIndex );

	sValue.m_fAccumulatedTickTime += FLT_EPSILON ; 
	sValue.m_rtRange.right = sValue.m_rtRange.left + (monSize*RECT_SIZE);
	sValue.m_rtRange.bottom= sValue.m_rtRange.top  +(((monSize==MON_TINY)?2:monSize)*RECT_SIZE);
}

void	SPBattleLoader::ReadValue( APCBEATTACKEDINFO& sValue, bool bRealSize, const char* strLine)
{
	sscanf(strLine, "%d %d %d %d %d %f %d", &sValue.m_nIndex,
		&sValue.m_rtRange.left, &sValue.m_rtRange.top, 
		&sValue.m_rtRange.right, &sValue.m_rtRange.bottom,
		&sValue.m_fAccumulatedTickTime, &sValue.m_iBeAttackedIndex);

	sValue.m_fAccumulatedTickTime += FLT_EPSILON ; 
}

void	SPBattleLoader::ReadValue( APCBEATTACKEDINFO& sValue,	const char* strLine)
{
	int	iBeAttackType;
	sscanf( strLine, "%d %d %d %f %d", &sValue.m_nIndex, &sValue.m_rtRange.left, &sValue.m_rtRange.top , &sValue.m_fAccumulatedTickTime, &iBeAttackType);	

	sValue.m_fAccumulatedTickTime+=FLT_EPSILON;
	sValue.m_iBeAttackedIndex = iBeAttackType;
	
	sValue.m_rtRange.right = sValue.m_rtRange.left+RECT_SIZE;
	sValue.m_rtRange.bottom = sValue.m_rtRange.top+ (3*RECT_SIZE);
}