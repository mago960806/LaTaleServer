#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <map>

// Modify by kkandori (11.9. 2004)
#ifdef _LATALE_CLIENT
	#include "SPCommon.h"
	#include "SPGOBClusterDEF.h"
	#include "SPMonsterModelUnitDef.h"
	#include "SPGOBAnimation.h"
	//#include "SPGOBModel.h"
	#include "SPDebug.h"

#else 
	#include "SPToolCommon.h"
	#include "SPGameObjectDef.h"
	#include "SPGameMessageDEF.H"
	#include "SPRenderModelDef.h"	
	#include "SPGOBClusterDEF.h"
	#include "SPAvatarModelUnitDef.h"
	#include "SPMonsterModelUnitDef.h"
	#include "SPGameObjectToolCustom.h"
	#include "SPGOBAnimation.h"
	#include "SPGOBModel.h"
#endif
// Modify End

#include "SPGOBCluster.h"
#include "SPMOBCluster.h"

#include "SPRenderModelDEF.h"
#include "SPGameObjectDef.h"
#include "SPResourceDef.h"


#ifdef _LATALE_CLIENT
	#include "SPGameObject.h"
#endif

#include "SPMonsterModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPMonsterModelUnit.h"

#include "SPUtil.h"

using namespace std;

//------------------------------------------------------------------------------------
SPMonsterModelUnit::SPMonsterModelUnit(SPGameObject* pParent)
: SPGOBModelUnit(pParent, GOB_MODEL_MONSTER),
m_strImagePath(NULL),
m_pMOBModel(NULL),
m_eAniIndex(ANI_NULL),
m_eLastIndex(ANI_NULL),
m_iCurAniFrame(-1),
m_fRenderPosX(0),
m_fRenderPosY(0),
m_fAccumulateAction(0.0f),
m_fAccumulateRenderTime(0.0f),
m_uiNPCSeqClassID(0),
m_uiNewPetClassID(0),
m_bMonster(true),
m_bMustQuery(false),
m_bAnimationLoop(true)
{	
	m_pMOBModel = new SPMOBModel;
	m_strImagePath = new TCHAR[256];
	m_strImagePath[0] = '\0';
}

//------------------------------------------------------------------------------------
SPMonsterModelUnit::~SPMonsterModelUnit()
{
	SAFE_DELETE(m_pMOBModel);
	SAFE_DELETE_ARRAY(m_strImagePath);
}

//------------------------------------------------------------------------------------
void SPMonsterModelUnit::Init()
{	
	m_eAniIndex = m_eLastIndex = ANI_MONSTER_STAND;
	m_iCurAniFrame = 0;
	
	m_fAccumulateAction = 0.0f;
	m_fAccumulateRenderTime	= 0.0f;

#if defined(_LATALE_CLIENT)
	
	unsigned int uiClassID = m_uiNPCSeqClassID;
	if (uiClassID == 0)
		uiClassID = m_uiNewPetClassID;

	if( uiClassID == 0 )
		uiClassID = m_pParent->GetClassID();

	SPMOBAnimation* pkAnimation = NULL;
	for( int i = ANI_MONSTER_BEGIN; i <= ANI_NPC_END; i++ )
	{
		if( pkAnimation = g_pMOBCluster->GetAnimation(uiClassID, (ANIMATION_INDEX)i) )
			break;
	}

	if( pkAnimation )
	{
		string strFullName;
		//strFullName = RES_WORK_RESOURCE_PATH;
		if( pkAnimation->eMobType == MT_EVENT )
			strFullName = RES_PATH_NPC;
		else
			strFullName = RES_PATH_MONSTER;

	#if defined(_USED_TEXTURE_TABLE)
		m_pMOBModel->ReloadTexturebyTable((TCHAR*)pkAnimation->strImageName.c_str(), (TCHAR*)strFullName.c_str());
	#else
		strFullName += pkAnimation->strImageName + ".PNG";
		m_pMOBModel->ReloadTexture((TCHAR*)strFullName.c_str());
	#endif
		
	}

#endif
}

