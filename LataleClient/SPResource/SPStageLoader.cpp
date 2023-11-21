#include <string>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <assert.h>
#include <string>

#ifdef _LATALE_CLIENT
	#include "SPCommon.h"
#elif _LATALE_SERVER
	#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif

#include "SPResourceDef.h"


#ifdef PLAN_TOOL
	#include "stdafx.h"
#else
	#include "SPMainGameDEF.h"
	#include "SPGameObjectDEF.h"
#endif

#ifdef _LATALE_CLIENT
	#include "SPManager.h"
#endif

#include "SPUtil.h"
#include "SPFileDEF.H"
#include "SPStream.h"

//#include "SPSubGameManager.h"
//#include "SPLoading.h"

#include "SPStage.h"
#include "SPStageLoader.h"

using namespace std;

#pragma warning(disable:4100)

SPStageLoader::SPStageLoader() {
	m_iStageCount = 0;
	m_iMapCount = 0;
}


SPStageLoader::~SPStageLoader() {
}


int SPStageLoader::LoadStageDataText(std::vector<SPStage*>* pvpBackStageInfo,
								 string strPath /*= ""*/,
								 string strFile /*= ""*/)
{
	m_iStageCount = 0;
	m_iMapCount = 0;
	
	std::string strResource;
	if("" == strPath) {
		//strResource = RES_WORK_RESOURCE_PATH;
		strResource = "";
	}
	else {
		strResource = strPath;
	}

	
	if("" == strFile) {
		//strResource += RES_FNAME_BGFORMAT;
		//strResource += ;
	}
	else {
		strResource += strFile;
	}

	SPStream* pStream;
#ifdef _LATALE_SERVER
	pStream = new SPFileStream( strResource.c_str() );	
#else
	SPResourceBase::GetInstance()->GetStreamData( strResource.c_str(), &pStream);
#endif
	if(pStream == NULL || !pStream->Valid())
	{
		// AJJIYA [5/24/2005]
		// 서버에서도 사용중 매크로가 없내용..
		// SAFE_RELEASE(pStream);
		if( pStream )
		{
			pStream->Release();
			pStream	=	NULL;
		}
		return 0;
	}

	//int iFileHandle = open(strResource.c_str(), O_RDONLY);
	//if (iFileHandle == -1) return 0;

	assert((*pvpBackStageInfo).empty());	

	string str;	

	//SPMapInfo* pMapInfo = NULL;
	SPStage* pStage = NULL;
	//SPBGInfo* pBGInfo = NULL;
	SPMapGroup* pMapGroup = NULL;
	SPMapInfo* pMapInfo = NULL;

	m_StateBlock	= STATE_BLOCK_NULL;	
	
	while ( ReadLineFromStream(pStream, str) != -1) {
		Trim(str);

		EXCUTE_WORDCASE_MAP ( COMMENT,	OnComment )
		EXCUTE_WORDCASE_MAP ( BLOCK_BEGIN,	OnBlockBegin )
		EXCUTE_WORDCASE_MAP ( BLOCK_END,	OnBlockEnd )

#ifdef _LATALE_CLIENT
		EXCUTE_WORDCASE_MAP ( "STAGE_HEADER", OnTextHeader )
		EXCUTE_WORDCASE_MAP ( "StageCount", OnStageCount )
		
		EXCUTE_WORDCASE_MAP ( "STAGE", OnStage )
		EXCUTE_WORDCASE_MAP ( "StageID", OnStageID )
		EXCUTE_WORDCASE_MAP ( "StageName", OnStageName )
		EXCUTE_WORDCASE_MAP ( "PaletteFile", OnPalette )

		EXCUTE_WORDCASE_MAP( "GROUP", OnGroup )
		EXCUTE_WORDCASE_MAP( "GroupID", OnGroupID )
		EXCUTE_WORDCASE_MAP( "GroupName", OnGroupName )
		EXCUTE_WORDCASE_MAP( "MapLink", OnMapLink )
		EXCUTE_WORDCASE_MAP( "BGID", OnBGID )
		EXCUTE_WORDCASE_MAP( "Type", OnType )
		EXCUTE_WORDCASE_MAP( "BGFile", OnBGFile )		
		EXCUTE_WORDCASE_MAP( "BGMFile", OnBGMFile )
		EXCUTE_WORDCASE_MAP( "SoundEffect", OnSoundEffect )
		EXCUTE_WORDCASE_MAP( "ThemeIcon", OnMiniMapResID )
		EXCUTE_WORDCASE_MAP( "ThemeIconIndex", OnMiniMapIconID )
		EXCUTE_WORDCASE_MAP( "Drop_Speed", OnGravity )
		EXCUTE_WORDCASE_MAP( "Max_Drop_Speed", OnMaxDrop )
		EXCUTE_WORDCASE_MAP( "Speed_X", OnMaxVelocity )
		EXCUTE_WORDCASE_MAP( "Speed_Y", OnJumpSpeed )
		EXCUTE_WORDCASE_MAP( "Rope_Speed_Y", OnUpDownVelocity )
		EXCUTE_WORDCASE_MAP( "Rope_Speed_X", OnHangingVelocity )
		
		EXCUTE_WORDCASE_MAP ( "MAP", OnMap )
		EXCUTE_WORDCASE_MAP ( "MapName", OnMapName )
		EXCUTE_WORDCASE_MAP ( "BGIndex", OnBGIndex )
		EXCUTE_WORDCASE_MAP ( "FormFile", OnFormFile )
		EXCUTE_WORDCASE_MAP ( "AttributeFile", OnAttributeFile )
		EXCUTE_WORDCASE_MAP ( "MiniMapFile", OnMiniMapFile)
#endif

#ifdef _LATALE_SERVER
		EXCUTE_WORDCASE_MAP ( "STAGE_HEADER", OnTextHeader )
		EXCUTE_WORDCASE_MAP ( "StageCount", OnStageCount )

		EXCUTE_WORDCASE_MAP ( "STAGE", OnStage );
		EXCUTE_WORDCASE_MAP ( "StageID", OnStageID )
		EXCUTE_WORDCASE_MAP ( "StageName", OnStageName )
		EXCUTE_WORDCASE_MAP ( "SynchRegionWidth", OnSyncRegionWidth )
		EXCUTE_WORDCASE_MAP ( "SynchRegionHeight", OnSyncRegionHeight )

		EXCUTE_WORDCASE_MAP( "GROUP", OnGroup )
		EXCUTE_WORDCASE_MAP( "GroupID", OnGroupID )
		EXCUTE_WORDCASE_MAP( "GroupName", OnGroupName )
		EXCUTE_WORDCASE_MAP( "MapLink", OnMapLink )
		EXCUTE_WORDCASE_MAP( "Type", OnType )
		EXCUTE_WORDCASE_MAP( "Drop_Speed", OnGravity )
		EXCUTE_WORDCASE_MAP( "Max_Drop_Speed", OnMaxDrop )
		EXCUTE_WORDCASE_MAP( "Speed_X", OnMaxVelocity )
		EXCUTE_WORDCASE_MAP( "Speed_Y", OnJumpSpeed )
		EXCUTE_WORDCASE_MAP( "Rope_Speed_Y", OnUpDownVelocity )
		EXCUTE_WORDCASE_MAP( "Rope_Speed_X", OnHangingVelocity )
		
		EXCUTE_WORDCASE_MAP ( "MAP", OnMap )
		EXCUTE_WORDCASE_MAP ( "MapName", OnMapName )
		EXCUTE_WORDCASE_MAP ( "AttributeFile", OnAttributeFile )
#endif
	}

	//close(iFileHandle);	
	pStream->Release();
	
	for(int i = 0; i < (signed)m_vpBackStageInfo.size(); ++i) {
		pvpBackStageInfo->push_back(NULL);		
	}

	std::copy( m_vpBackStageInfo.begin(),
			m_vpBackStageInfo.end(),
			(*pvpBackStageInfo).begin());

	m_vpBackStageInfo.clear();
	
	return 1;
}

