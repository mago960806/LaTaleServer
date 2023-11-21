#include "SPPhantom.h"
#include "SPCommon.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "PacketID.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"
#include "SPPlayer.h"
#include "SPGOBModelUnit.h"
#include "SPGOBCoordPhysics.h"
#include "SPAvatarModelUnit.h"
#include "SPCommandConvert.h"
#include "SPUtil.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include <vector>
using namespace std;

#include "SPDebug.h"

//--------------------------------------------------
#define MOVED_TOO_LARGE	(30)
//--------------------------------------------------
PhantomCfg	*g_pPhantomCfg = NULL;
bool initPhantomCfg( void )
{
	return true;
}
//--------------------------------------------------
// PhantomLDTCfg
//--------------------------------------------------
PhantomLDTCfg::PhantomLDTCfg( void )
{
	m_vecLdtCfg.clear();
	init();
}
PhantomLDTCfg::~PhantomLDTCfg( void )
{
	resetCfg();
}
//--------------------------------------------------
bool PhantomLDTCfg::LoadLdt( void )
{
	SPLDTFile	*pFile = NULL;
	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/PHANTOM.LDT", &pFile );
	if (!pFile)
	{
		assert( false && "phantom ldt load failed." );
		return false;
	}

	resetCfg();
	LDT_Field field;
	phantomLdt data;

	int iRecordCount = pFile->GetItemCount();
	int itemID = 0;
	for (int idx = 0; idx < iRecordCount; ++idx)
	{
		itemID = pFile->GetPrimaryKey( idx );

		pFile->GetFieldFromLable( itemID, "subtype", field );
		data.iSubtype = field.uData.lValue;

		pFile->GetFieldFromLable( itemID, "name", field );
		if (field.uData.pValue)
			data.name = field.uData.pValue;
		
		pFile->GetFieldFromLable( itemID, "phantomnumber", field );
		data.phantomNumber = field.uData.lValue;

		pFile->GetFieldFromLable( itemID, "delaytime", field );
		data.fDelayTime = field.uData.fValue;

		pFile->GetFieldFromLable( itemID, "startalpha", field );
		data.fStartAlpha = field.uData.fValue;
		if (data.fStartAlpha > 1.0f)
			data.fStartAlpha = 1.0f;
		if (data.fStartAlpha < 0.0f)
			data.fStartAlpha = 0.0f;

		pFile->GetFieldFromLable( itemID, "multicolor", field );
		data.iMultiColor = field.uData.lValue;

		for (int idx = 1; idx <= 7; ++idx) 
		{
			char buf_r[128] = {0};
			char buf_g[128] = {0};
			char buf_b[128] = {0};
			_snprintf( buf_r, 127, "color_r%d", idx );
			_snprintf( buf_g, 127, "color_g%d", idx );
			_snprintf( buf_b, 127, "color_b%d", idx );
			int r, g, b;
			pFile->GetFieldFromLable( itemID, buf_r, field );
			r = field.uData.lValue;
			pFile->GetFieldFromLable( itemID, buf_g, field );
			g = field.uData.lValue;
			pFile->GetFieldFromLable( itemID, buf_b, field );
			b = field.uData.lValue;
			D3DXCOLOR color = D3DCOLOR_ARGB( 255, r, g, b );
			data.vecColor.push_back( color );
		}

		m_vecLdtCfg.push_back( data );
		data.reset();
	}

	SAFE_RELEASE( pFile );
	return true;
}
bool PhantomLDTCfg::ReloadLdt( void )
{
	resetCfg();
	return LoadLdt();
}
//--------------------------------------------------
bool PhantomLDTCfg::GetCfgFromSubtype(int iSubtype, PhantomCfg &cfg)
{
	for (int idx = 0; idx < (int)m_vecLdtCfg.size(); ++idx)
	{
		if (m_vecLdtCfg[idx].iSubtype == iSubtype)
		{
			phantomLdt &ldt_cfg = m_vecLdtCfg[idx];
			cfg.m_fDelayTime = ldt_cfg.fDelayTime;
			cfg.m_fFirstAlpha = ldt_cfg.fStartAlpha;
			cfg.m_iMaxPhantomNbr = ldt_cfg.phantomNumber;
			if (ldt_cfg.vecColor.empty())
				cfg.m_color = D3DCOLOR_ARGB( 255, 255, 255, 255 );
			else
			{
				cfg.m_color = ldt_cfg.vecColor[0];
				cfg.m_iMultiColor = ldt_cfg.iMultiColor;
				cfg.m_vecColors = ldt_cfg.vecColor;
			}

			return true;
		}
	}

	return false; 
}
//--------------------------------------------------
bool PhantomLDTCfg::resetCfg( void )
{
	m_vecLdtCfg.clear();
	return true;
}
bool PhantomLDTCfg::init( void )
{
	resetCfg();
	return true;
}
//--------------------------------------------------
//--------------------------------------------------
// PhantomEffect
//--------------------------------------------------
PhantomEffect::PhantomEffect( void )
{
	m_pPlayer = NULL;
	m_itemID = 0;
	m_iSubtype = 0;
#ifdef _PHANTOM_EDITOR_ENABLE
	if (m_pPlayer && m_pPlayer->IsLocalPlayer())
	{
		g_pPhantomCfg = &m_cfg;
	}
#endif
}
PhantomEffect::PhantomEffect(SPPlayer *pParent, int itemID)
{
	m_pPlayer = pParent;
	m_itemID = itemID;
	m_iSubtype = 0;
#ifdef _PHANTOM_EDITOR_ENABLE
	if (m_pPlayer && m_pPlayer->IsLocalPlayer())
	{
		g_pPhantomCfg = &m_cfg;
	}
#endif
}
PhantomEffect::~PhantomEffect( void )
{
#ifdef _PHANTOM_EDITOR_ENABLE
	if (m_pPlayer && m_pPlayer->IsLocalPlayer())
	{
		g_pPhantomCfg = NULL;
	}
#endif
	m_pPlayer = NULL;
	m_itemID = 0;
	m_iSubtype = 0;
}
//--------------------------------------------------
#include "SPXTrace.h"
void PhantomEffect::Process(float fDeltaTime)
{
	if (!m_pPlayer || m_pPlayer->m_bTransformMode || m_pPlayer->m_bTumbleCloudMode)
		return;

	SPPlayer *pPlayer = m_pPlayer;
	if (!pPlayer || LOBBY_INFO == pPlayer->m_eLobbyAvatar)
		return;

	PhantomAnim			*pAnim = pPlayer->m_ptrPhantomAnim;
	SPAvatarModelUnit	*pAvatar = (SPAvatarModelUnit*)pPlayer->m_pGOBModelUnit;
	if (!pAnim || !pAvatar)
		return;

	//--------------------------------------------------
	// 记录当前点和当前的变化
	POINT ptMoved = {0,0};
	POINT ptCurrent = {0,0};
	PhantomData data;
	ptCurrent.x = pPlayer->m_pkCoordPhysics->GetPosX();
	ptCurrent.y = pPlayer->m_pkCoordPhysics->GetPosY();
	ptMoved.x = ptCurrent.x - pAnim->m_ptLastPos.x;
	ptMoved.y = ptCurrent.y - pAnim->m_ptLastPos.y;
	pAnim->m_ptLastPos = ptCurrent;
	data.fDeltaTime = fDeltaTime; 

	if (pAnim->m_fPhantomAccuTime < 0.01f
		&&
		(0 == ptMoved.x && 0 == ptMoved.y))
	{
		return;
	}

	if (pPlayer->m_uiCurAction & ACTION_MOVE_UP
		||
		pPlayer->m_uiCurAction & ACTION_MOVE_DOWN)
	{
		data.pos.x = -1;
		data.pos.y = -1;
	}
	else if ((0 == ptMoved.x && 0 == ptMoved.y)
		&&	
		pPlayer->m_uiCurAction == ACTION_STOP)
	{
		data.pos.x = -1;
		data.pos.y = -1;
	}
	else if (abs(ptMoved.x) > MOVED_TOO_LARGE)
	{
		data.pos.x = -1;
		data.pos.y = -1;
		data.fDeltaTime = 0.0f;
		pPlayer->m_ptrPhantomAnim->m_fPhantomAccuTime = 0.0f;
		pPlayer->m_ptrPhantomAnim->m_fAccuTime = 0.0f;
		pPlayer->m_ptrPhantomAnim->m_deqPhantomData.clear();
	}
	else
	{
		data.pos = ptCurrent;
	}

	pAnim->m_deqPhantomData.push_front( data ); 
	pAnim->m_fAccuTime += data.fDeltaTime;
	if (data.pos.x != -1 && data.pos.y != -1)
		pAnim->m_fPhantomAccuTime += data.fDeltaTime;
	while (pAnim->m_fAccuTime > m_cfg.m_fDelayTime*m_cfg.m_iMaxPhantomNbr)
	{
		pAnim->m_fAccuTime -= pAnim->m_deqPhantomData.back().fDeltaTime;
		if (pAnim->m_deqPhantomData.back().pos.x != -1
			&&
			pAnim->m_deqPhantomData.back().pos.y != -1)
			pAnim->m_fPhantomAccuTime -= pAnim->m_deqPhantomData.back().fDeltaTime;
		pAnim->m_deqPhantomData.pop_back();
	}

	//--------------------------------------------------
	for (int idx = 0; idx < m_cfg.m_iMaxPhantomNbr; ++idx)
	{
		PhantomAnim::replay_data &data = pAnim->m_vecPhantomIdx[idx];
		if (pAnim->m_deqPhantomData.empty())
			data.frameIdx = -1;
		else
			data.frameIdx = (idx+1)*(int)((pAnim->m_deqPhantomData.size())/m_cfg.m_iMaxPhantomNbr) - 1;
		data.frameAlpha = m_cfg.m_fFirstAlpha - (float)(idx) / m_cfg.m_iMaxPhantomNbr * m_cfg.m_fFirstAlpha;
		data.frameAlpha = data.frameAlpha < 0.2 ? 0.2 : data.frameAlpha;
	}
}
void PhantomEffect::Render( float fDeltaTime )
{
	if (!m_pPlayer || m_pPlayer->m_bTransformMode || m_pPlayer->m_bTumbleCloudMode)
		return;

	SPPlayer *pPlayer = m_pPlayer; 
	if (!pPlayer || LOBBY_INFO == pPlayer->m_eLobbyAvatar)
		return;

	PhantomAnim			*pAnim = pPlayer->m_ptrPhantomAnim;
	SPAvatarModelUnit	*pAvatar = (SPAvatarModelUnit*)pPlayer->m_pGOBModelUnit;;
	if (!pAnim || !pAvatar)
		return;

	if (pAnim->m_fPhantomAccuTime < 0.01f)
		return;

	LONG lastPosX = pPlayer->m_pkCoordPhysics->GetPosX();
	LONG lastPosY = pPlayer->m_pkCoordPhysics->GetPosY();
	int OffsetX = 0;
	int OffsetY = 0;
	int AccuChangeX = 0;	// 总的变换量
	int AccuChangeY = 0;	

	POINT oldPos;
	oldPos.x = pAvatar->GetRenderPosX();
	oldPos.y = pAvatar->GetRenderPosY();
	float oldTrans = pAvatar->GetTransparencyValue();
	float	fParsingX, fParsingY;
	WPARAM	wParsingX, wParsingY; 

	D3DXCOLOR old_color = pAvatar->getPhantomColor();
	D3DXCOLOR color = m_cfg.m_color;

	if (!pPlayer->m_bClipping && pPlayer->m_ptrPhantomAnim
		&& !pPlayer->m_ptrPhantomAnim->m_vecPhantomIdx.empty())
	{
		if (pPlayer->m_vFigureItemList.empty()
			&&
			pPlayer->m_pGOBModelUnit)
		{
			int phantom_num = (int)(pPlayer->m_ptrPhantomAnim->m_fPhantomAccuTime / m_cfg.m_fDelayTime + 0.9999f);
			if (phantom_num > m_cfg.m_iMaxPhantomNbr)
				phantom_num = m_cfg.m_iMaxPhantomNbr;

			if (phantom_num > 0)
			{
				for (int idx = phantom_num - 1; idx >= 0 ; --idx)
				{
					PhantomAnim::replay_data &replay = pPlayer->m_ptrPhantomAnim->m_vecPhantomIdx[idx]; 
					if (replay.frameIdx < 0)
						continue;
					PhantomData &data =
						pPlayer->m_ptrPhantomAnim->m_deqPhantomData[replay.frameIdx];
					if (data.pos.x == -1 || data.pos.y == -1)
						continue;
					if (data.pos.x == lastPosX && data.pos.y == lastPosY)
						continue;

					if (m_cfg.m_iMultiColor != 0)
					{
						if (idx < (int)m_cfg.m_vecColors.size())
							color = m_cfg.m_vecColors.at( idx );
					}

					OffsetX = (int)(data.pos.x - lastPosX);
					OffsetY = (int)(data.pos.y - lastPosY);
					pAvatar->setPhantomColor( D3DXCOLOR( color.r, color.g, color.b, replay.frameAlpha ) );
					pAvatar->ReCalcLayerPos( OffsetX, OffsetY );
					pAvatar->Render( 0.0f );
					lastPosX = data.pos.x;
					lastPosY = data.pos.y;
					AccuChangeX += OffsetX;
					AccuChangeY += OffsetY;
				}
			}
		}
	}

	pAvatar->SetTransparencyValue( oldTrans );
	pAvatar->setPhantomColor( old_color );
	pAvatar->ReCalcLayerPos( -AccuChangeX, -AccuChangeY );
}
//--------------------------------------------------
void PhantomEffect::SetSubtype(int iSubtype)
{
	PhantomCfg cfg;
	if (PhantomLDTCfg::getInstance().GetCfgFromSubtype( iSubtype, cfg ))
	{
		m_iSubtype = iSubtype;
		m_cfg = cfg;
	}
}
int PhantomEffect::GetEffectItemID( void ) const
{
	return m_itemID;
}

