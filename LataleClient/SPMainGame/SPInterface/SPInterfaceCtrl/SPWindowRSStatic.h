// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowRSStatic.h
//	created:	2007/9/21   Time:15:35
//	purpose:	
//*********************************************************************/
/*
China RS Client 용으로 XML-Link 내용을 포함
# MouseOver 시 글자색 변경 (Link가 있는 경우)
# MouseClick시 Link Value Return -> SendParent
*/

#pragma once

#include "SPWindowListDEF.h"

class SPWindow;
class SPWindowRSStatic :	public SPWindow {
public:
	SPWindowRSStatic(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent);
	~SPWindowRSStatic();

	virtual void Init();
	virtual void Clean();	
	virtual void Process(float fTime);	
	virtual void Render(float fTime);
	virtual void RenderText();
	virtual void SetWindowText(const char* pStr = NULL);
	virtual void RefreshRelationCoord();

	bool bHasReturn();
	void SetReturnValue(int iValue, const char* pRetStr=NULL) ;
	LIST_RETURNVALUE*	GetReturnValue() ;
	void SetSelectBar(const char* pszImage, RECT rcImageSrc, int iRelPosX, int iRelPosY) ;
	void SetSelectFontcolor( bool bSet, D3DXCOLOR FontColor= D3DXCOLOR(0,0,0,1.0f), D3DXCOLOR RvsColor = D3DXCOLOR(1.0f,0,0,1.0f) );	
	int GetTextCount()		{ return (int)m_vWindowTextList.size(); }
	int GetTextPixelHeight();

	void SetCenterRender( bool bCenter );
	bool GetCenterRender();


protected:

	SPIMESSAGE_OBJ_MAP_DECLARE()
		// {{ Message Map	
	int OnCursorEnter	( WPARAM, LPARAM );
	int OnCursorOut		( WPARAM, LPARAM );
	int OnLButtonUp		( WPARAM wParam, LPARAM lParam);
	int OnWheelUp		( WPARAM wParam, LPARAM lParam);
	int OnWheelDown		( WPARAM wParam, LPARAM lParam);
	// }}

protected:
	bool		m_bUseFontOver ;
	D3DXCOLOR	m_RvsFontColor	;			// MouseOver Change Color
	D3DXCOLOR	m_CurFontColor	;			// for Render Font Color
	DWORD		m_dwFormat;					// for MultiLine Font Format

	LIST_RETURNVALUE m_stReturnValue;		//	돌려줄 값
	bool		m_bUseSelectBar ;	// SelectBar 사용여부
	SPTexture*	m_pBarTexture ;
	RECT		m_rcSelectBarSrc;	// Bar의 소스좌표
	int			m_rcRelPosX;		// Bar의 출력 상대좌표
	int			m_rcRelPosY;		// Bar의 출력 상대좌표

	std::vector< std::string > m_vWindowTextList;

	bool	m_bCenterRender;
	RECT	m_rcTgtRect ;

	int		m_iSrcWidth ;
	int		m_iSrcHeight ;
	int		m_iSrcMargin ;
	bool	m_bFontShadow ;

};
