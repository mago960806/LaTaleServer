
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
#include "SPGOBClusterDEF.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBAnimation.h"
#include "SPGOBModel.h"

#include "SPMonsterModelUnitDef.h"

#ifdef _LATALE_CLIENT
#include "SPManager.h"
//#include "SPItem.h"
//#include "SPItemCluster.h"		//[2005/6/16] - jinhee

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPDebug.h"
#include "SPStream.h"
#endif

#include "SPResourceDef.h"
#include "SPGOBClusterDef.h"
#include "SPMonsterModelUnitDef.h"
#include "SPAniTableLoader.h"
#include "SPGOBCluster.h"

#include "SPBattle.h"
#include "SPBattleCluster.h"
#include "SPBattleLoader.h"

using namespace std;
using namespace stdext;

SPGOBCluster* g_pGOBCluster = NULL;

struct _IsEqualAnimation : binary_function<SPGOBAnimation*, unsigned int, bool> {
	bool operator() (SPGOBAnimation* pGOBAnimation, unsigned int uiAniIndex) const
	{
		if( pGOBAnimation && pGOBAnimation->uiAniInx == uiAniIndex ) return true;
		return false;
	}
} IsEqualAnimation;

struct _IsEqualPartFromPart : binary_function<SPGOBInfoSequencePart, SPGOBInfoSequencePart, bool> {
	bool operator() (SPGOBInfoSequencePart GOBPart, SPGOBInfoSequencePart CompareGOBPart) const
	{
		return (GOBPart == CompareGOBPart); // 연산자오버로딩 되있음
	}
} IsEqualPartFromPart;


#ifdef _LATALE_CLIENT

SPGOBCluster::SPGOBCluster(SPGOBManager* pParent)
: m_pParent(pParent)
{
	g_pGOBCluster = this;
	Init();
}

#else 

SPGOBCluster::SPGOBCluster()
{
	g_pGOBCluster = this;
	Init();
}

#endif


SPGOBCluster::~SPGOBCluster()
{
	Clean();
}

void SPGOBCluster::Clean()
{
	hash_map< string, SPGOBAnimation* >::iterator iter;
	iter = m_hmAnimationPool.begin();
	while( iter != m_hmAnimationPool.end() )
	{
		delete iter->second;
		iter++;
	}
	m_hmAnimationPool.clear();

	m_hmAnimation.clear();

	vector< SPTexture* >::iterator iterImage = m_vDefaultImageList.begin();
	while( iterImage != m_vDefaultImageList.end() )
	{
		SAFE_RELEASE(*iterImage);
		++iterImage;
	}
	m_vDefaultImageList.clear();

#if defined(_USED_TEXTURE_TABLE)
	m_hmTextureTableList.clear();
#endif

	std::map<int, MSG_BOARD*>::iterator  itMsgBoard = m_MsgBoardMap.begin();
	while( itMsgBoard != m_MsgBoardMap.end() )
	{
		delete itMsgBoard->second;
		itMsgBoard++;
	}
	m_MsgBoardMap.clear();

}


bool SPGOBCluster::Init(string strFilePath)
{
	bool bRet = true;

#ifdef _LATALE_CLIENT
	#ifndef _JUST_RUN
		#if defined(_DEBUG)
			DXUTOutputDebugString("SPGOBCluster::Init()\n");
		#endif

		string strLDTFile;
		strLDTFile = "DATA/LDT/MOTIONTYPE.LDT";

		if( strFilePath.empty() )
		{
			//strFilePath = RES_WORK_RESOURCE_PATH;
			//strFilePath += "Data/AniTable/";
			strFilePath = "DATA/ANITABLE/";
		}

		bRet = LoadBaseLDTFile(strLDTFile, strFilePath);

		strLDTFile = "DATA/LDT/LOBBY_MOTION.LDT";
		bRet = LoadLobbyLDTFile(strLDTFile, strFilePath);

		strLDTFile = "DATA/LDT/SKILL_MOTION.LDT";
		bRet = LoadSkillLDTFile(strLDTFile, strFilePath);

		strLDTFile = "DATA/LDT/messenger.LDT";
		bRet = LoadMessengerLDTFile(strLDTFile, strFilePath);

		#if defined(_USED_TEXTURE_TABLE)
			bRet = LoadTextureTableMetaFile( "Character.mft" , RES_PATH_CHAR );

			if( bRet == false )
			{
				strLDTFile = "DATA/CHAR/CHARLAYER/Character.tbl";
				bRet = LoadTextureTable(strLDTFile, "");
				bRet = LoadDefaultLayerImage("", RES_PATH_CHAR );
			}
		
		#else
			strLDTFile = "DATA/LDT/DEFAULT_LAYER.LDT";
			bRet = LoadDefaultLayerImage(strLDTFile, strFilePath);
		#endif
	#else
		SPBattleCluster::GetInstance();
	#endif
#endif

	return bRet;
}

