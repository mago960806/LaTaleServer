// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowPvpTimmer.h
//	created:	2007/11/14   Time:15:29
//	purpose:	
//*********************************************************************/

#pragma once

#define WIID_PVP_TIMMER_BG				(WIID_PVP_TIMMER + 1)
#define WIID_PVP_TIMMER_TITLE			(WIID_PVP_TIMMER + 2)
#define WIID_PVP_TIMMER_COLON			(WIID_PVP_TIMMER + 3)
#define WIID_PVP_TIMMER_MINUTE1			(WIID_PVP_TIMMER + 4)
#define WIID_PVP_TIMMER_MINUTE2			(WIID_PVP_TIMMER + 5)
#define WIID_PVP_TIMMER_SECOND1			(WIID_PVP_TIMMER + 6)
#define WIID_PVP_TIMMER_SECOND2			(WIID_PVP_TIMMER + 7)


#define MAX_PVP_TIME_VALUE				3600		// 60분까지 표시 가능
#define PVP_START_FX_EFFECT				15234
#define PVP_START_SOUND_EFFECT			884
#define PVP_WARN_SOUND_EFFECT			887


class SPTimer ;
class SPFXArchive;
class SPWindowPvpTimmer : public SPWindow {
public:	
	SPWindowPvpTimmer(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPvpTimmer();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	void SetTestData(int iSeconds, int iWarns);

protected:
	void SetTimerEffect(int iEffectID, int iSoundEffect) ;


protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()	
	int OnClose				( int iID, WPARAM wParam, LPARAM lParam );
	int OnSetPvpTime		(WPARAM wParam, LPARAM lParam );
	//}}

private:
	int		m_iSetTime ;		// 전체 시간
	int		m_iWarnTime ;		// 경고 시작 시간
	float	m_fElapsedTime ;	// 시작후 누적 시간
	float	m_fSecondTime ;		// 1초

	SPFXArchive*	m_pFXArchive;		// Start Effect
};