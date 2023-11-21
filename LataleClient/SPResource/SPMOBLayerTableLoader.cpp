#include <string>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <assert.h>

#ifdef _LATALE_CLIENT
#include "SPCommon.h"
#else 
#include "SPToolCommon.h"
#endif

#include "SPResourceDef.h"

#ifdef _LATALE_CLIENT
#include "SPMainGameDEF.h"
#endif

#include "SPGameObjectDEF.h"
#include "SPGOBClusterDEF.h"

#include "SPStream.h"
#include "SPMonsterModelUnitDef.h"
#include "SPUtil.h"
#include "SPFileDEF.H"
#include "SPMOBLayerTableLoader.h"

using namespace std;
using namespace mobLayer;

//------------------------------------------------------------------------------------
SPMOBLayerTableLoader::SPMOBLayerTableLoader()
: m_iDeep(0)
, m_pMOBLayerInfo(NULL)
{
}

//------------------------------------------------------------------------------------
SPMOBLayerTableLoader::~SPMOBLayerTableLoader()
{
	m_vpMOBLayerInfo.clear();
	if( m_pMOBLayerInfo )
		delete m_pMOBLayerInfo;

}

//------------------------------------------------------------------------------------
SPMOBLayerInfo* SPMOBLayerTableLoader::GetMOBLayerInfo(int iNo)
{
	if( iNo < 0 || iNo >= (int)m_vpMOBLayerInfo.size() )
		return NULL;

	return m_vpMOBLayerInfo[iNo];
}

