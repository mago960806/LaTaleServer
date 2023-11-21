
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <string>

#ifdef _LATALE_CLIENT
#include "SPCommon.h"
#else 
#include "SPToolCommon.h"
#endif

#include "SPUtil.h"

#include "SPGameObjectDEF.h"
#include "SPMonsterModelUnitDef.h"

#ifdef _LATALE_CLIENT
	#include "SPManager.h"
	//#include "SPItem.h"
	#include "SPItemCluster.h"		//[2005/6/16]
	#include "SPGOBManager.h"

	#include "SPLDTFile.h"
	#include "SPLDTManager.h"
	#include "SPResourceManager.h"
	#include "SPDebug.h"
	#include "SPStream.h"
#endif

#include "SPResourceDef.h"
#include "SPGOBClusterDef.h"

#include "SPMonsterModelUnitDef.h"
#include "SPMOBAniTableLoader.h"
#include "SPMOBLayerTableLoader.h"
#include "SPMOBCluster.h"
#include "SPGOBCluster.h"

#include "SPBattle.h"
#include "SPBattleCluster.h"
#include "SPBattleLoader.h"

using namespace std;

struct _IsEqualMobAnimation : binary_function<SPMOBAnimation*, ANIMATION_INDEX, bool> {
	bool operator() (SPMOBAnimation* pMOBAnimation, ANIMATION_INDEX AniIndex) const
	{
		if( pMOBAnimation && pMOBAnimation->eAniIndex == AniIndex ) return true;
		return false;
	}
} IsEqualMobAnimation;

SPMOBCluster* g_pMOBCluster = NULL;
//------------------------------------------------------------------------------------
#ifdef _LATALE_CLIENT
SPMOBCluster::SPMOBCluster(SPGOBManager* pkParent)
: m_pkParent(pkParent)
, m_pkIconTexture(NULL)
{
	g_pMOBCluster = this;
	Init();
	m_eAniDefaultIndex = ANI_MONSTER_STAND;
}
#else
SPMOBCluster::SPMOBCluster()
: m_pkIconTexture(NULL)
{
	g_pMOBCluster = this;
	Init();
	m_eAniDefaultIndex = ANI_NULL;
}
#endif

