
// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-11-29 오후 2:56:36
//***************************************************************************


#pragma once

enum SUBGAME_ID {
	SUBGAME_NULL		= -1,

	SUBGAME_FIRST		= 0,

	SUBGAME_LOGGIN				= 0,
	SUBGAME_LOBBY				= 1,
	SUBGAME_PROLOGUE			= 2,
	SUBGAME_LOADING				= 3,
	SUBGAME_ZONE				= 4,
	
	SUBGAME_LAST		= SUBGAME_ZONE,
};

// 실행옵션
enum EXECUTION_MODE {
	EMODE_NULL			= 0,
	EMODE_NORMAL		= EMODE_NULL,
	EMODE_CHAR_SELECT	= 1,
};


class SPClientCommandBinder;
class SPSubGameManager : public SPManager {
public:
	SPSubGameManager(SUBGAME_ID SubGameID);
	virtual ~SPSubGameManager();

	virtual void			Process	(float fTime);
	virtual	void			Render	(float fTime);
	virtual bool			Init	();
	virtual void			Clean();	
	
	virtual bool			WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual int				SPPerformMessage	(UINT msg, WPARAM wparam = 0, LPARAM lParam = 0);
	virtual void			RenderText();
	
	bool					IsRelease()		{ return m_iGameState == GS_RELEASE;	}
	bool					IsActive()		{ return m_bActive;		}
	GAME_STATE				GetGameState()	{ return m_iGameState;	}
	void					SetGameState(GAME_STATE iState)		{ m_iGameState = iState;}	//[2005/10/17]

	SUBGAME_ID				GetSubGameID()		{ return m_SubGameID; 	}
	SUBGAME_ID				GetNextSubGameID()	{ return m_NextSubGameID; 	}	
	
	bool					ClientCommandExcute(const char* pszCommand);
	bool					ServerCommandExcute(const char* pszCommand);

protected:
	SPGMESSAGE_MAP_DECLARE()

	virtual int OnCharSCSelect	(WPARAM wparam, LPARAM lparam);
	virtual int OnSetBGM		(WPARAM wparam, LPARAM lparam);
	virtual int OnSocketFailed	(WPARAM wparam, LPARAM lparam);
	//virtual int OnWorldEnter	(WPARAM wParam, LPARAM lParam);

	virtual int OnUserIn		(WPARAM wParam, LPARAM lParam);
	virtual int OnUserMove		(WPARAM wParam, LPARAM lParam);

protected:
	
	bool					m_bActive;
	GAME_STATE				m_iGameState;

	SUBGAME_ID				m_SubGameID;
	SUBGAME_ID				m_NextSubGameID;

	std::string				m_strLog;
	float					m_fWaitTime;

	SPClientCommandBinder*	m_pCommandBinder;
	
};

extern SPSubGameManager*	g_pSubGameManager;
extern EXECUTION_MODE		g_ExcutionMode;