//------------------------------------------------------------------------------------
bool SPMOBLayerTableLoader::LoadAniPallet(vector<SPMOBLayerInfo*>& vpMOBLayer, string strResource)
{
	if( strResource.empty() )
		return false;

	SPStream* pStream;
	SPResourceBase::GetInstance()->GetStreamData(strResource.c_str(), &pStream);
	if(pStream == NULL || !pStream->Valid())
	{
		SAFE_RELEASE(pStream);
		return false;
	}

	string	str;	
	int		iCurLine = 0; // for Debug

	m_iDeep	= 0;
	m_iLayerNum = 0;
	m_pMOBLayerInfo = 0;
	m_iCurLayerNo = -1;
	m_iBlockType = -1;
	m_eAniIndex = ANI_NULL;
	m_MOBLayerAnimation.eAniIndex = ANI_NULL;

	m_vpMOBLayerInfo.clear();
	while ( ReadLineFromStream( pStream, str) != -1 ) 
	{
		++iCurLine;
		Trim(str);

		EXCUTE_WORDCASE_MAP ( COMMENT				,		OnComment				)
		EXCUTE_WORDCASE_MAP ( BLOCK_BEGIN			,		OnBlockBegin			)
		EXCUTE_WORDCASE_MAP ( BLOCK_END				,		OnBlockEnd				)	

		// Set Animation Index.
		for(int i=ANI_MONSTER_BEGIN; i<=ANI_NPC_END; i++)
		{
			if ( true == isCase(g_strAnimationIndex[i], str.c_str() ) )
			{
				if( m_iDeep == 0 )
				{ 
					m_eAniIndex = (ANIMATION_INDEX)i;
					m_iBlockType = 1;
					continue;
				}
			}
		}
		
		EXCUTE_WORDCASE_MAP("LAYER",		OnLayer)
		EXCUTE_WORDCASE_MAP("SIZE",			OnSize)
		EXCUTE_WORDCASE_MAP("LAYERNO",		OnLayerNo)
		EXCUTE_WORDCASE_MAP("NAME",			OnName)
		EXCUTE_WORDCASE_MAP("IMAGE",		OnImage)
		EXCUTE_WORDCASE_MAP("FRAMEWIDTH",	OnFrameWidth)
		EXCUTE_WORDCASE_MAP("FRAMEHEIGHT",	OnFrameHeight)
		EXCUTE_WORDCASE_MAP("FRAMECOUNTX",	OnFrameCountX)
		EXCUTE_WORDCASE_MAP("FRAMECOUNTY",	OnFrameCountY)
		EXCUTE_WORDCASE_MAP("LIGHTMAP",		OnLightMap)

		EXCUTE_WORDCASE_MAP("FRAME",		OnFrame)
		
	}

	pStream->Release();
	vector<SPMOBLayerInfo*>::iterator iter = m_vpMOBLayerInfo.begin();
	while( iter != m_vpMOBLayerInfo.end() )
	{
		vpMOBLayer.push_back(*iter);
		++iter;
	}

	if( iCurLine == 0 )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
bool SPMOBLayerTableLoader::LoadBinaryAni(std::vector<SPMOBLayerInfo*>& vpMOBLayer, string strResource)
{
	if( strResource.empty() )
		return false;

	SPStream* pStream;
	SPResourceBase::GetInstance()->GetStreamData(strResource.c_str(), &pStream);
	if(pStream == NULL || !pStream->Valid())
	{
		SAFE_RELEASE(pStream);
		return false;
	}

	unsigned int uiClassID, uiVersion;
	int iLayerSize, iAniSize, i, j, k;
	char szBuf[32];
	SPMOBLayerInfo* pLayerInfo;
	SPMOBLayerAnimation Animation;
	SPMOBLayerAniFrameInfo FrameInfo;

	uiVersion = 0;
//#if defined(MOB_LIGHTMAP)
	pStream->Read(&uiVersion, sizeof(int));
//#endif

	pStream->Read(&uiClassID, sizeof(int));
	pStream->Read(&iLayerSize, sizeof(int));

	for( i = 0; i < iLayerSize; i++ )
	{
		pLayerInfo = new SPMOBLayerInfo;
		
		pStream->Read(szBuf, sizeof(szBuf));
		pLayerInfo->strName = szBuf;

		pStream->Read(szBuf, sizeof(szBuf));
#if defined(_USED_TEXTURE_TABLE)
		GetSeperateStringPeriod(szBuf, pLayerInfo->strImageName);
#else
		pLayerInfo->strImageName = szBuf;
#endif

		pStream->Read(&pLayerInfo->iFrameWidth, sizeof(int));
		pStream->Read(&pLayerInfo->iFrameHeight, sizeof(int));
		pStream->Read(&pLayerInfo->iFrameCountX, sizeof(int));
		pStream->Read(&pLayerInfo->iFrameCountY, sizeof(int));

		if( uiVersion > 0 )
			pStream->Read(&pLayerInfo->bLightMap, sizeof(bool));

		pStream->Read(&iAniSize, sizeof(int));
		for( j = 0; j < iAniSize; j++ )
		{
			Animation.Clear();

			pStream->Read(&Animation.eAniIndex, sizeof(ANIMATION_INDEX));
			pStream->Read(&Animation.fAccumulateTime, sizeof(float));
			pStream->Read(&Animation.iTotalFrame, sizeof(int));

			for( k = 0; k < Animation.iTotalFrame; k++ )
			{
				pStream->Read(&FrameInfo, sizeof(SPMOBLayerAniFrameInfo));
				Animation.vFrameInfo.push_back(FrameInfo);
			}

			pLayerInfo->vAnimation.push_back(Animation);
		}

		vpMOBLayer.push_back(pLayerInfo);
	}

	pStream->Release();
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnComment)
{
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnBlockBegin)
{
	m_iDeep++;		

	switch(m_iDeep) 
	{
	case 2:
		{
			if( m_iBlockType == 0 )
			{
				if( m_iCurLayerNo < 0 || m_pMOBLayerInfo )
					break;

				m_pMOBLayerInfo = new SPMOBLayerInfo;
			}
		}
		break;
	}

	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP( OnBlockEnd )
{
	assert(m_iDeep);	

	switch(m_iDeep) 
	{
	case 1:	// Size
		{
			if( m_iBlockType == 0 )
				m_iBlockType = -1;
			else if( m_iBlockType == 1 )
			{
				m_iBlockType = -1;
			}
		}
		break;

	case 2:
		{
			if( m_iBlockType == 0 )
			{
				if( m_pMOBLayerInfo )
					m_vpMOBLayerInfo.push_back(m_pMOBLayerInfo);

				m_pMOBLayerInfo = NULL;
			}
			else if( m_iBlockType == 1 )
			{
				SPMOBLayerInfo* pLayerInfo = GetMOBLayerInfo(m_iCurLayerNo);
				if( pLayerInfo == NULL )
					break;

				pLayerInfo->vAnimation.push_back(m_MOBLayerAnimation);
				
				m_MOBLayerAnimation.eAniIndex = ANI_NULL;
				m_MOBLayerAnimation.vFrameInfo.clear();
			}
		}
		break;

	default:
		break;
	}

	m_iDeep--;
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnLayer)
{
	CHECK_BLOCK_INVALID_LAYER();

	m_iBlockType = 0;
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnSize)
{
	CHECK_BLOCK_INVALID_SIZE();

	string strArgPart;
	int		iIter;
	int		iIter2;

	iIter = (int)strParam.find(STR_SEP);
	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter +1 ));

	m_iLayerNum = atoi(strArgPart.c_str());
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnLayerNo)
{
	CHECK_BLOCK_INVALID_LAYERNO();

	string strArgPart;
	int		iIter;
	int		iIter2;

	iIter = (int)strParam.find(STR_SEP);
	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter +1 ));

	if( m_iBlockType == 0 )
		m_iCurLayerNo = atoi(strArgPart.c_str());
	else if( m_iBlockType == 1 )
	{
		if( m_eAniIndex == ANI_NULL )
			return false;

		m_MOBLayerAnimation.eAniIndex = m_eAniIndex;

		m_iCurLayerNo = atoi(strArgPart.c_str());
		iIter = iIter2;
		
		iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
		strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter + 1));
		m_MOBLayerAnimation.fAccumulateTime = (float)atof(strArgPart.c_str());
		iIter = iIter2;

		iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
		strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter + 1));
		m_MOBLayerAnimation.iTotalFrame = atoi(strArgPart.c_str());
		m_MOBLayerAnimation.vFrameInfo.clear();
	}

	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnName)
{
	if( m_pMOBLayerInfo == NULL )
		return false;

	string strArgPart;
	int		iIter;
	int		iIter2;

	iIter = (int)strParam.find(STR_SEP);
	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter +1 ));

	m_pMOBLayerInfo->strName = strArgPart;
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnImage)
{
	if( m_pMOBLayerInfo == NULL )
		return false;

	string strArgPart;
	int		iIter;

	iIter = (int)strParam.find(STR_SEP);
	strArgPart = strParam.substr(iIter + 1);

	m_pMOBLayerInfo->strImageName = strArgPart;
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnFrameWidth)
{
	if( m_pMOBLayerInfo == NULL )
		return false;

	string strArgPart;
	int		iIter;
	int		iIter2;

	iIter = (int)strParam.find(STR_SEP);
	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter +1 ));

	m_pMOBLayerInfo->iFrameWidth = atoi(strArgPart.c_str());
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnFrameHeight)
{
	if( m_pMOBLayerInfo == NULL )
		return false;

	string strArgPart;
	int		iIter;
	int		iIter2;

	iIter = (int)strParam.find(STR_SEP);
	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter +1 ));

	m_pMOBLayerInfo->iFrameHeight = atoi(strArgPart.c_str());
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnFrameCountX)
{
	if( m_pMOBLayerInfo == NULL )
		return false;

	string strArgPart;
	int		iIter;
	int		iIter2;

	iIter = (int)strParam.find(STR_SEP);
	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter +1 ));

	m_pMOBLayerInfo->iFrameCountX = atoi(strArgPart.c_str());
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnFrameCountY)
{
	if( m_pMOBLayerInfo == NULL )
		return false;

	string strArgPart;
	int		iIter;
	int		iIter2;

	iIter = (int)strParam.find(STR_SEP);
	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter +1 ));

	m_pMOBLayerInfo->iFrameCountY = atoi(strArgPart.c_str());
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnLightMap)
{
	if( m_pMOBLayerInfo == NULL )
		return false;

	string strArgPart;
	int		iIter;
	int		iIter2;

	iIter = (int)strParam.find(STR_SEP);
	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter +1 ));
	int iLightMap = atoi(strArgPart.c_str());

	m_pMOBLayerInfo->bLightMap = iLightMap ? true : false;
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnFrame)
{
	CHECK_BLOCK_INVALID_FRAME()

	vector<string> vstrArg;
	vector<string>::iterator Iter;
	string		strTextureFile;

	GetSeperateString(strParam, vstrArg);
	Iter = vstrArg.begin();

	SPMOBLayerAniFrameInfo FrameInfo;
	FrameInfo.iNo = atoi((*Iter).c_str());
	++Iter;

	FrameInfo.iZOrder = atoi((*Iter).c_str());
	++Iter;

	FrameInfo.bVisible = (atoi((*Iter).c_str()) ? true : false);
	++Iter;

	FrameInfo.iResourceIndex = atoi((*Iter).c_str());
	++Iter;

	FrameInfo.iPosX = atoi((*Iter).c_str());
	++Iter;

	FrameInfo.iPosY = atoi((*Iter).c_str());
	++Iter;

	FrameInfo.iRotation = atoi((*Iter).c_str());
	++Iter;

	FrameInfo.fScaleX = (float)atof((*Iter).c_str());
	++Iter;

	FrameInfo.fScaleY = (float)atof((*Iter).c_str());
	++Iter;
	
	FrameInfo.fColorR = (float)atof((*Iter).c_str());
	++Iter;

	FrameInfo.fColorG = (float)atof((*Iter).c_str());
	++Iter;

	FrameInfo.fColorB = (float)atof((*Iter).c_str());
	++Iter;

	FrameInfo.fColorA = (float)atof((*Iter).c_str());
	++Iter;

	FrameInfo.fDelay = (float)atof((*Iter).c_str());
	++Iter;

	m_MOBLayerAnimation.vFrameInfo.push_back(FrameInfo);
	return true;
}