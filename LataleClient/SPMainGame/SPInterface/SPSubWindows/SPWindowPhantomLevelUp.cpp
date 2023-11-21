#include "SPCommon.h"
#include "SPUtil.h"

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <map>

//#include "SPGOBStatus.h"
#include "SPGOBManager.h"
//
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPCommandConvert.h"

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"

#include "SPCheckManager.h"

#include "SPPlayerInvenArchive.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowToolTip.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"

#include "SPDebug.h"


#include "SPWindowPhantomLevelUp.h"



SPWindowEffectItemUpgrade::SPWindowEffectItemUpgrade(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_iFrame(0)
, m_pQuestion(NULL)
, m_pDisable(NULL)
, m_pItemResult(NULL)
{
	Init();
}

SPWindowEffectItemUpgrade::SPWindowEffectItemUpgrade(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_EFFECTITEM_UPGRADE, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}


SPWindowEffectItemUpgrade::~SPWindowEffectItemUpgrade()
{
	Clean();
}


void SPWindowEffectItemUpgrade::Init()
{
	initLDT();
	m_iLevelUpStatus = LEVELUP_STATUS_FIRST;

	for(int i = 0; i < MAX_LEVELUP_SLOT; i++)
	{
		m_pItem[i] = NULL;
		m_pIcon[i] = NULL;
		m_SelectItem[i].Clear();
	}	
	m_pCursorFirst = NULL;
	m_pCursorSecond = NULL;
	m_pToolTip = NULL;
	m_pOK = NULL;
	m_pCancel = NULL;

	m_pItemResult = new SPItem(STORAGE, 0);

	std::string strTextureFile = "";
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);

	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY

	CreateSubWindow();

	m_bUse	=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_PHANTOM );
}


void SPWindowEffectItemUpgrade::Clean()
{
	for(int i = 0; i < MAX_LEVELUP_SLOT; i++)
	{
		m_pItem[i] = NULL;
		m_SelectItem[i].Clear();
		m_pIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	}

	m_pToolTip->SPSendMessage(SPIM_SET_IMAGECLEAR);

	SAFE_RELEASE(m_pDisable);

	SAFE_DELETE(m_pItemResult);

	SPWindow::Clean();
}


