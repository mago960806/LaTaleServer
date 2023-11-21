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
#include "SPMOBAniTableLoader.h"

using namespace std;


//------------------------------------------------------------------------------------
SPMOBAniTableLoader::SPMOBAniTableLoader()
:
m_iDeep(0),
m_eAniIndex(ANI_NULL),
m_pAnimation(NULL)
{
}

//------------------------------------------------------------------------------------
SPMOBAniTableLoader::~SPMOBAniTableLoader()
{
	m_vpAnimation.clear();
}

//------------------------------------------------------------------------------------
bool SPMOBAniTableLoader::LoadAniPallet(std::vector<SPMOBAnimation*>& vpMobAnimation, string strResource)
{
	ANIMATION_INDEX	tmpIndex = ANI_NULL;
	MONSTER_INDEX tmpMobIndex = MON_TINY;

	if( strResource.empty() )
	{
		return false;
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

	string	str;	
	int		iCurLine = 0; // for Debug

	m_iDeep	= 0;
	m_eAniIndex	= ANI_NULL;
	m_eMonsterIndex = MON_TINY;

	m_vpAnimation.clear();
	while ( ReadLineFromStream( pStream, str) != -1) 
	{
		++iCurLine;
		Trim(str);

		EXCUTE_WORDCASE_MAP ( COMMENT				,		OnComment				)
		EXCUTE_WORDCASE_MAP ( BLOCK_BEGIN			,		OnBlockBegin			)
		EXCUTE_WORDCASE_MAP ( BLOCK_END				,		OnBlockEnd				)	
		EXCUTE_WORDCASE_MAP ( "ANITABLE_HEADER"		,		OnTextHeader			)

		// Set Animation Index.
		for(int i=ANI_MONSTER_BEGIN; i<=ANI_NPC_END; i++)
		{
			if ( true == isCase(g_strAnimationIndex[i], str.c_str() ) )
			{
				if( m_iDeep == 0 )
				{ 
					m_eAniIndex = (ANIMATION_INDEX)i;
					continue;
				}
			}
		}

		tmpIndex = m_eAniIndex;
		
		// Set Monster Size
		EXCUTE_WORDCASE_MAP("SIZE", OnSize)
		EXCUTE_WORDCASE_MAP("FRAME", OnFrame)
	}

	pStream->Release();
	//close(iFileHandle);	

	vector<SPMOBAnimation*>::iterator iter = m_vpAnimation.begin();
	while( iter != m_vpAnimation.end() )
	{
		vpMobAnimation.push_back(*iter);
		++iter;
	}

	m_pAnimation = NULL;
	m_eAniIndex = tmpIndex ;

	if( iCurLine==0 )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
bool SPMOBAniTableLoader::LoadBinaryAni(std::vector<SPMOBAnimation*>& vpMobAnimation,
	string strFileName, string strImageName)
{
	//FILE* pFile = fopen(strFileName.c_str(), "rb");
	//if( pFile == NULL )
	//	return false;

	SPStream* pStream;
	SPResourceBase::GetInstance()->GetStreamData( strFileName.c_str(), &pStream);
	if(pStream == NULL || !pStream->Valid())
	{
		SAFE_RELEASE(pStream);
		return false;
	}

	unsigned int uiClassID;
	int iAniNum, iSize, i, j;

	pStream->Read(&uiClassID, sizeof(unsigned int));
	pStream->Read(&iAniNum, sizeof(int));

	SPMOBAnimation* pkAnimation;
	for( i = 0; i < iAniNum; i++ )
	{
		pkAnimation = new SPMOBAnimation;
		pkAnimation->strImageName = strImageName;

		pStream->Read(&pkAnimation->eAniIndex, sizeof(ANIMATION_INDEX));
		pStream->Read(&pkAnimation->eMobSize, sizeof(MONSTER_INDEX));
		pStream->Read(&pkAnimation->eMobType, sizeof(MONSTER_TYPE));

		pStream->Read(&pkAnimation->fAccumulateTime, sizeof(float));
		pStream->Read(&pkAnimation->iFrameCountX, sizeof(int));
		pStream->Read(&pkAnimation->iFrameCountY, sizeof(int));
		pStream->Read(&pkAnimation->iFrameWidth, sizeof(int));
		pStream->Read(&pkAnimation->iFrameHeight, sizeof(int));
		
		pStream->Read(&iSize, sizeof(int));
		SPMOBAniFrameInfo AniFrameInfo;
		for( j = 0; j < iSize; j++ )
		{
			pStream->Read(&AniFrameInfo, sizeof(SPMOBAniFrameInfo));
			pkAnimation->vAniFrameInfo.push_back(AniFrameInfo);
		}

		vpMobAnimation.push_back(pkAnimation);
	}

	pStream->Release();
	//fclose(pFile);
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
		case 1:
		{
			if( m_eAniIndex == ANI_NULL )
				break;

			if( !m_pAnimation )
			{
				m_pAnimation = new SPMOBAnimation;
				m_pAnimation->eAniIndex = m_eAniIndex;		
				m_pAnimation->fAccumulateTime = 0.0f;
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
			if( m_eAniIndex == ANI_NULL )
				break;
			
			//assert(m_pAnimation);
			m_eAniIndex = ANI_NULL;
			if( m_pAnimation )
			{
				m_vpAnimation.push_back( m_pAnimation );
				m_pAnimation = NULL;				
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
WORDCASE_MAP( OnTextHeader )
{
	return true; 
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnFrame)
{
	CHECK_BLOCK_INVALID_FRAME()

	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;
	std::string		strTextureFile;
	
	GetSeperateString(strParam, vstrArg);
	Iter = vstrArg.begin();
	
	int iCount = 0;

	int* piAniData;
	float* pfAniData;

	piAniData = &m_AniFrameInfo.iNum;
	pfAniData = &m_AniFrameInfo.fScaleX;
	while( Iter != vstrArg.end() )
	{
		std::string szTemp = (*Iter);
		if( iCount < 6 )
		{
			*piAniData = atoi((*Iter).c_str());
			piAniData++;
		}
		else
		{
			*pfAniData = (float)atof((*Iter).c_str());
			pfAniData++;
		}

		++Iter;
		++iCount;
	}

	m_pAnimation->vAniFrameInfo.push_back(m_AniFrameInfo);
	m_pAnimation->fAccumulateTime += m_AniFrameInfo.fDelay;
	m_AniFrameInfo.SetNull();
	return true;
}

//------------------------------------------------------------------------------------
WORDCASE_MAP(OnSize)
{
	CHECK_BLOCK_INVALID_SIZE();

	assert(m_pAnimation);	
	string strArgPart;
	int		iIter;
	int		iIter2;

	iIter = (int)strParam.find(STR_SEP);
	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);

	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter +1 ));

	if( strcmp(strArgPart.c_str(), "TINY") == 0)
		m_pAnimation->eMobSize=MON_TINY;
	else if( strcmp(strArgPart.c_str(), "SMALL") == 0)
		m_pAnimation->eMobSize=MON_SMALL;
	else if( strcmp(strArgPart.c_str(), "MED") == 0)
		m_pAnimation->eMobSize=MON_MED;
	else if( strcmp(strArgPart.c_str(), "BIG") == 0)
		m_pAnimation->eMobSize=MON_BIG;

	iIter = iIter2;

	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter + 1));
	m_pAnimation->iFrameCountX = atoi(strArgPart.c_str());
	iIter = iIter2;

	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter + 1));
	m_pAnimation->iFrameCountY = atoi(strArgPart.c_str());
	iIter = iIter2;
	
	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter + 1));
	m_pAnimation->iFrameWidth = atoi(strArgPart.c_str());
	iIter = iIter2;

	iIter2 = (int)strParam.find(STR_SEP, iIter + 1);
	strArgPart = strParam.substr(iIter + 1, iIter2 - (iIter + 1));
	m_pAnimation->iFrameHeight = atoi(strArgPart.c_str());

	return true;
}