#if defined(_LATALE_CLIENT)
bool SPGOBCluster::LoadBaseLDTFile(string strFileName, string strTablePath)
{
	bool bRet = true;
	
	SPBattleCluster::GetInstance();
	SPBattleLoader kBattleLoader;

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strFileName.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iFieldNum, i, j;
	unsigned int uiAniIndex;
	long lItemID;
	LDT_Field Field;
	string strTableFileName;

	iDataNum = pkLDTFile->GetItemCount();
	iFieldNum = pkLDTFile->GetFieldCount();

	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);
		for( j = 0; j < iFieldNum; j++ )
		{
			pkLDTFile->GetField(lItemID, j, Field);

			uiAniIndex = GetGOBSeqKey(lItemID, j + 1);
			if( uiAniIndex )
			{
				strTableFileName = strTablePath + Field.uData.pValue + "_r" + ".seq";
				bRet = LoadBinary(uiAniIndex, strTableFileName.c_str());
				if( !bRet )
					return false;

				strTableFileName = strTablePath + Field.uData.pValue + "_r" + ".drg";
				kBattleLoader.LoadBinaryBattleFile(strTableFileName, true, false, uiAniIndex);

				strTableFileName = strTablePath + Field.uData.pValue + "_r" + ".arg";
				kBattleLoader.LoadBinaryBattleFile(strTableFileName, true, true, uiAniIndex);
			}
		}
	}
	SAFE_RELEASE(pkLDTFile);

	return bRet;
}

bool SPGOBCluster::LoadSkillLDTFile(string strFileName, string strTablePath)
{
	bool bRet = true;

	SPBattleCluster::GetInstance();
	SPBattleLoader kBattleLoader;

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strFileName.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, i;
	unsigned int uiAniIndex;
	long lItemID;
	LDT_Field Field;
	string strTableFileName;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);
		uiAniIndex = lItemID;

		pkLDTFile->GetField(lItemID, 0, Field);		// _Motion
			
		strTableFileName = strTablePath + Field.uData.pValue + "_r" + ".seq";
		bRet = LoadBinary(uiAniIndex, strTableFileName.c_str());
		if( !bRet )
			return false;

		strTableFileName = strTablePath + Field.uData.pValue + "_r" + ".drg";
		kBattleLoader.LoadBinaryBattleFile(strTableFileName, true, false, uiAniIndex);

		strTableFileName = strTablePath + Field.uData.pValue + "_r" + ".arg";
		kBattleLoader.LoadBinaryBattleFile(strTableFileName, true, true, uiAniIndex);
	}

	SAFE_RELEASE(pkLDTFile);
	return bRet;
}

bool SPGOBCluster::LoadLobbyLDTFile(string strFileName, string strTablePath)
{
	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strFileName.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i;
	string strTableFileName;
	LDT_Field LFD_Motion, LFD_FX, LFD_SOUND;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);

		pkLDTFile->GetField(iItemID, 0,	LFD_Motion);		//_Motion
		pkLDTFile->GetField(iItemID, 1,	LFD_FX);			//_FX
		pkLDTFile->GetField(iItemID, 2,	LFD_SOUND);			//_Sound

		strTableFileName = strTablePath + LFD_Motion.uData.pValue + "_r" + ".seq";
		if( !LoadBinary(iItemID, strTableFileName.c_str()) )
			return false;
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
}

