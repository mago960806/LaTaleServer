// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD DESTROY WINDOW
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#pragma once

//------------------------------------------------------------------------------------
// 
#define WIID_DESTROYGUILD_ICON				(WIID_GUILD_CREATE + 501)
#define WIID_DESTROYGUILD_TEXT				(WIID_GUILD_CREATE + 502)

#define WIID_DESTROYGUILD_COMMIT			(WIID_GUILD_CREATE + 503)
#define WIID_DESTROYGUILD_CANCEL			(WIID_GUILD_CREATE + 504)

//------------------------------------------------------------------------------------

#define GUILD_DESTROY_BG_COUNT			12


class SPWindowGuildDestroy : public SPWindow
{
public:
	SPWindowGuildDestroy( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual	~SPWindowGuildDestroy();

	virtual	void		Init				();
	virtual	void		Clean				();
	virtual void		Show				(bool bWithChild = true);
	virtual void		Hide				(bool bSendServer = true);

	virtual	void		Process				( float fTime );
	virtual	void		Render				( float fTime );
	virtual void		RefreshRelationCoord();

private:
	void				ReposOwnImage();

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int					OnDestroyOK			( WPARAM wParam, LPARAM lParam );
	int					OnCancel			( WPARAM wParam, LPARAM lParam );

	// }}


protected:
	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//
	SPTexture*			m_pBGTexture;
	RECT				m_rcBGDest[ GUILD_DESTROY_BG_COUNT ];
	RECT				m_rcBGSrc[ GUILD_DESTROY_BG_COUNT ];
};

