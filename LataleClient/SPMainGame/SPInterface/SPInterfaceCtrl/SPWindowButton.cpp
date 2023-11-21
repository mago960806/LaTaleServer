
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"

#include "SPResourceDef.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"


SPWindowButton::SPWindowButton(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent, WND_ID CtrlType)
: SPWindow(CtrlType, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}

SPWindowButton::~SPWindowButton()
{
	Clean();
}


void SPWindowButton::InitFromXML(SPWindow_Base* rec_wnd_AttrMap)
{
	SPWindow::InitFromXML(rec_wnd_AttrMap);

	SetImageHit		(UI_STRING(rec_wnd_AttrMap,GET_SPWINDOW_IMAGE_FILENAME)	,UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_BUTTON_HIT_X)		,UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_BUTTON_HIT_Y));
	SetImagePush	(UI_STRING(rec_wnd_AttrMap,GET_SPWINDOW_IMAGE_FILENAME)	,UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_BUTTON_PUSH_X)		,UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_BUTTON_PUSH_Y));
	SetImageDisable	(UI_STRING(rec_wnd_AttrMap,GET_SPWINDOW_IMAGE_FILENAME)	,UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_BUTTON_DISABLE_X)	,UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_BUTTON_DISABLE_Y));
	SetImageCheck	(UI_STRING(rec_wnd_AttrMap,GET_SPWINDOW_IMAGE_FILENAME)	,UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_BUTTON_PUSH_X)		,UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_BUTTON_PUSH_Y));
}


void SPWindowButton::Init()
{	
	m_bDisable		= false;
	m_bPushed		= false;
	m_bChecked		= false;

	m_pImageDisable	= NULL;	
	m_pImageHit		= NULL;	
	m_pImagePush	= NULL;
	m_pImageChecked = NULL;

	m_strToolTip = "[SPWindowButton]";
}

void SPWindowButton::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pImageDisable	);
	SAFE_RELEASE( m_pImagePush		);
	SAFE_RELEASE( m_pImageHit		);
	SAFE_RELEASE( m_pImageChecked	);
}

bool SPWindowButton::IsChecked()
{
	return m_bChecked;
}

void SPWindowButton::SetImageDisable(SPTexture* pImage, int iXSrc, int iYSrc)
{	
	SAFE_RELEASE(m_pImageDisable);

	m_pImageDisable	= pImage;	
	m_ptDisableSrcPos.x = iXSrc;
	m_ptDisableSrcPos.y = iYSrc;
}

void	SPWindowButton::SetImageHit(SPTexture* pImage, int iXSrc, int iYSrc)
{	
	SAFE_RELEASE(m_pImageHit);

	m_pImageHit		= pImage;	
	m_ptHitSrcPos.x = iXSrc;
	m_ptHitSrcPos.y = iYSrc;
}	

void	SPWindowButton::SetImagePush(SPTexture* pImage, int iXSrc, int iYSrc)
{	
	SAFE_RELEASE(m_pImagePush);

	m_pImagePush	= pImage;
	m_ptPushSrcPos.x = iXSrc;
	m_ptPushSrcPos.y = iYSrc;
}	


bool SPWindowButton::SetImageDisable(const char* szImage, int iXSrc, int iYSrc)
{
	SPTexture* pImage = NULL;	
	std::string strResource;

	//strResource = RES_WORK_RESOURCE_PATH;
	strResource = szImage;
	
	g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage);
	SetImageDisable(pImage, iXSrc, iYSrc);	

	return true;
}

bool SPWindowButton::SetImageHit(const char* szImage, int iXSrc, int iYSrc)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	//strResource = RES_WORK_RESOURCE_PATH;
	strResource = szImage;

	g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage);
	SetImageHit(pImage, iXSrc, iYSrc);	

	return true;

}

bool SPWindowButton::SetImagePush(const char* szImage, int iXSrc, int iYSrc)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	//strResource = RES_WORK_RESOURCE_PATH;
	strResource = szImage;

	g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage);
	SetImagePush(pImage, iXSrc, iYSrc);	

	return true;
}

bool SPWindowButton::SetImageCheck(const char* szImage, int iXSrc, int iYSrc)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	SAFE_RELEASE(m_pImageChecked);
	
	strResource = szImage;
	g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage);

	m_pImageChecked	= pImage;
	m_ptCheckSrcPos.x = iXSrc;
	m_ptCheckSrcPos.y = iYSrc;	

	return true;
}


void SPWindowButton::Process(float fTime)
{
	//SPWindow::Process(fTime);
}

