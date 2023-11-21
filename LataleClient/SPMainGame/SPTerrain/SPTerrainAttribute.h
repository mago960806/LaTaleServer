// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPTerrainAttribute
// Comment     : 클라이언트, 서버, 맵툴에서 공용으로 사용.
//
// Creation    : metalgeni 2004-11-22 오전 10:26:19
//***************************************************************************

#pragma once


extern BG_ID;

#define MAX_ATTR_LAYER 3

#define TILE_WIDTH	32
#define TILE_HEIGHT	16

#define ATTR_PIXEL_TO_OFFSET_X(fPixelX)		(fPixelX/TILE_WIDTH)
#define ATTR_PIXEL_TO_OFFSET_Y(fPixelY)		(fPixelY/TILE_HEIGHT)

#define ATTR_OFFSET_TO_PIXEL_X(iOffsetX)	(iOffsetX*TILE_WIDTH)
#define ATTR_OFFSET_TO_PIXEL_Y(iOffsetY)	(iOffsetY*TILE_HEIGHT)


/**
 * 어트리뷰트 체크시 스위칭 관련 <br>
*/
enum POS_MOVE {
	POS_MOVE_NULL = 0,				/**< 스위칭 없음 <br> */
	POS_MOVE_UP = 1,				/**< 한블럭 위로 스위칭 <br> */
	POS_MOVE_DOWN = 10,				/**< 한블럭 아래로 스위칭 <br> */
	POS_MOVE_LEFT = 20,				/**< 한블럭 왼쪽으로 스위칭 <br> */
	POS_MOVE_RIGHT = 30,			/**< 한블럭 오른쪽으로 스위칭 <br> */
	POS_MOVE_CENTER = 40			/**< 블럭 X축 중앙으로 <br> */
};

////
//enum CLIMB {
//	CLIMB_NONE,
//	CLIMB_NONE_UP,
//	CLIMB_NONE_DOWN,
//	CLIMB_UP_DOWN
//};

class SPTerrainAttribute {

public:
	SPTerrainAttribute();
	virtual ~SPTerrainAttribute();

	bool Init();
	void Clean();		
	
	/**
	 * 주어진 grid Size에 의거 Attribute Layer들을 생성한다 <br>
	 @author Jinhee
	 @date 04.11.22 - jinhee
	 @param int Grid Width 
	 @param int Grid Height 
	 @return bool
	*/
	long Create(int iGridWidth, int iGridHeight);

	/**
	* Attribute 를 Load한다 <br>
	* Maptool에서 사용 <br>
	@author Jinhee
	@date 04.11.23 - jinhee
	@param string Path
	@param string File
	@param long GroupWidth		위치 보정
	@param long GroupHeight		위치 보정
	@return bool
	*/
	bool Load(std::string strPath = "", 
		std::string strFile = "",
		long lGroupWidth = 0, long lGroupHeight = 0);

	/**
	 * 해당 좌표에 위치한 Attribute Layer1내의 Grid Attribute값을 설정한다 <br>
	 @author Jinhee
	 @date 04.11.22 - jinhee
	 @param BYTE Attribute ID
	 @param float X
	 @param float Y
	 @return 
	*/
	long SetAttr1(BYTE bAttr = 0, float fX = 0.0f, float fY = 0.0f);
	
	/**
	* 해당 좌표에 위치한 Attribute Layer2내의 Grid Attribute값을 설정한다 <br>
	@author Jinhee
	@date 04.11.22 - jinhee
	@param BYTE Attribute ID
	@param float X
	@param float Y
	@return 
	*/
	long SetAttr2(BYTE bAttr = 0, float fX = 0.0f, float fY = 0.0f);

	/**
	* 해당 좌표에 위치한 Attribute Layer3내의 Grid Attribute값을 설정한다 <br>
	@author Jinhee
	@date 04.11.22 - jinhee
	@param BYTE Attribute ID
	@param float X
	@param float Y
	@return 
	*/
	long SetAttr3(BYTE bAttr = 0, float fX = 0.0f, float fY = 0.0f);




	/**
	 * 레이어1에서 해당 좌표에 위치하는 어트리뷰트값을 얻어온다 <br> 
	 @param int iLayer	해당 맵의 레이어(0Base)
	 @param float fX	해당 맵상의 원하는 X축 좌표
	 @param float fY	해당 맵상의 원하는 Y축 좌표
	 @return BYTE (0xFF인 경우 Fail)
	*/
	BYTE GetAttrLayer(int iLayer, float fX, float fY);

