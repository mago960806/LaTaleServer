// ***************************************************************
//  SPWindowLottery   version:  1.0   ·  date: 05/30/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowLottery_h__
#define SPWindowLottery_h__

#pragma once

#define WIID_LOTTERY_LOTTERY					( WIID_LOTTERY + 1 )
#define WIID_LOTTERY_TOP						( WIID_LOTTERY + 2 )
#define WIID_LOTTERY_LEFT						( WIID_LOTTERY + 3 )
#define WIID_LOTTERY_RIGHT						( WIID_LOTTERY + 4 )
#define WIID_LOTTERY_BOTTOM						( WIID_LOTTERY + 5 )

#define WIID_LOTTERY_CLOSE_ICON					( WIID_LOTTERY + 10 )
#define WIID_LOTTERY_REFRESH					( WIID_LOTTERY + 11 )
#define WIID_LOTTERY_BUY_TICKET					( WIID_LOTTERY + 12 )
#define WIID_LOTTERY_RANDOM_TICKET				( WIID_LOTTERY + 13 )
#define WIID_LOTTERY_BUY_ITEM					( WIID_LOTTERY + 14 )
#define WIID_LOTTERY_FILLING					( WIID_LOTTERY + 15 )
#define WIID_LOTTERY_SICE_ICON					( WIID_LOTTERY + 16 )

#define LOTTERY_GUIDE_X_MAX						20
#define LOTTERY_GUIDE_Y_MAX						20
#define LOTTERY_GUIDE_XY_MAX					( LOTTERY_GUIDE_X_MAX * LOTTERY_GUIDE_Y_MAX )


#define WIID_LOTTERY_MESSAGE_BOX_BG				( WIID_LOTTERY + 50 )

#define WIID_LOTTERY_MESSAGE_BOX_BG_L_T			( WIID_LOTTERY + 51 )
#define WIID_LOTTERY_MESSAGE_BOX_BG_C_T			( WIID_LOTTERY + 52 )
#define WIID_LOTTERY_MESSAGE_BOX_BG_R_T			( WIID_LOTTERY + 53 )

#define WIID_LOTTERY_MESSAGE_BOX_BG_L_M			( WIID_LOTTERY + 54 )
#define WIID_LOTTERY_MESSAGE_BOX_BG_C_M			( WIID_LOTTERY + 55 )
#define WIID_LOTTERY_MESSAGE_BOX_BG_R_M			( WIID_LOTTERY + 56 )

#define WIID_LOTTERY_MESSAGE_BOX_BG_L_B			( WIID_LOTTERY + 57 )
#define WIID_LOTTERY_MESSAGE_BOX_BG_C_B			( WIID_LOTTERY + 58 )
#define WIID_LOTTERY_MESSAGE_BOX_BG_R_B			( WIID_LOTTERY + 59 )

#define WIID_LOTTERY_MESSAGE_BOX_TITLE			( WIID_LOTTERY + 60 )

#define WIID_LOTTERY_MESSAGE_BOX_OK_TEXT		( WIID_LOTTERY + 61 )
#define WIID_LOTTERY_MESSAGE_BOX_OK				( WIID_LOTTERY + 62 )
#define WIID_LOTTERY_MESSAGE_BOX_YES			( WIID_LOTTERY + 63 )
#define WIID_LOTTERY_MESSAGE_BOX_NO				( WIID_LOTTERY + 64 )
#define WIID_LOTTERY_MESSAGE_BOX_TEXT			( WIID_LOTTERY + 65 )

#define WIID_LOTTERY_MESSAGE_BOX_BUY_BG_L		( WIID_LOTTERY + 66 )
#define WIID_LOTTERY_MESSAGE_BOX_BUY_BG_C		( WIID_LOTTERY + 67 )
#define WIID_LOTTERY_MESSAGE_BOX_BUY_BG_R		( WIID_LOTTERY + 68 )
#define WIID_LOTTERY_MESSAGE_BOX_BUY_BG			( WIID_LOTTERY + 69 )
#define WIID_LOTTERY_MESSAGE_BOX_BUY_HELP		( WIID_LOTTERY + 70 )

