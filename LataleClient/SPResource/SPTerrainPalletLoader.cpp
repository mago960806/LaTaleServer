

#include <string>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <assert.h>

#include "SPCommon.h"
#include "SPResourceDef.h"

#ifdef _LATALE_CLIENT
#include "SPDebug.h"
#endif

#include "SPMainGameDEF.h"
#include "SPGameObjectDEF.h"
#include "SPManager.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

//#include "SPFrameSkipper.h"
//#include "SPMainGameManager.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPTerrainModel.h"
#include "SPTerrainModelStatic.h"
#include "SPTerrainModelFrameAnimation.h"
#include "SPTerrainModelCircularMotion.h"

#include "SPUtil.h"
#include "SPFileDEF.H"
#include "SPStream.h"
#include "SPTerrainPalletLoader.h"

#ifdef _MAP_TOOL
#include "stdafx.h"
#include "ToolBase.h"
#include "MainFrm.h"
#include "ToolBaseDoc.h"

#endif

using namespace std;


SPTerrainPalletLoader::SPTerrainPalletLoader()
: m_pTerrainModel(NULL)
, m_iDeep(0)
, m_pTempImage(NULL)
, m_BS(BSTATE_NULL)
, m_bGlobal(false)
{}

SPTerrainPalletLoader::~SPTerrainPalletLoader()
{
}


bool SPTerrainPalletLoader::LoadImage(const char* pszPath, bool bGlobal /*= false*/)
{
	//std::string strResource = RES_WORK_RESOURCE_PATH;
	std::string strResource = "";

//
#ifdef _MAP_TOOL
	strResource = m_strFilePath;
#else
	if(bGlobal)
		strResource += RES_PATH_GLOBAL_TERRAIN;			//글로벌 타일파일을 읽는 경우
	else 
		strResource += RES_PATH_TERRAIN;				//로컬 타일파일을 읽는 경우
#endif	

#ifdef _MAP_TOOL
	CMsgLog::WriteMsgAppendRN(&theApp.GetLogPath(), "SPTerrainPalletLoader::LoadImage filePath[%s] file[%s]\n", strResource.c_str(), pszPath);	
	strResource += "/";	
#endif

	strResource += pszPath;
	
	//g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &m_pTempImage);
	g_pVideo->GetTextureMgr()->GetTexture(strResource.c_str(), &m_pTempImage,TEX_TERRAIN);
	if( m_pTempImage == NULL )
	{
#if defined(_DEBUG)
		//MessageBox(NULL, strResource.c_str(), "Error", MB_OK);
		//assert(0 && "Error Load Texture.");
#endif
		return false;
	}
	return true;
}


bool SPTerrainPalletLoader::LoadDataText(const char* pszPalletFile, 
										 std::vector<SPTerrainModel*>* pvpTerrainModel,
										 string strPath/* = ""*/,
										 bool bGlobal /*= false*/,
										 string strResPath /*= ""*/)
{
	m_bGlobal = bGlobal;
	
	//std::string strResource = RES_WORK_RESOURCE_PATH;	
	std::string strResource = "";	

	if("" != strPath){					//파라미터 패스 체크 jinhee 04.10.15
		strResource = strPath;		
	}

	m_strFilePath.clear();
#ifdef _MAP_TOOL
	if(strResPath != "")
		m_strFilePath = strResPath;
#endif
	
	if(pszPalletFile) {
		strResource += pszPalletFile;
	} else {
		strResource += RES_FNAME_TERRAINPALLET;
	}

	SPStream* pStream;
	SPResourceBase::GetInstance()->GetStreamData( strResource.c_str(), &pStream);
	if(pStream == NULL || !pStream->Valid())
	{
		SAFE_RELEASE(pStream);
		return false;
	}

	//int iFileHandle = open(strResource.c_str(), O_RDONLY);
	//if (iFileHandle == -1) return false;

	assert((*pvpTerrainModel).empty());	

	string str;
	while ( ReadLineFromStream(pStream, str) != -1) {
		Trim(str);
		
		EXCUTE_WORDCASE_MAP ( COMMENT					,	OnComment				)
		EXCUTE_WORDCASE_MAP ( BLOCK_BEGIN				,	OnBlockBegin			)
		EXCUTE_WORDCASE_MAP ( BLOCK_END					,	OnBlockEnd				)	
		EXCUTE_WORDCASE_MAP ( "TERRAINPALLET_HEADER"	,	OnTextHeader			)		
		EXCUTE_WORDCASE_MAP ( "STATIC_MODEL"			,	OnStaticModel			)
		EXCUTE_WORDCASE_MAP ( "FRAME_ANIMATION"			,	OnFrameAnimation		)
		EXCUTE_WORDCASE_MAP ( "CIRCULAR_MOTION"			,	OnCircularMotion		)
		EXCUTE_WORDCASE_MAP ( "MEMBER"					,	OnMember				)		
	}

	pStream->Release();

	//close(iFileHandle);
	
	for(int i = 0; i < (signed)m_vTerrainModel.size(); ++i) {
		pvpTerrainModel->push_back(NULL);
	}
	std::copy( m_vTerrainModel.begin(), m_vTerrainModel.end(), (*pvpTerrainModel).begin());
	m_vTerrainModel.clear();

	return true;
}

