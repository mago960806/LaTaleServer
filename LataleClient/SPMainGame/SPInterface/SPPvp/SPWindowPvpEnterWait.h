#pragma once

#define WIID_PVPENTERWAIT_ANI						(WIID_PVPENTERWAIT + 10)
#define WIID_PVPENTERWAIT_HELP						(WIID_PVPENTERWAIT + 20)
#define WIID_PVPENTERWAIT_CANCEL					(WIID_PVPENTERWAIT + 100)

////
//enum PVPENTERWAIT_BASE {
//	PVPENTERWAIT_BASE_TOP_LEFT,
//	PVPENTERWAIT_BASE_TOP,
//	PVPENTERWAIT_BASE_TOP_RIGHT,
//	PVPENTERWAIT_BASE_CENTER_LEFT,
//	PVPENTERWAIT_BASE_CENTER,
//	PVPENTERWAIT_BASE_CENTER_RIGHT,
//	PVPENTERWAIT_BASE_BOTTOM_LEFT,
//	PVPENTERWAIT_BASE_BOTTOM,
//	PVPENTERWAIT_BASE_BOTTOM_RIGHT,
//	PVPENTERWAIT_BASE_MAX,
//};


const int MAX_WAIT_ANI_FRAME			= 6;
//const int PVP_ENTERWAIT_SOUND_EFFECT	= 883;


/**
* @class SPWindowPvpReserve
* @brief PVP예약 
* @author Jinhee
* @version 00.00.01
* @date 2005.5.
* @bug 없음
* @warning 
*/
class SPWindowPvpEnterWait : public SPWindow {
public:	
	SPWindowPvpEnterWait(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPvpEnterWait();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	/**
	* 내부 컨트롤 생성 <br> 
	@param 
	@return  
	*/
	void InitSubControl();
	/**
	* Show때 Sound <br> 
	@param int iSoundID		 매칭 사운드 이펙트 ID
	@return  
	*/
	//void WaitEffectSound(int iSoundID);	// >> SPzone 에서 매칭 사운드로 변경

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()		
	int OnPvpRegist			(WPARAM wParam, LPARAM lParam);			/**< 신청대기 상태로 전환 <br> */
	int OnPvpRegistCancel	(WPARAM wParam, LPARAM lParam);			/**< 신청 Cancel 응답 <br> 컷인 해제 <br> */
	int OnCancel			(WPARAM wParam, LPARAM lParam);			/**< 신청 Cancel 버튼 <br> */
	//}}

private:
	SPTexture*		m_pTextureBase;									/**< 윈도우 바탕용 리소스 <br> */
	//RECT			m_rtSrcBase[PVPENTERWAIT_BASE_MAX];				/**< 윈도우 바탕 소스 영역 <br> */
	//RECT			m_rtDestBase[PVPENTERWAIT_BASE_MAX];			/**< 윈도우 바탕 대상 <br> */
	SPWindow*		m_pWindowMsg;
	
	int				m_iAniFrame;									/**< 출력 플레임 <br> */
	float			m_fAniRender;									/**< 프레임 시간 계산 <br> */
	float			m_fAniDelay;									/**< 프레임 간격 시간 <br> */
	RECT			m_rtAniSrc[MAX_WAIT_ANI_FRAME];
	RECT			m_rtAniDest;	
};