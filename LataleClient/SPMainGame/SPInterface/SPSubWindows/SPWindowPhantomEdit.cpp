#ifdef _PHANTOM_EDITOR_ENABLE

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
#include "SPWindowPhantomEdit.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "RTREffect/RTREffectManager.h"
#include "RTREffect/SPPhantom.h"
//--------------------------------------------------
SPWindowPhantomEditor::SPWindowPhantomEditor(WND_ID wndClassID, INSTANCE_ID instanceID)
: SPWindow( wndClassID, instanceID )
{
	Init();
}
SPWindowPhantomEditor::SPWindowPhantomEditor(INSTANCE_ID instanceID, 
									 int iX, int iY, int iCX, int iCY, 
									 SPWindow *pParent /* = NULL  */)
									 : SPWindow( WNDID_PHANTOM_EDITOR, instanceID, iX, iY, iCX, iCY, pParent )
{
	Init();
}
//--------------------------------------------------
SPWindowPhantomEditor::~SPWindowPhantomEditor( void )
{
	Clean();
}
//--------------------------------------------------
void SPWindowPhantomEditor::Init( void )
{
	m_pBtnApply = NULL;
	m_pColorPreview = NULL;
	m_pEditR = NULL;
	m_pEditG = NULL;
	m_pEditB = NULL;
	m_pEditMaxNum = NULL;
	m_pEditDelayTime = NULL;
	m_pEditFirstAlpha = NULL;

	if (!_initGUI())
		assert( false );
}
//--------------------------------------------------
void SPWindowPhantomEditor::Clean( void )
{
	SPWindow::Clean();
}
//--------------------------------------------------
void SPWindowPhantomEditor::Show(bool bWithChild /* = true  */)
{
	if (m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}
	SPWindow::Show( bWithChild );
}
void SPWindowPhantomEditor::Hide(bool bSendServer /* = true  */)
{
	if (m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}

	SPWindow::Hide();
}
//--------------------------------------------------
void SPWindowPhantomEditor::Process(float fDeltaTime)
{
	if (!m_bShow)
		return;

	SPWindow::Process( fDeltaTime );
}
void SPWindowPhantomEditor::Render(float fDeltaTime)
{
	if (!m_bShow)
		return;

	SPWindow::RenderReverse( fDeltaTime );
}
void SPWindowPhantomEditor::RefreshRelationCoord( void )
{
	SPWindow::RefreshRelationCoord();
}
bool SPWindowPhantomEditor::_initGUI( void )
{
	m_pBtnApply = new SPWindowButton( WIID_PHANTOM_EDITOR_APPLY, 60, 74, 68, 17, this );
	m_pBtnApply->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 440);
	m_pBtnApply->SetImageHit("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 458);
	m_pBtnApply->SetImagePush("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 476);
	m_pBtnApply->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 494);

	m_pEditR = new SPWindowEdit( WIID_PHANTOM_EDITOR_R, 18, 4, 34, 17, this);
	m_pEditR->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	m_pEditR->SetLimitText(3);	
	m_pEditR->Show();

	m_pEditG = new SPWindowEdit( WIID_PHANTOM_EDITOR_G, 68, 4, 34, 17, this);
	m_pEditG->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	m_pEditG->SetLimitText(3);	
	m_pEditG->Show();

	m_pEditB = new SPWindowEdit( WIID_PHANTOM_EDITOR_B, 121, 4, 34, 17, this);
	m_pEditB->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	m_pEditB->SetLimitText(3);	
	m_pEditB->Show();

	m_pEditMaxNum = new SPWindowEdit( WIID_PHANTOM_EDITOR_NUMBER, 55, 30, 50, 17, this);
	m_pEditMaxNum->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	m_pEditMaxNum->SetLimitText(2);	
	m_pEditMaxNum->Show();

	m_pEditDelayTime = new SPWindowEdit( WIID_PHANTOM_EDITOR_DELAYTIME, 75, 52, 130, 17, this);
	m_pEditDelayTime->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	m_pEditDelayTime->Show();

	m_pEditFirstAlpha = new SPWindowEdit( WIID_PHANTOM_EDITOR_FIRSTALPHA, 158, 30, 49, 17, this);
	m_pEditFirstAlpha->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	m_pEditFirstAlpha->Show();

	m_pColorPreview = new SPWindowStatic( WIID_PHANTOM_EDITOR_PREVIEW, 160, 2, 74, 20, this ); 
	m_pColorPreview->SetImage( "DATA/INTERFACE/CONCEPT/phantomeditor.png", 1, 101 );
	m_pColorPreview->SetSrcSize( 20, 10 );
	m_pColorPreview->Show();

	SPWindowStatic *bg = new SPWindowStatic( WIID_PHANTOM_EDITOR_BACKGROUD, 0, 0, 240, 100, this );
	bg->SetImage( "DATA/INTERFACE/CONCEPT/phantomeditor.png" );
	bg->SetSrcSize( 240, 100 );
	bg->Show();

	return true;
}
//--------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowPhantomEditor)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_PHANTOM_EDITOR_APPLY, _onBtnApply )
SPIMESSAGE_OBJ_MAP_END(SPWindow)
//--------------------------------------------------
int SPWindowPhantomEditor::_onBtnApply(WPARAM wParam, LPARAM lParam)
{
	_onApply();
	return 0;
}
int SPWindowPhantomEditor::_onBtnClose(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 0;
}
//--------------------------------------------------
static validateColorRange( float &color )
{
	if (color> 1.0f)
		color = 1.0f; 
	if (color < 0.0f)
		color = 0.0f;
}
bool SPWindowPhantomEditor::_onApply( void )
{
	float fColorR = ::atoi( m_pEditR->GetWindowText() ) / 255.0f;
	float fColorG = ::atoi( m_pEditG->GetWindowText() ) / 255.0f;
	float fColorB = ::atoi( m_pEditB->GetWindowText() ) / 255.0f;

	validateColorRange( fColorR );
	validateColorRange( fColorG );
	validateColorRange( fColorB );

	D3DCOLORVALUE color = { fColorR, fColorG, fColorB, 1.0f };

	int iMaxNumber = ::atoi( m_pEditMaxNum->GetWindowText() );
	if (iMaxNumber < 1)
		iMaxNumber = 1;
	float fDelayTime = ::atof( m_pEditDelayTime->GetWindowText() );
	if (fDelayTime < 0.01f)
		fDelayTime = 0.01f;

	float fFirstAlpha = ::atof( m_pEditFirstAlpha->GetWindowText() );
	validateColorRange( fFirstAlpha );

	if (g_pPhantomCfg)
	{
		g_pPhantomCfg->m_iMaxPhantomNbr = iMaxNumber;
		g_pPhantomCfg->m_fDelayTime = fDelayTime;
		g_pPhantomCfg->m_fFirstAlpha = fFirstAlpha;
		g_pPhantomCfg->m_color = color;
	}

	m_pColorPreview->SetColor( D3DXCOLOR( fColorR, fColorG, fColorB, 1.0f ) );

	return true;
}
//--------------------------------------------------
#endif