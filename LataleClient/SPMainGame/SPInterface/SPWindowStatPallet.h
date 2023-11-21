// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

//////////////////////////////////////////////////////////////////////////
// Child Window List
#define		WIID_SPALLET_JOB_ICON		(WIID_STATPALLET	+	5 ) 
#define		WIID_SPALLET_LEVEL_TEXT		(WIID_STATPALLET	+	7 ) 
#define		WIID_SPALLET_ID				(WIID_STATPALLET	+	9 ) 
#define		WIID_SPALLET_GAUGE_HP		(WIID_STATPALLET	+	10 ) 
#define		WIID_SPALLET_GAUGE_SP		(WIID_STATPALLET	+	11 ) 
#define		WIID_SPALLET_GAUGE_XP		(WIID_STATPALLET	+	12 ) 
#define		WIID_BACKBOARD_HP			(WIID_STATPALLET	+	13 ) 
#define		WIID_BACKBOARD_SP			(WIID_STATPALLET	+	14 ) 
#define		WIID_BACKBOARD_XP			(WIID_STATPALLET	+	15 ) 
#define		WIID_SPALLET_LV_DIGIT		(WIID_STATPALLET	+	16 ) 	
#define		WIID_SPALLET_HP_DIGIT		(WIID_STATPALLET	+	20 ) 
#define		WIID_SPALLET_SP_DIGIT		(WIID_STATPALLET	+	30 ) 
#define		WIID_SPALLET_XP_DIGIT		(WIID_STATPALLET	+	40 ) 
#define		WIID_ELY_BACKBOARD			(WIID_STATPALLET	+	50 )	
#define		WIID_SPALLET_ELY_DIGIT		(WIID_STATPALLET	+	60 )
#define		WIID_SPALLET_LETTER			(WIID_STATPALLET	+	70 )	
#define		WIID_SPALLET_LETTER_NEW		(WIID_STATPALLET	+	80 )

#define		WIID_STATPALLET_CASH		(WIID_STATPALLET	+	90 )
#define		WIID_STATPALLET_CASH_BACK	(WIID_STATPALLET	+	91 )
#define		WIID_STATPALLET_POINT		(WIID_STATPALLET	+	100 )
#define		WIID_STATPALLET_POINT_BACK	(WIID_STATPALLET	+	101 )



class SPWindow;
class SPWindowComboBox;

class SPWindowStatPallet : public SPWindow {

public:	
	SPWindowStatPallet(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowStatPallet();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);

	void Refresh(int iRefreshCate);

	void ReposOwnImage();
	bool SetImageFrameHead	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageFrameBody	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageFrameTail	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);

protected:
	void ArrangeControl();

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	virtual int OnRefresh		( WPARAM, LPARAM );
	// }}

protected:

	SPTexture*		m_pImageFrameHead;
	SPTexture*		m_pImageFrameBody;
	SPTexture*		m_pImageFrameTail;

	RECT			m_rcFrameHeadSrc;
	RECT			m_rcFrameBodySrc;
	RECT			m_rcFrameTailSrc;

	RECT			m_rcCalcFrameHead;
	RECT			m_rcCalcFrameBody;
	RECT			m_rcCalcFrameTail;




};
