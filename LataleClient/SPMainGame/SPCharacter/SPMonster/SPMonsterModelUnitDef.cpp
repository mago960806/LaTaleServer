
#ifdef _LATALE_CLIENT
#include "SPCommon.h"
#else 
#include "SPToolCommon.h"
#endif

#include "SPTexture.h"
#include "SPUtil.h"
#include "SPResourceDef.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"

#if defined(_USED_TEXTURE_TABLE)
#include "SPMOBCluster.h"
#endif

#include "SPMonsterModelUnitDef.h"

using namespace std;
//------------------------------------------------------------------------------------
SPMOBAniFrameInfo::SPMOBAniFrameInfo()
{
	SetNull();
}

//------------------------------------------------------------------------------------
void SPMOBAniFrameInfo::SetNull()
{
	iNum = -1;
	iLayer = 0;
	iResourceIndex = 0;
	iPosX = 0;
	iPosY = 0;
	iRotation = 0;

	fScaleX = 1.0f;
	fScaleY = 1.0f;

	fColorR = fColorG = fColorB = fColorA = 1.0f;
	fDelay = 0.0f;
}

//------------------------------------------------------------------------------------
SPMOBAniFrameInfo& SPMOBAniFrameInfo::operator = (const SPMOBAniFrameInfo& srcInfo)
{
	iNum = srcInfo.iNum;
	iLayer = srcInfo.iLayer;
	iResourceIndex = srcInfo.iResourceIndex;
	iPosX = srcInfo.iPosX;
	iPosY = srcInfo.iPosY;
	iRotation = srcInfo.iRotation;
	
	fScaleX = srcInfo.fScaleX;
	fScaleY = srcInfo.fScaleY;
	
	fColorR = srcInfo.fColorR;
	fColorG = srcInfo.fColorG;
	fColorB = srcInfo.fColorB;
	fColorA = srcInfo.fColorA;

	fDelay = srcInfo.fDelay;

	return *this;
}

//------------------------------------------------------------------------------------
SPMOBAnimation::SPMOBAnimation()
{
	SetNull();
}

//------------------------------------------------------------------------------------
SPMOBAnimation::~SPMOBAnimation()
{
	SetNull();
}

//------------------------------------------------------------------------------------
void SPMOBAnimation::SetNull()
{
	eAniIndex = ANI_NULL;
	eMobSize = MON_TINY;
	eMobType = MT_NULL;

	fAccumulateTime = 0.0f;
	iFrameCountX = 0;
	iFrameCountY = 0;
	iFrameWidth = 0;
	iFrameHeight = 0;

	strImageName = "";
	vAniFrameInfo.clear();
}

//------------------------------------------------------------------------------------
SPMOBAnimation& SPMOBAnimation::operator = (const SPMOBAnimation& srcAni)
{
	eAniIndex = srcAni.eAniIndex;
	eMobSize = srcAni.eMobSize;
	eMobType = srcAni.eMobType;

	fAccumulateTime = srcAni.fAccumulateTime;

	iFrameCountX = srcAni.iFrameCountX;
	iFrameCountY = srcAni.iFrameCountY;
	iFrameWidth = srcAni.iFrameWidth;
	iFrameHeight = srcAni.iFrameHeight;

	strImageName = srcAni.strImageName;
	vAniFrameInfo = srcAni.vAniFrameInfo;
	return *this;
}

//------------------------------------------------------------------------------------
SPMOBLayerAniFrameInfo::SPMOBLayerAniFrameInfo()
{
	SetNull();
}

//------------------------------------------------------------------------------------
void SPMOBLayerAniFrameInfo::SetNull()
{
	iNo = 0;
	iZOrder = 0;
	iResourceIndex = 0;
	iPosX = 0;
	iPosY = 0;
	iRotation = 0;
	bVisible = true;

	fDelay = 0.0f;
	fScaleX = 1.0f;
	fScaleY = 1.0f;

	fColorR = 1.0f;
	fColorG = 1.0f;
	fColorB = 1.0f;
	fColorA = 1.0f;
}

//------------------------------------------------------------------------------------
SPMOBLayerInfo::SPMOBLayerInfo()
{
	Clear();
}

//------------------------------------------------------------------------------------
SPMOBLayerInfo::~SPMOBLayerInfo()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPMOBLayerInfo::Clear()
{
	iFrameWidth = 0;
	iFrameHeight = 0;
	iFrameCountX = 0;
	iFrameCountY = 0;
	bLightMap = false;

	strName.clear();
	strImageName.clear();
	vAnimation.clear();
}

