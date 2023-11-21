#include "SPTransform.h"
#include "SPCommon.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "PacketID.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPPlayer.h"
#include "SPGOBModelUnit.h"
#include "SPGOBCoordPhysics.h"
#include "SPAvatarModelUnit.h"
#include "SPMonsterModelUnit.h"
#include "SPCommandConvert.h"
#include "SPUtil.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPMonsterStatus.h"
#include "SPMonsterStatusModel.h"
#include "SPMonster.h"
#include "SPDebug.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPTransformMonster.h"
#include "SPResourceDef.h"
#include <vector>
using namespace std;

//--------------------------------------------------
// TransformLdtCfg
//--------------------------------------------------
TransformLdtCfg::TransformLdtCfg( void )
{
	m_vecLdtCfg.clear();
	init();
}
TransformLdtCfg::~TransformLdtCfg( void )
{
	resetCfg();
}
//--------------------------------------------------
bool TransformLdtCfg::LoadLdt( void )
{
	SPLDTFile	*pFile = NULL;
	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/TRANSFORM.LDT", &pFile );
	if (!pFile)
	{
		assert( false && "transform ldt load failed." );
		return false;
	}

	resetCfg();
	LDT_Field field;
	transformCfg data;

	int iRecordCount = pFile->GetItemCount();
	int itemID = 0;
	for (int idx = 0; idx < iRecordCount; ++idx)
	{
		itemID = pFile->GetPrimaryKey( idx );

		pFile->GetFieldFromLable( itemID, "subtype", field );
		data.iSubtype = field.uData.lValue;

		pFile->GetFieldFromLable( itemID, "monsterID", field );
		data.iMonsterID = field.uData.lValue;

		m_vecLdtCfg.push_back( data );
		data.reset();
	}
	
	SAFE_RELEASE( pFile );
	return true;
}
bool TransformLdtCfg::ReloadLdt( void )
{
	resetCfg();
	return LoadLdt();
}
//--------------------------------------------------
bool TransformLdtCfg::GetCfgFromSubtype(int iSubtype, transformCfg &cfg)
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
//--------------------------------------------------
bool TransformLdtCfg::resetCfg( void )
{
	m_vecLdtCfg.clear();
	return true;
}
bool TransformLdtCfg::init( void )
{
	resetCfg();
	return true;
}
//--------------------------------------------------
// TransformEffect class
//--------------------------------------------------
TransformEffect::TransformEffect( void )
{
	m_pPlayer = NULL;
	m_pMonster = NULL;
	m_itemID = 0;
	m_iSubtype = 0;
	m_cfg.reset();
}
TransformEffect::TransformEffect(SPPlayer *pParent, int iItemID)
{
	m_pPlayer = pParent;
	m_pMonster = NULL;
	m_itemID = iItemID;
	m_iSubtype = 0;
	m_cfg.reset();
}
TransformEffect::~TransformEffect( void )
{
	if (m_pPlayer)
	{
		m_pPlayer->m_bTransformMode = false;
		m_pPlayer->SetShowValue( true, true, true );
		m_pPlayer->GetStatusModel()->SetNameOffset( 155 );
	}

	TransformMonsterPool::getInstance().ReleaseMonster( m_pMonsterData.ID );

	m_pPlayer = NULL;
	m_itemID = 0;
	m_iSubtype = 0;
	m_cfg.reset();
}
//--------------------------------------------------
void TransformEffect::SetSubtype( int iSubtype )
{
	if (m_pPlayer->GetLobbyAvatar() != LOBBY_NULL)
		return;

	transformCfg cfg;
	if (TransformLdtCfg::getInstance().GetCfgFromSubtype( iSubtype, cfg ))
	{
		m_iSubtype = iSubtype;
		m_cfg = cfg;

		if (m_pPlayer)
		{
			m_pPlayer->m_bTransformMode = true;
			m_pPlayer->SetShowValue( true, false, true );
		}

		m_pMonsterData = TransformMonsterPool::getInstance().GetMonster();
		m_pMonster = m_pMonsterData.pMonster;
		m_pMonster->SetClassID((CLASS_ID)m_cfg.iMonsterID);
		m_pMonster->Reload();
		m_pMonster->Process( 0.0001f );
		int iMonsterType = MonsterTypeManager::getInstance().GetMonsterType( m_cfg.iMonsterID );
		if (iMonsterType == 11)
			m_pMonster->m_bFly = true;
		else
			m_pMonster->m_bFly = false;
		SPMonsterModelUnit *monsterUnit = (SPMonsterModelUnit*)m_pMonster->m_pkModelUnit;
		RECT rgn = monsterUnit->GetRenderRgn();
		m_pPlayer->GetStatusModel()->SetNameOffset( rgn.bottom - rgn.top + 25 );
	}
}
int TransformEffect::GetEffectItemID( void ) const
{
	return m_itemID; 
}
//--------------------------------------------------
void TransformEffect::Process( float fDeltaTime )
{
	if (m_pMonster)
	{
		POINT ptCurrent = {0,0};
		float x = m_pPlayer->GetPosX() - g_pCameraManagr->GetCameraStruct()->fOffsetX;
		float y = m_pPlayer->GetPosY() - g_pCameraManagr->GetCameraStruct()->fOffsetY;

		//--------------------------------------------------
		// push the player's status change to monster
		// stand, walk, attack, beattack, jump, fall, dead, land
		// face - left & right
		UINT64 uiParentAction;
		m_pPlayer->SPGOBPerformMessage(SPGM_GETACTION, (LPARAM)&uiParentAction);

		if (uiParentAction & ACTION_MOVE_HANGING_LEFT)
		{
			uiParentAction |= ACTION_MOVE_LEFT;
			y -= 95.0f;
		}
		else if (uiParentAction & ACTION_MOVE_HANGING_RIGHT)
		{
			uiParentAction |= ACTION_MOVE_RIGHT;
			y -= 95.0f;
		}
		else if (uiParentAction & ACTION_HANGING_STOP)
		{
			uiParentAction |= ACTION_SIT_WAIT;
			y -= 95.0f;
		}

		if (uiParentAction & ACTION_RUN_LEFT)
			uiParentAction |= ACTION_MOVE_LEFT;
		else if (uiParentAction & ACTION_RUN_RIGHT)
			uiParentAction |= ACTION_MOVE_RIGHT;
		else if (uiParentAction & ACTION_RUN_STOP)
			uiParentAction |= ACTION_SIT_WAIT;

		if (uiParentAction & ACTION_VERTICAL_STOP
			||
			uiParentAction & ACTION_MOVE_UP
			||
			uiParentAction & ACTION_MOVE_DOWN)
			uiParentAction |= ACTION_SIT_WAIT;

		//--------------------------------------------------

		if (m_pMonster->m_bFly)
			y -= 30.0f;
		WPARAM	wParsingX, wParsingY;
		DataConvert<float,WPARAM>(x, wParsingX);
		DataConvert<float,WPARAM>(y, wParsingY);

		m_pMonster->m_pkModelUnit->SPSendMessage( MV_SET_RENDERPOSX, wParsingX );
		m_pMonster->m_pkModelUnit->SPSendMessage( MV_SET_RENDERPOSY, wParsingY );
		m_pMonster->m_pkModelUnit->Process( fDeltaTime );
		m_pMonster->SetAction( uiParentAction );
	}
}
void TransformEffect::Render( float fDeltaTime )
{
	if (m_pMonster)
		m_pMonster->m_pkModelUnit->Render( fDeltaTime );
}
//--------------------------------------------------
// class TransformMonsterPool
//--------------------------------------------------
TransformMonsterPool::TransformMonsterPool( void )
{
	m_iPoolSize = 10;
}
TransformMonsterPool::~TransformMonsterPool( void )
{
	Destroy();
}
//--------------------------------------------------
bool TransformMonsterPool::Init( void )
{
	for (int count = 0; count < m_iPoolSize; ++count)
	{
		TMonsterPoolData data;
		data.ID = generateID();
		SPTransformMonster *pMonster = new SPTransformMonster;
		data.pMonster = pMonster;
		m_vecFreeMonster.push_back( data );
	}
	return true;
}
bool TransformMonsterPool::Destroy( void )
{
	for (int idx = 0; idx < (int)m_vecFreeMonster.size(); ++idx)
	{
		SAFE_DELETE( m_vecFreeMonster[idx].pMonster );
	}
	for (int idx = 0; idx < (int)m_vecUsedMonster.size(); ++idx)
	{
		SAFE_DELETE( m_vecUsedMonster[idx].pMonster );
	}

	return true;
}
int TransformMonsterPool::generateID( void )
{
	static int s_ID = 0;
	return s_ID++;
}
//--------------------------------------------------
TMonsterPoolData TransformMonsterPool::GetMonster( void )
{
	TMonsterPoolData data;

	if (!m_vecFreeMonster.empty())
	{
		data = m_vecFreeMonster.back();
		m_vecFreeMonster.pop_back();
		m_vecUsedMonster.push_back( data );
	}
	else
	{
		data.ID = generateID();
		data.pMonster = new SPTransformMonster;
		m_vecUsedMonster.push_back( data );
	}

	return data;
}
bool TransformMonsterPool::ReleaseMonster( int iID )
{
	TMonsterPoolData data;
	vector< TMonsterPoolData >::iterator iter;
	for (iter = m_vecUsedMonster.begin();
		iter != m_vecUsedMonster.end();
		++iter)
	{
		if (iter->ID == iID)
		{
			data = *iter;
			break;
		}
	}

	if (iter != m_vecUsedMonster.end())
	{
		m_vecUsedMonster.erase( iter );
		m_vecFreeMonster.push_back( data );
		return true;
	}
	else
		return false;

}
//--------------------------------------------------
// MONSTERTYPEMANAGER
//--------------------------------------------------
MonsterTypeManager::MonsterTypeManager( void )
{
	m_vecMonsterType.clear();
}
MonsterTypeManager::~MonsterTypeManager( void )
{
	m_vecMonsterType.clear();
}
int MonsterTypeManager::GetMonsterType(int iMonsterID)
{
	for (int idx = 0; idx < (int)m_vecMonsterType.size(); ++idx)
	{
		if (m_vecMonsterType[idx].iMonsterID == iMonsterID)
			return m_vecMonsterType[idx].iMonsterType;
	}
	
	return -1;
}
//--------------------------------------------------
void MonsterTypeManager::Init( void )
{
	SPLDTFile	*pFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_MOBAI, &pFile );
	if (!pFile)
	{
		return;
	}

	LDT_Field field;
	MonsterTypeInfo data;
	int iRecordCount = pFile->GetItemCount();
	int itemID = 0;
	for (int idx = 0; idx < iRecordCount; ++idx)
	{
		itemID = pFile->GetPrimaryKey( idx );
		data.iMonsterID = itemID;

		pFile->GetFieldFromLable( itemID, "_Type", field );
		data.iMonsterType = field.uData.lValue;

		m_vecMonsterType.push_back( data );
	}

	SAFE_RELEASE( pFile );
	return;
}
//--------------------------------------------------
