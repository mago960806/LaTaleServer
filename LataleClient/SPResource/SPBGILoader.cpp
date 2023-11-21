

#include <string>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <assert.h>
#include <string>

#include "SPCommon.h"
#include "SPResourceDef.h"

#include "SPMainGameDEF.h"
#include "SPGameObjectDEF.h"
#include "SPManager.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

//#include "SPFrameSkipper.h"
//#include "SPMainGameManager.h"

#include "SPUtil.h"
#include "SPFileDEF.H"
#include "SPStream.h"
#include "SPBGILoader.h"

using namespace std;


SPBGILoader::SPBGILoader()
{
}

SPBGILoader::~SPBGILoader()
{
}

bool SPBGILoader::LoadBGDataText(std::vector<SPRotateScrollImage*>* pvpBackRotateImage,
								 string strPath/*= ""*/, string strFile/*= ""*/)
{
	
	std::string strResource;
	if("" == strPath) {
		//strResource = RES_WORK_RESOURCE_PATH;
		strResource = "";
	}
	else {
		strResource = strPath;
	}
	
	if("" == strFile) {
		strResource += RES_FNAME_BGFORMAT;
	}
	else {
		strResource += strFile;
	}

	//int iFileHandle = open(strResource.c_str(), O_RDONLY);
	//if (iFileHandle == -1) return false;

	SPStream* pStream;
	SPResourceBase::GetInstance()->GetStreamData( strResource.c_str(), &pStream);
	if(pStream == NULL || !pStream->Valid())
	{
		SAFE_RELEASE(pStream);
		return false;
	}

	assert((*pvpBackRotateImage).empty());	

	string str;	
	
	SPRotateScrollImage* pScrollImage = NULL;

	m_StateBlock	= STATE_BLOCK_NULL;
	m_bgStateImage	= BG_ID_LAYER_NULL;

	
	while ( ReadLineFromStream(pStream, str) != -1) {
		Trim(str);

		// Ver 0.1
		EXCUTE_WORDCASE_MAP ( COMMENT							,	OnComment				)
		EXCUTE_WORDCASE_MAP ( BLOCK_BEGIN						,	OnBlockBegin			)
		EXCUTE_WORDCASE_MAP ( BLOCK_END							,	OnBlockEnd				)	

		EXCUTE_WORDCASE_MAP ( "BGFORMAT_HEADER"					,	OnTextHeader			) 
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_BACKGROUND0"			,	OnLayerBackground0		)
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_BACKGROUND1"			,	OnLayerBackground1		)
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_BACKGROUND2"			,	OnLayerBackground2		)
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_BACKGROUND3"			,	OnLayerBackground3		)														
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_TERRAIN0"			,	OnLayerTerrain0			)
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_TERRAIN1"			,	OnLayerTerrain1			)
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_TERRAIN2"			,	OnLayerTerrain2			)													 
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_FOREGROUND0"			,	OnLayerForeground0		)
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_FOREGROUND1"			,	OnLayerForeground1		)
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_FOREGROUND2"			,	OnLayerForeground2		)
		EXCUTE_WORDCASE_MAP ( "BG_ID_LAYER_FOREGROUND3"			,	OnLayerForeground3		)														
		EXCUTE_WORDCASE_MAP ( "fX"								,	OnMemberFX				)
		EXCUTE_WORDCASE_MAP ( "fY"								,	OnMemberFY				)
		EXCUTE_WORDCASE_MAP ( "fSX"								,	OnMemberFSX				)
		EXCUTE_WORDCASE_MAP ( "fSY"								,	OnMemberFSY				)
		EXCUTE_WORDCASE_MAP ( "fDX"								,	OnMemberFDX				)
		EXCUTE_WORDCASE_MAP ( "fDY"								,	OnMemberFDY				)
		EXCUTE_WORDCASE_MAP ( "fFlowDX"							,	OnMemberFFLOWDX			)
		EXCUTE_WORDCASE_MAP ( "fFlowDY"							,	OnMemberFFLOWDY			)
		EXCUTE_WORDCASE_MAP ( "fRotateStepX"					,	OnMemberFROTATESTEPX	)
		EXCUTE_WORDCASE_MAP ( "fRotateStepY"					,	OnMemberFROTATESTEPY	)
		EXCUTE_WORDCASE_MAP ( "Image"							,	OnMemberIMAGE			)
		EXCUTE_WORDCASE_MAP ( "fScaleX"							,	OnMemberSCALEX			) //jinhee 04.11.03
		EXCUTE_WORDCASE_MAP ( "fScaleY"							,	OnMemberSCALEY			) //jinhee 04.11.03
		EXCUTE_WORDCASE_MAP ( "iLightMap"						,	OnMemberLIGHTMAP		) //jinhee 04.11.03
																
																
		// Ver 0.1b
		//EXCUTE_WORDCASE_MAP ( TERRAIN_LAYER_CX				,	OnMemberCXTerrain		)
		//EXCUTE_WORDCASE_MAP ( TERRAIN_LAYER_CY				,	OnMemberCYTerrain		)

	}

	pStream->Release();
	//close(iFileHandle);
	
	for(int i = 0; i < (signed)m_vpBackRotateImageCopy.size(); ++i) {
		pvpBackRotateImage->push_back(NULL);
	}

	std::copy( m_vpBackRotateImageCopy.begin(), m_vpBackRotateImageCopy.end(), (*pvpBackRotateImage).begin());

	m_vpBackRotateImageCopy.clear();	

	return true;

}

