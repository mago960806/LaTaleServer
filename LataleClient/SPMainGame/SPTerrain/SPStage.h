
// Copyright (C) jinhee
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : jinhee 2005-1-7 오후 2:56:36
//***************************************************************************


#pragma once

#ifdef _LATALE_SERVER
	#include "GlobalDefines.h"
#endif

class SPTerrainAttribute;


////
//struct SPBGInfo {
//	int iBGID;						/**< BG Index <br> */
//	std::string strBGFile;			/**< BG File <br> */
//
//	SPBGInfo() {
//		SetNull();
//	}
//
//	~SPBGInfo() {
//	}
//
//	void SetNull() {
//		iBGID = -1;
//		strBGFile = "";
//	}
//
//	void operator=(const SPBGInfo Src) {
//		iBGID = Src.iBGID;
//		strBGFile = Src.strBGFile;
//	}
//};


//
struct SPMapInfo {					//jinee - [2004/12/30] //Test용
	int iMapID;						/**< Map Index <br> */
	int iBGIndex;					/**< 사용되는 BG Index <br> */

	//int iLeftMap;					/**< 왼쪽에 나오는 맵 <br> */
	//int iRightMap;					/**< 오른쪽에 나오는 맵 <br> */

	float fMapLeft;					/**< 그룹상의 맵 시작 위치 X <br> */
	float fMapTop;					/**< 그룹상의 맵 시작 위치 Y <br> */
	float fMapRight;			
	float fMapBottom;
	float fMapWidth;				/**< 맵 넓이 <br> */
	float fMapHeight;				/**< 맵 높이 <br> */

	std::string strMapName;			/**< 맵 이름 <br> */
	std::string strFormFile;		/**< Form File <br> */
	std::string strAttributeFile;	/**< Attribute File <br> */
	std::string strMiniMapFile;		/**< Mini Map File <br> */

	SPMapInfo() {
		SetNull();
	}

	~SPMapInfo()
	{
	}

	void SetNull() 
	{
		iMapID = -1;
		iBGIndex = -1;
		//iLeftMap = -1;
		//iRightMap = -1;
		
		fMapLeft = -1.0f;
		fMapTop = -1.0f;
		fMapRight = -1.0f;
		fMapBottom = -1.0f;
		fMapWidth = 0.0f;
		fMapHeight = 0.0f;

		strMapName = "";
		strFormFile = "";
		strAttributeFile = "";
		strMiniMapFile = "";
	}	
	
	/**
	 * 한개의 맵이 그룹상에서 차지하는 영역을 설정한다 <br> 
	 @param float fMapX
	 @param float fMapY
	 @param float fWidth
	 @param float fHeight
	 @return NULL
	*/
	void SetMapRect(float fMapX, float fMapY, float fWidth, float fHeight)
	{
		fMapLeft = fMapX;
		fMapTop = fMapY;	
		fMapWidth = fWidth;
		fMapHeight = fHeight;

		fMapRight = fMapLeft + fWidth;
		fMapBottom = fMapTop + fHeight;
	}

	void operator=(const SPMapInfo Src)
	{
		iMapID				= Src.iMapID;
		iBGIndex			= Src.iBGIndex;
		//iLeftMap			= Src.iLeftMap;
		//iRightMap			= Src.iRightMap;

		fMapLeft			= Src.fMapLeft;
		fMapTop				= Src.fMapTop;
		fMapRight			= Src.fMapRight;
		fMapBottom			= Src.fMapBottom;
		fMapWidth			= Src.fMapWidth;
		fMapHeight			= Src.fMapHeight;

		strMapName			= Src.strMapName;
		strFormFile			= Src.strFormFile;
		strAttributeFile	= Src.strAttributeFile;
		strMiniMapFile		= Src.strMiniMapFile;
	}
};


/**
 * 맵 연결 모드 <br>
*/
enum MAP_LINK {
	MAP_LINK_NULL = 0,						/**< NO LINK <br> */
	MAP_LINK_HORIZONTAL = 1,				/**< horizontal(width) <br> */
	MAP_LINK_VERTICAL = 2					/**< vertical(height) <br> */
};

