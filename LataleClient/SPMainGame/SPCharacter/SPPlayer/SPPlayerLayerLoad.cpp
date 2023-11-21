#include "SPCommon.h"
#include "SPDebug.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPGOBClusterDEF.h"
#include "SPGOBCluster.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"

#include "SPManager.h"
//#include "SPItem.h"
#include "SPItemCluster.h"		//[2005/6/16]
#include "SPGOBManager.h"
#include "SPCameraManager.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"
#include "SPInputManager.h"

#include "SPSoundArchive.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPAvatarModelUnitDef.h"
//#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemStatusEquip.h"
#include "SPPlayerEquipInfo.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPCommandConvert.h"

#include "SPBattle.h"
#include "SPBattleCluster.h"

#include "SPUtil.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayerEquipDefine.h" // TEST Equip
#include "SPPlayer.h"

#include "SPStage.h"			//[2005/4/13] - jinhee
#include "SPStageManager.h"
#include "SPStageLoader.h"

#include "SPLootingBag.h"

#include "SPEventDEF.h"			//[2005/5/24]
#include "SPEventNpcHandler.h"
#include "SPEventManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPSoundManager.h"
#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"

#include "SPEffect.h"
#include "SPEffectArchive.h"
#include "SPEffectManager.h"
#include "SPCoolTimeManager.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPPlayerLayerLoad.h"


#if defined(_LAYER_THREAD_LOAD)

DWORD WINAPI EquipInitThread(LPVOID lParam)
{
	SPPlayer* pkPlayer = (SPPlayer*)lParam;

	EnterCriticalSection(&g_CriticalSection);

	for( int i = 0; i < pkPlayer->m_iEquipInitNum; i++ )
		pkPlayer->ItemSetting(pkPlayer->m_FigureItem[i]);

	DWORD dwID;
	HANDLE hHandle;

	pkPlayer->m_bUseThread[3] = true;
	hHandle = CreateThread(NULL, 0, WeaponChangeThread, pkPlayer, 0, &dwID);
	CloseHandle(hHandle);

	pkPlayer->m_bUseThread[0] = false;
	LeaveCriticalSection(&g_CriticalSection);

	return 0;
}

DWORD WINAPI EquipOnThread(LPVOID lParam)
{
	return 0;
}

DWORD WINAPI EquipOffThread(LPVOID lParam)
{
	return 0;
}

DWORD WINAPI WeaponChangeThread(LPVOID lParam)
{
	SPPlayer* pkPlayer = (SPPlayer*)lParam;

	EnterCriticalSection(&g_CriticalSection);

	if( pkPlayer->IsLocalPlayer() )
	{
		pkPlayer->GetInvenArchive()->OnSendMessage(
			SPGM_ITEM_SC_USE_WEAPON_SET,  pkPlayer->GetActiveWeapon());
		pkPlayer->GetInvenArchive()->OnSendMessage(
			SPGM_ITEM_SC_USE_WEAPON_CHANGE,  pkPlayer->GetActiveWeapon());
	}
	else
		pkPlayer->WeaponChange();

	pkPlayer->m_bUseThread[3] = false;
	LeaveCriticalSection(&g_CriticalSection);

	return 0;
}

bool SPPlayer::AllUnUsedThread()
{
	for( int i = 0; i < 4; i++ )
	{
		if( m_bUseThread[i] )
			return false;
	}

	return true;
}

#endif