// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPClientOptionArchive.h
// Comment     : 
// Creation    : metalgeni 2004-10-31 오후 12:13:47
//***************************************************************************

#pragma once

class SPManager;
class SPClientOptionArchive : public SPManager {
public:
	SPClientOptionArchive();
	virtual ~SPClientOptionArchive();

	virtual bool	Init();
	virtual void	Clean();
	virtual int		SPPerformMessage	(UINT msg, WPARAM wparam = 0, LPARAM lParam = 0);

	const SERVER_SIDE_OPTION GetServerSideOption(); 
	SPOptionStruct*   GetOptionStruct(); 

	void SetDefault(SPOptionStruct* pOption = NULL);

	/**
	 * <br>
	 @param
	 @return
	*/
	void OptionCopy(SPOptionStruct* stDest, SPOptionStruct* stSrc = NULL);

	void SetServerSideOption(const SERVER_SIDE_OPTION iServerOption);
	
	bool ClientSave();	
	
	/**
	 * ID를 레지스트리 옵션에 살짝 저장한다  <br> 
	 @param std::string strID = ""
	 @return bool
	*/
	bool SaveUserID(std::string strID = "");
	string  GetUserID() { return m_szLoginID; }

	/**
	 * 레지스트리 옵션에 살짝 저장한 ID를 가져온다 <br>
	 @param NONE
	 @return std::string
	*/
	//std::string LoadUserID();
	bool LoadUserID(std::string& strID);

	bool SaveLastLoginServer( std::string strLoginServer );
	bool LoadLastLoginServer( std::string& strLoginServer );

	bool IsTrade();
	bool IsTeam();
	bool IsAlert();
	bool IsWantTeam();
	bool IsWhisper();
	bool IsChatTeam();
	bool IsChatFriend();
	bool IsChatGuild();

	/**
	 * 스크린샷 포멧을 확인한다 <br>
	 @param NULL
	 @return int
	*/
	int GetScreenShotFormat()	{return m_OptionStruct.iScreenShot;}

private:

	SPOAMESSAGE_MAP_DECLARE()	
	int OnCommitToServer	(WPARAM wParam, LPARAM lparam);

	////
	//int OnDrawNamePlayer	(WPARAM wParam, LPARAM lparam);	
	//int OnDrawNameMonster	(WPARAM wParam, LPARAM lparam);
	//int OnDrawNameNpc		(WPARAM wParam, LPARAM lparam);
	//int OnDrawSpeechPlayer	(WPARAM wParam, LPARAM lparam);
	//int OnDrawSpeechTeam	(WPARAM wParam, LPARAM lparam);
	//int OnDrawSpeechNpc		(WPARAM wParam, LPARAM lparam);
	//int OnDrawCommTrade		(WPARAM wParam, LPARAM lparam);
	//int OnDrawCommInvite	(WPARAM wParam, LPARAM lparam);
	//int OnDrawCommAlert		(WPARAM wParam, LPARAM lparam);
	//
	int OnDrawHideMouse		(WPARAM wParam, LPARAM lparam);
	int OnCheckBGM			(WPARAM wParam, LPARAM lparam);
	int OnVolumeBGM			(WPARAM wParam, LPARAM lparam);
	int OnCheckSoundEffect	(WPARAM wParam, LPARAM lparam);
	int OnVolumeSoundEffect	(WPARAM wParam, LPARAM lparam);
	int OnVolumeGammaRamp	(WPARAM wParam, LPARAM lparam);

private:

	int m_iVer;
	SPOptionStruct		m_OptionStruct;
	char				m_szLoginID[LOGIN_ID_LEN];
	char				m_szLoginServer[_MAX_PATH];
	SERVER_SIDE_OPTION	m_usServerSideOption;

	bool	m_bHasChanged;
	bool	m_bLockReceive;

	std::string			m_strCompanyName;
	

};

extern SPClientOptionArchive* g_pClientOption;