//------------------------------------------------------------------------------------
SPMOBLayerInfo& SPMOBLayerInfo::operator = (const SPMOBLayerInfo& Src)
{
	iFrameWidth = Src.iFrameWidth;
	iFrameHeight = Src.iFrameHeight;
	iFrameCountX = Src.iFrameCountX;
	iFrameCountY = Src.iFrameCountY;
	bLightMap = Src.bLightMap;

	strName = Src.strName;
	strImageName = Src.strImageName;
	vAnimation = Src.vAnimation;

	return *this;
}

//------------------------------------------------------------------------------------
SPMOBLayerAnimation::SPMOBLayerAnimation()
{
	Clear();
}

//------------------------------------------------------------------------------------
SPMOBLayerAnimation::~SPMOBLayerAnimation()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPMOBLayerAnimation::Clear()
{
	fAccumulateTime = 0.0f;
	iTotalFrame = 0;
	eAniIndex = ANI_NULL;
	vFrameInfo.clear();
}

//------------------------------------------------------------------------------------
SPMOBLayerAnimation& SPMOBLayerAnimation::operator = (const SPMOBLayerAnimation& Src)
{
	eAniIndex = Src.eAniIndex;
	fAccumulateTime = Src.fAccumulateTime;
	iTotalFrame = Src.iTotalFrame;
	vFrameInfo = Src.vFrameInfo;

	return *this;
}

//------------------------------------------------------------------------------------
SPMOBModel::SPMOBModel()
{
	SetNull();
#if defined(_USED_TEXTURE_TABLE)
	vpTableInfo.clear();
	pFrameInfo = NULL;
#endif
}

//------------------------------------------------------------------------------------
SPMOBModel::~SPMOBModel()
{
	SetNull();
	ReleaseImage();
}

//------------------------------------------------------------------------------------
void SPMOBModel::SetNull()
{
	eAniIndex = ANI_NULL;
	eMobSize = MON_TINY;
	eMobType = MT_NULL;

	uiID = 0;
	uiKind = 0;

	iCurAniFrame = 0;
	iMaxAniFrame = 0;

	iFrameCountX = 0;
	iFrameCountY = 0;
	iFrameWidth = 0;
	iFrameHeight = 0;

	fAccumulateTime = 0.0f;

	iDefaultRotation = 0;
	FrameInfo.SetNull();
}

//------------------------------------------------------------------------------------
void SPMOBModel::ReleaseImage()
{
	SPTexture* pkTexture;
	vector< SPTexture* >::iterator iter = vpImage.begin();
	while( iter != vpImage.end() )
	{
		pkTexture = *iter;
		pkTexture->Release();
		iter++;
	}
	vpImage.clear();

#if defined(_USED_TEXTURE_TABLE)
	vpTableInfo.clear();
#endif
}

//------------------------------------------------------------------------------------
bool SPMOBModel::IsNull()
{
	return (eAniIndex == ANI_NULL)? true:false;	
}

//------------------------------------------------------------------------------------
bool SPMOBModel::ReloadTexture(TCHAR *strImageName)
{
	ReleaseImage();
	
	char szName[MAX_PATH] = {0};
	strcpy(szName, strImageName);

	char* pszLayerPos;
	int iLength;

	iLength = (int)strlen(szName);
	if( iLength <= 0 )
		return false;
	pszLayerPos = &szName[iLength - 5];

	int i;
	SPTexture* pkImage;
	for( i = 0; i < 4; i++ )
	{
		// '0' : 48
		*pszLayerPos = i + 48;

		//if( !g_pVideo->GetTextureMgr()->LoadMemoryTexture( szName, &pkImage ) )
		if( !g_pVideo->GetTextureMgr()->GetTexture( szName, &pkImage ,TEX_MON,LEVEL_LOW) )
			continue;

		vpImage.push_back(pkImage);
	}
	
	return true;
}

//------------------------------------------------------------------------------------
#if defined(_USED_TEXTURE_TABLE)
bool SPMOBModel::ReloadTexturebyTable(TCHAR *strImageName, TCHAR *strImagePath)
{	
	ReleaseImage();

	char szName[MAX_PATH] = {0}, szFullName[MAX_PATH] = {0};
	strcpy(szName, strImageName);

	char* pszLayerPos;
	int iLength;

	iLength = (int)strlen(szName);
	if( iLength <= 0 )
		return false;
	pszLayerPos = &szName[iLength - 1];
	
	int i;
	TEXTURE_TABLE_INFO* pTableInfo;
	SPTexture* pkImage;
	for( i = 0; i < 4; i++ )
	{
		// '0' : 48
		*pszLayerPos = i + 48;

		pTableInfo = g_pMOBCluster->GetTextureTable(szName);
		if( pTableInfo == NULL )
			continue;

		if( pTableInfo->strTextureName.empty() )
			continue;

		sprintf(szFullName, "%s%s", strImagePath, pTableInfo->strTextureName.c_str());
		//if( !g_pVideo->GetTextureMgr()->LoadMemoryTexture(szFullName, &pkImage) )
		if( !g_pVideo->GetTextureMgr()->GetTexture(szFullName, &pkImage,TEX_MON,LEVEL_LOW) )
			continue;

		vpTableInfo.push_back(pTableInfo);
		vpImage.push_back(pkImage);
	}
	return true;
}
#endif

