#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowChannel.h"

#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemQuest.h"
#include "SPWindowTreeBox.h"


#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPResourceManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

extern WORLD_ID g_uiWorldID;
using namespace std;
//------------------------------------------------------------------------------------
SPWindowChannel::SPWindowChannel(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowChannel::SPWindowChannel(INSTANCE_ID InstanceID,
								 int iX, int iY, int iCX, int iCY, SPWindow* pParent)
								 : SPWindow(WNDID_INVALID, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowChannel::~SPWindowChannel()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowChannel::Clean()
{
	SPWindow::Clean();
	m_vChannelList.clear();
}

//------------------------------------------------------------------------------------
void SPWindowChannel::Init()
{
	SPWindowStatic* pkSkinWindow, *pkChildWindow;
	SPWindowButton* pkButton;

	pkSkinWindow = new SPWindowStatic(WIID_CHANNEL_SKIN_UPPER, 0, 0, 208, 4, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1, 95);

	pkSkinWindow = new SPWindowStatic(WIID_CHANNEL_SKIN_LOWER, 0, 254, 208, 4, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1, 105);

	pkSkinWindow = new SPWindowStatic(WIID_CHANNEL_SKIN_MIDDLE, 0, 4, 208, 250, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1, 101);
	pkSkinWindow->SetSrcSize(208, 2);

	pkChildWindow = new SPWindowStatic(WIID_CHANNEL_TITLE_TEXT, 10, 3, 75, 15, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1, 321);

	pkButton = new SPWindowButton(WIID_CHANNEL_CLOSE, 187, 5, 12, 12, pkSkinWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 301, 321);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 314, 321);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 327, 321);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 340, 321);

	pkButton = new SPWindowButton(WIID_CHANNEL_OK, 130, 231, 68, 17, pkSkinWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 236, 241);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 236, 260);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 236, 279);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 236, 298);

	pkChildWindow = new SPWindowStatic(WIID_CHANNEL_SUB_BG, 2, 23, 204, 203, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 19, 53);
	pkChildWindow->SetSrcSize(4, 4);
	InitListBox(pkChildWindow);
	
	pkChildWindow = new SPWindowStatic(WIID_CHANNEL_SEPERATE_LINE1, 2, 22, 204, 1, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 24, 95);
	pkChildWindow->SetSrcSize(4, 1);

	pkChildWindow = new SPWindowStatic(WIID_CHANNEL_SEPERATE_LINE2, 2, 226, 204, 1, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 24, 95);
	pkChildWindow->SetSrcSize(4, 1);

	SetRect(&m_rcSrcStatus[0], 109, 358, 123, 372);
	SetRect(&m_rcSrcStatus[1], 94, 358, 108, 372);
	SetRect(&m_rcSrcStatus[2], 79, 358, 93, 372);
	SetRect(&m_rcSrcStatus[3], 109, 343, 123, 357);
}

//------------------------------------------------------------------------------------
void SPWindowChannel::InitListBox(SPWindow* pkParent)
{
	SPWindowTreeBox* pkTreeBox;

	pkTreeBox =	new SPWindowTreeBox(WIID_CHANNEL_LIST, 0, 0, 204, 203, pkParent);
	pkTreeBox->SetMargin(12, 8, 0, 0);
	pkTreeBox->UpdateBGPos();
	pkTreeBox->SetDoubleClick(true);

	pkTreeBox->SetScrollAutoHide(true);
	
	pkTreeBox->SetScrollUpSize( 12 , 11 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 460 , 386 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 473 , 386 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 486 , 386 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 499 , 386 );

	pkTreeBox->SetScrollDownSize( 12 , 11 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 460 , 428 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 473 , 428 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 486 , 428 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 499 , 428 );

	pkTreeBox->SetScrollSliderSize( 12 , 24 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 460 , 398 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 473 , 398 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 486 , 398 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 499 , 398 );

	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_ENABLE, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 460 , 423 , 472 , 427 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 473 , 423 , 485 , 427 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 486 , 423 , 498 , 427 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_DISABLE, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 499 , 423 , 511 , 427 );

	pkTreeBox->SetScrollMargin( 0 , 4 , 4 , 4 );
	pkTreeBox->UpdateScrollPos();

	pkTreeBox->Refresh();
}

