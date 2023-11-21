// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-10 오전 10:26:19
//***************************************************************************

#pragma once


extern BG_ID;

class SPTerrainManager;
class SPTerrainModel;
class SPTerrainUnit;
class SPTerrainAttribute;

struct SPBGInfo;
struct SPMapInfo;
class SPStage;

class SPTerrainCluster {

public:
	SPTerrainCluster();
	~SPTerrainCluster();
	bool Init(SPTerrainManager* pParent);
	void Clean();

	void SetCameraOffset(float, float);

	void RenderLayer1(float fTime);
	void RenderLayer2(float fTime);
	void RenderLayer3(float fTime);

	void RenderSecondLayer1(float fTime);
	void RenderSecondLayer2(float fTime);
	void RenderSecondLayer3(float fTime);

	void RenderAttr();

	void Process(float fTime);

	bool MoveDrop(float& fx, float& fy, BG_ID& bgLayer, const float fDropStep);
	bool InspectStandLayer(float fx, float fy, BG_ID& bgLayer);

	// 순서대로 불려야 한다..!!
	bool LoadPallet( const char* pszFName);
	bool LoadForm( SPStage* pStage = NULL);			//jinhee - [2005/1/11]
	bool LoadAttribute( SPStage* pStage = NULL);	//jinhee - [2005/1/11]

	/**
	 * 현재 스테이지 내의 TerrainLayerList의 내부를 비운다 <br> 
	 @param 
	 @return 
	*/
	bool DeleteAllTerrainLayerList();				//jinhee - [2005/1/11]
	
	/**
	 * 현재 스테이지 내이 TerainAttributeList의 내부를 비운다 <br>
	 @param 
	 @return 
	*/
	bool DeleteAllAttributeLayerList();				//jinhee - [2005/1/11]

	/**
	* 스테이지 변경시 터레인 벡터에 접근할 인덱스를 -1로 셋팅 <br>
	@param 
	@return 
	*/
	void ResetStage() {
		m_iFirstMapID		= -1;
		m_iSecondMapID		= -1;
	}

	bool SetShareLayer();

		//[xialin 2008/09/18]add
	bool IsTerrainLayer2Back( int x, int y );

	//[xialin 2008/09/18]add
	bool IsTerrainLayer3Back( int x, int y );


private:
	std::vector<SPTerrainModel*>	m_vpTerrainLocalModel;	
	std::vector<SPTerrainModel*>	m_vpTerrainGlobalModel;	

	std::vector<std::vector<SPTerrainUnit*> >	m_vpTerrainLayer1List;	//jinee - [2005/1/10]
	std::vector<std::vector<SPTerrainUnit*> >	m_vpTerrainLayer2List;	//jinee - [2005/1/10]
	std::vector<std::vector<SPTerrainUnit*> >	m_vpTerrainLayer3List;	//jinee - [2005/1/10]

	std::vector<SPTerrainUnit*> m_vpShareTerrainLayer1;					//[2005/11/8] - 공유하는 터레인
	std::vector<SPTerrainUnit*> m_vpShareTerrainLayer2;					//[2005/11/8] 
	std::vector<SPTerrainUnit*> m_vpShareTerrainLayer3;					//[2005/11/8]


	int m_iFirstMapID;		/**< 첫번째 맵 ID Process 에서 계산해준다 <br> */
	int m_iSecondMapID;		/**< 두번째 맵 ID Process 에서 계산해준다 <br> */ 

	bool m_bShowSecondLayer;

	// Block Attribute
	SPTexture* m_pAttrImageLayer1;
	SPTexture* m_pAttrImageLayer2;

	float	m_fLenX;
	float	m_fLenY;
	int		m_iMapID;

	float	m_fCameraOffsetX;
	float	m_fCameraOffsetY;

	SPTerrainManager*	m_pParent;
	SPTerrainAttribute*	m_pTerrainAttribute;
	
	std::vector<SPTerrainAttribute*> m_vpTerrainAttributeList;			//jinhee - [2005/1/10]

	friend SPTerrainManager;

};




