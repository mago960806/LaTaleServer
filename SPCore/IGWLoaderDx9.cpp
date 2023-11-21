#include "igwloaderdx9.h"
#include "strsafe.h"
#include "assert.h"
#include "windows.h"
#include "SPCoreDef.h"	


HINSTANCE CIGWLoaderDx9::hInstIGWDll = 0;
CIGWLoaderDx9*	g_pCIGWLoaderDx9 = NULL;
// 内部用用
#define IGWSAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

bool GetGameWidgetPath(LPBYTE lpBuf, DWORD dwSize)
{
	WCHAR szPath[] = L"SOFTWARE\\snda\\IGW";
	HKEY hKey = NULL;
	if (RegOpenKeyExW( HKEY_LOCAL_MACHINE, szPath, 0, KEY_READ, &hKey ) != ERROR_SUCCESS)
		return false;

	DWORD dwType = REG_EXPAND_SZ;
	if (RegQueryValueExW(hKey, L"dllfile", NULL, &dwType, lpBuf, &dwSize) != ERROR_SUCCESS )
		return false;
	return true;
}

HMODULE	WINAPI	SafeLoadLibraryW( IN LPCWSTR lpLibFileName, UINT ErrorMode = SEM_NOOPENFILEERRORBOX )
{
	// copy form SysUtils.pas
	WORD FPUControlWord;
	UINT OldMode;
	HMODULE CurResult = 0;
	OldMode = SetErrorMode( ErrorMode );
	__try
	{
		__asm{
			fnstcw	FPUControlWord
		}
		__try
		{
			CurResult = LoadLibraryW( lpLibFileName );
		}
		__finally
		{
			__asm{
				fnclex
				fldcw FPUControlWord
			}
		}
	}
	__finally
	{
		SetErrorMode( OldMode );
	}
	return CurResult;
}

CIGWLoaderDx9::CIGWLoaderDx9( const LPGameBasicInfoExA GameInfo )
{
	assert( GameInfo != NULL );

	if ( InitCreate() )
	{
		if ( igwInitExA )
		{
			if ( igwInitExA( IGW_VERSION, GameInfo ) == IGW_OK )
			{
				if ( !GetAllIGWInteface() )
				{
					CleanupIGWLoader();
				}
			}
		}
		else
		{
			if ( igwInit )
			{
				if ( igwInit( IGW_VERSION ) == IGW_OK )
				{
					if ( !GetAllIGWInteface() )
					{
						CleanupIGWLoader();
					}
				}
			}
		}
	}
}

CIGWLoaderDx9::CIGWLoaderDx9( const LPGameBasicInfoExW GameInfo )
{
	assert( GameInfo != NULL );

	if ( InitCreate() )
	{
		if ( igwInitExA )
		{
			if ( igwInitExW( IGW_VERSION, GameInfo ) == IGW_OK )
			{
				if ( !GetAllIGWInteface() )
				{
					CleanupIGWLoader();
				}
			}
		}
		else
		{
			if ( igwInit )
			{
				if ( igwInit( IGW_VERSION ) == IGW_OK )
				{
					if ( !GetAllIGWInteface() )
					{
						CleanupIGWLoader();
					}
				}
			}
		}
	}
}


CIGWLoaderDx9::~CIGWLoaderDx9(void)
{
    if (hInstIGWDll != 0)
    {
        if (igwTerminal)
        {
            // 主要是释放无法在DLL_PROCESS_DETACH时释放的资源
            igwTerminal();				
        }
        // 保证在退出后IGW的对应回调被清空
        SetAudioSetting(NULL, NULL, NULL, NULL);		

        // 加载后不释放，直到退出
        FreeLibrary(hInstIGWDll);
        hInstIGWDll = 0;
    }
    CleanupIGWLoader();	
}

void CIGWLoaderDx9::CleanupIGWLoader()
{
    // 清除所有资源
    if (bUseIGWSupportEx)
    { // 是调用igwSupportEx这套接口，所有接口内存需要自己释放
        IGWSAFE_DELETE(G_IGWExportDx9);
        IGWSAFE_DELETE(FTUserA);
        IGWSAFE_DELETE(FTUserW);
        IGWSAFE_DELETE(FIGWCustomizeInterfaceA);
        IGWSAFE_DELETE(FIGWCustomizeInterfaceW);
        IGWSAFE_DELETE(FIGWConfigure);
        IGWSAFE_DELETE(m_pCommunityInterfaceA);
        IGWSAFE_DELETE(m_pCommunityInterfaceW);
        IGWSAFE_DELETE(m_pLocationInterfaceA);
        IGWSAFE_DELETE(m_pLocationInterfaceW);
    }

    // 清除音量设置相关
    IGWSAFE_DELETE(FIGWGameAudioSetting);

    // 清空保证所有调用无效
    G_IGWExportDx9 = NULL;
    igwInit = NULL;
    igwSupportW = NULL;
    igwSupportA = NULL;
    igwSupportExA = NULL;
    igwSupportExW = NULL;
    igwTerminal = NULL;
    bUseIGWSupportEx = false;
    FTUserA = NULL;
    FTUserW = NULL;
    FIGWCustomizeInterfaceA = NULL;
    FIGWCustomizeInterfaceW = NULL;
    FIGWConfigure = NULL;
}