//------------------------------------------------------------------------------------
SPMOBCluster::~SPMOBCluster()
{
	Clean();
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::Init(string strFilePath)
{
	bool bRet = true;

#ifdef _LATALE_CLIENT
	#ifndef _JUST_RUN
		#if defined(_DEBUG)
			DXUTOutputDebugString("SPMOBCluster::Init()\n");
		#endif

		string strLDTFile;
		strLDTFile = "DATA/LDT/GLOBAL_MOB.LDT";

		if( strFilePath.empty() )
		{
			//strFilePath = RES_WORK_RESOURCE_PATH;
			strFilePath = "DATA/ANITABLE/";
		}

		if( !LoadLDTFile(strLDTFile, strFilePath) )
			return false;

		#if defined(_USED_TEXTURE_TABLE)
			bRet = LoadTextureTableMetaFile( "Monster.mft" , RES_PATH_MONSTER );

			if( bRet == false )
			{
				strLDTFile = "DATA/CHAR/MONSTER/Monster.tbl";
				bRet = LoadTextureTable(strLDTFile, "");
			}
		#endif

		//bRet = Load(CLASS_ID_MOB_PRIRING, strFilePath);
		//bRet = Load(CLASS_ID_MOB_BETTLEG, strFilePath);
		//bRet = Load(CLASS_ID_MAPEVENT_NPC, strFilePath, false);
		
		
		//kBattleLoader.LoadBattleTableFromFolder(strFilePath);

		//m_pkParent->AddMonster(5600.0f, 500.0f, CLASS_ID_MOB_BETTLEG);
		//m_pkParent->AddMonster(32100.0f, 500.0f, CLASS_ID_MOB_BETTLEG);
		//m_pkParent->AddMonster(32200.0f, 500.0f, CLASS_ID_MOB_PRIRING);

		g_pVideo->GetTextureMgr()->LoadTexture("DATA/GLOBALRES/MOB_LEVEL_ICON01.PNG", &m_pkIconTexture);
		PreDefaultTextureLoading();

	#endif
#endif
	
	return bRet;
}

//------------------------------------------------------------------------------------
#if defined(_LATALE_CLIENT)

bool SPMOBCluster::LoadLDTFile(string strFileName, string strTablePath)
{
	bool bRet = true;

	SPBattleCluster::GetInstance();
	SPBattleLoader kBattleLoader;

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strFileName.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iIconIndex, i, j;
	unsigned int uiClassID;
	long lItemID;
	LDT_Field Field;
	string strTableFileName, strSeqName, strImageName, strStage, strMessage;
	MONSTER_TYPE eMOBType;
	
	TALK_MESSAGE TalkMessage;
	vector< TALK_MESSAGE > vTalkAniIndex;

	LDT_Field	LFD_Name, LFD_Icon, LFD_IconIndex, LFD_Tip , LFD_Desc;
	LDT_Field	LFD_ShowName, LFD_ShowHP, LFD_ShowShadow, LFD_RespawnFX;
	SPMOBUIInfo	stMOBUIInfo;
	stdext::hash_map< std::string , bool >	hmStringData;
	
	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);
		uiClassID = lItemID;

		iIconIndex = 0;
		
		pkLDTFile->GetField(lItemID, 0, Field);			//_Sequence
		strSeqName = Field.uData.pValue;

		pkLDTFile->GetField(lItemID, 1, Field);			//_ImageResource
		strImageName = Field.uData.pValue;

		pkLDTFile->GetField(lItemID, 2, Field);			//_Type
		eMOBType = (MONSTER_TYPE)Field.uData.lValue;

		pkLDTFile->GetField(lItemID, 18, Field);		//_Stage
		strStage = Field.uData.pValue;

		pkLDTFile->GetField(lItemID, 29, Field);		//_Mark_IconIndex
		iIconIndex = Field.uData.lValue;

		for( j = 0; j < 3; j++ )
		{
			TalkMessage.vMessage.clear();

			pkLDTFile->GetField(lItemID, 3 + j * 5, Field);		//_Message_Ani_Index
			TalkMessage.iAnimationIndex = Field.uData.lValue;

			if( eMOBType == MT_EVENT )
				TalkMessage.iAnimationIndex = ANI_NPC_STAND01 + j;
			else
				TalkMessage.iAnimationIndex--;

			pkLDTFile->GetField(lItemID, 4 + j * 5, Field);		//_Message_Probability
			TalkMessage.iProbability = Field.uData.lValue;

			pkLDTFile->GetField(lItemID, 5 + j * 5, Field);		//_Message1
			strMessage = Field.uData.pValue;
			if( !strMessage.empty() )
				TalkMessage.vMessage.push_back(strMessage);

			pkLDTFile->GetField(lItemID, 6 + j * 5, Field);		//_Message2
			strMessage = Field.uData.pValue;
			if( !strMessage.empty() )
				TalkMessage.vMessage.push_back(strMessage);

			pkLDTFile->GetField(lItemID, 7 + j * 5, Field);		//_Message3
			strMessage = Field.uData.pValue;
			if( !strMessage.empty() )
				TalkMessage.vMessage.push_back(strMessage);

			if( !TalkMessage.vMessage.empty() )
			{
				vTalkAniIndex.push_back(TalkMessage);
				TalkMessage.vMessage.clear();
			}
		}
		
		//	AJJIYA [10/4/2005]
		//	UIMOBInfo 데이터
		pkLDTFile->GetField(lItemID, 19, LFD_Name);			//_Name
		pkLDTFile->GetField(lItemID, 20, LFD_Icon);			//_Icon
		pkLDTFile->GetField(lItemID, 21, LFD_IconIndex);	//_IconIndex
		pkLDTFile->GetField(lItemID, 23, LFD_Tip);			//_Tip
		pkLDTFile->GetField(lItemID, 22, LFD_Desc);			//_Description

		pkLDTFile->GetField(lItemID, 24, LFD_ShowName );	//_Name_Visible
		pkLDTFile->GetField(lItemID, 25, LFD_ShowHP );		//_Hp_Visible
		pkLDTFile->GetField(lItemID, 27, LFD_ShowShadow );	//_Shadow_Visible
		pkLDTFile->GetField(lItemID, 26, LFD_RespawnFX );	//_Creation_FX
		
		stMOBUIInfo.m_strMOBName	=	LFD_Name.uData.pValue;
		stMOBUIInfo.m_iMOBIcon		=	LFD_Icon.uData.lValue;
		stMOBUIInfo.m_iMOBIconIndex	=	LFD_IconIndex.uData.lValue;
		stMOBUIInfo.m_strMOBTip		=	LFD_Tip.uData.pValue;
		stMOBUIInfo.m_strMOBDesc	=	LFD_Desc.uData.pValue;

		stMOBUIInfo.m_bShowName		=	(bool)LFD_ShowName.uData.lValue;
		stMOBUIInfo.m_bShowHP		=	(bool)LFD_ShowHP.uData.lValue;
		stMOBUIInfo.m_bShowShadow	=	(bool)LFD_ShowShadow.uData.lValue;
		stMOBUIInfo.m_iRespawnFX	=	LFD_RespawnFX.uData.lValue;

		m_mMOBUIInfo.insert( std::map< unsigned int , SPMOBUIInfo >::value_type( uiClassID , stMOBUIInfo ) );
		//	끝

		strTableFileName = strTablePath + strSeqName + "_r" + ".seq";

		if( eMOBType == MT_LAYER_PET || eMOBType == MT_LAYER_BOSS )
			bRet = LoadBinaryMOBLayer(uiClassID, strTableFileName);
		else
			bRet = LoadBinary(uiClassID, strTableFileName, strImageName);

		if( !bRet )
		{
			vTalkAniIndex.clear();
#ifdef _DEBUG
			continue;
#endif
			return false;
		}

		strTableFileName = strTablePath + strSeqName + "_r" + ".drg";
		kBattleLoader.LoadBinaryBattleFile(strTableFileName, false, false, 0, uiClassID);

		strTableFileName = strTablePath + strSeqName + "_r" + ".arg";
		kBattleLoader.LoadBinaryBattleFile(strTableFileName, false, true, 0, uiClassID);

		if( !vTalkAniIndex.empty() )
		{
			m_mTalkMessage.insert(make_pair(uiClassID, vTalkAniIndex));
			vTalkAniIndex.clear();
		}

		ConvertStringToStage( hmStringData , strStage.c_str() );
		m_mStagePalette.insert( std::map< unsigned int , stdext::hash_map< std::string , bool > >::value_type( uiClassID , hmStringData ) );

		m_mMarkIcon.insert(make_pair(uiClassID, iIconIndex));
		m_mMonsterType.insert(make_pair(uiClassID, eMOBType));
	}

	SAFE_RELEASE(pkLDTFile);
	vTalkAniIndex.clear();
	return true;
}
#endif

//------------------------------------------------------------------------------------
#if defined(_CHARTOOL)

bool SPMOBCluster::InsertAnimation(unsigned int uiClassID, SPMOBAnimation* pkAnimation)
{
	vector< SPMOBAnimation* >* pvAnimation;
	if( IsEqualClassID(uiClassID, &pvAnimation) )
		pvAnimation->push_back(pkAnimation);
	else
	{
		vector< SPMOBAnimation* > vInsertAnimation;
		vInsertAnimation.push_back(pkAnimation);
		m_mAnimation.insert(std::make_pair(uiClassID, vInsertAnimation));

		vInsertAnimation.clear();
	}

	return true;
}

//------------------------------------------------------------------------------------
void SPMOBCluster::DeleteAnimation(unsigned int uiClassID, ANIMATION_INDEX AniIndex)
{
	vector< SPMOBAnimation* >* pvAnimation;
	if( !IsEqualClassID(uiClassID, &pvAnimation) )
		return;

	vector< SPMOBAnimation* >::iterator IterAnimation;
	IterAnimation = std::find_if(pvAnimation->begin(), pvAnimation->end(),
		std::bind2nd(IsEqualMobAnimation, AniIndex));
	
	if( IterAnimation == pvAnimation->end() ) 
		return;

	delete (*IterAnimation);
	pvAnimation->erase(IterAnimation);
}

#endif

