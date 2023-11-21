// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 ø¿¿ÅE10:26:19
//***************************************************************************

#pragma once

//////////////////////////////////////////////////////////////////////////
// Child Window List
#define		WIID_STC_INDEX1			(WIID_CANDIVERTICAL	+	10 ) 
#define		WIID_STC_INDEX2			(WIID_CANDIVERTICAL	+	11 ) 
#define		WIID_STC_INDEX3			(WIID_CANDIVERTICAL	+	12 ) 
#define		WIID_STC_INDEX4			(WIID_CANDIVERTICAL	+	13 ) 
#define		WIID_STC_INDEX5			(WIID_CANDIVERTICAL	+	14 ) 
#define		WIID_STC_INDEX6			(WIID_CANDIVERTICAL	+	15 ) 
#define		WIID_STC_INDEX7			(WIID_CANDIVERTICAL	+	16 ) 
#define		WIID_STC_INDEX8			(WIID_CANDIVERTICAL	+	17 ) 
#define		WIID_STC_INDEX9			(WIID_CANDIVERTICAL	+	18 ) 

#define		WIID_STC_TEXT1			(WIID_CANDIVERTICAL	+	20 ) 	
#define		WIID_STC_TEXT2			(WIID_CANDIVERTICAL	+	21 ) 
#define		WIID_STC_TEXT3			(WIID_CANDIVERTICAL	+	22 ) 
#define		WIID_STC_TEXT4			(WIID_CANDIVERTICAL	+	23 ) 
#define		WIID_STC_TEXT5			(WIID_CANDIVERTICAL	+	24 )	
#define		WIID_STC_TEXT6			(WIID_CANDIVERTICAL	+	25 )
#define		WIID_STC_TEXT7			(WIID_CANDIVERTICAL	+	26 )	
#define		WIID_STC_TEXT8			(WIID_CANDIVERTICAL	+	27 )
#define		WIID_STC_TEXT9			(WIID_CANDIVERTICAL	+	28 )

#define		WIID_STC_PAGE			(WIID_CANDIVERTICAL	+	40 )



const int NO_FRAME = 9;
const int ITEM_SX = 70;
const int ITEM_SY = 15;

class SPWindow;
class SPWindowComboBox;

class SPWindowCandidateVertical : public SPWindow {

public:	
	SPWindowCandidateVertical(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowCandidateVertical();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	void Refresh();

	void ReposOwnImage();
	bool SetImageFrameHead	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageFrameBody	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageFrameTail	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	virtual int OnRefresh		( WPARAM, LPARAM );
	// }}

protected:
	/* 
		Candidate Frame Array

			0 1 2
			3 4 5
			6 7 8

	*/

	SPTexture*		m_paImageFrame[NO_FRAME];
	RECT			m_rcaFrameSrc[NO_FRAME];
	RECT			m_rcaFrameCalc[NO_FRAME];

	int				m_iMaxCandLength;


	SPTexture*		m_pImageFrameHead;
	SPTexture*		m_pImageFrameBody;
	SPTexture*		m_pImageFrameTail;

	RECT			m_rcFrameHeadSrc;
	RECT			m_rcFrameBodySrc;
	RECT			m_rcFrameTailSrc;

	RECT			m_rcCalcFrameHead;
	RECT			m_rcCalcFrameBody;
	RECT			m_rcCalcFrameTail;

	SPTexture*		m_pImageFrameCursor;
	RECT			m_rcFrameCursorSrc;
	RECT			m_rcCalcFrameCursor;
	
	int				m_iCursorPos;
	int				m_aiCursorY[9];

	//vector<string> m_vstr

};
