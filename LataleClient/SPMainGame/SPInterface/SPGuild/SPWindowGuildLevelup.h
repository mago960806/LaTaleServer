// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD LEVELUP WINDOW
// Comment     : 
// Creation    : DURAGON 2007-8-23    11:18 
//***************************************************************************

#pragma once

//------------------------------------------------------------------------------------
// Property Select
#define WIID_GUILD_LEVELUP_CLOSE		(WIID_GUILD_LEVELUP + 1)
#define WIID_GUILD_LEVELUP_COMMIT		(WIID_GUILD_LEVELUP + 2)

#define WIID_GUILD_LEVELUP_TITLE		(WIID_GUILD_LEVELUP + 11)
#define WIID_GUILD_LEVELUP_DESC			(WIID_GUILD_LEVELUP + 12)

#define WIID_GUILD_LEVELUP_NEEDLIST		(WIID_GUILD_LEVELUP + 50)	// 8

#define WIID_GUILD_LEVELUP_END			(WIID_GUILD_LEVELUP + 100)



//------------------------------------------------------------------------------------
#define GUILD_LEVELUP_BG_IMG		12

class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPWindowEdit;
class SPWindowGuildLevelup : public SPWindow 
{

public:
	SPWindowGuildLevelup(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowGuildLevelup(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowGuildLevelup();

	virtual	void	Init				();
	virtual	void	Clean				();
	virtual void	Show				(bool bWithChild = true);
	virtual void	Hide				(bool bSendServer = true);

	virtual	void	Process				( float fTime );
	virtual	void	Render				( float fTime );

	virtual void	RefreshRelationCoord();

	void			SetGuildLevelup(int idestLevel) ;
	void			SetLevelUpNeedList() ;

private:
	void			ReposOwnImage() ;
	void			RenderBackGround(float fTime);

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int				OnClose				( WPARAM wParam, LPARAM lParam );	// 닫기
	int				OnCommit			( WPARAM wParam, LPARAM lParam );	// 확인

	int				OnListResize		( WPARAM wParam, LPARAM lParam );	// 생성 결과
	int				OnLevelupResult		( WPARAM wParam, LPARAM lParam );	// 생성 결과

	// }}

	//////////////////////////////////////////////////////////////////////////
	//	변수들
	SPWindow*		m_pNeedList ;
	SPTexture*		m_pBGTexture;
	int				m_iListSY ;			// 리스트의 시작위치
	int				m_iListSize ;		// 리트스의 크기
	RECT			m_rcBGDst[ GUILD_LEVELUP_BG_IMG ];
	RECT			m_rcBGSrc[ GUILD_LEVELUP_BG_IMG ];
};