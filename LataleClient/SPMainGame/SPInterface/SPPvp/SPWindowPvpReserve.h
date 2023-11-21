#pragma once 


#define WIID_PVPRESERVE_EXIT 					(WIID_PVPRESERVE + 1)
#define WIID_PVPRESERVE_TITLE					(WIID_PVPRESERVE + 2)
#define WIID_PVPRESERVE_LINE1					(WIID_PVPRESERVE + 3)

#define WIID_PVPRESERVE_SKIN					(WIID_PVPRESERVE + 10)
#define WIID_PVPRESERVE_HELP					(WIID_PVPRESERVE + 20)
#define WIID_PVPRESERVE_LIST_SKIN				(WIID_PVPRESERVE + 30)		//9번까지
#define WIID_PVPRESERVE_LIST_CHECK				(WIID_PVPRESERVE + 40)		//20개 여유
#define WIID_PVPRESERVE_LIST_TEXT				(WIID_PVPRESERVE + 60)		//20개 여유
#define WIID_PVPRESERVE_SELECT_ALL				(WIID_PVPRESERVE + 80)

#define WIID_PVPRESERVE_WARNING					(WIID_PVPRESERVE + 90)		//신청시 경고 메시지
#define WIID_PVPRESERVE_OK						(WIID_PVPRESERVE + 91)
#define WIID_PVPRESERVE_CANCEL					(WIID_PVPRESERVE + 92)




enum PVP_CHECK_TYPE {
	PVP_CHECK_TYPE_FFA_2,						/**< Free For All 2인 <br> */
	PVP_CHECK_TYPE_FFA_4,						/**< Free For All 4인 <br> */
	PVP_CHECK_TYPE_FFA_6,						/**< Free For All 6인 <br> */
	PVP_CHECK_TYPE_FFA_8,						/**< Free For All 8인 <br> */
	//PVP_CHECK_TYPE_ALL,							/**< 모두 선택 <br> */
	PVP_CHECK_TYPE_MAX,
};


/**
* @class SPWindowPvpReserve
* @brief PVP예약 
* @author Jinhee
* @version 00.00.01
* @date 2005.5.
* @bug 없음
* @warning 
*/
class SPWindowPvpReserve : public SPWindow {
public:	
	SPWindowPvpReserve(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPvpReserve();

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

	inline void SetCheckButton(SPWindowButton* pButton = NULL);

	/**
	 *
	 @param 
	 @return  
	*/
	void UpdatePvpScale();

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnExit				(WPARAM wParam, LPARAM lParam);						/**< 종료 버튼 <br> */		
	int OnOK				(WPARAM wParam, LPARAM lParam);
	int OnCancel			(WPARAM wParam, LPARAM lParam);
	int OnSelectAll			(WPARAM wParam, LPARAM lParam);	
	int OnSelect			(unsigned int uiID, WPARAM wParam, LPARAM lParam);
	//}}

private:
	SPTexture*		m_pTextureBase;									/**< 윈도우 바탕용 리소스 <br> */
	RECT			m_rtSrcBase[WINDOW_BASE_MAX];				/**< 윈도우 바탕 소스 영역 <br> */
	RECT			m_rtDestBase[WINDOW_BASE_MAX];				/**< 윈도우 바탕 대상 <br> */	
	
	SPWindow*		m_pWindowListSkin[WINDOW_BASE_MAX];			/**<  <br> */	
	
	SPWindow*		m_pWindowCheck[PVP_CHECK_TYPE_MAX];
	SPWindow*		m_pWindowText[PVP_CHECK_TYPE_MAX];

	UINT32			m_iPvpScale;
};