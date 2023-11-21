

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

#ifdef _LATALE_CLIENT
	#include "SPManager.h"
	#include "SPGOBClusterDEF.h"
	#include "SPAvatarModelUnitDef.h"
	#include "SPGOBAnimation.h"
	#include "SPGOBModel.h"
	#include "SPGOBCluster.h"	
	//#include "SPMainGameManager.h"
	//#include "SPDebug.h"
#else 
	#include "SPGOBClusterDEF.h"
	#include "SPAvatarModelUnitDef.h"
	#include "SPGOBAnimation.h"
	#include "SPGOBModel.h"
	#include "SPGOBCluster.h"
#endif

#include "SPStream.h"
#include "SPMonsterModelUnitDef.h"
#include "SPUtil.h"
#include "SPFileDEF.H"
#include "SPAniTableLoader.h"

using namespace std;

const int MAX_KEY	= 10;

class KeyBinder {
public:
	struct KeyAdjust {
		PART_LAYER	KeyPartLayer;
		int			iNOSubPart;
		struct _SubPartList {		
			PART_LAYER  SubPartLayer;
			int			iMoveX;
			int			iMoveY;		
		} SubPartList[8];
	};

	KeyAdjust KeyData[MAX_KEY];

	KeyBinder() 
	{
		ZeroMemory(KeyData, sizeof(KeyData));

		// 키레이어10개		// 키를제외한 따라야할 레이어 갯수		
		KeyData[0].KeyPartLayer = PL_ARM_OUT;
		KeyData[0].iNOSubPart	= 2;
		KeyData[0].SubPartList[0].SubPartLayer	= PL_HAND_ARM_OUT;
		KeyData[0].SubPartList[0].iMoveX		= 0;
		KeyData[0].SubPartList[0].iMoveY		= 0;
		KeyData[0].SubPartList[1].SubPartLayer	= PL_BLOUSE_ARM_OUT;
		KeyData[0].SubPartList[1].iMoveX		= 0;
		KeyData[0].SubPartList[1].iMoveY		= 0;

		KeyData[1].KeyPartLayer = PL_HAND_OUT;
		KeyData[1].iNOSubPart	= 1;
		KeyData[1].SubPartList[0].SubPartLayer	= PL_GLOVE_OUT;
		KeyData[1].SubPartList[0].iMoveX		= 0;
		KeyData[1].SubPartList[0].iMoveY		= 0;

		KeyData[2].KeyPartLayer = PL_LEG_OUT;
		KeyData[2].iNOSubPart	= 2;
		KeyData[2].SubPartList[0].SubPartLayer	= PL_PANTS_LEG_OUT;
		KeyData[2].SubPartList[0].iMoveX		= 0;
		KeyData[2].SubPartList[0].iMoveY		= 0;
		KeyData[2].SubPartList[1].SubPartLayer	= PL_FOOT_LEG_OUT;
		KeyData[2].SubPartList[1].iMoveX		= 0;
		KeyData[2].SubPartList[1].iMoveY		= 0;

		KeyData[3].KeyPartLayer = PL_FOOT_OUT;
		KeyData[3].iNOSubPart	= 1;
		KeyData[3].SubPartList[0].SubPartLayer	= PL_SHOE_OUT;
		KeyData[3].SubPartList[0].iMoveX		= 0;
		KeyData[3].SubPartList[0].iMoveY		= 0;

		KeyData[4].KeyPartLayer = PL_BODY;			// BODY가 Parent이다..Parent 중심
		KeyData[4].iNOSubPart	= 3;
		KeyData[4].SubPartList[0].SubPartLayer	= PL_BLOUSE;
		KeyData[4].SubPartList[0].iMoveX		= 30;
		KeyData[4].SubPartList[0].iMoveY		= 0;
		KeyData[4].SubPartList[1].SubPartLayer	= PL_PANTS;
		KeyData[4].SubPartList[1].iMoveX		= 9;
		KeyData[4].SubPartList[1].iMoveY		= -32;
		KeyData[4].SubPartList[2].SubPartLayer	= PL_COAT;
		KeyData[4].SubPartList[2].iMoveX		= -13;
		KeyData[4].SubPartList[2].iMoveY		= -32;

		KeyData[5].KeyPartLayer = PL_LEG_IN;
		KeyData[5].iNOSubPart	= 2;
		KeyData[5].SubPartList[0].SubPartLayer	= PL_PANTS_LEG_IN;
		KeyData[5].SubPartList[0].iMoveX		= 0;
		KeyData[5].SubPartList[0].iMoveY		= 0;
		KeyData[5].SubPartList[1].SubPartLayer	= PL_FOOT_LEG_IN;
		KeyData[5].SubPartList[1].iMoveX		= 0;
		KeyData[5].SubPartList[1].iMoveY		= 0;

		KeyData[6].KeyPartLayer = PL_FOOT_IN;
		KeyData[6].iNOSubPart	= 1;
		KeyData[6].SubPartList[0].SubPartLayer	= PL_SHOE_IN;
		KeyData[6].SubPartList[0].iMoveX		= 0;
		KeyData[6].SubPartList[0].iMoveY		= 0;

		KeyData[7].KeyPartLayer = PL_HEAD;
		KeyData[7].iNOSubPart	= 8;
		KeyData[7].SubPartList[0].SubPartLayer	= PL_CAP_FRONT;
		KeyData[7].SubPartList[0].iMoveX		= -32;
		KeyData[7].SubPartList[0].iMoveY		= -26;
		KeyData[7].SubPartList[1].SubPartLayer	= PL_GOGGLE;
		KeyData[7].SubPartList[1].iMoveX		= 0;
		KeyData[7].SubPartList[1].iMoveY		= 0;
		KeyData[7].SubPartList[2].SubPartLayer	= PL_EAR;
		KeyData[7].SubPartList[2].iMoveX		= 0;
		KeyData[7].SubPartList[2].iMoveY		= 0;
		KeyData[7].SubPartList[3].SubPartLayer	= PL_HAIR_FRONT;
		KeyData[7].SubPartList[3].iMoveX		= -10;
		KeyData[7].SubPartList[3].iMoveY		= -7;
		KeyData[7].SubPartList[4].SubPartLayer	= PL_HAIR_REAR;
		KeyData[7].SubPartList[4].iMoveX		= -10;
		KeyData[7].SubPartList[4].iMoveY		= -26;
		KeyData[7].SubPartList[5].SubPartLayer	= PL_FACE;
		KeyData[7].SubPartList[5].iMoveX		= 0;
		KeyData[7].SubPartList[5].iMoveY		= 0;
		KeyData[7].SubPartList[6].SubPartLayer	= PL_MAKEUP;
		KeyData[7].SubPartList[6].iMoveX		= 0;
		KeyData[7].SubPartList[6].iMoveY		= 0;
		KeyData[7].SubPartList[7].SubPartLayer	= PL_CAP_REAR;
		KeyData[7].SubPartList[7].iMoveX		= -32;
		KeyData[7].SubPartList[7].iMoveY		= -26;

		KeyData[8].KeyPartLayer = PL_ARM_IN;
		KeyData[8].iNOSubPart	= 2;
		KeyData[8].SubPartList[0].SubPartLayer	= PL_BLOUSE_ARM_IN;
		KeyData[8].SubPartList[0].iMoveX		= 0;
		KeyData[8].SubPartList[0].iMoveY		= 0;
		KeyData[8].SubPartList[1].SubPartLayer	= PL_HAND_ARM_IN;
		KeyData[8].SubPartList[1].iMoveX		= 0;
		KeyData[8].SubPartList[1].iMoveY		= 0;

		KeyData[9].KeyPartLayer = PL_HAND_IN;
		KeyData[9].iNOSubPart	= 1;
		KeyData[9].SubPartList[0].SubPartLayer	= PL_GLOVE_IN;
		KeyData[9].SubPartList[0].iMoveX		= 0;
		KeyData[9].SubPartList[0].iMoveY		= 0;
	}
};