//------------------------------------------------------------------------------------
void SPMonsterModelUnit::SetNPC(unsigned int uiClassID)
{
	m_bMonster = false;
	m_uiNPCSeqClassID = uiClassID;
}

//--------------------------------------------------
void SPMonsterModelUnit::SetNewPet(unsigned int uiClassID)
{
	m_uiNewPetClassID = uiClassID;
	Init();
}

//------------------------------------------------------------------------------------
void SPMonsterModelUnit::FrameInit()
{
	m_iCurAniFrame = 0; 
	m_bMustQuery = true;
}

//------------------------------------------------------------------------------------
void SPMonsterModelUnit::Process(float fTime)
{
	ProcessImage(m_fRenderPosX, m_fRenderPosY, fTime);
}

//------------------------------------------------------------------------------------
void SPMonsterModelUnit::ProcessImage(const float fPosX, const float fPosY, float fTime)
{
	m_fAccumulateAction += fTime;

	// PosSet
	FACING eDir = FACING_LEFT;
	unsigned int uiClassID;

#if defined(_CHARTOOL)
	eDir = GetParent()->GetDirection();
	if( fTime == 0 )
		m_fAccumulateAction = 0.0f;
	uiClassID = GetParent()->GetClassID();
#else
	if( GetParent() )
		eDir = (FACING)GetParent()->SPGOBPerformMessage(SPGM_GETFACING);

	uiClassID = m_uiNPCSeqClassID;
	if( uiClassID == 0 )
		uiClassID == m_uiNewPetClassID;

	if (uiClassID == 0)
		uiClassID = m_pParent->GetClassID();
#endif

	if( m_bMustQuery || (m_pMOBModel && m_fAccumulateAction > m_pMOBModel->FrameInfo.fDelay) )
	{
		//m_fAccumulateAction = 0.0f;
		m_fAccumulateAction -= m_pMOBModel->FrameInfo.fDelay;	// FX쪽과의 동기화를 맞추기 위해 AJJIYA [5/26/2006]
		m_eLastIndex = m_eAniIndex;
		m_bMustQuery = false;

		g_pMOBCluster->QueryAnimationSequence(&m_pMOBModel,
			uiClassID, m_eAniIndex, m_iCurAniFrame, m_bAnimationLoop);
	}

#if defined(_USED_TEXTURE_TABLE)
	SetFramePosbyTable(m_pMOBModel, (int)fPosX, (int)fPosY, eDir);
#else
	SetFramePos(m_pMOBModel, (int)fPosX, (int)fPosY, eDir);
#endif
	
}

//------------------------------------------------------------------------------------
void SPMonsterModelUnit::SetFramePos(SPMOBModel* pMOBModel, int iPosX, int iPosY, FACING eFacing)
{
	if( pMOBModel == NULL )
		return;

	if( pMOBModel->eAniIndex == ANI_NULL )
		return;

	int iResourceIndex, iIndexX, iIndexY;
	RECT rcDst, rcSrc;

	iResourceIndex = pMOBModel->FrameInfo.iResourceIndex - 1;
	if( iResourceIndex < 0 )
		return;

	iIndexX = iResourceIndex % pMOBModel->iFrameCountX;
	iIndexY = iResourceIndex / pMOBModel->iFrameCountX;

	rcSrc.left = iIndexX * pMOBModel->iFrameWidth;
	rcSrc.top = iIndexY * pMOBModel->iFrameHeight;
	rcSrc.right = (iIndexX + 1) * pMOBModel->iFrameWidth;
	rcSrc.bottom = (iIndexY + 1) * pMOBModel->iFrameHeight;

	if( eFacing == FACING_LEFT )
	{
		rcDst.left = iPosX - pMOBModel->FrameInfo.iPosX;
		rcDst.right = rcDst.left + pMOBModel->iFrameWidth;

		pMOBModel->FrameInfo.iRotation = pMOBModel->iDefaultRotation;
	}
	else
	{
		rcDst.left = iPosX + pMOBModel->FrameInfo.iPosX;
		rcDst.right = rcDst.left - pMOBModel->iFrameWidth;

		pMOBModel->FrameInfo.iRotation = -pMOBModel->iDefaultRotation;
	}

	rcDst.top = iPosY - pMOBModel->FrameInfo.iPosY;
	rcDst.bottom = rcDst.top + pMOBModel->iFrameHeight;

	pMOBModel->rcSrc = rcSrc;
	pMOBModel->rcDst = rcDst;
}