int SPStageLoader::LoadStageBinary(std::vector<SPStage*>* pvpBackStageInfo,
	string strPath, string strFile)
{
	//離劤돨벴벌경굶 08/12
	m_iStageCount = 0;
	m_iMapCount = 0;
	(*pvpBackStageInfo).clear();

	std::string strResource;
	if("" == strPath) {
		//strResource = RES_WORK_RESOURCE_PATH;
		strResource = "";
	} else {
		strResource = strPath;
	}
	
	if("" == strFile) {
		//strResource += RES_FNAME_BGFORMAT;
		//strResource += ;
	}
	else {
		strResource += strFile;
	}

	//SPStream* pkStream = new SPFileStream(strResource.c_str());
	
	SPStream* pkStream;
#ifdef _LATALE_SERVER
	pkStream = new SPFileStream( strResource.c_str() );	
#else
	SPResourceBase::GetInstance()->GetStreamData( strResource.c_str(), &pkStream);
#endif
	if( pkStream == NULL || !pkStream->Valid() )
	{
		DWORD dwError = GetLastError();

		// AJJIYA [5/24/2005]
		// 서버에서도 사용중 매크로가 없내용..
		// SAFE_RELEASE(pkStream);
		if( pkStream )
		{
			pkStream->Release();
			pkStream	=	NULL;
		}
		return 0;
	}

	pkStream->Read(&m_iStageCount, sizeof(int));

	SPStage* pkStage;
	SPMapGroup* pkMapGroup;
	SPMapInfo* pkMapInfo;
	char szBuf[64];
	int i, j, k, iGroupCount, iMapCount;

	for( i = 0; i < m_iStageCount; i++ )
	{
		pkStage = new SPStage;
		m_iMapCount = 0;
		
		pkStream->Read(&pkStage->m_iStageID, sizeof(int));
		pkStream->Read(&pkStage->m_iSyncRegionWidth, sizeof(int));
		pkStream->Read(&pkStage->m_iSyncRegionHeight, sizeof(int));

		pkStream->Read(szBuf, sizeof(szBuf));
		pkStage->m_strStageName = szBuf;

		pkStream->Read(szBuf, sizeof(szBuf));

		//pkStage->AddPaletteFile(GetUpperString2(szBuf));

		string strTemp;
		GetUpperString3(strTemp, szBuf);
		pkStage->AddPaletteFile(strTemp);

		pkStream->Read(&iGroupCount, sizeof(int));
		for( j = 0; j < iGroupCount; j++ )
		{
			pkMapGroup = new SPMapGroup;
			pkStream->Read(&pkMapGroup->m_iGroupID, sizeof(int));
			pkStream->Read(&pkMapGroup->m_iMapLink, sizeof(int));
			pkStream->Read(&pkMapGroup->m_iBGID, sizeof(int));
			pkStream->Read(&pkMapGroup->m_iType, sizeof(int));
			
			pkStream->Read(szBuf, sizeof(szBuf));
			pkMapGroup->m_strGroupName = szBuf;

			pkStream->Read(szBuf, sizeof(szBuf));

#ifndef _MAP_TOOL
			pkMapGroup->m_strBGFile = "DATA/BGFORMAT/";
#endif
			pkMapGroup->m_strBGFile += szBuf;

			//BGM File
			pkStream->Read(szBuf, sizeof(szBuf));
			pkMapGroup->m_strBGMFile = "DATA/BGM/";
			pkMapGroup->m_strBGMFile += szBuf;

			//[2005/6/30] Effect Type
			pkStream->Read(&pkMapGroup->m_iSoundEffectType, sizeof(int));

			// AJJIYA [5/17/2005]
			// 미니맵 리소스 정보 추가
			pkStream->Read(&pkMapGroup->m_iMiniMapIconID, sizeof(int));
			pkStream->Read(&pkMapGroup->m_iMiniMapResID	, sizeof(int));			
			//			
			
			int iGravity = 1000;
			pkStream->Read(&iGravity, sizeof(int));
			if(iGravity) {
				pkMapGroup->m_fGravity = iGravity * 0.001f;
			}			
			
			int iMaxDrop = 1000;
			pkStream->Read(&iMaxDrop, sizeof(int));
			if(iMaxDrop) {
				pkMapGroup->m_fMaxDropSpeed = iMaxDrop * 0.001f;
			}
			
			int iVelocityX = 1000;
			pkStream->Read(&iVelocityX, sizeof(int));
			if(iVelocityX) {
				pkMapGroup->m_fVelocityX = iVelocityX * 0.001f;
			}
			
			int iJumpSpeed = 1000;
			pkStream->Read(&iJumpSpeed, sizeof(int));
			if(iJumpSpeed) {
				pkMapGroup->m_fJumpSpeed = iJumpSpeed * 0.001f;
			}
			
			int iUpdownVelocity = 1000;
			pkStream->Read(&iUpdownVelocity, sizeof(int));
			if(iUpdownVelocity) {
				pkMapGroup->m_fUpDownVelocity = iUpdownVelocity * 0.001f;
			}
			
			int iHangingVelocity = 1000;
			pkStream->Read(&iHangingVelocity, sizeof(int));
			if(iHangingVelocity) {
				pkMapGroup->m_fHangingVelocity = iHangingVelocity * 0.001f;
			}
			
			pkStream->Read(&iMapCount, sizeof(int));
			for( k = 0; k < iMapCount; k++ )
			{
				pkMapInfo = new SPMapInfo;
				pkMapInfo->iMapID = m_iMapCount;

				pkStream->Read(&pkMapInfo->iBGIndex, sizeof(int));
				
				pkStream->Read(szBuf, sizeof(szBuf));
				pkMapInfo->strMapName = szBuf;
				
#ifndef _MAP_TOOL
				pkMapInfo->strFormFile = "DATA/BGFORMAT/";
				pkMapInfo->strAttributeFile = "DATA/BGFORMAT/";
				pkMapInfo->strMiniMapFile = "DATA/BGFORMAT/";
#endif
				pkStream->Read(szBuf, sizeof(szBuf));
				pkMapInfo->strFormFile += szBuf;

				pkStream->Read(szBuf, sizeof(szBuf));
				pkMapInfo->strAttributeFile += szBuf;

				pkStream->Read(szBuf, sizeof(szBuf));
				pkMapInfo->strMiniMapFile += szBuf;

				pkMapGroup->AddMapInfo(pkMapInfo);
				pkMapGroup->m_iMapCount++;

				m_iMapCount++;
			}
			
			pkStage->AddMapGroup(pkMapGroup);
			pkStage->m_iGroupCount++;
		}

		(*pvpBackStageInfo).push_back(pkStage);
	}

	pkStream->Release();
	return 1;
}


