
//#ifdef _INCLUDE_RSCLIENT_

#include "SPCommon.h"

#include "SPRSPacketHandler.h"
#include "SPRsManager.h"
#include "_Error_log.h"


//타임 아웃 관련: 
//	ServerIp：61.152.103.69,ServerPort：8025로 설정하시고 테스트 하기 바랍니다
//
//문자열길이의 최대치 관련:
//	문제 입력 최대 길이가 255byte,답변 최대 길이가 1800byte입니다
//
//#pragma comment(lib, "RSSDKDLL.lib")
HMODULE					g_hModule = NULL;
CREATERSCLIENT			g_pfnRSCreate	= NULL;
FREERSCLIENT			g_pfnRSFree		= NULL;

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define CHECKSETPARAM(v_ret) {if(v_ret != S_OK) {RSSDK_RETURN("SetParam", 0xFFFF, "Execute Result:", "Init Params Failed."); return;}}
#define CHECKFUNC(v_errorcode, v_func) {RSSDK_RETURN(v_func, v_errorcode, "Execute Result:", "Function Execute Failed::Catch Error"); return;}



SPRSPacketHandler::SPRSPacketHandler()
: m_pSDRSClient(NULL) 
, m_bCreateSDRS(false)
{
	InitializeCriticalSection(&m_OwnCriticalSection);

	m_vstrTag0x1010.push_back("AnswerID");
	m_vstrTag0x1010.push_back("AnswerDesc");
	m_vstrTag0x1010.push_back("UserLogIdx");

	m_vstrTag0x1015.push_back("AnswerID");
	m_vstrTag0x1015.push_back("RefuseAnswerDesc");
	m_vstrTag0x1015.push_back("UserLogIdx");

}

SPRSPacketHandler::~SPRSPacketHandler()
{
	DeleteCriticalSection(&m_OwnCriticalSection);
	if( g_pfnRSFree && m_pSDRSClient ) {
		g_pfnRSFree( m_pSDRSClient ) ;
		m_pSDRSClient = NULL ;
	}
	if( g_hModule ) {
		FreeLibrary(g_hModule) ;		// Dll Free
		g_hModule = NULL ;
	}
}


bool SPRSPacketHandler::Initialize()
{
	TCHAR szDLL[MAX_PATH];
	_stprintf(szDLL,_T("RSSDKDLL"));
	g_hModule = LoadLibrary(szDLL);
	if(!g_hModule)
	{
		DWORD dwError = GetLastError();
		ErrorFLogs->Error_Log("●●[%s] DLL Library Load Fail!!!", szDLL) ;
		return false;
	}
	g_pfnRSCreate		= (CREATERSCLIENT)GetProcAddress(g_hModule,FN_CREATERSCLIENT);
	if(!g_pfnRSCreate) {
		ErrorFLogs->Error_Log("●●DLL Function Load Fail[%s]", FN_CREATERSCLIENT) ;
		return false ;
	}
	g_pfnRSFree		= (FREERSCLIENT)GetProcAddress(g_hModule,FN_FREERSCLIENT);
	if(!g_pfnRSFree) {
		ErrorFLogs->Error_Log("●●DLL Function Load Fail[%s]", FN_FREERSCLIENT) ;
		return false ;
	}

	//---------------------------------------------------------------------
	//m_pSDRSClient = CreateRSClient();
	if( g_pfnRSCreate )
		m_pSDRSClient = g_pfnRSCreate();

	if( m_pSDRSClient ) {
		m_bCreateSDRS = true ;
		ErrorFLogs->Error_Log("●●DLL Function Create m_pSDRSClient SUCCESS") ;
		return true;
	}

	ErrorFLogs->Error_Log("●●DLL Function Create m_pSDRSClient Fail") ;
	return false;
}

void SPRSPacketHandler::SetGameType( int iGMType, int iArea )	
{	
	m_iGMType =	iGMType;
	m_iArea = iArea;
}

void SPRSPacketHandler::SetNetParam( const char* szRSIP, const char* szRSPort )
{	
	strcpy( m_szRSIP, szRSIP ) ;
	strcpy( m_szRSPort, szRSPort ) ;
}


