
//***************************************************************************
// System Name :	IRSSDK.H
// Comment     :	DO NOT MODIFY..commented by metalgeni
//					[9/4/2007 metalgeni]
// Creation    :	SNDA RSCLIENT SDK
//***************************************************************************




#pragma once

#define SDAPI __stdcall

class IRSSDK
{
public:
	virtual bool SDAPI SendQuestion(int gametype, int areacode, const char* szIDType, const char* szUserID, 
		const char* szQuestion, int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual bool SDAPI JudgeAnswer(int gametype, int areacode, const char* szQuestionID, const char* szPoint,
		int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual int SDAPI SetNetParam(char* szHostName, char* szServerPort = NULL, char* szKey = NULL, int nTimeOut = 2) = 0;
	virtual bool SDAPI SendQuestionEx(int gametype, int areacode, const char* szIDType, const char* szUserID, 
		const char* szQuestion,	int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual bool SDAPI SendQuestionNew(int gametype, int areacode, const char* szIDType, const char* szUserID, 
		const char* szQuestion,	const char* szUserLogIdx, int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual bool SDAPI RecvAnswer(int gametype, int areacode, const char* szUserLogIdx, const char* szAnsID,
		int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual bool SDAPI HotQuestionTopN(int gametype, int areacode, const char* szIDType, const char* szUserID,
		int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual bool SDAPI SendQuestionV2(int gametype, int areacode, const char* szIDType, const char* szUserID, 
		const char* szQuestion, const char* szUserLogIdx, const char* szIPAddress,
		int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;

	virtual bool SDAPI GetReasonOfUnSatisfaction(int gametype, int areacode, const char* szIDType, const char* szUserID, int& nRetCommand, char* szRetContent, 
		int& nRetContentLen) = 0;
	virtual bool JudgeUnsatisfiedReason(int gametype, int areacode, const char* szUserLogIdx, const char* szScore, 
		const char* szUnsatisfiedReasonID, const char* szUnsatisfiedReason, int& nRetCommand, char* szRetContent,
		int& nRetContentLen) = 0;

	virtual bool HotQuestionTopNV2(int GameType, int AreaCode, const char* szQueryType, const char* szAppcode, int& nRetCommand, 
		char* szRetContent, int &nRetContentLen) = 0;

};

extern IRSSDK * SDAPI CreateRSClient(void);
extern void SDAPI FreeRSClient(IRSSDK* pRsSDK);
extern void WINAPI GetDllVersion(unsigned short* MajorVersion, unsigned short* MinorVersion, unsigned short* Revision, unsigned short* Build);