struct _IsEqualSeqPartFromPartLayer : binary_function<SPGOBAniSeqPart, PART_LAYER, bool> {
	bool operator() (SPGOBAniSeqPart GOBPart, PART_LAYER PartLayer) const
	{		
		return (GOBPart.PartLayer == PartLayer);
	}
} IsEqualSeqPartFromPartLayer;


SPAniTableLoader::SPAniTableLoader()
: m_iDeep(0)
, m_pAnimation	(NULL)
{
	m_AniSeqPart.SetNull();
	m_AniSeq.SetNull();

	for(int layer = (int)PL_FIRST; layer < (int)PL_LAST; ++layer) {
		m_mStrToLayer[ g_cstrPartLayer[layer] ] = (PART_LAYER)layer;
	}
}

SPAniTableLoader::~SPAniTableLoader()
{
	m_vpAnimation.clear();
}

bool SPAniTableLoader::AssignPartLayerValue( const std::string strParam, SPGOBAniSeqPart& AniSeqPart)
{	
	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;
	std::string		strTextureFile;

	int iNOArg = GetSeperateString(strParam, vstrArg);
//#ifdef _LATALE_CLIENT
	//assert(iNOArg == 4);		// 임시 -_-)
//#else
	//assert(iNOArg == 5);		// 임시로 닫아둠 -_-)/
//#endif

	Iter = vstrArg.begin();

	if(Iter != vstrArg.end()) {
		m_AniSeqPart.iRX	= atoi((*Iter).c_str());
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		m_AniSeqPart.iRY = atoi((*Iter).c_str());
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		m_AniSeqPart.iRotationDegree = atoi((*Iter).c_str());
		++Iter;
	}

	if(Iter != vstrArg.end()) {
		m_AniSeqPart.iResourceIndex = atoi((*Iter).c_str());
		++Iter;
	}

	if(Iter != vstrArg.end()) 
	{
		m_AniSeqPart.bVisable = (atoi((*Iter).c_str())==1)?true:false;
		++Iter;
	}

	if(Iter != vstrArg.end()) 
	{
		m_AniSeqPart.bFlip = (atoi((*Iter).c_str())==1)?true:false;
		++Iter;
	}

#ifdef _CHARTOOL
	
#endif
	return true;
}



