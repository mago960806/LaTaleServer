#include "SPCommon.h"

#include <algorithm>
using namespace std;

#include "SPGameObject.h"
#include "SPGOBManager.h"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPUIUnit.h"
#include "SPUISkillUnit.h"
#include "SPUIUnitManager.h"
#include "SPDragnDropManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowSlider.h"
#include "SPWindowGrid.h"
#include "SPWindowGauge.h"
#include "SPWindowToolTip.h"
#include "SPWindowPet.h"
#include "SPWindowPetShop.h"

#include "SPCommandConvert.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPPlayerInvenArchive.h"

#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"

#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPSkillManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPPlayer.h"

#include "SPCheckManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPUtil.h"
#include "SPPet.h"
#include "SPPetManager.h"
#include "SPPetLevelMgr.h"
//--------------------------------------------------
static bool verifyPlace( PET_PLACE place )
{
	if (place >= PET_PRIMARY && place <= PET_TERTIARY)
		return true;
	else
		return false;
}
//--------------------------------------------------
SPPetManager::SPPetManager( void )
{
	Init();
}
SPPetManager::~SPPetManager( void )
{
	Destroy();
}
//--------------------------------------------------
void SPPetManager::Init( void )
{
	memset( m_vecPets, 0, sizeof(int)*(PET_PLACE_TYPE_NUMBER+1) );
	m_bPetShopUncloseErr = true;
}
void SPPetManager::Destroy( void )
{
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		SAFE_DELETE( m_vecPets[idx] );
	}
}
void SPPetManager::Process(float fDeltaTime)
{
	if (m_bPetShopUncloseErr)
	{
		for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
		{
			if (m_vecPets[idx] && m_vecPets[idx]->GetShopUncloseError())
			{
				char buf[LEN_NAME+1] = {0,};
				_snprintf( buf, LEN_NAME, "%s", m_vecPets[idx]->GetPetAttr()->szPetName );
				char bufMsg[512] = {0,};
				_snprintf( bufMsg, 511, "[系统]您的宠物[%s]正在摆摊中。", buf );
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)bufMsg );
			}
		}
		m_bPetShopUncloseErr = false;
	}

	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		if (m_vecPets[idx] && !IsPetOpenShop(m_vecPets[idx]))
			m_vecPets[idx]->Process( fDeltaTime );
	}
}
void SPPetManager::Render( float fDeltaTime )
{
	for (int idx = PET_PLACE_TYPE_NUMBER-1; idx > PET_PLACE_NULL; --idx)
	{
		if (m_vecPets[idx] && !IsPetOpenShop(m_vecPets[idx]))
		{
			m_vecPets[idx]->Render( fDeltaTime );
			m_bNeedRender[idx] = false;
		}
	}
}
//--------------------------------------------------
void SPPetManager::SetPet( PET_PLACE place, SPPet *pet )
{
	if (!verifyPlace(place))
		return;

	if (m_vecPets[place])
		DelPet( place );

	m_vecPets[place] = pet;
}
void SPPetManager::AddPet( SPPet *pet )
{
	if (m_vecPets[PET_PRIMARY])
		DelPet( PET_PRIMARY );

	m_vecPets[PET_PRIMARY] = pet;
}
void SPPetManager::DelPet( PET_PLACE place )
{
	if (!verifyPlace(place))
		return;

	SAFE_DELETE( m_vecPets[place] );
}
void SPPetManager::DelPet( SPPet *pet )
{
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		if (m_vecPets[idx] == pet)
		{
			SAFE_DELETE( m_vecPets[idx] );
			return;
		}
	}
}
bool SPPetManager::IsPetOpenShop(SPPet *pet)
{
	if (pet)
	{
		if (pet->GetOpenShopStatus())
			return true;
		else
			return false;
	}
	return false;
}
//--------------------------------------------------
bool SPPetManager::IsEnableSpecialSkill(SKILL_SPECIAL_TYPE eType, SPPet **pet /* = 0 */)
{
	bool result = false;
	for (int idx = 0; idx < (int)PET_PLACE_TYPE_NUMBER; ++idx)
	{
		if (m_vecPets[idx])
		{
			result = m_vecPets[idx]->IsEnableSpecialSkill( eType );
			if (result)
			{
				if (pet)
					*pet = m_vecPets[idx];
				 return true;
			}
		}
	}

	return false;
}
bool SPPetManager::IsToggleOnSpecialSkill(SKILL_SPECIAL_TYPE eType, SPPet **pet /* = 0 */)
{
	bool result = false;
	for (int idx = 0; idx < (int)PET_PLACE_TYPE_NUMBER; ++idx)
	{
		if (m_vecPets[idx])
		{
			result = m_vecPets[idx]->IsToggleOnSpecialSkill( eType );
			if (result)
			{
				if (pet)
					*pet = m_vecPets[idx];
				 return true;
			}
		}
	}

	return false;
}
bool SPPetManager::JudgePreRender(bool forceRender /* = false  */)
{
	for (int idx = 0; idx < (int)PET_PLACE_TYPE_NUMBER; ++idx)
	{
		if (forceRender)
			m_bNeedRender[idx] = true;
		else
		{
			if (m_vecPets[idx])
				if (m_vecPets[idx]->IsPreRendering()) m_bNeedRender[0] = false;
				else m_bNeedRender[idx] = true;
		}
	}
	return true;
}
void SPPetManager::SetRePos(int iStageID, int iMapGroupID, float fPosX, float fPosY )
{
	for (int idx = 0; idx < (int)PET_PLACE_TYPE_NUMBER; ++idx)
	{
		SPPet *pet = m_vecPets[idx];
		if (pet)
			pet->SetRePos( iStageID, iMapGroupID, fPosX, fPosY );	
	}
}
void SPPetManager::SetPos(float fX, float fY)
{
	for (int idx = 0; idx < (int)PET_PLACE_TYPE_NUMBER; ++idx)
	{
		SPPet *pet = m_vecPets[idx];
		if (pet)
			pet->SetPos( fX+40*idx, fY );
	}
}
void SPPetManager::SetCurStageID(int iStageID)
{
	for (int idx = 0; idx < (int)PET_PLACE_TYPE_NUMBER; ++idx)
	{
		SPPet *pet = m_vecPets[idx];
		if (pet)
			pet->SetCurStageID( iStageID );
	}
}
void SPPetManager::SetCurMapGroupID(int iMapGroupID)
{
	for (int idx = 0; idx < (int)PET_PLACE_TYPE_NUMBER; ++idx)
	{
		SPPet *pet = m_vecPets[idx];
		if (pet)
			pet->SetCurMapGroupID( iMapGroupID );
	}
}
void SPPetManager::SetSkillAnimation(PET_PLACE place, ANIMATION_INDEX eIndex)
{
	if (place <= PET_PLACE_NULL || place >= PET_PLACE_TYPE_NUMBER)
		return;
	SPPet *pet = m_vecPets[place];
	if (pet)
		pet->SetSkillAnimation( eIndex );
}
void SPPetManager::SetAction(PET_PLACE place, unsigned __int64 uiAction, bool bForce /* = false  */)
{
	if (place <= PET_PLACE_NULL || place >= PET_PLACE_TYPE_NUMBER)
		return;
	SPPet *pet = m_vecPets[place];
	if (pet)
		pet->SetAction( uiAction, bForce );
}
void SPPetManager::SetChatMsg(PET_PLACE place, std::vector< std::string > &vecChatMsg)
{
	SPPet *pet = m_vecPets[place];
	if (pet)
		pet->SetChatMsg( vecChatMsg );
}
SPPet* SPPetManager::GetPetFromITEMNO(ITEMNO itemNo)
{
	for (int idx = 0; idx < (int)PET_PLACE_TYPE_NUMBER; ++idx)
	{
		SPPet *pet = m_vecPets[idx];
		if (pet && pet->GetItemNoID() == itemNo)
			return pet;
	}

	return NULL;
}
//--------------------------------------------------
// end of file
