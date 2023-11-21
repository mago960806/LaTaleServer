
// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-11-29 오후 2:56:36
//***************************************************************************


#pragma once

enum LOAD_STATE {

	LS_NULL				= 0,

	LS_STAGE_LOAD		= 1,		//jinhee - [2004/12/30] //Stage정보 로드

	LS_BGI_LOADER		= 2,
	LS_TERRAIN_PALLET	= 3,
	LS_TERRAIN_FORM		= 4,
	LS_TERRAIN_ATTR		= 5,	

	LS_RENDER			= 6,		//[2005/9/9] //중간에 렌더하는 타임...

	LS_CASH_CHECK		= 7,

	LS_THREAD_WAIT		= 10,

	LS_END			   = 99,
};

struct SPBGInfo;
struct SPMapInfo;
class SPStage;

class SPLDTManager;
class SPLDTFile;

const int MAX_IMAGE_LIST = 3;				/**< ImageInfo에 포함되는 정보의 갯수 <br> */

struct SPLoadImageInfo {
	std::string m_strImage;					/**< 이미지 리소스 <br> */
	RECT m_rtSrc[MAX_IMAGE_LIST];			/**< 이미지 소스 좌표 리스트 <br> */
	RECT m_rtDest[MAX_IMAGE_LIST];			/**< 이미지 출력 좌표 리스트 <br> */
	int	m_iShowCount;						/**< 사용하는 좌표의 갯수 <br> */

	SPLoadImageInfo() {
		Clear();
	}

	~SPLoadImageInfo() {
		Clear();
	}

	void Clear() {
		m_strImage = "";
		m_strImage.clear();
		m_iShowCount = 0;

		for(int i = 0; i < MAX_IMAGE_LIST ; i++) {
			SetRect(&m_rtSrc[i], -1, -1, -1, -1);
			SetRect(&m_rtDest[i], -1, -1, -1, -1);
		}
	}

	void SetInfoRect(int iIndex, RECT rtSrc, RECT rtDest) {
		SetRect(&m_rtSrc[iIndex], rtSrc.left, rtSrc.top, rtSrc.right, rtSrc.bottom);
		int iWidth = rtSrc.right - rtSrc.left;
		int iHeight = rtSrc.bottom - rtSrc.top;
		
		SetRect(&m_rtDest[iIndex], rtDest.left, rtDest.top, rtDest.left + iWidth, rtDest.top + iHeight);
		m_iShowCount = iIndex + 1;
	}
};


struct SPLoadInfo {
	std::string m_strBGImage;				/**< 바탕에 사용할 이미지 리소스 <br> */
	SPLoadImageInfo m_LoadImage[MAX_IMAGE_LIST];	/**< 부가적으로 사용할 이미지 정보 리스트 <br> */
	int m_iImageCount;						/**< 사용하는 이미지의 갯수 <br> */

	SPLoadInfo() {
		Clear();
	}

	~SPLoadInfo() {
		Clear();
	}

	void Clear() {
		m_strBGImage = "";
		m_strBGImage.clear();
		m_iImageCount = 0;

		for(int i = 0; i < MAX_IMAGE_LIST; i++) {
			m_LoadImage[i].Clear();
		}
	}

	void SetBGFile(std::string strFile) {
		Clear();
		m_strBGImage = RES_PAHT_LOADING + strFile;
	}

	void SetImageFile(int iImageIndex, std::string strFile) {
		m_LoadImage[iImageIndex].m_strImage = RES_PAHT_LOADING + strFile;
		m_iImageCount = iImageIndex + 1;
	}

	void SetImageRect(int iImageIndex, int iInfoIndex, RECT rtSrc, RECT rtDest) {
		m_LoadImage[iImageIndex].SetInfoRect(iInfoIndex, rtSrc, rtDest);
	}
};


class SPLoading : public SPSubGameManager {
public:
	SPLoading();
	virtual ~SPLoading();

	virtual void			Render(float fTime);
	virtual void			Process(float fTime);
	virtual bool			Init();	
	
	virtual void			RenderText();

	virtual int				SPPerformMessage	(UINT msg, WPARAM wparam = 0, LPARAM lParam = 0);

	//void SetStageID(int iStage)			{ m_iStageID = iStage; }
	//void SetMapGroupID(int iGroupID)	{ m_iMapGroupID = iGroupID; }
	
	SPStage*				GetStage(int iCurStage = 0);	//test // 안사용할듯.

protected:

	void		ProcessImage(float fTime);

	/**
	 * Loading 에 필요한 Data를 LDT에서 읽어 LoadInfo에 설정한다 <br>
	 @param  int iItemID 여기서는 안사용한다 (내부적으로 id 생성)
	 @return SPLoadInfo*
	*/
	SPLoadInfo* LoadLoadingDataLDT(int iItemID = 0);

	/**
	 * LoadInfo의 정보를 통해 필요한 리소스를 읽는다 <br>
	 @param NONE
	 @return int
	*/
	int			LoadLoadingImage();

	SPGMESSAGE_MAP_DECLARE()
	int			OnLoadSetStage				(WPARAM wparam, LPARAM lparam);
	int			OnLoadSetGroup				(WPARAM wparam, LPARAM lparam);
	int			OnSocketFailed				(WPARAM wparam, LPARAM lparam);
	//int		OnGOBEnter					(WPARAM wparam, LPARAM lparam);
	//SPGMESSAGE_COMMAND( SPGM_GOB_ENTER_EX			, OnGOBEnterEx	)

	void InitLoadingThread();
	friend DWORD WINAPI LoadingThread(LPVOID lParam);

protected:
	SPTexture*				m_pImageBack;
	SPTexture*				m_pImageGauge;
	SPTexture*				m_pImageWrap;

	LOAD_STATE				m_LoadState;
	LOAD_STATE				m_LoadStateNext;	//[2005/9/9] /**< 중간에 렌더를 위해 다음 로드할 상태를 보관한다 <br> */
	float					m_fLoadProtage;
	float					m_fProtage;			//[2005/9/9] /**< Render Protage <br> */

	float					m_fLastRenderDelay;	//[2005/9/13]/**< 각 LoadStatus 의 마지막 시간을 기억 <br> */
	float					m_fAccumulateRender;//[2005/9/9] /**< 렌더링 시간 계산 <br> */
	float					m_fCurDelay;		//[2005/9/9] /**< 프레임 간격 시간 <br> */
	
	RECT					m_RectGauge;
	RECT					m_RectBack;
	RECT					m_RectLog;

	int						m_iStageID;		//[2005/4/13] /**< Stage ID <br> */
	int						m_iMapGroupID;	//[2005/4/13] /**< Map Group ID <br> */
	
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

	SPLDTFile*	m_pLDTLoadData;	//[2005/9/14] /**< Load Image Data LDT <br> */
	SPLoadInfo*	m_pLoadInfo;	//[2005/9/14] /**< 로드 시 필요한 이미지 정보를 지닌.. <br> */
	SPTexture*	m_pImageLoad[MAX_IMAGE_LIST]; /**< 로딩에 필요한 서브 이미지 <br> */

	HANDLE m_hThreadHandle;
	volatile bool m_bSendKeepAlive;
	volatile bool m_bThreadExit;
};