bool SPAniTableLoader::AdjustPartLayer()
{
	KeyBinder Binder;

	std::vector<SPGOBAnimation*>::iterator IterAni = m_vpAnimation.begin();
	std::vector<SPGOBAniSeq>::iterator IterSeq;
	std::vector<SPGOBAniSeqPart>::iterator IterKeyParent;
	std::vector<SPGOBAniSeqPart>::iterator IterChild;	

	for(;IterAni != m_vpAnimation.end(); ++IterAni) {
		IterSeq = (*IterAni)->vAniSeq.begin();
		for(;IterSeq != (*IterAni)->vAniSeq.end(); ++IterSeq) {
			for(int iKey = 0; iKey < MAX_KEY; ++iKey) {
				IterKeyParent = std::find_if(
					(*IterSeq).vAniSeqPart	.begin(), 
					(*IterSeq).vAniSeqPart	.end(), 
					std::bind2nd(IsEqualSeqPartFromPartLayer, Binder.KeyData[iKey].KeyPartLayer));			

				if(IterKeyParent == (*IterSeq).vAniSeqPart.end()) {
					assert(0 && "Parent Key not found");
				} else {
					for(int iSubPart = 0; iSubPart < Binder.KeyData[iKey].iNOSubPart; ++iSubPart) {
						IterChild = std::find_if(
							(*IterSeq).vAniSeqPart	.begin(), 
							(*IterSeq).vAniSeqPart	.end(), 
							std::bind2nd(IsEqualSeqPartFromPartLayer, (Binder.KeyData[iKey].SubPartList)[iSubPart].SubPartLayer));
						if(IterChild == (*IterSeq).vAniSeqPart.end() ) {
							assert(0 && "SubPart not found");
						} else {
							(*IterChild).iRX = (*IterKeyParent).iRX + (Binder.KeyData[iKey].SubPartList)[iSubPart].iMoveX;
							(*IterChild).iRY = (*IterKeyParent).iRY + (Binder.KeyData[iKey].SubPartList)[iSubPart].iMoveY;
							if((*IterChild).iResourceIndex == 0) { 
								(*IterChild).iResourceIndex = (*IterKeyParent).iResourceIndex;
							}
							(*IterChild).iRotationDegree = (*IterKeyParent).iRotationDegree;
							//(*IterChild).bVisable = (*IterKeyParent).bVisable;
						}
					}
				}
			}
		}
	}
	
	return true;
}

