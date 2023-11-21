// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPAvatarModelUnitDef
// Comment     : 
// Creation    : metalgeni 2004-11-02 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

#include <vector>
#include <string>
using namespace std;

class	SPTexture;
enum	ANIMATION_INDEX;
struct TEXTURE_TABLE_INFO;
struct FRAME_INFO;

/**Monster Index enum : tiny, small, medium, big*/
enum MONSTER_INDEX
{
	MON_TINY = 1, MON_SMALL, MON_MED, MON_BIG
};

enum MONSTER_TYPE
{
	MT_NULL = 0, MT_MONSTER, MT_EVENT, MT_BAG, MT_PET, MT_LAYER_PET, MT_LAYER_BOSS,
};

struct SPMOBAniFrameInfo
{
	int iNum;
	int iLayer;
	int iResourceIndex;
	int iPosX;
	int iPosY;
	int iRotation;
	
	float fScaleX;
	float fScaleY;
	
	float fColorR;
	float fColorG;
	float fColorB;
	float fColorA;

	float fDelay;

	SPMOBAniFrameInfo();
	void SetNull();

	SPMOBAniFrameInfo& operator = (const SPMOBAniFrameInfo& srcInfo);
};

struct SPMOBAnimation
{
	ANIMATION_INDEX eAniIndex;
	MONSTER_INDEX eMobSize;
	MONSTER_TYPE eMobType;

	float fAccumulateTime;

	int iFrameCountX;
	int iFrameCountY;
	int iFrameWidth;
	int iFrameHeight;

	std::string strImageName;
	std::vector< SPMOBAniFrameInfo > vAniFrameInfo;
	
	SPMOBAnimation();
	~SPMOBAnimation();
	void	SetNull();

	SPMOBAnimation& operator = (const SPMOBAnimation& srcAni);
};

struct SPMOBModel
{
	ANIMATION_INDEX eAniIndex;
	MONSTER_INDEX eMobSize;
	MONSTER_TYPE eMobType;

	unsigned int uiID;
	unsigned int uiKind;

	RECT rcSrc;
	RECT rcDst;

	int iCurAniFrame;
	int iMaxAniFrame;
	
	int iFrameCountX;
	int iFrameCountY;
	int iFrameWidth;
	int iFrameHeight;
	float fAccumulateTime;

	int iDefaultRotation;

	SPMOBAniFrameInfo FrameInfo;
	std::vector< SPTexture* > vpImage;

#if defined(_USED_TEXTURE_TABLE)
	std::vector< TEXTURE_TABLE_INFO* > vpTableInfo;
	FRAME_INFO* pFrameInfo;
#endif

	SPMOBModel();
	~SPMOBModel();

	void SetNull();
	void ReleaseImage();
	bool IsNull();

	void Render();
	bool ReloadTexture(TCHAR *strImageName);

#if defined(_USED_TEXTURE_TABLE)
	bool ReloadTexturebyTable(TCHAR *strImageName, TCHAR *strImagePath);
#endif

#if defined(_CHARTOOL)
	bool InsertTexture(const char* pszFileName);
#endif
};

struct SPMOBUIInfo
{
	std::string		m_strMOBName;
	int				m_iMOBIcon;
	int				m_iMOBIconIndex;
	std::string		m_strMOBTip;
	std::string		m_strMOBDesc;

	bool			m_bShowName;
	bool			m_bShowHP;
	bool			m_bShowShadow;
	int				m_iRespawnFX;
};

struct SPMOBLayerAniFrameInfo
{
	int iNo;
	int iZOrder;
	int iResourceIndex;
	int iPosX;
	int iPosY;
	int iRotation;
	bool bVisible;

	float fDelay;
	float fScaleX;
	float fScaleY;

	float fColorR;
	float fColorG;
	float fColorB;
	float fColorA;

	SPMOBLayerAniFrameInfo();
	void SetNull();
};

struct SPMOBLayerAnimation
{
	ANIMATION_INDEX eAniIndex;
	float fAccumulateTime;

	int iTotalFrame;
	std::vector< SPMOBLayerAniFrameInfo > vFrameInfo;

	SPMOBLayerAnimation();
	~SPMOBLayerAnimation();
	void Clear();

	SPMOBLayerAnimation& operator = (const SPMOBLayerAnimation& Src);
};

struct SPMOBLayerInfo
{
	int iFrameWidth;
	int iFrameHeight;
	int iFrameCountX;
	int iFrameCountY;
	bool bLightMap;

	std::string strName;
	std::string strImageName;
	std::vector< SPMOBLayerAnimation > vAnimation;

	SPMOBLayerInfo();
	~SPMOBLayerInfo();
	void Clear();

	SPMOBLayerInfo& operator = (const SPMOBLayerInfo& Src);
};

struct SPMOBLayerModel
{
	ANIMATION_INDEX eAniIndex;
	int iLayerNo;

	RECT rcSrc;
	RECT rcDst;

	int iCurAniFrame;
	int iMaxAniFrame;
	
	bool bLightMap;
	int iFrameCountX;
	int iFrameCountY;
	int iFrameWidth;
	int iFrameHeight;
	float fAccumulateTime;

	int iDefaultRotation;

	SPMOBLayerAniFrameInfo FrameInfo;
	SPTexture* pkTexture;

#if defined(_USED_TEXTURE_TABLE)
	std::vector< TEXTURE_TABLE_INFO* > vpTableInfo;
	FRAME_INFO* pFrameInfo;
#endif

	SPMOBLayerModel();
	~SPMOBLayerModel();

	void SetNull();
	void ReleaseImage();
	bool IsNull();

	void Render();
};