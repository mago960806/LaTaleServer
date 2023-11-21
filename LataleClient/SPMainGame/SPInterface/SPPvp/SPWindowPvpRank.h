// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowPvpRank.h
//	created:	2007/11/14   Time:15:29
//	purpose:	
//		PvpRank는 PVP 진행중에 중간 순위를 보여주는 창이다.
//		현재 PVP를 진행중인 사용자 수 만큼 보여진다.
//		자신의 순위는 Background를 통해서 부각된다.
//*********************************************************************/

#pragma once

#define WIID_PVP_RANKING_RANK			(WIID_PVP_RANKING + 1)			// Max 8
#define WIID_PVP_RANKING_PLAYER		(WIID_PVP_RANKING + 11)			// Max 8

#define PVP_RANKING_BG			9
#define PVP_RANK_SHOW_TIME		5.0f
#define PVP_RANK_MOVE_TIME		0.3f


enum RANK_SHOW_MODE {
	RANK_MODE_NULL = 0,
	RANK_MODE_IN ,			// Ranking Move In
	RANK_MODE_SHOW ,			// Ranking Showing
	RANK_MODE_OUT ,			// Ranking Move Out
} ;

class SPTimer ;
class SPWindowPvpRank : public SPWindow {
public:	
	SPWindowPvpRank(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPvpRank();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	void ProcessMove(float fTime) ;			// Move Window Show-IN or Show-OUT
	void ReposOwnImage() ;
	void InitSubControl();

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()	
	int OnClose				( int iID, WPARAM wParam, LPARAM lParam );
	int OnSetRankData		( WPARAM wParam, LPARAM lParam ) ;
	//}}

private:
	SPTimer*		m_pTimer ;
	SPTexture*		m_pTextureBase;
	SPTexture*		m_pTextureBar ;
	RECT			m_rtSrcBase[PVP_RANKING_BG];
	RECT			m_rtDestBase[PVP_RANKING_BG];

	RECT			m_rtSrcBar ;
	RECT			m_rtDstBar ;

	int				m_iUserCount ;
	int				m_iMyRank ;
	int				m_iRankShowMode ;
	float			m_fMoveAccumulate ;
};