#define WIID_LOTTERY_MESSAGE_BOX_TICKET_X		( WIID_LOTTERY + 71 )
#define WIID_LOTTERY_MESSAGE_BOX_TICKET_Y		( WIID_LOTTERY + 72 )

#define WIID_LOTTERY_MESSAGE_BOX_WAIT_START		WIID_LOTTERY_MESSAGE_BOX_OK
#define WIID_LOTTERY_MESSAGE_BOX_WAIT_END		WIID_LOTTERY_MESSAGE_BOX_TICKET_Y

#define WIID_LOTTERY_MESSAGE_BOX_OK_START		WIID_LOTTERY_MESSAGE_BOX_YES
#define WIID_LOTTERY_MESSAGE_BOX_OK_END			WIID_LOTTERY_MESSAGE_BOX_TICKET_Y

#define WIID_LOTTERY_MESSAGE_BOX_YESNO_START	WIID_LOTTERY_MESSAGE_BOX_OK_TEXT
#define WIID_LOTTERY_MESSAGE_BOX_YESNO_END		WIID_LOTTERY_MESSAGE_BOX_OK

#define WIID_LOTTERY_MESSAGE_BOX_NORMAL_START	WIID_LOTTERY_MESSAGE_BOX_BG
#define WIID_LOTTERY_MESSAGE_BOX_NORMAL_END		WIID_LOTTERY_MESSAGE_BOX_TICKET_Y


#define WIID_LOTTERY_ITEM_INVEN					(WIID_LOTTERY + 80)
#define WIID_LOTTERY_ITEM_INVEN_WHITE_BG		(WIID_LOTTERY + 81)
#define WIID_LOTTERY_ITEM_INVEN_EQUIP			(WIID_LOTTERY + 82)
#define WIID_LOTTERY_ITEM_INVEN_CONSUME			(WIID_LOTTERY + 83)
#define WIID_LOTTERY_ITEM_INVEN_ETC				(WIID_LOTTERY + 84)
#define WIID_LOTTERY_ITEM_INVEN_EVENT			(WIID_LOTTERY + 85)
#define WIID_LOTTERY_ITEM_INVEN_PET				(WIID_LOTTERY + 86)
#define WIID_LOTTERY_ITEM_INVEN_HOUSING			(WIID_LOTTERY + 87)
#define WIID_LOTTERY_ITEM_INVEN_UP				(WIID_LOTTERY + 88)
#define WIID_LOTTERY_ITEM_INVEN_DOWN			(WIID_LOTTERY + 89)
#define WIID_LOTTERY_ITEM_INVEN_THUMB			(WIID_LOTTERY + 90)
#define WIID_LOTTERY_ITEM_INVEN_SLOT			(WIID_LOTTERY + 91)
#define WIID_LOTTERY_ITEM_INVEN_TITLE			(WIID_LOTTERY + 92)

#define WIID_LOTTERY_SYSTEMMSG					(WIID_LOTTERY + 100)
#define WIID_LOTTERY_SYSTEMMSG_BG				(WIID_LOTTERY + 101)
#define WIID_LOTTERY_SYSTEMMSG_UP				(WIID_LOTTERY + 102)
#define WIID_LOTTERY_SYSTEMMSG_DOWN				(WIID_LOTTERY + 103)
#define WIID_LOTTERY_SYSTEMMSG_BOTTOM			(WIID_LOTTERY + 104)
#define WIID_LOTTERY_SYSTEMMSG_LIST				(WIID_LOTTERY + 105)

