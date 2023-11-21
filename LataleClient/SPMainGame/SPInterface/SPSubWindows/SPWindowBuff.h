// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-10-10    15:53 
//***************************************************************************

#pragma once

//#define WIID_BUFF_TITLE						( WIID_BUFF + 10 )

#define	BUFF_X								13
#define BUFF_Y								1
#define BUFF_X_GAP							27
#define BUFF_Y_GAP							27
#define BUFF_X_OFFSET						0
#define BUFF_Y_OFFSET						0

class SPWindow;
class SPUIUnitManager;
class SPUIUnit;
class SPEffect;

class SPWindowBuff : public SPWindow 
{
public:

	SPWindowBuff(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowBuff(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowBuff();

	virtual	void		Init					();
	virtual	void		Clean					();

	virtual	void		Process					( float fTime );
	virtual	void		Render					( float fTime );

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
//	int					OnLButtonUp				( WPARAM wParam, LPARAM lParam );
	int					OnCursorMove			( WPARAM wParam, LPARAM lParam );
	int					OnCursorOut				( WPARAM wParam, LPARAM lParam );
	int					OnPosUpdate				( WPARAM wParam, LPARAM lParam );


	void				InitIcon				();

	void				ProcessBuffEffect		();
	void				ProcessToolTip			( float fTime );

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	SPUIUnitManager						m_UIUnitManager;

	SPUIUnit*							m_pUIUnit;
	float								m_fLimitTime;
	float								m_fAccmulateTime;

};