//------------------------------------------------------------------------------------
bool SPMOBCluster::InsertLayer(unsigned int uiClassID, SPMOBLayerInfo* pLayerInfo)
{
	map< unsigned int, vector< SPMOBLayerInfo* > >::iterator iterKey = m_mLayer.find(uiClassID);
	if( iterKey == m_mLayer.end() )
	{
		vector< SPMOBLayerInfo* > vLayerInfo;
		vLayerInfo.push_back(pLayerInfo);
		m_mLayer.insert(make_pair(uiClassID, vLayerInfo));
	}
	else
	{
		vector< SPMOBLayerInfo* >::iterator iterLayer = (*iterKey).second.begin();
		while( iterLayer != (*iterKey).second.end() )
		{
			if( (*iterLayer)->strName == pLayerInfo->strName )
				return false;

			++iterLayer;
		}
		
		(*iterKey).second.push_back(pLayerInfo);
	}
	
	return true;
}

//------------------------------------------------------------------------------------
void SPMOBCluster::DeleteLayer(unsigned int uiClassID, const char* pszLayerName)
{
	map< unsigned int, vector< SPMOBLayerInfo* > >::iterator iterKey = m_mLayer.find(uiClassID);
	if( iterKey == m_mLayer.end() )
		return;

	vector< SPMOBLayerInfo* >::iterator iterLayer = (*iterKey).second.begin();
	while( iterLayer != (*iterKey).second.end() )
	{
		if( (*iterLayer)->strName == pszLayerName )
		{
			delete (*iterLayer);
			(*iterKey).second.erase(iterLayer);
			break;
		}

		++iterLayer;
	}

	if( (*iterKey).second.empty() )
		m_mLayer.erase(iterKey);
}

//------------------------------------------------------------------------------------
SPMOBAnimation* SPMOBCluster::GetAnimation(unsigned int uiClassID, ANIMATION_INDEX AniIndex)
{
	vector< SPMOBAnimation* >* pvAnimation;
	if( !IsEqualClassID(uiClassID, &pvAnimation) )
		return NULL;

	vector< SPMOBAnimation* >::iterator IterAnimation;
	IterAnimation = std::find_if(pvAnimation->begin(), pvAnimation->end(),
		std::bind2nd(IsEqualMobAnimation, AniIndex));

	if( IterAnimation == pvAnimation->end() ) 
		return NULL;

	return (*IterAnimation);
}

//------------------------------------------------------------------------------------
const char* SPMOBCluster::GetTalkMessage(unsigned int uiClassID, ANIMATION_INDEX eAniIndex)
{
	map< unsigned int, vector< TALK_MESSAGE > >::iterator iterKey;
	iterKey = m_mTalkMessage.find(uiClassID);
	if( iterKey == m_mTalkMessage.end() )
		return NULL;

	vector< TALK_MESSAGE >::iterator iterTalkMessage;
	iterTalkMessage = iterKey->second.begin();
	while( iterTalkMessage != iterKey->second.end() )
	{
		if( (*iterTalkMessage).iAnimationIndex == eAniIndex )
		{
			if( (*iterTalkMessage).iProbability > (rand() % 100) )
			{
				int iSize = (int)(*iterTalkMessage).vMessage.size();
				if( iSize == 0 )
					return NULL;

				vector< std::string >::iterator iterString;
				iterString = (*iterTalkMessage).vMessage.begin() + (rand() % iSize);

				return (*iterString).c_str();
			}
			else
				return NULL;
		}

		++iterTalkMessage;
	}
	
	return NULL;
}

//------------------------------------------------------------------------------------
void SPMOBCluster::Clean()
{
	map< unsigned int, vector< SPMOBAnimation* > >::iterator iterKey = m_mAnimation.begin();

	while( iterKey != m_mAnimation.end() )
	{
		vector< SPMOBAnimation* >::iterator iterValue = iterKey->second.begin();
		while( iterValue != iterKey->second.end() )
		{
			delete *iterValue;
			iterValue++;
		}

		++iterKey;
	}
	m_mAnimation.clear();

	map< unsigned int, vector< SPMOBLayerInfo* > >::iterator iterKey2 = m_mLayer.begin();
	while( iterKey2 != m_mLayer.end() )
	{
		vector< SPMOBLayerInfo* >::iterator iterValue2 = iterKey2->second.begin();
		while( iterValue2 != iterKey2->second.end() )
		{
			delete *iterValue2;
			iterValue2++;
		}

		++iterKey2;
	}
	m_mLayer.clear();
	
	m_mStagePalette.clear();
	m_mTalkMessage.clear();
	m_mMOBUIInfo.clear();
	m_mMarkIcon.clear();
	m_mMonsterType.clear();

	CleanStageTexture();
	CleanDefaultTexture();
	SAFE_RELEASE(m_pkIconTexture);

#if defined(_USED_TEXTURE_TABLE)
	m_hmTextureTableList.clear();
#endif
}

//------------------------------------------------------------------------------------
void SPMOBCluster::CleanStageTexture()
{
	vector< SPTexture* >::iterator iter = m_vStageTexture.begin();
	while( iter != m_vStageTexture.end() )
	{
		(*iter)->Release();
		++iter;
	}
	m_vStageTexture.clear();
}

