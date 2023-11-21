#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPStoryManager.h"

using namespace std;

SPStoryManager* SPStoryManager::m_pkInstance = NULL;
SPStoryManager* g_pkStoryManger = NULL;
//------------------------------------------------------------------------------------
SPStoryManager* SPStoryManager::GetInstance()
{
	if( m_pkInstance == NULL )
	{
		m_pkInstance = new SPStoryManager;
		g_pkStoryManger = m_pkInstance;
	}

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPStoryManager::Release()
{
	if( m_pkInstance )
	{
		g_pkStoryManger = NULL;

		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPStoryManager::SPStoryManager()
{
}

//------------------------------------------------------------------------------------
SPStoryManager::~SPStoryManager()
{
	m_mStoryFactor.clear();
}

//------------------------------------------------------------------------------------
bool SPStoryManager::Init()
{
	bool bRet;

	m_iStoryViewID = 0 ;
	bRet = LoadLDTFile();
	return bRet;
}

//------------------------------------------------------------------------------------
bool SPStoryManager::LoadLDTFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/STORY.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i, j;
	LDT_Field Field;
	//char szBuf[256];
	STORY_FACTOR tFactor;
	string strImage;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);
		tFactor.Clear();

		//pkLDTFile->GetFieldFromLable(iItemID, "_BGM", Field);
		pkLDTFile->GetField(iItemID, 0, Field);
		tFactor.strBGM = Field.uData.pValue;

		for( j = 0; j < 14; j++ )
		{
			/*sprintf(szBuf, "_Picture%d", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID, szBuf, Field);*/
			Field.Init() ;
			pkLDTFile->GetField(iItemID, 1+j, Field);
			if( Field.uData.pValue )
			{
				strImage = Field.uData.pValue;
				if( !strImage.empty() )
					tFactor.vstrImage.push_back(strImage);
			}
		}
		m_mStoryFactor.insert(make_pair(iItemID, tFactor));
	}

	return true;
}

//------------------------------------------------------------------------------------
STORY_FACTOR* SPStoryManager::GetStoryFactor(int iID)
{
	map< int, STORY_FACTOR >::iterator iter = m_mStoryFactor.find(iID);
	if( iter == m_mStoryFactor.end() )
		return NULL;

	return &(*iter).second;
}