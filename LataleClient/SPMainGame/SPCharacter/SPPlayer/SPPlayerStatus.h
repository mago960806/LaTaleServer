// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPPlayerStatus
// Comment     : 
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************

#pragma once


class SPGameObject;
class SPGOBStatus;
class SPPlayer;

class SPPlayerStatus : public SPGOBStatus {
public:
	SPPlayerStatus(SPGameObject* pParent);
	virtual ~SPPlayerStatus();
	virtual void	Init();	
	virtual bool	SetCharInfo(CHARACTER_INFO2* pCharInfo);
	virtual bool	SetStatusInfo(STATUS_INFO* StatusInfo);
	virtual CHAR_EXP* GetCHAR_EXP();

	virtual UINT	GetStatusValue(STATUS_TYPE StatusType);
	virtual bool	SetStatusValue(STATUS_TYPE StatusType, UINT uiValue);

	virtual UINT64	GetStatusValueEX(STATUS_EX_TYPE	 StatusType);
	virtual bool	SetStatusValueEX(STATUS_EX_TYPE	 StatusType, UINT64 uiValue);

	void			SetTitleID( UINT32 uiTitleID );
	UINT32			GetTitleID();

	int				GetGameOption();
	bool			IsTrade();
	bool			IsTeam();
	bool			IsAlert();
	bool			IsWantTeamHide();	
	
	bool			SetWantParty(bool bPvp = false);

	void			SetItemMix( ITEMMIX_INFO& stItemMixInfo , bool bShowChatMessage );
	ITEMMIX_INFO*	GetItemMix();

	void			SetLovePoint( int iLovePoint , bool bShowMsg );

protected:
	SPGMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnSetStatusInfo ( LPARAM lParam );
	virtual int OnSetCharInfo	( LPARAM lParam );
	virtual int OnStatLevelUp	( LPARAM lParam );
	virtual int OnStorageInMoney	( LPARAM lParam );		//[8/3/2005]
	virtual int OnStorageOutMoney	( LPARAM lParam );		//[8/3/2005]
	virtual int OnSetEffectStatusInfo(LPARAM lParam);
	virtual int OnSetOptions (LPARAM lParam);
	virtual int OnSetCashInfo(LPARAM lParam);


	void		UpdateOption();	//  [4/24/2008 AJJIYA]

protected:

	friend				SPPlayer;

	UINT32				m_uiTitleID;	
	UINT32				m_uiFameBase;
	UINT32				m_uiFame;
	CHAR_EXP			m_CharExp;

	UINT8				m_cCharType;
	GENDER				m_Gender;
	UINT64				m_uiMoney;
	UINT64				m_uiStorageMoney;			//[2005/7/20]
	int					m_iSkillPoint;
	int					m_uiArmor ;			// 방어력
	int					m_uiRegistEarth ;	// 저항력
	int					m_uiRegistWind ;	// 저항력
	int					m_uiRegistWater ;	// 저항력
	int					m_uiRegistFire ;	// 저항력
	int					m_uiRequireLv ;		// 제한레벨스텟
	int					m_uiProvEnchant ;	// 인첸트 성공확률보정.
	INT32				m_iGameOptions;		// Server Side Game Option

	ITEMMIX_INFO		m_stItemMix;
	PVP_STATUS			m_stPvpStat;		//[2007/11/27] Pvp Status Info

	int					m_iCash;			//[2008/3/12] 플레이어 보유 캐시
	int					m_iPoint;			//[2008/3/12] 플레이어 보유 포인트

	//-----------------------------------------------
	//{
	int					m_iLovePoint;		// 연애 포인트 현재값 [10/2/2008 AJJIYA]
	int					m_iLovePointMax;	// 연애 포인트 최대값 [10/2/2008 AJJIYA]
	int					m_iLoveDay;			// 연애 교제 일 수 [10/2/2008 AJJIYA]
	int					m_iLoveState;		// 연애 상태 ( 0 : None , 1 : 교제상태 , 2 : 프로포즈 완료 상태 , 4 : 결혼상태 ) [10/2/2008 AJJIYA]
	int					m_iLoveLevel;		// 연애 레벨 [10/27/2008 AJJIYA]

	int					m_iExpandCharSlot;		// 캐릭터 확장 슬롯 [10/20/2008 DURAGON]
	int					m_iSummonMobCount;		// 몬스터 소환 개체수[10/20/2008 DURAGON]
	
	int					m_iRankingLevel;
	int					m_iRankingPvp;
	UINT				m_iRankingLove;
	int					m_iRankingGuild;
	//-----------------------------------------------
	//}
};