WORDCASE_MAP( OnComment )
{
	return true;
}


WORDCASE_MAP( OnBlockBegin )
{
	//CHECK_BLOCK_INVALID_IN()
	
	if(STATE_BLOCK_NULL == m_StateBlock) {
		CHECK_BLOCK_INVALID_IN()
		m_StateBlock = STATE_BLOCK_BEGIN;
	}		
	else if(STATE_BLOCK_BEGIN == m_StateBlock) {
		CHECK_BLOCK_INNER_INVALID_IN()
		m_StateBlock = STATE_BLOCK_INNER_BEGIN;
	}		
	else if(STATE_BLOCK_INNER_BEGIN == m_StateBlock) {
		CHECK_BLOCK_INNER_INNER_INVALID_IN()
		m_StateBlock = STATE_BLOCK_INNER_INNER_BEGIN;
	}
	else {
		//return false;
	}
	
	return true;
}


WORDCASE_MAP( OnBlockEnd )
{
	//CHECK_BLOCK_INVALID_OUT()
	
	if(STATE_BLOCK_BEGIN == m_StateBlock) {
		CHECK_BLOCK_INVALID_OUT()
		m_StateBlock = STATE_BLOCK_NULL;

		if(pStage){
			m_vpBackStageInfo.push_back(pStage);	
			pStage = NULL;
			m_iMapCount = 0;
		}
	}
	else if(STATE_BLOCK_INNER_BEGIN == m_StateBlock) {
		CHECK_BLOCK_INNER_INVALID_OUT()
		m_StateBlock = STATE_BLOCK_BEGIN;

		if(pMapGroup && pStage){
			pStage->AddMapGroup(pMapGroup);
			pStage->m_iGroupCount++;	//[2005/1/24]
			pMapGroup = NULL;			
		}
	}
	else if(STATE_BLOCK_INNER_INNER_BEGIN == m_StateBlock) {
		CHECK_BLOCK_INNER_INNER_INVALID_OUT()
		m_StateBlock = STATE_BLOCK_INNER_BEGIN;		
		
		if(pMapInfo && pMapGroup){			
			pMapInfo->iMapID = m_iMapCount;	//[2005/3/31]
			
			pMapGroup->AddMapInfo(pMapInfo);				//백터인 경우
			//pMapGroup->AddMapInfo((SPMapInfo&)*pMapInfo);	//배열인 경우
			pMapGroup->m_iMapCount++;	//[2005/1/24]
			pMapInfo = NULL;
			
			m_iMapCount++;					//[2005/3/31]
		}
	}
	else {
		//return false;
	}	
	
	return true;
}