//------------------------------------------------------------------------------------
#if defined(_USED_TEXTURE_TABLE)
void SPMonsterModelUnit::SetFramePosbyTable(SPMOBModel* pMOBModel, int iPosX, int iPosY, FACING eFacing)
{
	if( pMOBModel == NULL )
		return;

	if( pMOBModel->FrameInfo.iLayer == 0 )
		return;

	RECT rcDst, rcSrc;
	TEXTURE_TABLE_INFO* pTableInfo;
	FRAME_INFO* pFrameInfo;
	int iResourceIndex, iSize;

	iSize = pMOBModel->vpTableInfo.size();
	if( (pMOBModel->FrameInfo.iLayer - 1) >= iSize )
		return;

	pTableInfo = pMOBModel->vpTableInfo[pMOBModel->FrameInfo.iLayer - 1];
	if( pTableInfo == NULL )
		return;

	iResourceIndex = pMOBModel->FrameInfo.iResourceIndex;
	pFrameInfo = GetResIndexRectbyTable(pTableInfo, iResourceIndex, rcSrc);
	
	pMOBModel->pFrameInfo = pFrameInfo;
	if( pFrameInfo == NULL )
		return;

	int iSrcWidth, iSrcHeight;
	iSrcWidth = pFrameInfo->rcResourceRgn.right - pFrameInfo->rcResourceRgn.left;
	iSrcHeight = pFrameInfo->rcResourceRgn.bottom - pFrameInfo->rcResourceRgn.top;

	if( eFacing == FACING_LEFT )
	{
		rcDst.left = iPosX - pMOBModel->FrameInfo.iPosX + pFrameInfo->iOffsetX;
		rcDst.right = rcDst.left + iSrcWidth;

		pMOBModel->FrameInfo.iRotation = pMOBModel->iDefaultRotation;
	}
	else
	{
		rcDst.left = iPosX + pMOBModel->FrameInfo.iPosX - pFrameInfo->iOffsetX;
		rcDst.right = rcDst.left - iSrcWidth;

		pMOBModel->FrameInfo.iRotation = -pMOBModel->iDefaultRotation;
	}

	rcDst.top = iPosY - pMOBModel->FrameInfo.iPosY + pFrameInfo->iOffsetY;
	rcDst.bottom = rcDst.top + iSrcHeight;

	pMOBModel->rcSrc = rcSrc;
	pMOBModel->rcDst = rcDst;
}
#endif

//------------------------------------------------------------------------------------
void SPMonsterModelUnit::Render(float fTime)
{
	m_pMOBModel->Render();
	g_pVideo->Flush();
}

//------------------------------------------------------------------------------------
void SPMonsterModelUnit::SetAccumulateAction(float fAccumulateAction)
{
	m_fAccumulateAction = fAccumulateAction;
}

//------------------------------------------------------------------------------------
void SPMonsterModelUnit::SetAnimationIndex(ANIMATION_INDEX AniInx, int iForce) 
{ 
	m_eAniIndex = AniInx;
	if( m_eAniIndex == m_eLastIndex && iForce == 0 )
		return;
	
	m_fAccumulateAction = 0.0f;
	m_fAccumulateRenderTime = 0.0f;

	m_pMOBModel->FrameInfo.fDelay = 0.0f;
	m_eLastIndex = m_eAniIndex;
	FrameInit();
}