bool SPGOBCluster::LoadMessengerLDTFile( string strFileName, string strTablePath )
{
	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strFileName.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i;
	string		strTableFileName;
	LDT_Field	LDTField;
	
	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		MSG_BOARD * pMsgBoard = new MSG_BOARD;
		if ( !pMsgBoard )
			return false;

		iItemID = pkLDTFile->GetPrimaryKey(i);
		pMsgBoard->m_iTypeID = iItemID;

		pkLDTFile->GetField(iItemID, 0,	LDTField);
		pMsgBoard->m_itemGUID = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 1,	LDTField);
		pMsgBoard->m_strName = LDTField.uData.pValue;

		pkLDTFile->GetField(iItemID, 2,	LDTField);
		pMsgBoard->m_iIcon = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 3,	LDTField);
		pMsgBoard->m_iImageX1 = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 4,	LDTField);
		pMsgBoard->m_iImageY1 = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 5,	LDTField);
		pMsgBoard->m_iImageX2 = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 6,	LDTField);
		pMsgBoard->m_iImageY2 = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 7,	LDTField);
		pMsgBoard->m_iTextX1 = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 8,	LDTField);
		pMsgBoard->m_iTextY1 = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 9,	LDTField);
		pMsgBoard->m_iTextX2 = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 10,	LDTField);
		pMsgBoard->m_iTextY2 = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 11,	LDTField);
		pMsgBoard->m_iTitleX = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 12,	LDTField);
		pMsgBoard->m_iTitleY = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 13,	LDTField);
		pMsgBoard->m_dwTime = LDTField.uData.lValue;

		pkLDTFile->GetField(iItemID, 14,	LDTField);
		pMsgBoard->m_destoryGUID = LDTField.uData.lValue;

		m_MsgBoardMap[pMsgBoard->m_iTypeID] = pMsgBoard;
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
}

bool SPGOBCluster::LoadDefaultLayerImage(string strFileName, string strTablePath)
{
#if defined(_USED_TEXTURE_TABLE)

	bool bRet;
	if( g_bUsePackFile )
		bRet = LoadTextureTablePackImage( strTablePath );
	else
		bRet = LoadTextureTableImage( strTablePath );

	return bRet;
#else
	char szName[MAX_PATH];
	SPTexture* pkTexture = NULL;

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strFileName.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i;
	LDT_Field LFD_LayerFile;
	
	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);
		pkLDTFile->GetFieldFromLable(iItemID, "_Layer_Image",	LFD_LayerFile);

		sprintf(szName, "%s%s.PNG", RES_PATH_CHAR, LFD_LayerFile.uData.pValue);
		g_pVideo->GetTextureMgr()->LoadTexture(szName, &pkTexture);
		if( pkTexture )
			m_vDefaultImageList.push_back(pkTexture);
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
#endif

}

bool SPGOBCluster::LoadTextureTableImage( string strPath )
{
	SPTexture* pkTexture = NULL;
	char szName[MAX_PATH];

	HANDLE hHandle;
	WIN32_FIND_DATA FindData;

	sprintf(szName, "%s%s*.png", RES_WORK_RESOURCE_PATH, strPath.c_str() );
	hHandle = FindFirstFile(szName, &FindData);

	if( hHandle ==  INVALID_HANDLE_VALUE )
		return false;

	sprintf(szName, "%s%s", strPath.c_str() , FindData.cFileName);
	g_pVideo->GetTextureMgr()->LoadTexture(szName, &pkTexture);
	if( pkTexture )
		m_vDefaultImageList.push_back(pkTexture);

	bool bExist = true;
	while( bExist )
	{
		bExist = FindNextFile(hHandle, &FindData);

		sprintf(szName, "%s%s", strPath.c_str() , FindData.cFileName);
		//g_pVideo->GetTextureMgr()->LoadTexture(szName, &pkTexture);
		g_pVideo->GetTextureMgr()->GetTexture( szName, &pkTexture, TEX_CHAR );
		if( pkTexture )
			m_vDefaultImageList.push_back(pkTexture);
	}

	return true;
}

extern float g_TexMaxSize[TYPE_NUM];

