
// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPRsManager.h
//	created:	2007/10/1   Time:11:55
//	purpose:	
//*********************************************************************/

#pragma once 

//#ifdef _INCLUDE_RSCLIENT_

#include <string>
#include "RSSDKdefine.h"

using namespace std;

#define DECLARE_RS_PROC(X)	void OnRS_##X(int iMsg, char* packet)
#define CASE_RS_PROC(X)		case X:{ g_pkRsManager->OnRS_##X(iMsg, packet);}break
#define DEFINE_RS_PROC(X)	void SPRsManager::OnRS_##X(int iMsg, char* packet)
#define ERROR_RS_PROC(X)	void SPRsManager::RETSZ_##X(int iMsg, char* packet)

//#ifndef SAFE_DELETE
//#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
//#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
//#endif

#define RS_DEF_SERVER_IP		"61.152.103.69"			// Test Server
#define RS_DEF_SERVER_PORT		"8025"					// Test Server
#define RS_DEF_GAMETYPE			39
#define RS_DEF_AREA				1
#define RS_DEF_USERID			"test"
#define RS_DEF_IDTYPE			"1"


enum HotQTopNV2_TYPE {
	TYPE_QNV2_MAIN		= 0,
	TYPE_QNV2_SUB		= 1,
	TYPE_QNV2_CONTEXT	= 2,
} ;



class SPRSPacketHandler;
class SPRsManager
{
public:
	static SPRsManager* GetInstance();
	static void Release();

	bool Init();
	void Clear();

	//---------------------------------------------------------------------
	// For Handler Param
	void SetNetParam		(const char* szRSIP, const char*szRSPort) ;		// RSSDK IP, PORT Setting
	void SetGameType		( int iGMType, int iArea );	
	//---------------------------------------------------------------------
	// For Local Param
	void SetlUserLogIdx		( const char* szlUserLogIdx );
	void SetAnsID			( const char* szAnsID );
	void SetIDType			( const char* szIDType );	
	void SetAccount			( const char* szAccount );
	void SetQuestion		( const char* szQuestion );
	
	void SetMainMenuToWindow() ;
	void SetSubMenuToWindow() ;
	//---------------------------------------------------------------------
	// RS Send Function
	int SendQMainMenu() ;
	int SendQSubMenu(const char* pAC) ;				// for MainMenu's Content lists (SubMenu)
	int SendQSubMenu(int iSelectIndex) ;
	int SendQHotTop10() ;
	int SendQSubMenuContents(const char* pAC) ;		// for SubMenu's Content lists
	int SendQSubMenuContents(int iSelectMain, int iSelectIndex) ;
	int SendQuestionText(const char* pText) ;
	int SendQuestionContext(const char* pText) ;
	int SendJudgeAnsYes() ;
	int SendGetUnsatReason() ;
	int SendJudgeUnsatReason(int iSelectIndex, const char* szReason = NULL) ;

	//---------------------------------------------------------------------
	// HotQTopNV2 Parsing
	void HotQTopNV2_MainMenu(char* packet) ;
	void HotQTopNV2_SubMenu(char* packet) ;
	void HotQTopNV2_Contents(char* packet) ;

	//---------------------------------------------------------------------
	bool HasMenu(int iSelectMain)				{	return (m_bHasMainMenu && m_bHasSubMenu[iSelectMain]) ;		}
	HotQTopNV2_TYPE GetHopQNV2Type()			{	return m_eHotQType;		}
	//---------------------------------------------------------------------
	void SetMainMenu(int iSelect)				{	m_iSelectMain = iSelect;	}
	int GetMainMenu()							{	return m_iSelectMain;		}
	const char* GetQuestionTitle()				{	return m_strSendQuest.c_str() ;	 }

protected:
	SPRsManager();
	virtual ~SPRsManager();

protected:
	static SPRsManager* m_pkInstance;

private:
	bool			m_bHasMainMenu ;
	bool			m_bHasSubMenu[5] ;
	HotQTopNV2_TYPE m_eHotQType ;
	RS_MENU*		m_pMainMenu ;
	RS_MENU*		m_pSubMenu ;
	int				m_iSelectMain ;
	// argument list
	string			m_strIDType;
	string			m_strAccount;
	string			m_strQuestion;
	string			m_strlUserLogIdx;
	string			m_strAnsID;
	string			m_strSendQuest ;		// 龙巩 力格

	SPRSPacketHandler*  m_pkRSHandle;

public:
	DECLARE_RS_PROC( RETN_JUDGEANSWER_OK		);		//：成功			//Send OK
	DECLARE_RS_PROC( RETN_JUDGEANSWER_SYSERR	);		//：系统故障		// System Error
	DECLARE_RS_PROC( RETN_JUDGEANSWER_SYSREFUSE);		//：系统拒绝		// System Refused request
	DECLARE_RS_PROC( RETN_JUDGEANSWER_PARAMERR	);		//：参数错误		// Parameter error
	DECLARE_RS_PROC( RETN_JUDGEANSWER_BUSY		);		//：系统忙			// System busy
	DECLARE_RS_PROC( RETN_JUDGEANSWER_NOMATCH	);		//：无匹配问题		// no match answer or question check failed

	DECLARE_RS_PROC( RETN_SENDQNEW_OK			);		//：（内容）返回答案										// Return right content
	DECLARE_RS_PROC( RETN_SENDQNEW_SYSERR		);		//：系统故障												// System Error
	DECLARE_RS_PROC( RETN_SENDQNEW_SYSREFUSE	);		//：系统拒绝												// System Refused request
	DECLARE_RS_PROC( RETN_SENDQNEW_PARAMERR		);		//：参数错误												// Parameter error
	DECLARE_RS_PROC( RETN_SENDQNEW_BUSY			);		//：系统忙（内容）例如：返回"系统忙，请1分钟后再试…."		// System busy
	DECLARE_RS_PROC( RETN_SENDQNEW_NOMATCH		);		//：（内容）无匹配答案或者脏语等警告语						// no match answer or question check failed

	DECLARE_RS_PROC( REASONSTAISFIED_OK			);		// UnSatisfyReason
	DECLARE_RS_PROC( REASONSTAISFIED_SYSERR		);
	DECLARE_RS_PROC( REASONSTAISFIED_SYSREFUSE	);
	DECLARE_RS_PROC( REASONSTAISFIED_PARAMERR	);
	DECLARE_RS_PROC( REASONSTAISFIED_BUSY		);

	//---------------------------------------------------------------------
	DECLARE_RS_PROC( RETSZ_HotQTopNV2_OK		);
	DECLARE_RS_PROC( RETSZ_HotQTopNV2_NOMATCH	);


#define HOTNODEQUESTION_OK					0x1C10		//：（内容）返回答案
#define HOTNODEQUESTION_SYSERR				0x1C11		//：系统故障
#define HOTNODEQUESTION_SYSREFUSE			0x1C12		//：系统拒绝
#define HOTNODEQUESTION_PARAMERR			0x1C13		//：参数错误
#define HOTNODEQUESTION_TOOLONG				0x1C14		//：结果超过数据包长度"
#define HOTNODEQUESTION_NOMATCH				0x1C15		//：查询无记录

};

//---------------------------------------------------------------------
// XML Parsing to 
void RsParsing(int iMsg, char* packet);
//---------------------------------------------------------------------

extern SPRsManager* g_pkRsManager;

//#endif