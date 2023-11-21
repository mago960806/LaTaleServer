#include <vector>

#ifdef _LATALE_CLIENT
#include "SPCommon.h"
#else 
#include "SPToolCommon.h"
#endif

#include "SPUtil.h"
#include "SPResourceDef.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"

#include "SPGOBAnimation.h"
#include "SPGOBModel.h"

#include "SPGOBCluster.h"

// MODEL
//-SPGOBInfoSequencePart-----------------------------------------------------------------------------------
SPGOBInfoSequencePart::SPGOBInfoSequencePart() 
: PartLayer(PL_NULL)	
, iResourceIndex(0)
, iRotationDegree(0)
, iRX(0)
, iRY(0)
, bVisable(true)
, bFlip(false)
#ifdef _CHARTOOL
, bSelect(false)
#endif

{}

void SPGOBInfoSequencePart::SetNull()
{	
	PartLayer		=	PL_NULL;
}

bool SPGOBInfoSequencePart::operator==(const SPGOBInfoSequencePart CompObj)
{
	// 파트정보만 비교
	if(PartLayer	!=	CompObj.PartLayer)		return false;
	return true;
}

SPGOBInfoSequencePart& SPGOBInfoSequencePart::operator=(const SPGOBInfoSequencePart &src)
{
	PartLayer		=src.PartLayer;	
	iResourceIndex	=src.iResourceIndex;
	iRotationDegree	=src.iRotationDegree;
	iRX				=src.iRX;
	iRY				=src.iRY;
	bVisable		= src.bVisable;
	bFlip			= src.bFlip;

#ifdef _CHARTOOL	
	bSelect			= src.bSelect;
#endif

	return *this;
}

//-SPGOBModelPart-----------------------------------------------------------------------------------
SPGOBModelPart::SPGOBModelPart()
: PartLayer		(PL_NULL)
, EquipClass	(EQCLASS_NULL)
, iItemID		(0)
, iColor		(0)
, iRotationDegree(0)
, iRenderType(0)
, pPartImage	(0)
, pPartImageReserve(NULL)
#if defined(_USED_TEXTURE_TABLE)
, pTableInfo(0)
, pTableInfoReserve(0)
, pFrameInfo(0)
#endif
{}

SPGOBModelPart::~SPGOBModelPart()
{
	SAFE_RELEASE(pPartImage);
	SAFE_RELEASE(pPartImageReserve);
}

bool SPGOBModelPart::isNull()
{
	//if( PartLayer	== PL_NULL	)	return true;
	if( EquipClass	== EQCLASS_NULL)	return true;
	if( iItemID		== 0		)	return true;
	if( iColor		== 0		)	return true;
	return false;
}

void SPGOBModelPart::SetNull()
{	
	EquipClass	= EQCLASS_NULL;
	iItemID		= 0			;
	iColor		= 0			;	
}

bool SPGOBModelPart::ReloadTexture(TCHAR	*strImagePath)
{
	SAFE_RELEASE(pPartImage);
	SAFE_RELEASE(pPartImageReserve);
	
	if( g_pGOBCluster == NULL || isNull() )
		return false;

	char szName[256] = { 0,	};

#if defined(_USED_TEXTURE_TABLE)
	sprintf(szName, "%02d_%04d_%03d_%02d", PartLayer, EquipClass, iItemID, iColor);
	pTableInfo = g_pGOBCluster->GetTextureTable(szName);
	if( pTableInfo == NULL )
		return false;

	if( pTableInfo->strTextureName.empty() )
		return false;

	bool bRet = false;
	if(::lstrlen(strImagePath)==0) {
		sprintf(szName, "%s%s", RES_PATH_CHAR, pTableInfo->strTextureName.c_str());
	} else {
		sprintf(szName, "%s%s", strImagePath, pTableInfo->strTextureName.c_str());	
	}

#if defined(_LATALE_CLIENT)
	//bRet = g_pVideo->GetTextureMgr()->LoadMemoryTexture(szName, &pPartImage);
	bRet = g_pVideo->GetTextureMgr()->GetTexture(szName,&pPartImage,TEX_CHAR);
#else
	bRet = g_pVideo->GetTextureMgr()->LoadTexture(szName, &pPartImage);
#endif

	if( PartLayer == PL_FACE )
	{
		sprintf(szName, "%02d_%04d_001_01", PartLayer, EquipClass, iItemID, iColor);
		pTableInfoReserve = g_pGOBCluster->GetTextureTable(szName);
		if( pTableInfoReserve == NULL )
			return false;

		if(::lstrlen(strImagePath)==0) {
			sprintf(szName, "%s%s", RES_PATH_CHAR, pTableInfoReserve->strTextureName.c_str());
		} else {
			sprintf(szName, "%s%s", strImagePath, pTableInfoReserve->strTextureName.c_str());	
		}

#if defined(_LATALE_CLIENT)
		//bRet = g_pVideo->GetTextureMgr()->LoadMemoryTexture(szName, &pPartImageReserve);
		bRet = g_pVideo->GetTextureMgr()->GetTexture(szName, &pPartImageReserve,TEX_CHAR);
#else
		bRet = g_pVideo->GetTextureMgr()->LoadTexture(szName, &pPartImageReserve);
#endif
	}
	
	return bRet;

#else
	if(::lstrlen(strImagePath)==0) {
		//sprintf(szName, "%s%s%02d_%04d_%03d_%02d.PNG", RES_WORK_RESOURCE_PATH, RES_PATH_CHAR, PartLayer, EquipClass, iItemID, iColor);
		sprintf(szName, "%s%02d_%04d_%03d_%02d.PNG", RES_PATH_CHAR, PartLayer, EquipClass, iItemID, iColor);
	} else {
		sprintf(szName, "%s\\%02d_%04d_%03d_%02d.PNG", strImagePath, PartLayer, EquipClass, iItemID, iColor);	
	}

#if defined(_LATALE_CLIENT)
	bRet = g_pVideo->GetTextureMgr()->GetTexture(szName, &pPartImageReserve,TEX_CHAR);
#else
	return g_pVideo->GetTextureMgr()->LoadTexture( szName, &pPartImage );
#endif

#endif
}

