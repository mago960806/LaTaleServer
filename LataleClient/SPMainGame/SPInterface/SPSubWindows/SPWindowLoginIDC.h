// ***************************************************************
//  SPWindowLoginIDC   version:  1.0   ¡¤  date: 05/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowLoginIDC_h__
#define SPWindowLoginIDC_h__

#pragma once

#include "./NetworkCore/Ping/LTPing.h"

#ifdef _DEFINE_DEPRECATED_HASH_CLASSES
#undef _DEFINE_DEPRECATED_HASH_CLASSES
#define _DEFINE_DEPRECATED_HASH_CLASSES 0	
#endif

#define WIID_LOGINIDC_SLIDER					( WIID_LOGINIDC + 10 )
#define WIID_LOGINIDC_SLIDER_UP					( WIID_LOGINIDC + 20 )
#define WIID_LOGINIDC_SLIDER_DOWN				( WIID_LOGINIDC + 30 )

#define WIID_LOGINIDC_CLOSE_ICON				( WIID_LOGINIDC + 40 )
#define WIID_LOGINIDC_CLOSE						( WIID_LOGINIDC + 50 )
#define WIID_LOGINIDC_OK						( WIID_LOGINIDC + 60 )
#define WIID_LOGINIDC_LOADING					( WIID_LOGINIDC + 70 )

#define WIID_LOGINIDC_SUB_PANNEL				( WIID_LOGINIDC + 100 )
#define WIID_LOGINIDC_SUB_TITLE_BG				( WIID_LOGINIDC + 200 )
#define WIID_LOGINIDC_SUB_TITLE					( WIID_LOGINIDC + 300 )
#define WIID_LOGINIDC_SUB_IDC_ICON				( WIID_LOGINIDC + 1000 )
#define WIID_LOGINIDC_SUB_IDC_TEXT				( WIID_LOGINIDC + 2000 )
#define WIID_LOGINIDC_SUB_IDC_PING				( WIID_LOGINIDC + 3000 )
#define WIID_LOGINIDC_SUB_IDC_BG				( WIID_LOGINIDC + 4000 )
#define WIID_LOGINIDC_SUB_IDC_BG_END			( WIID_LOGINIDC + 4999 )

enum LOGIN_IDC_BG
{
	LOGIN_IDC_BG_LEFT_TOP		=	0	,
	LOGIN_IDC_BG_CENTER_TOP				,
	LOGIN_IDC_BG_RIGHT_TOP				,

	LOGIN_IDC_BG_LEFT_MIDDLE			,
	LOGIN_IDC_BG_CENTER_MIDDLE			,
	LOGIN_IDC_BG_RIGHT_MIDDLE			,

	LOGIN_IDC_BG_LEFT_BOTTOM			,
	LOGIN_IDC_BG_CENTER_BOTTOM			,
	LOGIN_IDC_BG_RIGHT_BOTTOM			,

	LOGIN_IDC_BG_LINE1					,
	LOGIN_IDC_BG_SUB_WHITE				,
	LOGIN_IDC_BG_IDC_TITLE				,

	LOGIN_IDC_BG_COUNT					,
};

#define LOGIN_IDC_ZONE_X_MAX				2
#define LOGIN_IDC_ZONE_Y_MAX				3
#define LOGIN_IDC_ZONE_COUNT				LOGIN_IDC_ZONE_X_MAX * LOGIN_IDC_ZONE_Y_MAX
#define LOGIN_IDC_ZONE_SUB_COUNT			4
#define LOGIN_IDC_ZONE_SUB_ICON_COUNT		5
#define LOGIN_IDC_LOADING_IMAGE_COUNT		4

struct LoginIDC
{
	std::string								m_strZone;
	std::string								m_strIDC;
	std::vector< std::string >				m_strLoginServerIPList;
	std::string								m_strLabel;

	int										m_iPing;
	bool									m_bSelect;
};

typedef std::vector< LoginIDC* >						STD_VECTOR_LOGINIDC;

