

#include "SPSDRSClient.h"



//타임 아웃 관련: 
//
//	ServerIp：61.152.103.69,ServerPort：8025로 설정하시고 테스트 하기 바랍니다
//
//문자열길이의 최대치 관련:
//
//	문제 입력 최대 길이가 255byte,답변 최대 길이가 1800byte입니다
//

#ifdef _INCLUDE_RSCLIENT_

#pragma comment(lib, "RSSDKDLL.lib")


#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define CHECKSETPARAM(v_ret) {if(v_ret != S_OK) {RSSDK_RETURN("SetParam", 0xFFFF, "Execute Result:", "Init Params Failed."); return;}}
#define CHECKFUNC(v_errorcode, v_func) {RSSDK_RETURN(v_func, v_errorcode, "Execute Result:", "Function Execute Failed::Catch Error"); return;}



SPSDRSClient::SPSDRSClient(HWND hWnd)
: m_pSDRSClient(NULL) 
, m_bCreateSDRS(false)
, m_bShowSDRS(false)
, m_hWnd(hWnd)
, m_JudgeAnswer(JA_OK)
{
	InitializeCriticalSection(&m_OwnCriticalSection);

	m_vstrTag0x1010.push_back("AnswerID");
	m_vstrTag0x1010.push_back("AnswerDesc");
	m_vstrTag0x1010.push_back("UserLogIdx");

	m_vstrTag0x1015.push_back("AnswerID");
	m_vstrTag0x1015.push_back("RefuseAnswerDesc");
	m_vstrTag0x1015.push_back("UserLogIdx");

}

SPSDRSClient::~SPSDRSClient()
{
	DeleteCriticalSection(&m_OwnCriticalSection);
	//SAFE_RELEASE(m_pSDRSClient);
	FreeRSClient(m_pSDRSClient) ;
}


bool SPSDRSClient::Initialize()
{
	m_pSDRSClient = CreateRSClient();

	if(m_pSDRSClient)
		return true;

	return false;
}

void SPSDRSClient::SetGMType( int iGMType )	
{	
	m_iGMType =	iGMType;	
}

void SPSDRSClient::SetArea( int iArea )	
{	
	m_iArea = iArea;	
}

void SPSDRSClient::SetIDType( const char* szIDType)	
{	
	m_strIDType = szIDType;	
}

void SPSDRSClient::SetAccount( const char* szAccount)
{	
	m_strAccount = szAccount;	
}

void SPSDRSClient::SetQuestion( const char* szQuestion)
{	
	m_strQuestion = szQuestion;	
}

//void SPSDRSClient::SetRetCommand( int   iRetCommand)
//{	
//	m_iRetCommand = iRetCommand;	
//}
//
//void SPSDRSClient::SetRetContentLen( int   iRetContentLen)
//{	
//	m_iRetContentLen= iRetContentLen;	
//}

void SPSDRSClient::SetlUserLogIdx( const char* szlUserLogIdx)
{	
	m_strlUserLogIdx= szlUserLogIdx;	
}

void SPSDRSClient::SetClientIP( const char* szClientIP)
{	
	m_strClientIP = szClientIP;	
}

void SPSDRSClient::SetAnsID( const char* szAnsID)
{	
	m_strAnsID = szAnsID;	
}

void SPSDRSClient::SetJudgeAnswer( JUDGE_ANSWER JudgeAnswer)
{	
	m_JudgeAnswer = JudgeAnswer;	
}

void SPSDRSClient::RSSDK_RETURN(const char* szFuncName, int nRet, const char* szTag, const char* szContent)
{		
	int len = 50 + strlen(szFuncName) + strlen(szTag) + strlen(szContent);
	char pContent[PACK_MAX] = {"\0"};
	try
	{
		/// convert RSClient sample code CTime::GetCurrentTime().Format("%H:%M:%S") to SYSTEMTIME 
		SYSTEMTIME st;
		char szTime[64];
		GetLocalTime(&st);
		sprintf(szTime, "%d:%d:%d", st.wHour, st.wMinute, st.wSecond);
		//////////////////////////////////////////////////////////////////////////

		//sprintf(pContent,   "OperationName：%s [FinishTime:%s].\r\n", szFuncName, CTime::GetCurrentTime().Format("%H:%M:%S"));
		sprintf(pContent,   "OperationName：%s [FinishTime:%s].\r\n", szFuncName, szTime);
		sprintf(pContent, "%sReturnCommand：0x%04x.\r\n", pContent, nRet);
		sprintf(pContent, "%s%s：%s.\r\n", pContent, szTag, szContent);

		//CString str = pContent;
		//m_ctrlRetValue.SetWindowText(str);
	}
	catch (...)
	{
		return;
	}
}

