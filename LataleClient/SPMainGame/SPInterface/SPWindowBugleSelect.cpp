#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"
#include "SPWindowSlider.h"
#include "SPWindowGrid.h"
#include "SPWindowGridEx.h"

#include "SPUIUnit.h"
#include "SPUIChatMacro.h"
#include "SPUIUnitManager.h"
#include "SPDragnDropManager.h"

#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPAbuseFilter.h"
#include "SPNoticeBox.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"
#include "SPManager.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPLocalizeManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPWindowToolTip.h"

#include "SPWindowBugleSelect.h"
#include "SPWindowBugleMenu.h"
#include "SPWindowVisualBugle.h"

#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPCommandConvert.h"
#include "SPGOBManager.h"
#include "SPPlayerInvenArchive.h"

#include "SPImageProcessor.h"
#include "SPWindowPresentation.h"

//--------------------------------------------------
int SPWindowBugleSelect::m_iCurBugleInfoIdx;
vector< BugleInfo > SPWindowBugleSelect::m_vecBugleInfo;
vector< BugleInfo > SPWindowBugleSelect::m_vecGMBugleInfo;
const float PreviewAnim::SHOWTIMELIMIT = 0.3f;
PreviewAnim::~PreviewAnim( void )
{
	m_bShowed = true;;
	m_fAccuTime = 1000.0f;
}
void PreviewAnim::Release( void )
{
	if (m_pFadeIn)
		SAFE_DELETE( m_pFadeIn );

	if (m_pFadeOut)
		SAFE_DELETE( m_pFadeOut );
}
//--------------------------------------------------
SPWindowBugleSelect::SPWindowBugleSelect(WND_ID wndClassID, INSTANCE_ID instanceID )
: SPWindow( wndClassID, instanceID )
{
	Init();
}
//--------------------------------------------------
SPWindowBugleSelect::SPWindowBugleSelect(INSTANCE_ID instanceID, 
					int iX, int iY, int iCX, int iCY, 
					SPWindow *pParent /* = NULL  */)
: SPWindow( WNDID_BUGLE_SELECT, instanceID, iX, iY, iCX, iCY, pParent )
{
	Init();
}
//--------------------------------------------------
SPWindowBugleSelect::~SPWindowBugleSelect( void )
{
	Clean();
}
//--------------------------------------------------
void SPWindowBugleSelect::Init( void )
{
	//--------------------------------------------------
	m_pImgBugleIcon = NULL;
	m_pImgBuglePreview = NULL;
	m_pSlider = NULL;
	m_pbtnClose = NULL;
	m_pGrid = NULL;
	m_ptrPlayer = NULL;
	m_wndVisualBuglePreview = NULL;
	m_pImgBuglePreviewBG = NULL;
	m_pTop = NULL;
	m_FadeInBugleIdx = -1;
	m_FadeInBugleIdx = -1;
	m_vecBugleInfo.clear();
	m_vecGMBugleInfo.clear();
	m_vecPreviewAnims.clear();
	//--------------------------------------------------
	m_strToolTip = "[SPWindowBugleSelect]";

	if (false == _initGUI())
	{
		assert( false && "GUI init failed." );
	}

	ReloadBugleLDT( false );

	//--------------------------------------------------
	m_ptrPlayer = new SPPlayer;
	m_ptrPlayer->SetLobbyAvatar(LOBBY_INFO);
	m_ptrPlayer->Init();
	m_ptrPlayer->SetShowValue(false, false, false);
	buildLocalAvatar();
	m_ptrPlayer->SetAction(ACTION_NULL, true);
	m_ptrPlayer->SetPos(461.0f, 569.0f);
	//--------------------------------------------------
}