#define WIID_LOTTERY_CHAT						(WIID_LOTTERY + 111)
#define WIID_LOTTERY_CHAT_UP					(WIID_LOTTERY + 112)
#define WIID_LOTTERY_CHAT_DOWN					(WIID_LOTTERY + 113)
#define WIID_LOTTERY_CHAT_BOTTOM				(WIID_LOTTERY + 114)
#define WIID_LOTTERY_CHAT_LIST					(WIID_LOTTERY + 115)

#define WIID_LOTTERY_TURN						(WIID_LOTTERY + 120)
#define WIID_LOTTERY_TURN_BACK					(WIID_LOTTERY + 121)
#define WIID_LOTTERY_TURN_FRAME					(WIID_LOTTERY + 122)
#define WIID_LOTTERY_TURN_CONTINUE				(WIID_LOTTERY + 123)
#define WIID_LOTTERY_TURN_STOP					(WIID_LOTTERY + 124)
#define WIID_LOTTERY_TURN_ACCEPT				(WIID_LOTTERY + 125)
#define WIID_LOTTERY_TURN_LIGHT					(WIID_LOTTERY + 126)	//+9
#define WIID_LOTTERY_TURN_ITEM_BG				(WIID_LOTTERY + 136)	//+9
#define WIID_LOTTERY_TURN_ITEM					(WIID_LOTTERY + 146)	//+9
#define WIID_LOTTERY_TURN_LEVEL					(WIID_LOTTERY + 156)	//+9
#define WIID_LOTTERY_TURN_NUM					(WIID_LOTTERY + 166)	//+9
#define WIID_LOTTERY_TURN_DISABLE				(WIID_LOTTERY + 176)	//+9
#define WIID_LOTTERY_TURN_TIP					(WIID_LOTTERY + 186)
#define WIID_LOTTERY_TURN_TIP_STATIC1			(WIID_LOTTERY + 187)
#define WIID_LOTTERY_TURN_TIP_STATIC2			(WIID_LOTTERY + 188)
#define WIID_LOTTERY_TURN_TIP_STATIC3			(WIID_LOTTERY + 189)
#define WIID_LOTTERY_TURN_TIP_STATIC4			(WIID_LOTTERY + 190)
#define WIID_LOTTERY_TURN_TIP_CHECK				(WIID_LOTTERY + 191)
#define WIID_LOTTERY_TURN_TIP_CLOSE				(WIID_LOTTERY + 192)


#define WIID_LOTTERY_TICKET						( WIID_LOTTERY + 500 )
#define WIID_LOTTERY_TICKET_SELL				( WIID_LOTTERY + 1000 )
#define WIID_LOTTERY_TICKET_DISABLE				( WIID_LOTTERY + 1500 )



enum LOTTERY_STATE
{
	LOTTERY_STATE_NORMAL	=	0	,
	LOTTERY_STATE_WAIT				,
	LOTTERY_STATE_OK				,
	LOTTERY_STATE_YESNO				,

	LOTTERY_STATE_MAX				,
};

////////////////////////////////////////////////////////////////
struct ANI_NODE
{
	float m_fStartTime;
	float m_fSpeedTime;
	int   m_iCurFrame;
	int   m_iLoop;

	RECT			m_rcDst;
	vector<RECT>	m_vrcSrc;

	ANI_NODE()
	{
		m_fStartTime	= 0.0f;
		m_fSpeedTime	= 0.02f;
		m_iCurFrame		= -1;
		m_iLoop			= 0;

		m_vrcSrc.clear();
	}

	void	Play(int iLoop = 1) { m_iLoop = iLoop; }
	bool	isPlay() { return (m_iLoop <= 0 ? false : true); }
	void	Stop() { m_iLoop = 0; }

	void	Process(float fTime)
	{
		if ( m_iLoop <= 0 )
		{
			m_fStartTime = 0.0f;
			m_iCurFrame = -1;
			return;
		}

		m_fStartTime += fTime;
		if ( m_fStartTime >= m_fSpeedTime )
		{
			m_fStartTime = 0.0f;
			if ( ++m_iCurFrame >= m_vrcSrc.size() )
			{
				m_iCurFrame = 0;
				--m_iLoop;
			}
		}
	}
};