void SPWindowEffectItemUpgrade::CreateSubWindow()
{
	SPWindowStatic* pStatic = NULL;
	SPWindowButton* pButton = NULL;
	SPWindowStatic* pCenter = NULL;
	SPWindowStatic* pSkin = NULL;

	// Top
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_TOP, 0, 0, 240, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 31);
	pStatic->Show();
	pStatic = NULL;

	// Center
	pCenter = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_CENTER, 0, 4, 240, 117, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 37);
	pCenter->SetSrcSize(240, 2);
	pCenter->Show();

	// bottom
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_BOTTOM, 0, 121, 240, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 41);
	pStatic->Show();
	pStatic = NULL;

	// WIID_EFFECTITEM_UPGRADE_TITLE
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_TITLE, 10, 6, 143, 16, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/CNUI01.PNG", 66, 57);
	pStatic->Show();
	pStatic = NULL;

	// WIID_EFFECTITEM_UPGRADE_CLOSE
	pButton = new SPWindowButton(WIID_EFFECTITEM_UPGRADE_CLOSE, 219, 9 - 4, 12, 12, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG",	301,	321);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI103.PNG",	314,	321); //on
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG",	327,	321); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	340,	321); //disable
	pButton->Show();
	pButton = NULL;	

	// WIID_EFFECTITEM_UPGRADE_LINE1
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_LINE1, 2, 26 - 4, 236, 1, pCenter);
	pStatic->SetSrcSize(6, 1);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pStatic->Show();
	pStatic = NULL;

	// WIID_EFFECTITEM_UPGRADE_LINE2
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_LINE2, 2, 97 - 4, 236, 1, pCenter);
	pStatic->SetSrcSize(6, 1);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pStatic->Show();
	pStatic = NULL;

	// WIID_EFFECTITEM_UPGRADE_SKIN
	pSkin = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_SKIN, 2, 27 - 4, 236, 70, pCenter);
	pSkin->SetSrcSize(6, 6);
	pSkin->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 477);
	pSkin->Show();

	//WIID_EFFECTITEM_UPGRADE_QUESTION
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_QUESTION, 182 - 2, 44 - 27, 12, 18, pSkin);	
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 360, 321);
	pStatic->Show();
	pStatic = NULL;

	int iStartX[MAX_LEVELUP_SLOT] = {36, 104, 172};
	for(int i = 0; i < MAX_LEVELUP_SLOT; i++) 
	{
		pButton = new SPWindowButton(WIID_EFFECTITEM_UPGRADE_SLOTBUTTON + i, iStartX[i] - 2, 37 - 27, 32, 32, pSkin);	
		pButton->Show();
		pButton = NULL;

		pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_SLOTLINE + i, iStartX[i] - 2, 37 - 27, 32, 32, pSkin);	
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
		pStatic->Show();
		pStatic = NULL;	

		pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_SLOTICON + i, iStartX[i] - 2, 37 - 27, 32, 32, pSkin);	
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
		pStatic->Show();
		m_pIcon[i] = pStatic;
		m_pIcon[i]->GetWindowRect(m_rtStack[i]);
		pStatic = NULL;	

		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;
		m_iStack[i] = 0;
	}

	//WIID_EFFECTITEM_UPGRADE_CURSOR1
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_CURSOR1, 47 - 2, 67 - 27, 10, 10, pSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 354, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 365, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 376, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 387, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 398, 261);
	pStatic->Show();
	m_pCursorFirst = pStatic;
	pStatic = NULL;

	//WIID_EFFECTITEM_UPGRADE_CURSOR2
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_CURSOR2, 115 - 2, 67 - 27, 10, 10, pSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 354, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 365, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 376, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 387, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 398, 261);
	pStatic->Show();
	m_pCursorSecond = pStatic;
	pStatic = NULL;

	//WIID_EFFECTITEM_UPGRADE_TOOLTIP
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_TOOLTIP, 16 - 2, 79 - 27, 208, 15, pSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/CNUI01.PNG", 1, 42);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 77);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 93);
	pStatic->Show();
	pStatic->SetFontColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ));
	m_pToolTip = pStatic;
	pStatic = NULL;

	//WIID_EFFECTITEM_UPGRADE_PLUS
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_PLUS, 78 - 2, 44 - 27, 16, 16, pSkin);	
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 239, 423);
	pStatic->Show();
	pStatic = NULL;		

	//WIID_EFFECTITEM_UPGRADE_EQUAL
	pStatic = new SPWindowStatic(WIID_EFFECTITEM_UPGRADE_EQUAL, 146 - 2, 45 - 27, 16, 14, pSkin);	
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 239, 408);
	pStatic->Show();
	pStatic = NULL;

	//WIID_EFFECTITEM_UPGRADE_OK	
	m_pOK = new SPWindowButton(WIID_EFFECTITEM_UPGRADE_OK, 90, 102 - 4, 68, 17, pCenter);
	m_pOK->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			346,	440);
	m_pOK->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		346,	458);	//on
	m_pOK->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		346,	476);	//push
	m_pOK->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	346,	494); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pOK->Show();

	//WIID_EFFECTITEM_UPGRADE_CANCEL
	m_pCancel = new SPWindowButton(WIID_EFFECTITEM_UPGRADE_CANCEL, 162, 102 - 4, 68, 17, pCenter);
	m_pCancel->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			415,	440);
	m_pCancel->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		415,	458);	//on
	m_pCancel->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		415,	476);	//push
	m_pCancel->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	415,	494); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pCancel->Show();

}


void SPWindowEffectItemUpgrade::Process(float fTime)
{
	SPWindow::Process(fTime);	
	ProcessCursor(fTime);
}


void SPWindowEffectItemUpgrade::ProcessCursor(float fTime)
{
	m_fCursorAccmulateTime += fTime;
	if( m_fCursorAccmulateTime > 0.05f )
	{
		if( m_iCursorDirection )
		{
			m_iFrame--;
			if( m_iFrame < 0 )
			{
				m_iFrame = 1;
				m_iCursorDirection = 0;
			}
		}
		else
		{
			m_iFrame++;
			if( m_iFrame > 4 )
			{
				m_iFrame = 3;
				m_iCursorDirection = 1;
			}
		}

		if(m_iLevelUpStatus == LEVELUP_STATUS_FIRST)
		{
			m_pCursorFirst->SPSendMessage(SPIM_SET_IMAGEINDEX, m_iFrame);
			m_pCursorSecond->Hide();
		}
		else if(m_iLevelUpStatus == LEVELUP_STATUS_SECOND)
		{
			m_pCursorFirst->Hide();
			m_pCursorSecond->SPSendMessage(SPIM_SET_IMAGEINDEX, m_iFrame);
		}
		else 
		{
			m_pCursorFirst->Hide();
			m_pCursorSecond->Hide();
		}

		m_fCursorAccmulateTime = 0.0f;
	}
}