WORDCASE_MAP( OnTextHeader )
{ 
	CHECK_BLOCK_INVALID_IN()
	return true; 
}


WORDCASE_MAP ( OnStageCount )
{
	CHECK_BLOCK_INVALID_OUT()
	
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
		
	m_iStageCount = (int)atoi(strParam.c_str());
	
	return true;
}


WORDCASE_MAP( OnStage )
{
	CHECK_BLOCK_INVALID_IN()
	
	pStage = new SPStage;
	return true;
}


WORDCASE_MAP( OnStageID )
{
	CHECK_BLOCK_INVALID_OUT()
	
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pStage->m_iStageID = (int)atoi(strParam.c_str());
	
	return true;
}


WORDCASE_MAP( OnStageName ){
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	
	pStage->m_strStageName = strParam.c_str();
	
	return true;
}


WORDCASE_MAP( OnSyncRegionWidth )
{
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pStage->m_iSyncRegionWidth = (int)atoi(strParam.c_str());

	return true;
}


WORDCASE_MAP( OnSyncRegionHeight )
{
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pStage->m_iSyncRegionHeight = (int)atoi(strParam.c_str());

	return true;
}


WORDCASE_MAP( OnPalette )
{
	CHECK_BLOCK_INVALID_OUT()
	
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);	

	pStage->AddPaletteFile(strParam);

	return true;
}