/**
	Return Function for Success or ErrorMessages
*/
void SPRSPacketHandler::RSSDK_RETURN(const char* szFuncName, int nRet, const char* szTag, const char* szContent)
{
	if( false == m_bCreateSDRS )		// If Not Use
		return ;

	int len = 50 + strlen(szFuncName) + strlen(szTag) + strlen(szContent);
	char pXmlData[PACK_MAX] = {"\0"};
	char pContent[PACK_MAX] = {"\0"};
	try
	{
		/// convert RSClient sample code CTime::GetCurrentTime().Format("%H:%M:%S") to SYSTEMTIME 
		SYSTEMTIME st;
		char szTime[64];
		GetLocalTime(&st);
		sprintf(szTime, "%d:%d:%d", st.wHour, st.wMinute, st.wSecond);
		//////////////////////////////////////////////////////////////////////////
		sprintf(pContent,   "OperationName：%s [FinishTime:%s].\r\n", szFuncName, szTime);
		sprintf(pContent, "%sReturnCommand：0x%04x.\r\n", pContent, nRet);
		sprintf(pContent, "%s%s：%s.\r\n", pContent, szTag, szContent);

		strcpy(pXmlData, szContent) ;
		RsParsing(nRet, pXmlData) ;
	}
	catch (...)
	{
		return;
	}
}

/**
	Return Function for Exception
*/
void SPRSPacketHandler::RSSDK_RETURN(const char* szFuncName, int nRet, const char* szContent, int nContentLen, vector<string> vectRetTag)
{	
	if( false == m_bCreateSDRS )		// If Not Use
		return ;

	vector<string> vectRetContent;		
	char p[PACK_MAX] = {"\0"};
	try
	{
		memset(p, 0, nContentLen+1);
		memcpy(p, szContent, nContentLen);
		char* q = p;
		for(int i = 0; i<nContentLen; i++)
		{
			if(*q==0) 
			{
				q++;
				continue;
			}
			else
			{
				vectRetContent.push_back(q);
				q = q + strlen(q);
			}
		}
	}
	catch (...)
	{
		return;
	}

	char pContent[PACK_MAX] = {"\0"};
	try
	{		
		/// convert RSClient sample code CTime::GetCurrentTime().Format("%H:%M:%S") to SYSTEMTIME 
		SYSTEMTIME st;
		char szTime[64];
		GetLocalTime(&st);
		sprintf(szTime, "%d:%d:%d", st.wHour, st.wMinute, st.wSecond);
		//////////////////////////////////////////////////////////////////////////
		int MinSize = vectRetTag.size()>=vectRetContent.size()?vectRetContent.size():vectRetTag.size();
		sprintf(pContent,   "OperationName：%s [FinishTime:%s].\r\n", szFuncName, szTime);
		sprintf(pContent, "%sReturnName：0x%04x.\r\n", pContent, nRet);
		for(int n=0; n<MinSize; n++)
		{
			sprintf(pContent, "%s%s：%s.\r\n", pContent, vectRetTag[n].c_str(), vectRetContent[n].c_str());
		}

		//CString str = pContent;
		//m_ctrlRetValue.SetWindowText(str);
	}
	catch (...) 
	{
		return;
	}
}



/**
	Return value clear
*/
void SPRSPacketHandler::InitParam()
{
	m_iRetCommand = 0;
	m_iRetContentLen = PACK_MAX;
	memset(m_szRetContent, 0, PACK_MAX);
}


//////////////////////////////////////////////////////////////////////////
//	
//	RS Packet Sending
//	
//////////////////////////////////////////////////////////////////////////
/**
	각종 메뉴 및 목록을 얻어오기 위한 HopQuestionTopNV2
 */
void SPRSPacketHandler::FuncHotQTopNV2(const char* szQueryType, const char* szAppCode) 
{
	if( false == m_bCreateSDRS )		// If Not Use
		return ;

	try
	{
		int nRet = m_pSDRSClient->SetNetParam(m_szRSIP, m_szRSPort);
		CHECKSETPARAM(nRet);
		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		nRet = m_pSDRSClient->HotQuestionTopNV2(m_iGMType, m_iArea, szQueryType,szAppCode, m_iRetCommand, m_szRetContent, m_iRetContentLen);
		LeaveCriticalSection(&m_OwnCriticalSection);
		//if(m_iRetCommand == RETN_SENDQNEW_NOMATCH)
		//	RSSDK_RETURN("HotQuestionTopNV2", m_iRetCommand, m_szRetContent, m_iRetContentLen, m_vstrTag0x1015);
		//else
		RSSDK_RETURN("HotQuestionTopNV2", m_iRetCommand, "Execute Result:", m_szRetContent);
	}
	catch (...) 
	{
		CHECKFUNC(GetLastError(), "HotQuestionTopNV2");
	}
}