void SPSDRSClient::RSSDK_RETURN(const char* szFuncName, int nRet, const char* szContent, int nContentLen, vector<string> vectRetTag)
{	
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
		//sprintf(pContent,   "OperationName：%s [FinishTime:%s].\r\n", szFuncName, CTime::GetCurrentTime().Format("%H:%M:%S"));
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




void SPSDRSClient::InitParam()
{
	m_iRetCommand = 0;
	m_iRetContentLen = PACK_MAX;
	memset(m_szRetContent, 0, PACK_MAX);
}

void SPSDRSClient::FuncSendQuestion()
{
	try
	{
		
		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		int nRet = m_pSDRSClient->SendQuestion(m_iGMType, m_iArea, m_strIDType.c_str(), m_strAccount.c_str(), m_strQuestion.c_str(), 
			m_iRetCommand, m_szRetContent, m_iRetContentLen);
		
		LeaveCriticalSection(&m_OwnCriticalSection);
		if(m_iRetCommand == RETN_SENDQ_OK)
			RSSDK_RETURN("SendQuestion", m_iRetCommand, m_szRetContent, m_iRetContentLen, m_vstrTag0x1010);
		else if(m_iRetCommand == RETN_SENDQ_NOMATCH)
			RSSDK_RETURN("SendQuestion", m_iRetCommand, m_szRetContent, m_iRetContentLen, m_vstrTag0x1015);
		else
			RSSDK_RETURN("SendQuestion", m_iRetCommand, "Execute Result:", m_szRetContent);
	}
	catch (...)
	{
		CHECKFUNC(GetLastError(), "SendQuestion");
	}
}

void SPSDRSClient::FuncSendQuestionEx()
{
	try
	{
		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		int nRet = m_pSDRSClient->SendQuestionEx(m_iGMType, m_iArea, m_strIDType.c_str(), m_strAccount.c_str(), m_strQuestion.c_str(), 
			m_iRetCommand, m_szRetContent, m_iRetContentLen);
		LeaveCriticalSection(&m_OwnCriticalSection);
		if(m_iRetCommand == RETN_SENDQEX_NOMATCH)
			RSSDK_RETURN("SendQuestionEx", m_iRetCommand, m_szRetContent, m_iRetContentLen, m_vstrTag0x1015);
		else
			RSSDK_RETURN("SendQuestionEx", m_iRetCommand, "Execute Result:", m_szRetContent);
	}
	catch (...) 
	{
		CHECKFUNC(GetLastError(), "SendQuestionEx");
	}
}

void SPSDRSClient::FuncSendQuestionNew()
{
	try
	{
		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		int nRet = m_pSDRSClient->SendQuestionNew(m_iGMType, m_iArea, m_strIDType.c_str(), m_strAccount.c_str(), m_strQuestion.c_str(),
			m_strlUserLogIdx.c_str(), m_iRetCommand, m_szRetContent, m_iRetContentLen);
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

void SPSDRSClient::FuncRecvAns()
{
	try
	{
		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		int nRet = m_pSDRSClient->RecvAnswer(m_iGMType, m_iArea, m_strlUserLogIdx.c_str(), m_strAnsID.c_str(),
			m_iRetCommand, m_szRetContent, m_iRetContentLen);
		LeaveCriticalSection(&m_OwnCriticalSection);
		RSSDK_RETURN("RecvAnswer", m_iRetCommand, "Execute Result:", m_szRetContent);
	}
	catch (...) 
	{
		CHECKFUNC(GetLastError(), "RecvAnswer");
	}
}

void SPSDRSClient::FuncJudgeAns()
{
	try
	{
		//char point[50];
		//itoa(m_ctrlPoint.GetCurSel()+1, point, 10);

		//1: OK
		//2: Part OK
		//3: Not OK
		string strJudge;
		switch(m_JudgeAnswer) {
		case JA_OK:
			strJudge = "1";
			break;
		case JA_PART_OK:
			strJudge = "2";
			break;
		case JA_NOT_OK:
			strJudge = "3";
			break;
		}

		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		int nRet = m_pSDRSClient->JudgeAnswer(m_iGMType, m_iArea, m_strlUserLogIdx.c_str(), strJudge.c_str(),
			m_iRetCommand, m_szRetContent, m_iRetContentLen);
		LeaveCriticalSection(&m_OwnCriticalSection);
		RSSDK_RETURN("JudgeAnswer", m_iRetCommand, "Execute Result:", m_szRetContent);
	}
	catch (...) 
	{
		CHECKFUNC(GetLastError(), "JudgeAnswer");
	}
}

void SPSDRSClient::FuncHotQTopN()
{
	try
	{
		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		int nRet = m_pSDRSClient->HotQuestionTopN(m_iGMType, m_iArea, m_strIDType.c_str(), m_strAccount.c_str(),
			m_iRetCommand, m_szRetContent, m_iRetContentLen);
		LeaveCriticalSection(&m_OwnCriticalSection);
		RSSDK_RETURN("HotQuestionTopN", m_iRetCommand, "Execute Result:", m_szRetContent);
	}
	catch (...) 
	{
		CHECKFUNC(GetLastError(), "HotQuestionTopN");
	}
}

void SPSDRSClient::FuncSendQuestionV2()
{
	try
	{
		EnterCriticalSection(&m_OwnCriticalSection);
		InitParam();
		int nRet = m_pSDRSClient->SendQuestionV2(m_iGMType, m_iArea, m_strIDType.c_str(), m_strAccount.c_str(), m_strQuestion.c_str(),
			m_strlUserLogIdx.c_str(), m_strClientIP.c_str(), m_iRetCommand, m_szRetContent, m_iRetContentLen);
		LeaveCriticalSection(&m_OwnCriticalSection);
		if(m_iRetCommand == RETN_SENDQV2_NOMATCH)
			RSSDK_RETURN("SendQuestionV2", m_iRetCommand, m_szRetContent, m_iRetContentLen, m_vstrTag0x1015);
		else
			RSSDK_RETURN("SendQuestionV2", m_iRetCommand, "Execute Result:", m_szRetContent);
	}
	catch (...) 
	{
		CHECKFUNC(GetLastError(), "SendQuestionV2");
	}
}

void SPSDRSClient::GetReasonOfUnSatisfaction()
{
	EnterCriticalSection(&m_OwnCriticalSection);
	InitParam();
	int nRet = m_pSDRSClient->GetReasonOfUnSatisfaction(39, 1, "1", "test", m_iRetCommand, m_szRetContent, m_iRetContentLen);

	LeaveCriticalSection(&m_OwnCriticalSection);

}

void SPSDRSClient::JudgeUnsatisfiedReason()
{
	EnterCriticalSection(&m_OwnCriticalSection);
	InitParam();
	int nRet = m_pSDRSClient->JudgeUnsatisfiedReason(39, 1, "[dbo].[TUserLog20070724] 8", "3", "4", "Test", m_iRetCommand, m_szRetContent, m_iRetContentLen);

	LeaveCriticalSection(&m_OwnCriticalSection);


}


void SPSDRSClient::FuncHotQTopNV2()
{
	EnterCriticalSection(&m_OwnCriticalSection);
	InitParam();
	int nRet = m_pSDRSClient->HotQuestionTopNV2(39, 1, "3", "RHOTQUESTION_BASE", m_iRetCommand, m_szRetContent, m_iRetContentLen);
	LeaveCriticalSection(&m_OwnCriticalSection);
}


#endif










