#ifndef __SPPLAYER_STATUS_MODEL__
#define __SPPLAYER_STATUS_MODEL__

class SPPlayer;

struct SPTitleData;
class SPTitleRenderBase;
class SPFXArchive ;
class SPPlayerStatusModel
{
	enum TALKBALLON_TYPE
	{
		TALKBALLON_TOP		,
		TALKBALLON_MIDDLE	,
		TALKBALLON_BOTTOM	,
		TALKBALLON_COUNT	,
	};

	enum LEVEL_TYPE
	{
		LEVEL_1								,
		LEVEL_2								,
		LEVEL_3								,
		LEVEL_COUNT							,
		LEVEL_MAX			=	999			,
		LEVEL_X_START		=	0			,
		LEVEL_Y_START		=	53			,
		LEVEL_X_STEP		=	8			,
		LEVEL_Y_STEP		=	9			,
	};

public:
	SPPlayerStatusModel(SPPlayer* pkParent);
	~SPPlayerStatusModel();

	void Render(float fTime);
	void Process(float fTime);

	void Init();

	void SetObjectRgn(RECT& rcRgn, int iCenterPosX, int iCenterPosY, bool bShadow);
	void SetObjectName(const char* pszName);
	void SetHPValue(unsigned int uiMaxHP, unsigned int uiCurHP);
	void SetShowValue(bool bName, bool bHP, bool bShadow);

	void SetNameColorType(int iType);
	void SetLocal(bool bLocal);

	void SetTalkBalloon( const char* pszString , BROADCAST_SCOPE eScope , D3DXCOLOR color );
	void UpdateDrawLevel( int iCenterPosX , int iStartPosY );

	void SetTitle( SPTitleData& stTitleData );

	void SetUserShopName(std::string strName);		//[2006/5/22]
	std::string GetUserShopName();

	void SetGuildInfo(const char* pszGuildName, int iIcon = 0, int iIconBG = 0, int iFxID = 0);

	//bool SetUserShop(int iType);
	void ProcessUserShop(float fTime);
	void RenderUserShop(float fTime);
	void SetUserShopRgn(RECT& rcRgn, int iCenterPosX, int iCenterPosY);
	RECT* GetUserShopBoardRect();

	SPTitleRenderBase*	GetTitle();

	void SetLocalPlayerTeam( bool bLocalPlayerTeam );
	bool GetLocalPlayerTeam();
	void SetNameOffset( int iOffset ) { m_iNameOffset = iOffset; }


protected:
	void Destroy();

	SPTexture*		m_pkGageTexture;
	SPTexture*		m_pkShadowTexture;	
	
	std::string		m_szName;
	unsigned int	m_uiMaxHP;
	unsigned int	m_uiCurHP;
	float			m_fHPRatio;
	
	RECT			m_rcNameRgn;
	RECT			m_rcHPRgn;
	POINT			m_ptHPOffset;

	RECT			m_rcHPSrcRgn;
	RECT			m_rcHPBKSrcRgn;

	RECT			m_rcShadowSrcRgn;
	RECT			m_rcShadowDstRgn;

	D3DXCOLOR		m_NameColor;
	bool			m_bLocal;
	bool			m_bShowDraw;

	bool			m_bShowName;
	bool			m_bShowHP;
	bool			m_bShowShadow;
	int				m_iBeforeGuildFx ;

	SPTitleRenderBase*			m_pTiTleRender;	// 타이틀 [3/31/2006 AJJIYA]

	//	AJJIYA [8/25/2005]
	//	말풍선

	SPTexture*					m_pkTalkBalloon;
	RECT						m_rcTalkBallonImageSrcRgn[ TALKBALLON_COUNT ];
	RECT						m_rcTalkBallonImageDestRgn[ TALKBALLON_COUNT ];

	std::vector< std::string >	m_vTalkBalloonText;
	D3DXCOLOR					m_colorTalkBalloonText;
	std::vector< RECT >			m_vTalkBallonTextDestRgn;

	D3DXCOLOR					m_colorTalkBalloonTitle;
	RECT						m_rcTalkBallonTitleDestRgn;
	D3DXCOLOR					m_colorTalkBalloonID;
	D3DXCOLOR					m_colorTalkBalloonIDOther;
	RECT						m_rcTalkBallonIDDestRgn;

	int							m_iTalkBalloonImageWidth;
	int							m_iTalkBalloonImageTopHeight;
	int							m_iTalkBalloonImageBottomHeight;
	int							m_iTalkBalloonTextWidth;
	int							m_iTalkBalloonTextHeight;
	float						m_fTalkBalloonLimitTime;
	float						m_fTalkBalloonAccmulateTime;

	BROADCAST_SCOPE				m_eChatScope;

	SPTexture*					m_pkLevel;
	RECT						m_rcLevelTitleSrc;
	RECT						m_rcLevelTitleDest;
	POINT						m_ptLevelTitleSize;
	RECT						m_rcLevelSrc[ LEVEL_COUNT ];
	POINT						m_ptLevelPos[ LEVEL_COUNT ];
	int							m_iLevelCount;

	std::string					m_strShopName;			//[2006/5/19] - 임시, 상점인경우 상점명	
	RECT						m_rtDestShopBoard;		//[2006/6/5] 상점 간판
	RECT						m_rtDestShopName;		//[2006/6/5] 상점 주인 이름
	RECT						m_rtDestShopTitle;		//[2006/6/5] 상점 명
	RECT						m_rtDestShopOverLap;

	int							m_iCurFrame;			//[2006/6/5] 현재 프레임
	float						m_fLastRenderDelay;		//[2005/9/13]/**< 각 LoadStatus 의 마지막 시간을 기억 <br> */
	float						m_fAccumulateRender;	//[2005/9/9] /**< 렌더링 시간 계산 <br> */	

	bool						m_bLocalPlayerTeam;	// 현재 로컬 플레이어와 팀중이냐? [2/26/2007 AJJIYA]

	RECT m_rcGuildIcon;
	RECT m_rcGuildIconSrc;
	RECT m_rcGuildIconBGSrc;
	RECT m_rcGuildRgn;
	D3DXCOLOR m_GuildColor;
	std::string m_strGuild;
	SPTexture* m_pkGuildIconTexture;
	SPTexture* m_pkGuildIconBGTexture;
	SPFXArchive* m_pkGuildFxArchive ;
	bool		m_bSetGuildFx ;
	int m_iCenterPosX ;
	int m_iCenterPosY ;
	int m_iNameOffset;
	

	SPPlayer* m_pkParent;
};

#endif