/**
	Question을 입력한 경우
 */
void SPRSPacketHandler::FuncSendQuestionNew(const char* szIDType, const char* szUserID, const char* szQuestion, const char* szUserLogIdx)
{
	if( false == m_bCreateSDRS )		// If Not Use
		return ;

	try
	{
		int nRet = m_pSDRSClient->SetNetParam(m_szRSIP, m_szRSPort);
		CHECKSETPARAM(nRet);

		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		nRet = m_pSDRSClient->SendQuestionNew(m_iGMType, m_iArea, szIDType, szUserID, szQuestion,	szUserLogIdx, m_iRetCommand, m_szRetContent, m_iRetContentLen);
		LeaveCriticalSection(&m_OwnCriticalSection);

		if(m_iRetCommand == RETN_SENDQNEW_NOMATCH)
			RSSDK_RETURN("SendQuestionNew", m_iRetCommand, m_szRetContent, m_iRetContentLen, m_vstrTag0x1015);
		else
			RSSDK_RETURN("SendQuestionNew", m_iRetCommand, "Execute Result:", m_szRetContent);
	}
	catch (...) 
	{
		CHECKFUNC(GetLastError(), "SendQuestionNew");
	}
}

/**
	만족이라고 답변한 경우 보내기
 */
void SPRSPacketHandler::FuncSendJudgeAnswer(const char* szQuestionID, const char* szPoint) 
{
	if( false == m_bCreateSDRS )		// If Not Use
		return ;

	try
	{
		int nRet = m_pSDRSClient->SetNetParam(m_szRSIP, m_szRSPort);
		CHECKSETPARAM(nRet);

		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		nRet = m_pSDRSClient->JudgeAnswer(m_iGMType, m_iArea, szQuestionID, szPoint,	m_iRetCommand, m_szRetContent, m_iRetContentLen);
		LeaveCriticalSection(&m_OwnCriticalSection);

		RSSDK_RETURN("JudgeAnswer", m_iRetCommand, "Execute Result:", m_szRetContent);
	}
	catch (...) 
	{
		CHECKFUNC(GetLastError(), "JudgeAnswer");
	}
}

/**
	불만족 이유 목록 받아오기
 */
void SPRSPacketHandler::FuncSendGetUnsatReason(const char* szIDType, const char* szUserID) 
{
	if( false == m_bCreateSDRS )		// If Not Use
		return ;

	try
	{
		int nRet = m_pSDRSClient->SetNetParam(m_szRSIP, m_szRSPort);
		CHECKSETPARAM(nRet);

		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		nRet = m_pSDRSClient->GetReasonOfUnSatisfaction(m_iGMType, m_iArea, szIDType, szUserID, m_iRetCommand, m_szRetContent, m_iRetContentLen);
		LeaveCriticalSection(&m_OwnCriticalSection);

		RSSDK_RETURN("GetReasonOfUnSatisfaction", m_iRetCommand, "Execute Result:", m_szRetContent);
	}
	catch (...) 
	{
		CHECKFUNC(GetLastError(), "GetReasonOfUnSatisfaction");
	}
}


/**
	불만족 이유를 입력 전송 및 결과
 */
void SPRSPacketHandler::FuncSendJudgeUnsatReason(const char* szUserLogIdx, const char* szScore, const char* szUnsatReasonID, const char* szUnsatReason/*=NULL*/) 
{
	if( false == m_bCreateSDRS )		// If Not Use
		return ;

	int nRet = m_pSDRSClient->SetNetParam(m_szRSIP, m_szRSPort);
	CHECKSETPARAM(nRet);

	EnterCriticalSection(&m_OwnCriticalSection);
	InitParam();
	nRet = m_pSDRSClient->JudgeUnsatisfiedReason(m_iGMType, m_iArea, szUserLogIdx, szScore, szUnsatReasonID, szUnsatReason, m_iRetCommand, m_szRetContent, m_iRetContentLen);
	LeaveCriticalSection(&m_OwnCriticalSection);

	//RSSDK_RETURN("JudgeUnsatisfiedReason", m_iRetCommand, "Execute Result:", m_szRetContent);
	// JudgeUnsatReason is can't RETURN then move to HotTop10
	g_pkRsManager->SendQHotTop10() ;
}

//#endif