bool SPGOBCluster::LoadTextureTablePackImage( string strPath )
{
	SPResourceBase* pkResourceBase = SPResourceBase::GetInstance();
	if( pkResourceBase == NULL )
		return false;

	SPTexture* pkTexture = NULL;
	char szDir[MAX_PATH], szName[MAX_PATH];
	bool bResult;
	int iResourceInstance;
	SPStream* pStream;

	sprintf(szDir, "%s%s", RES_WORK_RESOURCE_PATH, strPath.c_str() );
	bResult = pkResourceBase->FindPackFirst(szDir, szName, "png");
	if( !bResult )
		return false;

	if(1)
	{
		do
		{
			g_pVideo->GetTextureMgr()->GetTexture(szName, &pkTexture,TEX_CHAR);
			if( pkTexture )
				m_vDefaultImageList.push_back(pkTexture);
		}while(pkResourceBase->FindPackNext(szName));
	}
	else
	{
		int ii = 0;
		do
		{
			ii ++;
			g_pVideo->GetTextureMgr()->GetTexture(szName, &pkTexture,TEX_CHAR);
			if( pkTexture )
				m_vDefaultImageList.push_back(pkTexture);
			if(ii > 65)
				break;

		}while(pkResourceBase->FindPackNext(szName));
	}

	return true;
}

#if defined(_USED_TEXTURE_TABLE)
bool SPGOBCluster::LoadTextureTable(string strFileName, string strTablePath)
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

	if( strTempPath.find( RES_PATH_CHAR ) == 0 )
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

TEXTURE_TABLE_INFO* SPGOBCluster::GetTextureTable(string strID)
{
	hash_map< std::string, TEXTURE_TABLE_INFO >::iterator iterTable;
	iterTable = m_hmTextureTableList.find(strID);
	if( iterTable == m_hmTextureTableList.end() )
		return NULL;

	return &(iterTable->second);
}

FRAME_INFO* SPGOBCluster::GetFrame(TEXTURE_TABLE_INFO* pTableInfo, int iFrame)
{
	if( pTableInfo == NULL )
		return NULL;

	hash_map< int, FRAME_INFO >::iterator iterFrame;
	iterFrame = pTableInfo->hmFrameList.find(iFrame);

	if( iterFrame == pTableInfo->hmFrameList.end() )
		return NULL;

	return &(iterFrame->second);
}


FRAME_INFO* SPGOBCluster::GetFrame(string strID, int iFrame)
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

bool SPGOBCluster::LoadTextureTableMetaFile( string strFileName , string strTablePath )
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
	std::string	strTempFullPath;
	std::string	strTempPath;
	std::string	strTempName;

	for( int i = 0 ; i < iFileCount ; ++i )
	{
		pStream->Read( &iSize , sizeof( iSize ) );

		if( iSize > 0 )
		{
			pStream->Read( szBuf , sizeof( char) * iSize );

			strTempFullPath = strTablePath + szBuf;

			GetSeperateStringDir( strTempFullPath , strTempPath , strTempName );

			LoadTextureTable( szBuf , strTablePath );

			LoadDefaultLayerImage( strTempName.c_str() , strTempPath.c_str() );

			memset( szBuf , 0 , sizeof( szBuf ) );
		}
	}

	SAFE_RELEASE( pStream );

	return true;

}

#endif

#endif

bool SPGOBCluster::Load(unsigned int uiAniIndex, string strFilePath)
{
	bool bRet = false;
	SPAniTableLoader AniTableLoader;
	string strTemp, strFileName;

	if( strFilePath.empty() )
		return false;

	SPGOBAnimation* pkAnimation = NULL;
	pkAnimation = GetAnimation(uiAniIndex);
	if( pkAnimation )
		return true;

	GetSeperateString(strFilePath, strTemp, strFileName);
	pkAnimation = GetAnimation(strFileName.c_str());
	if( pkAnimation )
	{
		m_hmAnimation.insert(hash_map< unsigned int, SPGOBAnimation* >::value_type(uiAniIndex, pkAnimation));
		return true;
	}

	bRet = AniTableLoader.LoadAniPallet(&pkAnimation, strFilePath);
	if( pkAnimation == NULL )
		return false;

	m_hmAnimationPool.insert(hash_map< string, SPGOBAnimation* >::value_type(strFileName, pkAnimation));
	m_hmAnimation.insert(hash_map< unsigned int, SPGOBAnimation* >::value_type(uiAniIndex, pkAnimation));
	return bRet;
}