BOOL CIGWLoaderDx9::InitializeGraphic(IDirect3DDevice9* Dev9, D3DPRESENT_PARAMETERS* Params, bool HookGameWnd)
{
    if (G_IGWExportDx9 != NULL)
    {
        if(G_IGWExportDx9->FInitializeGraphic)
            return G_IGWExportDx9->FInitializeGraphic(Dev9, Params, HookGameWnd);
        else
            return false;
    }
    else
        return false;
}

BOOL CIGWLoaderDx9::Render()
{
	if ( !FVisible )
	{
		// 控制是否显示
		return false;
	}

    if (G_IGWExportDx9 != NULL)
    {
        if(G_IGWExportDx9->FRender)
            return G_IGWExportDx9->FRender();
        else
            return false;
    }
    else
        return false;
}

BOOL CIGWLoaderDx9::FinalizeGraphic()
{
    if (G_IGWExportDx9 != NULL)
    {
        if(G_IGWExportDx9->FFinalizeGraphic)
            return G_IGWExportDx9->FFinalizeGraphic();
        else
            return false;
    }
    else
        return false;
}

VOID CIGWLoaderDx9::OnDeviceReset(D3DPRESENT_PARAMETERS* Params)
{
    if (G_IGWExportDx9 != NULL)
    {
        if( G_IGWExportDx9->FOnDeviceReset)
            return G_IGWExportDx9->FOnDeviceReset(Params);
    }
}

VOID CIGWLoaderDx9::OnDeviceLost()
{
    if (G_IGWExportDx9 != NULL)
    {
        if(G_IGWExportDx9->FOnDeviceLost)
            return G_IGWExportDx9->FOnDeviceLost();
    }
}

HRESULT CIGWLoaderDx9::OnWindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( !FVisible )
	{
		return S_FALSE;
	}
	
    if (G_IGWExportDx9 != NULL)
    {
        if(G_IGWExportDx9->FOnIGWWndProc)
		{
			if ( VK_RETURN == wParam || VK_ESCAPE == wParam )
			{
				return S_FALSE;
			}
			else if ( WM_MOUSEMOVE == uMsg || WM_SETCURSOR == uMsg )
			{	
				G_IGWExportDx9->FOnIGWWndProc(hWnd, uMsg, wParam, lParam);
				return S_FALSE;
			}
			else
			{
				return G_IGWExportDx9->FOnIGWWndProc(hWnd, uMsg, wParam, lParam);
			}
		}
		else
		{
            return S_FALSE;
		}    
	}
    else
        return S_FALSE;
}

HRESULT CIGWLoaderDx9::RenderEx(WPARAM wParam, LPARAM lParam)
{
	if ( !FVisible )
	{
		// 控制是否显示
		return false;
	}

    if (G_IGWExportDx9 != NULL)
    {
        if(G_IGWExportDx9->FRenderEx)
            return G_IGWExportDx9->FRenderEx(wParam, lParam);
        else
            return S_FALSE;
    }
    else
        return S_FALSE;
}

VOID	CIGWLoaderDx9::InitGameInfoA ( const LPGameBasicInfoA GameInfo )
{
    if ( this->FTUserA )
    {
        if ( this->FTUserA->InitGameInfo )
            this->FTUserA->InitGameInfo ( GameInfo ) ;
    }
}

VOID	CIGWLoaderDx9::InitGameInfoW ( const LPGameBasicInfoW GameInfo )
{
    if ( this->FTUserW )
    {
        if ( this->FTUserW->InitGameInfo )
            this->FTUserW->InitGameInfo ( GameInfo ) ;
    }
}

VOID	CIGWLoaderDx9::LoginA ( const PUserBasicInfoA UserInfo, int LoginType)
{
    if( this->FTUserA )
    {
        if ( this->FTUserA->Login )
            this->FTUserA->Login ( UserInfo, LoginType ) ;
    }
}

VOID	CIGWLoaderDx9::LoginW ( const PUserBasicInfoW UserInfo, int LoginType)
{
    if ( this->FTUserW )
    {
        if ( this->FTUserW->Login )
            this->FTUserW->Login ( UserInfo, LoginType ) ;
    }
}

