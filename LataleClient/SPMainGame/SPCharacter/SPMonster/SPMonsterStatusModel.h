#ifndef __SPMONSTER_STATUS_MODEL__
#define __SPMONSTER_STATUS_MODEL__

enum MONSTER_INDEX;

class SPGameObject;
class SPMonsterStatusModel
{
	enum TALKBALLON_TYPE
	{
		TALKBALLON_TOP,
		TALKBALLON_MIDDLE,
		TALKBALLON_BOTTOM,
		TALKBALLON_COUNT,
	};

public:
	SPMonsterStatusModel(SPGameObject* pkParent);
	~SPMonsterStatusModel();

	void Render(float fTime);
	void Process(float fTime);

	void Init();

	void SetObjectRgn(RECT& rcRgn, int iCenterPosX, int iCenterPosY, bool bShadow);
	void SetObjectName(const char* pszName, D3DXCOLOR* pColor = NULL);
	void SetHPValue(unsigned int uiMaxHP, unsigned int uiCurHP);

	void SetMonterType(MONSTER_INDEX eType);
	void SetShowValue(bool bName, bool bHP, bool bShadow);
	void SetNameColor(LPARAM lParam);
	void SetToggleName(bool bEnable);

	void SetIconIndex(int iIndex);
	int  GetIconIndex();
	void SetTalkBalloon(const char* pszString);

	void SetPet(bool bPet);
	void SetPetGageRgn(int iCurSP, int iMaxSP, INT64 iCurXP, INT64 iMaxXP);

	bool IsShowName() { return m_bShowName; }
	bool IsShowHP() { return m_bShowHP; }
	//--------------------------------------------------
	// for pet shop use
	void ProcessPetShop( float fTime );
	void RenderPetShop( float fTime );
	void SetPetShopRgn( RECT &rcRgn, int iCenterPosX, int iCenterPosY );
	RECT *GetPetShopBoardRect( void );
	void SetShopType( int iShopType )
	{
		m_iShopType = iShopType;
		if (m_iShopType > 0)
			m_bOpenShop = true;
		else
			m_bOpenShop = false;
	}
	void SetShopName( std::string strShopName )
	{
		m_strShopName = strShopName;
	}
	//--------------------------------------------------
protected:
	void Destroy();

	SPTexture* m_pkGageTexture;
	SPTexture* m_pkShadowTexture;
	
	std::string m_szName;
	unsigned int m_uiMaxHP;
	unsigned int m_uiCurHP;
	float m_fHPRatio;
	float m_fSPRatio;
	float m_fXPRatio;
	
	RECT m_rcNameRgn;
	RECT m_rcHPRgn;
	POINT m_ptHPOffset;
	RECT m_rcSPRgn;
	RECT m_rcXPRgn;

	RECT m_rcHPSrcRgn;
	RECT m_rcHPBKSrcRgn;
	RECT m_rcSPSrcRgn;
	RECT m_rcSPBKSrcRgn;
	RECT m_rcXPSrcRgn;
	RECT m_rcXPBKSrcRgn;

	RECT m_rcShadowSrcRgn;
	RECT m_rcShadowDstRgn;
	bool m_bShadowDraw;

	bool m_bShowName;
	bool m_bShowHP;
	bool m_bShowShadow;
	bool m_bToggleName;
	bool m_bIsPet;

	int m_iIconIndex;
	RECT m_rcIconDstRgn;

	D3DXCOLOR m_NameColor;
	SPGameObject* m_pkParent;

	SPTexture*	m_pkTalkBalloon;
	RECT m_rcTalkBallonImageSrcRgn[ TALKBALLON_COUNT ];
	RECT m_rcTalkBallonImageDestRgn[ TALKBALLON_COUNT ];

	std::vector< std::string > m_vTalkBalloonText;
	D3DXCOLOR m_colorTalkBalloonText;
	std::vector< RECT >	m_vTalkBallonTextDestRgn;

	D3DXCOLOR					m_colorTalkBalloonID;
	RECT						m_rcTalkBallonIDDestRgn;

	int	m_iTalkBalloonImageWidth;
	int	m_iTalkBalloonImageTopHeight;
	int	m_iTalkBalloonImageBottomHeight;
	int	m_iTalkBalloonTextWidth;
	int	m_iTalkBalloonTextHeight;
	float m_fTalkBalloonLimitTime;
	float m_fTalkBalloonAccmulateTime;

	//--------------------------------------------------
	// for pet shop
	bool			m_bOpenShop;
	int				m_iShopType;
	std::string		m_strShopName;
	RECT			m_rtDestShopBoard;
	RECT			m_rtDestShopName;
	RECT			m_rtDestShopTitle;
	RECT			m_rtDestShopOverLap;
	int				m_iCurFrame;
	float			m_fLastRenderDelay;
	float			m_fAccumulateRender;
	//--------------------------------------------------
};

#endif
