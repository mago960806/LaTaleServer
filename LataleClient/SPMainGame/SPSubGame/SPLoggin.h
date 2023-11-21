
// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-11-29 오후 2:56:36
//***************************************************************************


#pragma once


enum LOGIN_SHOW_STATE {
	SHOW_NON_DLG = 0,
	SHOW_LOGIN_DLG,
	SHOW_EKEY_DLG,
	SHOW_SERIAL_DLG,
};



class SPFXArchive;
class SPTimer;
enum NOTICE_TYPE;
class SPLoggin : public SPSubGameManager 
{
public:
	SPLoggin();
	virtual ~SPLoggin();

	virtual void			Render(float fTime);
	virtual void			Process(float fTime);
	virtual bool			Init();
	
	virtual bool			WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void			RenderText();

	virtual int				SPPerformMessage	(UINT msg, WPARAM wparam = 0, LPARAM lParam = 0);

protected:

	void PrintLogs(const TCHAR* fmt, ...) ;

	bool IsWebLogin();
//	bool ParseWebArgs() ;
	bool ParseWebArgs_New() ;
	void ReserveExit() ;
	bool ShowNoticeBox(NOTICE_TYPE iType, const TCHAR* szMsg);
	void ShowHomePage(const TCHAR* szURL) ;
	void ForceCloseNotice();

	SPGMESSAGE_MAP_DECLARE()
	// Network Event	
	int OnResultAuthLogin	(WPARAM wparam, LPARAM lparam);	
	int OnResultAuthEkey	(WPARAM wparam, LPARAM lparam);	
	int OnResultAuthSerial	(WPARAM wparam, LPARAM lparam);	
	int OnResultEkeyResult	(WPARAM wparam, LPARAM lparam);	

	// Other Evnet
	int OnReqConnectTry		(WPARAM wparam, LPARAM lparam);		// ID/Pass Auth
	int OnReqEkeyConnectTry	(WPARAM wparam, LPARAM lparam);		// Ekey Auth
	int OnReqSerialConnectTry	(WPARAM wparam, LPARAM lparam);	// Serial Auth
	int OnCheckWebLogin		(WPARAM wparam, LPARAM lparam);
	int OnCheckReserveExit(WPARAM wparam, LPARAM lparam);
	int OnCheckTimeOut(WPARAM wparam, LPARAM lparam);

	int OnSoftKeyPress(WPARAM wparam, LPARAM lparam);
	int OnSoftKeyPressBack(WPARAM wparam, LPARAM lparam);
	int OnSoftKeyPressEnter(WPARAM wparam, LPARAM lparam);
	int OnSoftKeyPressClose(WPARAM wparam, LPARAM lparam);

	int OnSocketFailed	(WPARAM wparam, LPARAM lparam);

	int OnUserIn(WPARAM wParam, LPARAM lParam);
	int OnUserMove(WPARAM wParam, LPARAM lParam);
	int IGWLogin();


protected:	
	LOGIN_SHOW_STATE m_iShowState ;
	//char m_szWebID[128];
	//char m_szWebKey[1024];
	std::string m_strWebID;
	std::string m_strWebKey;

	bool	m_bReserveExit ;		// Exit Reserve
	bool	m_bConnectTimeOut ;		// Connect Timeout Flag
	bool	m_bWebConnectTry ;


	//////////////////////////////////////////////////////////////////////////
	SPTexture*	m_pInterface;		//[2005/3/21] /**< Login Interface Image <br> */

	//////////////////////////////////////////////////////////////////////////
	SPTexture*		m_pLogInImage;	// 1. Back Image Layer
	SPFXArchive*	m_pkFXArchive;	// 2. Back FX01 Layer
	SPTexture*		m_pLogInImage2;	// 3. Front Image Layer
	SPFXArchive*	m_pkFXArchive2;	// 4. Front FX02 Layer
	//////////////////////////////////////////////////////////////////////////


	std::string m_strVersion;		//[2005/3/23] /**< Version 정보 텍스트 <br> */

	RECT		m_rtSrc1;			//[2005/3/21] /**< 로그인 이미지 1 번 캐릭터 위치하는곳 <br> */
	RECT		m_rtSrc2;			//[2005/3/21] /**< 로그인 이미지 2 번 인터페이스 위치하는곳 <br> */
	RECT		m_rtSrc3;			//[2005/3/21] /**< 로그인 이미지 3 번 왼쪽 바닥 <br> */
	RECT		m_rtSrc4;			//[2005/3/21] /**< 로그인 이미지 4 번 바닥 <br> */
	RECT		m_rtSrc5;			//[2005/3/21] /**< 로그인 이미지 5 번 바닥 <br> */
	RECT		m_rtSrc6;			//[2005/3/21] /**< 로그인 이미지 6 번 바닥 <br> */

	RECT		m_rtDest1;			//[2005/3/21] /**< 로그인 이미지 1 번 캐릭터 위치하는곳 <br> */
	RECT		m_rtDest2;			//[2005/3/21] /**< 로그인 이미지 2 번 인터페이스 위치하는곳 <br> */
	RECT		m_rtDest3;			//[2005/3/21] /**< 로그인 이미지 3 번 왼쪽 바닥 <br> */
	RECT		m_rtDest4;			//[2005/3/21] /**< 로그인 이미지 4 번 바닥 <br> */
	RECT		m_rtDest5;			//[2005/3/21] /**< 로그인 이미지 5 번 바닥 <br> */
	RECT		m_rtDest6;			//[2005/3/21] /**< 로그인 이미지 6 번 바닥 <br> */

	RECT		m_rtSrcInterBase;	//[2005/3/21] /**< 인터페이스 베이스 <br> */
	RECT		m_rtSrcAgeMark;		//[2005/3/21] /**< 이용가 <br> */
	RECT		m_rtDestAgeMark;
	RECT		m_rtVer;			//[2005/3/23] /**< 버전 <br> */	

	float		m_fAccumulateRender;//[2005/3/22] /**< 렌더링 시간 계산 <br> */
	float		m_fCurDelay;		//[2005/3/22] /**< 프레임 간격 시간 <br> */
	float		m_fAccumulateWebLoginSend;		// Calculate WebLogin Sending Time 
	
	D3DXCOLOR	m_LogColor;
	SPTimer*	m_pEkeyTimer ;
	SPTimer*	m_pLoginTimer ;
};


