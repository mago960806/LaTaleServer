// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowPvpLoadWait.h
//	created:	2007/11/14   Time:11:46
//	purpose:	
//		PVPLoadWait은 Zone에서 PVPLoading이 완료되는 시점까지 보여진다.
//		사용자의 모든 입력은 막혀야 한다. -> SPZone, InterfaceManger 처리
//		사용자의 계정명 및 Ready 상태가 보여지며, 하단에 Animation이 있다.
//*********************************************************************/

#pragma once

#define WIID_PVP_LOADWAIT_BG			(WIID_PVP_LOADWAIT + 1)			// Max PVP_MAX_ROOMUSER 8
#define WIID_PVP_LOADWAIT_PLAYER		(WIID_PVP_LOADWAIT + 2)			// Max PVP_MAX_ROOMUSER 8
#define WIID_PVP_LOADWAIT_READY			(WIID_PVP_LOADWAIT + 12)		// Max PVP_MAX_ROOMUSER 8

#define PVP_LOADWAIT_COLOR				RGBA(187, 134, 123, 255)
#define PVP_LOADCOMPLETE_COLOR			RGBA( 84,  69,  44, 255)


class SPWindowPvpLoadWait : public SPWindow {
public:	
	SPWindowPvpLoadWait(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPvpLoadWait();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	//void SetTestData(int iCount) ;
	void SetLoadBGTexture(const char* strFileName) ;

protected:
	//void SetWaitUser(int iUserCount, const char* pUserList);
	void InitSubControl();

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()	
	int OnUpdatePVPWait		( WPARAM wParam, LPARAM lParam );
	int OnClose				( WPARAM wParam, LPARAM lParam );
	//}}

private:
	SPTexture*		m_pTextureBG;
	SPTexture*		m_pTextureWnd;
	SPTexture*		m_pTextureText ;
	RECT			m_rtSrcBG;
	RECT			m_rtDstBG;

	int				m_iAlphaInc;		// 증감값
	int				m_iAlpha;
	float			m_fAniRender;
	float			m_fAniDelay;
	RECT			m_rtAniSrc;
	RECT			m_rtAniDest;

	RECT		m_rtSrc1;		//[2005/3/21] /**< 이미지 1 번 <br> */
	RECT		m_rtSrc2;		//[2005/3/21] /**< 이미지 2 번 <br> */
	RECT		m_rtSrc3;		//[2005/3/21] /**< 이미지 3 번 왼쪽 바닥 <br> */
	RECT		m_rtSrc4;		//[2005/3/21] /**< 이미지 4 번 바닥 <br> */
	RECT		m_rtSrc5;		//[2005/3/21] /**< 이미지 5 번 바닥 <br> */
	RECT		m_rtSrc6;		//[2005/3/21] /**< 이미지 6 번 바닥 <br> */

	RECT		m_rtDest1;		//[2005/3/21] /**< 이미지 1 번 <br> */
	RECT		m_rtDest2;		//[2005/3/21] /**< 이미지 2 번 <br> */
	RECT		m_rtDest3;		//[2005/3/21] /**< 이미지 3 번 왼쪽 바닥 <br> */
	RECT		m_rtDest4;		//[2005/3/21] /**< 이미지 4 번 바닥 <br> */
	RECT		m_rtDest5;		//[2005/3/21] /**< 이미지 5 번 바닥 <br> */
	RECT		m_rtDest6;		//[2005/3/21] /**< 이미지 6 번 바닥 <br> */
};