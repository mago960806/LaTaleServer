
#include <string>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <assert.h>


#ifdef _LATALE_SERVER
	#include <WTypes.h>	
	#include "SPResourceDef.h"

	//#define RES_WORK_RESOURCE_PATH					"//BELOS/RESOURCE/SERVER/"

#endif


#ifdef _LATALE_CLIENT	
	#include "SPCommon.h"	
	#include "SPResourceDef.h"
	//TODO : Client Side

#endif 

#ifdef _MAP_TOOL
	#include "SPCommon.h"	
	#include "SPResourceDef.h"
	//TODO : MapTool Side
#endif


#include "SPUtil.h"
#include "SPFileDEF.H"
#include "SPStream.h"
#include "SPTerrainAttributeLoader.h"

//
#ifdef _DEBUG
#include <iostream>			//jinhee - [2005/1/3] //테스트를 위한...
#include <fstream>
#endif

using namespace std;


SPTerrainAttributeLoader::SPTerrainAttributeLoader()
:m_iCXTerrain	( 0 )
,m_iCYTerrain	( 0 )
,m_iDeep		( 0 )
,m_ppCurLayer	( NULL )
,m_pAttrLayer1	( NULL )
,m_pAttrLayer2	( NULL )
,m_pAttrLayer3	( NULL )
{}

SPTerrainAttributeLoader::~SPTerrainAttributeLoader() {}


bool SPTerrainAttributeLoader::LoadDataText(const char* pszFile, 
											BYTE** ppLayer1, 
											BYTE** ppLayer2,
											BYTE** ppLayer3,
											int& iSize,
											long lGroupWidth /*= 0*/,
											long lGroupHeight /*= 0*/,
											const char* strPath /*= NULL*/)
{
	//std::string strResource = RES_WORK_RESOURCE_PATH;		
	std::string strResource = "";		
	std::string strNewPath = strPath;	

	if(strNewPath != ""){		
		strResource = strPath;			//파라미터 패스 체크 jinhee 04.11.22
	}

	if(pszFile) {	
		strResource += pszFile;
	} else {
		strResource += RES_FNAME_TERRAINATTRIBUTE;
	}

#ifdef _MAP_TOOL
	TRACE("Attribute Load strResource [%s]\n", strResource.c_str());
#endif

#ifdef _LATALE_SERVER
	//printf("strResource = [%s]\n",strResource.c_str());
#endif

	//int iFileHandle = open(strResource.c_str(), O_RDONLY);
	//if (iFileHandle == -1) return false;	

	SPStream* pStream;
	//SPResourceBase::GetInstance()->GetStreamData( strResource.c_str(), &pStream);

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
		return false;
	}

	string str;	
	m_lGroupWidth = lGroupWidth;
	m_lGroupHeight = lGroupHeight;

	
	while ( ReadLineFromStream(pStream, str) != -1) {
		Trim(str);		
		EXCUTE_WORDCASE_MAP ( COMMENT					,		OnComment				)
		EXCUTE_WORDCASE_MAP ( BLOCK_BEGIN				,		OnBlockBegin			)
		EXCUTE_WORDCASE_MAP ( BLOCK_END					,		OnBlockEnd				)		

		EXCUTE_WORDCASE_MAP ( "TERRAINATTRIBUTE_HEADER"	,		OnHeader				)
		EXCUTE_WORDCASE_MAP ( "TERRAIN_LAYER_CX"		,		OnTerrainLayerCX		)
		EXCUTE_WORDCASE_MAP ( "TERRAIN_LAYER_CY"		,		OnTerrainLayerCY		)		
		EXCUTE_WORDCASE_MAP ( "TERRAIN_ATTR_LAYER1"		,		OnTerrainAttrLayer1		)
		EXCUTE_WORDCASE_MAP ( "TERRAIN_ATTR_LAYER2"		,		OnTerrainAttrLayer2		)
		EXCUTE_WORDCASE_MAP ( "TERRAIN_ATTR_LAYER3"		,		OnTerrainAttrLayer3		)
		EXCUTE_WORDCASE_MAP ( "ATTR"					,		OnAttr					)
	}

	pStream->Release();	

	(*ppLayer1) = m_pAttrLayer1;
	(*ppLayer2) = m_pAttrLayer2;
	(*ppLayer3) = m_pAttrLayer3;
	iSize		= m_iAttrSize;

	return true;
}