bool SPTerrainPalletLoader::LoadBinary(const char* pszPalletFile, 
	std::vector<SPTerrainModel*>* pvpTerrainModel,
	string strPath,
	bool bGlobal /*= false*/)
{
	m_bGlobal = bGlobal;

	//std::string strResource = RES_WORK_RESOURCE_PATH;
	std::string strResource = "";
	(*pvpTerrainModel).clear();

	if("" != strPath)
		strResource = strPath;		

	if(pszPalletFile)
		strResource += pszPalletFile;
	else
		strResource += RES_FNAME_TERRAINPALLET;

	SPStream* pkStream;
	SPResourceBase::GetInstance()->GetStreamData(strResource.c_str(), &pkStream);

	//SPStream* pkStream = new SPFileStream(strResource.c_str());
	if( pkStream == NULL || !pkStream->Valid() )
	{
		SAFE_RELEASE(pkStream);
		return false;
	}

	CONVERT_STATIC_MODEL StaticModel;
	CONVERT_CIRCULAR_MODEL CircularModel;
	CONVERT_FRAME_ANIMATION FrameAnimation;
	int iSize, i, j;

	pkStream->Read(&iSize, sizeof(int));
	for( i = 0; i < iSize; i++ )
	{
		pkStream->Read(&StaticModel, sizeof(CONVERT_STATIC_MODEL));
		LoadImage(StaticModel.szImageName, m_bGlobal);
		
		m_pTerrainModel = new SPTerrainModelStatic(StaticModel.iInstance,
			&m_pTempImage, StaticModel.iLightMap);
		m_pTerrainModel->GetMemberValue(MV_SET_FILENAME, (LPARAM)StaticModel.szImageName);

		(*pvpTerrainModel).push_back( m_pTerrainModel );
		m_pTerrainModel = NULL;
		m_pTempImage = NULL;
	}

	pkStream->Read(&iSize, sizeof(int));
	for( i = 0; i < iSize; i++ )
	{
		pkStream->Read(&CircularModel, sizeof(CONVERT_CIRCULAR_MODEL));
		LoadImage(CircularModel.szImageName, m_bGlobal);

		m_pTerrainModel = new SPTerrainModelCircularMotion(CircularModel.iInstance,
			CircularModel.fDelay, CircularModel.fRadian, &m_pTempImage, CircularModel.iLightMap);
		m_pTerrainModel->GetMemberValue(MV_SET_FILENAME, (LPARAM)CircularModel.szImageName);
		
		(*pvpTerrainModel).push_back( m_pTerrainModel );
		m_pTerrainModel = NULL;
		m_pTempImage = NULL;
	}

	pkStream->Read(&iSize, sizeof(int));
	for( i = 0; i < iSize; i++ )
	{
		//pkStream->Read(&FrameAnimation.iInstance, sizeof(int));
		pkStream->Read(&FrameAnimation.iInstance, sizeof(INT64));
		pkStream->Read(&FrameAnimation.fDelay, sizeof(float));
		pkStream->Read(&FrameAnimation.fMinStart, sizeof(float));
		pkStream->Read(&FrameAnimation.fMaxStart, sizeof(float));
		pkStream->Read(&FrameAnimation.iLightMap, sizeof(int));
		pkStream->Read(&FrameAnimation.iSyncTile, sizeof(int));
		
		m_pTerrainModel = new SPTerrainModelFrameAnimation(FrameAnimation.iInstance,
			FrameAnimation.fDelay, FrameAnimation.fMinStart, FrameAnimation.fMaxStart, FrameAnimation.iLightMap, FrameAnimation.iSyncTile);

		int iImageSize;
		pkStream->Read(&iImageSize, sizeof(int));
		for( j = 0; j < iImageSize; j++ )
		{
			int iAlpha;
			char szBuf[64];
			
			pkStream->Read(&iAlpha, sizeof(int));
			pkStream->Read(szBuf, sizeof(szBuf));

			LoadImage(szBuf, m_bGlobal);
			m_pTerrainModel->GetMemberValue(MV_SET_FILELIST, (LPARAM)szBuf);
			m_pTerrainModel->SetImage(&m_pTempImage, iAlpha);
			m_pTempImage = NULL;
		}

		(*pvpTerrainModel).push_back( m_pTerrainModel );
		m_pTerrainModel = NULL;
	}

	pkStream->Release();
	return true;
}