/**
 * 그룹 타입 <br> 
*/
// 2006.05.19 dhpark 그룹타입을 중복설정가능하게 변경합니다.
const int GROUP_TYPE_FIELD		= 1<<0;	// 1
const int GROUP_TYPE_MINIMAP	= 1<<1;	// 2
const int GROUP_TYPE_EVENT		= 1<<2;	// 4
const int GROUP_TYPE_MARKET		= 1<<3;	// 8
const int GROUP_TYPE_EXP		= 1<<4;	// 16
const int GROUP_TYPE_PVP		= 1<<5;	// 32
const int GROUP_TYPE_CASH		= 1<<7; // 128
const int GROUP_TYPE_INDUN		= 1<<8; // 256

//enum GROUP_TYPE {
//	GROUP_TYPE_HOUSE			= 0,		/**< 건물 내부 <br> */
//	//GROUP_TYPE_VILLAGE = 10,				/**< 일반 마을 <br> */
//	//GROUP_TYPE_ELIAS = 100,				/**< BELOS 마을 <br> */
//	//GROUP_TYPE_DUNGUEN = 200,				/**< 던전 <br> */
//	GROUP_TYPE_PVP_ENABLE		= 1000,		/**< PVP 가능 지역인지 <br> */
//	GROUP_TYPE_EVENT			= 2000,
//};

const int MAX_MAP = 255;


/**
* @class SPMapGroup
* @brief 맵군(서버에서는 이 그룹을 하나의 맵으로 인식한다)
* @author Jinhee
* @version 00.00.01
* @date 2005.1.13
* @bug 없음
* @warning 없음
*/
class SPMapGroup {
public:
	SPMapGroup() {
		SetNull();
	}

	~SPMapGroup() {
		DeleteAllMapInfo();
	}
	
	/**
	 * MapGroup의 MapInfoList에 MapInfo를 추가한다. <br>
	 @param SPMapInfo* pMapInfo = NULL
	 @return bool
	*/
	bool AddMapInfo(SPMapInfo* pMapInfo = NULL) {		// 백터사용
		m_vpMapInfo.push_back(pMapInfo);		
		return true;
	}
	
	//
	//bool AddMapInfo(SPMapInfo pMapInfo) {				// 배열사용
	//	//m_vpMapInfo.push_back(pMapInfo);		
	//	m_pMapInfo[m_iMapCount] = pMapInfo;
	//	return true;
	//}
	
	/**
	 * Group좌표 위치상의 맵 ID 를 얻어온다 <br>
	 @param float fPointX = 0.0f	계산할 X좌표
	 @param float fPointY = 0.0f	계산할 Y좌표
	 @return int	찾아낸 맵의 ID
	*/
	int FindMapID(float fPointX = 0.0f, float fPointY = 0.0f);	
	
	/**
	 * 그룹내 첫번째 맵의 리스트상 맵 ID를 얻어온다 <br> 
	 @param NONE
	 @return int
	*/
	int GetFirstMapID();
	
	/**
	 * 그룹내 맵의 갯수를 얻어온다 <br>
	 @param NONE
	 @return int
	*/
	int GetGroupSize();

	/**
	 * Group의 MapList에서 좌표에 해당하는 MapInfo 데이터의 인덱스를 얻어온다 <br>
	 @param float fPointX = 0.0f	계산할 X좌표
	 @param float fPointY = 0.0f	계산할 Y좌표
	 @return int	찾아낸 맵의 ID 
	*/
	int FindMap(float fPointX = 0.0f, float fPointY = 0.0f);

	SPMapInfo* FindMapInfo(float fPosX = 0.0f, float fPosY = 0.0f);

	/**
	 * MapGroup의 좌표를 해당맵의 좌표로 변환한다 <br> 
	 @param float fPointX	계산할 X좌표
	 @param float fPointY	계산할 Y좌표
	 @param int iMapID = -1	이미 현재 맵을 계산해놓은 경우 사용
	 @return float 현재맵의 위치시작으로 보정된 X좌표
	*/
	float GetMapPosX(float fPointX, float fPointY, int iMapID = -1);
	