enum   ANI_STATUS
{
	ANI_1,
	ANI_2,
	ANI_3,
	ANI_4,
	ANI_5,
	ANI_END,
};

class  CTicketAniManager
{
public:
	CTicketAniManager();
	~CTicketAniManager();

	bool	Init();
	void	Process(float fTime);
	void	Process1(float fTime);
	void	Process2(float fTime);
	void	Process3(float fTime);
	void	Process4(float fTime);
	void	Process5(float fTime);
	void	Render(float fTime);
	void	SetAni(ANI_STATUS  status);

private:
	float				m_fStartTime;
	SPTexture *			m_pTexture;
	vector<ANI_NODE>	m_vAni;
	ANI_STATUS			m_aniStatus;
};

///////////////////////////////////////////////////////////////
struct START_ANIMATION
{
	DWORD	m_dwAccmulateTime;
	DWORD	m_dwDelayTime;
	DWORD	m_dwStartTime;
	
	int     m_iDstX;
	int     m_iDstY;
	int		m_iPosX;
	int		m_iPosY;
	RECT	m_rcSrc;

	SPTexture * m_pTexture;

	//
	START_ANIMATION()
	{
		m_dwAccmulateTime = 2000;
		m_dwDelayTime = 1000;
		m_dwStartTime = 0;

		m_pTexture = NULL;
	}

	~START_ANIMATION()
	{
		SAFE_RELEASE(m_pTexture);
	}

	void    LoadTexture(const char* szFileName, int iX, int iY, int iSX, int iSY)
	{
		g_pVideo->GetTextureMgr()->LoadTexture( szFileName , &m_pTexture );
		m_rcSrc.left	= iX;
		m_rcSrc.top		= iY;
		m_rcSrc.right   = iSX;
		m_rcSrc.bottom  = iSY;
	}

	void	Start(int iX, int iY, int iLastX, int iLastY, DWORD	dwAccmulateTime = 2000, DWORD dwDelayTime = 1000)
	{
		m_iDstX = iLastX;
		m_iDstY = iLastY;
		m_iPosX = iX;
		m_iPosY = iY;
		m_dwAccmulateTime = dwAccmulateTime;
		m_dwDelayTime = dwDelayTime;

		m_dwStartTime = GetTickCount();
	}

	void	Render(float fTime)
	{
		if ( m_dwStartTime > 0 && m_pTexture )
		{
			DWORD  dwNewTime = GetTickCount();
			if ( (dwNewTime - m_dwStartTime) > (m_dwAccmulateTime + m_dwDelayTime) )
			{
				m_dwStartTime = 0;
			}
			else if ( (dwNewTime - m_dwStartTime) > m_dwDelayTime )
			{
				DWORD	dwTime = dwNewTime - m_dwStartTime - m_dwDelayTime;
				float	fDistanceX = (m_iDstX - m_iPosX)/(float)m_dwAccmulateTime;
				float	fDistanceY = (m_iDstY - m_iPosY)/(float)m_dwAccmulateTime;
				
				int iPosX = m_iPosX + fDistanceX * dwTime;
				int iPosY = m_iPosY + fDistanceY * dwTime;
				m_pTexture->RenderTexture(iPosX, iPosY, &m_rcSrc);
			}
		}
	}
};

/////////////////////////////////////////////////////////////////
struct   TEXT_NODE
{
	string      m_strText;
	string      m_strRenderText;
	float       m_fPosX;
	float       m_fPosY;
	int         m_iSpeed;
	RECT		m_rcText;
	bool        m_bPlay;
	DWORD       m_dwTextColor;
	bool        m_bDelete;
	int         m_iOffset;
	int         m_iOffsetLast;
	int         m_iOffsetLast2;

	RECT		m_rcSrc;
	SPTexture * m_pTexture;

