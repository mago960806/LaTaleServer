// Copyright (C) jinhee
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : jinhee 2004-12-30 오전 10:55:54 
//***************************************************************************

#pragma once

#include <string>
using namespace std;

//struct SPBGInfo;

struct SPMapInfo;
class SPMapGroup;
class SPStage;

const float CURRENT_STAGE_FORMAT_VERSION	= 0.4f;

namespace {

	enum STATE_BLOCK { 
		STATE_BLOCK_NULL = 0,  
		STATE_BLOCK_BEGIN,  
		STATE_BLOCK_END,
		STATE_BLOCK_INNER_BEGIN,
		STATE_BLOCK_INNER_END,
		STATE_BLOCK_INNER_INNER_BEGIN,
		STAGE_BLOCK_INNER_INNER_END
	};

//
#define EXCUTE_WORDCASE_MAP(_CASE_, _CASE_MAP_FUNCTION_) \
	if ( true == isCase( _CASE_ ,  str.c_str() ) )  { \
	if(_CASE_MAP_FUNCTION_(pStage, pMapGroup, pMapInfo, str)) \
	continue; \
		else \
		return false; \
	}

#define DECLARE_WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool _CASE_MAP_FUNCTION_ (SPStage*& pStage, SPMapGroup*& pMapGroup, SPMapInfo*& pMapInfo, string strParam);

#define WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool SPStageLoader::_CASE_MAP_FUNCTION_ (SPStage*& pStage, SPMapGroup*& pMapGroup, SPMapInfo*& pMapInfo, string strParam)	


//
#define CHECK_BLOCK_INVALID_OUT() \
	if( m_StateBlock != STATE_BLOCK_BEGIN ) { \
	return false; \
	}

#define CHECK_BLOCK_INVALID_IN() \
	if( m_StateBlock != STATE_BLOCK_NULL ) { \
	return false; \
	}

#define CHECK_BLOCK_INNER_INVALID_OUT() \
	if( m_StateBlock != STATE_BLOCK_INNER_BEGIN ) { \
	return false; \
	}

#define CHECK_BLOCK_INNER_INVALID_IN() \
	if( m_StateBlock != STATE_BLOCK_BEGIN ) { \
	return false; \
	}

#define CHECK_BLOCK_INNER_INNER_INVALID_IN() \
	if( m_StateBlock != STATE_BLOCK_INNER_BEGIN ) { \
	return false; \
	}

#define CHECK_BLOCK_INNER_INNER_INVALID_OUT() \
	if( m_StateBlock != STATE_BLOCK_INNER_INNER_BEGIN ) { \
	return false; \
	}
}

struct CONVERT_MAP_INFO
{
	int iBGIndex;

	char szName[64];
	char szFormFile[64];
	char szAttributeFile[64];
	char szMiniMapFile[64];

	void Clear()
	{
		iBGIndex = 0;

		szName[0] = '\0';
		szFormFile[0] = '\0';
		szAttributeFile[0] = '\0';
		szMiniMapFile[0] = '\0';
	}

	CONVERT_MAP_INFO& operator = (const CONVERT_MAP_INFO& mapInfo)
	{
		iBGIndex = mapInfo.iBGIndex;

		strcpy(szName, mapInfo.szName);
		strcpy(szFormFile, mapInfo.szFormFile);
		strcpy(szAttributeFile, mapInfo.szAttributeFile);
		strcpy(szMiniMapFile, mapInfo.szMiniMapFile);

		return *this;
	}
};

struct CONVERT_GROUP_INFO
{
	int iID;
	int iMapLink;
	int iBGID;
	int iType;

	char szName[64];
	char szBGFile[64];
	char szBGMFile[64];

	std::vector< CONVERT_MAP_INFO > vMapList;

	void Clear()
	{
		iID = 0;
		iMapLink = 0;
		iBGID = 0;
		iType = 0;

		szName[0] = '\0';
		szBGFile[0] = '\0';
		szBGMFile[0] = '\0';							//[2005/6/10]

		vMapList.clear();
	}

	CONVERT_GROUP_INFO& operator = (const CONVERT_GROUP_INFO& groupInfo)
	{
		iID = groupInfo.iID;
		iMapLink = groupInfo.iMapLink;
		iBGID = groupInfo.iBGID;
		iType = groupInfo.iType;

		strcpy(szName, groupInfo.szName);
		strcpy(szBGFile, groupInfo.szBGFile);
		strcpy(szBGMFile, groupInfo.szBGMFile);			//[2005/6/10]

		vMapList = groupInfo.vMapList;
		return *this;
	}
};

struct CONVERT_STAGE_INFO
{
	int iID;
	int iRegionWidth;
	int iRegionHeight;

	char szName[64];
	char szPaletteFile[64];

	std::vector< CONVERT_GROUP_INFO > vGroupList;

	void Clear()
	{
		iID = 0;
		iRegionWidth = 0;
		iRegionHeight = 0;

		szName[0] = '\0';
		szPaletteFile[0] = '\0';

		vGroupList.clear();
	}