WORDCASE_MAP( OnGroup )
{
	CHECK_BLOCK_INNER_INVALID_IN()

	pMapGroup = new SPMapGroup;
	
	return true;
}


WORDCASE_MAP( OnGroupID )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
	
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	
	pMapGroup->m_iGroupID = (int)atoi(strParam.c_str());
	
	return true;
}


WORDCASE_MAP( OnGroupName )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	
	pMapGroup->m_strGroupName = strParam;

	return true;
}


WORDCASE_MAP( OnMapLink )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pMapGroup->m_iMapLink = (MAP_LINK)atoi(strParam.c_str());

	return true;
}


WORDCASE_MAP( OnBGID )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	
	pMapGroup->m_iBGID = (int)atoi(strParam.c_str());

	return true;
}


WORDCASE_MAP( OnType )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pMapGroup->m_iType = atoi(strParam.c_str());

	return true;
}


WORDCASE_MAP( OnBGFile )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	
	std::string strResource = "";
	
#ifndef _MAP_TOOL
	strResource += "DATA/BGFORMAT/";
#endif
	
	strResource += strParam;
	
	pMapGroup->m_strBGFile = strResource;

	return true;
}


WORDCASE_MAP( OnBGMFile )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	std::string strResource = "";

	strResource += "DATA/BGM/";
	strResource += strParam;

	pMapGroup->m_strBGMFile = strResource;

	return true;
}