	TEXT_NODE(string str, DWORD dwColor = 0xFFFFFFFF);
	~TEXT_NODE();

	void    Play();
	bool    isPlay()   { return m_bPlay; }
	bool    isDelete() { return m_bDelete; }
	void	Process(float fTime);
	void	Render(float fTime);
	int     GetLastPosX();

	void    LoadTexture(const char* szFileName, int iX, int iY, int iSX, int iSY);
	void    LoadTexture(SPTexture * pTexture, int iX, int iY, int iSX, int iSY);
	void    SetPlayInfo(int iSpeed, RECT & rcText);
};

class    CTextScrollManager
{
public:
	CTextScrollManager();
	~CTextScrollManager();

	void    AddTextNode(TEXT_NODE & text);

	void	Process(float fTime);
	void	Render(float fTime);

	void    Clear() { m_vecTextNode.clear(); }

	void    LoadTexture(const char* szFileName, int iX, int iY, int iSX, int iSY);
	void    SetPlayInfo(int iSpeed, int iDistance, RECT & rcText);
	
private:
	int         m_iSpeed;
	int         m_iDistance;
	RECT		m_rcText;

	RECT		m_rcDst[2];
	RECT		m_rcSrc;
	SPTexture * m_pTexture;

	std::vector<TEXT_NODE>   m_vecTextNode;
};

//////////////////////////////////////////////////////////////////
struct LOTTERY_LDT 
{
	int					m_iKey;
	int					m_iLevel;
	int					m_iItemID;
	string				m_strName;
	int					m_iIconFile;
	int					m_iIconIndex;
	ITEM_RARE_LIMIT		m_eRareLimit;
	int					m_iOpt_Min;
	int					m_iOpt_Max;
	int					m_iItemCount;
	int					m_iOpenIconFile;
	int					m_iOpenIconIndex;
	int					m_iFX1;
	int					m_iFX2;
	int					m_iSoundEffect;

};

typedef std::vector< t_Gamble_Status >						STD_VECTOR_LOTTERY_TICKET;
typedef std::map< int , LOTTERY_LDT >						STD_MAP_LOTTERY_LDT;

class SPWindow;
class SPUIUnitManager;
class SPFXArchive;