void SPWindowEffectItemUpgrade::Render(float fTime)
{
	SPWindow::Render(fTime);

	for(int i = 0; i < MAX_LEVELUP_SLOT; i++)
	{
		//Icon Disable 钎泅
		if(m_pDisable)
		{
			RECT rtDest;
			m_pIcon[i]->GetWindowRect(rtDest);
			if(m_bItemAble[i] == false)
			{		
				m_pDisable->RenderTexture(&rtDest, &m_rtSrcDisable);
			}
			if(m_iItemMark[i] > ICON_MARK_NULL)
			{
				m_pDisable->RenderTexture(&rtDest, &m_rtMarkSrc[m_iItemMark[i] - 1]);
			}			
		}

		//Stack Count Local 钎泅
		if(m_iStack[i] > 9)
		{
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iStack[i] / 10;
			int iOneStack = m_iStack[i] % 10;
			m_pIcon[i]->GetWindowRect(m_rtStack[i]);
			SetRect(&rtTenNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);
			SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtStack[i].bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtStack[i].bottom);

			rtTenNumSrc.left = iTenStack * 9;
			rtTenNumSrc.right = rtTenNumSrc.left + 9;
			rtTenNumSrc.top = 0;
			rtTenNumSrc.bottom = 10;

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtTenNumTarget , &rtTenNumSrc);
			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
		else if(m_iStack[i] > 1) 
		{
			int iOneStack = m_iStack[i] % 10;
			RECT rtNumTarget, rtNumSrc;
			m_pIcon[i]->GetWindowRect(m_rtStack[i]);
			SetRect(&rtNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
	}
}


void SPWindowEffectItemUpgrade::Show(bool bWithChild /*= true*/)
{
	m_curSelect.reset();
	if( m_bUse == false )
		return;

	for(int i = 0; i < MAX_LEVELUP_SLOT; i++) 
	{
		m_pItem[i] = NULL;
		m_pIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	}	

	ClearAllSlotInterface();	
	m_pItemResult->SetClear();
	SetStatus(LEVELUP_STATUS_FIRST);	

	SPWindow::Show(bWithChild);
}


void SPWindowEffectItemUpgrade::Hide(bool bSendServer /*= true*/)
{	
	if(m_bShow) 
	{
		for(int i = 0; i < MAX_LEVELUP_SLOT; i++)
		{
			m_pItem[i] = NULL;
			m_pIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		}

		ClearAllSlotInterface();		
	}
	SPWindow::Hide(bSendServer);
}


void SPWindowEffectItemUpgrade::SetStatus(LEVELUP_STATUS iStatus)
{	
	m_iLevelUpStatus = iStatus;

	switch(m_iLevelUpStatus)
	{
		case LEVELUP_STATUS_FIRST:
			m_pCursorFirst->Show();
			m_pCursorSecond->Hide();
			//m_pToolTip->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);
			m_pToolTip->SetWindowText( "请放入要升级的勋章" );
			m_pOK->SetEnable(false);
			m_pIcon[1]->SPSendMessage(SPIM_SET_IMAGECLEAR);
			m_pIcon[2]->SPSendMessage(SPIM_SET_IMAGECLEAR);
			break;
		case LEVELUP_STATUS_SECOND:
			m_pCursorFirst->Hide();
			m_pCursorSecond->Show();
			//m_pToolTip->SPSendMessage(SPIM_SET_IMAGEINDEX, 1);
			m_pToolTip->SetWindowText( m_curSelect.strInfo_First.c_str() );
			m_pIcon[1]->SPSendMessage(SPIM_SET_IMAGECLEAR);
			if (m_curSelect.iIcon_First_icon > 0 && m_curSelect.iIcon_First_index > 0)
			{
				POINT pos;
				SPTexture *pTex =
					g_pItemCluster->GetItemTextureFromTextureInx( m_curSelect.iIcon_First_icon, m_curSelect.iIcon_First_index, pos);
				if (pTex)
					m_pIcon[1]->SetImage( pTex, pos.x, pos.y ); 
			}
			m_pOK->SetEnable(false);
			break;
		case LEVELUP_STATUS_READY:
			m_pOK->SetEnable(true);
			m_pIcon[2]->SPSendMessage(SPIM_SET_IMAGECLEAR);
			if (m_curSelect.iIcon_Second_icon > 0 && m_curSelect.iIcon_Second_index > 0)
			{
				POINT pos;
				SPTexture *pTex =
					g_pItemCluster->GetItemTextureFromTextureInx( m_curSelect.iIcon_Second_icon, m_curSelect.iIcon_Second_index, pos);
				if (pTex)
					m_pIcon[2]->SetImage( pTex, pos.x, pos.y ); 
			}
			break;
		case LEVELUP_STATUS_RESULT:
			m_pCursorFirst->Hide();
			m_pCursorSecond->Hide();			
			m_pOK->SetEnable(false);
			break;
		default:
			m_pCursorFirst->Hide();
			m_pCursorSecond->Hide();
			//m_pToolTip->SPSendMessage(SPIM_SET_IMAGECLEAR);
			m_pOK->SetEnable(false);
			break;
	}

	m_iFrame = 0;
	m_fCursorAccmulateTime = 0.0f;
	m_iCursorDirection = 0;	
}


void SPWindowEffectItemUpgrade::ClearAllSlotInterface()
{
	for(int i = 0; i < MAX_LEVELUP_SLOT; i++) 
	{
		ClearSlotInterface(i);
	}
}


void SPWindowEffectItemUpgrade::ClearSlotInterface(int iIndex)
{
	m_pIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);		
	m_bItemAble[iIndex] = true;
	m_iItemMark[iIndex] = ICON_MARK_NULL;
	m_iStack[iIndex] = 0;
	m_pItem[iIndex] = NULL;
	m_SelectItem[iIndex].Clear();	

	if(m_bShow) 
	{
		//
		SPPlayerInvenArchive* pInven = NULL;
		if(g_pGOBManager->GetLocalPlayer())
		{
			pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
			pInven->ClearBlockItem(iIndex);
		}
	}	
}


