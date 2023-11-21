// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 오전 10:26:19
//***************************************************************************

#pragma once

//#define WIID_MINIMAP_BASE			(WIID_MINIMAP	+	10 )	//컨트롤 바탕
#define WIID_MINIMAP_ICON			(WIID_MINIMAP	+	20 )	//테마 아이콘
#define WIID_MINIMAP_NAME			(WIID_MINIMAP	+	30 )	//맵 이름
#define WIID_MINIMAP_NAME_BOLD		(WIID_MINIMAP	+	31 )	//맵 이름 진하게
#define WIID_MINIMAP_INFO_BG		(WIID_MINIMAP	+	40 )	//맵 번호 배경
#define WIID_MINIMAP_INFO			(WIID_MINIMAP	+	41 )	//맵 번호(좌표)
#define WIID_MINIMAP_BUTTON			(WIID_MINIMAP	+	50 )	//미니맵 닫기 열기 버튼
#define WIID_MINIMAP_VIEW			(WIID_MINIMAP	+	60 )	//미니맵 View

#define MINIMAP_BG_COUNT			3

class SPWindow;
class SPWindowMiniMapView;

enum GROUP_TYPE;

class SPWindowMiniMap : public SPWindow {

public:
	SPWindowMiniMap(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowMiniMap(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowMiniMap();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);					//	보여주고 안보여주고를 체크하기위해

	void	ResetData();

public:


protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnCursorEnter	( WPARAM, LPARAM );
	virtual int OnCursorOut		( WPARAM, LPARAM );
	virtual int OnCursorLDown	( WPARAM, LPARAM );
	virtual int OnCursorLUp		( WPARAM, LPARAM );
	virtual int OnMiniMapButton	( WPARAM, LPARAM );		//	미니맵 버튼

	int OnSetStageID			( WPARAM wParam, LPARAM lParam); //StageID, GroupID 둘다 받는다.
	int OnRefreshMiniMapGOB		( WPARAM wParam, LPARAM lParam); //WPARAM엔 Add,Del 속성 LPARAM엔 GOB포인터
	int OnRefreshIconTeam		( WPARAM wParam, LPARAM lParam); //WPARAM엔 Add , Del 타입 , LPARAM엔 GU_ID
	int	OnPurgeAll				( WPARAM wParam, LPARAM lParam );	// 채널간 이동시 데이터 초기화 AJJIYA [8/3/2005]

	int	OnWorldMap				( WPARAM wParam, LPARAM lParam );
	int	OnStageMap				( WPARAM wParam, LPARAM lParam );

	// }}
	
protected:
	void UpdateMapInfo();
	void UpdatePos();
	void ShowMiniMapButton( bool bStatus );
	void ShowThemeIcon( int iResID , int iResIndex );
	void ReposOwnImage();

	//////////////////////////////////////////////////////////////////////////
	//
	//
	//

	std::string m_strMapName;		/**< Map Name <br> */
	char m_szMapInfo[128];			/**< Map Information <br> */
	
	int m_iStageID;					/**< Stage ID <br> */
	int m_iMapGroupID;				/**< Map Group ID <br> */
	int m_iMapID;					/**< Map ID <br> */
	int m_iMapIDMax;				/**< Map ID Max <br> */
	float m_fPosX;					/**< Local Player Pos X <br> */
	float m_fPosY;					/**< Local Player Pos Y <br> */	
	int m_iGroupHeight;				/**< Group Height <br> */

	float m_fCamX;
	float m_fCamY;
	
	SPTexture* m_pTheme;			/**< Theme Icon Image <br> */

	SPWindowMiniMapView*	m_pView;	/**< MiniMap View Window <br> */
	SPWindowButton*			m_pMiniMapClose;
	
	//std::vector<SPTexture*> m_vpMiniMap;
	bool					m_bBuildingIn;					//	빌딩안이면 미니맵을 표시하지 않는다.
	bool					m_bMiniMapViewStatus;			//	현재 미니맵 상태.
	SPWindowStatic*			m_pMiniMapInfo;					//	미니맵 좌표 정보

	SPTexture*				m_pBGTexture;
	RECT					m_rcBGDest[ MINIMAP_BG_COUNT ];
	RECT					m_rcBGSrc[ MINIMAP_BG_COUNT ];


};