	/**
	 * MapGroup의 좌표를 해당맵의 좌표로 변환한다 <br> 
	 @param float fPointX	계산할 X좌표
	 @param float fPointY	계산할 Y좌표
	 @param int iMapID = -1	이미 현재 맵을 계산해놓은 경우 사용
	 @return float 현재맵의 위치시작으로 보정된 Y좌표
	*/
	float GetMapPosY(float fPointX, float fPointY, int iMapID = -1);

	void GetMapPos(float fPosX, float fPosY,
		float& fMapOffsetX, float& fMapOffsetY);

	/**
	 * MapIndex와 현재맵의 좌표를 이용하여 맵그룹의 좌표로 변환한다 <br> 
	 @param int iMapID 현 맵그룹상에서의 맵 인덱스
	 @param float fPointX 현 맵상에서의 X좌표
	 @return float	맵 그룹상에서의 X좌표
	*/
	float GetGroupPosX(int iMapID, float fPointX);
	
	/**
	* MapIndex와 현재맵의 좌표를 이용하여 맵그룹의 좌표로 변환한다 <br> 
	@param int iMapID 현 맵그룹상에서의 맵 인덱스
	@param float fPointY 현 맵상에서의 Y좌표
	@return float	맵 그룹상에서의 Y좌표
	*/
	float GetGroupPosY(int iMapID, float fPointY);
	
	/**
	 * 그룹타입을 가져온다 <br> 
	 @param NONE
	 @return int GROUP_TYPE
	*/
	int GetType() {return m_iType;}	
	// 2006.05.19 dhpark 맵의 맵그룹타입을 사용하기 위한 함수
	bool IsMapGroupType(int iMapGroupType) { return (m_iType & iMapGroupType) ? true : false; }

protected:
	/**
	 * MapGroup의 Mamber를 초기화 한다 <br>
	 @param NONE
	 @return NONE
	*/
	void SetNull() {
		m_iGroupID = -1;
		m_iMapLink = MAP_LINK_NULL;
		m_iMapCount = 0;
		m_lGroupWidth = 0;
		m_lGroupHeight = 0;
		m_iSoundEffectType = 0;
		m_iMiniMapIconID = 0;
		m_iMiniMapResID = 0;
		m_strGroupName = "";
		m_strBGFile = "";
		m_strBGMFile = "";
		m_iBGID = -1;
		m_iType = 0;
		m_fVelocityX = 1.0f;
		m_fJumpSpeed = 1.0f;
		m_fHangingVelocity = 1.0f;
		m_fUpDownVelocity = 1.0f;
		m_fGravity = 1.0f;
		m_fMaxDropSpeed = 1.0f;
	}
	
	/**
	 * MapGroup의 MapList를 초기화한다 <br>
	 @param NONE
	 @return bool
	*/
	bool DeleteAllMapInfo() {
		//백터인 경우
		std::vector<SPMapInfo*>::iterator iter;
		for(iter = m_vpMapInfo.begin(); iter != m_vpMapInfo.end(); ++iter) {
			SAFE_DELETE(*iter);
		}
		m_vpMapInfo.clear();

		return true;
	}

public:
	int m_iGroupID;							/**< 맵그룹 ID <br> */
	int m_iMapCount;						/**< 포함하는 맵의 갯수 <br> */
	int m_iBGID;							/**< 사용하는 배경 ID <br> */
	int m_iType;							/**< 그룹 타입 <br> */
	
	MAP_LINK m_iMapLink;					/**< 맵 연결 상태 <br> */
	long m_lGroupWidth;						/**< 그룹 상태에서 맵 Width <br> */
	long m_lGroupHeight;					/**< 그룹 상태에서 맵 Height <br> */
	
	// 추가 예정
	int m_iSoundEffectType;					/**< 사운드 이펙트 Type <br> */	
	int m_iMiniMapResID;					/**< 미니맵 아이콘 리소스 인덱스 <br> */
	int m_iMiniMapIconID;					/**< 미니맵 아이콘 리소스 내 인덱스 <br> */	

	std::string m_strGroupName;				/**< 맵 그룹 명 <br> */
	std::string m_strBGFile;				/**< 사용하는 BG File <br> */

	std::string m_strBGMFile;				/**< 맵 그룹에 사용할 BGM File <br> */

