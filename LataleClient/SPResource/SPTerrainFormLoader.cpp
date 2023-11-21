

#include <string>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#include <functional>
#include <vector>
#include <assert.h>

#include "SPCommon.h"
#include "SPResourceDef.h"

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
#include "SPTerrainUnit.h"

#include "SPUtil.h"
#include "SPFileDEF.H"
#include "SPStream.h"
#include "SPTerrainPalletLoader.h"
#include "SPTerrainFormLoader.h"

//
#ifdef _DEBUG
#include <iostream>			//jinhee - [2005/1/3] //테스트를 위한...
#include <fstream>
#endif

using namespace std;

namespace {

	////
	//struct _IsEqualInstance : public binary_function<SPTerrainModel*, int, bool>
	//{
	//	bool operator() ( SPTerrainModel* pTerrainModel, int iInstance) const
	//	{		
	//		if(pTerrainModel->GetInstanceID() == iInstance) {
	//			return true;
	//		}
	//		return false;
	//	}
	//} IsEqualInstance;

	struct _IsEqualInstance : public binary_function<SPTerrainModel*, INT64, bool>
	{
		bool operator() ( SPTerrainModel* pTerrainModel, INT64 iInstance) const
		{		
			if(pTerrainModel->GetInstanceID() == iInstance) {
				return true;
			}
			return false;
		}
	} IsEqualInstance;
}




SPTerrainFormLoader::SPTerrainFormLoader()
{
	m_fCXTerrain = 0.f;
	m_fCYTerrain = 0.f;
	m_iDeep	= 0;
	m_iCurLayer = 0;
	m_pvpCurLayer = NULL;
	m_pvpTerrainCommonModel = NULL;
}

SPTerrainFormLoader::~SPTerrainFormLoader()
{
}

void SPTerrainFormLoader::SetCommonPallet(std::vector<SPTerrainModel*>* pvpTerrainCommonModel)
{ 
	m_pvpTerrainCommonModel = pvpTerrainCommonModel;
}