//------------------------------------------------------------------------------------
void SPMOBCluster::CleanDefaultTexture()
{
	vector< SPTexture* >::iterator iter = m_vDefaultTexture.begin();
	while( iter != m_vDefaultTexture.end() )
	{
		(*iter)->Release();
		++iter;
	}
	m_vDefaultTexture.clear();
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::IsEqualStage( unsigned int uiClassID, int iStage )
{
	std::map< unsigned int , stdext::hash_map< std::string , bool > >::iterator mIter = m_mStagePalette.find( uiClassID );
	if( mIter == m_mStagePalette.end() )
		return false;

	stdext::hash_map< std::string , bool >*	phmStringData	=	&((*mIter).second);

	if( phmStringData == NULL )
		return false;

	char	szBuf[_MAX_PATH]	=	{};
	itoa( iStage , szBuf , 10 );

	std::string	strStage	=	szBuf;

	if( strStage.empty() == true )
		return false;

	//////////////////////////////////////////////////////////////////////////

	//std::string		strStageTest;
	//stdext::hash_map< std::string , bool >::iterator	hmIterTest	=	phmStringData->begin();

	//while ( hmIterTest != phmStringData->end() )
	//{
	//	strStageTest	=	((*hmIterTest).first);
	//	++hmIterTest;
	//}

	//////////////////////////////////////////////////////////////////////////

	int istrLength	=	(int)strlen(szBuf);

	std::string											strAsterisk;
	stdext::hash_map< std::string , bool >::iterator	hmIter;

	for( int j = 3 - istrLength ; j >= 0 ; --j )
	{
		for( int i = 0 ; i < istrLength ; ++i )
		{
			strStage	=	strAsterisk;
			strStage	+=	szBuf;

			hmIter	=	phmStringData->find( strStage );

			if( hmIter != phmStringData->end() )
				return true;

			szBuf[ i ]	=	'*';

			strStage	=	strAsterisk;
			strStage	+=	szBuf;

			if( strStage.empty() == true )
				return false;

			hmIter	=	phmStringData->find( strStage );

			if( hmIter != phmStringData->end() )
				return true;
		}

		itoa( iStage , szBuf , 10 );
		strAsterisk	+=	"*";
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::ConvertStringToStage( stdext::hash_map< std::string , bool >& hmOutData , const char* pszString )
{
	hmOutData.clear();

	if( pszString == NULL )
		return false;

	std::string	strString	=	pszString;

	if( strString.empty() == true )
		return false;

	std::string	strWord;
	std::string	strSeparate	=	",";
	size_t		iFindPos	=	0;
	size_t		iCurPos		=	0;

	while( iCurPos != std::string::npos )
	{
		iCurPos	=	strString.find( strSeparate , iFindPos );

		if( iFindPos != std::string::npos )
		{
			strWord		=	strString.substr( iFindPos , iCurPos - iFindPos );
            iFindPos	=	iCurPos	+	1;

			hmOutData.insert( stdext::hash_map< std::string , bool >::value_type( strWord , true ) );
		}
	}

	return true;
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::Load(unsigned int uiClassID, string strFilePath)
{
	std::vector< SPMOBAnimation* >* pvAnimation;
	if( IsEqualClassID(uiClassID, &pvAnimation) )
		return true;

	bool bRet;
	SPMOBAniTableLoader* pkAniLoader;
	pkAniLoader = new SPMOBAniTableLoader;

	std::vector< SPMOBAnimation* > vRefAnimation;
	std::string szFileName;
	
#if defined(_CHARTOOL)
	szFileName = strFilePath;
#else
	szFileName = strFilePath + GetMOBFileName(uiClassID) + ".seq";
#endif
	
	bRet = pkAniLoader->LoadAniPallet(vRefAnimation, szFileName);
	if( bRet )
		m_mAnimation.insert(std::make_pair(uiClassID, vRefAnimation));
	
	vRefAnimation.clear();
	delete pkAniLoader;

#if	!defined(_CHARTOOL)
	SPBattleLoader kBattleLoader;
	
	szFileName = strFilePath + GetMOBFileName(uiClassID) + ".drg";
	kBattleLoader.LoadBattleFile(szFileName, false, false, 0, uiClassID);

	szFileName = strFilePath + GetMOBFileName(uiClassID) + ".arg";
	kBattleLoader.LoadBattleFile(szFileName, false, true, 0, uiClassID);
#endif
	
	return bRet;
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::LoadBinary(unsigned int uiClassID, string strFileName, string strImageName)
{
	std::vector< SPMOBAnimation* >* pvAnimation;
	if( IsEqualClassID(uiClassID, &pvAnimation) )
		return true;

	bool bRet;
	SPMOBAniTableLoader* pkAniLoader;
	pkAniLoader = new SPMOBAniTableLoader;

	std::vector< SPMOBAnimation* > vRefAnimation;
	
	bRet = pkAniLoader->LoadBinaryAni(vRefAnimation, strFileName, strImageName);
	if( bRet )
		m_mAnimation.insert(std::make_pair(uiClassID, vRefAnimation));

	vRefAnimation.clear();
	delete pkAniLoader;

	return true;
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::LoadMOBLayer(unsigned int uiClassID, string strFilePath)
{
	if( GetMOBLayerSize(uiClassID) )
		return true;

	SPMOBLayerTableLoader* pkAniLoader;
	pkAniLoader = new SPMOBLayerTableLoader;
	std::string szFileName;

#if defined(_CHARTOOL)
	szFileName = strFilePath;
#else
	szFileName = strFilePath + GetMOBFileName(uiClassID) + ".seq";
#endif

	std::vector< SPMOBLayerInfo* > vMOBLayerInfo;

	bool bRet = pkAniLoader->LoadAniPallet(vMOBLayerInfo, szFileName);
	if( bRet )
		m_mLayer.insert(make_pair(uiClassID, vMOBLayerInfo));

	vMOBLayerInfo.clear();
	delete pkAniLoader;

	return bRet;
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::LoadBinaryMOBLayer(unsigned int uiClassID, string strFilePath)
{
	if( GetMOBLayerSize(uiClassID) )
		return true;

	SPMOBLayerTableLoader* pkAniLoader;
	pkAniLoader = new SPMOBLayerTableLoader;

	std::vector< SPMOBLayerInfo* > vMOBLayerInfo;

	bool bRet = pkAniLoader->LoadBinaryAni(vMOBLayerInfo, strFilePath);
	if( bRet )
		m_mLayer.insert(make_pair(uiClassID, vMOBLayerInfo));

	vMOBLayerInfo.clear();
	delete pkAniLoader;

	return bRet;
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::QueryAnimationSequence(SPMOBModel** ppMOBModel,
	unsigned int uiClassID, ANIMATION_INDEX AniInx, int& iAniFrame, bool bLoop)
{
	assert(ppMOBModel);
	assert(*ppMOBModel);

	if( ppMOBModel == NULL || *ppMOBModel == NULL ) 
		return false;
	(*ppMOBModel)->SetNull();

	vector< SPMOBAnimation* >* pvAnimation;
	if( !IsEqualClassID(uiClassID, &pvAnimation) )
	{
		unsigned int uiDefaultClassID = (uiClassID & 0xFFFFFF00) + 1;
		if( !IsEqualClassID(uiDefaultClassID, &pvAnimation) )
			return false;
	}

	vector< SPMOBAnimation* >::iterator IterAnimation;
	IterAnimation = std::find_if(pvAnimation->begin(), pvAnimation->end(),
		std::bind2nd(IsEqualMobAnimation, AniInx));
	
	if( IterAnimation == pvAnimation->end() ) 
	{
		if( m_eAniDefaultIndex != ANI_NULL )
		{
			AniInx = m_eAniDefaultIndex;
			IterAnimation = std::find_if(pvAnimation->begin(), pvAnimation->end(),
				std::bind2nd(IsEqualMobAnimation, AniInx));
		}

		if( IterAnimation == pvAnimation->end() ) 
		{
			OutputDebugString("Default animation sequence not found.\n");
			return false;
		}
	}

	int iTotalFrame = (int)(*IterAnimation)->vAniFrameInfo.size();
	if( iAniFrame >= iTotalFrame )
		return false;

	std::vector< SPMOBAniFrameInfo >::iterator iterFrameInfo;
	iterFrameInfo = (*IterAnimation)->vAniFrameInfo.begin() + iAniFrame;

	(*ppMOBModel)->eAniIndex = (*IterAnimation)->eAniIndex;
	(*ppMOBModel)->eMobSize = (*IterAnimation)->eMobSize;

	(*ppMOBModel)->iFrameCountX = (*IterAnimation)->iFrameCountX;
	(*ppMOBModel)->iFrameCountY = (*IterAnimation)->iFrameCountY;
	(*ppMOBModel)->iFrameWidth = (*IterAnimation)->iFrameWidth;
	(*ppMOBModel)->iFrameHeight = (*IterAnimation)->iFrameHeight;
	(*ppMOBModel)->fAccumulateTime = (*IterAnimation)->fAccumulateTime;
	(*ppMOBModel)->iCurAniFrame = iAniFrame;
	(*ppMOBModel)->iMaxAniFrame = iTotalFrame;
	(*ppMOBModel)->FrameInfo = *iterFrameInfo;
	(*ppMOBModel)->iDefaultRotation = (*iterFrameInfo).iRotation;

	iAniFrame++;
	if( iAniFrame >= iTotalFrame )
	{
		if( bLoop )
			iAniFrame = 0;
		else
			iAniFrame = iTotalFrame - 1;
	}

	return true;
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::QueryAnimationSequence(SPMOBLayerModel* pMOBLayerModel, int iLayerNum,
	unsigned int uiClassID, ANIMATION_INDEX AniInx, int& iAniFrame, bool bLoop)
{
	int iSize = GetMOBLayerSize(uiClassID);
	if( iSize == 0 || iLayerNum == 0 || iSize != iLayerNum )
		return false;

	SPMOBLayerInfo* pLayerInfo;
	SPMOBLayerAnimation* pLayerAnimation;
	vector< SPMOBLayerAniFrameInfo >::iterator iterFrame;
	int i, iZOrder, iTotalFrame = 0;

	for( i = 0; i < iSize; i++ )
		pMOBLayerModel[i].SetNull();

	for( i = 0; i < iSize; i++ )
	{
		pLayerInfo = GetMOBLayerInfo(uiClassID, i);
		if( pLayerInfo == NULL )
			continue;

		pLayerAnimation = GetMOBLayerAnimation(uiClassID, i, AniInx);
		if( pLayerAnimation && pLayerAnimation->iTotalFrame )
		{
			iterFrame = pLayerAnimation->vFrameInfo.begin() + iAniFrame;
			iZOrder = (*iterFrame).iZOrder;
			if( iZOrder >= 0 || iZOrder < iSize )
			{
				pMOBLayerModel[iZOrder].iLayerNo = i;
				pMOBLayerModel[iZOrder].iFrameWidth = pLayerInfo->iFrameWidth;
				pMOBLayerModel[iZOrder].iFrameHeight = pLayerInfo->iFrameHeight;
				pMOBLayerModel[iZOrder].iFrameCountX = pLayerInfo->iFrameCountX;
				pMOBLayerModel[iZOrder].iFrameCountY = pLayerInfo->iFrameCountY;
				pMOBLayerModel[iZOrder].bLightMap = pLayerInfo->bLightMap;

				pMOBLayerModel[iZOrder].eAniIndex = AniInx;
				pMOBLayerModel[iZOrder].iCurAniFrame = iAniFrame;
				pMOBLayerModel[iZOrder].iMaxAniFrame = pLayerAnimation->iTotalFrame;
				pMOBLayerModel[iZOrder].fAccumulateTime = pLayerAnimation->fAccumulateTime;
				pMOBLayerModel[iZOrder].iDefaultRotation = (*iterFrame).iRotation;
				pMOBLayerModel[iZOrder].FrameInfo = (*iterFrame);

				iTotalFrame = pLayerAnimation->iTotalFrame;
			}
		}
	}

	iAniFrame++;
	if( iAniFrame >= iTotalFrame )
	{
		if( bLoop )
			iAniFrame = 0;
		else
			iAniFrame = iTotalFrame - 1;
	}

	return true;
}

//------------------------------------------------------------------------------------
float SPMOBCluster::GetAnimationTime(unsigned int uiClassID,
	const ANIMATION_INDEX AniInx)
{
	vector< SPMOBAnimation* >* pvAnimation;
	if( !IsEqualClassID(uiClassID, &pvAnimation) )
	{
		unsigned int uiDefaultClassID = (uiClassID & 0xFFFFFF00) + 1;
		if( !IsEqualClassID(uiDefaultClassID, &pvAnimation) )
			return 0.0f;
	}

	vector< SPMOBAnimation* >::iterator IterAnimation;
	IterAnimation = std::find_if(pvAnimation->begin(), pvAnimation->end(),
		std::bind2nd(IsEqualMobAnimation, AniInx));

	if( IterAnimation == pvAnimation->end() )
		return 0.0f;

	return (*IterAnimation)->fAccumulateTime;
}

//------------------------------------------------------------------------------------
float SPMOBCluster::GetAnimationTimeLayer(unsigned int uiClassID,
	const ANIMATION_INDEX AniInx)
{
	int iSize = GetMOBLayerSize(uiClassID);
	if( iSize == 0 )
		return 0.0f;

	SPMOBLayerInfo* pLayerInfo;
	SPMOBLayerAnimation* pLayerAnimation;
	
	pLayerInfo = GetMOBLayerInfo(uiClassID, 0);
	if( pLayerInfo == NULL )
		return 0.0f;

	pLayerAnimation = GetMOBLayerAnimation(uiClassID, 0, AniInx);
	if( pLayerAnimation == NULL )
		return 0.0f;

	return pLayerAnimation->fAccumulateTime;
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::IsEqualClassID(unsigned int uiClassID,
	vector< SPMOBAnimation* >** ppvAnimation)
{
	map< unsigned int, std::vector< SPMOBAnimation* > >::iterator iterClassID = m_mAnimation.find(uiClassID);
	if( iterClassID == m_mAnimation.end() )
		return false;
	
	*ppvAnimation = &iterClassID->second;
	return true;

	//map< unsigned int, std::vector< SPMOBAnimation* > >::iterator iterClassID =
	//	m_mAnimation.begin();

	//while( iterClassID != m_mAnimation.end() )
	//{
	//	if( iterClassID->first == uiClassID )
	//	{
	//		*ppvAnimation = &iterClassID->second;
	//		return true;
	//	}
	//	++iterClassID;
	//}

	//return false;
}

//------------------------------------------------------------------------------------
SPMOBUIInfo* SPMOBCluster::GetMOBUIInfo( unsigned int uiClassID )
{
	std::map< unsigned int , SPMOBUIInfo >::iterator	Iter	=	m_mMOBUIInfo.find( uiClassID );

	if( Iter != m_mMOBUIInfo.end() )
	{
		return &( (*Iter).second );
	}

	return NULL;
}

//------------------------------------------------------------------------------------
SPTexture* SPMOBCluster::GetIconTexture(int iIndex, RECT& rcSrc)
{
	if( iIndex < 0 )
		return NULL;

	rcSrc.left = iIndex * 16;
	rcSrc.top = 0;
	rcSrc.right = (iIndex + 1) * 16;
	rcSrc.bottom = 16;

	return m_pkIconTexture;
}

//------------------------------------------------------------------------------------
int SPMOBCluster::GetIconIndex(unsigned int uiClassID)
{
	map< unsigned int, int >::iterator iter = m_mMarkIcon.find(uiClassID);
	if( iter == m_mMarkIcon.end() )
		return 0;

	return (*iter).second;
}

//------------------------------------------------------------------------------------
#if defined(_USED_TEXTURE_TABLE)
bool SPMOBCluster::LoadTextureTableMetaFile( string strFileName , string strTablePath )
{
	SPStream* pStream;
	string strFullName = strTablePath + strFileName;
	SPResourceBase::GetInstance()->GetStreamData( strFullName.c_str(), &pStream );

	if( pStream == NULL || !pStream->Valid())
	{
		SAFE_RELEASE( pStream );
		return false;
	}

	MFTStreamHeader	stHeader;

	pStream->Read( &stHeader , sizeof( stHeader ) );

	if( stHeader.iBinary != MFTStreamHeader::BINARY_CHECK )
	{
		SAFE_RELEASE( pStream );
		return false;
	}

	int		iFileCount = 0;

	pStream->Read( &iFileCount , sizeof( iFileCount ) );

	if( iFileCount <= 0 )
	{
		SAFE_RELEASE( pStream );
		return false;
	}

	int			iSize;
	char		szBuf[ _MAX_PATH * 10 ];

	for( int i = 0 ; i < iFileCount ; ++i )
	{
		pStream->Read( &iSize , sizeof( iSize ) );

		if( iSize > 0 )
		{
			pStream->Read( szBuf , sizeof( char) * iSize );

			LoadTextureTable( szBuf , strTablePath );

			memset( szBuf , 0 , sizeof( szBuf ) );
		}
	}

	SAFE_RELEASE( pStream );

	return true;

}

//------------------------------------------------------------------------------------
bool SPMOBCluster::LoadTextureTable(string strFileName, string strTablePath)
{
	SPStream* pStream;
	string strFullName = strTablePath + strFileName;
	SPResourceBase::GetInstance()->GetStreamData( strFullName.c_str(), &pStream);

	if( pStream == NULL || !pStream->Valid())
	{
		SAFE_RELEASE(pStream);
		return false;
	}

	string	strTempPath;
	string	strTempName;

	GetSeperateStringDir( strFileName , strTempPath , strTempName );

	if( strTempPath.find( RES_PATH_MONSTER ) == 0 )
	{
		strTempPath.clear();
	}

	int iVersion, iSize, iFrameSize, i, j;
	char szBuf[128];
	TEXTURE_TABLE_INFO TextureTableInfo;
	FRAME_INFO FrameInfo;

	pStream->Read(&iVersion, sizeof(int));
	if( iVersion != 100 )
		return false;

	pStream->Read(&iSize, sizeof(int));
	for( i = 0; i < iSize; i++ )
	{
		pStream->Read(&TextureTableInfo.iTotalFrame, sizeof(int));
		pStream->Read(szBuf, 128 * sizeof(char));
		TextureTableInfo.strID = szBuf;

		TextureTableInfo.strTextureName.clear();
		TextureTableInfo.hmFrameList.clear();
		pStream->Read(&iFrameSize, sizeof(int));
		for( j = 0; j < iFrameSize; j++ )
		{
			pStream->Read(&FrameInfo.iFrameNo, sizeof(int));
			pStream->Read(&FrameInfo.iOffsetX, sizeof(int));
			pStream->Read(&FrameInfo.iOffsetY, sizeof(int));
			pStream->Read(&FrameInfo.fCenterOffsetX, sizeof(float));
			pStream->Read(&FrameInfo.fCenterOffsetY, sizeof(float));
			pStream->Read(&FrameInfo.rcResourceRgn.left, sizeof(int));
			pStream->Read(&FrameInfo.rcResourceRgn.top, sizeof(int));
			pStream->Read(&FrameInfo.rcResourceRgn.right, sizeof(int));
			pStream->Read(&FrameInfo.rcResourceRgn.bottom, sizeof(int));

			pStream->Read(szBuf, 128 * sizeof(char));

			if( strTempPath.empty() == true )
				FrameInfo.strTextureName = szBuf;
			else
				FrameInfo.strTextureName = strTempPath + szBuf;

			if( TextureTableInfo.strTextureName.empty() )
			{
				if( strTempPath.empty() == true )
					TextureTableInfo.strTextureName = szBuf;
				else
					TextureTableInfo.strTextureName = strTempPath + szBuf;
			}

			TextureTableInfo.hmFrameList.insert(
				hash_map< int, FRAME_INFO >::value_type(FrameInfo.iFrameNo, FrameInfo));
		}

		m_hmTextureTableList.insert(
			hash_map< string, TEXTURE_TABLE_INFO >::value_type(TextureTableInfo.strID, TextureTableInfo));
	}

	SAFE_RELEASE(pStream);

	return true;
}

//------------------------------------------------------------------------------------
TEXTURE_TABLE_INFO* SPMOBCluster::GetTextureTable(string strID)
{
	hash_map< std::string, TEXTURE_TABLE_INFO >::iterator iterTable;
	iterTable = m_hmTextureTableList.find(strID);
	if( iterTable == m_hmTextureTableList.end() )
		return NULL;

	return &(iterTable->second);
}

//------------------------------------------------------------------------------------
FRAME_INFO* SPMOBCluster::GetFrame(TEXTURE_TABLE_INFO* pTableInfo, int iFrame)
{
	if( pTableInfo == NULL )
		return NULL;

	hash_map< int, FRAME_INFO >::iterator iterFrame;
	iterFrame = pTableInfo->hmFrameList.find(iFrame);

	if( iterFrame == pTableInfo->hmFrameList.end() )
		return NULL;

	return &(iterFrame->second);
}

//------------------------------------------------------------------------------------
FRAME_INFO* SPMOBCluster::GetFrame(string strID, int iFrame)
{
	TEXTURE_TABLE_INFO* pTableInfo = GetTextureTable(strID);
	if( pTableInfo == NULL )
		return NULL;

	hash_map< int, FRAME_INFO >::iterator iterFrame;
	iterFrame = pTableInfo->hmFrameList.find(iFrame);

	if( iterFrame == pTableInfo->hmFrameList.end() )
		return NULL;

	return &(iterFrame->second);
}

//------------------------------------------------------------------------------------
void SPMOBCluster::PreDefaultTextureLoading()
{
	map< unsigned int, MONSTER_TYPE >::iterator iter = m_mMonsterType.begin();
	while( iter != m_mMonsterType.end() )
	{
		if( (*iter).second == MT_PET || (*iter).second == MT_LAYER_PET )
			PreTextureLoadingbyID((*iter).first, true, ((*iter).second == MT_LAYER_PET) ? true : false);

		++iter;
	}
}

//------------------------------------------------------------------------------------
void SPMOBCluster::PreTextureLoading(int iStage)
{
	CleanStageTexture();

	MONSTER_TYPE eType;

	map< unsigned int, std::vector< SPMOBAnimation* > >::iterator iter = m_mAnimation.begin();
	while( iter != m_mAnimation.end() )
	{
		if( IsEqualStage(iter->first, iStage) )
			PreTextureLoadingbyID(iter->first);

		++iter;
	}

	std::map< unsigned int, std::vector< SPMOBLayerInfo* > >::iterator iterLayer = m_mLayer.begin();
	while( iterLayer != m_mLayer.end() )
	{
		if( IsEqualStage(iterLayer->first, iStage) )
		{
			eType = GetMonsterType(iterLayer->first);
			if( eType == MT_LAYER_BOSS )
				PreTextureLoadingbyID(iterLayer->first, false, true);
		}

		++iterLayer;
	}
}

//------------------------------------------------------------------------------------
void SPMOBCluster::PreTextureLoadingbyID(unsigned int uiClassID, bool bDefault, bool bLayer)
{
	char szName[MAX_PATH], szFullName[MAX_PATH];
	char* pszLayerPos;
	int iLength, iLayerNum, i;
	SPMOBLayerInfo* pLayerInfo;
	TEXTURE_TABLE_INFO* pTableInfo;
	SPTexture* pkImage;
	string strPath = RES_PATH_MONSTER;

	if( bLayer )
	{
		iLayerNum = GetMOBLayerSize(uiClassID);
		for( i = 0; i < iLayerNum; i++ )
		{
			pLayerInfo = GetMOBLayerInfo(uiClassID, i);
			if( pLayerInfo == NULL )
				continue;

			if( pLayerInfo->strImageName.empty() )
				continue;
			
			pTableInfo = g_pMOBCluster->GetTextureTable(pLayerInfo->strImageName);
			if( pTableInfo == NULL )
				continue;

			sprintf(szFullName, "%s%s", strPath.c_str(), pTableInfo->strTextureName.c_str());
			//if( !g_pVideo->GetTextureMgr()->LoadTexture(szFullName, &pkImage) )
			if( !g_pVideo->GetTextureMgr()->GetTexture(szFullName, &pkImage,TEX_MON,LEVEL_LOW) )
				continue;

			if( bDefault )
				m_vDefaultTexture.push_back(pkImage);
			else
				m_vStageTexture.push_back(pkImage);
		}

		return;
	}
	
	map< unsigned int, std::vector< SPMOBAnimation* > >::iterator iter = m_mAnimation.find(uiClassID);
	if( iter == m_mAnimation.end() )
		return;

	SPMOBAnimation* pkAnimation = NULL;
	for( i = ANI_MONSTER_BEGIN; i <= ANI_NPC_END; i++ )
	{
		if( pkAnimation = GetAnimation(iter->first, (ANIMATION_INDEX)i) )
			break;
	}

	if( pkAnimation == NULL )
		return;

	strcpy(szName, pkAnimation->strImageName.c_str());
	iLength = (int)strlen(szName);
	if( iLength <= 0 )
		return;

	pszLayerPos = &szName[iLength - 1];
	for( i = 0; i < 4; i++ )
	{
		// '0' : 48
		*pszLayerPos = i + 48;

		pTableInfo = g_pMOBCluster->GetTextureTable(szName);
		if( pTableInfo == NULL )
			continue;

		if( pTableInfo->strTextureName.empty() )
			continue;

		sprintf(szFullName, "%s%s", strPath.c_str(), pTableInfo->strTextureName.c_str());
		//if( !g_pVideo->GetTextureMgr()->LoadTexture(szFullName, &pkImage) )
		if( !g_pVideo->GetTextureMgr()->GetTexture(szFullName, &pkImage,TEX_MON,LEVEL_LOW) )
			continue;

		if( bDefault )
			m_vDefaultTexture.push_back(pkImage);
		else
			m_vStageTexture.push_back(pkImage);
	}
}

#endif

//------------------------------------------------------------------------------------
SPMOBLayerInfo* SPMOBCluster::GetMOBLayerInfo(unsigned int uiClassID,
	const char* pszLayer)
{
	map< unsigned int, vector< SPMOBLayerInfo* > >::iterator iterKey = m_mLayer.find(uiClassID);
	if( iterKey == m_mLayer.end() )
		return NULL;

	vector< SPMOBLayerInfo* >::iterator iterLayer = (*iterKey).second.begin();
	while( iterLayer != (*iterKey).second.end() )
	{
		if( (*iterLayer)->strName == pszLayer )
			return (*iterLayer);

		++iterLayer;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
SPMOBLayerInfo* SPMOBCluster::GetMOBLayerInfo(unsigned int uiClassID, int iNo)
{
	map< unsigned int, vector< SPMOBLayerInfo* > >::iterator iterKey = m_mLayer.find(uiClassID);
	if( iterKey == m_mLayer.end() )
		return NULL;

	int iCount = 0;
	vector< SPMOBLayerInfo* >::iterator iterLayer = (*iterKey).second.begin();
	while( iterLayer != (*iterKey).second.end() )
	{
		if( iCount == iNo )
			return (*iterLayer);

		++iterLayer;
		++iCount;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
int SPMOBCluster::GetMOBLayerSize(unsigned int uiClassID)
{
	int iSize = 0;
	map< unsigned int, vector< SPMOBLayerInfo* > >::iterator iterKey = m_mLayer.find(uiClassID);
	if( iterKey != m_mLayer.end() )
		iSize = (*iterKey).second.size();

	return iSize;
}

//------------------------------------------------------------------------------------
int SPMOBCluster::GetMOBLayerNo(unsigned int uiClassID, const char* pszLayer)
{
	map< unsigned int, vector< SPMOBLayerInfo* > >::iterator iterKey = m_mLayer.find(uiClassID);
	if( iterKey == m_mLayer.end() )
		return -1;

	int iCount = 0;
	vector< SPMOBLayerInfo* >::iterator iterLayer = (*iterKey).second.begin();
	while( iterLayer != (*iterKey).second.end() )
	{
		if( (*iterLayer)->strName == pszLayer )
			return iCount;

		++iterLayer;
		++iCount;
	}

	return -1;
}

//------------------------------------------------------------------------------------
SPMOBLayerAnimation* SPMOBCluster::GetMOBLayerAnimation(unsigned int uiClassID,
	int iNo, ANIMATION_INDEX eIndex)
{
	SPMOBLayerInfo* pLayerInfo = GetMOBLayerInfo(uiClassID, iNo);
	if( pLayerInfo == NULL )
		return NULL;

	vector< SPMOBLayerAnimation >::iterator iterAni;
	iterAni = pLayerInfo->vAnimation.begin();
	while( iterAni != pLayerInfo->vAnimation.end() )
	{
		if( (*iterAni).eAniIndex == eIndex )
			return &(*iterAni);

		++iterAni;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
void SPMOBCluster::InsertMOBLayerAnimation(unsigned int uiClassID, SPMOBLayerAnimation ani)
{
	int iSize = GetMOBLayerSize(uiClassID);
	int i;

	SPMOBLayerInfo* pLayerInfo;
	vector< SPMOBLayerAnimation >::iterator iterAni;
	for( i = 0; i < iSize; i++ )
	{
		pLayerInfo = GetMOBLayerInfo(uiClassID, i);
		if( pLayerInfo == NULL )
			continue;

		pLayerInfo->vAnimation.push_back(ani);
	}
}

//------------------------------------------------------------------------------------
bool SPMOBCluster::DeleteMOBLayerAnimation(unsigned int uiClassID, ANIMATION_INDEX eIndex)
{
	int iSize = GetMOBLayerSize(uiClassID);
	int i;

	SPMOBLayerInfo* pLayerInfo;
	vector< SPMOBLayerAnimation >::iterator iterAni;
	for( i = 0; i < iSize; i++ )
	{
		pLayerInfo = GetMOBLayerInfo(uiClassID, i);
		if( pLayerInfo == NULL )
			continue;

		iterAni = pLayerInfo->vAnimation.begin();
		while( iterAni != pLayerInfo->vAnimation.end() )
		{
			if( (*iterAni).eAniIndex == eIndex )
			{
				pLayerInfo->vAnimation.erase(iterAni);
				break;
			}
			++iterAni;
		}
	}

	return true;
}

//------------------------------------------------------------------------------------
SPMOBLayerAniFrameInfo* SPMOBCluster::GetMOBLayerAniFrame(unsigned int uiClassID,
	int iNo, ANIMATION_INDEX eIndex, int iFrameNo)
{
	if( iFrameNo < 0 )
		return NULL;

	SPMOBLayerAnimation* pLayerAnimation;
	pLayerAnimation = GetMOBLayerAnimation(uiClassID, iNo, eIndex);
	if( pLayerAnimation == NULL )
		return NULL;

	int iSize = pLayerAnimation->vFrameInfo.size();
	if( iFrameNo >= iSize )
		return NULL;

	vector< SPMOBLayerAniFrameInfo >::iterator iterFrame
		= pLayerAnimation->vFrameInfo.begin() + iFrameNo;
	
	return &(*iterFrame);
}

//------------------------------------------------------------------------------------
SPMOBLayerAniFrameInfo* SPMOBCluster::FindMOBLayerZOrder(unsigned int uiClassID,
	ANIMATION_INDEX eIndex, int iFrameNo, int iZOrder)
{
	int iSize = GetMOBLayerSize(uiClassID);
	int i;

	SPMOBLayerAniFrameInfo* pAniFrameInfo;
	for( i = 0; i < iSize; i++ )
	{
		pAniFrameInfo = GetMOBLayerAniFrame(uiClassID, i, eIndex, iFrameNo);
		if( pAniFrameInfo == NULL )
			continue;

		if( pAniFrameInfo->iZOrder == iZOrder )
			return pAniFrameInfo;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
MONSTER_TYPE SPMOBCluster::GetMonsterType(unsigned int uiClassID)
{
	map< unsigned int, MONSTER_TYPE >::iterator iter = m_mMonsterType.find(uiClassID);

	MONSTER_TYPE eMonsterType = MT_NULL;
	if( iter != m_mMonsterType.end() )
		eMonsterType = (*iter).second;

	return eMonsterType;
}