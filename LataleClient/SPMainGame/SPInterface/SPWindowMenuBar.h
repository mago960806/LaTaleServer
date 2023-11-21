// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

#define WIID_MENUBAR_TOP			(WIID_MENUBAR + 1)
#define WIID_MENUBAR_CENTER			(WIID_MENUBAR + 2)

#define WIID_MENUBAR_LINE1			(WIID_MENUBAR + 10)
#define WIID_MENUBAR_LINE2			(WIID_MENUBAR + 11)
#define WIID_MENUBAR_LINE3			(WIID_MENUBAR + 12)
#define LINE_COUNT					3

#define WIID_MENUBAR_SELECT 		(WIID_MENUBAR + 50)
#define WIID_MENUBAR_BUTTON			(WIID_MENUBAR + 100)


enum MENU_BUTTON
{
	MENU_BUTTON_ITEM			= 0		,
	MENU_BUTTON_EQUIP			= 1		,
	MENU_BUTTON_STATUS			= 2		,
	MENU_BUTTON_SKILL			= 3		,
	MENU_BUTTON_QUEST			= 4		,
	MENU_BUTTON_GUILD			= 5		,
	MENU_BUTTON_PARTY			= 6		,
	MENU_BUTTON_FRIEND			= 7		,
	MENU_BUTTON_PET				= 8		,
	MENU_BUTTON_OPTION			= 9		,
	MENU_BUTTON_HELP			= 10	,
	MENU_BUTTON_EXIT			= 11	,
	MENU_BUTTON_RSCLIENT		= 12	,
	MENU_BUTTON_CHATMACRO		= 13	,
	MENU_BUTTON_GUIDEBOOK		= 14	,
	MENU_BUTTON_CHATTINGROOM	= 15	,
	MENU_BUTTON_AUTOSET			= 16	,
	MENU_BUTTON_LOVE			= 17	,
	MENU_BUTTON_MAX						,
};

enum MENUBAR_BG
{
	MENUBAR_BG_TOP			=	0	,
	MENUBAR_BG_MIDDLE				,

	MENUBAR_BG_COUNT				,
};

class SPWindow;
class SPWindowMenuBar : public SPWindow {

public:
	SPWindowMenuBar(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowMenuBar(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowMenuBar();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

public:

protected:
	void FadeProcess(float fTime);	

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int OnMenuClick				(unsigned int uiID, WPARAM wParam, LPARAM lParam );

	// }}
	 
protected:

	SPTexture*					m_pBGTexture;
	RECT						m_rcBGDest[ MENUBAR_BG_COUNT ];
	RECT						m_rcBGSrc[ MENUBAR_BG_COUNT ];
	
	int							m_iAlpha;
	
	float						m_fLastRenderDelay;	
	float						m_fAccumulateRender;
	float						m_fCurDelay;

};