bool SPTerrainFormLoader::LoadDataText(const char* pszFile, 
									   std::vector<SPTerrainUnit*>* pvpTerrainUnit1,
									   std::vector<SPTerrainUnit*>* pvpTerrainUnit2,
									   std::vector<SPTerrainUnit*>* pvpTerrainUnit3,
									   std::vector<SPTerrainModel*>* pvpTerrainModel,
									   float fStartPosX /*= 0.0f*/,
									   float fStartPosY /*= 0.0f*/,
									   const char* strPath/* = NULL*/)
{	
	std::string strResource;
	if(NULL != strPath){		
		strResource = strPath;		
	}
	else{
		//strResource = RES_WORK_RESOURCE_PATH;
		strResource = "";
	}

	if(pszFile) {
		strResource += pszFile;	
	} else {
		strResource += RES_FNAME_TERRAINFORM;
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


	string str;	
	m_fStartPosX = fStartPosX;
	m_fStartPosY = fStartPosY;

	m_vpTerrainLayer1.clear();
	m_vpTerrainLayer2.clear();
	m_vpTerrainLayer3.clear();
	

	while ( ReadLineFromStream(pStream, str) != -1) {
		Trim(str);

		// Ver 0.1
		EXCUTE_WORDCASE_MAP ( COMMENT				,		OnComment				)
		EXCUTE_WORDCASE_MAP ( BLOCK_BEGIN			,		OnBlockBegin			)
		EXCUTE_WORDCASE_MAP ( BLOCK_END				,		OnBlockEnd				)

		EXCUTE_WORDCASE_MAP ( "TERRAIN_LAYER_CX"	,		OnTerrainLayerCX		)
		EXCUTE_WORDCASE_MAP ( "TERRAIN_LAYER_CY"	,		OnTerrainLayerCY		)
		EXCUTE_WORDCASE_MAP ( "TERRAINFORM_HEADER"	,		OnTerrainformHeader		)
		EXCUTE_WORDCASE_MAP ( "COMMON_PALLET"		,		OnCommonPallet			)
		EXCUTE_WORDCASE_MAP ( "LOCAL_PALLET"		,		OnLocalPallet			)
		EXCUTE_WORDCASE_MAP ( "TERRAIN_LAYER1"		,		OnTerrainLayer1			)
		EXCUTE_WORDCASE_MAP ( "TERRAIN_LAYER2"		,		OnTerrainLayer2			)
		EXCUTE_WORDCASE_MAP ( "TERRAIN_LAYER3"		,		OnTerrainLayer3			)
		EXCUTE_WORDCASE_MAP ( "TILE"				,		OnTile					)

	}

	pStream->Release();
	//close(iFileHandle);


	std::vector<SPTerrainUnit*>::iterator IterUnit;
	int i;

	pvpTerrainUnit1->clear();
	pvpTerrainUnit2->clear();
	pvpTerrainUnit3->clear();	
	pvpTerrainModel->clear();

	for(i = 0; i < m_vpTerrainLayer1.size(); ++i)  pvpTerrainUnit1->push_back(NULL);
	for(i = 0; i < m_vpTerrainLayer2.size(); ++i)  pvpTerrainUnit2->push_back(NULL);
	for(i = 0; i < m_vpTerrainLayer3.size(); ++i)  pvpTerrainUnit3->push_back(NULL);
	for(i = 0; i < m_vpTerrainLocalModel.size(); ++i)  pvpTerrainModel->push_back(NULL);

	std::copy( m_vpTerrainLayer1.begin(), m_vpTerrainLayer1.end(), pvpTerrainUnit1->begin());
	std::copy( m_vpTerrainLayer2.begin(), m_vpTerrainLayer2.end(), pvpTerrainUnit2->begin());
	std::copy( m_vpTerrainLayer3.begin(), m_vpTerrainLayer3.end(), pvpTerrainUnit3->begin());
	std::copy( m_vpTerrainLocalModel.begin(), m_vpTerrainLocalModel.end(), pvpTerrainModel->begin());

	return true;
}

bool SPTerrainFormLoader::LoadBinary(const char* pszFile, 
	vector<SPTerrainUnit*>* pvpTerrainUnit1, vector<SPTerrainUnit*>* pvpTerrainUnit2,
	vector<SPTerrainUnit*>* pvpTerrainUnit3, vector<SPTerrainModel*>* pvpTerrainModel,
	float fStartPosX, float fStartPosY, const char* strPath)
{
	(*pvpTerrainUnit1).clear();
	(*pvpTerrainUnit2).clear();
	(*pvpTerrainUnit3).clear();
	//(*pvpTerrainModel).clear();				//[2005/5/31] - jinhee

	std::string strResource;
	if(NULL != strPath) {
		strResource = strPath;		
	} else {
		//strResource = RES_WORK_RESOURCE_PATH;
		strResource = "";
	}

	if(pszFile)
		strResource += pszFile;	
	else
		strResource += RES_FNAME_TERRAINFORM;

	//SPStream* pkStream = new SPFileStream(strResource.c_str());

	SPStream* pkStream;
	SPResourceBase::GetInstance()->GetStreamData(strResource.c_str(), &pkStream);
	

	if( pkStream==NULL || !pkStream->Valid() )
	{
		SAFE_RELEASE(pkStream);
		return false;
	}
	
	m_fStartPosX = fStartPosX;
	m_fStartPosY = fStartPosY;

	char szBuf[64];
	pkStream->Read(&m_fCXTerrain, sizeof(int));
	pkStream->Read(&m_fCYTerrain, sizeof(int));
	pkStream->Read(szBuf, sizeof(szBuf));

	if( strlen(szBuf) )
	{
		string strFile = RES_PATH_BGFORMAT;
		strFile += szBuf;

		SPTerrainPalletLoader PalletLoader;
		if( !PalletLoader.LoadBinary(strFile.c_str(), pvpTerrainModel) )
			return false;
	}

	CONVERT_FORM_INFO FormInfo;
	SPTerrainUnit* pkTerrainUnit;
	SPTerrainModel* pkTerrainModel;
	int iFormSize, i, j;

	vector<SPTerrainUnit*>* ppvTerrainArray[3];
	ppvTerrainArray[0] = pvpTerrainUnit1;
	ppvTerrainArray[1] = pvpTerrainUnit2;
	ppvTerrainArray[2] = pvpTerrainUnit3;

	for( i = 0; i < 3; i++ )
	{
		pkStream->Read(&iFormSize, sizeof(int));
		for( j = 0; j < iFormSize; j++ )
		{
			pkStream->Read(&FormInfo, sizeof(CONVERT_FORM_INFO));
			
			if( pkTerrainModel = IsExistModel(FormInfo.iInstance, pvpTerrainModel) )
			{
				pkTerrainUnit = new SPTerrainUnit(&pkTerrainModel,
					FormInfo.fX + m_fStartPosX, FormInfo.fY + m_fStartPosY,
					FormInfo.iArg0, FormInfo.iArg1, FormInfo.iArg2, FormInfo.iArg3,
					FormInfo.fScaleX, FormInfo.fScaleY);

				(*ppvTerrainArray[i]).push_back(pkTerrainUnit);
			}
			else
			{
#if defined(_DEBUG)
				char szBuf[128];
				sprintf(szBuf, "Form Instance: %I64d가 존재하지 않습니다.", FormInfo.iInstance);
				//MessageBox(NULL, szBuf, "Error", MB_OK);
				assert(0 && szBuf);
#endif
			}
		}
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
	++m_iDeep;
	return true;
}

WORDCASE_MAP( OnBlockEnd )
{
	--m_iDeep;
	if(m_pvpCurLayer) {
		m_pvpCurLayer = NULL;
	}

	return true;
}

WORDCASE_MAP( OnTerrainformHeader	)
{
	CHECK_BLOCK_INVALID( 0 )
	return true;
}

WORDCASE_MAP( OnCommonPallet		)
{
	CHECK_BLOCK_INVALID( 1 )
	return true;
}

WORDCASE_MAP( OnLocalPallet			)
{
	CHECK_BLOCK_INVALID( 1 )
	//LOCAL_PALLET	= TerrainPallet.txt
	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);

	string strFile = RES_PATH_BGFORMAT;
	strFile += strParam;

	SPTerrainPalletLoader PalletLoader;
	bool bRet = PalletLoader.LoadDataText(strFile.c_str(), &m_vpTerrainLocalModel);
	assert(bRet && " 로컬 팔레트 에러" );


	return true;
}

WORDCASE_MAP( OnTerrainLayerCX		)
{
	CHECK_BLOCK_INVALID( 1 )	

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	m_fCXTerrain = (float)atof(strParam.c_str());
	return true;
}

WORDCASE_MAP( OnTerrainLayerCY		)
{
	CHECK_BLOCK_INVALID( 1 )

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	m_fCYTerrain = (float)atof(strParam.c_str());
	return true;
}

WORDCASE_MAP( OnTerrainLayer1		)
{
	CHECK_BLOCK_INVALID( 0 )
	m_pvpCurLayer = &m_vpTerrainLayer1;
	m_iCurLayer = 1 ;
	return true;
}

WORDCASE_MAP( OnTerrainLayer2		)
{
	CHECK_BLOCK_INVALID( 0 )
	m_pvpCurLayer = &m_vpTerrainLayer2;
	m_iCurLayer = 2 ;
	return true;
}

WORDCASE_MAP( OnTerrainLayer3		)
{
	CHECK_BLOCK_INVALID( 0 )
	m_pvpCurLayer = &m_vpTerrainLayer3;
	m_iCurLayer = 3 ;
	return true;
}

WORDCASE_MAP( OnTile				)
{
	CHECK_BLOCK_INVALID( 1 )

	assert(m_pvpCurLayer && "Invalid Terrain Layer");
	if(m_pvpCurLayer == NULL) return false;

	//	TILE : 1001 : 30 : 40 : red : greed : blue : alpha : XScale : YScale
	//int iInstance;
	INT64 iInstance;
	float fX;
	float fY;
	int	iArg0;
	int	iArg1;
	int	iArg2;
	int	iArg3;
	float fScaleX = 1.0f;	//jinhee [12/7/2004]
	float fScaleY = 1.0f;	//jinhee [12/7/2004]

	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator IterArg;

	int iNOArg = GetSeperateString(strParam, vstrArg);
	
#ifndef _MAP_TOOL			//jinhee [12/7/2004]
	//assert(iNOArg == 7);
	assert(iNOArg == 9);
#endif
	
	IterArg = vstrArg.begin();

	if(IterArg != vstrArg.end()) {
		//iInstance = atoi((*IterArg).c_str());
		iInstance = _atoi64((*IterArg).c_str());
		++IterArg;
	}

	if(IterArg != vstrArg.end()) {
		fX = (float)atof((*IterArg).c_str());
		fX = m_fStartPosX + fX;
		++IterArg;
	}

	if(IterArg != vstrArg.end()) {
		fY = (float)atof((*IterArg).c_str());	
		fY = m_fStartPosY + fY;
		++IterArg;
	}

	if(IterArg != vstrArg.end()) {
		iArg0 = atoi((*IterArg).c_str());	
		++IterArg;
	}

	if(IterArg != vstrArg.end()) {
		iArg1 = atoi((*IterArg).c_str());	
		++IterArg;
	}

	if(IterArg != vstrArg.end()) {
		iArg2 = atoi((*IterArg).c_str());	
		++IterArg;
	}

	if(IterArg != vstrArg.end()) {
		iArg3 = atoi((*IterArg).c_str());			
		++IterArg; //
	}	
	
	if(9 == iNOArg){
		if(IterArg != vstrArg.end()) {		//jinhee [12/7/2004]
			fScaleX = atof((*IterArg).c_str());
			++IterArg;
		}

		if(IterArg != vstrArg.end()) {		//jinhee [12/7/2004]
			fScaleY = atof((*IterArg).c_str());
			++IterArg;
		}
	}	

	std::vector<SPTerrainModel*>::iterator Iter;
	bool	bFind = false;

	Iter = std::find_if(
			m_vpTerrainLocalModel.begin(), 
			m_vpTerrainLocalModel.end(), 
			std::bind2nd(IsEqualInstance, iInstance));

	if(Iter != m_vpTerrainLocalModel.end()) {
		bFind = true;
	} else {
		if(m_pvpTerrainCommonModel) {
			Iter = std::find_if(
				m_pvpTerrainCommonModel->begin(), 
				m_pvpTerrainCommonModel->end(), 
				std::bind2nd(IsEqualInstance, iInstance));
		} 
		if(Iter != m_pvpTerrainCommonModel->end()) 
			bFind = true;
	}	

	////
	//#ifdef _DEBUG	
	//ofstream file("./FormInfo.txt", std::ios::out | std::ios::app);
	//if(!file){
	//	return true;
	//}
	//
	//file << "TILE : " << fX << " : " << fY <<  endl;
	//#endif


	if(m_pvpCurLayer && bFind)		
		m_pvpCurLayer->push_back( new SPTerrainUnit(&(*Iter), fX, fY, iArg0, iArg1, iArg2, iArg3, fScaleX, fScaleY) );  //jinhee [12/7/2004]

	if( bFind == false )
	{
		char szBuf[128];
		sprintf(szBuf, "TerrainForm::[TERRAIN_LAYER%d] Instance:[%010I64d] 이 존재하지 않습니다.", m_iCurLayer, iInstance);
		MessageBox(NULL, szBuf, "Error", MB_OK);
	}
	

	return true;
}

//SPTerrainModel* SPTerrainFormLoader::IsExistModel(
//	int iInstance, std::vector<SPTerrainModel*>* ppvLocalModel)
SPTerrainModel* SPTerrainFormLoader::IsExistModel(
	INT64 iInstance, std::vector<SPTerrainModel*>* ppvLocalModel)
{
	std::vector<SPTerrainModel*>::iterator Iter;
	bool	bFind = false;

	Iter = std::find_if(
		(*ppvLocalModel).begin(), 
		(*ppvLocalModel).end(), 
		std::bind2nd(IsEqualInstance, iInstance));

	if( Iter != (*ppvLocalModel).end() )
		bFind = true;
	else
	{
		if( m_pvpTerrainCommonModel )
		{
			Iter = std::find_if(
				m_pvpTerrainCommonModel->begin(), 
				m_pvpTerrainCommonModel->end(), 
				std::bind2nd(IsEqualInstance, iInstance));
		} 

		if( Iter != m_pvpTerrainCommonModel->end() ) 
			bFind = true;
	}

	if( bFind )
		return (*Iter);

	return NULL;
}



























