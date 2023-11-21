
#include <hash_map>
#include <string>

#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPGOBClusterDef.h"

#include "SPSoundManager.h"
#include "SPDebug.h"

using namespace std;
using namespace stdext;

SPSoundManager* SPSoundManager::m_pkInstance = NULL;
//------------------------------------------------------------------------------------
SPSoundManager* SPSoundManager::GetInstance()
{
	if( m_pkInstance == NULL )
		m_pkInstance = new SPSoundManager;

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPSoundManager::Release()
{
	if( m_pkInstance )
	{
		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPSoundManager::SPSoundManager()
{
}

//------------------------------------------------------------------------------------
SPSoundManager::~SPSoundManager()
{
	m_hmSoundList.clear();
	m_hmGOBMotionSoundList.clear();

	hash_map< unsigned int , hash_map< ANIMATION_INDEX, unsigned int > >::iterator iter;
	iter = m_hmMOBMotionSoundList.begin();
	while( iter != m_hmMOBMotionSoundList.end() )
	{
		(*iter).second.clear();
		++iter;
	}
	m_hmMOBMotionSoundList.clear();
}

//------------------------------------------------------------------------------------
void SPSoundManager::Init()
{
#if defined(_DEBUG)
	DXUTOutputDebugString("SoundManager::Init()\n");
#endif

#if !defined(_JUST_RUN)
	if( !LoadSoundList() )
		return;

	if( !LoadGOBMotionSoundList() )
		return;

	if( !LoadMOBMotionSoundList() )
		return;
#endif
}

//------------------------------------------------------------------------------------
bool SPSoundManager::LoadSoundList()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/SOUND_EFFECT.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, i, j;
	long lItemID;
	LDT_Field Field;
//	char szBuf[128];
	
	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);
		SOUND_TABLE_INFO SoundInfo;

		for( j = 0; j < 3; j++ )
		{
			/*sprintf(szBuf, "_DelayTime%d", j + 1);
			pkLDTFile->GetFieldFromLable( lItemID , szBuf,	Field );*/
			Field.Init() ;
			pkLDTFile->GetField(lItemID, 0+j*3, Field);
			SoundInfo.fDelayTime[j] = (float)Field.uData.lValue / 10.0f;

			/*sprintf(szBuf, "_Sound%d", j + 1);
			pkLDTFile->GetFieldFromLable( lItemID , szBuf,	Field );*/
			Field.Init() ;
			pkLDTFile->GetField(lItemID, 1+j*3, Field);
			SoundInfo.strFileName[j] = Field.uData.pValue;

			/*sprintf(szBuf, "_Loop%d", j + 1);
			pkLDTFile->GetFieldFromLable( lItemID , szBuf,	Field );*/
			Field.Init() ;
			pkLDTFile->GetField(lItemID, 2+j*3, Field);
			SoundInfo.bLoop[j] = (bool)Field.uData.lValue;
		}

		m_hmSoundList.insert(
			hash_map< unsigned int, SOUND_TABLE_INFO >::value_type(lItemID, SoundInfo));
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
}

//------------------------------------------------------------------------------------
bool SPSoundManager::LoadGOBMotionSoundList()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/MOTION_SOUND.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iFieldNum, i, j;
	unsigned int uiAniIndex;
	long lItemID;
	LDT_Field Field;

	iDataNum = pkLDTFile->GetItemCount();
	iFieldNum = pkLDTFile->GetFieldCount();

	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);
		for( j = 0; j < iFieldNum; j++ )
		{
			pkLDTFile->GetField(lItemID, j, Field);
			if( Field.uData.lValue == 0 )
				continue;

			uiAniIndex = GetGOBSeqKey(lItemID, j + 1);
			m_hmGOBMotionSoundList.insert(
				hash_map< unsigned int, unsigned int >::value_type(uiAniIndex, Field.uData.lValue));
		}
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
}

//------------------------------------------------------------------------------------
bool SPSoundManager::LoadMOBMotionSoundList()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/MOB_MOTIONSOUNDTYPE.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iFieldNum, i, j;
	unsigned int uiClassID;
	long lItemID;
	LDT_Field Field;

	hash_map< ANIMATION_INDEX, unsigned int > hmAniSoundList;
	ANIMATION_INDEX eAniIndex;

	iDataNum = pkLDTFile->GetItemCount();
	iFieldNum = pkLDTFile->GetFieldCount();

	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);
		uiClassID = lItemID;
		
		for( j = 0; j < iFieldNum; j++ )
		{
			pkLDTFile->GetField(lItemID, j, Field);
			if( Field.uData.lValue == 0 )
				continue;

			eAniIndex = (ANIMATION_INDEX)j;
			hmAniSoundList.insert(
				hash_map< ANIMATION_INDEX, unsigned int >::value_type(eAniIndex, Field.uData.lValue));
		}

		if( !hmAniSoundList.empty() )
		{
			m_hmMOBMotionSoundList.insert(
				hash_map< unsigned int, hash_map< ANIMATION_INDEX, unsigned int > >::value_type(uiClassID, hmAniSoundList));
			hmAniSoundList.clear();
		}
	}

	SAFE_RELEASE(pkLDTFile);
	hmAniSoundList.clear();
	return true;
}

//------------------------------------------------------------------------------------
SOUND_TABLE_INFO* SPSoundManager::GetGOBSoundTable(unsigned int uiAniIndex)
{
	hash_map< unsigned int, unsigned int >::iterator iterGOBMotion;
	hash_map< unsigned int, SOUND_TABLE_INFO >::iterator iterSoundList;

	iterGOBMotion = m_hmGOBMotionSoundList.find(uiAniIndex);
	if( iterGOBMotion == m_hmGOBMotionSoundList.end() )
		return NULL;

	iterSoundList = m_hmSoundList.find(iterGOBMotion->second);
	if( iterSoundList == m_hmSoundList.end() )
		return NULL;

	return &iterSoundList->second;
}

//------------------------------------------------------------------------------------
SOUND_TABLE_INFO* SPSoundManager::GetMOBSoundTable(
	unsigned int uiClassID, ANIMATION_INDEX eIndex)
{
	hash_map< unsigned int , hash_map< ANIMATION_INDEX, unsigned int > >::iterator iterMotion;
	iterMotion = m_hmMOBMotionSoundList.find(uiClassID);
	if( iterMotion == m_hmMOBMotionSoundList.end() )
		return NULL;

	hash_map< ANIMATION_INDEX, unsigned int >::iterator iterAni;
	iterAni = (*iterMotion).second.find(eIndex);
	if( iterAni == (*iterMotion).second.end() )
		return NULL;

	hash_map< unsigned int, SOUND_TABLE_INFO >::iterator iterSoundList;
	iterSoundList = m_hmSoundList.find((*iterAni).second);
	if( iterSoundList == m_hmSoundList.end() )
		return NULL;

	return &iterSoundList->second;
}

//------------------------------------------------------------------------------------
SOUND_TABLE_INFO* SPSoundManager::GetSoundTable(unsigned int uiKeyValue)
{
	hash_map< unsigned int, SOUND_TABLE_INFO >::iterator iterSoundList;
	iterSoundList = m_hmSoundList.find(uiKeyValue);
	if( iterSoundList == m_hmSoundList.end() )
		return NULL;

	return &iterSoundList->second;
}