void SPWindowButton::Render(float fTime)
{
	SPWindow::Render(fTime);

	RenderText();
	//if(!m_strWindowText.empty()) { 	 
	//	RECT TarRect;
	//	GetWindowRect(TarRect);
	//	if(m_bPushed) {
	//		OffsetRect(&TarRect, 1, 1);
	//	}
	//	m_pFont->RenderText(m_strWindowText.c_str(), &TarRect, m_dwAlign);			
	//}

}


void SPWindowButton::RenderText()
{
	//////////////////////////////////////////////////////////////////////////	
	//  Render Text Only Current Window..!! 
	//

	if( g_pVideo == NULL )
		return;

	if( m_pFont == NULL )
		return;

	if(!m_strWindowText.empty()) {
		RECT TarRect;
		bool bTemp = m_pFont->IsShadow();

		GetWindowRect(TarRect);
		
		//[xialin 2008/08/18] edit >>>
		//if(m_bPushed) {
		//		OffsetRect(&TarRect, 1, 1);
		//}

		if (m_bPushed || m_bChecked)
		{
			if(m_dwAlign & DT_VCENTER)
				OffsetRect(&TarRect, 1, (m_iSY- m_pFont->GetHeight())/2 +1);
			else
				OffsetRect(&TarRect, 1, 1);
		}
		else
		{
			if(m_dwAlign & DT_VCENTER)
				OffsetRect(&TarRect, 0, (m_iSY- m_pFont->GetHeight())/2);
		}
		//[xialin 2008/08/18] edit <<<

		m_pFont->SetColor(	m_FontColor );
		m_pFont->SetEffect( m_FontEffect );
		m_pFont->SetPeriod(m_bTextPeriod);
		m_pFont->RenderText(m_strWindowText.c_str(), &TarRect, m_dwAlign);

		m_pFont->SetColor();
		m_pFont->SetShadow(	bTemp );
		m_pFont->SetPeriod(false);
	}
}


void SPWindowButton::SetEnable(bool bEnable)
{
	m_bDisable = !bEnable;
	if(m_bDisable) 	{
		m_ppImageActive = &m_pImageDisable;
		m_iSrcX = m_ptDisableSrcPos.x;
		m_iSrcY = m_ptDisableSrcPos.y;
	} else {
		if(m_bChecked) {
			if(m_pImageChecked) {
				m_ppImageActive = &m_pImageChecked;
				m_iSrcX = m_ptCheckSrcPos.x;
				m_iSrcY = m_ptCheckSrcPos.y;
			} else {
				m_ppImageActive = &m_pImageHit;
				m_iSrcX = m_ptHitSrcPos.x;
				m_iSrcY = m_ptHitSrcPos.y;
			}
		} else {
			m_ppImageActive = &m_pImageNormal;
			m_iSrcX = m_iNormalSrcX;
			m_iSrcY = m_iNormalSrcY;
		}
	}

	SPWindow::SetEnable(bEnable);
}


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowButton )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnLButtonDown	)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnLButtonUP		)
	SPIMESSAGE_COMMAND(	SPIM_RBUTTON_DOWN,		OnRButtonDown	)
	SPIMESSAGE_COMMAND(	SPIM_RBUTTON_UP,		OnRButtonUP		)
	SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN,		OnKeyEnter		)
	SPIMESSAGE_COMMAND(	SPIM_BTN_SELECT,		OnBtnSelect		)
	SPIMESSAGE_COMMAND(	SPIM_BTN_ENABLE,		OnBtnEnable		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DBLCLICK,	OnLButtonDBLClick		)
	SPIMESSAGE_COMMAND( SPIM_SET_COLOR,			OnSetColor		)

	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_MOVE,		OnToolTipMove	)
	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_ENTER,		OnToolTipEnter	)
	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_OUT,		OnToolTipOut	)

	SPIMESSAGE_COMMAND(SPIM_MOVE_BEGIN	,	OnBeginMove		)
	SPIMESSAGE_COMMAND(SPIM_MOVE_END	,	OnEndMove		)
	SPIMESSAGE_COMMAND(SPIM_MOVE		,	OnMove			)
	
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowButton::OnCursorEnter( WPARAM wParam, LPARAM lParam)
{	
	if(!m_bDisable)
	{
		if(m_bChecked) {
			if(m_pImageChecked) {
				m_ppImageActive = &m_pImageChecked;
				m_iSrcX = m_ptCheckSrcPos.x;
				m_iSrcY = m_ptCheckSrcPos.y;
			} else {

				if(m_pImageHit) {				
					m_ppImageActive = &m_pImageHit;
					m_iSrcX = m_ptHitSrcPos.x;
					m_iSrcY = m_ptHitSrcPos.y;
				} else {
					m_ppImageActive = &m_pImageNormal;
					m_iSrcX = m_iNormalSrcX;
					m_iSrcY = m_iNormalSrcY;
				}
				//m_iSrcX = m_ptHitSrcPos.x;
				//m_iSrcY = m_ptHitSrcPos.y;
				//g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_BUTTON_ON.WAV");
			}
			return SPSendMessageToParent(SPIM_CURSOR_ENTER, wParam, lParam, this);	
		} else {
			//m_ppImageActive = &m_pImageHit;
			//m_iSrcX = m_ptHitSrcPos.x;
			//m_iSrcY = m_ptHitSrcPos.y;

			if(m_pImageHit) {				
				m_ppImageActive = &m_pImageHit;
				m_iSrcX = m_ptHitSrcPos.x;
				m_iSrcY = m_ptHitSrcPos.y;
			} else {
				m_ppImageActive = &m_pImageNormal;
				m_iSrcX = m_iNormalSrcX;
				m_iSrcY = m_iNormalSrcY;
			}
			//g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_BUTTON_ON.WAV");
			return SPSendMessageToParent(SPIM_CURSOR_ENTER, wParam, lParam, this);	
		}
	}
	return 1;
	
}

