
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPRSPacketHandler
// Comment     : 
// Creation    : metalgeni 2004-12-03 ¿ÀÈÄ 2:56:36
// Modify	   : Duragon 2007-11-02
//***************************************************************************


#pragma once 

//#ifdef _INCLUDE_RSCLIENT_

#include <windows.h>
#include <vector>
#include <string>


#include "RSSDKdefine.h"
#include "IRSSDK.h" // DO NOT Modify..!!


using namespace std;

//enum JUDGE_ANSWER {
//	JA_OK,
//	JA_PART_OK,
//	JA_NOT_OK,
//};

#define PACK_MAX 20480

class SPRSPacketHandler {
public:
	SPRSPacketHandler();
	~SPRSPacketHandler();

	bool Initialize();
	void InitParam();

	// for Network Setting
	void SetNetParam		( const char* szRSIP, const char*szRSPort );
	void SetGameType		( int iGMType, int iArea );	

	// for Sending 
	void FuncHotQTopNV2(const char* szQueryType, const char* szAppCode) ;
	void FuncSendQuestionNew(const char* szIDType, const char* szUserID, const char* szQuestion, const char* szUserLogIdx);
	void FuncSendJudgeAnswer(const char* szQuestionID, const char* szPoint) ;
	void FuncSendGetUnsatReason(const char* szIDType, const char* szUserID) ;
	void FuncSendJudgeUnsatReason(const char* szUserLogIdx, const char* szScore, const char* szUnsatReasonID, const char* szUnsatReason=NULL) ;

	// From RS Sample Code
	void RSSDK_RETURN(const char* szFuncName, int nRet, const char* szTag, const char* szContent);
	void RSSDK_RETURN(const char* szFuncName, int nRet, const char* szContent, int nContentLen, vector<string> vectRetTag);

private:	 
	
	IRSSDK*			m_pSDRSClient; 
	bool			m_bCreateSDRS;

	// Rs Network
	int				m_iGMType;
	int				m_iArea;
	char			m_szRSIP[50];
	char			m_szRSPort[50] ;

	// Return Result
	int				m_iRetCommand;
	char			m_szRetContent[PACK_MAX];
	int				m_iRetContentLen;


	vector<string> m_vstrTag0x1010;
	vector<string> m_vstrTag0x1015;

	CRITICAL_SECTION m_OwnCriticalSection;	
};

//#endif
