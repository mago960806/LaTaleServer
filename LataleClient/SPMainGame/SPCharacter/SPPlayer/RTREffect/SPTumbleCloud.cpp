#include "SPTumbleCloud.h"
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

#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"
#include "SPFXManager.h"
#include "SPFXDamageUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "./SPDebug.h"

#include <vector>
using namespace std;
#include "SPDebug.h"
//--------------------------------------------------
//--------------------------------------------------
TumbleCloudEffect::TumbleCloudEffect( void )
: m_pPlayer( NULL )
, m_itemID( 0 )
, m_iSubtype( 0 )
{
	m_cfg.reset();
}
TumbleCloudEffect::TumbleCloudEffect( SPPlayer *pPlayer, int itemID )
: m_pPlayer( pPlayer )
, m_itemID( itemID )
, m_iSubtype( 0 )
, m_lastAction( 0UL )
, m_restoreSeq( 0 )
{
	m_cfg.reset();
}
TumbleCloudEffect::~TumbleCloudEffect( void )
{
	if (m_pPlayer)
	{
		SPFXArchive* pLocalFXArchive	=	NULL;
		m_pPlayer->SPGOBPerformMessage( SPGM_GETFXARCHIVE , (LPARAM)&pLocalFXArchive );

		if (pLocalFXArchive)
		{
			int lastFxID = GetFxIDByAction( m_lastAction );
			if (lastFxID > 0)
				pLocalFXArchive->DeleteFXGroup( (UINT)lastFxID );

			if (m_pPlayer->GetModelUnit())
			{
				if (m_pPlayer->GetModelUnit()->SPSendMessage(MV_GET_ANIMATION_INDEX) != m_restoreSeq)
					m_pPlayer->GetModelUnit()->SPSendMessage(MV_SET_ANIMATION_INDEX, m_restoreSeq);
			}
		}

		m_pPlayer->SetAvatarOffset( 0, 0 ); 
		m_pPlayer->SetWeaponVisible( true );
		m_pPlayer->m_bTumbleCloudMode = false;
	}
	m_pPlayer = NULL;
	m_itemID = 0;
	m_iSubtype = 0;
	m_lastAction = 0UL;
}
//--------------------------------------------------
void TumbleCloudEffect::PreProcess( float fDeltaTime )
{
}
void TumbleCloudEffect::Process( float fDeltaTime )
{
	if (!m_pPlayer || LOBBY_INFO == m_pPlayer->GetLobbyAvatar() 
		|| LOBBY_CASH == m_pPlayer->GetLobbyAvatar())
		return;

	SPFXArchive* pLocalFXArchive	=	NULL;
	m_pPlayer->SPGOBPerformMessage( SPGM_GETFXARCHIVE , (LPARAM)&pLocalFXArchive );
	UINT64 uiAction = m_pPlayer->GetCurAction();

	bool bNeedProcess = false;
	tc_ActionData actionData;
	bNeedProcess = GetActionData( uiAction, actionData );
	if (bNeedProcess)
	{
		if (m_pPlayer->GetModelUnit())
		{
			if (m_pPlayer->GetModelUnit()->SPSendMessage(MV_GET_ANIMATION_INDEX) != actionData.sequence_id)
				m_pPlayer->GetModelUnit()->SPSendMessage( MV_SET_ANIMATION_INDEX, actionData.sequence_id );
		}
	}

	// 0. if need change
	// 2. animation sequence change
	// 3. FX prepare 
	//--------------------------------------------------
	if (uiAction == m_lastAction)
		return;

	if (!bNeedProcess)
	{
		if (pLocalFXArchive)
		{
			int lastFxID = GetFxIDByAction( m_lastAction );
			if (lastFxID > 0)
			{
				pLocalFXArchive->DeleteFXGroup( (UINT)lastFxID );
				m_pPlayer->ChangeAvatarOffset(-m_cfg.offsetX, -m_cfg.offsetY);
			}
		}
		m_pPlayer->SetWeaponVisible(true);
		m_lastAction = uiAction;
		DXUTOutputDebugString( "%d\n", (int)uiAction );

		return;
	}
	//--------------------------------------------------
	if (pLocalFXArchive)
	{
		int lastFxID = GetFxIDByAction( m_lastAction );
		int curFxID = GetFxIDByAction( uiAction );
		if (lastFxID != curFxID)
		{
			if (lastFxID > 0)
			{
				pLocalFXArchive->DeleteFXGroup( (UINT)lastFxID );
				m_pPlayer->ChangeAvatarOffset(-m_cfg.offsetX, -m_cfg.offsetY);
			}
			if (curFxID > 0)
			{
				pLocalFXArchive->InsertFXGroup( (SPID_FX_GROUP)curFxID );
				m_pPlayer->ChangeAvatarOffset(m_cfg.offsetX, m_cfg.offsetY);	
			}
		}
		m_pPlayer->SetWeaponVisible(false);
		m_lastAction = uiAction;
	}
	//--------------------------------------------------
}
void TumbleCloudEffect::Render( float fDeltaTime )
{
	// FX render
}
//--------------------------------------------------
void TumbleCloudEffect::SetSubtype( int iSubtype )
{
	if (!m_pPlayer || m_pPlayer->GetLobbyAvatar() == LOBBY_CASH)
	{
		m_pPlayer = NULL;
		return;
	}

	tumbleCloudCfg cfg;
	if (TumbleCloudLdtCfg::getInstance().GetCfgFromSubtype( iSubtype, cfg ))
	{
		m_iSubtype = iSubtype;
		m_cfg = cfg;
		m_lastAction = m_pPlayer->GetCurAction();
		m_pPlayer->m_bTumbleCloudMode = true;

		SPFXArchive* pLocalFXArchive	=	NULL;
		m_pPlayer->SPGOBPerformMessage( SPGM_GETFXARCHIVE , (LPARAM)&pLocalFXArchive );

		if (!pLocalFXArchive)
			return;

		m_restoreSeq = (int)m_pPlayer->GetModelUnit()->SPSendMessage(MV_GET_ANIMATION_INDEX);
		tc_ActionData actionData;
		if (GetActionData( m_lastAction, actionData ))
		{
			int fx_id = GetFxIDByAction( m_lastAction );
			if (fx_id <= 0)
				return;

			pLocalFXArchive->InsertFXGroup( (SPID_FX_GROUP)fx_id );
			if (m_pPlayer->GetModelUnit())
			{
				if (m_pPlayer->GetModelUnit()->SPSendMessage(MV_GET_ANIMATION_INDEX) != actionData.sequence_id)
					m_pPlayer->GetModelUnit()->SPSendMessage( MV_SET_ANIMATION_INDEX, actionData.sequence_id );
			}

			m_pPlayer->ChangeAvatarOffset( m_cfg.offsetX, m_cfg.offsetY );
			m_pPlayer->SetWeaponVisible( false );
		}
	}
}
int TumbleCloudEffect::GetEffectItemID( void ) const
{
	return m_itemID;
}
int TumbleCloudEffect::GetFxIDByAction( UINT64 &uiAction )
{
	for (int idx = 0; idx < MAX_TC_ACTION; idx++)
	{
		UINT64 &a = uiAction;
		UINT64 &b = m_cfg.action_data[idx].uiAction;

		//if ( (a==b) || ((a&b)&&(!(a&(~b)))) )
		if ( (a&b)==a )
		{
			return m_cfg.action_data[idx].fx_id;
		}
	}

	return -1;
}
bool TumbleCloudEffect::GetActionData( UINT64 uiAction, tc_ActionData &actionData )
{
	bool bResult = false;
	for (int idx = 0; idx < MAX_TC_ACTION; ++idx)
	{
		UINT64 &a = uiAction;
		UINT64 &b = m_cfg.action_data[idx].uiAction;

		//if ( (a==b) || ((a&b)&&(!(a&(~b)))) )
		if ( (a&b)==a )
		{
			bResult = true;
			actionData = m_cfg.action_data[idx];
			break;
		}
	}

	return bResult;
}
//--------------------------------------------------
//--------------------------------------------------
// configure 
//--------------------------------------------------
TumbleCloudLdtCfg::TumbleCloudLdtCfg( void )
{
	init();
}
TumbleCloudLdtCfg::~TumbleCloudLdtCfg( void )
{
	resetCfg();
}
//--------------------------------------------------
#ifndef LDT_GETINTVALUE
#define LDT_GETINTVALUE(id,name,to) \
	pFile->GetFieldFromLable((id),name,field); \
	(to) = field.uData.lValue;
