#pragma once

#include <d3d9.h>
#include "winbase.h"
#include "IGWInterface.h"

/* 定义GameWidget.dll中的导出函数 */
typedef DECLSPEC_IMPORT bool (WINAPI* PFNInitializeGraphic)(IDirect3DDevice9* Dev9, D3DPRESENT_PARAMETERS* Params, bool HookGameWnd);
typedef DECLSPEC_IMPORT bool (WINAPI* PFNRender)();
//typedef DECLSPEC_IMPORT bool (WINAPI* PFNUpdate)();
typedef DECLSPEC_IMPORT bool (WINAPI* PFNFinalizeGraphic)();
typedef DECLSPEC_IMPORT void (WINAPI* PFNOnDeviceReset)(D3DPRESENT_PARAMETERS* Params);
typedef DECLSPEC_IMPORT void (WINAPI* PFNOnDeviceLost)();
typedef DECLSPEC_IMPORT HRESULT (WINAPI* PFNOnWindowProc)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
typedef DECLSPEC_IMPORT HRESULT (WINAPI* PFNOnWindowProcEx)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lResult);
typedef DECLSPEC_IMPORT HRESULT (WINAPI* PFNRenderEx)(WPARAM wParam, LPARAM lParam);


#pragma pack(push,1)
struct IGWExportDx9
{
	PFNInitializeGraphic		FInitializeGraphic;
	PFNRender					FRender;	
	PFNRenderEx					FRenderEx;
	PFNFinalizeGraphic			FFinalizeGraphic;
	PFNOnDeviceReset			FOnDeviceReset;
	PFNOnDeviceLost				FOnDeviceLost;
	PFNOnWindowProc				FOnIGWWndProc;
	PFNOnWindowProcEx			FOnIGWWndProcEx;
};
#pragma pack(pop)

typedef DECLSPEC_IMPORT bool (WINAPI* PFNGetIGWExportDx9Data)( IGWExportDx9* IGWExportDx9Data );

//社群事件类型常量，参考IGWInterface.h中的IGWUserCommunityInterfaceA定义
enum IGW_CommunityEventType {
    ceSetGuildTitle, ceCreateGuild, ceJoinGuild,
    ceQuitGuild, ceAddFriend, ceRemoveFriend, ceMarried, ceDivorced,
    ceJoinMaster, ceLeaveMaster
};


class CIGWLoaderDx9
{
private:
	static HINSTANCE			hInstIGWDll;
	IGWExportDx9*				G_IGWExportDx9;		// D3D9图形接口函数表
	LPIGWUserInterfaceA			FTUserA ;			// 用户接口表1
	LPIGWUserInterfaceW			FTUserW ;			// 用户接口表2
	LPIGWConfigureInterface		FIGWConfigure;		// 配置接口

	LPIGWCustomizeInterfaceA	FIGWCustomizeInterfaceA;		// 定制的接口1
	LPIGWCustomizeInterfaceW	FIGWCustomizeInterfaceW;		// 定制的接口2
	LPIGWGameAudioSetting		FIGWGameAudioSetting;           // 游戏音量设置接口

    LPIGWUserCommunityInterfaceA m_pCommunityInterfaceA;    //社群接口
    LPIGWUserCommunityInterfaceW m_pCommunityInterfaceW;

    LPIGWLocationInterfaceA     m_pLocationInterfaceA;      //位置信息接口
    LPIGWLocationInterfaceW     m_pLocationInterfaceW;

	// 用于控制是否显示，默认总是显示的
	bool				FVisible;

	PFNigwInit			igwInit;
	PFNigwInitExA		igwInitExA;
	PFNigwInitExW		igwInitExW;
	PFNigwSupportW		igwSupportW;
	PFNigwSupportA		igwSupportA;
	PFNigwSupportExA	igwSupportExA;
	PFNigwSupportExW	igwSupportExW;
	PFNigwTerminal		igwTerminal;	// FreeLibrary前调整释放Dll中分配的资源
	// 用于区分是使用哪组接口
	bool				bUseIGWSupportEx;
	// 清除所有资源
	void CleanupIGWLoader();
	// 初始化构造时需要准备的信息
	bool InitCreate();
	// 得到所有接口信息相关
	bool GetAllIGWInteface();
public:
	CIGWLoaderDx9( const LPGameBasicInfoExA GameInfo );
	CIGWLoaderDx9( const LPGameBasicInfoExW GameInfo );

	~CIGWLoaderDx9(void);

// 
// D3D9图形接口
// 
public:
	// 初始化IGW图形部分
	BOOL InitializeGraphic(IDirect3DDevice9* Dev9, D3DPRESENT_PARAMETERS* Params, bool HookGameWnd);

