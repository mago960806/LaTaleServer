// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 오전 10:26:19
//***************************************************************************

#pragma once


//////////////////////////////////////////////////////////////////////////
// Child Window List
#define		WIID_STATUS_BTN_CLOSE					(WIID_STATUS	+	101 )
#define		WIID_STC_JOB_ICON						(WIID_STATUS	+	102 )
#define		WIID_STATUS_STC_ID						(WIID_STATUS	+	103 )
#define		WIID_LEVEL_TEXT							(WIID_STATUS	+	104 )
//#define		WIID_LEVEL_BG_IMAGE						(WIID_STATUS	+	141 )	///<★
//#define		WIID_TITLE_ICON							(WIID_STATUS	+	146 )	///<★

#define		WIID_STATUS_HP							(WIID_STATUS	+	111 )
#define		WIID_STATUS_HP_ADJ						(WIID_STATUS	+	112 )
#define		WIID_STATUS_SP							(WIID_STATUS	+	113 )
#define		WIID_STATUS_SP_ADJ						(WIID_STATUS	+	114 )
#define		WIID_STATUS_XP							(WIID_STATUS	+	115 )
#define		WIID_STATUS_FAME_VAL					(WIID_STATUS	+	116 )
#define		WIID_STATUS_FAME_VAL_ADJ				(WIID_STATUS	+	117 )
#define		WIID_STATUS_STRENGTH					(WIID_STATUS	+	118 )
#define		WIID_STATUS_STRENGTH_ADJ				(WIID_STATUS	+	119 )
#define		WIID_STATUS_DEFENCE						(WIID_STATUS	+	120 )
#define		WIID_STATUS_DEFENCE_ADJ					(WIID_STATUS	+	121 )
#define		WIID_STATUS_MAGIC						(WIID_STATUS	+	122 )
#define		WIID_STATUS_MAGIC_ADJ					(WIID_STATUS	+	123 )
#define		WIID_STATUS_LUCK						(WIID_STATUS	+	124 )
#define		WIID_STATUS_LUCK_ADJ					(WIID_STATUS	+	125 )

#define		WIID_STATUS_EARTH						(WIID_STATUS	+	126 )
#define		WIID_STATUS_FIRE						(WIID_STATUS	+	128 )
#define		WIID_STATUS_WATER						(WIID_STATUS	+	130 )
#define		WIID_STATUS_WIND						(WIID_STATUS	+	132 )
#define		WIID_STATUS_REGIST_EARTH				(WIID_STATUS	+	141 )
#define		WIID_STATUS_REGIST_FIRE					(WIID_STATUS	+	142 )
#define		WIID_STATUS_REGIST_WATER				(WIID_STATUS	+	143 )
#define		WIID_STATUS_REGIST_WIND					(WIID_STATUS	+	144 )

#define		WIID_STATUS_ATTC_RANGE					(WIID_STATUS	+	134 )
#define		WIID_STATUS_ARMOR						(WIID_STATUS	+	136 )

#define		WIID_STATUS_INDUN_RESET					(WIID_STATUS	+	140 )

#define		WIID_STATUS_PVP_PANEL					(WIID_STATUS	+	160 )
#define		WIID_STATUS_PVP_LV_ICON					(WIID_STATUS	+	161 )
#define		WIID_STATUS_PVP_LV_TEXT					(WIID_STATUS	+	162 )
#define		WIID_STATUS_PVP_FAME_ICON				(WIID_STATUS	+	163 )
#define		WIID_STATUS_PVP_FAME_TEXT				(WIID_STATUS	+	164 )
#define		WIID_STATUS_PVP_POINT_ICON				(WIID_STATUS	+	165 )
#define		WIID_STATUS_PVP_POINT_TEXT				(WIID_STATUS	+	166 )



#define		WIID_STATUS_COMBO						(WIID_STATUS	+	301 )

#define		_BG_MAX_	26			///< 배경 이미지 개수

class SPWindow;
class SPWindowComboBox;
class SPTimer ;
class SPWindowStatus : public SPWindow {

public:
	SPWindowStatus( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowStatus();

	virtual void Init();
	virtual void Init_Background();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild );
	virtual void Hide(bool bSendServer = true);		//[2005/11/15] - Option Window에만 적용

	void Refresh();

	virtual void RefreshRelationCoord();

public:
	void ReposOwnImage();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int OnRefresh		( WPARAM, LPARAM );
	int OnExitHide		( WPARAM, LPARAM );

	int			OnWndPosUpdate	( WPARAM wParam, LPARAM lParam );
	int			OnPurge			( WPARAM wParam, LPARAM lParam );

	int			OnTitleGetList	( WPARAM wParam, LPARAM lParam );
	int			OnTitleChange	( WPARAM wParam, LPARAM lParam );
	int			OnTitleAdd		( WPARAM wParam, LPARAM lParam );

	int			OnComboBoxToolTipMove	( WPARAM wParam, LPARAM lParam );
	int			OnComboBoxToolTipOut	( WPARAM wParam, LPARAM lParam );
	int			OnComboBoxSelect		( WPARAM wParam, LPARAM lParam );
	int			OnComboBoxListShowShow	( WPARAM wParam, LPARAM lParam );

	
	int			OnIndunReset	( WPARAM wParam, LPARAM lParam );
	int			OnYes			( WPARAM wParam, LPARAM lParam );
	int			OnNo			( WPARAM wParam, LPARAM lParam );
	// }}

protected:
	void		SetIndunResetButton(int iSet) ;
	void		RefreshIndunResetButton();

protected:
	bool				m_bIndunNotice ;			
	SPWindow*			m_pHide;					/**< 숨김 버튼 <br> */
	SPTimer*			m_pResetTimer ;				// 인던 리셋 타이머

	SPWindowComboBox*	m_pComboBoxTitle;

	RECT				m_rcSrc[_BG_MAX_] ;			///< 배경 Source-Rect
	RECT				m_rcBG[_BG_MAX_] ;			///< 배경 Display-Rect
	SPTexture*			m_pImageSrcBack;
};





