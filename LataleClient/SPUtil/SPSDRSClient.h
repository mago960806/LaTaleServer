
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPSDRSClient
// Comment     : 
// Creation    : metalgeni 2004-12-03 ¿ÀÈÄ 2:56:36
//***************************************************************************


#pragma once 


#ifdef _INCLUDE_RSCLIENT_


#include <windows.h>
#include <vector>
#include <string>


#include "RSSDKdefine.h"
#include "IRSSDK.h" // DO NOT Modify..!!

//#pragma comment(lib, "SDRSClient.lib")
//#pragma comment(lib, "RSSDKDLL.lib")

using namespace std;

enum JUDGE_ANSWER {
	JA_OK,
	JA_PART_OK,
	JA_NOT_OK,
};

#define PACK_MAX 20480

class SPSDRSClient {


public:
	SPSDRSClient(HWND hWnd);
	~SPSDRSClient();

	bool Initialize();
	void InitParam();

	// From RS Sample Code
	void RSSDK_RETURN(const char* szFuncName, int nRet, const char* szTag, const char* szContent);
	void RSSDK_RETURN(const char* szFuncName, int nRet, const char* szContent, int nContentLen, vector<string> vectRetTag);

	void SetGMType			( int iGMType );	
	void SetArea			( int iArea );	
	void SetIDType			( const char* szIDType );	
	void SetAccount			( const char* szAccount );
	void SetQuestion		( const char* szQuestion );
	//void SetRetCommand		( int iRetCommand );
	//void SetRetContentLen	( int iRetContentLen );
	void SetlUserLogIdx		( const char* szlUserLogIdx );
	void SetClientIP		( const char* szClientIP );
	void SetAnsID			( const char* szAnsID );
	void SetJudgeAnswer		( JUDGE_ANSWER JudgeAnswer );

	void FuncSendQuestion();
	void FuncSendQuestionEx();
	void FuncSendQuestionNew();
	void FuncRecvAns();
	void FuncJudgeAns();
	void FuncHotQTopN();
	void FuncSendQuestionV2();
	void GetReasonOfUnSatisfaction();
	void JudgeUnsatisfiedReason();
	void FuncHotQTopNV2() ;


private:	 

	IRSSDK*			m_pSDRSClient; 
	bool			m_bCreateSDRS;
	bool			m_bShowSDRS;

	// Return Result
	int				m_iRetCommand;
	char			m_szRetContent[PACK_MAX];
	int				m_iRetContentLen;

	// argument list
	int				m_iGMType;
	int				m_iArea;
	string			m_strIDType;
	string			m_strAccount;
	string			m_strQuestion;
	string			m_strlUserLogIdx;
	string			m_strClientIP;
	string			m_strAnsID;
	JUDGE_ANSWER	m_JudgeAnswer;

	HWND			m_hWnd;

	vector<string> m_vstrTag0x1010;
	vector<string> m_vstrTag0x1015;

	CRITICAL_SECTION m_OwnCriticalSection;	


};


#endif