//------------------------------------------------------------------------------------
void SPWindowChannel::Show(bool bWithChild)
{
	// 채널 목록 요청
	CPacket packet;
	packet.AddUINT32(WORLD_CS_CHANNELSTATUS);
	packet.AddUINT16(g_uiWorldID);

	SENDPACKET(packet);

	m_iCurServerID = -1;
	SPWindow::Show(bWithChild);
}

//------------------------------------------------------------------------------------
void SPWindowChannel::Hide(bool bSendServer)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide(bSendServer);
	ClearChannelList();
}

//------------------------------------------------------------------------------------
void SPWindowChannel::ClearChannelList()
{
	SPWindowTreeBox* pkTreeBox;
	pkTreeBox = (SPWindowTreeBox*)Find(WIID_CHANNEL_LIST, true);
	if( pkTreeBox )
		pkTreeBox->DelItemAll();

	m_iCurSelIndex = -1;
	m_vChannelList.clear();
}

//------------------------------------------------------------------------------------
void SPWindowChannel::Process(float fTime)
{
	SPWindow::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowChannel::Render(float fTime)
{
	SPWindow::Render(fTime);
}

//------------------------------------------------------------------------------------
// Message
int SPWindowChannel::OnCursorEnter( WPARAM, LPARAM )
{
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowChannel::OnCursorOut( WPARAM, LPARAM )
{
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowChannel::OnSetChannel(WPARAM wParam, LPARAM lParam)
{
	ClearChannelList();

	CPacket* pPacket = (CPacket*)lParam;

	UINT8 nChannels = 0;
	CHANNEL_INFO channelInfo[MAX_CHANNELS] = {0,};
	CHANNEL_INFO TempInfo;
	pPacket->ExtractUINT8(&nChannels);
	nChannels = max(0,nChannels);
	nChannels = min(nChannels,MAX_CHANNELS);
	pPacket->Extract(&channelInfo,sizeof(CHANNEL_INFO)*nChannels);

	SERVER_ID	uiServerID = 0;
	pPacket->ExtractUINT16(&uiServerID);

	m_iCurServerID = uiServerID;
	g_pNetworkManager->SetServerID(uiServerID);
	int iStatus, i, j;
	string strBuf;

	// sort
	for( i = 0; i < nChannels; i++ )
	{
		for( j = i + 1; j < nChannels; j++ )
		{
			bool bChange = false;
			if( channelInfo[i].channelID > channelInfo[j].channelID )
				bChange = true;

			if( bChange )
			{
				TempInfo = channelInfo[j];
				channelInfo[j] = channelInfo[i];
				channelInfo[i] = TempInfo;
			}
		}
	}

	SPWindowTreeBox* pkTreeBox = (SPWindowTreeBox*)Find(WIID_CHANNEL_LIST, true);
	if( pkTreeBox )
	{
		SPTreeBoxItemQuest*	pItem;
		for( i = 0; i < nChannels; i++ )
		{
			pItem = new SPTreeBoxItemQuest;
			pItem->SetWindow(this);
			pItem->SetReturnValue(i);
			pItem->SetSize(93, 12);
			pItem->SetTextMargin(28, 3);

			strBuf = channelInfo[i].name;
			iStatus = channelInfo[i].status;

			const char* pszStatus = NULL;
			switch( iStatus )
			{
			case 0:
				pszStatus = g_pResourceManager->GetGlobalString(70001);
				break;
			case 1:
				pszStatus = g_pResourceManager->GetGlobalString(70002);
				break;
			case 2:
				pszStatus = g_pResourceManager->GetGlobalString(70003);
				break;
			case 3:
				pszStatus = g_pResourceManager->GetGlobalString(70004);
				break;
			}

			if( pszStatus )
			{
				strBuf += " ";
				strBuf += pszStatus;
			}

			if( iStatus == 3 || uiServerID == channelInfo[i].channelID )
				pItem->SetStatus(DATA_STATUS_DISABLE);

			pItem->SetText(strBuf.c_str());
			for( j = 0 ; j < DATA_STRING_COUNT ; ++j )
			{
				if( j == (DATA_STRING_DISABLE * 2) || j == (DATA_STRING_DISABLE * 2) + 1 )
					pItem->SetTextColor(j, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				else
					pItem->SetTextColor(j, D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f));
			}
			
			pItem->SetQuestSize(163 , 18);
			pItem->SetQuestOffset(0, 0);

			if( iStatus >= 0 && iStatus < 4 )
			{
				pItem->SetTexture(SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2,
					"DATA/INTERFACE/CONCEPT/UI101.PNG" ,
					m_rcSrcStatus[iStatus].left, m_rcSrcStatus[iStatus].top,
					m_rcSrcStatus[iStatus].right, m_rcSrcStatus[iStatus].bottom);
				pItem->SetTexture(SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1,
					"DATA/INTERFACE/CONCEPT/UI101.PNG" ,
					m_rcSrcStatus[iStatus].left, m_rcSrcStatus[iStatus].top,
					m_rcSrcStatus[iStatus].right, m_rcSrcStatus[iStatus].bottom);
			}

			pItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_MOUSE_OVER * 2 ,
				"DATA/INTERFACE/CONCEPT/UI101.PNG" , 257 , 47 , 420 , 63 );

			pItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,			7 , 2 );
			pItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,		7 , 2 );

			pkTreeBox->AddItem(NULL, pItem);
			m_vChannelList.push_back(channelInfo[i]);
		}
		pkTreeBox->Refresh();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowChannel::OnExit(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowChannel::OnOK(WPARAM wParam, LPARAM lParam)
{
	if( m_iCurSelIndex < 0 )
		return 1;

	if( m_iCurSelIndex >= (int)m_vChannelList.size() )
		return 1;

	vector< CHANNEL_INFO >::iterator iter = m_vChannelList.begin();
	iter += m_iCurSelIndex;

	if( m_iCurServerID == (*iter).channelID )
		return 1;

	g_pSubGameManager->SPPerformMessage(SPGM_CHANNEL_CHANGE, (WPARAM)&(*iter));
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowChannel::OnListBoxLButton(WPARAM wParam, LPARAM lParam)
{
	TREEBOX_ITEM* pSelectQuestItem	=	(TREEBOX_ITEM*)wParam;

	if( pSelectQuestItem == NULL )
		return 1;

	SPTreeBoxItemQuest*	pQuestItem		=	(SPTreeBoxItemQuest*)( pSelectQuestItem->m_pItemBase );
	int iIndex = pQuestItem->GetReturnValue();
	m_iCurSelIndex = iIndex;

	return 1;

}

//------------------------------------------------------------------------------------
int SPWindowChannel::OnListBoxDBLButton(WPARAM wParam, LPARAM lParam)
{
	TREEBOX_ITEM* pSelectQuestItem	=	(TREEBOX_ITEM*)wParam;

	if( pSelectQuestItem == NULL )
		return 1;

	SPTreeBoxItemQuest*	pQuestItem		=	(SPTreeBoxItemQuest*)( pSelectQuestItem->m_pItemBase );
	int iIndex = pQuestItem->GetReturnValue();
	m_iCurSelIndex = iIndex;
	OnOK(0, 0);

	return 1;

}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowChannel)

SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
SPIMESSAGE_COMMAND(	SPIM_CHANNEL_SET,		OnSetChannel	)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,				WIID_CHANNEL_CLOSE,			OnExit	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,				WIID_CHANNEL_OK,			OnOK	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,				WIID_CHANNEL_LIST,			OnListBoxLButton )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_DBLCLICK,			WIID_CHANNEL_LIST,			OnListBoxDBLButton )

SPIMESSAGE_OBJ_MAP_END(SPWindow)