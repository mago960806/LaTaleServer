// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD EMBLEM WINDOW
// Comment     : 
// Creation    : DURAGON 2007-8-22    11:18 
//***************************************************************************

#pragma once

//------------------------------------------------------------------------------------
// Property Select
#define WIID_EMBLEMGUILD_MODAL				(WIID_GUILD_EMBLEM + 1)
#define WIID_EMBLEMGUILD_BG					(WIID_GUILD_EMBLEM + 2)
#define WIID_EMBLEMGUILD_CLOSE				(WIID_GUILD_EMBLEM + 3)

#define WIID_EMBLEMGUILD_BUYCONFIRM			(WIID_GUILD_EMBLEM + 300)

#define WIID_GUILDEMBLEM_SYM_PAGE			(WIID_GUILD_EMBLEM + 11)
#define WIID_GUILDEMBLEM_SYM_LEFT_TAG		(WIID_GUILD_EMBLEM + 12)
#define WIID_GUILDEMBLEM_SYM_RIGHT_TAG		(WIID_GUILD_EMBLEM + 13)
#define WIID_GUILDEMBLEM_SYM_LEFT			(WIID_GUILD_EMBLEM + 14)
#define WIID_GUILDEMBLEM_SYM_RIGHT			(WIID_GUILD_EMBLEM + 15)
#define WIID_GUILDEMBLEM_SYM_SHOW			(WIID_GUILD_EMBLEM + 16)
#define WIID_GUILDEMBLEM_SYM_COMMIT			(WIID_GUILD_EMBLEM + 17)

#define WIID_GUILDEMBLEM_BG_PAGE			(WIID_GUILD_EMBLEM + 21)
#define WIID_GUILDEMBLEM_BG_LEFT_TAG		(WIID_GUILD_EMBLEM + 22)
#define WIID_GUILDEMBLEM_BG_RIGHT_TAG		(WIID_GUILD_EMBLEM + 23)
#define WIID_GUILDEMBLEM_BG_LEFT			(WIID_GUILD_EMBLEM + 24)
#define WIID_GUILDEMBLEM_BG_RIGHT			(WIID_GUILD_EMBLEM + 25)
#define WIID_GUILDEMBLEM_BG_SHOW			(WIID_GUILD_EMBLEM + 26)
#define WIID_GUILDEMBLEM_BG_COMMIT			(WIID_GUILD_EMBLEM + 27)

#define WIID_GUILDEMBLEM_FX_PAGE			(WIID_GUILD_EMBLEM + 41)
#define WIID_GUILDEMBLEM_FX_LEFT_TAG		(WIID_GUILD_EMBLEM + 42)
#define WIID_GUILDEMBLEM_FX_RIGHT_TAG		(WIID_GUILD_EMBLEM + 43)
#define WIID_GUILDEMBLEM_FX_LEFT			(WIID_GUILD_EMBLEM + 44)
#define WIID_GUILDEMBLEM_FX_RIGHT			(WIID_GUILD_EMBLEM + 45)
#define WIID_GUILDEMBLEM_FX_SHOW			(WIID_GUILD_EMBLEM + 46)
#define WIID_GUILDEMBLEM_FX_COMMIT			(WIID_GUILD_EMBLEM + 47)

#define WIID_GUILDEMBLEM_SHOW_BG			(WIID_GUILD_EMBLEM + 61)
#define WIID_GUILDEMBLEM_SHOW_DARK1			(WIID_GUILD_EMBLEM + 62)
#define WIID_GUILDEMBLEM_SHOW_DARK2			(WIID_GUILD_EMBLEM + 63)
#define WIID_GUILDEMBLEM_SHOW_ARROW			(WIID_GUILD_EMBLEM + 64)
#define WIID_GUILDEMBLEM_SHOW_TEXT1			(WIID_GUILD_EMBLEM + 65)
#define WIID_GUILDEMBLEM_SHOW_TEXT2			(WIID_GUILD_EMBLEM + 66)
#define WIID_GUILDEMBLEM_BEFORE_SYM			(WIID_GUILD_EMBLEM + 67)
#define WIID_GUILDEMBLEM_BEFORE_BG			(WIID_GUILD_EMBLEM + 68)
#define WIID_GUILDEMBLEM_BEFORE_FX			(WIID_GUILD_EMBLEM + 69)
#define WIID_GUILDEMBLEM_NEXT_SYM			(WIID_GUILD_EMBLEM + 70)
#define WIID_GUILDEMBLEM_NEXT_BG			(WIID_GUILD_EMBLEM + 71)
#define WIID_GUILDEMBLEM_NEXT_FX			(WIID_GUILD_EMBLEM + 72)