WORDCASE_MAP( OnComment )
{
	return true;
}


WORDCASE_MAP( OnBlockBegin )
{
	m_iDeep++;		
	if(m_BS == BSTATE_NULL) return false;
	return true;
}

WORDCASE_MAP( OnBlockEnd )
{
	assert(m_iDeep);	

	switch(m_iDeep) {
	case 1:	// Animation		
		{
			switch(m_BS) {
			case BSTATE_FRAME_ANI:
				{
					if(m_pTerrainModel == NULL) {
						assert(0);
						return false;
					}
					m_vTerrainModel.push_back( m_pTerrainModel );
					m_pTerrainModel = NULL;
				}
				break;
			}
		}
		break;
	}

	m_iDeep--;
	m_BS = BSTATE_NULL;
	return true;
}

WORDCASE_MAP( OnTextHeader )
{ 
	CHECK_BLOCK_INVALID_FIRST()	
	m_BS = BSTATE_HEADER;
	return true; 
}

WORDCASE_MAP( OnStaticModel			)
{
	CHECK_BLOCK_INVALID_FIRST()	
	assert(m_pTerrainModel == NULL);
	assert(m_pTempImage == NULL);

	//STATIC_MODEL 	: 1001	:	b001.tga	
	//int iInstance;
	INT64 iInstance;
	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;

	int iNOArg = GetSeperateString(strParam, vstrArg);	
	char strPath[128];									//jinhee
	int iLightMap = FALSE;								//jinhee

	//assert(iNOArg == 2);
	assert(iNOArg == 3);								//jinhee

	Iter = vstrArg.begin();

	if(Iter != vstrArg.end()) {
		//iInstance = atoi((*Iter).c_str());
		iInstance = _atoi64((*Iter).c_str());
		++Iter;
	}	
	
	if(Iter != vstrArg.end()) {		
		Trim((*Iter));
		LoadImage((*Iter).c_str(), m_bGlobal);		
		sprintf(strPath, "%s", (*Iter).c_str());		//jinhee
		//TRACE("Static : strFile %s\n", strPath);
		++Iter;
	}	
	
	//
	if(Iter != vstrArg.end()) {							//jinhee - 04.11.04
		iLightMap = atoi((*Iter).c_str());
		//TRACE("iLightMap[%s][%d] \n", (*Iter).c_str(), iLightMap);
		++Iter;
	}

	m_pTerrainModel = new SPTerrainModelStatic(iInstance, &m_pTempImage, iLightMap);	

	m_pTerrainModel->GetMemberValue(MV_SET_FILENAME, (LPARAM)strPath); //jinhee		
	m_vTerrainModel.push_back( m_pTerrainModel );
	m_pTerrainModel = NULL;
	m_pTempImage = NULL;

	return true;
}



