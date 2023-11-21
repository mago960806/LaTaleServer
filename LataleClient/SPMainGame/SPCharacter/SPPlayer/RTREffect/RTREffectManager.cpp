#include "RTREffectManager.h"
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
#include "SPPhantom.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPPhantom.h"
#include "SPTransform.h"
#include "SPTumbleCloud.h"
#include <vector>
#include <algorithm>
#include "SPXTrace.h"
using namespace std;
//--------------------------------------------------
// RTREffectCfg
//--------------------------------------------------
RTREffectCfg &RTREffectCfg::getInstance( void )
{
	static RTREffectCfg __instance;
	return __instance;
}
RTREffectCfg::RTREffectCfg( void )
{
	m_vecData.clear();
}
RTREffectCfg::~RTREffectCfg( void )
{
	// nothing till now.
}
bool RTREffectCfg::LoadLdt( void )
{
	SPLDTFile	*pFile = NULL;
	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/RTREFFECT.LDT", &pFile );
	if (!pFile)
	{
		assert( false && "real time render effect ldt load failed." );
		return false;
	}

	m_vecData.clear();
	LDT_Field field;
	RTREffectData data;

	int iRecordCount = pFile->GetItemCount();
	int iItemID = 0;
	for (int idx = 0; idx < iRecordCount; ++idx)
	{
		iItemID = pFile->GetPrimaryKey( idx );

		pFile->GetFieldFromLable( iItemID, "itemID", field );
		data.itemID = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "name", field );
		if (field.uData.pValue)
			data.strName = field.uData.pValue;

		pFile->GetFieldFromLable( iItemID, "type", field );
		if (field.uData.lValue > RTRTYPE_NULL
			&&
			field.uData.lValue < RTRTYPEENUMNUMBER)
		{
			data.type = (RTRType)(field.uData.lValue);
		}

		pFile->GetFieldFromLable( iItemID, "subtype", field );
		data.subtype = (int)field.uData.lValue;

		m_vecData.push_back( data );
		data.reset();
	}

	SAFE_RELEASE( pFile );
	return true;
}
bool RTREffectCfg::ReloadLdt( void )
{
	resetCfg();
	return LoadLdt();
}
bool RTREffectCfg::resetCfg( void )
{
	m_vecData.clear();
	return true;
}
bool RTREffectCfg::init( void )
{
	m_vecData.clear();
	return true;
}
bool RTREffectCfg::GetEffectFromItemID(int itemID, RTREffectData &data)
{
	for (int idx = 0; idx < (int)m_vecData.size(); ++idx)
	{
		if (m_vecData[idx].itemID == itemID)
		{
			data = m_vecData[idx];
			return true;
		}
	}

	return false;
}
//--------------------------------------------------
// RTEEffectManager
//--------------------------------------------------
class __deleteEffect
{
public:
	__deleteEffect( int itemID )
	{
		m_itemID = itemID;
	}
	int m_itemID;
	bool operator()( IRTREffect *effect )
	{
		if (effect->GetEffectItemID() == m_itemID)
			return true;
		else
			return false;
	}
};
bool RTREffectManager::m_bInited = false;
RTREffectManager::RTREffectManager( void )
{
	m_pParent = NULL;
	m_vecRTREffect.clear();

	Init();
}
RTREffectManager::RTREffectManager(SPPlayer *pParent)
{
	m_pParent = pParent;
	m_vecRTREffect.clear();

	Init();
}
RTREffectManager::~RTREffectManager( void )
{
	m_pParent = NULL;
	m_vecRTREffect.clear();
}
bool RTREffectManager::Init( void )
{
	if (!m_bInited)
	{
		TransformMonsterPool::getInstance().Init();
		MonsterTypeManager::getInstance().Init();
		RTREffectCfg::getInstance().LoadLdt();
		PhantomLDTCfg::getInstance().LoadLdt();
		TransformLdtCfg::getInstance().LoadLdt();
		TumbleCloudLdtCfg::getInstance().LoadLdt();
		m_bInited = true;
	}
	return true;
}
bool RTREffectManager::Clear( void )
{
	m_vecRTREffect.clear();
	return true;
}
//--------------------------------------------------
void RTREffectManager::Process( float fDeltaTime )
{
	for (int idx = 0; idx < (int)m_vecRTREffect.size(); ++idx)
	{
		IRTREffect *effect = m_vecRTREffect[idx];
		if (effect)
			effect->Process( fDeltaTime );
	}
}
void RTREffectManager::Render( float fDeltaTime )
{
	for (int idx = 0; idx < (int)m_vecRTREffect.size(); ++idx)
	{
		IRTREffect *effect = m_vecRTREffect[idx];
		if (effect)
			effect->Render( fDeltaTime );
	}
}
//--------------------------------------------------
void RTREffectManager::AddItemRTREffect(int itemID)
{
	RTREffectData data;
	if (RTREffectCfg::getInstance().GetEffectFromItemID( itemID, data ))
	{
		switch (data.type)
		{
		case RTRTYPE_PHANTOM:
			{
				vector< IRTREffect* >::iterator iter =
					find_if( m_vecRTREffect.begin(), m_vecRTREffect.end(), __deleteEffect( itemID ) );
				if (iter == m_vecRTREffect.end())
				{
					IRTREffect *effect = new PhantomEffect( m_pParent, itemID );
					effect->SetSubtype( data.subtype );
					m_vecRTREffect.push_back( effect );
				}
			}
			break;
		case RTRTYPE_TRANSFORM:
			{
				vector< IRTREffect* >::iterator iter =
					find_if( m_vecRTREffect.begin(), m_vecRTREffect.end(), __deleteEffect( itemID ));
				if (iter == m_vecRTREffect.end())
				{
					IRTREffect *effect = new TransformEffect( m_pParent, itemID );
					effect->SetSubtype( data.subtype );
					m_vecRTREffect.push_back( effect );
				}
			}
			break;
		case RTRTYPE_TUMBLECLOUD:
			{
				vector< IRTREffect* >::iterator iter =
					find_if( m_vecRTREffect.begin(), m_vecRTREffect.end(), __deleteEffect( itemID ));
				if (iter == m_vecRTREffect.end())
				{
					IRTREffect *effect = new TumbleCloudEffect( m_pParent, itemID );
					effect->SetSubtype( data.subtype );
					m_vecRTREffect.push_back( effect );
				}
			}
			break;
		default:
			break;
		}
	}
}
void RTREffectManager::DelItemRTREffect(int itemID)
{
	for (int idx = 0; idx < (int)m_vecRTREffect.size(); ++idx)
	{
		IRTREffect *effect = m_vecRTREffect[idx];
		if (effect && effect->GetEffectItemID() == itemID)
		{
			delete effect;
			m_vecRTREffect[idx] = NULL;
		}
	}

	vector< IRTREffect* >::iterator iter =
		remove( m_vecRTREffect.begin(), m_vecRTREffect.end(), (IRTREffect*)NULL );
	if (iter != m_vecRTREffect.end())
		m_vecRTREffect.erase( iter, m_vecRTREffect.end() );
}
//--------------------------------------------------
//--------------------------------------------------
// end of file