bool SPAniTableLoader::LoadAniPallet(SPGOBAnimation** ppAnimation, string strResource)
{	
	if(strResource.empty())
	{
		//strResource = RES_WORK_RESOURCE_PATH;
		strResource = RES_FNAME_COMMONANITABLE2;
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

	m_iDeep			= 0;
	m_vpAnimation.clear();		// add kkandori
	while ( ReadLineFromStream(pStream, str) != -1) 
	{
		++iCurLine;
		Trim(str);

		EXCUTE_WORDCASE_MAP ( COMMENT				,		OnComment				)
		EXCUTE_WORDCASE_MAP ( BLOCK_BEGIN			,		OnBlockBegin			)
		EXCUTE_WORDCASE_MAP ( BLOCK_END				,		OnBlockEnd				)	
		EXCUTE_WORDCASE_MAP ( "ANITABLE_HEADER"		,		OnTextHeader			) 		
		EXCUTE_WORDCASE_MAP ( "ANI_CHARACTER"		,		OnAniIndex			) 		

		// Total Animation Time
		EXCUTE_WORDCASE_MAP ( "ACCUMULATE_TIME"		,		OnAccumulateTime		) 

		// Set Sequence Number, Delay Time.
		EXCUTE_WORDCASE_MAP ( "SEQ"					,		OnSeq					) 

		// Set Sequence Part Layer.
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[1]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[2]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[3]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[4]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[5]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[6]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[7]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[8]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[9]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[10]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[11]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[12]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[13]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[14]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[15]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[16]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[17]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[18]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[19]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[20]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[21]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[22]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[23]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[24]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[25]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[26]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[27]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[28]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[29]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[30]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[31]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[32]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[33]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[34]	,		OnSeqPart		)
		EXCUTE_WORDCASE_MAP ( g_cstrPartLayer[35]	,		OnSeqPart		)
	}


	pStream->Release();

	AdjustPartLayer();

	
	/*for(int i = 0; i < (signed)m_vpAnimation.size(); ++i) {
		vpAnimation.push_back(NULL);
	}
	std::copy( m_vpAnimation.begin(), m_vpAnimation.end(), vpAnimation.begin());*/

	if(iCurLine==0 || m_vpAnimation.size()==0)
		return false;

	vector<SPGOBAnimation*>::iterator iter = m_vpAnimation.begin();
	*ppAnimation = (*iter);
	
	return true;
}

bool SPAniTableLoader::LoadBinaryAni(SPGOBAnimation** ppAnimation, string strResource)
{	
	SPStream* pStream;
	SPResourceBase::GetInstance()->GetStreamData( strResource.c_str(), &pStream);

	if( pStream == NULL || !pStream->Valid())
	{
		SAFE_RELEASE(pStream);
		return false;
	}
		
	m_pAnimation = new SPGOBAnimation;
	
	unsigned int uiAniIndex;

	pStream->Read( &uiAniIndex, sizeof(unsigned int));
	pStream->Read( &m_pAnimation->fAccumulateTime, sizeof(float));
	pStream->Read( &m_pAnimation->uiAniInx, sizeof(unsigned int));


	int iSeqSize, iPartSize, i, j;
	pStream->Read( &iSeqSize, sizeof(int));
	
	SPGOBAniSeq AniSeqInfo;
	SPGOBAniSeqPart AniSeqPartInfo;

	for( i = 0; i < iSeqSize; i++ )
	{
		pStream->Read(&AniSeqInfo.iSeqNum, sizeof(int));
		pStream->Read(&AniSeqInfo.fDelayTime, sizeof(float));

		pStream->Read(&iPartSize, sizeof(int));
		for( j = 0; j < iPartSize; j++ )
		{
			pStream->Read(&AniSeqPartInfo, sizeof(SPGOBAniSeqPart));
			AniSeqInfo.vAniSeqPart.push_back(AniSeqPartInfo);
		}

		m_pAnimation->vAniSeq.push_back(AniSeqInfo);
		AniSeqInfo.vAniSeqPart.clear();
	}

	*ppAnimation = m_pAnimation;
	m_pAnimation = NULL;

	pStream->Release();
	return true;
}