WORDCASE_MAP( OnFrameAnimation		)
{
	CHECK_BLOCK_INVALID_FIRST()	
	assert(m_pTerrainModel == NULL);
	assert(m_pTempImage == NULL);

	//FRAME_ANIMATION : 2001 : 0.5 : 0.0 : 0.6
	//{
	//	MEMBER = b001.tga
	//	MEMBER = b002.tga
	//	MEMBER = b003.tga
	//}

	//int iInstance; 
	INT64 iInstance; 
	float fDelay;
	float fMinStart;
	float fMaxStart;
	int iLightMap;
	int iSyncTile = 0;
	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;

	int iNOArg = GetSeperateString(strParam, vstrArg);

	assert(iNOArg == 5 || iNOArg == 6);

	Iter = vstrArg.begin();

	if(Iter != vstrArg.end()) {
		//iInstance = atoi((*Iter).c_str());
		iInstance = _atoi64((*Iter).c_str());
		++Iter;
	}
	
	if(Iter != vstrArg.end()) {
		fDelay = (float)atof((*Iter).c_str());
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		fMinStart = (float)atof((*Iter).c_str());
		++Iter;
	}
	
	if(Iter != vstrArg.end()) {
		fMaxStart = (float)atof((*Iter).c_str());		
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		iLightMap = atoi((*Iter).c_str());		
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		if((*Iter).c_str() != "") {
			iSyncTile = atoi((*Iter).c_str());		
			++Iter;
		}		
	}


	m_pTerrainModel = new SPTerrainModelFrameAnimation(iInstance, fDelay, fMinStart, fMaxStart, iLightMap, iSyncTile);
	m_BS = BSTATE_FRAME_ANI;

	return true;
}


WORDCASE_MAP( OnCircularMotion		)
{	
	CHECK_BLOCK_INVALID_FIRST()	
	assert(m_pTerrainModel == NULL);
	assert(m_pTempImage == NULL);

	//CIRCULAR_MOTION : 3001 : 0.5 : 0.25 : t003.tga : 0
	//int iInstance; 
	INT64 iInstance; 
	float fDelay;
	float fRadian;
	int iLightMap;
	char strPath[128];									//jinhee - 04.11.11
	
	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;

	int iNOArg = GetSeperateString(strParam, vstrArg);

	assert(iNOArg == 5);	

	Iter = vstrArg.begin();

	if(Iter != vstrArg.end()) {
		//iInstance = atoi((*Iter).c_str());
		iInstance = _atoi64((*Iter).c_str());
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		fDelay = (float)atof((*Iter).c_str());
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		fRadian = (float)atof((*Iter).c_str());
		++Iter;
	}

	if(Iter != vstrArg.end()) {		
		Trim((*Iter));
		LoadImage((*Iter).c_str(), m_bGlobal);
		sprintf(strPath, "%s", (*Iter).c_str());		//jinhee - 04.11.11		
		++Iter;
	}	

	if(Iter != vstrArg.end()) {
		iLightMap = atoi((*Iter).c_str());
		++Iter;
	}

	m_pTerrainModel = new SPTerrainModelCircularMotion(iInstance, fDelay, fRadian, &m_pTempImage, iLightMap);
	m_pTerrainModel->GetMemberValue(MV_SET_FILENAME, (LPARAM)strPath); //jinhee	- 04.11.11
	m_vTerrainModel.push_back( m_pTerrainModel );

	m_pTerrainModel = NULL;
	m_pTempImage = NULL;

	return true;
}


WORDCASE_MAP( OnMember 	)
{
	assert( m_pTerrainModel && m_pTerrainModel->GetClassID() == TERRAIN_MODEL_CLASS_FRAME_ANIMATION);
	assert(	m_pTempImage == NULL );

	int iAlpha; 

	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;

	int iNOArg = GetSeperateString(strParam, vstrArg);

	assert(iNOArg == 2);

	Iter = vstrArg.begin();
	
	char strPath[128];									//jinhee
	if(Iter != vstrArg.end()) {
		Trim((*Iter));
		LoadImage((*Iter).c_str(), m_bGlobal);
		sprintf(strPath, "%s", (*Iter).c_str());
		//TRACE("Member : strFile %s\n", strPath);
		++Iter;		
	}

	if(Iter != vstrArg.end()) {
		iAlpha = atoi((*Iter).c_str());		
	}
	
	m_pTerrainModel->GetMemberValue(MV_SET_FILELIST, (LPARAM)strPath); //jinhee	
	m_pTerrainModel->SetImage(&m_pTempImage, iAlpha);	
	m_pTempImage = NULL;
	return true;

}










