// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowTextBook.h
//	created:	2007/9/21   Time:11:32
//	purpose:	
//*********************************************************************/

#pragma once
//------------------------------------------------------------------------------------
#define WIID_TEXTBOOK_TEXT				(WIID_TEXTBOOK + 1)
#define WIID_TEXTBOOK_EXIT				(WIID_TEXTBOOK + 2)
#define WIID_TEXTBOOK_PRE_PAGE_BG		(WIID_TEXTBOOK + 4)
#define WIID_TEXTBOOK_PRE_PAGE			(WIID_TEXTBOOK + 5)
#define WIID_TEXTBOOK_NEXT_PAGE_BG		(WIID_TEXTBOOK + 6)
#define WIID_TEXTBOOK_NEXT_PAGE			(WIID_TEXTBOOK + 7)


#define MAX_PAGEANIMAION		13			// 0 ~ 12 : 13Page 

enum TEXTBOOK_STATE {
	TEXTBOOK_FADEIN,
	TEXTBOOK_TEXT_IN,
	TEXTBOOK_TEXT_READ,
	TEXTBOOK_TEXT_OUT,
	TEXTBOOK_PAGENEXT,
	TEXTBOOK_PAGEPRE,
	TEXTBOOK_FADEOUT,
} ;



class SPWindowTextBook : public SPWindow
{
public:
	SPWindowTextBook(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowTextBook(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowTextBook();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);
	virtual void Show(bool bWithChild = true);

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnExit(WPARAM wParam, LPARAM lParam);
	virtual int OnNextPage(WPARAM wParam, LPARAM lParam);
	virtual int OnPrePage(WPARAM wParam, LPARAM lParam);

	virtual int OnSetFactor(WPARAM wParam, LPARAM lParam);

protected:
	void ImageRender();
	void BookTextRender(float fTime) ;
	void LoadPageAnimation() ;
	void ClearAnimationTexture();

	void ButtonHide() ;				// 페이지 이동시에도 없애고 보이도록 설정
	void ButtonShow() ;				//

	void ProcessFade(float fTime) ;
	void SetViewState(TEXTBOOK_STATE iViewState);
	void ReadTextStory(int iIncIndex);

protected:
	//int m_iCurIndex;			// 현재 페이지 번호
	//int m_iMaxIndex;			// 마지막 페이지 번호
	int m_iReadIncrement ;		// Fade 이후 읽어야 할 LDT Index 증감
	int m_iCurRead ;			// 현재 LDT DATA Index
	int m_iPreRead ;			// 이전페이지 LDT DATA
	int m_iNextRead ;			// 다음페이지 LDT DATA

	TEXTBOOK_STATE	m_iViewState ;
	TEXTBOOK_STATE	m_iDestState ;
	float	m_fFadeTime ;
	RECT	m_rtSrcFade ;
	RECT	m_rtDestFade ;
	RECT	m_rtSrcIllust ;
	RECT	m_rtDestIllust ;
	int		m_iBlackAlpha;
	int		m_iTextAlpha ;
	int		m_iBookAniIndex ;
	SPTexture* m_pFadeTexture ;
	SPTexture** m_ppCurAni ;
	SPTexture* m_pIllust ;

	SPWindow* m_pBookText ;
	std::vector< SPTexture* > m_vTexture;
	//std::vector< std::string > m_vstrStory;
};


