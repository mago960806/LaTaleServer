// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

class SPWindowGauge : public SPWindow {

public:
	SPWindowGauge(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent);
	virtual ~SPWindowGauge();

	virtual void Init();
	virtual void InitFromXML(SPWindow_Base* rec_wnd_AttrMap);
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void RefreshRelationCoord();

	void SetImageEdge(const char* pszImage, RECT rcEdge);	
	void SetImageGauge(const char* pszImage, RECT rcGaugeSrc);
	void SetImageDeco(const char* pszImage, RECT rcDeco);

	void Refresh();

	void ReposOwnImage();
	bool SetImageFrameHead	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageFrameBody	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageFrameTail	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);

	// flicker ±ôºýÀÌ´Â°Å ¼¼ÆÃ [1/29/2007 AJJIYA]
	void SetFlicker			( float fPercent );

	bool SetImageFlickerEdge( const char* szImage , int iXSrc, int iYSrc, int iX2Src, int iY2Src );
	bool SetImageFlickerGauge( const char* szImage , int iXSrc, int iYSrc, int iX2Src, int iY2Src );
	bool SetImageFlickerDeco( const char* szImage , int iXSrc, int iYSrc, int iX2Src, int iY2Src );


protected:

	SPIMESSAGE_OBJ_MAP_DECLARE()	
	int OnSetMaxValue	( WPARAM wParam, LPARAM lParam );
	int OnSetCurValue	( WPARAM wParam, LPARAM lParam );	
	int OnRefresh		( WPARAM wParam, LPARAM lParam );

protected:

	SPTexture*		m_pImageEdge;
	SPTexture*		m_pImageGauge;
	SPTexture*		m_pImageDeco;
	INT64			m_iMaxValue;
	INT64			m_iCurValue;	

	RECT			m_rcGaugeSrc;	
	RECT			m_rcGaugeEdgeSrc;
	RECT			m_rcDecoSrc;
	RECT			m_rcCalcGauge;
	RECT			m_rcCalcGaugeEdge;
	RECT			m_rcCalcDeco;

	SPTexture*		m_pImageFrameHead;
	SPTexture*		m_pImageFrameBody;
	SPTexture*		m_pImageFrameTail;

	RECT			m_rcFrameHeadSrc;
	RECT			m_rcFrameBodySrc;
	RECT			m_rcFrameTailSrc;

	RECT			m_rcCalcFrameHead;
	RECT			m_rcCalcFrameBody;
	RECT			m_rcCalcFrameTail;

	//////////////////////////////////////////////////////////////////////////

	bool			m_bShowFlicker;
	float			m_fFlickerPercent;
	
	float			m_fFlickerAccmulateTime;
	float			m_fFlickerHideLimitTime;
	float			m_fFlickerShowLimitTime;
	float			m_fFlickerTotalLimitTime;

	SPTexture*		m_pImageFlickerEdge;
	SPTexture*		m_pImageFlickerGauge;
	SPTexture*		m_pImageFlickerDeco;
	RECT			m_rcFlickerEdgeSrc;
	RECT			m_rcFlickerGaugeSrc;
	RECT			m_rcFlickerDecoSrc;

};
