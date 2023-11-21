// Copyright (C) jinhee
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : jinhee 2005-04-14 오전 10:26:19
//***************************************************************************

#pragma once

// .NET2003 전용 해시맵 사용때문에.. 
#ifdef _DEFINE_DEPRECATED_HASH_CLASSES
#undef _DEFINE_DEPRECATED_HASH_CLASSES
#define _DEFINE_DEPRECATED_HASH_CLASSES 0	
#endif

#include <hash_map>

const int MINI_MAP_DEST_WIDTH = 185 - 1;
const int MINI_MAP_DEST_HEIGHT = 74;

const int MINIMAP_SCALE_X	=	16;
const int MINIMAP_SCALE_Y	=	16;

const int MINIMAP_OUTLINE	=	3;

class SPStage;
class SPMapGroup;
class SPWindow;
class SPWindowMiniMapIcon;
class SPWindowMiniMapToolTip;

struct SAVE_POINT
{
	UINT16	uiStageID;		
	UINT16	uiMapGroupID;		
	UINT16	uiMapID;			
	float	fPosX;
//	float	fPosY;
	BOOL	bCal;
};

typedef	std::multimap< GU_ID , SAVE_POINT >		STD_MULTIMAP_SAVEPOINT;
typedef std::vector< SAVE_POINT* >				STD_VECTOR_SAVEPOINT;

class SPWindowMiniMapView :	public SPWindow {
public:
	SPWindowMiniMapView(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent);
	~SPWindowMiniMapView();

	virtual void Init();
	virtual void Clean();	
	virtual void Process(float fTime);	
	virtual void Render(float fTime);	

	virtual bool SetImage(const char* szImage, int iSrcX=0, int iSrcY=0);
	virtual void SetImage(SPTexture* pImage, int iSrcX= 0, int iSrcY=0);

	/**
	 * 해당 그룹의 미니맵 이미지 들을 로딩한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void SetMiniMapImageList();			//[2005/4/14]

	/**
	 * 해당 그룹의 미니맵 이미지를 초기화 한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void ResetMiniMapImageList();		//[2005/4/14]
	
	/**
	 * 미니맵 이미지 출력을 위한 영역 계산 작업을 한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void SetRenderProcess();

	/**
	 * 미니맵 이미지 렌더 <br>
	 @param NONE
	 @return NONE
	*/
	bool RenderMiniMap();

	//	월드상에서의 로컬 플레이어를 기준한 미니맵 영역
	RECT GetMiniMapRect()	{	return m_rtMiniMap;	}

protected:

	SPIMESSAGE_OBJ_MAP_DECLARE()
	// {{ Message Map	
	int OnCursorEnter		( WPARAM, LPARAM );
	int OnCursorOut			( WPARAM, LPARAM );
	int OnSetImageIndex		( WPARAM wParam, LPARAM lParam);
	int OnRefreshMiniMapGOB	( WPARAM wParam, LPARAM lParam); //WPARAM엔 GOB 타입, LPARAM엔 GOB포인터
	int OnRefreshIconTeam	( WPARAM wParam, LPARAM lParam); //WPARAM엔 Add , Del 타입 , LPARAM엔 GU_ID
	int OnPurgeAll			( WPARAM wParam, LPARAM lParam );	// 채널 변경시 데이터 초기화 AJJIYA [8/3/2005]
	// }}

protected:

	void	IconClean();								//	Icon 지우기
	void	IconProcess( float fTime );					//	Icon 돌리기
	void	IconRender();								//	Icon 렌더
	void	IconRectInCheck( int iCurX , int iCurY );	//	Icon 영역내 마우스 위치 감지

	//	월드상의 맵에서 미니맵이 보여주는 부분을 계산한다.
	RECT	GetMiniMapRect		( int iLocalPlayerPosX , int iLocalPlayerPosY );

	//	SAVE_POINT을 위한 LDT 로딩
	void	InitSavePoint		();
	void	LoadSavePoint		();
	void	AddSavePoint		( SAVE_POINT& SavePointInfo );
	void	CleanSavePoint		();
	void	GetSavePoint		( int iStageID , int iMapGroupID );
	void	RenderSavePoint		( float fTime );

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	RECT							m_rtMiniMap;			//	월드상에서 현재 미니맵의 영역
	bool							m_bShowToolTip;			//	툴팁 표시 여부
	SPWindowMiniMapToolTip			m_ToolTip;
	
	SPWindowMiniMapIcon				m_LocalPlayer;			//	로컬 플레이어는 따로 둔다.
	
	stdext::hash_map< INSTANCE_ID , SPWindowMiniMapIcon*>	m_hmGOBIcon;	//	Instance_ID로 GOB를 보관해 두자.
	
	STD_MULTIMAP_SAVEPOINT									m_mmSavePoints;	//	스테이지와 맵그룹ID를 이용하여 저장한다.
	STD_VECTOR_SAVEPOINT									m_vSavePoints;	//	현재 렌더링 되어야할 세이브 포인트.
	SPWindow*												m_pSaveLine;
	SPWindow*												m_pSavePoint;
	SPWindow*												m_pSavePlayer;
	int														m_iMaxLine;
	int														m_iSavePointSizeX;
	int														m_iSaveLineOffSetX;
	int														m_iSaveLineOffSetY;


	struct IMAGE_ATTR {
		SPTexture*	pImage;
		POINT		SrcPoint;
	};
	std::vector<IMAGE_ATTR> m_vpImageAttr;

	std::vector<SPTexture*> m_vpImage; /**< MiniMap Image List <br> */

	int m_iCurImage;
	
	int m_iStage;					/**< Current Stage ID <br> */
	int m_iMapGroup;				/**< Current Map Group ID <br> */

	SPMapGroup* m_pMapGroup;		/**< Current Map Group <br> */
	int m_iFirstMapID;				/**< First Map Index <br> */
	int m_iSecondMapID;				/**< Second Map Index <br> */
	
	RECT m_rtDestRect;				/**< Dest MiniMap Rect <br> */

	RECT m_rtFirstSrc;				/**< First MiniMap Source Rect <br> */
	RECT m_rtSecondSrc;				/**< Second MiniMap Source Rect <br> */
	RECT m_rtFirstDest;				/**< First MiniMap Dest Rect <br> */
	RECT m_rtSecondDest;			/**< Second MiniMap Dest Rect <br> */
	
	int m_iPosX;					/**< Local Player Pos X <br> */
	int m_iPosY;					/**< Local Player Pos Y <br> */

	int m_iGroupWidth;				/**< map Group Width <br> */
	int m_iGroupHeight;				/**< map Group Height <br> */
	int m_iMapWidth;				/**< Map Widht <br> */
	int m_iMapHeight;				/**< Map Height <br> */

	SPTexture*		m_pImageOutLine;		// 미니맵 테두리 이미지 [1/30/2007 AJJIYA]
	RECT			m_rcDestOutLine[ MINIMAP_OUTLINE ];	// 미니맵 테두리 대상 영역 [1/30/2007 AJJIYA]
	RECT			m_rcSrcOutLine[ MINIMAP_OUTLINE ];	// 미니맵 테두리 소스 영역 [1/30/2007 AJJIYA]

};
