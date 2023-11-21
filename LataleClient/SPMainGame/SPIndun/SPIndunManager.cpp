#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPItemAttr.h"

#include "SPIndun.h"
#include "SPIndunManager.h"
#include "SPIndunGuildRoom.h"
#include "SPIndunNormal.h"
#include "SPLocalizeManager.h"

#include "_Error_log.h"

using namespace std;

SPIndunManager* g_pkIndunManager = NULL;
SPIndunManager* SPIndunManager::m_pkInstance = NULL;
//------------------------------------------------------------------------------------
SPIndunManager* SPIndunManager::GetInstance()
{
	if( m_pkInstance == NULL )
	{
		m_pkInstance = new SPIndunManager;
		g_pkIndunManager = m_pkInstance;
	}

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPIndunManager::Release()
{
	if( m_pkInstance )
	{
		g_pkIndunManager = NULL;

		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPIndunManager::SPIndunManager()
: m_pCurIndun(NULL)
, m_bForceExit(false)
{
}

//------------------------------------------------------------------------------------
SPIndunManager::~SPIndunManager()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPIndunManager::Clear()
{
	ClearIndunInfo() ;
	m_mIndunData.clear();
}

//------------------------------------------------------------------------------------
bool SPIndunManager::Init()
{
	bool bRet = false ;
	m_uiCurIndunLdtID		= 0 ;

	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_INDUN) )
		bRet = LoadLDTFile();
	return bRet;
}

//------------------------------------------------------------------------------------
bool SPIndunManager::LoadLDTFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/INDUN.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i, j;
	LDT_Field ldtField;
//	char szBuf[128];
	iDataNum = pkLDTFile->GetItemCount();

	INDUN_INFO stIndunInfo;
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);
		stIndunInfo.Clear();

		
		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Map_Type",		ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 0, ldtField);
		stIndunInfo.eType = (INDUN_TYPE)ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapin_Stage",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 1, ldtField);
		stIndunInfo.iMapInStage = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapin_Group",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 2, ldtField);
		stIndunInfo.iMapInGroup = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapin_X",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 3, ldtField);
		stIndunInfo.iMapInPosX = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapin_Y",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 4, ldtField);
		stIndunInfo.iMapInPosY = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapout_Stage",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 5, ldtField);
		stIndunInfo.iMapOutStage = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapout_Group",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 6, ldtField);
		stIndunInfo.iMapOutGroup = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapout_X",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 7, ldtField);
		stIndunInfo.iMapOutPosX = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapout_Y",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 8, ldtField);
		stIndunInfo.iMapOutPosY = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Mission_Monster",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 29, ldtField);
		stIndunInfo.strIndunName = ldtField.uData.pValue;

		for( j = 0; j < INDUN_REQUIRE_NUM; j++ )
		{
			/*sprintf(szBuf, "_Ind_Require%d_Type", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID, szBuf,	ldtField);*/
			ldtField.Init() ;
			pkLDTFile->GetField(iItemID, 9+j*4, ldtField);
			stIndunInfo.stRequire[j].m_iType = ldtField.uData.lValue;

			/*sprintf(szBuf, "_Ind_Require%d_ID", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID, szBuf,	ldtField);*/
			ldtField.Init() ;
			pkLDTFile->GetField(iItemID, 10+j*4, ldtField);
			stIndunInfo.stRequire[j].m_iID = ldtField.uData.lValue;

			/*sprintf(szBuf, "_Ind_Require%d_Value1", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID, szBuf,	ldtField);*/
			ldtField.Init() ;
			pkLDTFile->GetField(iItemID, 11+j*4, ldtField);
			stIndunInfo.stRequire[j].m_iValue1 = ldtField.uData.lValue;

			/*sprintf(szBuf, "_Ind_Require%d_Value2", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID, szBuf,	ldtField);*/
			ldtField.Init() ;
			pkLDTFile->GetField(iItemID, 12+j*4, ldtField);
			stIndunInfo.stRequire[j].m_iValue2 = ldtField.uData.lValue;
		}

		m_mIndunData.insert(make_pair(iItemID, stIndunInfo));
	}

	return true;
}

//------------------------------------------------------------------------------------
void SPIndunManager::Process(float fTime)
{
	if( m_pCurIndun )
		m_pCurIndun->Process(fTime) ;
}

//------------------------------------------------------------------------------------
void SPIndunManager::Render()
{
	if( m_pCurIndun )
		m_pCurIndun->Render() ;
}


//------------------------------------------------------------------------------------
bool SPIndunManager::FindIndunInfo(int iIndunID, INDUN_INFO& indunInfo)
{
	std::map< int, INDUN_INFO >::iterator mIter ;

	mIter = m_mIndunData.find(iIndunID) ;
	if( mIter != m_mIndunData.end() )
	{
		indunInfo = (*mIter).second ;
		return true ;
	}
	return false ;
}


bool SPIndunManager::GetIndunName(int iIndunID, std::string& strIndunName)
{
	INDUN_INFO findIndun ;
	if( iIndunID > 0 )
	{
		if( FindIndunInfo( iIndunID, findIndun ) ) {
			strIndunName = findIndun.strIndunName ;
			return true ;
		}
	}
	return false ;
}


//------------------------------------------------------------------------------------
bool SPIndunManager::EnterIndun(UINT32 iInstanceID, int iIndunID/*=0*/)
{
	ClearIndunInfo() ;

	INDUN_INFO IndunInfo ;
	if( iIndunID == 0 )		iIndunID = m_uiCurIndunLdtID ;
	if( FindIndunInfo(iIndunID, IndunInfo) == false )
	{
		ErrorFLogs->Error_Log("##Can't Find IndunInfo IndunLdtID[%d]", iIndunID);
		return false ;
	}

	switch( IndunInfo.eType )
	{
		case IDT_NULL:
			break ;
		case IDT_GUILD:
			m_pCurIndun = new SPIndunGuildRoom(iIndunID, iInstanceID, IndunInfo.eType) ;
			m_pCurIndun->Init() ;
			break ;

		case IDT_PARTY:
		case IDT_SINGLE:
		case IDT_EVENT:
		case IDT_GINDUN:
			m_pCurIndun = new SPIndunNormal(iIndunID, iInstanceID, IndunInfo.eType) ;
			m_pCurIndun->Init() ;
			break ;
	}
	return true ;
}

//------------------------------------------------------------------------------------
void SPIndunManager::ClearIndunInfo()
{
	SAFE_DELETE(m_pCurIndun) ;
	m_bForceExit = false ;
}

//------------------------------------------------------------------------------------
void SPIndunManager::DisplayIndunInfo()
{
	TCHAR szMessage[256] ;
	if( m_pCurIndun ) {
		INDUN_INFO IndunInfo ;
		if( FindIndunInfo(m_pCurIndun->GetIndunID(), IndunInfo) == false )
			return ;
		sprintf( szMessage, "Current IndunID[%d] Type[%d] : Stage[%d] Group[%d]", m_pCurIndun->GetIndunID(), m_pCurIndun->GetIndunType(), IndunInfo.iMapInStage, IndunInfo.iMapInGroup) ;
	}
	else
		sprintf( szMessage, "Here isn't Indun") ;
	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMessage );
}
