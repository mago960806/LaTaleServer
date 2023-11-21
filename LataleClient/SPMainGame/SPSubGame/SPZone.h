
// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-17 오후 2:56:36
//***************************************************************************

#pragma once

const int MAX_FADE_ALPHA = 255;					/**< <br> */
const int PVP_LOADWAIT_SOUND = 885 ;
const int PVP_MATCHOK_SOUND = 883 ;

const  int  CASHSHOP_OFFSETCAMX = 280;
const  int  CASHSHOP_OFFSETCAMY = 300;

class SPZone : public SPSubGameManager {
public:
	SPZone();
	virtual ~SPZone();

	virtual void			Render(float fTime);
	virtual void			Process(float fTime);
	virtual bool			Init();
	virtual void			Clean();

	virtual bool			WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual int				SPPerformMessage	(UINT msg, WPARAM wparam = 0, LPARAM lParam = 0);

	virtual void			RenderText();

protected:

	SPGMESSAGE_MAP_DECLARE()
	int		OnGOBEnter					(WPARAM wparam, LPARAM lparam);
	int		OnGOBEnterID				(WPARAM wparam, LPARAM lparam);
	int		OnGOBEnterEx				(WPARAM wparam, LPARAM lparam);
	int		OnGOBLeave					(WPARAM wparam, LPARAM lparam);
	int		OnGOBMove					(WPARAM wparam, LPARAM lparam);
	int		OnMessageString				(WPARAM wparam, LPARAM lparam);

	int		OnPlayerEquipDefaultChange	(WPARAM wparam, LPARAM lparam);
	int		OnPlayerEquipChange			(WPARAM wparam, LPARAM lparam);

	int		OnExitZone					(WPARAM wparam, LPARAM lparam);

	int		OnExitStageChange			(WPARAM wparam, LPARAM lparam);	//[2005/5/30] - 스테이지가 변하는 경우

	int		OnVersionChecked			(WPARAM wparam, LPARAM lparam);	
	int		OnLeaveWorld				(WPARAM wparam, LPARAM lparam);
	int		OnChannelChange				(WPARAM wparam, LPARAM lparam);
	int		OnChannelChangeWait			(WPARAM wparam, LPARAM lparam);
	int		OnChannelChangeReady		(WPARAM wparam, LPARAM lparam);
	int		OnGetChannelChangeType		(WPARAM wparam, LPARAM lparam);

	int		OnCashShopInRequest			(WPARAM wparam, LPARAM lparam);
	int		OnCashShopInReceive			(WPARAM wparam, LPARAM lparam);
	int		OnCashShopInfo				(WPARAM wparam, LPARAM lparam);
	int		OnCashShopUpdate			(WPARAM wparam, LPARAM lparam);
	int		OnCashShopBuy				(WPARAM wparam, LPARAM lparam);
	int		OnCashShopGift				(WPARAM wparam, LPARAM lparam);
	int		OnCashShopExact 			(WPARAM wparam, LPARAM lparam);
	int		OnCashShopSell				(WPARAM wparam, LPARAM lparam);
	int		OnCashShopCoupon			(WPARAM wparam, LPARAM lparam);
	int		OnCashShopCartList			(WPARAM wparam, LPARAM lparam);
	int		OnCashShopCartAdd			(WPARAM wparam, LPARAM lparam);
	int		OnCashShopCartDel			(WPARAM wparam, LPARAM lparam);
	int		OnCashShopItemInit			(WPARAM wparam, LPARAM lparam);
	int		OnCashShopItemUpdate		(WPARAM wparam, LPARAM lparam);
	int		OnCashShopOutReady			(WPARAM wparam, LPARAM lparam);
	int		OnQuickBuyInfoAck			(WPARAM wparam, LPARAM lparam);

	//[xialin 2008/10/13]add
	int		OnLotteryInRequest			(WPARAM wparam, LPARAM lparam);
	int		OnLotteryInReceive			(WPARAM wparam, LPARAM lparam);
	int		OnLotteryStatus				(WPARAM wparam, LPARAM lparam);
	int		OnLotteryUpdate				(WPARAM wparam, LPARAM lparam);
	int		OnLotteryBuy				(WPARAM wparam, LPARAM lparam);
	int		OnLotteryUnBuy				(WPARAM wparam, LPARAM lparam);
	int		OnLotteryAcceptItem			(WPARAM wparam, LPARAM lparam);
	int		OnLotteryAcceptItemSecond	(WPARAM wparam, LPARAM lparam);
	int		OnLotteryInitial			(WPARAM wparam, LPARAM lparam);


	int		OnCashShopParcel(WPARAM wParam, LPARAM lParam);
	int		OnCashShopBeExact(WPARAM wParam, LPARAM lParam);

	int		OnSetStoryView				(WPARAM wparam, LPARAM lparam);
	int		OnSetTextView				(WPARAM wparam, LPARAM lparam);

	int		OnSetPVPIndunIn				(WPARAM wparam, LPARAM lparam);
	int		OnShowPVPLoadWait			(WPARAM wparam, LPARAM lparam);

	int		OnGuildIndunEnter			(WPARAM wparam, LPARAM lparam);
	int		IndunInRequest				(WPARAM wparam, LPARAM lparam);
	int		OnIndunIn					(WPARAM wparam, LPARAM lparam);
	int		OnIndunOut					(WPARAM wparam, LPARAM lparam);
	int		OnIndunForceExit			(WPARAM wparam, LPARAM lparam);
	int		OnIndunTeamExit				(WPARAM wparam, LPARAM lparam);
	int		OnIndunReset				(WPARAM wparam, LPARAM lparam);

	int		OnCharacterSelect			(WPARAM wParam, LPARAM lParam);
	int		OnWorldEnter				(WPARAM wParam, LPARAM lParam);

	int		OnUserIn					(WPARAM wParam, LPARAM lParam);
	int		OnUserMove					(WPARAM wParam, LPARAM lParam);

	int		OnSocketFailed				(WPARAM wparam, LPARAM lparam);	//[2006/2/22] - CutIn시 연결 종료 처리

protected:
	void	ProcessFade(float fTime);
	void	ProcessNewFade(float fTime) ;
	void	SetFadeState(GAME_STATE eCurState, GAME_STATE eNextState = GS_NULL) ;

	void	DirectExit();
	void	IndunOutRequest();

protected:

	bool					m_bActive;
	bool					m_bChannelWait;
	bool					m_bSendDirectExit ;			// Alt+F4에서 한번만 호출되도록 Flag
	float					m_fChannelWaitTime;
	bool					m_bChannelChangeRetry;
	int						m_iChannelChangeRetryCount;

	// 1 : change channel, 2 : change indun
	int						m_iChannelChangeType;
	
	int						m_iAlpha;				//[2005/10/17]
	SPTexture*				m_pTexture;
	SPTexture*				m_pPreloadSymbol;
	SPTexture*				m_pPreloadBG;
	RECT					m_rtSrcFade;
	RECT					m_rtDestFade;

	GAME_STATE				m_iNextState;
	float					m_fLastRenderDelay;	//[2005/9/13]/**< 각 LoadStatus 의 마지막 시간을 기억 <br> */
	float					m_fAccumulateRender;//[2005/9/9] /**< 렌더링 시간 계산 <br> */
	float					m_fCurDelay;		//[2005/9/9] /**< 프레임 간격 시간 <br> */	

	SERVER_ID				m_iChangeServerID;
	bool					m_bCashShopInReq;
	bool					m_bSendIndunInReq ;
	bool					m_bLotteryInReq;      //[xialin 2008/10/13]add
};