WORDCASE_MAP( OnComment )
{
	return true;
}

WORDCASE_MAP( OnBlockBegin )
{
	m_iDeep++;		

	switch(m_iDeep) {
		case 1:	// Animation		
		{
			if(!m_pAnimation)
			{
				m_pAnimation = new SPGOBAnimation;
				m_pAnimation->fAccumulateTime	= 0.0f;
			}
		}
		break;
	}

	return true;
}

WORDCASE_MAP( OnBlockEnd )
{
	assert(m_iDeep);	

	switch(m_iDeep) 
	{
		case 1:	// Animation		
		{
			if(m_pAnimation)
			{
				m_vpAnimation.push_back( m_pAnimation );
				m_pAnimation = NULL;				
			}			
		}
		break;

		case 2:	// Sequence
		{
			if(m_pAnimation)
			{
				assert(m_pAnimation);
				
				//assert(!m_AniSeq.isNull());
				if(m_AniSeq.isNull())
					break;

				m_pAnimation->vAniSeq.push_back( m_AniSeq );
				m_AniSeq.SetNull();
			}
			//SAFE_DELETE( m_pAniSeq );	
		}
		break;
	default:
		break;
	}
	m_iDeep--;
	return true;
}

WORDCASE_MAP( OnTextHeader )
{
	return true; 
}

WORDCASE_MAP( OnAniIndex )
{
	return true; 
}

WORDCASE_MAP(OnSeq)
{
	CHECK_BLOCK_INVALID_SEQ()

	assert(m_AniSeq.isNull());	

	string strArgPart;
	int		iIter;
	int		iIter2;
	
	iIter = strParam.find(STR_SEP);
	iIter2 = strParam.find(STR_SEP, iIter+1);
	
	strArgPart = strParam.substr( iIter+1,  iIter2 - (iIter+1) );
	m_AniSeq.iSeqNum = atoi(strArgPart.c_str());
	strArgPart.clear();
	
	strArgPart = strParam.substr( iIter2+1);	
	m_AniSeq.fDelayTime = (float)atof(strArgPart.c_str());

	// add kkandori
	assert(m_pAnimation);
	m_pAnimation->fAccumulateTime += m_AniSeq.fDelayTime;

	return true;
}

WORDCASE_MAP( OnAccumulateTime )
{	
	CHECK_BLOCK_INVALID_SEQ()
	assert(m_pAnimation);	
	string strArgPart;
	int		iIter;	

	iIter = strParam.find(STR_SEP);

	strArgPart = strParam.substr( iIter+1);	
	Trim(strArgPart);
	m_pAnimation->fAccumulateTime	= (float)atof(strArgPart.c_str());	
	return true;
}


WORDCASE_MAP( OnSeqPart	)
{
	CHECK_BLOCK_INVALID_SEQ_PART()

	std::string strCase = GetCase(strParam.c_str()); // str을 이용해 LayerID를 참조한다.

	m_AniSeqPart.PartLayer = m_mStrToLayer[ strCase.c_str() ]; 
	AssignPartLayerValue( strParam, m_AniSeqPart );
	m_AniSeq.vAniSeqPart.push_back( m_AniSeqPart );

	m_AniSeqPart.SetNull();

	return true;
}