void SPWindowEffectItemUpgrade::SetSlotInterface(int iIndex, SPItem* pItem)
{
	POINT ptSrcPos;

	SPTexture* pTexture = g_pItemCluster->GetItemTexture(pItem->GetItemStatus()->GetItemID(), ptSrcPos);

	m_pIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pIcon[iIndex]->Hide();
	m_pIcon[iIndex]->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
	m_pIcon[iIndex]->Show();

	m_iStack[iIndex] = pItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
	m_bItemAble[iIndex] = g_pCheckManager->CheckItemStatus(pItem->GetItemStatus()->GetItemID());
	m_bItemAble[iIndex] &= pItem->GetItemStatus()->IsTimeAvailable();
	m_iItemMark[iIndex] = g_pItemCluster->GetItemIconMark(pItem->GetItemStatus()->GetItemID());	
}


bool SPWindowEffectItemUpgrade::SetFirstItem()               //设置第一个物品
{	
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
	{
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	}
	else
	{
		return false;
	}

	if(pInven->m_bPickup == false)						//是否能被PickUp	
	{
		//ResetItemControl();
		return false;
	}

	if(m_iLevelUpStatus != LEVELUP_STATUS_FIRST)		//判断是否为第一格的状态
	{
		return false;
	}

	m_SelectItem[0] = pInven->GetPickupItem();			//得到物品在那个container的第几个格子
	m_pItem[0] = pInven->GetInventoryItem(m_SelectItem[0].m_iContainer, m_SelectItem[0].m_iSlotIndex);//根据container和格子找到具体物品

	// 只有物品窗物品才可登录
	if( m_SelectItem[0].GetContainerType() >= INVENTORY_HOUSING )
	{
		//DXUTOutputDebugString("ItemSell Equip, Costume... Inven.. \n")	;
		if(g_pGOBManager->GetLocalPlayer())
		{
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		}
		if(g_pResourceManager->GetGlobalString(13000002))
		{
			std::string strMsg = g_pResourceManager->GetGlobalString(13000002);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		return false;
	}

	if(m_pItem[0] == NULL || m_pItem[0]->GetItemStatus() == NULL || m_pItem[0]->GetItemStatus()->GetItemID() == 0) 
	{
		return false;
	}

	// 判断是否是添加时间的物品
	if(isItemUpgradable(m_pItem[0]->GetItemStatus()->GetItemID(), m_curSelect ) == false) 
	{
		char bufMsg[] = "不是合法的特效物品";
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)bufMsg );
		return false;
	}
	
	SetSlotInterface(0, m_pItem[0]);
	pInven->SetBlockItem(0, m_SelectItem[0].GetContainerType(), m_SelectItem[0].GetSlotIndex());	//将物品栏的物品设置为灰色

	SetStatus(LEVELUP_STATUS_SECOND);			//第一格设置成功,将状态设置为第二格
	return true;
}


bool SPWindowEffectItemUpgrade::SetSecondItem()
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
	{
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	}
	else
	{
		return false;
	}
	
	//物品能否被PickUp
	if(pInven->m_bPickup == false) 
	{
		//ResetItemControl();
		return false;
	}
	
	//判断状态是否为second
	if(m_iLevelUpStatus != LEVELUP_STATUS_SECOND)
	{
		return false;
	}

	//判断物品状态和物品ID,确定物品存在
	if(m_pItem[0] == NULL || m_pItem[0]->GetItemStatus() == NULL || m_pItem[0]->GetItemStatus()->GetItemID() == 0)
	{
		return false;
	}
	
	//得到具体的物品
	m_SelectItem[1] = pInven->GetPickupItem();
	m_pItem[1] = pInven->GetInventoryItem(m_SelectItem[1].m_iContainer, m_SelectItem[1].m_iSlotIndex);

	// 只有物品窗物品才可登录
	if( m_SelectItem[1].GetContainerType() >= INVENTORY_HOUSING ) {
		//DXUTOutputDebugString("ItemSell Equip, Costume... Inven.. \n")	;
		if(g_pGOBManager->GetLocalPlayer())
		{
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		}
		if(g_pResourceManager->GetGlobalString(13000101))
		{
			std::string strMsg = g_pResourceManager->GetGlobalString(13000101);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}

	//判断物品是否存在
	if(m_pItem[1] == NULL || m_pItem[1]->GetItemStatus() == NULL || m_pItem[1]->GetItemStatus()->GetItemID() == 0) 
	{
		return false;
	}

	// is material suitable
	int iItemID = m_pItem[0]->GetItemStatus()->GetItemID();
	int iMaterialID = m_pItem[1]->GetItemStatus()->GetItemID();
	if (isItemSuitableMaterial( iItemID, iMaterialID, m_curSelect ) == false)
	{
		char bufMsg[] = "升级材料不正确"; 
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)bufMsg );
		return false;
	}

	// 设置图片
	SetSlotInterface(1, m_pItem[1]);
	pInven->SetBlockItem(1, m_SelectItem[1].GetContainerType(), m_SelectItem[1].GetSlotIndex());//将物品栏物品设置为不可选
	SetSlotInterface(2, m_pItemResult);															//设置第三张图片

	//促澜窜拌肺
	SetStatus(LEVELUP_STATUS_READY);															//将状态设置为READY
	char msgBuf[512] = {0};
	_snprintf( msgBuf, 511, "%s%d%s", "升级成功率为", m_curSelect.iChance, "%" );
	m_pToolTip->SetWindowText( msgBuf );
	
	return true;
}