	// 渲染界面内部已经调用BeginScene和EndScene，确认在EndScene之后调用它
	BOOL Render();

	// 确保在BeginScene、EndScene之间调用RenderEx，参数暂时未定义为扩展保留
	HRESULT RenderEx(WPARAM wParam, LPARAM lParam);

	// 退出IGW时处理
	BOOL FinalizeGraphic();

	// 设备重置
	VOID OnDeviceReset(D3DPRESENT_PARAMETERS* Params);

	// 设备丢失
	VOID OnDeviceLost();

	// 消息处理过程
	HRESULT OnWindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	// 消息处理过程扩展的
	HRESULT OnWindowProcEx( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lResult);

//
// 用户接口
//
public:
	// 初始化游戏信息
	VOID	InitGameInfoA ( const LPGameBasicInfoA GameInfo ) ;
	VOID	InitGameInfoW ( const LPGameBasicInfoW GameInfo ) ;
	
	// 登录，在游戏登录成功后调用( LoginType: 登录类型 )
	VOID	LoginA ( const PUserBasicInfoA UserInfo, int LoginType);
	VOID	LoginW ( const PUserBasicInfoW UserInfo, int LoginType);

	// 登出，在游戏登出后调用
	VOID	LogoutA () ;
	VOID	LogoutW () ;

	//更改玩家角色信息，登录成功后玩家角色信息可用时调用
	VOID	ChangeUserInfoA ( const PUserDetailInfoA UserInfo );
	VOID	ChangeUserInfoW ( const PUserDetailInfoW UserInfo );

	// 更改位置信息
	VOID	ChangeLocationA ( const PUserLocationA pLocation ) ;
	VOID	ChangeLocationW ( const PUserLocationW pLocation ) ;

    //设置社群信息
    void SetCommunityInfoA(const PUserCharacterInfoA pCharacter);
    void SetCommunityInfoW(const PUserCharacterInfoW pCharacter);

    //社群相关的通用回调
    void OnCommunityEventA(int EventType, LPCSTR Target);
    void OnCommunityEventW(int EventType, LPCWSTR Target);
    //角色升级回调
    void OnLevelUp(int NewLevel);
    //角色在行会中的职位改变回调
    void OnGuildTitleChangedA(LPCSTR OldTitle, LPCSTR NewTitle, DWORD Change);
    void OnGuildTitleChangedW(LPCWSTR OldTitle, LPCWSTR NewTitle, DWORD Change);

    //设置进入场所(房间)回调
    void SetEnterLocationCallback(const PFNChangeLocationA CBFuncA, const PFNChangeLocationW CBFuncW);

	void Terminal(); // 清空IGW用到的资源

	// 配置IGW相关
	bool GetSelfCursor();
	void SetSelfCursor(const bool value);
	int GetScreenStatus();
	void SetScreenStatus(const int value);
	bool GetScreenEnabled();
	void SetScreenEnabled(const bool value);
	bool GetTaskBarPosition(PPOINT p);
	bool SetTaskBarPosition(const POINT p);
	bool GetFocus();
	void SetFocus(const bool value);

	// 定制的接口相关
	int GetWinPropertyA(LPCSTR WinName, PIGWWinProperty WinProperty);
	int SetWinPropertyA(LPCSTR WinName, PIGWWinProperty WinProperty);
	int GetWinPropertyW(LPCWSTR WinName, PIGWWinProperty WinProperty);
	int SetWinPropertyW(LPCWSTR WinName, PIGWWinProperty WinProperty);
	// 打开关闭Widget接口
	int OpenWidgetA(LPCSTR WidgetNameSpace);
	int CloseWidgetA(LPCSTR WidgetNameSpace);
	int OpenWidgetW(LPCWSTR WidgetNameSpace);
	int CloseWidgetW(LPCWSTR WidgetNameSpace);

	
	// 游戏实现相关接口
		// 实现游戏音乐和音效控制相关
	bool SetAudioSetting(	PFNGetAudioSoundVolume		AGetAudioSoundVolume,	
							PFNSetAudioSoundVolume		ASetAudioSoundVolume,
							PFNGetAudioEffectVolume		AGetAudioEffectVolume,
							PFNSetAudioEffectVolume		ASetAudioEffectVolume	);
	// 通知IGW相关接口
		// 通知IGW游戏中的音乐或者音效大小改变，它令在SetAudioSet设置成功后才有效
	bool NodifyAudioChanged();

	// 用于控制是否显示IGW，即使Render()、RenderEx()失效
	void SetVisible(const bool Value);
	bool GetVisible();
};

extern CIGWLoaderDx9*	g_pCIGWLoaderDx9;
