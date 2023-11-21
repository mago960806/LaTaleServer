
// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-11-29 ¿ÀÈÄ 2:56:36
//***************************************************************************


#pragma once

enum NOTICE_TYPE;
class SPVector;
class SPBezier;
class SPFXArchive;
//class SPTimer;
#include <map>

class SPLobby : public SPSubGameManager
{
public:
	SPLobby();
	virtual ~SPLobby();

	virtual bool Init();
	virtual void Clean();
	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual bool WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual int	SPPerformMessage(UINT msg, WPARAM wparam = 0, LPARAM lParam = 0);

protected:
	SPGMESSAGE_MAP_DECLARE()

	// Network Event
	// sc
	int OnGetWorldListInfo(WPARAM wParam, LPARAM lParam);
	int OnGetChannelListInfo(WPARAM wParam, LPARAM lParam);
	int OnGameServerConnect(WPARAM wParam, LPARAM lParam);
	int OnGetCharacterListInfo(WPARAM wParam, LPARAM lParam);
	int OnCharacterNameCheck(WPARAM wParam, LPARAM lParam);
	int OnCharacterCreate(WPARAM wParam, LPARAM lParam);
	int OnCharacterDelete(WPARAM wParam, LPARAM lParam);
	int OnWorldEnter(WPARAM wParam, LPARAM lParam);
	int OnChannelSelect(WPARAM wParam, LPARAM lParam);
	int OnCharacterSelect(WPARAM wParam, LPARAM lParam);

	int OnUserIn(WPARAM wParam, LPARAM lParam);
	int OnUserMove(WPARAM wParam, LPARAM lParam);

	// cs
	int OnChannelListInfoRequest(WPARAM wParam, LPARAM lParam);
	int OnGameServerConnectRequest(WPARAM wParam, LPARAM lParam);

	int OnServerBack(WPARAM wParam, LPARAM lParam);
	void GetWorldListInfoRequest();
	int OnWorldListScroll(WPARAM wParam, LPARAM lParam);

	int OnCharacterMenuSelect(WPARAM wParam, LPARAM lParam);
	int OnCharacterMenuCreate(WPARAM wParam, LPARAM lParam);
	int OnCharacterMenuDelete(WPARAM wParam, LPARAM lParam);
	int OnCharacterCreateRequest(WPARAM wParam, LPARAM lParam);
	int OnCharacterNameCheckRequest(WPARAM wParam, LPARAM lParam);
	int OnCharacterBack(WPARAM wParam, LPARAM lParam);
	void GetCharacterListRequest();

	int OnCharacterCreateBack(WPARAM wParam, LPARAM lParam);
	
	void ProcessHotKey();

	void RgnInit();
	void ProcessParticle(float fTime);

	void BackgroundRender(float fTime);

	struct PARTICLE
	{
		int iResourceType;
		int iPathType;
		bool bLive;

		float fTime;
		float fLiveTime;
		float fResponeTime;

		SPVector kPos;
		SPBezier kBezier;

		PARTICLE& operator = (const PARTICLE& par)
		{
			iResourceType = par.iResourceType;
			iPathType = par.iPathType;
			bLive = par.bLive;
			fTime = par.fTime;
			fLiveTime = par.fLiveTime;
			fResponeTime = par.fResponeTime;
			kPos = par.kPos;
			kBezier = par.kBezier;

			return *this;
		}
	};

	void ParticleInit();
	void ParticleRespone(PARTICLE* pParticle);
	void ParticleRender();

	void InitEnterTime();
	bool CheckEnterTime(float fTime);
	
	void ProcessFade(float fTime);
	void FadeRender();

	bool ShowNoticeBox(bool bShow, NOTICE_TYPE iType, const TCHAR* szMsg = NULL);

	//void PrintLogs(const TCHAR* fmt, ...) ;
	//IGW
	std::string GetIGWServerName()	{ return m_IGWServerName; }
	int			GetIGWServerID()	{ return m_IGWServerID; }
protected: 
	SPTexture*		m_pkBackgroundFrame;
	SPTexture*		m_pkBackground1;	// 1. Layer Back
	SPFXArchive*	m_pkFXArchive1;		// 2. Layer FX
	SPTexture*		m_pkBackground2;	// 3. Layer Front
	SPFXArchive*	m_pkFXArchive2;		// 4. Layer Front FX

	///< Delete 2007.01.02
	//SPTexture* m_pkBackCloud;
	float m_fCloudRotation;
	float m_fCloudIncRotation;
	float m_fCloudDelayTime;
	float m_fCloudAccmulateTime;

	int m_iCurSunFrame;
	float m_fSunDelayTime;
	RECT m_rcSunRgn;
	std::vector< float > m_vSunAlpha;

	std::map< int, std::vector< SPVector > > m_mBezierControl;
	std::vector< PARTICLE > m_vParticle;

	// rgn
	std::vector< RECT > m_vBackgroundSrcRgn;
	std::vector< RECT > m_vBackgroundDstRgn;
	std::vector< RECT > m_vBackFrameSrcRgn;
	std::vector< RECT > m_vBackFrameDstRgn;
	std::vector< RECT > m_vParticleSrcRgn;

	// worldInfo
	int m_iCurServer;
	std::vector< WORLD_INFO > m_vWorldInfo;
	std::vector< CHANNEL_INFO > m_vChannelInfo;
	std::vector< CHARACTER_INFO2 > m_vCharacterInfo;
	std::map< unsigned int, std::vector< CONTAINER_ITEM > > m_mCharContainerItem;
	//std::vector< SPPlayer* > m_pvCharacterInfo;

	bool m_bGameServerConnectReq;
	int m_iSelectCard;

	float m_fEnterAccmulateTime;
	float m_fMaxEnterTime;
	bool m_bEnterTimeCheck;

	int m_iConnectType;
	float m_fGameServerConnectWaitTime;
	int m_iGameServerConnectType;
	SERVER_ID m_iGameServerID;

	bool m_bOnceSendPacket;
	bool m_bPrologueShow[3];

	SPTexture* m_pkFadeTexture;
	RECT m_rcSrcFade;
	RECT m_rcDstFade;
	int m_iAlphaFade;
	float m_fFadeAccmulateTime;

	SPTimer* m_pMsgTimer ;
	bool m_bChannelMsgClose ;

	std::string m_IGWServerName;
	int			m_IGWServerID;			
};








