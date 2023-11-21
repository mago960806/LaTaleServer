#pragma once


enum PAGE_SELECT {
	PAGE_SELECT_BUNDLE_PREV		= 0,		/**< 묶음 단위로 이전 <br> */
	PAGE_SELECT_PREV			= 1,		/**< 페이지 단위로 이전 <br> */
	PAGE_SELECT_NEXT			= 2,		/**< 페이지 단위로 다음 <br> */
	PAGE_SELECT_BUNDLE_NEXT		= 3,		/**< 묶음 단위로 다음 <br> */
	PAGE_SELECT_MAX				= 4,
};

const int PAGE_START			= 1;		/**< 컨트롤 페이지 버튼 ID <br> */
const int NUMBER_START			= 10;		/**< 컨트롤 넘버용 ID <br> */
const int MAX_BUNDLE			= 10;		/**< 번들내 페이지의 최대 갯수 <br> */

class SPWindowPageSelect :	public SPWindow
{
public:	
	SPWindowPageSelect(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent);
	~SPWindowPageSelect();

	virtual void Init();
	virtual void Clean();	
	virtual void Process(float fTime);	
	virtual void Render(float fTime);
	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = false);	
	
	/**
	 * 버튼 사이즈를 설정 <br>
	 @param PAGE_SELECT buttonType
	 @param int width
	 @param int height
	 @return NULL
	*/
	void SetPageButtonSize(PAGE_SELECT iButton, int iWidth, int iHeight);
	
	/**
	 * 숫자 사이즈를 설정 <br>
	 @param int width
	 @param int height
	 @return NULL
	*/
	void SetNumberSize(int iWidth, int iHeight);
	
	/**
	 * 숫자 컬러를 설정 <br>
	 @param D3DXCOLOR
	 @return NULL
	*/
	void SetNumberColor(D3DXCOLOR FontColor);
	
	/**
	 * 컨트롤간 간격 설정 <br>
	 @param int iInterval
	 @return NULL
	*/
	void SetControlInterval(int iInterval);
	
	/**
	 * 버튼 이미지 설정 <br>
	 @param PAGE_SELECT iButton
	 @param const char* szImage
	 @param int iSrcX
	 @param int iSrcY
	 @return NULL
	*/
	void SetPageButtonImage(PAGE_SELECT iButton, const char* szImage, int iSrcX, int iSrcY);
	
	/**
	* 버튼 이미지 설정 <br>
	@param PAGE_SELECT iButton
	@param const char* szImage
	@param int iSrcX
	@param int iSrcY
	@return NULL
	*/
	void SetPageButtonImageHit(PAGE_SELECT iButton, const char* szImage, int iSrcX, int iSrcY);
	
	/**
	* 버튼 이미지 설정 <br>
	@param PAGE_SELECT iButton
	@param const char* szImage
	@param int iSrcX
	@param int iSrcY
	@return NULL
	*/
	void SetPageButtonImagePush(PAGE_SELECT iButton, const char* szImage, int iSrcX, int iSrcY);
	
	/**
	* 버튼 이미지 설정 <br>
	@param PAGE_SELECT iButton
	@param const char* szImage
	@param int iSrcX
	@param int iSrcY
	@return NULL
	*/
	void SetPageButtonImageDisable(PAGE_SELECT iButton, const char* szImage, int iSrcX, int iSrcY);	

	/**
	* 최대 페이지와 화면에서 보여줄 페이지 단위 설정 <br>
	@param int iMaxPage
	@param int iBundle = MAX_BUNDLE
	@return NULL
	*/
	void SetMaxPageInfo(int iMaxPage, bool bInit = true, int iBundle = MAX_BUNDLE);

	/**
		해당 페이지가 선택된것으로 보여준다.
		@param int iPage
	 */
	void SetSelectPage(int iPage) ;
	

protected:
	/**
	 * 컨트롤 재배치를 한다 <br>
	 @param int iViewPage = 0
	 @return NULL
	*/
	void ArrangeControl(int iViewPage = 0);
	
	/**
	 * 컨트롤 내부 표현을 업데이트 한다 <br>
	 @param int iBundle = MAX_BUNDLE
	 @return NULL
	*/
	void UpdateContorl(int iBundle = MAX_BUNDLE);
	

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()
	virtual int OnPageClick		(unsigned int uiID, WPARAM wParam, LPARAM lParam);		/**< 페이지 관련 버튼 사용시 <br> */
	virtual int OnNumberClick	(unsigned int uiID, WPARAM wParam, LPARAM lParam);		/**< 숫자 관련 버튼 사용시 <br> */	


private:	
	SPWindowButton*			m_pPageMove[PAGE_SELECT_MAX];
	SPWindow*				m_pNumber[MAX_BUNDLE];

	bool					m_bShowDebugRect ;	
	D3DXCOLOR				m_SelectColor;
	RECT					m_rtSelect;
	int						m_iInterval;

	int						m_iMaxPage;				/**< 최대 페이지 <br> */
	int						m_iMaxBundle;			/**< 최대 묶음 <br> */
	int						m_iBundleUnit;			/**< 묶음 단위 <br> */
	int						m_iViewPage;			/**< 보여줄 페이지 갯수 <br> 보여줄 숫자 버튼의 갯수 <br> */

	int						m_iPage;				/**< 묶음 단위내 선택한 페이지 <br> */
	int						m_iBundle;				/**< 단위내 선택한 페이지 <br> */
	int						m_iSelectPage;			/**< 현재 페이지 <br> m_iBundle * m_iBundleUnit + m_iPage <br> */
	
};