//发送新消息
bool SPWindowEffectItemUpgrade::SendAddTime()
{
	//ITEM_CS_ADDTIME,
	// int				iItemAddTimeID;				// ITEM_ADD_TIME.LDT::PrimaryKey
	//----------------------------------------------------------
	// CONTAINER_TYPE	iMainContainerType;			// 皋牢
	// UINT8			iMainContainerSlot;			// 皋牢
	// ITEMNO			iMainItemNo;				// 皋牢

	// CONTAINER_TYPE	iMaterialContainerType;		// 犁丰 酒捞袍 困摹  
	// UINT8			iMaterialContainerSlot;		// 犁丰 酒捞袍 困摹
	// ITEMNO			iMaterialItemNo;			// 犁丰 酒捞袍 锅龋
	// UINT8			iMaterialItemCount;			// 犁丰 酒捞袍 夸备肮荐.  -- 曼炼侩
	//-------------------------------------------------------------

	if(m_pItem[0] == NULL || m_pItem[0]->GetItemStatus() == NULL || m_pItem[0]->GetItemStatus()->GetItemID() == 0) {
		return false;
	}

	if(m_pItem[1] == NULL || m_pItem[1]->GetItemStatus() == NULL || m_pItem[1]->GetItemStatus()->GetItemID() == 0) {
		return false;
	}

	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_ADDTIME);
	//UINT8 byType = 1;
	//Packet.AddUINT8(byType);
	Packet.AddUINT32(m_pItem[0]->GetItemStatus()->GetItemID());
	Packet.AddUINT32(m_SelectItem[0].GetContainerType());
	Packet.AddUINT8(m_SelectItem[0].GetSlotIndex() + 1);
	Packet.AddUINT64(*(ITEMNO*)m_pItem[0]->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO ));

	Packet.AddUINT32(m_SelectItem[1].GetContainerType());
	Packet.AddUINT8(m_SelectItem[1].GetSlotIndex() + 1);
	Packet.AddUINT64(*(ITEMNO*)m_pItem[1]->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO ));
	Packet.AddUINT8(m_pItem[1]->GetItemStatus()->GetStackCount());	

	g_pNetworkManager->SPPerformMessage(ITEM_CS_ADDTIME, 0, (LPARAM)&Packet);
	return true;
}




SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowEffectItemUpgrade )
if (Spim == SPIM_LBUTTON_UP)
{
	if (iID == 805908520)
		iID = iID;
	else
	{
		iID = iID;
		SPWindow *wnd = Find( iID, true );
		int a = 0;
	}
}
SPIMESSAGE_COMMAND(SPIM_ITEM_UPGRADE_RESULT	,	OnResult	)

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_TOP		, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_CENTER		, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_BOTTOM		, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_TITLE		, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_SKIN		, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_LINE1		, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_LINE2		, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_CURSOR1	, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_CURSOR2	, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_TOOLTIP	, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_PLUS		, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_EQUAL		, OnLButtonUp)		//酒捞袍酒捞能 府目滚
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_EFFECTITEM_UPGRADE_QUESTION, OnLButtonUp)		//酒捞袍酒捞能 府目滚

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_EFFECTITEM_UPGRADE_CLOSE	,		OnClose		)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_EFFECTITEM_UPGRADE_OK	,		OnOK		)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_EFFECTITEM_UPGRADE_CANCEL,		OnCancel	)

SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP,		WIID_EFFECTITEM_UPGRADE_SLOTBUTTON,	 WIID_EFFECTITEM_UPGRADE_SLOTBUTTON + MAX_LEVELUP_SLOT,	OnItemClick)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_EFFECTITEM_UPGRADE_SLOTBUTTON,	 WIID_EFFECTITEM_UPGRADE_SLOTBUTTON + MAX_LEVELUP_SLOT,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_EFFECTITEM_UPGRADE_SLOTBUTTON,	 WIID_EFFECTITEM_UPGRADE_SLOTBUTTON + MAX_LEVELUP_SLOT,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,		WIID_EFFECTITEM_UPGRADE_SLOTBUTTON,	 WIID_EFFECTITEM_UPGRADE_SLOTBUTTON + MAX_LEVELUP_SLOT,	OnToolTipMove)

SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_EFFECTITEM_UPGRADE_QUESTION,	 WIID_EFFECTITEM_UPGRADE_QUESTION,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_EFFECTITEM_UPGRADE_QUESTION,	 WIID_EFFECTITEM_UPGRADE_QUESTION,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,		WIID_EFFECTITEM_UPGRADE_QUESTION,	 WIID_EFFECTITEM_UPGRADE_QUESTION,	OnToolTipMove)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowEffectItemUpgrade::OnResult(WPARAM wParam, LPARAM lParam)
{
	UINT8				uiError			=	(UINT8)lParam;
	GLOBAL_STRING_ID	eGlobalStringID	=	(GLOBAL_STRING_ID)wParam;

	if( eGlobalStringID > 0 )
	{
		if( g_pInterfaceManager == NULL || eGlobalStringID == NULL )
			return 0;

		D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)eGlobalStringID , (LPARAM)&color );
	}

	SPPlayerInvenArchive* pInven = NULL; 
	ClearSlotInterface(0);
	ClearSlotInterface(1);
	ClearSlotInterface(2);
	SetStatus(LEVELUP_STATUS_FIRST);
	m_pItemResult->SetClear();
	return 1;
}


int SPWindowEffectItemUpgrade::OnClose(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);	

	////
	//SPPlayerInvenArchive* pInven = NULL;
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	//	pInven->ClearUserShopItem(0);
	//	pInven->ClearUserShopItem(1);
	//	pInven->ClearUserShopItem(2);
	//}

	Hide();
	return 1;
}


int SPWindowEffectItemUpgrade::OnOK(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	if(m_iLevelUpStatus != LEVELUP_STATUS_READY) 
		return 1;

	if (doUpgrade() == false)
		return 0;

	//if(SendAddTime() == false)
	//{
	//}
	return 1;
}


int SPWindowEffectItemUpgrade::OnCancel(WPARAM wParam, LPARAM lParam)
{
	/*
	if(g_pGOBManager->GetLocalPlayer()) 
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer()) {
	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	pInven->ClearUserShopItem(0);
	pInven->ClearUserShopItem(1);
	pInven->ClearUserShopItem(2);
	}

	Hide();
	*/

	OnClose(wParam, lParam);
	return 1;
}


int SPWindowEffectItemUpgrade::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	return 1;
}


int SPWindowEffectItemUpgrade::OnItemClick(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectIndex = iUiID - WIID_EFFECTITEM_UPGRADE_SLOTBUTTON;	

	if(iSelectIndex == 0) {
		//捞固 酒捞袍捞 乐绰 版快
		if(m_pItem[0] && m_pItem[0]->GetItemStatus() && m_pItem[0]->GetItemStatus()->GetItemID()) {
			ClearSlotInterface(0);
			ClearSlotInterface(1);
			ClearSlotInterface(2);
			SetStatus(LEVELUP_STATUS_FIRST);

			////
			//SPPlayerInvenArchive* pInven = NULL;
			//if(g_pGOBManager->GetLocalPlayer()) {
			//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
			//	pInven->ClearUserShopItem(0);
			//	pInven->ClearUserShopItem(1);
			//	pInven->ClearUserShopItem(2);
			//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
			//}			
		}

		if(SetFirstItem() == false) {
			ClearSlotInterface(0);
			ClearSlotInterface(1);
			ClearSlotInterface(2);
			SetStatus(LEVELUP_STATUS_FIRST);
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
			return 1;
		}
	}
	else if(iSelectIndex == 1) {
		//捞固 酒捞袍捞 乐绰 版快
		if(m_pItem[1] && m_pItem[1]->GetItemStatus() && m_pItem[1]->GetItemStatus()->GetItemID()) {
			ClearSlotInterface(1);
			ClearSlotInterface(2);
			SetStatus(LEVELUP_STATUS_SECOND);
			////
			//SPPlayerInvenArchive* pInven = NULL;
			//if(g_pGOBManager->GetLocalPlayer()) {
			//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();				
			//	pInven->ClearUserShopItem(1);
			//	pInven->ClearUserShopItem(2);
			//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
			//}			
		}

		if(SetSecondItem() == false) {
			ClearSlotInterface(1);
			ClearSlotInterface(2);
			if (m_pItem[0])
				SetStatus( LEVELUP_STATUS_SECOND );
			else
				SetStatus( LEVELUP_STATUS_FIRST );
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
			return 1;
		}
	}
	else {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	}

	return 1;
}


