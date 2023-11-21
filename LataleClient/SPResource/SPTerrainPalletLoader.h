// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-29 오후 2:32:54 
//***************************************************************************


#pragma once

#include <string>

using namespace std;


//!!!/////////////////////////////////////////////////////////////////////////////////////////
//
// caution : certainly marking version changed date, to version control 
//
// Latest Version assigned 2004/03/31 by metalgeni.
//
const float CURRENT_TERRAIN_PALLET_VERSION	= 0.1f;
//
//////////////////////////////////////////////////////////////////////////////////////////////


namespace {

#define DECLARE_WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool _CASE_MAP_FUNCTION_ (string strParam);

#define WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool SPTerrainPalletLoader::_CASE_MAP_FUNCTION_ (string strParam) 


#define CHECK_BLOCK_INVALID_FIRST() \
	if( m_iDeep != 0 ) { \
		return false; \
	}

#define EXCUTE_WORDCASE_MAP(_CASE_, _CASE_MAP_FUNCTION_) \
	if ( true == isCase( _CASE_ ,  str.c_str() ) )  { \
		if(_CASE_MAP_FUNCTION_(str)) \
			continue; \
		else \
			return false; \
	}


enum STATE_BLOCK { 
	STATE_BLOCK_NULL = 0,  
	STATE_BLOCK_BEGIN,  
	STATE_BLOCK_END,
};

enum BSTATE {
	BSTATE_NULL = 0,
	BSTATE_HEADER,
	BSTATE_FRAME_ANI,
};




}

struct CONVERT_STATIC_MODEL
{
	//int iInstance;
	INT64 iInstance;
	int iLightMap;

	char szImageName[64];

	void Clear()
	{
		iInstance = 0;
		iLightMap = 0;
		szImageName[0] = '\0';
	}

	CONVERT_STATIC_MODEL& operator = (const CONVERT_STATIC_MODEL& modelInfo)
	{
		iInstance = modelInfo.iInstance;
		iLightMap = modelInfo.iLightMap;
		strcpy(szImageName, modelInfo.szImageName);

		return *this;
	}
};

struct CONVERT_CIRCULAR_MODEL
{
	//int iInstance;
	INT64 iInstance;
	float fDelay;
	float fRadian;
	int iLightMap;

	char szImageName[64];

	void Clear()
	{
		iInstance = 0;
		fDelay = 0.0f;
		fRadian = 0.0f;
		iLightMap = 0;

		szImageName[0] = '\0';
	}

	CONVERT_CIRCULAR_MODEL& operator = (const CONVERT_CIRCULAR_MODEL& modelInfo)
	{
		iInstance = modelInfo.iInstance;
		fDelay = modelInfo.fDelay;
		fRadian = modelInfo.fRadian;
		iLightMap = modelInfo.iLightMap;

		strcpy(szImageName, modelInfo.szImageName);

		return *this;
	}
};

struct CONVERT_FRAME_ANIMATION
{
	//int iInstance;
	INT64 iInstance;
	float fDelay;
	float fMinStart;
	float fMaxStart;

	int iLightMap;
	
	int iSyncTile;

	int iAlpha;

	char szImageName[64];

	void Clear()
	{
		iInstance = 0;
		fDelay = 0.0f;
		fMinStart = 0.0f;
		fMaxStart = 0.0f;

		iLightMap = 0;
		iSyncTile = 0;
		iAlpha = 0;

		szImageName[0] = '\0';
	}

	CONVERT_FRAME_ANIMATION& operator = (const CONVERT_FRAME_ANIMATION& aniInfo)
	{
		iInstance = aniInfo.iInstance;
		fDelay = aniInfo.fDelay;
		fMinStart = aniInfo.fMinStart;
		fMaxStart = aniInfo.fMaxStart;
		iLightMap = aniInfo.iLightMap;
		iSyncTile = aniInfo.iSyncTile;
		iAlpha = aniInfo.iAlpha;

		strcpy(szImageName, aniInfo.szImageName);
		return *this;
	}
};


class SPTerrainPalletLoader
{

public:
	SPTerrainPalletLoader();
	virtual ~SPTerrainPalletLoader();	
	bool LoadDataText(const char* szPalletFile, 
		std::vector<SPTerrainModel*>* pvpTerrainModel,
		string strPath = "", 		
		bool bGlobal = false,				//패스를 파라미터로 받을수 있도록 수정 jinhee - 04.10.15
		string strResPath = "");			//맵툴에서 사용할 리소스 패스 추가...
	bool LoadBinary(const char* szPalletFile, 
		std::vector<SPTerrainModel*>* pvpTerrainModel,
		string strPath = "", bool bGlobal = false);
 
protected:
	bool LoadImage(const char* pszPath, bool bGlobal = false);

	DECLARE_WORDCASE_MAP( OnComment				);	
	DECLARE_WORDCASE_MAP( OnBlockBegin			);
	DECLARE_WORDCASE_MAP( OnBlockEnd			);
	DECLARE_WORDCASE_MAP( OnTextHeader			);		
	//DECLARE_WORDCASE_MAP( OnListTerrainModel	);
	DECLARE_WORDCASE_MAP( OnStaticModel			);
	DECLARE_WORDCASE_MAP( OnFrameAnimation		);
	DECLARE_WORDCASE_MAP( OnCircularMotion		);
	DECLARE_WORDCASE_MAP( OnMember				);


protected:

	BSTATE							m_BS;
	int								m_iDeep;
	SPTexture*						m_pTempImage;
	std::vector<SPTerrainModel*>	m_vTerrainModel;
	SPTerrainModel*					m_pTerrainModel;
	bool							m_bGlobal;		//[2005/6/15] - Global Palette 인 경우 리소스 폴더 위치를 다르게 하기 위함
	std::string						m_strFilePath;	//[2005/8/29]

};