bool SPTerrainAttributeLoader::LoadBinary(const char* pszFile, 
	BYTE** ppLayer1, BYTE** ppLayer2, BYTE** ppLayer3,
	int& iSize, long lGroupWidth, long lGroupHeight, const char* strPath)
{
	//std::string strResource = RES_WORK_RESOURCE_PATH;		
	std::string strResource = "";		
	std::string strNewPath = strPath;	

	if(strNewPath != "")
		strResource = strPath;

	if(pszFile)
		strResource += pszFile;
	else
		strResource += RES_FNAME_TERRAINATTRIBUTE;

#ifdef _MAP_TOOL
	TRACE("Attribute Load strResource [%s]\n", strResource.c_str());
#endif

#ifdef _LATALE_SERVER
	//printf("strResource = [%s]\n",strResource.c_str());
#endif

	//SPStream* pkStream = new SPFileStream(strResource.c_str());

	SPStream* pkStream;
	//SPResourceBase::GetInstance()->GetStreamData(strResource.c_str(), &pkStream);

#ifdef _LATALE_SERVER
	pkStream = new SPFileStream( strResource.c_str() );	
#else
	SPResourceBase::GetInstance()->GetStreamData( strResource.c_str(), &pkStream);
#endif



	if( !pkStream->Valid() )
	{
		// AJJIYA [5/24/2005]
		// 서버에서도 사용중 매크로가 없내용..
		// SAFE_RELEASE(pkStream);
		if( pkStream )
		{
			pkStream->Release();
			pkStream	=	NULL;
		}
		return false;
	}

	pkStream->Read(&m_iCXTerrain, sizeof(int));
	pkStream->Read(&m_iCYTerrain, sizeof(int));
	
	m_lGroupWidth = lGroupWidth;
	m_lGroupHeight = lGroupHeight;

	CONVERT_ATTRIBUTE_INFO AttributeInfo;
	int iAttrSize, iIndex, i, j;

	const int iLenX = m_iCXTerrain / 32;
	const int iLenY = m_iCYTerrain / 16;
	
	m_iAttrSize = iLenX * iLenY;
	BYTE* ucAttrLayer[3];

	for( i = 0; i < 3; i++ )
	{
		ucAttrLayer[i] = new BYTE[m_iAttrSize];
		memset(ucAttrLayer[i], 0, m_iAttrSize);

		pkStream->Read(&iAttrSize, sizeof(int));
		for( j = 0; j < iAttrSize; j++ )
		{
			pkStream->Read(&AttributeInfo, sizeof(CONVERT_ATTRIBUTE_INFO));

			iIndex = AttributeInfo.iDiffX + iLenX * AttributeInfo.iDiffY;
			ucAttrLayer[i][iIndex] = AttributeInfo.ucType;
		}
	}

	(*ppLayer1) = ucAttrLayer[0];
	(*ppLayer2) = ucAttrLayer[1];
	(*ppLayer3) = ucAttrLayer[2];
	iSize = m_iAttrSize;

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
	if(m_ppCurLayer) {
		// Attr Stat
		m_ppCurLayer = NULL;
	} else {
		// Header Stat
		const int iLenX = m_iCXTerrain / 32;
		const int iLenY = m_iCYTerrain / 16;

		assert( m_pAttrLayer1 == NULL );
		assert( m_pAttrLayer2 == NULL );
		assert( m_pAttrLayer3 == NULL );

		m_iAttrSize = iLenX * iLenY;

		m_pAttrLayer1 = new BYTE[m_iAttrSize];
		m_pAttrLayer2 = new BYTE[m_iAttrSize];
		m_pAttrLayer3 = new BYTE[m_iAttrSize];

		memset(m_pAttrLayer1, 0, m_iAttrSize);
		memset(m_pAttrLayer2, 0, m_iAttrSize);
		memset(m_pAttrLayer3, 0, m_iAttrSize);
	}
	--m_iDeep;
	return true;
}