int SPWindowEffectItemUpgrade::OnToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectIndex = iUiID - WIID_EFFECTITEM_UPGRADE_SLOTBUTTON;	
	SPWindow* pWindow = NULL;
	//SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);
	if(iSelectIndex == 2 || WIID_EFFECTITEM_UPGRADE_QUESTION == iUiID) {
		if(LEVELUP_STATUS_READY == m_iLevelUpStatus)
		{
			TOOLTIP_NORMAL_DATA stToolTipData;
			stToolTipData.m_iContentsCount = 0;
			stToolTipData.m_strName = "生成物";
			stToolTipData.m_strDesc = m_curSelect.strInfo_Second;
			stToolTipData.m_cDesc = D3DXCOLOR(0.192f, 0.2980f, 0.38f, 1.0f);
			stToolTipData.m_cName = D3DXCOLOR(0.192f, 0.2980f, 0.38f, 1.0f);

			pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)(&stToolTipData), SPWindowToolTip::TOOLTIP_SHOW_TYPE_NORMAL);
		}
	}
	else {
		if(m_pItem[iSelectIndex] && m_pItem[iSelectIndex]->GetItemStatus() && m_pItem[iSelectIndex]->GetItemStatus()->GetItemID()) {
			pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)m_pItem[iSelectIndex]->GetItemStatus(), lParam);
		}
	}

	if(pWindow)
		pWindow->Show();

	return 1;
}


int SPWindowEffectItemUpgrade::OnToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectIndex = iUiID - WIID_EFFECTITEM_UPGRADE_SLOTBUTTON;	
	SPWindow* pWindow = NULL;
	//SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);
	if(iSelectIndex == 2 || WIID_EFFECTITEM_UPGRADE_QUESTION == iUiID) {
		if(LEVELUP_STATUS_READY == m_iLevelUpStatus)
		{
			TOOLTIP_NORMAL_DATA stToolTipData;
			stToolTipData.m_iContentsCount = 0;
			stToolTipData.m_strName = "生成物";
			stToolTipData.m_strDesc = m_curSelect.strInfo_Second;
			stToolTipData.m_cDesc = D3DXCOLOR(0.192f, 0.2980f, 0.38f, 1.0f);
			stToolTipData.m_cName = D3DXCOLOR(0.192f, 0.2980f, 0.38f, 1.0f);

			pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)(&stToolTipData), SPWindowToolTip::TOOLTIP_SHOW_TYPE_NORMAL);
		}
	}
	else {
		if(m_pItem[iSelectIndex] && m_pItem[iSelectIndex]->GetItemStatus() && m_pItem[iSelectIndex]->GetItemStatus()->GetItemID()) {
			pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)m_pItem[iSelectIndex]->GetItemStatus(), lParam);
		}
	}

	if(pWindow)
		pWindow->Show();

	return 1;
}


int SPWindowEffectItemUpgrade::OnToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}