bool SPGOBCluster::LoadBinary(unsigned int uiAniIndex, string strFileName)
{
	bool bRet = false;
	SPAniTableLoader AniTableLoader;

	if( strFileName.empty() )
		return false;

	SPGOBAnimation* pkAnimation = NULL;
	pkAnimation = GetAnimation(uiAniIndex);
	if( pkAnimation )
		return true;

	pkAnimation = GetAnimation(strFileName.c_str());
	if( pkAnimation )
	{
		m_hmAnimation.insert(hash_map< unsigned int, SPGOBAnimation* >::value_type(uiAniIndex, pkAnimation));
		return true;
	}

	bRet = AniTableLoader.LoadBinaryAni(&pkAnimation, strFileName);
	if( pkAnimation == NULL )
		return false;

	m_hmAnimationPool.insert(hash_map< string, SPGOBAnimation* >::value_type(strFileName, pkAnimation));
	m_hmAnimation.insert(hash_map< unsigned int, SPGOBAnimation* >::value_type(uiAniIndex, pkAnimation));
	return bRet;
}

bool SPGOBCluster::GetAnimationTime(const unsigned int uiAniInx, float& fTotalTime)
{
	fTotalTime = 0.0f;
	SPGOBAnimation* pkAnimation = GetAnimation(uiAniInx);
	if( pkAnimation == NULL )
		return false;

#if defined(_LATALE_CLIENT)
	fTotalTime = pkAnimation->fAccumulateTime;
	return true;

#else
	// Delay Time을 누적한다음 return한다.
	std::vector<SPGOBAniSeq>::iterator	IterAniSeq = pkAnimation->vAniSeq.begin();
	while( IterAniSeq != pkAnimation->vAniSeq.end() )
	{
		fTotalTime += (*IterAniSeq).fDelayTime;
		++IterAniSeq;
	}
#endif

	return true;
}

////////////////////////////////////////////////////////////////////
#ifdef _CHARTOOL		

void SPGOBCluster::SetAnimationFrame(const unsigned int uiAniInx, int nFrame, SPGOBAniSeq GOBAniSeq)
{
	SPGOBAnimation* pkAnimation = GetAnimation(uiAniInx);
	if( pkAnimation )
		pkAnimation->vAniSeq[nFrame] = GOBAniSeq;
}


void SPGOBCluster::SetAnimation(const unsigned int uiAniInx, SPGOBAnimation* pGOBAnimation)
{
	SPGOBAnimation* pkDstAnimation = GetAnimation(uiAniInx);
	if( pkDstAnimation )
	{
		if( pkDstAnimation == pGOBAnimation )
			return;

		m_hmAnimation[uiAniInx] = pGOBAnimation;
	}
}

#endif		

SPGOBAniSeq	SPGOBCluster::GetAnimationFrame(const unsigned int uiAniInx, int nFrame)
{
	SPGOBAniSeq pDummy;
	// 해당 Animation을 찾아서
	hash_map< unsigned int, SPGOBAnimation* >::iterator	iter;
	iter = m_hmAnimation.find(uiAniInx);
	if( iter != m_hmAnimation.end() )
		return iter->second->vAniSeq[nFrame];

	pDummy.SetNull();
	return pDummy;
}

SPGOBAnimation* SPGOBCluster::GetAnimation(const char* pszName)
{
	stdext::hash_map< std::string, SPGOBAnimation* >::iterator iter;
	iter = m_hmAnimationPool.find(pszName);
	if( iter != m_hmAnimationPool.end() )
		return iter->second;

	return NULL;
}

SPGOBAnimation*	SPGOBCluster::GetAnimation(const unsigned int uiAniInx)
{
	hash_map< unsigned int, SPGOBAnimation* >::iterator iter;
	iter = m_hmAnimation.find(uiAniInx);
	if( iter != m_hmAnimation.end() )
		return iter->second;

	return NULL;
}
////////////////////////////////////////////////////////////////////