//------------------------------------------------------------------------------------
#if defined(_CHARTOOL)
bool SPMOBModel::InsertTexture(const char* pszFileName)
{
	SPTexture* pkImage;
	if( !g_pVideo->GetTextureMgr()->GetTexture( pszFileName, &pkImage ,TEX_MON) )
		return false;

	vpImage.push_back(pkImage);
	return true;
}
#endif

//------------------------------------------------------------------------------------
void SPMOBModel::Render()
{
	SPTexture* pkTexture = NULL;

	if( FrameInfo.iLayer == 0 )
		return;

	int iImageSize = (int)vpImage.size();
	if( (FrameInfo.iLayer - 1) >= iImageSize )
		return;

	pkTexture = vpImage[FrameInfo.iLayer - 1];
	if( pkTexture == NULL )
		return;

	pkTexture->SetColor(D3DXCOLOR(FrameInfo.fColorR, FrameInfo.fColorG, FrameInfo.fColorB, FrameInfo.fColorA));
	pkTexture->SetScale(FrameInfo.fScaleX, FrameInfo.fScaleY);
	
#if defined(_USED_TEXTURE_TABLE)
	if( pFrameInfo )
	{
		pkTexture->RenderTexture(&rcDst, &rcSrc,
			(float)pFrameInfo->iOffsetX, (float)pFrameInfo->iOffsetY,
			pFrameInfo->fCenterOffsetX, pFrameInfo->fCenterOffsetY, D3DXToRadian(FrameInfo.iRotation));
	}
#else
	pkTexture->RenderTexture(&rcDst, &rcSrc, D3DXToRadian(FrameInfo.iRotation));
#endif
	
	pkTexture->SetScale();
	pkTexture->SetColor();
}


//------------------------------------------------------------------------------------
SPMOBLayerModel::SPMOBLayerModel()
: pkTexture(NULL)
{
	SetNull();
#if defined(_USED_TEXTURE_TABLE)
	vpTableInfo.clear();
	pFrameInfo = NULL;
#endif
}

//------------------------------------------------------------------------------------
SPMOBLayerModel::~SPMOBLayerModel()
{
	SetNull();
	ReleaseImage();
}

//------------------------------------------------------------------------------------
void SPMOBLayerModel::SetNull()
{
	eAniIndex = ANI_NULL;
	iLayerNo = -1;
	iCurAniFrame = 0;
	iMaxAniFrame = 0;
	fAccumulateTime = 0.0f;
	iDefaultRotation = 0;
	bLightMap = false;
	FrameInfo.SetNull();
}

//------------------------------------------------------------------------------------
void SPMOBLayerModel::ReleaseImage()
{
#if defined(_USED_TEXTURE_TABLE)
	vpTableInfo.clear();
#endif
}

//------------------------------------------------------------------------------------
bool SPMOBLayerModel::IsNull()
{
	return (eAniIndex == ANI_NULL)? true:false;	
}

//------------------------------------------------------------------------------------
void SPMOBLayerModel::Render()
{
	if( pkTexture == NULL || FrameInfo.bVisible == false )
		return;

	pkTexture->SetColor(D3DXCOLOR(FrameInfo.fColorR, FrameInfo.fColorG, FrameInfo.fColorB, FrameInfo.fColorA));
	pkTexture->SetScale(FrameInfo.fScaleX, FrameInfo.fScaleY);

#if defined(_USED_TEXTURE_TABLE)
	if( pFrameInfo )
	{
		pkTexture->RenderTexture(&rcDst, &rcSrc,
			(float)pFrameInfo->iOffsetX, (float)pFrameInfo->iOffsetY,
			pFrameInfo->fCenterOffsetX, pFrameInfo->fCenterOffsetY, D3DXToRadian(FrameInfo.iRotation));
	}
#else
	pkTexture->RenderTexture(&rcDst, &rcSrc, D3DXToRadian(FrameInfo.iRotation));
#endif

	pkTexture->SetScale();
	pkTexture->SetColor();
}