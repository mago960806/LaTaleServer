#ifndef __SPEVENT_STATUS_MODEL__
#define __SPEVENT_STATUS_MODEL__

class SPEventStatusModel
{
	enum TALKBALLON_TYPE
	{
		TALKBALLON_TOP		,
		TALKBALLON_MIDDLE	,
		TALKBALLON_BOTTOM	,
		TALKBALLON_COUNT	,
	};

public:
	SPEventStatusModel();
	~SPEventStatusModel();

	void Render(float fTime);
	void Process(float fTime);

	void Init();

	void SetObjectRgn(RECT& rcRgn, int iCenterPosX, int iCenterPosY, bool bShadow);
	void SetObjectName(const char* pszName);

	void SetTalkBalloon(const char* pszString);

protected:
	void Destroy();
	
	std::string					m_szName;
	RECT						m_rcNameRgn;
	D3DXCOLOR					m_NameColor;

	SPTexture*					m_pkShadowTexture;
	RECT						m_rcShadowSrcRgn;
	RECT						m_rcShadowDstRgn;
	bool						m_bShadowDraw;

	//	AJJIYA [8/25/2005]
	//	¸»Ç³¼±

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

};

#endif