struct LoginZone
{
	std::string								m_strZone;
	STD_VECTOR_LOGINIDC						m_strIDCList;
};

typedef std::vector< LoginZone* >						STD_VECTOR_LOGINZONE;

typedef	stdext::hash_map< std::string , LoginZone* >	STDEXT_HASH_MAP_LOGINZONE;
typedef	stdext::hash_map< std::string , LoginIDC* >		STDEXT_HASH_MAP_LOGINIDC;
typedef	stdext::hash_map< std::string , LoginIDC* >		STDEXT_HASH_MAP_LOGINLABEL;

class SPWindowLoginIDC : public SPWindow 
{

public:
	SPWindowLoginIDC(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowLoginIDC(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowLoginIDC();

	virtual	void		Init				();
	virtual	void		Clean				();
	virtual void		Show				( bool bWithChild = true );
	virtual void		Hide				( bool bSendServer = true) ;

	virtual	void		Process				( float fTime );
	virtual	void		Render				( float fTime );

	virtual void		RefreshRelationCoord();

private:
	void				ReposOwnImage		();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int					OnIDCShow			( WPARAM wParam, LPARAM lParam );
	int					OnEscape			( WPARAM wParam, LPARAM lParam );
	int					OnKeyReturn			( WPARAM wParam, LPARAM lParam );

	int					OnClose				( WPARAM wParam, LPARAM lParam );
	int					OnOk				( WPARAM wParam, LPARAM lParam );

	int					OnSliderUp			( WPARAM wParam, LPARAM lParam );
	int					OnSliderDown		( WPARAM wParam, LPARAM lParam );

	int					OnSliderUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnSliderDown		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnSlider			( WPARAM wParam, LPARAM lParam );

	int					OnIDCSelect			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnLButtonDblClick	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	// }}

	//////////////////////////////////////////////////////////////////////////

	void				InitLDTLoad			();
	void				InitChildZone		();
	void				AddZoneIDC			( const char* pstrZone , const char* pstrIDC , const char* pstrLoginServerIP , const char* pstrLabel );
	void				RefreshPage			();

	LoginZone*			GetLoginZone		( const char* pstrZone );
	LoginIDC*			GetLoingIDC			( const char* pstrZone , const char* pstrIDC );

	void				DeleteLoginZoneIDC	();

	void				UpdateIDC			();
	void				UpdateIDC			( int iPos , LoginZone* pLoginIDCZone );

	void				ProcessPing			();

	void				LastLoginServer		();

	void				SetBGButtonEnable	( SPWindow* pWindow , bool bEnable );
	void				SetTextEnable		( SPWindow* pWindow , bool bEnable );

	int					CalPage				( int iItemCount );
	void				CheckButton			();

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	bool									m_bInitAutoShow;

	SPTexture*								m_pBGTexture;
	RECT									m_rcBGDest[ LOGIN_IDC_BG_COUNT ];
	RECT									m_rcBGSrc[ LOGIN_IDC_BG_COUNT ];

	SPWindowSlider*							m_pSlider;

	STD_VECTOR_LOGINZONE					m_vLoginZone;

	STDEXT_HASH_MAP_LOGINZONE				m_hmLoginZone;
	STDEXT_HASH_MAP_LOGINIDC				m_hmLoginIDC;
	STDEXT_HASH_MAP_LOGINLABEL				m_hmLoginLabel;

	int										m_iCurPage;

	INSTANCE_ID								m_eSelectID;
	LoginIDC*								m_pSelectIDC;

	int										m_iCurPingZonePos;
	int										m_iCurPingIDCPos;
	int										m_iCurPingIPCPos;
	bool									m_bPingInit;
	CLTPing									m_PingManager;
	bool									m_bPingComplete;

	float									m_fPingLimitTime;
	float									m_fPingAccmulateTime;

	float									m_fLoadingLimitTime;
	float									m_fLoadingAccmulateTime;
	int										m_iLoadingFrame;

};

#endif // SPWindowLoginIDC_h__