int SPWindowButton::OnCursorOut( WPARAM wParam, LPARAM lParam)
{
	if(!m_bDisable)
	{
		if(m_bChecked) {
			if(m_pImageChecked) {
				m_ppImageActive = &m_pImageChecked;
				m_iSrcX = m_ptCheckSrcPos.x;
				m_iSrcY = m_ptCheckSrcPos.y;
			} else {
				if(m_pImageHit) {				
					m_ppImageActive = &m_pImageHit;
					m_iSrcX = m_ptHitSrcPos.x;
					m_iSrcY = m_ptHitSrcPos.y;
				} else {
					m_ppImageActive = &m_pImageChecked;
					m_iSrcX = m_ptCheckSrcPos.x;
					m_iSrcY = m_ptCheckSrcPos.y;
				}
				//m_ppImageActive = &m_pImageHit;
				//m_iSrcX = m_ptHitSrcPos.x;
				//m_iSrcY = m_ptHitSrcPos.y;
			}
		} else {
			m_ppImageActive = &m_pImageNormal;
			m_iSrcX = m_iNormalSrcX;
			m_iSrcY = m_iNormalSrcY;
		}

		return SPSendMessageToParent(SPIM_CURSOR_OUT, wParam, lParam, this);
	}
	return 1;	
}

int SPWindowButton::OnLButtonDown( WPARAM wParam, LPARAM lParam)
{
	if(!m_bDisable) 
	{
		m_ppImageActive = &m_pImagePush;
		m_iSrcX = m_ptPushSrcPos.x;
		m_iSrcY = m_ptPushSrcPos.y;
		m_bPushed = true;
		return SPSendMessageToParent(SPIM_LBUTTON_DOWN, wParam, lParam, this);
	}
	return 1;
}

int SPWindowButton::OnLButtonUP( WPARAM wParam, LPARAM lParam)
{
	if(!m_bDisable) 
	{
		if(GetClassID() == WNDID_CTRL_BUTTON_CHECK) {
			(m_bChecked == true) ? OnBtnSelect(FALSE,0) : OnBtnSelect(TRUE,0);
		}
	
		if(m_bChecked)
		{
			if(m_pImageChecked)
			{
				m_ppImageActive = &m_pImageChecked;
				m_iSrcX = m_ptCheckSrcPos.x;
				m_iSrcY = m_ptCheckSrcPos.y;
			}
			else
			{
				m_ppImageActive = &m_pImageNormal;
				m_iSrcX = m_iNormalSrcX;
				m_iSrcY = m_iNormalSrcY;
			}
		}
		else if(m_pImageHit)
		{		
			m_ppImageActive = &m_pImageHit;
			m_iSrcX = m_ptHitSrcPos.x;
			m_iSrcY = m_ptHitSrcPos.y;
		} 
		else
		{
			m_ppImageActive = &m_pImageNormal;
			m_iSrcX = m_iNormalSrcX;
			m_iSrcY = m_iNormalSrcY;
		}

		//if(m_pImageHit) {		
		//	m_ppImageActive = &m_pImageHit;
		//	m_iSrcX = m_ptHitSrcPos.x;
		//	m_iSrcY = m_ptHitSrcPos.y;
		//} else {
		//	if(m_bChecked && m_pImageChecked) {
		//		m_ppImageActive = &m_pImageChecked;
		//		m_iSrcX = m_ptCheckSrcPos.x;
		//		m_iSrcY = m_ptCheckSrcPos.y;
		//	} else {
		//		m_ppImageActive = &m_pImageNormal;
		//		m_iSrcX = m_iNormalSrcX;
		//		m_iSrcY = m_iNormalSrcY;
		//	}
		//}
		m_bPushed = false;
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_BUTTON_CASE2.WAV");
		return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, lParam, this);
	}
	return 1;
}