#endif

#ifndef LDT_GETUINT64VALUE
#define LDT_GETUINT64VALUE(id,name,to) \
	pFile->GetFieldFromLable((id),name,field); \
	(to) = (UINT64)(field.uData.lValue);
#endif

#ifndef MAX_STRING_SIZE
#define MAX_STRING_SIZE 512
#endif
//--------------------------------------------------
bool TumbleCloudLdtCfg::LoadLdt( void )
{
	SPLDTFile	*pFile = NULL;
	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/TUMBLECLOUD.LDT", &pFile );
	if (!pFile)
	{
		assert( false && "tumble cloud ldt cannot be found." );
		return false;
	}

	resetCfg();
	LDT_Field	field;
	tumbleCloudCfg	data;
	tc_ActionData	action_data;

	int iRecordCount = pFile->GetItemCount();
	int itemID = 0;
	char buf[MAX_STRING_SIZE] = {0,};
	for (int idx = 0; idx < iRecordCount; ++idx)
	{
		itemID = pFile->GetPrimaryKey( idx );
		if (0 == itemID)
			continue;

		LDT_GETINTVALUE(itemID,"subtype",data.iSubtype);
		LDT_GETINTVALUE(itemID,"offsetX",data.offsetX);
		LDT_GETINTVALUE(itemID,"offsetY",data.offsetY);
		
		for (int iActionIndex = 0; iActionIndex < MAX_TC_ACTION; ++iActionIndex)
		{
			_snprintf( buf, MAX_STRING_SIZE-1, "action_%d", iActionIndex );
			LDT_GETUINT64VALUE(itemID,buf,action_data.uiAction);
			_snprintf( buf, MAX_STRING_SIZE-1, "sequence_%d", iActionIndex );
			LDT_GETINTVALUE(itemID,buf,action_data.sequence_id);
			_snprintf( buf, MAX_STRING_SIZE-1, "fx_%d", iActionIndex );
			LDT_GETINTVALUE(itemID,buf,action_data.fx_id);

			data.action_data[iActionIndex] = action_data;
		}

		m_vecLdtCfg.push_back( data );
		data.reset();
	}

	SAFE_RELEASE( pFile );
	return true;
}
bool TumbleCloudLdtCfg::ReloadLdt( void )
{
	resetCfg();
	return LoadLdt();
}
bool TumbleCloudLdtCfg::GetCfgFromSubtype( int iSubtype, tumbleCloudCfg &cfg )
{
	for (int idx = 0; idx < (int)m_vecLdtCfg.size(); ++idx)
	{
		if (m_vecLdtCfg[idx].iSubtype == iSubtype)
		{
			cfg = m_vecLdtCfg[idx];
			return true;
		}
	}
	return false;
}
bool TumbleCloudLdtCfg::resetCfg( void )
{
	m_vecLdtCfg.clear();
	return true;
}
bool TumbleCloudLdtCfg::init( void )
{
	return resetCfg();
}
//--------------------------------------------------