//------------------------------------------------------------------------------------
float SPMonsterModelUnit::GetAniAccumulateTime()
{
#if defined(_LATALE_CLIENT)
	unsigned int uiClassID = m_uiNPCSeqClassID;
	if( uiClassID == 0 )
		uiClassID = m_uiNewPetClassID;

	if (uiClassID == 0)
		uiClassID = m_pParent->GetClassID();
	return g_pMOBCluster->GetAnimationTime(uiClassID, m_eAniIndex);
#endif
	return 0.0f;
}

//------------------------------------------------------------------------------------
RECT SPMonsterModelUnit::GetRenderRgn()
{
	RECT rcRet;
	if( m_pMOBModel )
	{
		rcRet = m_pMOBModel->rcDst;

		int iHeight = m_pMOBModel->rcDst.bottom - m_pMOBModel->rcDst.top;
		rcRet.bottom = m_pMOBModel->rcDst.top + (int)((float)iHeight * m_pMOBModel->FrameInfo.fScaleY);
	}

	return rcRet;
}

//------------------------------------------------------------------------------------
float SPMonsterModelUnit::GetRenderPosX()
{
	return m_fRenderPosX;
}

//------------------------------------------------------------------------------------
float SPMonsterModelUnit::GetRenderPosY()
{
	return m_fRenderPosY;
}

//------------------------------------------------------------------------------------
#if defined(_CHARTOOL)
SPMOBModel* SPMonsterModelUnit::GetMOBModel()
{
	return m_pMOBModel;
}

void SPMonsterModelUnit::SetCurAniFrame(int iFrame)
{
	m_iCurAniFrame = iFrame;
	g_pMOBCluster->QueryAnimationSequence(&m_pMOBModel,
			GetParent()->GetClassID(), m_eAniIndex, iFrame, m_bAnimationLoop);
}
#endif

//------------------------------------------------------------------------------------
void SPMonsterModelUnit::SetAnimationLoop(bool bLoop)
{
	m_bAnimationLoop = bLoop;
}

//------------------------------------------------------------------------------------
LRESULT SPMonsterModelUnit::SPSendMessage(MODEL_VALUE_TYPE mvMsg, WPARAM wParam, LPARAM lParam)
{
	switch( mvMsg )
	{
	case MV_FRAMEINIT:				
		FrameInit();						
		return 0;

	case MV_SET_ACCUMUL_ACTION:	
		float fAccTime;
		DataConvert<WPARAM,float>(wParam, fAccTime);		// Data Convert!!!
		SetAccumulateAction	(fAccTime);
		return 0;

	case MV_SET_ANIMATION_INDEX:
		SetAnimationIndex((ANIMATION_INDEX)wParam, (int)lParam);
		return 0;

	case MV_SET_ANI_LOOP:
		SetAnimationLoop((bool)wParam);
		return 0;

	case MV_GET_ANIMATION_INDEX:
		return (LRESULT)m_eAniIndex;

	case MV_GET_CURR_FRAME:
		return (LRESULT)m_iCurAniFrame;

	case MV_GET_ANI_ACCUMUL_TIME:
		{
			float fTime = GetAniAccumulateTime();
			*((float*)wParam) = fTime;
		}
		return 0;

	case MV_SET_RENDERPOSX:
		{
			DataConvert<WPARAM,float>(wParam, m_fRenderPosX);
		}
		return 0;

	case MV_SET_RENDERPOSY:
		{
			DataConvert<WPARAM,float>(wParam, m_fRenderPosY);
		}
		return 0;

	case MV_GET_RENDER_RGN:
		{
			*((RECT*)wParam) = GetRenderRgn();
		}
		return 0;
	}

	return SPGOBModelUnit::SPSendMessage(mvMsg, wParam, lParam);
}