WORDCASE_MAP( OnHeader	)
{
	CHECK_BLOCK_INVALID( 0 )
	return true;
}


WORDCASE_MAP( OnTerrainLayerCX		)
{
	CHECK_BLOCK_INVALID( 1 )	

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	m_iCXTerrain = (int)(float)atof(strParam.c_str());
	return true;
}

WORDCASE_MAP( OnTerrainLayerCY		)
{
	CHECK_BLOCK_INVALID( 1 )

	string::iterator Iter =	std::find(strParam.begin(), strParam.end(), STR_EQ);
	strParam.erase( strParam.begin(), Iter+1);
	Trim(strParam);
	m_iCYTerrain = (int)(float)atof(strParam.c_str());
	return true;
}

WORDCASE_MAP( OnTerrainAttrLayer1		)
{
	CHECK_BLOCK_INVALID( 0 )
	m_ppCurLayer = &m_pAttrLayer1;
	return true;
}

WORDCASE_MAP( OnTerrainAttrLayer2		)
{
	CHECK_BLOCK_INVALID( 0 )
	m_ppCurLayer = &m_pAttrLayer2;
	return true;
}

WORDCASE_MAP( OnTerrainAttrLayer3		)
{
	CHECK_BLOCK_INVALID( 0 )
		m_ppCurLayer = &m_pAttrLayer3;
	return true;
}

WORDCASE_MAP( OnAttr					)
{
	CHECK_BLOCK_INVALID( 1 )

	assert(m_ppCurLayer && "Invalid Terrain Layer");
	if(m_ppCurLayer == NULL) return false;

	//	ATTR : 1 : 0 : 32 : 1024
	int iInstance = 0;
	bool bInvert = false;
	long lX = 0;			//int를 long 으로 형변경 jinhee - [2005/1/17]
	long lY = 0;
	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;

	int iNOArg = GetSeperateString(strParam, vstrArg);

	assert(iNOArg == 4);

	Iter = vstrArg.begin();

	if(Iter != vstrArg.end()) {
		iInstance = atoi((*Iter).c_str());
		assert(iInstance < 16);
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		bInvert = (atoi((*Iter).c_str())) ? true : false;
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		lX = atoi((*Iter).c_str());	
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		lY = atoi((*Iter).c_str());
	}

	int iLenX = (int)m_iCXTerrain / 32;
	int iFX = (lX) / 32;
	//int iFY = (iY) / 16 + 1;
	int iFY = (lY) / 16;					//jinhee - 04.12.02

	BYTE ucBlock = (BYTE)(iInstance & 0x0000000F); 
	if(bInvert) 
		ucBlock = ucBlock | (1<<4);

	long iArrayIndex = iFX + (iLenX * iFY);
	
	*( (*m_ppCurLayer) + (iFX+((iLenX)*(iFY)))) = ucBlock;

//#ifdef _MAP_TOOL
//	long lIndex = (long)(iFX+((iLenX)*(iFY)));
//	TRACE("Load Attribute[%d] iX[%d] iFX[%d] - iY[%d] iFY[%d]\n", lIndex, iX, iFX, iY, iFY);
//#endif

	////
	//#ifdef _DEBUG	
	//ofstream file("./AttrInfo.txt", std::ios::out | std::ios::app);
	//if(!file){
	//	return true;
	//}
	//int iLenY = m_iCYTerrain / 16;
	//int iSize = iLenX * iLenY;
	//file << "Attr : " << iInstance << " : " << bInvert << " : " << lX  << " : " << lY << "  ArrayIndex : " << iArrayIndex  << " iFX : " << iFX << " iFX : " << iFY << " m_iATTRSize : " << iSize << endl;
	//#endif

	return true;
}























