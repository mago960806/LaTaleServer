#pragma once


#define WIID_NPC_PORTAL_TITLE						(WIID_NPC_PORTAL + 1)
#define WIID_NPC_PORTAL_EXIT						(WIID_NPC_PORTAL + 5)
#define WIID_NPC_PORTAL_LINE						(WIID_NPC_PORTAL + 7)

#define WIID_NPC_PORTAL_BASE						(WIID_NPC_PORTAL + 9)

#define WIID_NPC_PORTAL_LIST_SKIN					(WIID_NPC_PORTAL + 10)
#define WIID_NPC_PORTAL_LIST_BUTTON					(WIID_NPC_PORTAL + 20)
#define WIID_NPC_PORTAL_LIST_ICON					(WIID_NPC_PORTAL + 30)
#define WIID_NPC_PORTAL_LIST_NAME					(WIID_NPC_PORTAL + 40)
#define WIID_NPC_PORTAL_LIST_ELY					(WIID_NPC_PORTAL + 50)
#define WIID_NPC_PORTAL_LIST_ELY_IMAGE				(WIID_NPC_PORTAL + 60)
#define WIID_NPC_PORTAL_LIST_ITEM_SKIN				(WIID_NPC_PORTAL + 70)
#define WIID_NPC_PORTAL_LIST_ITEM					(WIID_NPC_PORTAL + 80)
#define WIID_NPC_PORTAL_LIST_SELECT					(WIID_NPC_PORTAL + 90)

#define WIID_NPC_PORTAL_SLIDER						(WIID_NPC_PORTAL + 100)
#define WIID_NPC_PORTAL_UP							(WIID_NPC_PORTAL + 101)
#define WIID_NPC_PORTAL_DOWN						(WIID_NPC_PORTAL + 102)
#define WIID_NPC_PORTAL_MOVE						(WIID_NPC_PORTAL + 110)



const int PORTAL_PAGE			= 8;				/**< 한화면에 보이는 리스트이 갯수 <br> */

class SPWindow;
class SPWindowButton;
class SPWindowSlider;

//#include "SPQuestAttr.h"
#include "SPEventDEF.h"
//#include "SPEvent"
//struct SPRequire;
//struct SPNpcPortalInfo;


class SPWindowNpcPortal : public SPWindow {

public:
	SPWindowNpcPortal( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowNpcPortal();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

protected:
	/**
	 * 내부 컨트롤 생성 <br> 
	 @param 
	 @return  
	*/
	void InitSubControl();
	
	/**
	 * 리스트 초기화 <br> 
	 @param 
	 @return  
	*/
	void ResetControl();
	
	/**
	 * 주어진 인덱스의 리스트를 초기화 <br> 
	 @param 
	 @return  
	*/
	void ClearControl(int iIndex);
	
	/**
	 * 화면상의 리스트를 업데이트 <br> 
	 @param 
	 @return  
	*/
	void UpdateControl();
	
	/**
	 * 주어진 컨트롤에 포탈 정보의 아이콘을 설정 <br> 
	 @param 
	 @return  
	*/
	void SetDestIcon(SPNpcPortalInfo* pInfo, SPWindow* pWindow);	

protected:
	// {{ ShopTalk Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnExit				(WPARAM wParam, LPARAM lParam);						/**< 종료 버튼 <br> */
	int OnListUp			(WPARAM wParam, LPARAM lParam);						/**< 리스트 업 버튼 <br> */
	int OnListDown			(WPARAM wParam, LPARAM lParam);						/**< 리스트 다운 버튼 <br> */
	int OnSliderPageUp		(WPARAM wParam, LPARAM lParam);
	int OnSliderPageDown	(WPARAM wParam, LPARAM lParam);
	int OnSliderPageNum		(WPARAM wParam, LPARAM lParam);
	int OnPageUp			(unsigned int uiID, WPARAM wParam, LPARAM lParam);
	int OnPageDown			(unsigned int uiID, WPARAM wParam, LPARAM lParam);
	int OnPortalClick		(unsigned int uiID, WPARAM wParam, LPARAM lParam);
	int OnMove				(WPARAM wParam, LPARAM lParam);						/**< 이동 버튼 <br> */
	//}}


private:
	SPTexture*		m_pTextureBase;					/**< 윈도우 바탕용 리소스 <br> */
	RECT			m_rtSrcBase[WINDOW_BASE_MAX];	/**< 윈도우 바탕 소스 영역 <br> */
	RECT			m_rtDestBase[WINDOW_BASE_MAX];	/**< 윈도우 바탕 대상 <br> */	
	
	SPWindow*		m_pSelect[PORTAL_PAGE];			/**< 리스트 선택표현용 <br> */
	SPWindow*		m_pButton[PORTAL_PAGE];			/**< 리스트 이벤트 처리용 <br> */
	SPWindow*		m_pDestIcon[PORTAL_PAGE];		/**< 목적지 아이콘 <br> */
	SPWindow*		m_pName[PORTAL_PAGE];			/**< 목적지 이름 <br> */
	SPWindow*		m_pEly[PORTAL_PAGE];			/**< 이동 비용 <br> */
	SPWindow*		m_pElyImage[PORTAL_PAGE];		/**< 이동 비용뒤 엘릭 아이콘 이미지 <br> */
	SPWindow*		m_pItemIcon[PORTAL_PAGE];		/**< 아이템 아이콘 <br> */
	SPWindow*		m_pItemSkin[PORTAL_PAGE];		/**< 아이템 아이콘 스킨 <br> */
	SPWindowSlider* m_pSlider;

	int				m_iMaxPage;						/**< 최대 페이지 <br> */
	int				m_iCurPage;						/**< 현재 페이지 <br> */
	int				m_iSelect;						/**< 리스트 상 현재 선택위치 <br> */
	int				m_iSelectIndex;					/**< 데이터 상의 실 위치 <br> */
	
	SPTexture*		m_pDisable;						/**< Disable 표현용 Texture <br> */	
	RECT			m_rtMarkSrc[4];					/**< Icon Mark Src <br> */
	ICON_MARK		m_iMark[PORTAL_PAGE];			/**< Icon Mark <br> */
};