VOID	CIGWLoaderDx9::LogoutA ()
{
    if ( this->FTUserA )
    {
        if ( this->FTUserA->Logout )
            this->FTUserA->Logout ( ) ;
    }
}

VOID	CIGWLoaderDx9::LogoutW () 
{
    if ( this->FTUserW )
    {
        if (this->FTUserW->Logout)
            this->FTUserW->Logout () ;
    }
}

VOID	CIGWLoaderDx9::ChangeUserInfoA ( const PUserDetailInfoA UserInfo )
{
    if ( this->FTUserA )
    {
        if(this->FTUserA->ChangeUserInfo)
            this->FTUserA->ChangeUserInfo ( UserInfo ) ;
    }
}

VOID	CIGWLoaderDx9::ChangeUserInfoW ( const PUserDetailInfoW UserInfo )
{
    if ( this->FTUserW )
    {
        if (this->FTUserW->ChangeUserInfo)
            this->FTUserW->ChangeUserInfo ( UserInfo ) ;
    }
}

VOID	CIGWLoaderDx9::ChangeLocationA ( const PUserLocationA Location )
{
    if ( this->FTUserA )
    {
        if (this->FTUserA->ChangeLocation)
            this->FTUserA->ChangeLocation ( Location ) ;
    }
}

VOID	CIGWLoaderDx9::ChangeLocationW ( const PUserLocationW Location )
{
    if ( this->FTUserW )
    {
        if(this->FTUserW->ChangeLocation)
            this->FTUserW->ChangeLocation ( Location ) ;
    }
}

bool CIGWLoaderDx9::GetSelfCursor()
{
    if ( this->FIGWConfigure )
    {
        if(this->FIGWConfigure->GetSelfCursor)
            return this->FIGWConfigure->GetSelfCursor();
        else
            return false;
    }
    else
        return false;
}

void CIGWLoaderDx9::SetSelfCursor( const bool value )
{
    if ( this->FIGWConfigure )
    {
        if(this->FIGWConfigure->SetSelfCursor)
            this->FIGWConfigure->SetSelfCursor(value);
    }
}

int CIGWLoaderDx9::GetScreenStatus()
{
    if ( this->FIGWConfigure )
    {
        if (this->FIGWConfigure->GetScreenStatus)		
            return this->FIGWConfigure->GetScreenStatus();
        else
            return igwScreenNone;
    }
    else
        return igwScreenNone;
}

void CIGWLoaderDx9::SetScreenStatus( const int value )
{
    if ( this->FIGWConfigure )
    {
        if (this->FIGWConfigure->SetScreenStatus)
            this->FIGWConfigure->SetScreenStatus(value);
    }
}

bool CIGWLoaderDx9::GetScreenEnabled()
{
    if ( this->FIGWConfigure )
    {
        if (this->FIGWConfigure->GetScreenEnabled)
            return this->FIGWConfigure->GetScreenEnabled();
        else
            return false;
    }
    else
        return false;
}

void CIGWLoaderDx9::SetScreenEnabled( const bool value )
{
    if ( this->FIGWConfigure )
    {
        if(this->FIGWConfigure->SetScreenEnabled)
            this->FIGWConfigure->SetScreenEnabled(value);
    }
}

bool CIGWLoaderDx9::GetTaskBarPosition( PPOINT p )
{
    if ( this->FIGWConfigure )
    {
        if (this->FIGWConfigure->GetTaskBarPosition)
            return this->FIGWConfigure->GetTaskBarPosition(p);
        else
            return false;
    }
    else
    {
        return false;
    }
}

bool CIGWLoaderDx9::SetTaskBarPosition( const POINT p )
{
    if ( this->FIGWConfigure )
    {
        if (this->FIGWConfigure->SetTaskBarPosition)
            return this->FIGWConfigure->SetTaskBarPosition(p);
        else
            return false;
    }
    else
        return false;
}

int CIGWLoaderDx9::GetWinPropertyA( LPCSTR WinName, PIGWWinProperty WinProperty )
{
    if ( this->FIGWCustomizeInterfaceA )
    {
        if (this->FIGWCustomizeInterfaceA->GetWinProperty)
            return this->FIGWCustomizeInterfaceA->GetWinProperty(WinName, WinProperty);
        else
            return IGW_FALSE;
    }
    else
        return IGW_FALSE;
}

int CIGWLoaderDx9::SetWinPropertyA( LPCSTR WinName, PIGWWinProperty WinProperty )
{
    if ( this->FIGWCustomizeInterfaceA )
    {
        if (this->FIGWCustomizeInterfaceA->SetWinProperty)		
            return this->FIGWCustomizeInterfaceA->SetWinProperty(WinName, WinProperty);
        else
            return IGW_FALSE;
    }
    else
        return IGW_FALSE;
}