	float	m_fVelocityX;					/**< X축 이동 <br> */
	float	m_fJumpSpeed;					/**< 점프 <br> */
	float	m_fHangingVelocity;				/**< 매달리기 이동 <br> */
	float	m_fUpDownVelocity;				/**< 오르내리기 <br> */
	float	m_fGravity;						/**< 중력 <br> */
	float	m_fMaxDropSpeed;				/**< 최대 낙하 값 <br> */
	
	std::vector<SPMapInfo*> m_vpMapInfo;	/**< 포함하는 맵 List 백터 <br> */	
	//SPMapInfo m_pMapInfo[MAX_MAP];		/**< 포함하는 맵 List 배열 <br> */
};


/**
* @class SPStage
* @brief 하나의 스테이지
* @author Jinhee
* @version 00.00.01
* @date 2005.1.7
* @bug 없음
* @warning 없음
*/
class SPStage {
public:
	SPStage();
	~SPStage();
	
	/**
	 * List 외 내부 맴버 데이터를 초기화한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void Init();
	
	/**
	 * List 맵버 데이터를 초기화한다 <br>
	 @param NONE
	 @return NONE
	*/
	void ResetStage();

	/**
	 * 해당 스테이지 내의 그룹 수를 가져온다 <br>
	 @param NONE
	 @return int GroupCount
	*/
	int GetGroupCount() { return m_iGroupCount; }
	
	/**
	 * Palette File 을 List 에 추가한다 <br>
	 @param std::string strPalette = ""
	 @return bool
	*/
	bool AddPaletteFile(std::string strPalette = "");	

	/**
	 * MapGroup을 추가한다 <br>
	 @param SPMapGroup* pMapGroup = NULL
	 @return bool
	*/
	bool AddMapGroup(SPMapGroup* pMapGroup = NULL);

	/**
	 * MapGroup을 리셋한다 <br>
	 @param NONE
	 @return bool
	*/
	bool ResetMapGroup();

	/**
	 * 핸재 맵 그룹 인덱스를 설정한다 <br>
	 @param int iGroupIndex = -1
	 @return NULL
	*/
	void SetCurMapGroupIndex(int iGroupIndex = -1);

	/**
	 * 현재 선택된 맵 그룹 인덱스를 반환한다 <br>
	 @param NONE
	 @return int
	*/
	int GetCurMapGroupIndex();

	/**
	 * 해당인덱스의 맵 그룹을 가져옵니다 <br>
	 @param int iGroupIndex = -1
	 @return SPMapGroup*
	*/
	SPMapGroup* GetMapGroup(int iGroupIndex = -1);

	/**
	 * 현재 좌표의 맵 어트리뷰트를 가져온다 <br> 
	 * 우선 접근만 하도록 되어있음 그냥 내부에서 계산할수 있지 않을까 하는...<br>
	 @param float fPointX	그룹상좌표
	 @param float fPointY	그룹상좌표
	 @param int iGroupID = -1
	 @param int iMapID = -1
	 @return SPTerrainAttribute*
	*/
	SPTerrainAttribute* GetCurTerrainAttribute(float fPointX, float fPointY,
		float& fMapOffsetX, float& fMapOffsetY, int iGroupID = -1, int iMapID = -1);


//protected:
public:
	int m_iStageID;							/**< 스테이지 인덱스 <br> */
	std::string m_strStageName;				/**< 스테이지 명 <br> */
	
	int m_iGroupCount;						/**< 그룹 수 <br> */	

	int m_iSyncRegionWidth;					/**< 동기화 영역 Width <br> */
	int m_iSyncRegionHeight;				/**< 동기화 영역 Height <br> */	

	long m_dwMapWidth;						/**< 그룹 넓이 <br> */
	long m_dwMapHeight;						/**< 그룹 높이 <br> */

	std::vector<std::string> m_vpPalette;	/**< 파레트File 리스트 <br> */

	std::vector<SPMapGroup*> m_vpMapGroup;	/**< MapGroup 리스트 <br> */
	
	std::vector<SPTerrainAttribute*> m_vpTerrainAttribute;

protected:
	int m_iCurMapGroupIndex;				/**< 현재 선택된 그룹의 인덱스 <br> */
};