#define WIID_GUILDEMBLEM_SYM_ICON_LIST		(WIID_GUILD_EMBLEM + 101)		// 12개
#define WIID_GUILDEMBLEM_BG_ICON_LIST		(WIID_GUILD_EMBLEM + 131)		// 12개
#define WIID_GUILDEMBLEM_FX_ICON_LIST		(WIID_GUILD_EMBLEM + 161)		// 12개
#define WIID_GUILDEMBLEM__LIST_END			(WIID_GUILD_EMBLEM + 180)		// 12개


#define WIID_GUILD_EMBLEM_TOOLTIP			(WIID_GUILD_EMBLEM + 201)
#define WIID_GUILD_EMBLEM_TOOLTIP_ARROW		(WIID_GUILD_EMBLEM_TOOLTIP + 1)
#define WIID_GUILD_EMBLEM_TOOLTIP_BG		(WIID_GUILD_EMBLEM_TOOLTIP + 2)
#define WIID_GUILD_EMBLEM_TOOLTIP_LIST		(WIID_GUILD_EMBLEM_TOOLTIP + 3)

#define WIID_EMBLEMGUILD_END				(WIID_GUILD_EMBLEM + 210)

//------------------------------------------------------------------------------------

#define MAX_SYMBOL_EMBLEM_COUNT		100
#define MAX_SYMBOL_BG_COUNT			100
#define MAX_SYMBOL_FX_COUNT			100
#define MAX_ONEPAGE_SHOW			12
#define MAX_EMBLEM_PAGE				9			// 100 / 12 = 8.4
#define MAX_ICON_ANI_FRAME			4
#define MAX_ICON_HORIZEN			21			// 1줄 21개 아이콘 (512 / 24)
#define EMBLEM_ICON_SIZE			24			// Icon Size 24 * 24

#define GUILD_EMBLEM_BG_IMG			23