bool SPBGILoader::LoadBGBinary(std::vector<SPRotateScrollImage*>* pvpBackRotateImage,
	string strPath, string strFile)
{
	std::string strResource;
	(*pvpBackRotateImage).clear();

	if("" == strPath) {
		//strResource = RES_WORK_RESOURCE_PATH;
		strResource = "";
	} else {
		strResource = strPath;
	}

	if("" == strFile) {
		strResource += RES_FNAME_BGFORMAT;
	}
	else {
		strResource += strFile;
	}

	//SPStream* pkStream = new SPFileStream(strResource.c_str());
	SPStream* pkStream;
	SPResourceBase::GetInstance()->GetStreamData(strResource.c_str(), &pkStream);
	if( pkStream == NULL || !pkStream->Valid() )
	{
		SAFE_RELEASE(pkStream);
		return false;
	}

	CONVERT_BG_INFO BGInfo;
	SPRotateScrollImage* pkScrollImage;
	int iSize, i;
	
	pkStream->Read(&iSize, sizeof(int));
	for( i = 0; i < iSize; i++ )
	{
		pkStream->Read(&BGInfo, sizeof(CONVERT_BG_INFO));
		pkScrollImage = new SPRotateScrollImage;
		
		pkScrollImage->PicID = BGInfo.eBGLayer;
		pkScrollImage->fX = BGInfo.fX;
		pkScrollImage->fY = BGInfo.fY;
		pkScrollImage->fSX = BGInfo.fSX;
		pkScrollImage->fSY = BGInfo.fSY;
		pkScrollImage->fDX = BGInfo.fDX;
		pkScrollImage->fDY = BGInfo.fDY;
		pkScrollImage->fFlowDX = BGInfo.fFlowDX;
		pkScrollImage->fFlowDY = BGInfo.fFlowDY;
		pkScrollImage->fRotateStepX = BGInfo.fRotateX;
		pkScrollImage->fRotateStepY = BGInfo.fRotateY;
		pkScrollImage->fScaleX = BGInfo.fScaleX;
		pkScrollImage->fScaleY = BGInfo.fScaleY;
		pkScrollImage->iLightMap = BGInfo.iLightMap;

		//strResource = RES_WORK_RESOURCE_PATH;
		strResource = "DATA/BACKGROUND/";	
		strResource += BGInfo.szImageName;

		pkScrollImage->strFile = BGInfo.szImageName;
		//g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pkScrollImage->pImage);
		g_pVideo->GetTextureMgr()->GetTexture(strResource.c_str(), &pkScrollImage->pImage,TEX_TERRAIN);
		
		(*pvpBackRotateImage).push_back(pkScrollImage);
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
	CHECK_BLOCK_INVALID_IN()
	m_StateBlock = STATE_BLOCK_BEGIN;
	return true;
}

WORDCASE_MAP( OnBlockEnd )
{
	CHECK_BLOCK_INVALID_OUT()

	if(pScrollImage && m_bgStateImage != BG_ID_LAYER_NULL && pScrollImage->PicID != BG_ID_LAYER_NULL) {
		m_vpBackRotateImageCopy.push_back(pScrollImage);		
	}
	
	m_StateBlock = STATE_BLOCK_NULL;
	m_bgStateImage = BG_ID_LAYER_NULL;	
	//pScrollImage->SetNull();
	pScrollImage = NULL;

	return true;
}

WORDCASE_MAP( OnTextHeader )
{ 
	CHECK_BLOCK_INVALID_IN()
	m_bgStateImage = BG_ID_LAYER_HEADER;
	
	return true; 
}

WORDCASE_MAP( OnLayerBackground0	)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_BACKGROUND0;	

	return true; 
}	

WORDCASE_MAP( OnLayerBackground1	)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_BACKGROUND1;
	return true; 
}	