class SPWindowLottery : public SPWindow 
{

public:
	SPWindowLottery(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowLottery(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowLottery();

	virtual	void		Init				();
	virtual	void		Clean				();
	virtual void		Show				( bool bWithChild = true );
	virtual void		Hide				( bool bSendServer = true) ;

	virtual	void		Process				( float fTime );
	virtual	void		Render				( float fTime );

	virtual void		RefreshRelationCoord();


protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int					OnCursorEnter		( WPARAM wParam, LPARAM lParam );
	int					OnCursorOut			( WPARAM wParam, LPARAM lParam );
	int					OnCursorMove		( WPARAM wParam, LPARAM lParam );

	int					OnLotteryStatus				(WPARAM wparam, LPARAM lparam);
	int					OnLotteryUpdateItem			(WPARAM wparam, LPARAM lparam);
	int					OnLotteryBuy				(WPARAM wparam, LPARAM lparam);
	int					OnLotteryUnBuy				(WPARAM wparam, LPARAM lparam);
	int					OnLotteryAcceptItem			(WPARAM wparam, LPARAM lparam);
	int					OnLotteryAcceptItemSecond	(WPARAM wparam, LPARAM lparam);
	int					OnLotteryInitial			(WPARAM wparam, LPARAM lparam);
	int					OnLotteryBroadCast			(WPARAM wparam, LPARAM lparam);
	int					OnLotterySetName			(WPARAM wparam, LPARAM lparam);

	int					OnPurge				( WPARAM wParam, LPARAM lParam );

	int					OnMessageOk			( WPARAM wParam, LPARAM lParam );
	int					OnMessageYes		( WPARAM wParam, LPARAM lParam );
	int					OnMessageNo			( WPARAM wParam, LPARAM lParam );

	int					OnClose				( WPARAM wParam, LPARAM lParam );
	int					OnRefresh			( WPARAM wParam, LPARAM lParam );
	int					OnBuy				( WPARAM wParam, LPARAM lParam );
	int					OnRandom			( WPARAM wParam, LPARAM lParam );
	int					OnItemBuy			( WPARAM wParam, LPARAM lParam );
	int					OnFilling			( WPARAM wParam, LPARAM lParam );
	int					OnContinue			( WPARAM wParam, LPARAM lParam );
	int					OnAccept			( WPARAM wParam, LPARAM lParam );
	int					OnStopTurn			( WPARAM wParam, LPARAM lParam );

	int					OnCursorEnter		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnCursorOut			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnCursorMove		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnLButtonUP			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnLButtonDblClick	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	// }}

	//////////////////////////////////////////////////////////////////////////

	void				InitWindowLottery();
	void				InitWindowSystemMsg();
	void				InitWindowItemInven();
	void				InitWindowChat();
	void				InitWindowMessageBox();
	void				InitWindowButtons	();
	void				InitWindowTickets	();
	void				InitWindowTurn		();
	void				InitWindowEtc		();

	void				InitLDTLoad			();

	void				LoadGambleTipSetting();
	void				SaveGambleTipSetting( bool bShowTip = true );

	bool				CheckRequire		( bool bCheckTicket = true );

	void				PrintErrorMsg		( GLOBAL_STRING_ID uiGlobalStringID );

	void				InitMessageBox		();
	void				ProcessMessageBox	( float fTime );
	void				HideMessageBox		();
	void				SetMessageBox		( LOTTERY_STATE eState , const char* pstrMessage );
	void				SetMessageBoxPos	( int iPosX , int iPosY );
	void				UpdateMessageBox	();

	void                ShowWindowTurn		();
	void                HideWindowTurn		();
	void                StartTurn			();
	void                StopTurn			();
	void                ProcessWindowTurn	( float fTime );

	void				ShowTurnTip			();
	void				HideTurnTip			();
	

	void                PlayLotteryFx       (int iSetID);

	void				SetSelectTicket		( unsigned int iID );
	void				SetSelectTicket		( int iXPos , int iYPos );
	void				ClearSelectTicket	();

	void				SetTicketData		( UINT16 uiTicketCount , t_Gamble_Status  status[] );
	void				UpdateTicket		();
	void				UpdateTicket		( int iIndex );

	int					AcceptItem			( int iIndex, bool bAccept = true );

	void				AddPrizeLDT			( LOTTERY_LDT& stPrize );
	LOTTERY_LDT*		GetPrizeLDT			( int iSetID );

	void				AddChatMessage		( string strMsg, ITEMID iSetID = 0 );
	void				AddSystemMessage	( string strMsg, ITEMID iSetID = 0 );
	void				AddScrollMessage	( string strMsg, ITEMID iSetID = 0 );

	SPItem*				GetItemInvenItem    (int iSlotIndex);
	void				ClearSelectItem		();

	void				EmptyItemInvenSlot	();
	void				RefreshItemInven	();

	void				SetItemInvenCategory(int iType);
	int					SetSliderMaxPage(int iType, int iPage);
	int					SetSliderPage(int iType, int iPage);

	void				SetTicketName(int  iIndex, const char * szName);
	const char *        GetTicketName(int  iIndex);

	void				ReposOwnImage();

	virtual int			OnItemInvenUp			(WPARAM wParam, LPARAM lParam);
	virtual int			OnItemInvenDown			(WPARAM wParam, LPARAM lParam);
	virtual int			OnSetItemInvenUp		(WPARAM wParam, LPARAM lParam);
	virtual int			OnSetItemInvenDown		(WPARAM wParam, LPARAM lParam);
	virtual int			OnSetItemInvenCurPage	(WPARAM wParam, LPARAM lParam);
	virtual int			OnItemInvenToolTipEnter	(WPARAM wParam, LPARAM lParam);
	virtual int			OnItemInvenToolTipOut	(WPARAM wParam, LPARAM lParam);
	virtual int			OnItemInvenToolTipMove	(WPARAM wParam, LPARAM lParam);
	virtual int			OnItemInvenLButtonDown	(WPARAM wParam, LPARAM lParam);
	virtual int			OnItemInvenLButtonUp	(WPARAM wParam, LPARAM lParam);
	virtual int			OnItemInvenCursorMove	(WPARAM wParam, LPARAM lParam);
	virtual int			OnClearDrag				(WPARAM wParam, LPARAM lParam);

	virtual int			OnChatScrollUp				( WPARAM wParam, LPARAM lParam );
	virtual int			OnChatScrollDown			( WPARAM wParam, LPARAM lParam );
	virtual int			OnChatScrollLatest			( WPARAM wParam, LPARAM lParam );
	virtual int			OnSystemMsgScrollUp			( WPARAM wParam, LPARAM lParam );
	virtual int			OnSystemMsgScrollDown		( WPARAM wParam, LPARAM lParam );
	virtual int			OnSystemMsgScrollLatest		( WPARAM wParam, LPARAM lParam );

	virtual int			OnChatScrollUpLBDown		( WPARAM wParam, LPARAM lParam );
	virtual int			OnChatScrollDownLBDown		( WPARAM wParam, LPARAM lParam );
	virtual int			OnChatScrollUpCursorOut		( WPARAM wParam, LPARAM lParam );
	virtual int			OnChatScrollDownCursorOut	( WPARAM wParam, LPARAM lParam );
	virtual int			OnSystemMsgScrollUpLBDown	( WPARAM wParam, LPARAM lParam );
	virtual int			OnSystemMsgScrollDownLBDown	( WPARAM wParam, LPARAM lParam );
	virtual int			OnSystemMsgScrollUpCursorOut( WPARAM wParam, LPARAM lParam );
	virtual int			OnSystemMsgScrollDownCursorOut( WPARAM wParam, LPARAM lParam );

	virtual int			OnChatWheelUp				( WPARAM wParam, LPARAM lParam );
	virtual int			OnChatWheelDown				( WPARAM wParam, LPARAM lParam );
	virtual int			OnSystemMsgWheelUp			( WPARAM wParam, LPARAM lParam );
	virtual int			OnSystemMsgWheelDown		( WPARAM wParam, LPARAM lParam );

	virtual int			OnLoadGambleTip				( WPARAM wParam, LPARAM lParam );
	virtual int			OnSaveGambleTip				( WPARAM wParam, LPARAM lParam );

	virtual int			OnTurnTipCheck				( WPARAM wParam, LPARAM lParam );
	virtual int			OnTurnTipClose				( WPARAM wParam, LPARAM lParam );

	virtual int			OnTicketToolTipEnter		(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int			OnTicketToolTipOut			(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int			OnTicketToolTipMove			(unsigned int iID, WPARAM wParam, LPARAM lParam);

	virtual int			OnTurnToolTipEnter			(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int			OnTurnToolTipOut			(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int			OnTurnToolTipMove			(unsigned int iID, WPARAM wParam, LPARAM lParam);

	int					OnReflashItemInven		(WPARAM wParam, LPARAM lParam);
	int					OnInvenCategory			(unsigned int iID, WPARAM wParam, LPARAM lParam);


	void				BtnPush					( int iBtn, int iBtnDown );

	int					GetLevelIndex			(int iLevel);
	DWORD				GetLevelColor			(int iLevelIndex);
	bool				GetTicketSellIcon		( string strFileName, int iIndex, SPTexture * &pTexture, POINT& ptSrcPos );
	bool				GetLevelIcon			( string strFileName, int iIndex, SPTexture * &pTexture,	POINT& ptSrcPos );

	//////////////////////////////////////////////////////////////////////////
	//
	//	函荐甸
	//

	bool									m_bUse;

	SPTexture*								m_pBGTexture;
	SPTexture*								m_pTurnTexture;

	LOTTERY_STATE							m_eState;

	int										m_iSelectTicketX;
	int										m_iSelectTicketY;

	STD_VECTOR_LOTTERY_TICKET				m_vTicketBuffer;
	STD_VECTOR_LOTTERY_TICKET				m_vRandomBuffer;
	vector<std::string>                     m_vecStrName;

	SPWindow*								m_pTicket[ LOTTERY_GUIDE_XY_MAX ];
	SPWindow*								m_pTicketSell[ LOTTERY_GUIDE_XY_MAX ];
	SPWindow*								m_pTicketDisable[ LOTTERY_GUIDE_XY_MAX ];
	SPWindow*								m_pMessageBox;

	STD_MAP_LOTTERY_LDT						m_mPrize;

	int										m_iRequireItem;
	UINT64									m_uiRequireEly;

	//////////////////////////////////////////////////////////////////////////

	UINT64									m_uiVersion;
	unsigned short							m_usServerPoit;		//	是否是服务器节点购买
	unsigned short							m_usMyPoit;			//	是否是个人节点购买

	//////////////////////////////////////////////////////////////////////////

	float									m_fMessageBoxLimit;
	float									m_fMessageBoxAccmulateTime;
	bool									m_bMessageBoxShow;

	//////////////////////////////////////////////////////////////////////////

	SPFXArchive*							m_pFXArchive;

	//////////////////////////////////////////////////////////////////////////

	int										m_iMessageBoxTicketYX;
	int										m_iMessageBoxTicketYY;
	int										m_iMessageBoxTicketXX;
	int										m_iMessageBoxTicketXY;
	int										m_iMessageBoxBuyHelpX;
	int										m_iMessageBoxBuyHelpY;
	int										m_iMessageBoxBuyBGX;
	int										m_iMessageBoxBuyBGY;


	int m_iItemInvenCategory;
	int m_iItemInvenMaxPage[6];
	int m_iItemInvenCurPage[6];

	int m_iSelectSlot;
	SPItem* m_pkSelectItem;


	int			m_iChatBtnDown;
	int			m_iSystemMsgBtnDown;
	float		m_fDownLimitTime;
	float		m_fDownAccmulateTime;
				
	float       m_fEnterToolTipTime;					// 提示计时
	float       m_fEnterStopTime;						// 停留时间


	START_ANIMATION				m_aniStart[2];

	vector<t_Gamble_Getted_Item>	m_vecTurnItem;			//	摆设物品	
	t_Gamble_Getted_Item			m_itemIDFirstGetted;	//	获得的物品
	t_Gamble_Getted_Item			m_itemIDSecondGetted;	//	获得的物品
	UINT64							m_itemVersion;			//  物品版本
	bool							m_bAcceptFirst;         //  是否接收第一次物品


	bool						m_bStartTurn;			//  是否启动
	bool						m_bStopTurn;			//  是否停止
	bool						m_bTurnStoping;			//  是否按下停止
	int							m_iTurnRandom;          //  中奖位置
	int							m_iCurItemIndex;		//  当前位置
	float						m_fChangeSpeed;			//  速度变量(第格几秒)
	float						m_fTurnDstTime;			//  间隔时间
	float						m_fStartDstTime;		//  启动计时


	RECT						m_rcBGSrc[6];
	std::vector<RECT>			m_rcBGDst[6];           // 背景
	
	RECT						m_rcTurnSrc;
	RECT						m_rcTurnDst;            // 转盘
	float						m_fTurnRotation;		// 角度

	CTicketAniManager			m_ticketAniManager;
	CTextScrollManager          m_textScrollManager;

	bool						m_bShowGambleTip;

};

#endif // SPWindowLottery_h__