//--------------------------------------------------
// added by jmulro, for ldt and more things
//--------------------------------------------------
bool SPWindowEffectItemUpgrade::initLDT( void )
{
	SPLDTFile	*pFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_EFFECT_ITEM_UPGRADE, &pFile );
	if (!pFile)
	{
		assert( false && "effect item upgrade ldt read failed." );
		return false;
	}

	m_vecEffectCfg.clear();
	LDT_Field field;
	EffectUpdateCfg	data;

	int iRecordCount = pFile->GetItemCount();
	int id = 0;
	for (int idx = 0; idx < iRecordCount; ++idx)
	{
		id = pFile->GetPrimaryKey( idx );
		if (id == 0)
			continue;

		pFile->GetFieldFromLable( id, "_UpgradeID", field );
		data.iUpgradeID = field.uData.lValue;

		pFile->GetFieldFromLable( id, "_ID", field );
		data.iItemID = field.uData.lValue;

		pFile->GetFieldFromLable( id, "_MaterialID", field );
		data.iMaterialID = field.uData.lValue;

		pFile->GetFieldFromLable( id, "_IconOne", field );
		//data.iIcon_First_icon = 10001205;
		data.iIcon_First_icon = field.uData.lValue;

		pFile->GetFieldFromLable( id, "_IconTwo", field );
		data.iIcon_Second_icon = field.uData.lValue;

		pFile->GetFieldFromLable( id, "_IconOnecode", field );
		data.iIcon_First_index = field.uData.lValue;

		pFile->GetFieldFromLable( id, "_IconTwocode", field );
		data.iIcon_Second_index = field.uData.lValue;

		pFile->GetFieldFromLable( id, "_Chance", field );
		data.iChance = field.uData.lValue;

		pFile->GetFieldFromLable( id, "_InfoOne", field );
		if (field.uData.pValue)
			data.strInfo_First = field.uData.pValue;
		else
			data.strInfo_First = "请放入物品";

		pFile->GetFieldFromLable( id, "_InfoTwo", field );
		if (field.uData.pValue)
			data.strInfo_Second = field.uData.pValue;
		else
			data.strInfo_First = "请放入物品";

        m_vecEffectCfg.push_back( data );
		data.reset();
	}

	SAFE_RELEASE( pFile );
	return true;
}
//--------------------------------------------------
class _findUpgradable
{
public:
	int m_iItemID;
	_findUpgradable( int iItemID )
	{
		m_iItemID = iItemID;
	}
	bool operator()( const EffectUpdateCfg &data )
	{
		if (data.iItemID == m_iItemID)
			return true;
		else
			return false;
	}
};
bool SPWindowEffectItemUpgrade::isItemUpgradable( int iItemID, EffectUpdateCfg &cfg )
{
	if (iItemID <= 0)
		return false;

	vector<EffectUpdateCfg>::iterator iter =
		find_if( m_vecEffectCfg.begin(), m_vecEffectCfg.end(), _findUpgradable( iItemID ));
	if (iter != m_vecEffectCfg.end())
	{
		cfg = *iter;
		return true;
	}
	else
		return false;
}
//--------------------------------------------------
class _findSuitableMaterial
{
public:
	int m_iItemID;
	int m_iMaterialID;
	_findSuitableMaterial( int iItemID, int iMaterialID )
	{
		m_iItemID = iItemID;
		m_iMaterialID = iMaterialID;
	}
	bool operator()( const EffectUpdateCfg &data )
	{
		if (data.iItemID == m_iItemID
			&&
			data.iMaterialID == m_iMaterialID)
			return true;
		else
			return false;
	}
};
bool SPWindowEffectItemUpgrade::isItemSuitableMaterial(int iItemID, int iMaterialID, EffectUpdateCfg &cfg)
{
	if (iItemID <= 0 || iMaterialID <= 0)
		return false;

	vector< EffectUpdateCfg>::iterator iter = 
		find_if( m_vecEffectCfg.begin(), m_vecEffectCfg.end(), _findSuitableMaterial( iItemID, iMaterialID ));
	if (iter != m_vecEffectCfg.end())
	{
		cfg = *iter;
		return true;
	}
	else
		return false;

	return true;
}
//--------------------------------------------------
bool SPWindowEffectItemUpgrade::doUpgrade(void)
{	
	//ITEM_CS_EFFECTUPGRADE,
	// int				iUpgradeID;					// 
	// CONTAINER_TYPE	iMainContainerType;			// 
	// UINT8			iMainContainerSlot;			// 
	// ITEMNO			iMainItemNo;				// 
	// MATERIAL_DATA	MaterialData;				//
	// ITEMNO			iResultItemID;				//

	if(m_pItem[0] == NULL || m_pItem[0]->GetItemStatus() == NULL || m_pItem[0]->GetItemStatus()->GetItemID() == 0) {
		return false;
	}

	if(m_pItem[1] == NULL || m_pItem[1]->GetItemStatus() == NULL || m_pItem[1]->GetItemStatus()->GetItemID() == 0) {
		return false;
	}


	MATERIAL_DATA	stMaterialData;
	stMaterialData.iContainerType	=	m_SelectItem[1].GetContainerType();
	stMaterialData.iContainerSlot	=	m_SelectItem[1].GetSlotIndex() + 1;
	stMaterialData.iItemCount		=	1;
	stMaterialData.iItemNo			=	*((ITEMNO*)m_pItem[1]->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO ));
	
	CPacket Packet;
	Packet.Add( (uint32)ITEM_CS_EFFECTUPGRADE );
	Packet.AddUINT32( m_curSelect.iUpgradeID );	// TODO: iUpgrageID;
	Packet.AddUINT32(m_SelectItem[0].GetContainerType());
	Packet.AddUINT8(m_SelectItem[0].GetSlotIndex() + 1);
	Packet.AddUINT64(*(ITEMNO*)m_pItem[0]->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO ));
	Packet.AddData( &stMaterialData , sizeof( MATERIAL_DATA ));
	Packet.AddUINT64(*(ITEMNO*)m_pItem[1]->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO ));
	g_pNetworkManager->GetPacketHandler()->PostMsg( &Packet );
	return true;
}
//--------------------------------------------------
int SPWindowEffectItemUpgrade::OnUpgradeAck(WPARAM wParam, LPARAM lParam)
{
	//ITEM_SC_EFFECTUPGRADE,
	// GU_ID			iGUID;					//
	// UIN8				iUpgraded;				//
	// GLOBAL_STRING_ID	iResultError;			//
	// int				iUpgradeID;				//

	return 1;
}
//--------------------------------------------------