WORDCASE_MAP( OnSoundEffect )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pMapGroup->m_iSoundEffectType = (int)atoi(strParam.c_str());
	return true;
}


WORDCASE_MAP( OnMiniMapResID )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pMapGroup->m_iMiniMapResID = (int)atoi(strParam.c_str());

	return true;
}


WORDCASE_MAP( OnMiniMapIconID )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pMapGroup->m_iMiniMapIconID = (int)atoi(strParam.c_str());

	return true;
}


WORDCASE_MAP( OnGravity )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	int iGravity = (int)atoi(strParam.c_str());

	pMapGroup->m_fGravity = iGravity * 0.0001f;

	return true;
}


WORDCASE_MAP( OnMaxDrop )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	int iMaxDrop = (int)atoi(strParam.c_str());
	
	pMapGroup->m_fMaxDropSpeed = iMaxDrop * 0.0001f;

	return true;
}


WORDCASE_MAP( OnMaxVelocity )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	int iVelocityX = (int)atoi(strParam.c_str());
	pMapGroup->m_fVelocityX = iVelocityX * 0.0001f;

	return true;
}


WORDCASE_MAP( OnJumpSpeed )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	int iJumpSpeed = (int)atoi(strParam.c_str());
	pMapGroup->m_fJumpSpeed = iJumpSpeed * 0.0001f;

	return true;
}


WORDCASE_MAP( OnUpDownVelocity )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	
	int iUpdownVelocity = (int)atoi(strParam.c_str());
	pMapGroup->m_fUpDownVelocity = iUpdownVelocity * 0.0001f;

	return true;
}


WORDCASE_MAP( OnHangingVelocity )
{
	CHECK_BLOCK_INNER_INVALID_OUT()
		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	
	int iHangingVelocity = (int)atoi(strParam.c_str());
	pMapGroup->m_fHangingVelocity = iHangingVelocity * 0.0001f;

	return true;
}


WORDCASE_MAP( OnMap	)
{
	CHECK_BLOCK_INNER_INNER_INVALID_IN()

	pMapInfo = new SPMapInfo;
	
	return true;
}


WORDCASE_MAP( OnMapName )
{
	CHECK_BLOCK_INNER_INNER_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	if(pMapInfo)
		pMapInfo->strMapName = strParam;

	return true;
}


WORDCASE_MAP( OnBGIndex )
{
	CHECK_BLOCK_INNER_INNER_INVALID_OUT()
	
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	if(pMapInfo)
		pMapInfo->iBGIndex = (int)atoi(strParam.c_str());
		
	return true;
}


WORDCASE_MAP( OnFormFile )
{
	CHECK_BLOCK_INNER_INNER_INVALID_OUT()
	
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	
	std::string strResource = "";
	
#ifndef _MAP_TOOL
	strResource += "DATA/BGFORMAT/";
#endif

	strResource += strParam;
	
	//pMapInfo->strFormFile = strParam;	//파일명만
	if(pMapInfo)
		pMapInfo->strFormFile = strResource;
	
	//실제 FormFile Load
	
	return true;
}


WORDCASE_MAP( OnAttributeFile )
{
	CHECK_BLOCK_INNER_INNER_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	
	std::string strResource = "";

#ifndef _MAP_TOOL
	strResource += "DATA/BGFORMAT/";
#endif
	
	strResource += strParam;
	
	//pMapInfo->strAttributeFile = strParam;//파일명만
	if(pMapInfo)
		pMapInfo->strAttributeFile = strResource;

	//실제 AttributeFile Load

	return true;
}


WORDCASE_MAP( OnMiniMapFile )
{
	CHECK_BLOCK_INNER_INNER_INVALID_OUT()

		string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	std::string strResource = "";

#ifndef _MAP_TOOL
	strResource += "DATA/BGFORMAT/";
#endif

	strResource += strParam;

	//pMapInfo->strAttributeFile = strParam;//파일명만
	if(pMapInfo)
		pMapInfo->strMiniMapFile = strResource;

	//실제 AttributeFile Load

	return true;
}