void SPWindowBugleSelect::buildLocalAvatar()
{
	if (!m_ptrPlayer)
		return;

	SPPlayer* pkLocalPlayer;
	if( m_ptrPlayer == NULL || (pkLocalPlayer = g_pGOBManager->GetLocalPlayer()) == NULL )		// Player 확인
		return;

	SPPlayerInvenArchive* pkInvenArchive = pkLocalPlayer->GetInvenArchive();	// Player의 Inven 가져옴
	if( pkInvenArchive == NULL )
		return;

	m_ptrPlayer->InitFigureInfo();
	m_ptrPlayer->SetPlayerClass(pkLocalPlayer->GetPlayerClass());

	SPItem* pkItem;
	SPItemStatus* pkItemStatus ;
	FIGURE_ITEM FigureItem;
	int iSize, i, j;
	CONTAINER_TYPE eFigureIndex[3+1] = {FIGURE_BASIC, FIGURE_EQUIP, FIGURE_FASHION, FIGURE_EFFECT};
	for( i = 0; i < 3+1; i++ )
	{
		iSize = pkInvenArchive->GetInventorySize(eFigureIndex[i]);
		for( j = 0; j < iSize; j++ )
		{
			pkItem = pkInvenArchive->GetInventoryItem(eFigureIndex[i], j);
			if( pkItem == NULL )
				continue;

			FigureItem.iItemID = pkItem->GetItemStatusValue(MV_ITEM_GETUID);
			if( FigureItem.iItemID == 0 )
				continue;

			pkItemStatus = pkItem->GetItemStatus();
			if( pkItemStatus == NULL || !pkItemStatus->IsTimeAvailable() )
				continue;

			FigureItem.SlotInfo.ContainerType = pkItem->GetContainerType();
			FigureItem.SlotInfo.SlotIndex = pkItem->GetSlotIndex() + 1;
			if (FigureItem.SlotInfo.SlotIndex == EQ_WEAPON2
				||
				FigureItem.SlotInfo.SlotIndex == EQ_BUCKLER2
				||
				FigureItem.SlotInfo.SlotIndex == EQ_WEAPON1
				||
				FigureItem.SlotInfo.SlotIndex == EQ_BUCKLER1)
			{
				continue;
			}

			m_ptrPlayer->ItemSetting(FigureItem);
		}
	}
}
//--------------------------------------------------
void SPWindowBugleSelect::Clean( void )
{
	if (m_ptrPlayer)
	{
		SAFE_DELETE( m_ptrPlayer );
	}

	for (int idx = 0; idx < (int)m_vecPreviewAnims.size(); ++idx)
	{
		PreviewAnim &anim = m_vecPreviewAnims[idx];
		anim.Release();
	}

	SPWindow::Clean();
}
//--------------------------------------------------
void SPWindowBugleSelect::Show( bool bWithChild )
{
	if (m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}

	m_pSlider->SPSendMessage( SPIM_POS_UPDATE );
	SPWindow::Show( bWithChild );
}
//--------------------------------------------------
void SPWindowBugleSelect::Hide( bool bSendServer )
{
	if (m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}

	SPWindow *ptrWnd =
		g_pInterfaceManager->FindWindow( WIID_VISUAL_BUGLE_PREVIEW );
	if (ptrWnd)
		ptrWnd->Hide();
    
	SPWindow::Hide( bSendServer );
}
//--------------------------------------------------
void SPWindowBugleSelect::Process( float fDeltaTime )
{

	if (!m_vecPreviewAnims.empty())
	{
		if ((int)m_vecPreviewAnims.size() > 1)
		{
			if (m_vecPreviewAnims.front().m_fAccuTime > PreviewAnim::SHOWTIMELIMIT)
			{
				m_vecPreviewAnims.front().Release();
				m_vecPreviewAnims.erase( m_vecPreviewAnims.begin() );
			}
		}
		
		SPImageProcessor	*pFadeIn = m_vecPreviewAnims.front().m_pFadeIn;
		SPImageProcessor	*pFadeOut = m_vecPreviewAnims.front().m_pFadeOut;

		if (pFadeIn)
			pFadeIn->Process( fDeltaTime );

		if (pFadeOut)
			pFadeOut->Process( fDeltaTime );

		m_vecPreviewAnims.front().m_fAccuTime += fDeltaTime;
	}

	if (!m_bShow)
	{
		return;
	}

    SPWindow::Process( fDeltaTime );
}
//--------------------------------------------------
void SPWindowBugleSelect::Render( float fDeltaTime )
{
	if (!m_bShow)
	{
		return;
	}

	SPWindow::RenderReverse( fDeltaTime );

	if (!m_vecPreviewAnims.empty())
	{
		SPImageProcessor	*pFadeIn = m_vecPreviewAnims.front().m_pFadeIn;
		SPImageProcessor	*pFadeOut = m_vecPreviewAnims.front().m_pFadeOut;
		
		if (pFadeIn)
			pFadeIn->Render();

		if (pFadeOut)
			pFadeOut->Render();

		if (m_pTop)
			m_pTop->Render(fDeltaTime);

		if (m_pImgBugleIcon)
			m_pImgBugleIcon->Render( fDeltaTime );

		if (m_pbtnClose)
			m_pbtnClose->Render( fDeltaTime );
	}
}
//--------------------------------------------------
void SPWindowBugleSelect::RefreshRelationCoord( void )
{
	SPWindow::RefreshRelationCoord();
	_reposOwnImage();
}
//--------------------------------------------------
void SPWindowBugleSelect::_reposOwnImage( void )
{
	// nothing.
}
//--------------------------------------------------
//--------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowBugleSelect )
	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP,		_onBugleItemsPageUp )
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN,	_onBugleItemsPageDown )
	SPIMESSAGE_CONTROL( SPIM_WHEEL_UP,			WIID_BUGLE_SELECT_GRID,			_onBugleItemsPageUp		)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN,		WIID_BUGLE_SELECT_GRID,			_onBugleItemsPageDown	)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_BUGLE_SELECT_SLIDER_UP,	_onBugleItemsPageUp )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_BUGLE_SELECT_SLIDER_DOWN,	_onBugleItemsPageDown )
	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP,			WIID_BUGLE_SELECT_SLIDER,		_onSliderUp )
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN,		WIID_BUGLE_SELECT_SLIDER,		_onSliderDown )
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP,		WIID_BUGLE_SELECT_SLIDER,		_onSliderUp )
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN,	WIID_BUGLE_SELECT_SLIDER,		_onSliderDown )
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE,		WIID_BUGLE_SELECT_SLIDER,		_onSliderPageNum )

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_BUGLE_SELECT_CLOSE,		_onClose )
	
    SPIMESSAGE_CONTROL( SPIM_TOOLTIP_ENTER,		WIID_BUGLE_SELECT_GRID,			_onToolTipEnter )
	SPIMESSAGE_CONTROL( SPIM_TOOLTIP_OUT,		WIID_BUGLE_SELECT_GRID,			_onToolTipOut )
	SPIMESSAGE_CONTROL( SPIM_TOOLTIP_MOVE,		WIID_BUGLE_SELECT_GRID,			_onToolTipMove )

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DBLCLICK,	WIID_BUGLE_SELECT_GRID,			_onBugleSelect )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_BUGLE_SELECT_GRID,			_onBuglePreview )
SPIMESSAGE_OBJ_MAP_END( SPWindow )
//--------------------------------------------------
//--------------------------------------------------
int SPWindowBugleSelect::_onBugleItemsPageUp( WPARAM wParam, LPARAM lParam )
{
	if (m_pSlider)
		m_pSlider->SPSendMessage( SPIM_SLIDER_UP );

	return 1;
}
int SPWindowBugleSelect::_onBugleItemsPageDown(WPARAM wParam, LPARAM lParam)
{
	if (m_pSlider)
		m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );

	return 1;
}
//--------------------------------------------------
int SPWindowBugleSelect::_onSliderUp(WPARAM wParam, LPARAM lParam)
{
	if (m_pSlider)
	{
		m_pGrid->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
	}

	if (m_pGrid)
		m_pGrid->ScrollUp();

	g_pInterfaceManager->SetFocusWindow( NULL );
	return 1;
}
//--------------------------------------------------
int SPWindowBugleSelect::_onSliderDown(WPARAM wParam, LPARAM lParam)
{
	if (m_pSlider)
	{
		m_pGrid->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
	}

	if (m_pGrid)
		m_pGrid->ScrollDown();

	g_pInterfaceManager->SetFocusWindow( NULL );
	return 1;
}
//--------------------------------------------------
int SPWindowBugleSelect::_onSlider(WPARAM wParam, LPARAM lParam)
{
	// update show.

	return 1;
}
int SPWindowBugleSelect::_onSliderPageNum(WPARAM wParam, LPARAM lParam)
{
	if (!m_pGrid)
		return 0;

	int iCurPage = (int)wParam;
	int iOldPage = (int)lParam;
	int iChangedPage = iCurPage - iOldPage;
	if (0 == iChangedPage)
		return 0;

	if (iChangedPage < 0)
	{
		m_pGrid->ScrollUp( -iChangedPage );
	}
	else
	{
		m_pGrid->ScrollDown( iChangedPage );
	}

	return 1;
}
//--------------------------------------------------
//--------------------------------------------------
int SPWindowBugleSelect::_onClose(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}
//--------------------------------------------------
int SPWindowBugleSelect::_onBuglePreview(WPARAM wParam , LPARAM lParam)
{
	if (!m_pImgBugleIcon || !m_pImgBuglePreview)
		return 0;

	int idx = (int)lParam;
	if (idx == -1)
		return 0;
	//if (idx == m_FadeInBugleIdx)
		//return 0;	// save idx preview

	if ((int)m_vecBugleInfo.size() > idx)
	{
		m_FadeOutBugleIdx = m_FadeInBugleIdx;
		m_FadeInBugleIdx = idx;

		BugleInfo &info = m_vecBugleInfo[idx];
		POINT ptIconSrcPos;
		const char *texstr =
			g_pItemCluster->GetItemTextureStr( info.m_iItemShopID, ptIconSrcPos );

		m_pImgBugleIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pImgBugleIcon->SetImage( texstr, ptIconSrcPos.x, ptIconSrcPos.y );

		//m_pImgBuglePreview->SPSendMessage(SPIM_SET_IMAGECLEAR);
		//m_pImgBuglePreview->SetImage( info.m_strPreviewFilename.c_str(), 
			//info.m_iPreviewX, info.m_iPreviewY );

		static RECT RECT_FADEOUT = { 175, 422, 175, 400 };
		static RECT RECT_FADEIN = { 175, 436, 175, 422 };

		RECT FadeOutsrcRect;
		RECT FadeInSrcRect;
		SPImageProcessor *AnimFadeIn = NULL;
		SPImageProcessor *AnimfadeOut = NULL;

		if (m_FadeOutBugleIdx >= 0 && m_FadeOutBugleIdx < (int)m_vecBugleInfo.size())
		{/* FadeOut */
			BugleInfo &fadeOutInfo = m_vecBugleInfo[m_FadeOutBugleIdx];
			FadeOutsrcRect.left = fadeOutInfo.m_iPreviewX;
			FadeOutsrcRect.top = fadeOutInfo.m_iPreviewY;
			FadeOutsrcRect.right = fadeOutInfo.m_iPreviewX + fadeOutInfo.m_iPreviewSizeX;
			FadeOutsrcRect.bottom = fadeOutInfo.m_iPreviewY + fadeOutInfo.m_iPreviewSizeY;

			AnimfadeOut = new SPWindowPresentation( WIID_PREVIEW_FADEOUT,
				fadeOutInfo.m_strPreviewFilename.c_str(), FadeOutsrcRect, RECT_FADEOUT.left, RECT_FADEOUT.top,
				RECT_FADEOUT.right, RECT_FADEOUT.bottom, 0.3, false );
		}

		if (1)
		{/* FadeIn */
			BugleInfo &fadeInInfo = m_vecBugleInfo[m_FadeInBugleIdx];
			FadeInSrcRect.left = fadeInInfo.m_iPreviewX;
			FadeInSrcRect.top = fadeInInfo.m_iPreviewY;
			FadeInSrcRect.right = fadeInInfo.m_iPreviewX + fadeInInfo.m_iPreviewSizeX;
			FadeInSrcRect.bottom = fadeInInfo.m_iPreviewY + fadeInInfo.m_iPreviewSizeY;

			AnimFadeIn = new SPWindowPresentation( WIID_PREVIEW_FADEIN,
				fadeInInfo.m_strPreviewFilename.c_str(), FadeInSrcRect, RECT_FADEIN.left, RECT_FADEIN.top,
				RECT_FADEIN.right, RECT_FADEIN.bottom, 0.3, true );

		}

		PreviewAnim preAnim;
		preAnim.m_pFadeIn = AnimFadeIn;
		preAnim.m_pFadeOut = AnimfadeOut;
		m_vecPreviewAnims.push_back( preAnim );

		m_pImgBugleIcon->Show();
		m_pImgBuglePreview->Show();

		//--------------------------------------------------
		m_wndVisualBuglePreview = 
			(SPWindowVisualBugle *)g_pInterfaceManager->FindWindow( WIID_VISUAL_BUGLE_PREVIEW );
		if (BUGLE_CHANNELBG_VISIBLE == info.m_channelBgType)
		{/* Visual Bugle Type */
			buildLocalAvatar();
			m_wndVisualBuglePreview->ShowVisualBugleImme( m_ptrPlayer,
				info.m_strName, info, true );
		}
		else
		{
			m_wndVisualBuglePreview->UnshowImme();
		}
		//--------------------------------------------------
	}
	return 1;
}
//--------------------------------------------------
int SPWindowBugleSelect::_onBugleSelect(WPARAM wParam , LPARAM lParam)
{
	int idx = (int)lParam;
	if (idx == -1)
		return 0;

	if ((int)m_vecBugleInfo.size() > idx)
	{
		m_iCurBugleInfoIdx = idx;
		SPWindowBugleMenu	*pWnd = (SPWindowBugleMenu*)g_pInterfaceManager->FindWindow( WIID_BUGLE_MENU );
		if (!pWnd)
			return 0;

		Hide();
		pWnd->SetSlotBugleInfo( m_vecBugleInfo[m_iCurBugleInfoIdx] );
	}
	return 1;
}
//--------------------------------------------------
//--------------------------------------------------
bool SPWindowBugleSelect::_loadBugleLDT( bool bGM )
{
	SPLDTFile	*pFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_BUGLE_TYPE, &pFile );
	if (!pFile)
	{
		assert( false && "bugle ldt file load failed." );
		return false;
	}

	m_vecBugleInfo.clear();
	m_vecGMBugleInfo.clear();
	LDT_Field field;
	BugleInfo bugleInfo;
    
	int iRecordCount = pFile->GetItemCount();
	int iItemID = 0;
	for (int idx = 0; idx < iRecordCount; ++idx)
	{
		iItemID = pFile->GetPrimaryKey( idx );

		pFile->GetFieldFromLable( iItemID, "bugle_itemid", field );
		bugleInfo.m_iItemShopID = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_name", field );
		bugleInfo.m_strName = field.uData.pValue;

		pFile->GetFieldFromLable( iItemID, "bugle_icon_filename", field );
		bugleInfo.m_strIconFilename = field.uData.pValue;

		pFile->GetFieldFromLable( iItemID, "bugle_icon_x", field );
		bugleInfo.m_iIconX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_icon_y", field );
		bugleInfo.m_iIconY = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_icon_size_x", field );
		bugleInfo.m_iIconSizeX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_icon_size_y", field );
		bugleInfo.m_iIconSizeY = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_color_r", field );
		bugleInfo.m_iColorR = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_color_g", field );
		bugleInfo.m_iColorG = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_color_b", field );
		bugleInfo.m_iColorB = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_bg_filename", field );
		bugleInfo.m_strBgFilename = field.uData.pValue;

		pFile->GetFieldFromLable( iItemID, "bugle_bg_x", field );
		bugleInfo.m_iBgX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_bg_y", field );
		bugleInfo.m_iBgY = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_bg_size_x", field );
		bugleInfo.m_iBgSizeX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_bg_size_y", field );
		bugleInfo.m_iBgSizeY = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_preview_filename", field );
		bugleInfo.m_strPreviewFilename = field.uData.pValue;

		pFile->GetFieldFromLable( iItemID, "bugle_preview_x", field );
		bugleInfo.m_iPreviewX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_preview_y", field );
		bugleInfo.m_iPreviewY = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_preview_size_x", field );
		bugleInfo.m_iPreviewSizeX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_preview_size_y", field );
		bugleInfo.m_iPreviewSizeY = field.uData.lValue;

		// header icon
		pFile->GetFieldFromLable( iItemID, "bugle_header_icon_filename", field );
		bugleInfo.m_strBgHeader = field.uData.pValue;

		pFile->GetFieldFromLable( iItemID, "bugle_header_icon_x", field );
		bugleInfo.m_iBgHeaderX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_header_icon_y", field );
		bugleInfo.m_iBgHeaderY = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_header_icon_size_x", field );
		bugleInfo.m_iBgHeaderSizeX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_header_icon_size_y", field );
		bugleInfo.m_iBgHeaderSizeY = field.uData.lValue;

		// tail icon
		pFile->GetFieldFromLable( iItemID, "bugle_tail_icon_filename", field );
		bugleInfo.m_strBgTail = field.uData.pValue;

		pFile->GetFieldFromLable( iItemID, "bugle_tail_icon_x", field );
		bugleInfo.m_iBgTailX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_tail_icon_y", field );
		bugleInfo.m_iBgTailY = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_tail_icon_size_x", field );
		bugleInfo.m_iBgTailSizeX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "bugle_tail_icon_size_y", field );
		bugleInfo.m_iBgTailSizeY = field.uData.lValue;

		// bugle channel bg type
		pFile->GetFieldFromLable( iItemID, "bugle_type_client", field );
		bugleInfo.m_channelBgType = (BugleChannelBgType)field.uData.lValue;

		// visual resource config
		pFile->GetFieldFromLable( iItemID, "visual_filename", field );
		bugleInfo.m_strVisualFilename = field.uData.pValue;

		pFile->GetFieldFromLable( iItemID, "visual_x", field );
		bugleInfo.m_iVisualX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "visual_y", field );
		bugleInfo.m_iVisualY = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "visual_size_x", field );
		bugleInfo.m_iVisualSizeX = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "visual_size_y", field );
		bugleInfo.m_iVisualSizeY = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "visual_text_color_r", field );
		bugleInfo.m_iVisualColorR = field.uData.lValue;
		pFile->GetFieldFromLable( iItemID, "visual_text_color_g", field );
		bugleInfo.m_iVisualColorG = field.uData.lValue;
		pFile->GetFieldFromLable( iItemID, "visual_text_color_b", field );
		bugleInfo.m_iVisualColorB = field.uData.lValue;

		pFile->GetFieldFromLable( iItemID, "visual_text_x", field );
		bugleInfo.m_iVisualTextX = field.uData.lValue;
		pFile->GetFieldFromLable( iItemID, "visual_text_y", field );
		bugleInfo.m_iVisualTextY = field.uData.lValue;
		pFile->GetFieldFromLable( iItemID, "visual_text_size_x", field );
		bugleInfo.m_iVisualTextSizeX = field.uData.lValue;
		pFile->GetFieldFromLable( iItemID, "visual_text_size_y", field );
		bugleInfo.m_iVisualTextSizeY = field.uData.lValue;
		
		pFile->GetFieldFromLable( iItemID, "viusal_text_preview", field );
		bugleInfo.m_strPreviewText = field.uData.pValue;

		if (BUGLE_CHANNELBG_GM == bugleInfo.m_channelBgType && !bGM)
		{
			m_vecGMBugleInfo.push_back( bugleInfo );
			bugleInfo.reset();
		}
		else
		{
			m_vecBugleInfo.push_back( bugleInfo );
			bugleInfo.reset();
		}
	}
	
	SAFE_RELEASE( pFile );
	return true;
}
//--------------------------------------------------
bool SPWindowBugleSelect::_initGrid( void )
{
	if (!m_pGrid || !m_pSlider)
		return false;
	m_pGrid->RemoveAllCell();

	UIGridCellPtrArray vecPtrGridCell;
	for (int idx = 0; idx < (int)m_vecBugleInfo.size(); ++idx)
	{
		BugleInfo &info = m_vecBugleInfo[idx];
		POINT ptIconSrcPos;
		const char *texstr =
			g_pItemCluster->GetItemTextureStr( info.m_iItemShopID, ptIconSrcPos );
	//	assert( texstr );

		RECT rtSrc;
		rtSrc.top = ptIconSrcPos.y;
		rtSrc.left = ptIconSrcPos.x;
		rtSrc.right = rtSrc.left + 32;
		rtSrc.bottom = rtSrc.top + 32;
		vecPtrGridCell.push_back( new BugleItemCell( texstr, rtSrc, info.m_iItemShopID ) );
	}

	m_pGrid->AddCell( vecPtrGridCell );
	
	int iScrollableLines = 0;
	iScrollableLines = m_pGrid->GetDataLines() - m_pGrid->GetGridLineCount();
	if (iScrollableLines < 0)
		iScrollableLines = 0;

	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE, iScrollableLines + 1 );

	return true;
}
//--------------------------------------------------
bool SPWindowBugleSelect::_initGUI( void )
{
	m_bShow = false;
	//SPWindow *pTop = NULL;
	SPWindow *pCenter = NULL;
	SPWindow *pBottom = NULL;
	SPWindow *pSkin = NULL;
	SPWindowButton *pButton = NULL;
	SPWindow *pStatic = NULL;	// tmp spwindow pointer.

	// close
	m_pbtnClose = new SPWindowButton( WIID_BUGLE_SELECT_CLOSE, 159, 9, 11, 11, this );
	m_pbtnClose->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	217);	
	m_pbtnClose->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	230);	
	m_pbtnClose->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	243);
	m_pbtnClose->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	256);
	m_pbtnClose->Show();

	// bugle icon
	m_pImgBugleIcon = new SPWindowStatic( WIID_BUGLE_SELECT_ICON, 12, 30, 30, 30, this );
	m_pImgBugleIcon->Clean();
	m_pImgBugleIcon->Hide();

	// top
	m_pTop = new SPWindowStatic( WIID_BUGLE_SELECT_TOP, 0, 0, 183, 66, this);
	m_pTop->SetImage("DATA/INTERFACE/CONCEPT/BUGLE.PNG", 1, 62);
	m_pTop->Show();

	// bugle preview image
	m_pImgBuglePreview = new SPWindowStatic( WIID_BUGLE_SELECT_PREVIEW, 50, 41, 118, 14, this );
	m_pImgBuglePreview->Clean();
	m_pImgBuglePreview->Hide();

	// bugle preview image backgroud
	m_pImgBuglePreviewBG = new SPWindowStatic( WIID_BUGLE_SELECT_PREVIEW_BG, 49, 36, 121, 25, this );
	m_pImgBuglePreviewBG->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE.PNG", 121, 196 );
	m_pImgBuglePreviewBG->Show();

	// center
	pCenter = new SPWindowStatic(WIID_BUGLE_SELECT_CENTER, 0, 66, 183, 122, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/BUGLE.PNG", 1, 130);
	pCenter->SetSrcSize(183, 3);
	pCenter->Show();

	// bottom
	pBottom = new SPWindowStatic(WIID_BUGLE_SELECT_BOTTOM, 0, 188, 183, 12, this);
	pBottom->SetImage("DATA/INTERFACE/CONCEPT/BUGLE.PNG", 1, 136);
	pBottom->Show();

	// title image
	pStatic = new SPWindowStatic(WIID_BUGLE_SELECT_TITLE_IMAGE, 9, 11, 32, 32, this);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/BUGLE.PNG", 33, 343);
	pStatic->Show();
	pStatic = NULL;

	// title
	pStatic = new SPWindowStatic(WIID_BUGLE_SELECT_TITLE, 42, 7, 69, 15, this);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/BUGLE.PNG", 69, 367);
	pStatic->Show();
	pStatic = NULL;

	//Center Skin
	//pSkin = new SPWindowStatic(WIID_BUGLE_SELECT_SKIN, 2, 63-4, 180 - 1 , 182, pCenter);
	//pSkin->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349, 321);
	//pSkin->SetSrcSize(6, 6);
	//pSkin->Hide();

	// slider
	m_pSlider = new SPWindowSlider( WIID_BUGLE_SELECT_SLIDER, 159, 18, 12, 84, pCenter );
	RECT rtRect;
	SetRect( &rtRect , 460 , 470 , 472 , 494 );
	m_pSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 473 , 470 , 485 , 494 );	
	m_pSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 486 , 470 , 498 , 494 );	
	m_pSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 499 , 470 , 511 , 494 );
	m_pSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 460 , 496 , 472 , 498 );	
	m_pSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 473 , 496 , 485 , 498 );	
	m_pSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 486 , 496 , 498 , 498 );	
	m_pSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 499 , 496 , 511 , 498 );
	m_pSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	m_pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pSlider->SetCurpage( 0 );
	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 3 );

	pButton = new SPWindowButton( WIID_BUGLE_SELECT_SLIDER_UP, 159, 8, 12, 11, pCenter);
	pButton->Show();
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 460,	458);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 473,	458);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 486,	458);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 499,	458);	
	pButton = NULL;

	pButton = new SPWindowButton( WIID_BUGLE_SELECT_SLIDER_DOWN, 159, 102, 12, 11, pCenter);
	pButton->Show();
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 460,	500);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 473,	500);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 486,	500);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 499,	500);	
	pButton = NULL;

	// bugle grid
	//m_pGrid = new SPWindowGrid( WNDID_CTRL_GRID, WIID_BUGLE_SELECT_GRID, 12, 9, 152, 152, pSkin, 4, 4, 38, 38, 32);
	m_pGrid = new CUIGridEx( WNDID_CTRL_GRID, WIID_BUGLE_SELECT_GRID, 12, 8, 140, 104, pCenter, 4, 3, 36, 36, 32, 32);
	m_pGrid->SetGridImage( "DATA/INTERFACE/CONCEPT/UI102.PNG", 358, 382 );
	m_pGrid->RemoveAllCell();

	return true;
}
//--------------------------------------------------
int SPWindowBugleSelect::_onToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	SPWindow	*ptrWindow = g_pInterfaceManager->GetToolTipWindow();
	if (!ptrWindow)
		return 0;

	SPItemAttr	*ptrItemAttr = NULL;
	
	int iBugleShopID = 0;
	int idx = (int)lParam;
	if ((int)m_vecBugleInfo.size() > idx)
	{
		iBugleShopID = m_vecBugleInfo[idx].m_iItemShopID;
	}
	else
	{
		//assert( false && "bugle item not found in vector." );
		ptrWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam );
		ptrWindow->Show();
		return 0;
	}

	g_pItemCluster->GetItemInfo( iBugleShopID, ptrItemAttr );
	if (!ptrItemAttr)
		return 0;

	TOOLTIP_NORMAL_DATA stToolTipData;
	stToolTipData.m_iContentsCount = 0;
	stToolTipData.m_strName = ptrItemAttr->m_strName;
	stToolTipData.m_strDesc = ptrItemAttr->m_strDescription;
	stToolTipData.m_cDesc = D3DXCOLOR(0.192f, 0.2980f, 0.38f, 1.0f);
	stToolTipData.m_cName = D3DXCOLOR(0.172f, 0.56f, 0.207f, 1.0f);

	ptrWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)(&stToolTipData), SPWindowToolTip::TOOLTIP_SHOW_TYPE_NORMAL );
	ptrWindow->Show();

	return 1;
}
int SPWindowBugleSelect::_onToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SPWindow *ptrWindow = g_pInterfaceManager->GetToolTipWindow();
	if (ptrWindow)
	{
		ptrWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam );
		ptrWindow->Show();
	}
	return 1;
}
int SPWindowBugleSelect::_onToolTipMove(WPARAM wParam, LPARAM lParam)
{
	return _onToolTipEnter( wParam, lParam );
}
//--------------------------------------------------
//--------------------------------------------------
// class BugleItemCell
//--------------------------------------------------
BugleItemCell::BugleItemCell( void )
: m_ptrTexture( NULL )
, m_iBugleShopID( 0 )
{
	Reset();
}
BugleItemCell::BugleItemCell(const char *szImage, const RECT &rtSrc, int iBugleShopID)
: m_ptrTexture( NULL )
, m_iBugleShopID( 0 )
{
	Reset();
	m_iBugleShopID = iBugleShopID;
	SetTexture( szImage, rtSrc );
}
BugleItemCell::~BugleItemCell( void )
{
	if (m_ptrTexture)
	{
		SAFE_RELEASE( m_ptrTexture );
	}
}
//--------------------------------------------------
bool BugleItemCell::Reset( void )
{
	if (m_ptrTexture)
	{
		SAFE_RELEASE( m_ptrTexture );
	}
	memset( &m_rtSrc, 0, sizeof( m_rtSrc ));

	return true;
}
//--------------------------------------------------
bool BugleItemCell::CellRender(const RECT &rtDst)
{
	if (m_ptrTexture)
	{
		m_ptrTexture->RenderTexture( (RECT*)&rtDst, &m_rtSrc );
	}
	
	return true;
}
//--------------------------------------------------
void BugleItemCell::Destroy( void )
{
	if (m_ptrTexture)
	{
		SAFE_RELEASE( m_ptrTexture );
	}

	delete this;
}
//--------------------------------------------------
RECT BugleItemCell::GetSize( void ) const
{
	return m_rtSrc;
}
//--------------------------------------------------
void BugleItemCell::SetTexture(const char *szImage, const RECT &rtSrc)
{
	if (m_ptrTexture)
	{
		SAFE_RELEASE( m_ptrTexture );
	}

	if (!szImage)
		return;

	g_pVideo->GetTextureMgr()->GetTexture(
		szImage, &m_ptrTexture, TEX_INTERFACE, LEVEL_NOW );
	m_rtSrc = rtSrc;
}
//--------------------------------------------------
class _findBugInfo
{
public:
	_findBugInfo( int iItemID )
	{
		m_iItemID = iItemID;
	}
	bool operator()( const BugleInfo& info )
	{
		if (info.m_iItemShopID == m_iItemID)
			return true;
		else
			return false;
	}
	int	m_iItemID;
};
BugleInfo SPWindowBugleSelect::GetBugleInfo( const int iItemID )
{
	vector< BugleInfo >::iterator iter =
		find_if( m_vecBugleInfo.begin(), m_vecBugleInfo.end(), _findBugInfo( iItemID ) );
	if (iter != m_vecBugleInfo.end())
	{
		return (*iter);
	}

	vector< BugleInfo >::iterator iter_gm =
		find_if( m_vecGMBugleInfo.begin(), m_vecGMBugleInfo.end(), _findBugInfo( iItemID ) );
	if (iter_gm != m_vecGMBugleInfo.end())
	{
		return (*iter_gm);
	}

	return BugleInfo();
}
bool SPWindowBugleSelect::ReloadBugleLDT( bool bGM )
{
	if (false == _loadBugleLDT(bGM))
	{
		assert( false && "Load bugle type LDT failed." );
	}

	if (false == _initGrid())
	{
		assert( false && "Init grid failed." );
	}

	return true;
}
//--------------------------------------------------
// end of file