int CIGWLoaderDx9::GetWinPropertyW( LPCWSTR WinName, PIGWWinProperty WinProperty )
{
    if ( this->FIGWCustomizeInterfaceW )
    {
        if (this->FIGWCustomizeInterfaceW->GetWinProperty)
            return this->FIGWCustomizeInterfaceW->GetWinProperty(WinName, WinProperty);
        else
            return IGW_FALSE;
    }
    else
        return IGW_FALSE;
}

int CIGWLoaderDx9::SetWinPropertyW( LPCWSTR WinName, PIGWWinProperty WinProperty )
{
    if ( this->FIGWCustomizeInterfaceW )
    {
        if (this->FIGWCustomizeInterfaceW->SetWinProperty)
            return this->FIGWCustomizeInterfaceW->SetWinProperty(WinName, WinProperty);
        else
            return IGW_FALSE;
    }
    else
        return IGW_FALSE;
}

bool CIGWLoaderDx9::GetFocus()
{
    // 在0.9.0.39版本中没有
    if (!bUseIGWSupportEx)
        return false;

    if ( this->FIGWConfigure )
    {
        if (this->FIGWConfigure->GetFocus) 
        {
            return this->FIGWConfigure->GetFocus();
        }
        else
            return false;

    }
    else
        return false;
}

void CIGWLoaderDx9::SetFocus( const bool value )
{
    // 在0.9.0.39版本中没有
    if (bUseIGWSupportEx)
    {
        if ( this->FIGWConfigure )
        {
            if (this->FIGWConfigure->SetFocus)
            {
                return this->FIGWConfigure->SetFocus(value);
            }
        }
    }
}