int SPWindowButton::OnRButtonDown( WPARAM wParam, LPARAM lParam)
{
	if(!m_bDisable) 
	{
		m_ppImageActive = &m_pImagePush;
		m_iSrcX = m_ptPushSrcPos.x;
		m_iSrcY = m_ptPushSrcPos.y;
		m_bPushed = true;
		return SPSendMessageToParent(SPIM_RBUTTON_DOWN, wParam, lParam, this);
	}
	return 1;
}

int SPWindowButton::OnRButtonUP( WPARAM wParam, LPARAM lParam)
{
	if(!m_bDisable) 
	{
		//m_ppImageActive = &m_pImageHit;
		//m_iSrcX = m_ptHitSrcPos.x;
		//m_iSrcY = m_ptHitSrcPos.y;

		if(m_pImageHit) {		
			m_ppImageActive = &m_pImageHit;
			m_iSrcX = m_ptHitSrcPos.x;
			m_iSrcY = m_ptHitSrcPos.y;
		} else {
			if(m_bChecked && m_pImageChecked) {
				m_ppImageActive = &m_pImageChecked;
				m_iSrcX = m_ptCheckSrcPos.x;
				m_iSrcY = m_ptCheckSrcPos.y;
			} else {
				m_ppImageActive = &m_pImageNormal;
				m_iSrcX = m_iNormalSrcX;
				m_iSrcY = m_iNormalSrcY;
			}
		}

		m_bPushed = false;
		return SPSendMessageToParent(SPIM_RBUTTON_UP, wParam, lParam, this);
	}
	return 1;
}

int SPWindowButton::OnKeyEnter( WPARAM wParam, LPARAM lParam)
{
	if(!m_bDisable) 
	{
		if(GetClassID() == WNDID_CTRL_BUTTON_CHECK) {
			(m_bChecked == true) ? OnBtnSelect(FALSE,0) : OnBtnSelect(TRUE,0);
		}
		m_ppImageActive = &m_pImagePush;
		m_iSrcX = m_ptPushSrcPos.x;
		m_iSrcY = m_ptPushSrcPos.y;
		m_bPushed = true;
		return SPSendMessageToParent(SPIM_KEY_RETURN, wParam, lParam, this);
	}
	return 1;
}


int SPWindowButton::OnBtnSelect( WPARAM wParam, LPARAM lParam)
{
	if(!m_bDisable) 
	{
		if(wParam == TRUE) {		
			// 체크를 했을때 체크이미지 없으면 HitTest이미지로 렌더
			if(m_pImageChecked) {
				m_ppImageActive = &m_pImageChecked;
				m_iSrcX = m_ptCheckSrcPos.x;
				m_iSrcY = m_ptCheckSrcPos.y;
			} else {
				m_ppImageActive = &m_pImageHit;
				m_iSrcX = m_ptHitSrcPos.x;
				m_iSrcY = m_ptHitSrcPos.y;
			}
			m_bChecked = true;

		} else {
			m_ppImageActive = &m_pImageNormal;
			m_iSrcX = m_iNormalSrcX;
			m_iSrcY = m_iNormalSrcY;
			m_bChecked = false;
		}

		return SPSendMessageToParent(SPIM_BTN_SELECT, wParam, lParam, this);
	}
	return 1;
}


int SPWindowButton::OnBtnEnable( WPARAM wParam, LPARAM lParam)
{
	SetEnable( wParam == TRUE ? true : false);
	return SPSendMessageToParent(SPIM_BTN_ENABLE, wParam, lParam, this);
}

int SPWindowButton::OnLButtonDBLClick( WPARAM wParam, LPARAM lParam)
{
	if(!m_bDisable) 
		return SPSendMessageToParent(SPIM_LBUTTON_DBLCLICK, wParam, lParam, this);
	
	return 1;
}

int SPWindowButton::OnSetColor( WPARAM wParam, LPARAM lParam )
{
	m_RGBA	=	(DWORD)wParam;
	return 0;
}

int SPWindowButton::OnToolTipMove( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_TOOLTIP_MOVE, wParam, lParam, this);
}

int SPWindowButton::OnToolTipEnter( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_TOOLTIP_ENTER, wParam, lParam, this);
}

int SPWindowButton::OnToolTipOut( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_TOOLTIP_OUT, 0, 0, this);
}


int SPWindowButton::OnBeginMove( WPARAM, LPARAM )
{
	return 1;
}


int SPWindowButton::OnEndMove( WPARAM, LPARAM )
{
	return 1;
}


int SPWindowButton::OnMove( WPARAM, LPARAM )
{
	return 1;
}
