
// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPRsManager.cpp
//	created:	2007/10/1   Time:11:59
//	purpose:	
//*********************************************************************/

//#ifdef _INCLUDE_RSCLIENT_

#include <stdio.h>

#ifdef _LATALE_CLIENT
	#include "SPCommon.h"
	#include "SPUtil.h"

	#include "SPMainGameDEF.H"
	#include "SPGameObjectDEF.h"

	#include "SPSubGameManager.h"
	#include "SPGameObject.h"
	#include "SPManager.h"
	#include "SPMouseCursor.h"
	#include "SPIMEPool.h"

	#include "SPResourceDef.h"
	#include "SPWindowDEF.h"
	#include "SPInterfaceManager.h"
	#include "SPResourceManager.h"
	#include "SPLocalizeManager.h"
#endif

#include "RSSDKdefine.h"
#include "SPRsManager.h"
#include "SPRsPacketHandler.h"
#include "_Error_log.h"


SPRsManager* g_pkRsManager = NULL;
SPRsManager* SPRsManager::m_pkInstance = NULL;
//------------------------------------------------------------------------------------
SPRsManager* SPRsManager::GetInstance()
{
	if( m_pkInstance == NULL )
	{
		m_pkInstance = new SPRsManager;
		g_pkRsManager = m_pkInstance;
	}

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPRsManager::Release()
{
	if( m_pkInstance )
	{
		g_pkRsManager = NULL;

		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPRsManager::SPRsManager()
: m_pkRSHandle(NULL)
, m_pMainMenu(NULL)
, m_pSubMenu(NULL)
{
}

//------------------------------------------------------------------------------------
SPRsManager::~SPRsManager()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPRsManager::Clear()
{
	SAFE_DELETE( m_pMainMenu ) ;
	SAFE_DELETE_ARRAY( m_pSubMenu ) ;
	SAFE_DELETE( m_pkRSHandle );
}

//------------------------------------------------------------------------------------
bool SPRsManager::Init()
{
	m_pMainMenu = NULL ;
	m_bHasMainMenu = false ;

	for( int i = 0 ; i < 5 ; i++ )
	{
		m_bHasSubMenu[i] = false ;
	}
	m_iSelectMain = 0 ;

	m_pMainMenu = new RS_MENU ;
	m_pSubMenu = new RS_MENU[5];
	
	// Only Use China
	if( g_eCountryLocale == CL_CHINA )
	{
		m_pkRSHandle = new SPRSPacketHandler ;
		if( m_pkRSHandle->Initialize() == false )
		{
			SAFE_DELETE( m_pkRSHandle );
			ErrorFLogs->Error_Log("RSHandler Initialize Fail!!!") ;
			return false ;
		}
		m_pkRSHandle->SetNetParam(RS_DEF_SERVER_IP, RS_DEF_SERVER_PORT) ;
	}
	else
	{
		m_pkRSHandle = NULL ;
	}
	
	
	return true ;
}



/**
	RS ¿¬°áÀ» À§ÇÑ IP & Port Setting
	¸Å Àü¼Û¸¶´Ù PacketHandler ¿¡¼­ Á¢¼ÓÇÏ°Ô µÈ´Ù.
*/
void SPRsManager::SetNetParam	( const char* szRSIP, const char*szRSPort )
{	
	if( m_pkRSHandle )
		m_pkRSHandle->SetNetParam(szRSIP, szRSPort) ;
}

void SPRsManager::SetGameType( int iGMType, int iArea )	
{	
	if( m_pkRSHandle )
		m_pkRSHandle->SetGameType(iGMType, iArea) ;
}

//---------------------------------------------------------------------
// ¹ÞÀº ÆÐÅ¶ÀÇ ³»¿ëÀ¸·Î ¼ÂÆÃµÇ¾î ´ÙÀ½ Àü¼Û½Ã¿¡ ÀÌ¿ëµÊ
void SPRsManager::SetlUserLogIdx( const char* szlUserLogIdx)
{	
	m_strlUserLogIdx = szlUserLogIdx;	
}

void SPRsManager::SetAnsID( const char* szAnsID)
{	
	m_strAnsID = szAnsID;	
}

void SPRsManager::SetQuestion( const char* szQuestion)
{	
	m_strQuestion = szQuestion;	
}


//---------------------------------------------------------------------
// IDType & Account ´Â PacketHandler·Î ÀÌµ¿µÉ¼öµµ ÀÖÀ½
void SPRsManager::SetIDType( const char* szIDType)	
{	
	m_strIDType = szIDType;	
}

void SPRsManager::SetAccount( const char* szAccount)
{	
	m_strAccount = szAccount;	
}

//---------------------------------------------------------------------
/**
	For Main Menu HotQuestion
*/
int SPRsManager::SendQMainMenu()
{
	if( m_pkRSHandle ) {
		if( m_bHasMainMenu )
		{
			// ¸ÞÀÎ ¸Þ´º¸¦ ÀÌ¹Ì °¡Áö°í ÀÖ´Â °æ¿ì
			SetMainMenuToWindow() ;
		}
		else
		{
			m_eHotQType = TYPE_QNV2_MAIN ;
			m_pkRSHandle->FuncHotQTopNV2("1", "HOTQUESNODE_ROOT") ;
		}
	}
	return 0 ;
}

/**
	For Sub Menu : Request List
	@param const char* pAC : Main Menu's AC
*/
//int SPRsManager::SendQSubMenu(const char* pAC)
//{
//	if( m_pkRSHandle )	{
//		m_eHotQType = TYPE_QNV2_SUB ;
//		m_pkRSHandle->FuncHotQTopNV2("2", pAC) ;
//	}
//	return 0 ;
//}
/**
	For Sub Menu : Request List
	@param int iSelectIndex : Selected Main Menu Index
 */
int SPRsManager::SendQSubMenu(int iSelectIndex)
{
	if( m_pkRSHandle )	{
		if( m_pMainMenu->iCount > iSelectIndex )
		{
			m_iSelectMain = iSelectIndex ;
			m_strSendQuest = m_pMainMenu->szText[m_iSelectMain] ;
			if( m_bHasSubMenu[m_iSelectMain] == true )
			{
				// ÀÌ¹Ì ¹Þ¾Ò´Ù¸é, ÀÖ´ø ÄÁÅÙÃ÷¸¦ º¸³½´Ù.
				SetSubMenuToWindow() ;
			}
			else
			{
				m_eHotQType = TYPE_QNV2_SUB ;
				m_pkRSHandle->FuncHotQTopNV2("2", m_pMainMenu->szAC[iSelectIndex] ) ;
			}
		}
	}
	return 0 ;
}


/**
	For Main Contents : Request HotTop10
*/
int SPRsManager::SendQHotTop10()
{
	if( m_pkRSHandle )	{
		m_iSelectMain = 0 ;
		m_eHotQType = TYPE_QNV2_CONTEXT ;
		if( g_pResourceManager->GetGlobalString(5008001) )
			m_strSendQuest = g_pResourceManager->GetGlobalString(5008001) ;
		else
			m_strSendQuest = "HotTop10" ;
		m_pkRSHandle->FuncHotQTopNV2("3", "HOTQUESTION_BASE") ;
	}
	return 0 ;
}

//---------------------------------------------------------------------
// SubMenu's Contents click
/**
	If SubMenu's Contents Click then send this.
	@param const char* pAC : Sub Menu's AC
*/
int SPRsManager::SendQSubMenuContents(const char* pAC)
{
	if( m_pkRSHandle )	{
		m_eHotQType = TYPE_QNV2_CONTEXT ;
		m_pkRSHandle->FuncHotQTopNV2("3", pAC) ;
	}
	return 0 ;
}

/**
	If SubMenu's Contents Click then send this.
	@param const char* pAC : Sub Menu's AC
*/
int SPRsManager::SendQSubMenuContents(int iSelectMain, int iSelectIndex)
{
	if( m_pkRSHandle )	
	{
		m_strSendQuest = m_pSubMenu[iSelectMain].szText[iSelectIndex] ;
		if( m_pSubMenu[iSelectMain].iCount > iSelectIndex )
		{
			m_eHotQType = TYPE_QNV2_CONTEXT ;
			m_pkRSHandle->FuncHotQTopNV2("3", m_pSubMenu[iSelectMain].szAC[iSelectIndex] ) ;
		}
		else
		{
			ErrorFLogs->Error_Log("Select Main Index Overflow Total[%d], Select[%d]", m_pSubMenu[iSelectMain].iCount, iSelectIndex);
		}
	}
	return 0 ;
}

/**
	Edit¸¦ ÅëÇÑ Áú¹® °Ë»ö
	@param const char* pText : Edit box text
*/
int SPRsManager::SendQuestionText(const char* pText)
{
	if( pText == NULL )
		return 0;
	if( strlen(pText) < 1)
		return 0 ;

	if( m_pkRSHandle )
	{
		m_strSendQuest = pText ;
		m_pkRSHandle->FuncSendQuestionNew(m_strIDType.c_str(), m_strAccount.c_str(), pText, "0") ;
	}
	return 0 ;
}	

/**
	Context¸¦ ÅëÇÑ Áú¹® °Ë»ö
	@param const char* pText : Clicked context
*/
int SPRsManager::SendQuestionContext(const char* pText)
{
	if( m_pkRSHandle )
	{
		m_strSendQuest = pText ;
		m_pkRSHandle->FuncSendQuestionNew(m_strIDType.c_str(), m_strAccount.c_str(), pText, m_strlUserLogIdx.c_str()) ;
	}
	return 0 ;
}	

//---------------------------------------------------------------------
/**
	Judge ¿¡¼­ [YES]¼±ÅÃ½Ã
*/
int SPRsManager::SendJudgeAnsYes()
{
	if( m_pkRSHandle )
		m_pkRSHandle->FuncSendJudgeAnswer(m_strlUserLogIdx.c_str(), "1") ;
	return 0 ;
}

/**
	GetUnsatReason ºÒ¸¸Á· ¼±ÅÃ½Ã
*/
int SPRsManager::SendGetUnsatReason()
{
	if( m_pkRSHandle )
	{
		m_pkRSHandle->FuncSendGetUnsatReason(m_strIDType.c_str(), m_strAccount.c_str()) ;
	}
	return 0 ;
}

/**
	UnSatisfiReason ÀÔ·Â½Ã (szReasonÀÌ ÀÖÀ»¶§¿Í ¾øÀ»¶§ ±¸ºÐ)
*/
int SPRsManager::SendJudgeUnsatReason(int iSelectIndex, const char* szReason/*= NULL*/)
{
	char szSelectIndex[32] ;
	if( m_pkRSHandle )
	{
		if( szReason == NULL )
		{
			sprintf(szSelectIndex, "%d", iSelectIndex) ;
			m_pkRSHandle->FuncSendJudgeUnsatReason(m_strlUserLogIdx.c_str(), "3", szSelectIndex) ;
		}
		else
			m_pkRSHandle->FuncSendJudgeUnsatReason(m_strlUserLogIdx.c_str(), "3", "-1", szReason) ;
	}
	return 0 ;
}



//---------------------------------------------------------------------
void SPRsManager::SetMainMenuToWindow()
{
	g_pInterfaceManager->SPChildWndSendMessage(WIID_RSCLIENT, SPIM_SDRS_RECV_MAINMENU, 0, (LPARAM)m_pMainMenu) ;
	// After MainMenu -> Request HotTop10
	g_pkRsManager->SendQHotTop10() ;
}

void SPRsManager::SetSubMenuToWindow()
{
	g_pInterfaceManager->SPChildWndSendMessage(WIID_RSCLIENT, SPIM_SDRS_RECV_SUBMENU, 0, (LPARAM)&m_pSubMenu[m_iSelectMain]) ;
}

///////////////////////////////////////////////////////////////////////////////////
// XML Parsing function
void RsParsing(int iMsg, char* packet)
{
	switch (iMsg)
	{
		CASE_RS_PROC( RETN_JUDGEANSWER_OK		);		//£º³É¹¦			//Send OK
		CASE_RS_PROC( RETN_JUDGEANSWER_SYSERR	);		//£ºÏµÍ³¹ÊÕÏ		// System Error
		CASE_RS_PROC( RETN_JUDGEANSWER_SYSREFUSE);		//£ºÏµÍ³¾Ü¾ø		// System Refused request
		CASE_RS_PROC( RETN_JUDGEANSWER_PARAMERR	);		//£º²ÎÊý´íÎó		// Parameter error
		CASE_RS_PROC( RETN_JUDGEANSWER_BUSY		);		//£ºÏµÍ³Ã¦			// System busy
		CASE_RS_PROC( RETN_JUDGEANSWER_NOMATCH	);		//£ºÎÞÆ¥ÅäÎÊÌâ		// no match answer or question check failed

		CASE_RS_PROC( RETN_SENDQNEW_OK			);		//£º£¨ÄÚÈÝ£©·µ»Ø´ð°¸										// Return right content
		CASE_RS_PROC( RETN_SENDQNEW_SYSERR		);		//£ºÏµÍ³¹ÊÕÏ												// System Error
		CASE_RS_PROC( RETN_SENDQNEW_SYSREFUSE	);		//£ºÏµÍ³¾Ü¾ø												// System Refused request
		CASE_RS_PROC( RETN_SENDQNEW_PARAMERR	);		//£º²ÎÊý´íÎó												// Parameter error
		CASE_RS_PROC( RETN_SENDQNEW_BUSY		);		//£ºÏµÍ³Ã¦£¨ÄÚÈÝ£©ÀýÈç£º·µ»Ø"ÏµÍ³Ã¦£¬Çë1·ÖÖÓºóÔÙÊÔ¡­."		// System busy
		CASE_RS_PROC( RETN_SENDQNEW_NOMATCH		);		//£º£¨ÄÚÈÝ£©ÎÞÆ¥Åä´ð°¸»òÕßÔàÓïµÈ¾¯¸æÓï						// no match answer or question check failed

		CASE_RS_PROC( REASONSTAISFIED_OK		);
		CASE_RS_PROC( REASONSTAISFIED_SYSERR	);
		CASE_RS_PROC( REASONSTAISFIED_SYSREFUSE	);
		CASE_RS_PROC( REASONSTAISFIED_PARAMERR	);
		CASE_RS_PROC( REASONSTAISFIED_BUSY		);

		CASE_RS_PROC( RETSZ_HotQTopNV2_OK		);
		CASE_RS_PROC( RETSZ_HotQTopNV2_NOMATCH	);
	default:
		ErrorFLogs->Error_Log("@@@@ Cant Find Parsing [%d]", iMsg) ;
	}
}

//#endif