HRESULT CIGWLoaderDx9::OnWindowProcEx( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lResult )
{
	if ( !FVisible )
	{
		return S_FALSE;
	}
	
    if (G_IGWExportDx9 != NULL)
    {
        if(G_IGWExportDx9->FOnIGWWndProcEx)
		{
			if ( VK_RETURN == wParam || VK_ESCAPE == wParam )
			{
				return S_FALSE;
			}
			else if ( WM_MOUSEMOVE == uMsg || WM_SETCURSOR == uMsg  )
			{	
				G_IGWExportDx9->FOnIGWWndProcEx(hWnd, uMsg, wParam, lParam, lResult);
				return S_FALSE;
			}
			else
			{
				return G_IGWExportDx9->FOnIGWWndProcEx(hWnd, uMsg, wParam, lParam, lResult);
			}
		}
		else
        {
			// 按照新版本的接入方式加载到一旧版本的则这样处理
            return this->OnWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }
    else
        return S_FALSE;

}

bool CIGWLoaderDx9::SetAudioSetting( PFNGetAudioSoundVolume AGetAudioSoundVolume, PFNSetAudioSoundVolume ASetAudioSoundVolume,
                                    PFNGetAudioEffectVolume AGetAudioEffectVolume, PFNSetAudioEffectVolume ASetAudioEffectVolume )
 {
    if (igwSupportExA)
    {
        if (!FIGWGameAudioSetting)
        {
            FIGWGameAudioSetting = new IGWGameAudioSetting();
        }

        FIGWGameAudioSetting->NodifyAudioVolumeChanged = NULL;
        FIGWGameAudioSetting->GetAudioSoundVolume = AGetAudioSoundVolume;
        FIGWGameAudioSetting->SetAudioSoundVolume = ASetAudioSoundVolume;
        FIGWGameAudioSetting->GetAudioEffectVolume = AGetAudioEffectVolume;
        FIGWGameAudioSetting->SetAudioEffectVolume = ASetAudioEffectVolume;
        if ( IGW_OK == igwSupportExA(igwGameAudioSetting, FIGWGameAudioSetting, sizeof(IGWGameAudioSetting)) )
        {// 操作成功返回true
            return true;
        }
        else
        {// 操作失败返回false、并清空FIGWGameAudioSetting以确保NodifyAudioChanged()正确
            IGWSAFE_DELETE(FIGWGameAudioSetting);
            return false;
        }
    }
    else
        return false;
}

bool CIGWLoaderDx9::NodifyAudioChanged()
{
    if (FIGWGameAudioSetting)
    {
        if (FIGWGameAudioSetting->NodifyAudioVolumeChanged)
        {	// 通知IGW音量设置相关有变化
            FIGWGameAudioSetting->NodifyAudioVolumeChanged();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void CIGWLoaderDx9::Terminal()
{
    if (igwTerminal)
    {
        // 主要是释放无法在DLL_PROCESS_DETACH时释放的资源
        igwTerminal();				
    }
}

int CIGWLoaderDx9::OpenWidgetA( LPCSTR WidgetNameSpace )
{
    if ( this->FIGWCustomizeInterfaceA )
    {
        if (this->FIGWCustomizeInterfaceA->OpenWidget)		
            return this->FIGWCustomizeInterfaceA->OpenWidget(WidgetNameSpace);
        else
            return IGW_FALSE;
    }
    else
        return IGW_FALSE;
}

int CIGWLoaderDx9::CloseWidgetA( LPCSTR WidgetNameSpace )
{
    if ( this->FIGWCustomizeInterfaceA )
    {
        if (this->FIGWCustomizeInterfaceA->CloseWidget)		
            return this->FIGWCustomizeInterfaceA->CloseWidget(WidgetNameSpace);
        else
            return IGW_FALSE;
    }
    else
        return IGW_FALSE;
}

int CIGWLoaderDx9::OpenWidgetW( LPCWSTR WidgetNameSpace )
{
    if ( this->FIGWCustomizeInterfaceW )
    {
        if (this->FIGWCustomizeInterfaceW->OpenWidget)		
            return this->FIGWCustomizeInterfaceW->OpenWidget(WidgetNameSpace);
        else
            return IGW_FALSE;
    }
    else
        return IGW_FALSE;
}

int CIGWLoaderDx9::CloseWidgetW( LPCWSTR WidgetNameSpace )
{
    if ( this->FIGWCustomizeInterfaceW )
    {
        if (this->FIGWCustomizeInterfaceW->CloseWidget)		
            return this->FIGWCustomizeInterfaceW->CloseWidget(WidgetNameSpace);
        else
            return IGW_FALSE;
    }
    else
        return IGW_FALSE;
}

//社群相关的通用回调
void CIGWLoaderDx9::OnCommunityEventA(int EventType, LPCSTR Target)
{
    if (m_pCommunityInterfaceA == NULL)
        return;

    switch(EventType)
    {
    case ceSetGuildTitle:
        if (m_pCommunityInterfaceA->OnSetGuildTitle)
            m_pCommunityInterfaceA->OnSetGuildTitle(Target);
        break;
    case ceAddFriend:
        if (m_pCommunityInterfaceA->OnAddFriend)
            m_pCommunityInterfaceA->OnAddFriend(Target);
        break;
    case ceRemoveFriend:
        if (m_pCommunityInterfaceA->OnRemoveFriend)
            m_pCommunityInterfaceA->OnRemoveFriend(Target);
        break;
    case ceCreateGuild:
        if (m_pCommunityInterfaceA->OnCreateGuild)
            m_pCommunityInterfaceA->OnCreateGuild(Target);
        break;
    case ceJoinGuild:
        if (m_pCommunityInterfaceA->OnJoinGuild)
            m_pCommunityInterfaceA->OnJoinGuild(Target);
        break;
    case ceQuitGuild:
        if (m_pCommunityInterfaceA->OnQuitGuild)
            m_pCommunityInterfaceA->OnQuitGuild(Target);
        break;
    case ceMarried:
        if (m_pCommunityInterfaceA->OnMarried)
            m_pCommunityInterfaceA->OnMarried(Target);
        break;
    case ceDivorced:
        if (m_pCommunityInterfaceA->OnDivorced)
            m_pCommunityInterfaceA->OnDivorced(Target);
        break;
    case ceJoinMaster:
        if (m_pCommunityInterfaceA->OnJoinMaster)
            m_pCommunityInterfaceA->OnJoinMaster(Target);
        break;
    case ceLeaveMaster:
        if (m_pCommunityInterfaceA->OnLeaveMaster)
            m_pCommunityInterfaceA->OnLeaveMaster(Target);
        break;
    } 
}

void CIGWLoaderDx9::OnCommunityEventW(int EventType, LPCWSTR Target)
{
    if (m_pCommunityInterfaceW == NULL)
        return;

    switch(EventType)
    {
    case ceSetGuildTitle:
        if (m_pCommunityInterfaceW->OnSetGuildTitle)
            m_pCommunityInterfaceW->OnSetGuildTitle(Target);
        break;
    case ceAddFriend:
        if (m_pCommunityInterfaceW->OnAddFriend)
            m_pCommunityInterfaceW->OnAddFriend(Target);
        break;
    case ceRemoveFriend:
        if (m_pCommunityInterfaceW->OnRemoveFriend)
            m_pCommunityInterfaceW->OnRemoveFriend(Target);
        break;
    case ceCreateGuild:
        if (m_pCommunityInterfaceW->OnCreateGuild)
            m_pCommunityInterfaceW->OnCreateGuild(Target);
        break;
    case ceJoinGuild:
        if (m_pCommunityInterfaceW->OnJoinGuild)
            m_pCommunityInterfaceW->OnJoinGuild(Target);
        break;
    case ceQuitGuild:
        if (m_pCommunityInterfaceW->OnQuitGuild)
            m_pCommunityInterfaceW->OnQuitGuild(Target);
        break;
    case ceMarried:
        if (m_pCommunityInterfaceW->OnMarried)
            m_pCommunityInterfaceW->OnMarried(Target);
        break;
    case ceDivorced:
        if (m_pCommunityInterfaceW->OnDivorced)
            m_pCommunityInterfaceW->OnDivorced(Target);
        break;
    case ceJoinMaster:
        if (m_pCommunityInterfaceW->OnJoinMaster)
            m_pCommunityInterfaceW->OnJoinMaster(Target);
        break;
    case ceLeaveMaster:
        if (m_pCommunityInterfaceW->OnLeaveMaster)
            m_pCommunityInterfaceW->OnLeaveMaster(Target);
        break;
    } 
}

//角色升级回调
void CIGWLoaderDx9::OnLevelUp(int NewLevel)
{
    if (m_pCommunityInterfaceA && m_pCommunityInterfaceA->OnLevelUp)
        m_pCommunityInterfaceA->OnLevelUp(NewLevel);
    if (m_pCommunityInterfaceW && m_pCommunityInterfaceW->OnLevelUp)
        m_pCommunityInterfaceW->OnLevelUp(NewLevel);
}

//角色在行会中的职位改变回调
void CIGWLoaderDx9::OnGuildTitleChangedA(LPCSTR OldTitle, LPCSTR NewTitle, DWORD Change)
{
    if (m_pCommunityInterfaceA && m_pCommunityInterfaceA->OnGuildTitleChanged)
        m_pCommunityInterfaceA->OnGuildTitleChanged(OldTitle, NewTitle, Change);
}

void CIGWLoaderDx9::OnGuildTitleChangedW(LPCWSTR OldTitle, LPCWSTR NewTitle, DWORD Change)
{
    if (m_pCommunityInterfaceW && m_pCommunityInterfaceW->OnGuildTitleChanged)
        m_pCommunityInterfaceW->OnGuildTitleChanged(OldTitle, NewTitle, Change);
}


void CIGWLoaderDx9::SetCommunityInfoA(const PUserCharacterInfoA pCharacter)
{
    if (m_pCommunityInterfaceA && m_pCommunityInterfaceA->SetCharacterInfo)
        m_pCommunityInterfaceA->SetCharacterInfo(pCharacter);
}

void CIGWLoaderDx9::SetCommunityInfoW(const PUserCharacterInfoW pCharacter)
{
    if (m_pCommunityInterfaceW && m_pCommunityInterfaceW->SetCharacterInfo)
        m_pCommunityInterfaceW->SetCharacterInfo(pCharacter);
}

//设置进入场所(房间)的回调
void CIGWLoaderDx9::SetEnterLocationCallback(const PFNChangeLocationA CBFuncA,
                                                const PFNChangeLocationW CBFuncW)
{
    char szTmp[256];
    m_pLocationInterfaceA = new IGWLocationInterfaceA;
    if (igwSupportExA)
    {
        if ( IGW_OK != igwSupportExA(igwLocationInterface, m_pLocationInterfaceA, sizeof(IGWLocationInterfaceA)) )
            IGWSAFE_DELETE(m_pLocationInterfaceA);
    }

    m_pLocationInterfaceW = new IGWLocationInterfaceW;
    if (igwSupportExW)
    {
        if ( IGW_OK != igwSupportExW(igwLocationInterface, m_pLocationInterfaceW, sizeof(IGWLocationInterfaceW)) )
            IGWSAFE_DELETE(m_pLocationInterfaceW);
    }

    if (m_pLocationInterfaceA)
    {
        m_pLocationInterfaceA->Enter = CBFuncA;
        _snprintf(szTmp, sizeof(szTmp), "LocationInterfaceA->Enter function has set on address %X\n", CBFuncA);
        OutputDebugStringA(szTmp);
    }
    if (m_pLocationInterfaceW)
    {
        m_pLocationInterfaceW->Enter = CBFuncW;
        _snprintf(szTmp, sizeof(szTmp), "LocationInterfaceW->Enter function has set on address %X\n", CBFuncW);
        OutputDebugStringA(szTmp);
    }
}

bool CIGWLoaderDx9::InitCreate()
{
	bool tmpResult = false;
	FVisible = true;

	G_IGWExportDx9 = NULL;
	igwInit = NULL;
	igwInitExA = NULL;
	igwInitExW = NULL;
	igwSupportW = NULL;
	igwSupportA = NULL;
	igwSupportExA = NULL;
	igwSupportExW = NULL;
	igwTerminal = NULL;

	bUseIGWSupportEx = false;
	FTUserA = NULL;
	FTUserW = NULL;
	FIGWCustomizeInterfaceA = NULL;
	FIGWCustomizeInterfaceW = NULL;
	FIGWConfigure = NULL;
	FIGWGameAudioSetting = NULL;
	m_pCommunityInterfaceA = NULL;
	m_pCommunityInterfaceW = NULL;
	m_pLocationInterfaceA = NULL;
	m_pLocationInterfaceW = NULL;

	if (!hInstIGWDll)
	{
		WCHAR strExePath[MAX_PATH] = {0};
		WCHAR strExeName[MAX_PATH] = {0};
		WCHAR* strLastSlash = NULL;
		GetModuleFileNameW( NULL, strExePath, MAX_PATH );
		strExePath[MAX_PATH-1]=0;
		strLastSlash = wcsrchr( strExePath, TEXT('\\') );
		if( strLastSlash )
		{
			StringCchCopyW( strExeName, MAX_PATH, &strLastSlash[1] );

			// Chop the exe name from the exe path
			*strLastSlash = 0;

			// Chop the .exe from the exe name
			strLastSlash = wcsrchr( strExeName, TEXT('.') );
			if( strLastSlash )
				*strLastSlash = 0;
		}
		// CPP不熟只好发现什么用什么啦,
		WCHAR strInGameGuiDll[MAX_PATH];
		//GetGameWidgetPath((LPBYTE)strInGameGuiDll, MAX_PATH);

		if ( !(GetFileAttributesW(strInGameGuiDll) != 0xFFFFFFFF) )
		{
			StringCchPrintfW( strInGameGuiDll, MAX_PATH, L"%s\\GameWidget.dll", strExePath );
		}
		if ( !(GetFileAttributesW(strInGameGuiDll) != 0xFFFFFFFF) )
		{
			StringCchPrintfW( strInGameGuiDll, MAX_PATH, L"%s\\GameWidget\\GameWidget.dll", strExePath );
		}


		if ( GetFileAttributesW(strInGameGuiDll) != 0xFFFFFFFF )
		{
			//hInstIGWDll = LoadLibraryW( strInGameGuiDll );
			// 安全加载dll
			hInstIGWDll = SafeLoadLibraryW( strInGameGuiDll );

			if (hInstIGWDll)
			{
				// 获取基本的接口函数
				igwInit = (PFNigwInit)GetProcAddress( hInstIGWDll, "igwInit" );
				igwInitExA = (PFNigwInitExA)GetProcAddress( hInstIGWDll, "igwInitExA" );
				igwInitExW = (PFNigwInitExW)GetProcAddress( hInstIGWDll, "igwInitExW" );
				igwTerminal = (PFNigwTerminal)GetProcAddress( hInstIGWDll, "igwTerminal" );

				igwSupportW = (PFNigwSupportW)GetProcAddress( hInstIGWDll, "igwSupportW" );
				igwSupportA = (PFNigwSupportW)GetProcAddress( hInstIGWDll, "igwSupportA" );
				igwSupportExA = (PFNigwSupportExA)GetProcAddress( hInstIGWDll, "igwSupportExA" );
				igwSupportExW = (PFNigwSupportExW)GetProcAddress( hInstIGWDll, "igwSupportExW" );

				// 由于版本问题，这里仅判断部分导出函数，以兼容老版本
				tmpResult = (igwInit && igwTerminal && igwSupportA && igwSupportW);
				if (!tmpResult)
				{
					FreeLibrary(hInstIGWDll);
					hInstIGWDll = 0;
				}
			}			
		}

	}

	return tmpResult;
}

bool CIGWLoaderDx9::GetAllIGWInteface()
{
	bool tmpResult = false;

	if ((igwSupportExA != NULL) && (igwSupportExW != NULL))
	{// 优先使用Ex接口，它对新旧版本的支持要好得多，这组接口应用在0.9.0.42之后的版本才有
		bUseIGWSupportEx = true;
		OutputDebugStringW(L"igwSupportEx \n");
		G_IGWExportDx9 = new IGWExportDx9();
		if ( igwSupportExW(igwGraphicD3D9, G_IGWExportDx9, sizeof(IGWExportDx9)) == IGW_OK )
			OutputDebugStringW(L"G_IGWExportDx9 ok\n");
		else
		{
			delete G_IGWExportDx9;
			G_IGWExportDx9 = NULL;
			OutputDebugStringW(L"G_IGWExportDx9 error\n");
		}


		FTUserA = new IGWUserInterfaceA();
		if ( igwSupportExA(igwUserInterface, FTUserA, sizeof(IGWUserInterfaceA)) == IGW_OK  )
			OutputDebugStringW(L"igwUserInterface A ok\n");
		else
		{
			delete FTUserA;
			FTUserA = NULL;
			OutputDebugStringW(L"igwUserInterface A error\n");
		}
		FTUserW = new IGWUserInterfaceW();
		if ( igwSupportExW(igwUserInterface, FTUserW, sizeof(IGWUserInterfaceW)) == IGW_OK  )
			OutputDebugStringW(L"igwUserInterface W ok\n");
		else
		{
			delete FTUserW;
			FTUserW = NULL;
			OutputDebugStringW(L"igwUserInterface W error\n");
		}

		FIGWConfigure = new IGWConfigureInterface();
		if ( igwSupportExW(igwConfigureInterface, FIGWConfigure, sizeof(IGWConfigureInterface)) == IGW_OK )
			OutputDebugStringW(L"igwConfigureInterface  ok\n");
		else
		{
			delete FIGWConfigure;
			FIGWConfigure = NULL;
			OutputDebugStringW(L"igwConfigureInterface  error\n");
		}

		FIGWCustomizeInterfaceA = new IGWCustomizeInterfaceA();
		if ( igwSupportExA(igwCustomizeInterface, FIGWCustomizeInterfaceA, sizeof(IGWCustomizeInterfaceA)) == IGW_OK )
			OutputDebugStringW(L"igwCustomizeInterface A ok\n");
		else
		{
			delete FIGWCustomizeInterfaceA;
			FIGWCustomizeInterfaceA = NULL;
			OutputDebugStringW(L"igwCustomizeInterface A error\n");
		}
		FIGWCustomizeInterfaceW = new IGWCustomizeInterfaceW();
		if ( igwSupportExW(igwCustomizeInterface, FIGWCustomizeInterfaceW, sizeof(IGWCustomizeInterfaceW)) == IGW_OK )
			OutputDebugStringW(L"igwCustomizeInterface W ok\n");
		else
		{
			delete FIGWCustomizeInterfaceW;
			FIGWCustomizeInterfaceW = NULL;
			OutputDebugStringW(L"igwCustomizeInterface W error\n");
		}

		////社群接口
		m_pCommunityInterfaceA = new IGWUserCommunityInterfaceA;
		if (igwSupportExA(igwUserCommunityInterface, m_pCommunityInterfaceA, sizeof(IGWUserCommunityInterfaceA)) == IGW_OK)
			OutputDebugStringW(L"IGWUserCommunityInterface A OK\n");
		else{
			delete m_pCommunityInterfaceA;
			m_pCommunityInterfaceA = NULL;
			OutputDebugStringW(L"IGWUserCommunityInterface A error\n");
		}

		m_pCommunityInterfaceW = new IGWUserCommunityInterfaceW;
		if (igwSupportExW(igwUserCommunityInterface, m_pCommunityInterfaceW, sizeof(IGWUserCommunityInterfaceW)) == IGW_OK)
			OutputDebugStringW(L"IGWUserCommunityInterface W OK\n");
		else{
			delete m_pCommunityInterfaceW;
			m_pCommunityInterfaceW = NULL;
			OutputDebugStringW(L"IGWUserCommunityInterface W error\n");
		}
	}
	else
	{
		if ((igwSupportW != NULL) && (igwSupportA != NULL))
		{
			bUseIGWSupportEx = false;
			OutputDebugStringW(L"igwSupport \n");
			// 获取函数列表
			if ( igwSupportW(igwGraphicD3D9, &G_IGWExportDx9) == IGW_OK )
			{
				OutputDebugStringW(L"G_IGWExportDx9 ok\n");
			}

			if ( this->igwSupportA ( igwUserInterface, &this->FTUserA ) == IGW_OK )
				OutputDebugStringW(L"igwUserInterface A ok\n");

			if ( this->igwSupportW ( igwUserInterface, &this->FTUserW ) == IGW_OK )
				OutputDebugStringW(L"igwUserInterface W ok\n");

			if ( this->igwSupportA( igwConfigureInterface, &this->FIGWConfigure ) == IGW_OK )
				OutputDebugStringW(L"igwConfigureInterface  ok\n");

			if ( this->igwSupportW ( igwCustomizeInterface, &this->FIGWCustomizeInterfaceW ) == IGW_OK )
				OutputDebugStringW(L"igwCustomizeInterface W ok\n");

			if ( this->igwSupportA ( igwCustomizeInterface, &this->FIGWCustomizeInterfaceA ) == IGW_OK )
				OutputDebugStringW(L"igwCustomizeInterface A ok\n");

		}
	}
	tmpResult = ( G_IGWExportDx9 != NULL );
	return tmpResult;
}

void CIGWLoaderDx9::SetVisible( const bool Value )
{
	FVisible = Value;	
}

bool CIGWLoaderDx9::GetVisible()
{
	return FVisible;
}