WORDCASE_MAP( OnLayerBackground2	)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_BACKGROUND2;
	return true; 
}		
WORDCASE_MAP( OnLayerBackground3	)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_BACKGROUND3;
	return true; 
}	

WORDCASE_MAP( OnLayerTerrain0		)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_TERRAIN0;
	return true; 
}			

WORDCASE_MAP( OnLayerTerrain1		)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_TERRAIN1;
	return true; 
}		
WORDCASE_MAP( OnLayerTerrain2		)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_TERRAIN2;
	return true; 
}		

WORDCASE_MAP( OnLayerForeground0	)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_FOREGROUND0;
	return true; 
}		

WORDCASE_MAP( OnLayerForeground1	)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_FOREGROUND1;
	return true; 
}	

WORDCASE_MAP( OnLayerForeground2	)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_FOREGROUND2;
	return true; 
}

WORDCASE_MAP( OnLayerForeground3	)
{ 
	CHECK_BLOCK_INVALID_IN()
	pScrollImage = new SPRotateScrollImage;
	pScrollImage->PicID = m_bgStateImage = BG_ID_LAYER_FOREGROUND3;
	return true; 
}		

WORDCASE_MAP( OnMemberFX			)
{ 
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fX = (float)atof(strParam.c_str());

	return true; 
}

WORDCASE_MAP( OnMemberFY			)
{ 
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fY = (float)atof(strParam.c_str());

	return true; 
}		

WORDCASE_MAP( OnMemberFSX			)
{ 
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fSX = (float)atof(strParam.c_str());

	return true; 
}		

WORDCASE_MAP( OnMemberFSY			)
{ 
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fSY = (float)atof(strParam.c_str());

	return true; 
}

WORDCASE_MAP( OnMemberFDX			)
{ 
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fDX = (float)atof(strParam.c_str());

	return true; 
}

WORDCASE_MAP( OnMemberFDY			)
{ 
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fDY = (float)atof(strParam.c_str());

	return true; 
}

WORDCASE_MAP( OnMemberFFLOWDX		)
{ 
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fFlowDX = (float)atof(strParam.c_str());

	return true; 
}		
WORDCASE_MAP( OnMemberFFLOWDY		)
{ 
	CHECK_BLOCK_INVALID_OUT()
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fFlowDY = (float)atof(strParam.c_str());

	return true; 
}		

WORDCASE_MAP( OnMemberFROTATESTEPX	)
{ 
	CHECK_BLOCK_INVALID_OUT()
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fRotateStepX = (float)atof(strParam.c_str());

	return true; 
}		

WORDCASE_MAP( OnMemberFROTATESTEPY	)
{ 
	CHECK_BLOCK_INVALID_OUT()
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fRotateStepY = (float)atof(strParam.c_str());

	return true; 
}		


WORDCASE_MAP( OnMemberIMAGE			)
{ 
	CHECK_BLOCK_INVALID_OUT()	

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	//std::string strResource = RES_WORK_RESOURCE_PATH;
	std::string strResource = "";
	strResource += "DATA/BACKGROUND/";	
	strResource += strParam;

	pScrollImage->strFile = strParam; //

	//g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pScrollImage->pImage);
	g_pVideo->GetTextureMgr()->GetTexture(strResource.c_str(), &pScrollImage->pImage,TEX_TERRAIN);

	return true; 
}


WORDCASE_MAP( OnMemberSCALEX		)		//jinhee - 04.11.03
{
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fScaleX = (float)atof(strParam.c_str());

	return true; 
}

WORDCASE_MAP( OnMemberSCALEY		)		//jinhee - 04.11.03
{
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->fScaleY = (float)atof(strParam.c_str());

	return true; 
}

WORDCASE_MAP( OnMemberLIGHTMAP		)		//jinhee - 04.11.03
{
	CHECK_BLOCK_INVALID_OUT()

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	pScrollImage->iLightMap = (int)atoi(strParam.c_str());

	return true; 
}

//WORDCASE_MAP( OnMemberCXTerrain )
//{
//	CHECK_BLOCK_INVALID_OUT()
//	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
//	strParam.erase( strParam.begin(), Iter+1);
//	Trim(strParam);
//
//	m_fCXTerrain = atof(strParam.c_str());
//	return true;
//}
//
//WORDCASE_MAP( OnMemberCYTerrain	)	
//{
//	CHECK_BLOCK_INVALID_OUT()
//	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
//	strParam.erase( strParam.begin(), Iter+1);
//	Trim(strParam);
//
//	m_fCYTerrain = atof(strParam.c_str());
//	return true;
//}
//