class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPWindowEdit;
class SPFXArchive;
enum EMBLEM_LAYER;
class SPWindowGuildEmblem : public SPWindow 
{
public:
	SPWindowGuildEmblem(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowGuildEmblem(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowGuildEmblem();

	virtual	void	Init				();
	virtual	void	Clean				();
	virtual void	Show				(bool bWithChild = true);
	virtual void	Hide				(bool bSendServer = true);

	virtual	void	Process				( float fTime );
	virtual	void	Render				( float fTime );

	virtual void	RefreshRelationCoord();

private:
	void			Init_Window() ;
	void			Init_ListWindow() ;
	void			InitTooltipWindow() ;
	void			InitModalWindow() ;

	void			ReposOwnImage() ;
	void			RenderBackGround(float fTime);
	void			RenderAnimation(float fTime);

	void			SetShowUserEmblem(int iSymIndex, int iBGIndex, int iFXIndex) ;
	void			SetBeforeEmblem(EMBLEM_LAYER eLayer, int iIndex);
	bool			GetEmblemLayerPage(EMBLEM_LAYER eLayer, int iNextPage) ;
	bool			GetSymbolPage(int iGetStart, int iGetEnd) ;
	bool			GetBGPage(int iGetStart, int iGetEnd) ;
	bool			GetFXPage(int iGetStart, int iGetEnd) ;
	int				GetIconSrcX(int iIndex) ;
	int				GetIconSrcY(int iIndex) ;
	int				IndexToPage(int iIndex)		{	return iIndex ? (iIndex/MAX_ONEPAGE_SHOW) : 0 ;	}
	void			SetMouseAnimation(int iID) ;
	void			SetBeforeEmblemFx(int iFxID) ;
	void			SetAfterEmblemFx(int iFxID) ;
	void			ShowToolTip(EMBLEM_LAYER eLayer, int iMouseID, bool bShow) ;
	void			ClearEmblemFX() ;
	
	int				CheckGuildEmblemChange(EMBLEM_LAYER eLayer, int& iRetValue );
	void			SetErrorNoticeDlg(int iErrorNo, int iNeedItemID = 0 ) ;
protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int				OnClose				( WPARAM wParam, LPARAM lParam );	// 닫기

	int				OnSymbolSelect		( unsigned int iID, WPARAM wParam , LPARAM lParam );	// 심볼Select
	int				OnSymbolLeft		( WPARAM wParam, LPARAM lParam );						// 심볼 Left
	int				OnSymbolRight		( WPARAM wParam, LPARAM lParam );						// 심볼 Right
	int				OnSymbolCommit		( WPARAM wParam, LPARAM lParam );						// 심볼 구입

	int				OnBGSelect		( unsigned int iID, WPARAM wParam , LPARAM lParam );	// BGSelect
	int				OnBGLeft		( WPARAM wParam, LPARAM lParam );						// BG Left
	int				OnBGRight		( WPARAM wParam, LPARAM lParam );						// BG Right
	int				OnBGCommit		( WPARAM wParam, LPARAM lParam );						// BG 구입

	int				OnFXSelect		( unsigned int iID, WPARAM wParam , LPARAM lParam );	// FX Select
	int				OnFXLeft		( WPARAM wParam, LPARAM lParam );						// FX Left
	int				OnFXRight		( WPARAM wParam, LPARAM lParam );						// FX Right
	int				OnFXCommit		( WPARAM wParam, LPARAM lParam );						// FX 구입

	int				OnBuyYes		( WPARAM wParam, LPARAM lParam );						// 구입 확인
	int				OnBuyNo			( WPARAM wParam, LPARAM lParam );						// 구입 취소

	int				OnSymCursorEnter	( unsigned int iID, WPARAM wParam , LPARAM lParam );	// Mouse Enter
	int				OnSymCursorOut		( unsigned int iID, WPARAM wParam , LPARAM lParam );	//
	int				OnBGCursorEnter		( unsigned int iID, WPARAM wParam , LPARAM lParam );	//
	int				OnBGCursorOut		( unsigned int iID, WPARAM wParam , LPARAM lParam );	//
	int				OnFXCursorEnter		( unsigned int iID, WPARAM wParam , LPARAM lParam );	//
	int				OnFXCursorOut		( unsigned int iID, WPARAM wParam , LPARAM lParam );	//

	int				OnChangeResult		( WPARAM wParam, LPARAM lParam );	// 변경 결과
	int				OnListResize		( WPARAM wParam , LPARAM lParam  );
	int				OnTest				( unsigned int iID, WPARAM wParam , LPARAM lParam );
	// }}

protected:
	struct SELECT_ANIMATION
	{
		bool bEnable ;
		float fAccmulateTime;
		float fDelayTime;
		int iTotalFrame;
		int iCurFrame;
		int	iDir ;

		RECT rcDst;
		RECT rcSrc[MAX_ICON_ANI_FRAME];

		void Clear()
		{
			bEnable = false ;
			fAccmulateTime	= 0;
			fDelayTime	= 0.1f;
			iTotalFrame	= MAX_ICON_ANI_FRAME ;
			iCurFrame	= 0;
			iDir		= 1 ;
			SetRect(&rcDst, 0, 0, 0, 0);
			for( int i = 0; i < MAX_ICON_ANI_FRAME; i++)
				SetRect(&rcSrc[i], 0, 0, 0, 0) ;
		}
		void Process(float fTime)
		{
			fAccmulateTime += fTime ;
			if( fAccmulateTime > fDelayTime )
			{
				iCurFrame += iDir ;
				if( iCurFrame >= iTotalFrame-1 )
					iDir = -1 ;
				if( iCurFrame <= 0 )
					iDir = 1 ;
				fAccmulateTime = 0 ;
			}
		}
	};

protected:
	//////////////////////////////////////////////////////////////////////////
	//	변수들
	int				m_iShowPage[LAYER_GUILD_MAX] ;			// 보고 있는 페이지
	int				m_iSelPage[LAYER_GUILD_MAX] ;			// Select Page
	int				m_iSelIndex[LAYER_GUILD_MAX] ;			// Select Index

	int				m_iMyEmblem[LAYER_GUILD_MAX] ;			// My Emblem Index
	int				m_iPreBeforeFxID ;
	int				m_iPreSelectFxID ;

	SPTexture*		m_pBGTexture5;
	SPTexture*		m_pBGTexture7;
	SPFXArchive*	m_pkFXArchive ;		///<FX
	RECT			m_rcBGDest[ GUILD_EMBLEM_BG_IMG ];
	RECT			m_rcBGSrc[ GUILD_EMBLEM_BG_IMG ];
	RECT			m_rcIconBGSrc ;
	RECT			m_rcIconBGDst[6] ;
	EMBLEM_INFO*	m_pBuyEmblem ;						// 구입선택한 Emblem

	SPWindow*		m_pWndBG ;
	SPWindow*		m_pWndModal ;
	SPWindow*		m_pStaticSym[MAX_ONEPAGE_SHOW] ;	// Symbol List Static Control
	SPWindow*		m_pStaticBG[MAX_ONEPAGE_SHOW] ;		// BG List Static Control
	SPWindow*		m_pStaticFX[MAX_ONEPAGE_SHOW] ;		// FX List Static Control
	SPWindow*		m_pWindowToolTip ;					// ToolTip Window
	SPWindow*		m_pWindowTipNeed ;					// ToolTip용 NeedList
	int				m_iListHeight ;

	SELECT_ANIMATION	m_selAni[LAYER_GUILD_MAX] ;		// Select Animation
	SELECT_ANIMATION	m_mouseAni ;					// Mouse Over Animation
};