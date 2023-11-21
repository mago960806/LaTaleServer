// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-19 오전 10:26:19
//***************************************************************************

#pragma once

#include <vector>
#include <hash_map>

class SPTexture;

struct SPGOBInfoSequencePart;
struct SPGOBAniSeq;
struct SPGOBModel;
struct SPGOBAnimation;

struct SPMOBModel;
struct SPMOBAnimation;

#ifdef _LATALE_CLIENT
class SPGOBManager;
#endif

struct FRAME_INFO
{
	int iFrameNo;
	int iOffsetX;
	int iOffsetY;
	float fCenterOffsetX;
	float fCenterOffsetY;

	RECT rcResourceRgn;
	std::string strTextureName;
};

struct TEXTURE_TABLE_INFO
{
	int iTotalFrame;
	std::string strID;
	std::string strTextureName;

	stdext::hash_map< int, FRAME_INFO > hmFrameList;

	~TEXTURE_TABLE_INFO()
	{
		hmFrameList.clear();
	}
};

struct MFTStreamHeader
{
	enum { BINARY_CHECK = 415 };

	char			szDescription[255];
	int				iBinary;
	float			fVersion;
	unsigned long	ulExpansion;

	MFTStreamHeader()
	{
		memset( szDescription , 0 , sizeof( szDescription ) );
		iBinary		=	0;
		fVersion	=	0.0f;
		ulExpansion	=	0;
	}
};

//[xialin 2008/09/10]add
struct MSG_BOARD
{
	int			m_iTypeID;
	GU_ID		m_itemGUID;
	string		m_strName;
	int			m_iIcon;
	int			m_iImageX1;
	int			m_iImageY1;
	int			m_iImageX2;
	int			m_iImageY2;
	int			m_iTextX1;
	int			m_iTextY1;
	int			m_iTextX2;
	int			m_iTextY2;
	int			m_iTitleX;
	int			m_iTitleY;
	DWORD       m_dwTime;
	GU_ID		m_destoryGUID;

	MSG_BOARD()
	{
		m_iTypeID = 0;
		m_itemGUID = 0;
		m_strName = "";
		m_iIcon = 0;
		m_iImageX1 = 0;
		m_iImageY1 = 0;
		m_iImageX2 = 0;
		m_iImageY2 = 0;
		m_iTextX1 = 0;
		m_iTextY1 = 0;
		m_iTextX2 = 0;
		m_iTextY2 = 0;
		m_iTitleX = 0;
		m_iTitleY = 0;
	    m_dwTime = 0;
		m_destoryGUID = 0;
	}

};

using namespace std;

class SPGOBCluster {

public:

#ifdef _LATALE_CLIENT
	SPGOBCluster(SPGOBManager* pParent);
#else 
	SPGOBCluster();
#endif


	~SPGOBCluster();
	bool Init(string strFilePath="");
	void Clean();

#ifdef _LATALE_CLIENT
	bool LoadBaseLDTFile(string strFileName, string strTablePath);
	bool LoadSkillLDTFile(string strFileName, string strTablePath);
	bool LoadLobbyLDTFile(string strFileName, string strTablePath);
	bool LoadDefaultLayerImage(string strFileName, string strTablePath);
	bool LoadTextureTable(string strFileName, string strTablePath);
	bool LoadMessengerLDTFile( string strFileName, string strTablePath );

	bool LoadTextureTableImage( string strPath );
	bool LoadTextureTablePackImage( string strPath );
	

#endif

#if defined(_USED_TEXTURE_TABLE)
	
	TEXTURE_TABLE_INFO* GetTextureTable(string strID);
	FRAME_INFO* GetFrame(TEXTURE_TABLE_INFO* pTableInfo, int iFrame);
	FRAME_INFO* GetFrame(string strID, int iFrame);

	bool LoadTextureTableMetaFile( string strFileName , string strTablePath );

#endif

	bool Load(unsigned int uiAniIndex, string strFilePath = "");
	bool LoadBinary(unsigned int uiAniIndex, string strFileName);

	bool QueryAnimationSequence (	SPGOBModel** ppGOBModel,	// 받아올 모델
									unsigned int uiAniInx,		// 애니메이션 인덱스
									int& iAniFrame, bool bLoop = true, bool bWeaponVisible = true );			// 에니메이션 프레임(내부 조정된다)	
	
	bool GetAnimationTime(const unsigned int uiAniInx, float& fTotalTime);
#ifdef _CHARTOOL
	void			SetAnimationFrame(const unsigned int uiAniInx, int nFrame, SPGOBAniSeq  GOBAniSeq);
	void			SetAnimation(const unsigned int uiAniInx, SPGOBAnimation* pGOBAnimation);	
#endif

	SPGOBAnimation*	GetAnimation(const unsigned int uiAniInx);
	SPGOBAnimation* GetAnimation(const char* pszName);
	SPGOBAniSeq		GetAnimationFrame(const unsigned int AniInx, int nFrame);

	MSG_BOARD *     GetMsgBoardByType(int iType);
	MSG_BOARD *     GetMsgBoardByItme(UINT64 iItemID);

private:
	
	stdext::hash_map< unsigned int, SPGOBAnimation* > m_hmAnimation;
	stdext::hash_map< std::string, SPGOBAnimation* > m_hmAnimationPool;

	std::vector< SPTexture* > m_vDefaultImageList;
	
#ifdef _LATALE_CLIENT
	SPGOBManager*							m_pParent;	
#endif

#if defined(_USED_TEXTURE_TABLE)
	stdext::hash_map< std::string, TEXTURE_TABLE_INFO > m_hmTextureTableList;
#endif

	//[xialin 2008/09/10]add
	std::map<int, MSG_BOARD *>            m_MsgBoardMap;
};

extern SPGOBCluster* g_pGOBCluster;