bool SPGOBCluster::QueryAnimationSequence(	SPGOBModel** ppGOBModel,	// 받아올 모델
											unsigned int uiAniInx,		// 애니메이션 인덱스
											int& iAniFrame, bool bLoop, bool bWeaponVisible)	// 에니메이션 프레임(내부 조정된다)
{
	assert( ppGOBModel);
	assert( *ppGOBModel );
	if(ppGOBModel == NULL || *ppGOBModel == NULL) return false;
	(*ppGOBModel)->SetNull();

	std::vector<SPGOBAniSeq>::iterator		IterAniSeq;

	SPGOBAnimation* pkAnimation;
	pkAnimation = GetAnimation(uiAniInx);
	if( pkAnimation == NULL ) {
		return false;
	}

	int iTotalFrame = (int)pkAnimation->vAniSeq.size();

#ifdef _CHARTOOL
	++iAniFrame;
	if( iAniFrame >=  iTotalFrame )
		iAniFrame = 0;
#endif

	
	// 찾은 애니메숑에서 원하는 시퀀스 가져오기
	IterAniSeq = pkAnimation->vAniSeq.begin() + iAniFrame;
	(*ppGOBModel)->iAniSequence = iAniFrame;
	(*ppGOBModel)->iMaxSequence = iTotalFrame;
	(*ppGOBModel)->fDelayTime	= (*IterAniSeq).fDelayTime;
	(*ppGOBModel)->uiAniIndex = uiAniInx;

	// 애니메이션 파트별로 m_vGOBPart(로드된이미지) 중에서매치된것을 찾아 좌표와 이미지를 넣는다. 	
	SPGOBInfoSequencePart InfoSeqPart;
	std::vector<SPGOBAniSeqPart>::iterator IterAniSeqPart = (*IterAniSeq).vAniSeqPart.begin();
	for( ; IterAniSeqPart != (*IterAniSeq).vAniSeqPart.end(); ++IterAniSeqPart ) {

		// Version 2.0 방식, 서치가 없어졌다
		if( (*IterAniSeqPart).PartLayer >= PL_FIRST && (*IterAniSeqPart).PartLayer < PL_LAST ) {
			InfoSeqPart.SetNull();

			InfoSeqPart.PartLayer = (*IterAniSeqPart).PartLayer;
			InfoSeqPart.iRX = (*IterAniSeqPart).iRX;
			InfoSeqPart.iRY = (*IterAniSeqPart).iRY;
			InfoSeqPart.iRotationDegree = (*IterAniSeqPart).iRotationDegree;
			InfoSeqPart.iResourceIndex = (*IterAniSeqPart).iResourceIndex;
			InfoSeqPart.bVisable = (*IterAniSeqPart).bVisable;
			InfoSeqPart.bFlip = (*IterAniSeqPart).bFlip;
#ifdef _CHARTOOL			
			InfoSeqPart.bSelect = (*IterAniSeqPart).bSelect;
#endif

			if (InfoSeqPart.PartLayer == PL_WEAPON_IN || InfoSeqPart.PartLayer == PL_WEAPON_OUT)
			{
				if (bWeaponVisible == false) InfoSeqPart.bVisable = false;
			}
			(*ppGOBModel)->vGOBInfoSequencePart.push_back(  InfoSeqPart );

		} else {
			assert( 0 && "Animation Seq failed");
		}
	}	

#ifndef _CHARTOOL
	++iAniFrame;
	if( iAniFrame >= iTotalFrame )
	{
		if( bLoop )
			iAniFrame  = 0;
		else
			iAniFrame = iTotalFrame - 1;
	}
#endif

	return true;
}



MSG_BOARD * SPGOBCluster::GetMsgBoardByType(int iType)
{
	std::map<int, MSG_BOARD *>::iterator  it = m_MsgBoardMap.find(iType);
	if ( it != m_MsgBoardMap.end() )
		return it->second;
	return NULL;
}

MSG_BOARD * SPGOBCluster::GetMsgBoardByItme(UINT64 iItemID)
{
	std::map<int, MSG_BOARD *>::iterator  it = find_if(m_MsgBoardMap.begin(), m_MsgBoardMap.end(), 
								FindNode_pairPtr(iItemID, MSG_BOARD, m_itemGUID, GU_ID));
	for ( it = m_MsgBoardMap.begin(); it != m_MsgBoardMap.end(); it++ )
	{
		if ( it->second->m_itemGUID == iItemID )
			return it->second;
	}
	return NULL;
}