	CONVERT_STAGE_INFO& operator = (const CONVERT_STAGE_INFO& stageInfo)
	{
		iID = stageInfo.iID;
		iRegionWidth = stageInfo.iRegionWidth;
		iRegionHeight = stageInfo.iRegionHeight;

		strcpy(szName, stageInfo.szName);
		strcpy(szPaletteFile, stageInfo.szPaletteFile);

		vGroupList = stageInfo.vGroupList;
		return *this;
	}
};


/**
* @class SPStageLoader
* @brief Stage File에서 Stage 데이터를 로드
* @author Jinhee
* @version 00.00.03
* @date 2004.12.30
* @bug 없음
* @warning 없음
*/
class SPStageLoader
{	
public:
	SPStageLoader();
	virtual ~SPStageLoader();
	
	/**
	 * Stage File 에서 Stage데이터를 로딩해낸다 <br> 
	 @param std::vector<SPStage*>* pvpBackStageInfo
	 @param string Path
	 @param string File
	 @return int
	*/
	int LoadStageDataText(std::vector<SPStage*>* pvpBackStageInfo,
		string strPath = "", string strFile = "");
	int LoadStageBinary(std::vector<SPStage*>* pvpBackStageInfo,
		string strPath = "", string strFile = "");
	
	/**
	 * 포함하는 Stage의 갯수를 가져온다 <br>
	 @param NONE
	 @return int
	*/
	int GetStageCount()	{ 
		return m_iStageCount; 
	}

	int GetMapCount() {
		return m_iMapCount;
	}

protected:
	DECLARE_WORDCASE_MAP( OnComment	);
	DECLARE_WORDCASE_MAP( OnBlockBegin );
	DECLARE_WORDCASE_MAP( OnBlockEnd );
	
	DECLARE_WORDCASE_MAP( OnTextHeader );
	DECLARE_WORDCASE_MAP( OnStageCount );
	
	DECLARE_WORDCASE_MAP( OnStage );
	DECLARE_WORDCASE_MAP( OnStageID );
	DECLARE_WORDCASE_MAP( OnStageName );
	//DECLARE_WORDCASE_MAP( OnGroupCount );
	DECLARE_WORDCASE_MAP( OnSyncRegionWidth );		/**< 브로드캐스팅 영역 Widht <br> */
	DECLARE_WORDCASE_MAP( OnSyncRegionHeight );		/**< 브로드캐스팅 영역 Height <br> */
	DECLARE_WORDCASE_MAP( OnPalette );

	DECLARE_WORDCASE_MAP( OnGroup );
	DECLARE_WORDCASE_MAP( OnGroupID );
	DECLARE_WORDCASE_MAP( OnGroupName );
	DECLARE_WORDCASE_MAP( OnMapLink );				/**< 맵 링크 모드 <br> */
	//DECLARE_WORDCASE_MAP( OnMapCount );
	DECLARE_WORDCASE_MAP( OnBGID );
	DECLARE_WORDCASE_MAP( OnType );					/**< 그룹 타입 <br> */
	DECLARE_WORDCASE_MAP( OnBGFile );
	DECLARE_WORDCASE_MAP( OnBGMFile );				/**< 그룹 배경음악 파일 <br> */
	DECLARE_WORDCASE_MAP( OnSoundEffect );			/**< 그룹 사운드 이펙트 타입 <br> */
	DECLARE_WORDCASE_MAP( OnMiniMapResID );				// AJJIYA [5/16/2005]
	DECLARE_WORDCASE_MAP( OnMiniMapIconID );			// AJJIYA [5/16/2005]		
	DECLARE_WORDCASE_MAP( OnGravity );
	DECLARE_WORDCASE_MAP( OnMaxDrop );
	DECLARE_WORDCASE_MAP( OnMaxVelocity );
	DECLARE_WORDCASE_MAP( OnJumpSpeed );
	DECLARE_WORDCASE_MAP( OnUpDownVelocity );
	DECLARE_WORDCASE_MAP( OnHangingVelocity );

	DECLARE_WORDCASE_MAP( OnMap	);
	DECLARE_WORDCASE_MAP( OnMapID );	
	DECLARE_WORDCASE_MAP( OnMapName );
	DECLARE_WORDCASE_MAP( OnBGIndex );
	DECLARE_WORDCASE_MAP( OnLeftMap );
	DECLARE_WORDCASE_MAP( OnRightMap );
	DECLARE_WORDCASE_MAP( OnFormFile );
	DECLARE_WORDCASE_MAP( OnAttributeFile );
	DECLARE_WORDCASE_MAP( OnMiniMapFile );

protected:
	STATE_BLOCK		m_StateBlock;				/**< Block 구분자 <br> */
	int m_iStageCount;							/**< 스테이지의 갯수 <br> */
	int m_iMapCount;							/**< 전채 맵의 갯수 <br> */
	std::vector<SPStage*> m_vpBackStageInfo;	/**< Stage 데이터를 기억할 임시 List <br> */
};