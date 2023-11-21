
// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-17 오후 2:56:36
//***************************************************************************


#pragma once

class SPManager;
class SPTerrainManager;	
class SPGOBManager;		
class SPFXManager;		
class SPInputManager;		
class SPInterfaceManager;	
class SPCameraManager;	
class SPNetworkManager;
class SPResourceManager;
class SPEventManager;		//[2005/4/20] - jinhee
class SPCheckManager;		//[2005/9/27] - jinhee
class SPLogoThread;


struct ReportStuff {
	GU_ID			ObjectGUID;					// 게임오브젝트아이디
	SYSTEMTIME		stServerTime;				// 서버시간
	WORLD_ID		wWorldID;					// 월드아이디
	SERVER_ID		wServerID;					// 서버아이디
	TCHAR			szAccountID[MAX_LEN_ID];	// 계정아이디
	TCHAR			szClientIP[LEN_IP];			// 클라이언트아이피
	TCHAR			szName[LEN_NAME+1];			// 캐릭터명
	UINT			uiStageID;					// 스테이지아이디
	UINT			uiMapGroupID;				// 맵그룹아이디
	UINT			uiPosX;						// X좌표
	UINT			uiPosY;						// Y좌표
};




DWORD WINAPI InitializeThread(LPVOID lParam);

class SPMainGameManager {
public:
	SPMainGameManager();
	virtual ~SPMainGameManager();
	
	void					Init();	
	void					InitAllocate();
	void					PostInit();
	void					Render();
	bool					Process();
	
	/**
	 * 공지 메시지 용 <br>  
	 @param float fTime
	 @return NONE
	*/
	void					ProcessNotice(float fTime);
	
	bool					WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	void					RenderText();

	/**
	 * 공지 메시지 용 <br>  
	 @param NONE 
	 @return NONE
	*/
	void					RenderNotice();

	bool					ChangeSubGame(SUBGAME_ID SubGameID, bool bPurgeAll = true);	//bPurgeAll은 임시로 추가
	SUBGAME_ID				GetSubGameID		()	{ return m_pMgrSubGame->GetSubGameID(); }
	SPSubGameManager*		GetSubGameManager	()	{ return m_pMgrSubGame;		}

	SPTerrainManager*		GetTerrainManager	()	{ return m_pMgrTerrain;		}
	SPGOBManager*			GetGOBManager		()	{ return m_pMgrGob;			}
	SPFXManager*			GetFXManager		()	{ return m_pMgrFX;			}
	SPInputManager*			GetInputManager		()	{ return m_pMgrInput;		}
	SPInterfaceManager*		GetInterfaceManager	()	{ return m_pMgrInterface;	}
	SPCameraManager*		GetCameraManager	()	{ return m_pMgrCamera;		}
	SPResourceManager*		GetResourceManager	()	{ return m_pMgrResource;	}
	SPNetworkManager*		GetNetworkManager	()	{ return m_pMgrNetwork;		}
	SPEventManager*			GetEventManager		()	{ return m_pMgrEvent;		}
	void					SetExit()				{ m_bExit = true; }	

	bool					GetRenderDebugInfo()	{ return m_bRenderDebugInfo; }

	void					BeginInitialize();
	bool					GetInitializeComplete()	{ return m_bInitialize; }
	bool					IsProcessReady();
	friend DWORD WINAPI InitializeThread(LPVOID lParam);

	void					OnSetNotice(std::string strMsg, float fDisplayTime = 0, int iOption = 0);

	void					InitLogoDisplay() ;
	void					ProcessLogoDisplay() ;
	void					DestroyLogoDisplay() ;

	void					WebEnableCallBack(bool bEnable);

	bool					SnapShot();	
	bool					ReportShot(ReportStuff* pReportStuff);	


protected:	

	float						m_fOffsetX;
	float						m_fOffsetY;

	bool						m_bActive;

	GAME_STATE					m_iGameState;

	SPCameraManager*			m_pMgrCamera;
	SPTerrainManager*			m_pMgrTerrain;
	SPGOBManager*				m_pMgrGob;
	SPFXManager*				m_pMgrFX;
	SPInputManager*				m_pMgrInput;
	SPInterfaceManager*			m_pMgrInterface;
	SPResourceManager*			m_pMgrResource;
	SPNetworkManager*			m_pMgrNetwork;
	SPEventManager*				m_pMgrEvent;	//[2005/4/20] - jinhee
	SPCheckManager*				m_pMgrCheck;	//[2005/9/27] - jinhee
	SPLogoThread*				m_pLogoDisplay ;

	std::vector<SPManager*>		m_vpManager;

	SPSubGameManager*			m_pMgrSubGame;	

	FLOAT						m_fLastTime;
	FLOAT						m_fElapsedTime;
	FLOAT						m_fFPS;	
	FLOAT						m_fUpdateTime ;
	bool						m_bRenderDebugInfo;

	bool						m_bPlayBGM;
	bool						m_bExit;
	
	bool						m_bInitialize;
	bool						m_bOnceInitialize;
	HANDLE						m_hInitThreadHandle;

	std::string					m_strNotice;			/**< 실 출력할 공지메시지 <br> */
	std::string					m_strOrgNotice;			/**< 서버로부터 받은 오리지널 공지 메시지 <br> */
	RECT						m_rtNotice;				/**< 공지 출력 영역 <br> */
	FLOAT						m_fAccumulateNotice;	/**< 타임 계산용 <br> */
	FLOAT						m_fDelayNotice;			/**< 초별로 갱신하기 위한 용도 <br> */	
	FLOAT						m_fEndNotice;			/**< 실체 출력할 시간 초단위 <br> 0인경우 영구적으로 <br> */	
	
	int							m_iRemainTime;			/**< displayTime 이 필요한 경우 <br> */
	char						m_szRemain[32];			/**< 남은 시간 표현용 <br> */
	bool						m_bShowAll;				/**< 영구적으로 보여줄것인지 <br> true인 경우 영구 <br> */

	int							m_iReportIndicate;
	
	int							m_iNoticeOption;
};


extern SPMainGameManager*	g_pMainGameManager;
#define g_pSubGameManager	g_pMainGameManager->GetSubGameManager()