	/**
	 * OffSet을 이용하여 해당 위치의 어트리뷰트값을 얻어온다 <br>
	 @param int iLayer		해당 맵의 레이어(0Base)
	 @param int iOffSetX	해당 맵상의 X축 OffSet
	 @param int iOffSetY	해당 맵상의 Y축 OffSet
	 @return BYTE (0xFF인 경우 Fail)
	*/
	BYTE GetOffSetAttrLayer(int iLayer, int iOffSetX, int iOffSetY);
	
	
	/**
	 * map 의 Widht를 가져온다 <br>
	 @author Jinhee
	 @date 04.11.25 - jinhee
	 @param NONE
	 @return float
	*/
	int GetWidth() { return m_iLenX; }

	/**
	 * map의 Height를 가져온다 <br>
	 @author Jinhee
	 @date 04.11.25 - jinhee
	 @param NONE
	 @return float
	*/
	int GetHeight() { return m_iLenY; }
	
	// Inspect attribute size mismatch.
	//bool InspectTerrain();


	// 도트단위
	bool MoveDrop(float fX, float fY, BG_ID eLayer);
	char InspectStandLayer(int iX, int iY, BG_ID& eLayer, int& iRevision);

	void SetLength(float fLenX, float fLenY) 
	{
		m_iLenX = (int)fLenX;
		m_iLenY = (int)fLenY;
	}


	// 오프셋단위

	// 오프셋 좌표를 이용하여 애트리뷰트유닛을 검색하여 리턴한다.
	// 레이어가 0번일경우 모든레이어 검색을 하고 검색된 레이어를 리턴한다
	// 레이어 우선순위는 1번 (모든레이어가 다 존재할경우 1번을 리턴한다)
	bool GetAttrType(int iOffsetX, int iOffsetY, BYTE& cAttr, int& iLayer);	
	
	int GetPosCorrect(BYTE cAttr, int iX, int iY, bool& bReCalculate);

	bool EnableLadderUp(float fX, float fY);
	bool EnableLadderDown(float fX, float fY);
	bool EnableVerRopeUp(float fX, float fY);
	bool EnableVerRopeDown(float fX, float fY);
	bool EnableHozRopeUp(float fX, float fY);

	bool IsBlock(BYTE cAttr);
	bool IsVerRope(BYTE cAttr);
	bool IsLadder(BYTE cAttr);
	bool IsHozRope(BYTE cAttr);
	bool IsUpBlock(BYTE cAttr);
	bool IsGround(BYTE CAttr, int iX, int iY);

private:
	// "distance" is over through value. 
	//// return attrubute unit.
	//BYTE GetAttr1(float fx, float fy, float& fDistanceX, float& fDistanceY);
	//BYTE GetAttr2(float fx, float fy, float& fDistanceX, float& fDistanceY);
	//BYTE GetAttr3(float fx, float fy, float& fDistanceX, float& fDistanceY);	
	/**
	 * 해당 좌표상에 맞는 Attribute값을 얻어온다 <br> 
	 * 좌표 보정을 위한 순수하지 않은 Attribute값이 읽혀질수도 있다 <br>
	 * 그런 경우 GetAttrLayer1 ~ GetAttrLayer3 의 함수를 이용하여 읽어낼수 있다 <br>
	 @param int iLayer				해당 맵의 레이어(0Base)
	 @param float& fx				해당 맵에서의 x축 Offset
	 @param float& fy				해당 맵에서의 y축 Offset
	 @return BYTE					현재 좌표상의 Attribute값
	*/
	BYTE GetAttr(int iLayer, int iX, int iY);

	char CheckStandLayer(int iLayer, int iX, int iY,
		int& iRevision, bool& bReCalculate);
	
	/**
	 * 해당 어트리뷰트에서 X축 좌표를 기준으로 실제 위치해야 할 Y축 좌표를 계산해낸다. <br>
	 * jinhee - [2005/1/27] 
	 @param BYTE bAttr				검색할 Attribute값
	 @float fx						해당 어트리뷰트 셀의 원점에서 부터의 X축 OffSet
	 @float fy						해당 어트리뷰트 셀의 원점에서 부터의 Y축 OffSet
	 @param float fVelocityX		현재 X축으로의 방향성
	 @param	float fVelocityY		현재 Y축으로의 방향성
	 @return float					보정되어져야 할 Y축의 값 (이 값을 더하면 자동적으로 원하는 값이 된다)
	*/	

private:

	//float	m_fLenX;
	//float	m_fLenY;
	int m_iLenX;
	int m_iLenY;


	int m_iHalfTileX;
	int m_iHalfTileY;
	int m_iQuadTileX;
	int m_iQuadTileY;
	
	BYTE* m_pkAttribute[MAX_ATTR_LAYER];
};

//extern SPTerrainAttribute* g_pTerrainAttribute;