void SPGOBModelPart::Render()
{	
	if(pPartImage) {
		//// 확인할려구
		//if(PartLayer == PL_WEAPON_IN) {
		//	g_pVideo->GetSysLine()->DrawRect(TarRect);			
		//} else
		//g_pVideo->GetSysLine()->DrawRect(TarRect);
		//
#if defined(_USED_TEXTURE_TABLE)
		if( pFrameInfo )
		{
			if( iRenderType == 0 )
			{
				pPartImage->RenderTexture(&TarRect, &SrcRect,
					(float)pFrameInfo->iOffsetX, (float)pFrameInfo->iOffsetY,
					pFrameInfo->fCenterOffsetX, pFrameInfo->fCenterOffsetY, D3DXToRadian(iRotationDegree));
			}
			else if( pPartImageReserve )
			{
				pPartImageReserve->RenderTexture(&TarRect, &SrcRect,
					(float)pFrameInfo->iOffsetX, (float)pFrameInfo->iOffsetY,
					pFrameInfo->fCenterOffsetX, pFrameInfo->fCenterOffsetY, D3DXToRadian(iRotationDegree));
			}
		}
#else
		pPartImage->RenderTexture(&TarRect, &SrcRect, D3DXToRadian(iRotationDegree));		
#endif
	}
}

SPGOBModelPart& SPGOBModelPart::operator=(const SPGOBModelPart &src)
{
	PartLayer	= src.PartLayer;
	EquipClass	= src.EquipClass;
	iItemID		= src.iItemID;
	iColor		= src.iColor;

	iRotationDegree = src.iRotationDegree;
	SrcRect.bottom	= src.SrcRect.bottom;
	SrcRect.left	= src.SrcRect.left;
	SrcRect.right	= src.SrcRect.right;
	SrcRect.top		= src.SrcRect.top;

	TarRect.bottom	= src.TarRect.bottom;
	TarRect.left	= src.TarRect.left;
	TarRect.right	= src.TarRect.right;
	TarRect.top		= src.TarRect.top;

	pPartImage	= src.pPartImage;	

	return *this;
}


//-SPGOBModel----------------------------------------------------------------------------
SPGOBModel::SPGOBModel()
: bPCEnable(false)
{ 
	SetNull(); 
}

void SPGOBModel::SetNull()
{	
	fDelayTime		= 0.f;	

	uiAniIndex		= 0;
	iAniSequence	= 0;
	iMaxSequence	= 0;
	//bPCEnable		= false;

	vGOBInfoSequencePart.clear();  //m_aGOBInfoSequencePart에서 delete
}

void SPGOBModel::Destroy()
{	
}
SPGOBModel& SPGOBModel::operator=(const SPGOBModel &src)
{
	vGOBInfoSequencePart	= src.vGOBInfoSequencePart;
	fDelayTime				= src.fDelayTime;

	uiAniIndex	= src.uiAniIndex;
	iAniSequence= src.iAniSequence;
	iMaxSequence= src.iMaxSequence;
	PartModel[PL_LAST] = src.PartModel[PL_LAST];
	
	return *this;
}