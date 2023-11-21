#pragma once


#define WIID_CASH_TRADE_WAIT_LEFT						(WIID_CASH_TRADE_WAIT	+	10 )
#define WIID_CASH_TRADE_WAIT_CENTER						(WIID_CASH_TRADE_WAIT	+	20 )
#define WIID_CASH_TRADE_WAIT_RIGHT						(WIID_CASH_TRADE_WAIT	+	30 )

#define WIID_CASH_TRADE_WAIT_CANCEL						(WIID_CASH_TRADE_WAIT	+	41 )

#define WIID_CASH_TRADE_WAIT_ITEMIMG					(WIID_CASH_TRADE_WAIT	+	50 )
#define WIID_CASH_TRADE_WAIT_CUSTOMERIMG				(WIID_CASH_TRADE_WAIT	+	60 )
#define WIID_CASH_TRADE_WAIT_HELP						(WIID_CASH_TRADE_WAIT	+	70 )


const int WAITING_FRAME = 14;					/**< 총 대상 영역 갯수 <br> */
const int WAITING_SRC = 10;						/**< 한 영역당 보여질 이미지 갯수 <br> */

/**
 * 한 영역이 현재 에니메이션 중인지 <br> 
*/
enum WAIT_ANI_STATUS {
	WAIT_ANI_STATUS_READY,						/**< 대기중 <br> */
	WAIT_ANI_STATUS_PLAY,						/**< 에니메이션 중 <br> */
	WAIT_ANI_STATUS_END,						/**< 10개의 이미지를 모두 플레이 <br> */
};


/**
 * 총 대상 영역 갯수 만큼 존재 <br> 
 * 각 영역 별 대기 애니메이션 정보를 기억 <br>
*/
struct SPWaitAniInfo {
	int				m_iSrcIndex;				/**< 현재 보여주는 이미지 인덱스 <br> */
	WAIT_ANI_STATUS m_iAniStatus;				/**< 현재 에니메이션 상태 <br> */

	SPWaitAniInfo() {
		Clear();
	}
	~SPWaitAniInfo() {
		Clear();
	}

	void Clear() {
		m_iSrcIndex			= 0;
		m_iAniStatus		= WAIT_ANI_STATUS_READY;
	}
};


class SPWindowCashTradeWait : public SPWindow {

public:	
	SPWindowCashTradeWait(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowCashTradeWait();
  
	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);


	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	void InitSubControl();

	/**
	 * 14개의 영역 대한 에니메이션 정보 초기화 <br> 
	 @param 
	 @return  
	*/
	void InitAnimationInfo();
	
	/**
	 * 에니메이션 초기화 <br>
	 @param 
	 @return  
	*/
	void ResetAnimation();

	/**
	 * 대기 에니메이션 프로세스 <br>
	 @param 
	 @return  
	*/
	void ProcessAnimation(float fTime);

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnCancel					(WPARAM wParam, LPARAM lParam);
	// }}

private:
	RECT				m_rtAniSrc[WAITING_SRC];
	RECT				m_rtAniDesc[WAITING_FRAME];
	
	SPWaitAniInfo		m_AniInfo[WAITING_FRAME];	
	
	float				m_fDeley;
	float				m_fAccumulate;

	SPTexture*			m_pAnimation;
};





