#pragma once
#pragma pack(push,1)

const static BYTE IGW_DICTIONARY[256] =
{
    0xF0, 0x1F, 0x0B, 0x64, 0xCD, 0x34, 0x4D, 0x10, 0x8C, 0xA5, 0xA3, 0xD9, 0x8D, 0xDC, 0xCF, 0xD1,
        0x4A, 0x76, 0x95, 0x07, 0x13, 0x3D, 0x4B, 0xDD, 0xBF, 0x5F, 0x44, 0x7D, 0xFB, 0xD5, 0x8B, 0x04,
        0x5A, 0x6F, 0xF1, 0x85, 0x42, 0x99, 0x4E, 0xE6, 0xA4, 0x8C, 0xE6, 0x83, 0xBB, 0x11, 0xF1, 0xE3,
        0xA0, 0xE2, 0x8D, 0x03, 0xAE, 0xE1, 0x49, 0x1A, 0xA1, 0x34, 0xDC, 0xDD, 0x84, 0x8B, 0xBA, 0x8A,
        0x65, 0xB8, 0x68, 0x0B, 0xD3, 0x0C, 0x44, 0xD8, 0x85, 0xA3, 0xA3, 0x2C, 0xD7, 0xDE, 0xA5, 0x44,
        0x39, 0x43, 0x42, 0x70, 0x84, 0x4D, 0x44, 0xE8, 0xB0, 0x86, 0x61, 0x33, 0x19, 0x92, 0x55, 0xAF,
        0x06, 0x16, 0x61, 0xF1, 0x53, 0x7C, 0x44, 0x72, 0x9D, 0x74, 0xAD, 0x08, 0xEC, 0xAE, 0x1E, 0x23,
        0x5E, 0xAD, 0x93, 0xD2, 0x4E, 0x76, 0x44, 0x49, 0x8F, 0x71, 0x38, 0x8B, 0xD5, 0x4B, 0xA7, 0x79,
        0x0E, 0xC6, 0x08, 0xD0, 0xC7, 0x16, 0x40, 0x7B, 0x8D, 0x1A, 0x68, 0x38, 0x9F, 0xC5, 0x17, 0x67,
        0xF6, 0x80, 0x5E, 0x9F, 0xD3, 0x45, 0x4F, 0x8B, 0xBD, 0x56, 0x4E, 0xE0, 0x1B, 0xC5, 0x2D, 0xD0,
        0x6F, 0x83, 0x35, 0xB8, 0xE1, 0x88, 0x46, 0x59, 0xA7, 0xDE, 0x9C, 0x17, 0xEE, 0x31, 0x8C, 0x6F,
        0x48, 0xF3, 0x6F, 0xCA, 0x88, 0x87, 0x46, 0x5D, 0xB1, 0xA7, 0x4E, 0x14, 0xC7, 0x0F, 0x79, 0x8A,
        0xAB, 0xB2, 0xB7, 0xCB, 0xF5, 0xB5, 0x4A, 0x62, 0xA7, 0xEF, 0xA0, 0xBF, 0x8B, 0x6D, 0xAB, 0x8C,
        0xB2, 0xA6, 0x77, 0xFD, 0x4E, 0xC9, 0x4F, 0xE4, 0xBD, 0xAE, 0x28, 0x1B, 0xD9, 0xAA, 0x8E, 0xC2,
        0x9D, 0x42, 0x28, 0xF6, 0x2E, 0x0A, 0x4C, 0x5C, 0x8D, 0x86, 0xDD, 0x4A, 0xD1, 0xD6, 0x7C, 0x74,
        0xE4, 0x0B, 0xBF, 0x2E, 0xAA, 0x03, 0x49, 0x6E, 0xA6, 0xB1, 0x89, 0xEF, 0xB3, 0x15, 0x95, 0x79
};

// 当前 IGW 接口版本
#define IGW_VERSION                 0x010

// IGW 返回值
#define IGW_OK                      0
#define IGW_FALSE                   1

// IGW 功能组代码定义
// 图形接口
#define igwGraphicGDI               0x0000
#define igwGraphicDDraw             0x0001
#define igwGraphicD3D8              0x0002
#define igwGraphicD3D9              0x0003
#define igwGraphicD3D10             0x0004
#define igwGraphicOpenGL            0x0005
// 用户接口
#define igwUserInterface            0x0100
// 用户社群接口
#define igwUserCommunityInterface   0x0101

// IGW配置接口 (用于动态配置IGW的一些特性
#define igwConfigureInterface       0x0200

// 为游戏定制的接口 
#define igwCustomizeInterface		0x0400
// 位置信息接口
#define igwLocationInterface        0x0401

// 游戏功能接口
#define igwGameSetting      		0x0500
#define igwGameAudioSetting 		0x0501

#define igwDirectDraw  0x0001
#define igwDirectDraw2 0x0002
#define igwDirectDraw3 0x0003
#define igwDirectDraw4 0x0004
#define igwDirectDraw7 0x0005

// 以下为图形接口定义



// 以下为用户接口定义

// 游戏类型
#define GAMETYPE_UNKNOWN            0
#define GAMETYPE_GDI                1
#define GAMETYPE_DDRAW              2
#define GAMETYPE_D3D8               3
#define GAMETYPE_D3D9               4
#define GAMETYPE_D3D10              5
#define GAMETYPE_OPENGL             6

// 登录类型
#define LOGINTYPE_PTACCOUNT         0    // 盛大通行证 + 密码//彩虹岛
#define LOGINTYPE_PTNUMID           1    // 数字ID + 密码
#define LOGINTYPE_PTACCOUNTWITHEKEY 2    // 盛大通行证 + 密码 + 密宝//彩虹岛
#define LOGINTYPE_PTNUMIDWITHEKEY   3    // 数字ID + 密码 + 密宝
#define LOGINTYPE_SSOACCESSKEY      4    // SSO AccessKey


// IGW屏幕状态类型
#define igwScreenNone    0      // Screen没有被创建
#define igwScreenNormal  1      // 正常显示状态
#define igwScreenMini    2      // 最小化状态

// 以下记录体中的 LPCSTR/LPCWSTR 指向的字符串，如果没有特别说明，其长度
// 最大限制为128字符(包括0结尾在内，注意是字符而不是字节)，超长将被截断

// 游戏基本信息
typedef struct _GameBasicInfoA {
    DWORD   cbSize;       // sizeof(GameBasicInfoA) 
    int     GameID;       // 盛大统一游戏ID
    LPCSTR  GameName;     // 游戏名称
    LPCSTR  GameVer;      // 游戏版本
    int     GameType;     // 游戏类型
    int     MaxUser;      // 游戏同时支持用户数目，通常为1
    // 2008.4.9
    LPCSTR  SPID;         // SPID
} GameBasicInfoA, *PGameBasicInfoA, *LPGameBasicInfoA;

typedef struct _GameBasicInfoW {
    DWORD   cbSize;       // sizeof(GameBasicInfoW) 
    int     GameID;       // 盛大统一游戏ID
    LPCWSTR GameName;     // 游戏名称
    LPCWSTR GameVer;      // 游戏版本
    int     GameType;     // 游戏类型
    int     MaxUser;      // 游戏同时支持用户数目，通常为1
    // 2008.4.9
    LPCWSTR SPID;         // SPID
} GameBasicInfoW, *PGameBasicInfoW, *LPGameBasicInfoW;

// 游戏基本信息(Ex版, igwInitEx使用)
typedef struct _GameBasicInfoExA {
	DWORD		cbSize;			// = SizeOf(TGameBasicInfoA)
	int			GameID;			// 盛大统一游戏ID
	LPCSTR		GameName;		// 游戏名称
	LPCSTR		GameVer;		// 游戏版本
	int			GameType;		// 游戏类型
	int			MaxUser;		// 游戏同时支持用户数目，通常为1
	LPCSTR		GameArea;		// 游戏所在区名称		(尽可能要有)
	LPCSTR		GameServer;		// 游戏所在服务器名称	(尽可能要有)
	int			GameAreaID;		// 游戏区ID				(注意：不可用时传-1)
	int			GameServerID;	// 游戏区服务器ID		(注意：不可用时传-1)
} GameBasicInfoExA, *PGameBasicInfoExA, *LPGameBasicInfoExA;

typedef struct _GameBasicInfoExW {
	DWORD		cbSize;			// = SizeOf(TGameBasicInfoA)
	int			GameID;			// 盛大统一游戏ID
	LPCWSTR		GameName;		// 游戏名称
	LPCWSTR		GameVer;		// 游戏版本
	int			GameType;		// 游戏类型
	int			MaxUser;		// 游戏同时支持用户数目，通常为1
	LPCWSTR		GameArea;		// 游戏所在区名称		(尽可能要有)
	LPCWSTR		GameServer;		// 游戏所在服务器名称	(尽可能要有)
	int			GameAreaID;		// 游戏区ID				(注意：不可用时传-1)
	int			GameServerID;	// 游戏区服务器ID		(注意：不可用时传-1)
} GameBasicInfoExW, *PGameBasicInfoExW, *LPGameBasicInfoExW;


// 用户基本信息（登录用）
typedef struct _UserBasicInfoA {
    DWORD   cbSize;                    // sizeof(UserBasicInfoA)
    LPCSTR  PTAccount;                 // 用户账号
    LARGE_INTEGER PTNumID;             // 用户数字ID
    BYTE    Password[64];              // 用户密码
    LPCSTR  EKey;                      // 密宝
    BYTE    SSOAccessKey[128];         // SSO AccessKey
} UserBasicInfoA, *PUserBasicInfoA, *LPUserBasicInfoA;

typedef struct _UserBasicInfoW {
    DWORD   cbSize;                    // sizeof(UserBasicInfoW)
    LPCWSTR PTAccount;                 // 用户账号
    LARGE_INTEGER PTNumID;             // 用户数字ID
    BYTE    Password[64];              // 用户密码
    LPCWSTR EKey;                      // 密宝
    BYTE    SSOAccessKey[128];         // SSO AccessKey
} UserBasicInfoW, *PUserBasicInfoW, *LPUserBasicInfoW;

// 用户详细信息
typedef struct _UserDetailInfoA {
    DWORD   cbSize;                    // sizeof(UserDetailInfoA)
    LPCSTR  NickName;                  // 昵称
    int     Gender;                    // 性别(0: 女; 1: 男)
    LPCSTR  GameArea;                  // 游戏所在区
    LPCSTR  GameServer;                // 游戏所在服务器
    int     GameAreaID;                // 盛大统一游戏区号ID（默认-1）      // 2008.3.5 增加游戏区服数字ID信息
    int     GameServerID;              // 盛大统一游戏服务器ID（默认-1）
    // more
} UserDetailInfoA, *PUserDetailInfoA, *LPUserDetailInfoA;

typedef struct _UserDetailInfoW {
    DWORD   cbSize;                    // sizeof(UserDetailInfo)
    LPCWSTR NickName;                  // 昵称
    int     Gender;                    // 性别(0: 女; 1: 男)
    LPCWSTR GameArea;                  // 游戏所在区
    LPCWSTR GameServer;                // 游戏所在服务器
    int     GameAreaID;                // 盛大统一游戏区号ID（默认-1）      // 2008.3.5 增加游戏区服数字ID信息
    int     GameServerID;              // 盛大统一游戏服务器ID（默认-1）
    // more
} UserDetailInfoW, *PUserDetailInfoW, *LPUserDetailInfoW;

// 用户位置信息(由 Path + Room 组成，可以类比为定位文件的 Path + File)
// 注：Path/Room是由游戏自定义的内部数据，游戏自己理解即可
//     PathTitle/RoomTitle是显示给用户看的标题
typedef struct _UserLocationA {
    LPCSTR  Path;                      // 路径唯一标识(如果有多级，用"/"做分隔符)
    LPCSTR  PathTitle;                 // 路径标题(如果有多级，用"/"做分隔符)
    LPCSTR  Room;                      // 房间唯一标识
    LPCSTR  RoomTitle;                 // 房间标题
} UserLocationA, *PUserLocationA, *LPUserLocationA;

typedef struct _UserLocationW {
    LPCWSTR Path;                      // 路径唯一标识(如果有多级，用"/"做分隔符)
    LPCWSTR PathTitle;                 // 路径标题(如果有多级，用"/"做分隔符)
    LPCWSTR Room;                      // 房间唯一标识
    LPCWSTR RoomTitle;                 // 房间标题
} UserLocationW, *PUserLocationW, *LPUserLocationW;

// 用户社群信息
typedef struct _UserCharacterInfoA{
    LPCSTR      ChracterName;        // 角色名称
    DWORD       Gender;              // 性别(0: 女; 1: 男)
    LPCSTR      Occupation;          // 角色职业
    int         Level;               // 角色等级
    LPCSTR      GuildName;           // 所在行会
    LPCSTR      GuildTitle;          // 在行会中的头衔
}UserCharacterInfoA, *PUserCharacterInfoA;

typedef struct _UserCharacterInfoW{
    LPCWSTR     ChracterName;        // 角色名称
    DWORD       Gender;              // 性别(0: 女; 1: 男)
    LPCWSTR     Occupation;          // 角色职业
    int         Level;               // 角色等级
    LPCWSTR     GuildName;           // 所在行会
    LPCWSTR     GuildTitle;          // 在行会中的头衔
}UserCharacterInfoW, *PUserCharacterInfoW;


// GameWidget.dll中导出的接口的类型定义
typedef DECLSPEC_IMPORT int (WINAPI* PFNigwInit)(DWORD dwVersion);
typedef DECLSPEC_IMPORT int (WINAPI* PFNigwInitExA)(DWORD dwVersion,  const LPGameBasicInfoExA GameInfo );
typedef DECLSPEC_IMPORT int (WINAPI* PFNigwInitExW)(DWORD dwVersion,  const LPGameBasicInfoExW GameInfo );
typedef DECLSPEC_IMPORT int (WINAPI* PFNigwSupportA)(int nCode, void* FuncGroup);
typedef DECLSPEC_IMPORT int (WINAPI* PFNigwSupportW)(int nCode, void* FuncGroup);
typedef DECLSPEC_IMPORT int (WINAPI* PFNigwSupportExA)(int nCode, void* FuncGroup, int FuncGroupSize);
typedef DECLSPEC_IMPORT int (WINAPI* PFNigwSupportExW)(int nCode, void* FuncGroup, int FuncGroupSize);
typedef DECLSPEC_IMPORT int (WINAPI* PFNigwTerminal)();


// 初始化游戏基本信息
typedef DECLSPEC_IMPORT void (WINAPI* PFNInitGameInfoA)(const LPGameBasicInfoA GameInfo);
typedef DECLSPEC_IMPORT void (WINAPI* PFNInitGameInfoW)(const LPGameBasicInfoW GameInfo);
// 登录，在游戏登录成功后调用
// LoginType: 登录类型，请参考上面LOGINTYPE的常量定义
typedef DECLSPEC_IMPORT void (WINAPI* PFNLoginA)(const PUserBasicInfoA UserInfo, int LoginType);
typedef DECLSPEC_IMPORT void (WINAPI* PFNLoginW)(const PUserBasicInfoW UserInfo, int LoginType);
// 登出，在游戏登出后调用
typedef DECLSPEC_IMPORT void (WINAPI* PFNLogoutA)(void);
typedef DECLSPEC_IMPORT void (WINAPI* PFNLogoutW)(void);
//更改玩家角色信息，登录成功后玩家角色信息可用时调用
typedef DECLSPEC_IMPORT void (WINAPI* PFNChangeUserInfoA)(const PUserDetailInfoA UserInfo);
typedef DECLSPEC_IMPORT void (WINAPI* PFNChangeUserInfoW)(const PUserDetailInfoW UserInfo);
// 更改位置信息
typedef void (WINAPI* PFNChangeLocationA)(const PUserLocationA Location);
typedef void (WINAPI* PFNChangeLocationW)(const PUserLocationW Location);


// 角色升级事件
typedef void (WINAPI* pfnLevelUpEventA)(int Level);
typedef void (WINAPI* pfnLevelUpEventW)(int Level);
// 角色头衔改变事件
typedef void (WINAPI* pfnGuildTitleChangeEventA) (
    LPCSTR OldTitle,    //原头衔
    LPCSTR NewTitle,    //新头衔
    DWORD Change        //0:忽略；1:升职；2:降职
    ); 
typedef void (WINAPI* pfnGuildTitleChangeEventW) (LPCWSTR OldTitle, LPCWSTR NewTitle, DWORD Change); 
// 获得角色称号（头衔）事件
typedef void (WINAPI* pfnSetGuildTitleEventA) (LPCSTR Title); 
typedef void (WINAPI* pfnSetGuildTitleEventW) (LPCWSTR Title); 
// 创立/加入/退出行会事件
typedef void (WINAPI* pfnGuildRelationChangeEventA) (LPCSTR GuildName); 
typedef void (WINAPI* pfnGuildRelationChangeEventW) (LPCWSTR GuildName); 
// 改变与目标的关系的事件，包括
//   增加/删除好友事件
//   拜师/收徒
//   结婚/离婚事件
typedef void (WINAPI* pfnChangeRelationshipEventA)(LPCSTR Target); 
typedef void (WINAPI* pfnChangeRelationshipEventW)(LPCWSTR Target); 
// 获得用户社区信息
typedef void (WINAPI* pfnSetCharacterInfoA)(const PUserCharacterInfoA CharacterInfo);
typedef void (WINAPI* pfnSetCharacterInfoW)(const PUserCharacterInfoW CharacterInfo);

//-------------------------------------------------------------------------------------------
// 第2种登录方式

typedef struct LOGINRESULT {
    DWORD dwSize;                      // sizeof(LoginResult)，为以后扩充提供可能           
    char token[1024];
    char sdid[20];
    char idcard[20];

    LOGINRESULT()
    {
        dwSize = sizeof(LOGINRESULT);
        memset(token, 0, 1024);
        memset(sdid, 0, 20);
        memset(idcard, 0, 20);
    }
} LoginResult, *PLoginResult;

// 登录回调函数
// ErrorCode:     登录返回错误代码
// LoginResult:   登录返回结果，只在 ErrorCode = LOGIN_OK 时才有效
// UserData:      在调用ShowLoginDialog时传入的用户数据
// Reserved:      保留，暂不使用，目前总是为0(不等于ShowLoginDialog中传入的Reserved参数)
// 返回值:        为True将关闭对话框，为False不关闭
typedef BOOL (CALLBACK* LOGINCALLBACKPROC)(int ErrorCode, const PLoginResult LoginResult, int UserData, int Reserved);

// 显示登录对话框
// Callback:      回调函数
// UserData:      用户数据，在Callback中传回
// Reserved:      保留，暂不使用，必须为0
// 返回值:        为0表示成功
typedef DECLSPEC_IMPORT int (WINAPI* PFNShowLoginDialog)(const LOGINCALLBACKPROC Callback, int UserData, int Reserved);
// 显示充值对话框
// Reserved1:     保留，暂不使用，必须为0
// Reserved2:     保留，暂不使用，必须为0
// 返回值:        为0表示成功
typedef DECLSPEC_IMPORT int (WINAPI* PFNShowPaymentDialog)(int Reserved1, int Reserved2);
// 显示支付对话框
// gameUserId:    玩家游戏帐号
// spSiteCode:    Shop Server的唯一编码. 是BillingGate System统一分配的编码
// orderId:       订单号码，标识唯一的一次数据请求
// payInfo:       付费信息(购买道具费用，现在只支持盛大点卷(3)货币类型)：格式：3|amount
// goodsNum:      有效道具总数
// goodsInfo:     道具详细信息,采用字段的形式表示itemid,itemnum,itemattr#...... itemid,itemnum,itemattr。
// signedString:  签名规则：按照字符串的方式将uidType + userId + gameUserID + spSiteCode + payInfo + goodsNum + goodsInfo + 加密密码 相加，然后使用MD5算法对这些信息进行加密的值。加密密码:BillingGate System统一分配的跟spSiteCode对应的编码
// sendItemUrl:   通知发道具的回调webservice的Url
// reserveInt:    扩展字段，可不填写
// reserveString: 扩展字段，可不填写
// 返回值:        为0表示成功
typedef DECLSPEC_IMPORT int (WINAPI* PFNShowBillingDialog)(const BSTR gameUserId,
                                                           const BSTR spSiteCode, const BSTR orderId, const BSTR payInfo, int goodsNum,
                                                           const BSTR goodsInfo, const BSTR signedString, const BSTR sendItemUrl,
                                                           int reserveInt, const BSTR reserveString);
// 获取错误描述信息
typedef DECLSPEC_IMPORT void (WINAPI* PFNGetErrorDesc)(int ErrorCode, BSTR& Desc);


//-------------------------------------------------------------------------------------------
// IGW配置接口相关
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetSelfCursor)(const bool value);
typedef DECLSPEC_IMPORT bool (WINAPI* PFNGetSelfCursor)();

// 注：IGW屏幕包括接入后IGW所有显示的可视元素
// 获取IGW当前屏幕状态，值为上面定义的igwScreenNone、igwScreenNormal、igwScreenMini
typedef DECLSPEC_IMPORT int (WINAPI* PFNGetScreenStatus)();
// 设置屏幕状态参数value值为igwScreenNormal、igwScreenMini
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetScreenStatus)(const int value);
// 获取当前屏幕可用状态,false为不可用，true为可用
typedef DECLSPEC_IMPORT bool (WINAPI* PFNGetScreenEnabled)();
// 设置当前屏幕可用状态,false为不可用，true为可用
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetScreenEnabled)(const bool value);
// 获取当前TaskBar的位置 p不能为NULL
typedef DECLSPEC_IMPORT bool (WINAPI* PFNGetTaskBarPosition)(PPOINT p);
// 设置当前TaskBar的位置
typedef DECLSPEC_IMPORT bool (WINAPI* PFNSetTaskBarPosition)(const POINT p);
typedef DECLSPEC_IMPORT bool (WINAPI* PFNGetFocus)();
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetFocus)(const bool value);



// IGW UserInterface
typedef struct _IGWUserInterfaceA {
    PFNInitGameInfoA   InitGameInfo;     // 初始化游戏基本信息
    PFNLoginA          Login;            // 登录
    PFNLogoutA         Logout;           // 登出
    PFNChangeUserInfoA ChangeUserInfo;   // 初始化玩家角色详细信息
    PFNChangeLocationA ChangeLocation;   // 位置信息改变
    // 2008.4.9 增加第2种登录方式
    PFNShowLoginDialog   ShowLoginDialog;     // 登录
    PFNShowPaymentDialog ShowPaymentDialog; // 充值
    PFNShowBillingDialog ShowBillingDialog; // 支付
    PFNGetErrorDesc      GetErrorDesc;        // 获取错误描述信息
} IGWUserInterfaceA, *PIGWUserInterfaceA, *LPIGWUserInterfaceA;

typedef struct _IGWUserInterfaceW {
    PFNInitGameInfoW   InitGameInfo;     // 初始化游戏基本信息
    PFNLoginW          Login;            // 登录
    PFNLogoutW         Logout;           // 登出
    PFNChangeUserInfoW ChangeUserInfo;   // 初始化玩家角色详细信息
    PFNChangeLocationW ChangeLocation;   // 位置信息改变
    // 2008.4.9 增加第2种登录方式
    PFNShowLoginDialog   ShowLoginDialog;   // 登录
    PFNShowPaymentDialog ShowPaymentDialog; // 充值
    PFNShowBillingDialog ShowBillingDialog; // 支付
    PFNGetErrorDesc      GetErrorDesc;      // 获取错误描述信息
} IGWUserInterfaceW, *PIGWUserInterfaceW, *LPIGWUserInterfaceW;


// IGW Configure
typedef struct _IGWConfigureInterface{
    PFNGetSelfCursor		GetSelfCursor;      // 得到当前IGW的鼠标指针绘制状态 true为自绘 false为不处理
    PFNSetSelfCursor		SetSelfCursor;      // 设置IGW鼠标绘制状态，参数同上
    PFNGetScreenStatus		GetScreenStatus;    // 获取IGW屏幕状态，值为上面定义的igwScreenNone、igwScreenNormal、igwScreenMini
    PFNSetScreenStatus		SetScreenStatus;    // 设置屏幕状态参数value值为igwScreenNormal、igwScreenMini
    PFNGetScreenEnabled		GetScreenEnabled;   // 获取屏幕可用状态
    PFNSetScreenEnabled		SetScreenEnabled;   // 设置屏幕可用状态
    PFNGetTaskBarPosition	GetTaskBarPosition; // 获取任务控制条的位置
    PFNSetTaskBarPosition	SetTaskBarPosition; // 设置任务控制条的位置
    PFNGetFocus				GetFocus;			// 获取当前IGW是否处于焦点状态true为焦点状态，false没有焦点状态
    PFNSetFocus				SetFocus;			// 设置IGW当前是来有焦点true为有焦点，false则无焦点
} IGWConfigureInterface, *PIGWConfigureInterface, *LPIGWConfigureInterface;


// 08-05-15 社群、社交方面的接口
typedef struct _IGWUserCommunityInterfaceA{
    pfnSetCharacterInfoA           SetCharacterInfo;     //设置角色信息
    pfnLevelUpEventA               OnLevelUp;            // 升级
    pfnGuildTitleChangeEventA      OnGuildTitleChanged;  // 行会头衔改变
    pfnSetGuildTitleEventA         OnSetGuildTitle;      // 获得行会头衔
    pfnChangeRelationshipEventA    OnAddFriend;          // 增加好友
    pfnChangeRelationshipEventA    OnRemoveFriend;       // 删除好友
    pfnGuildRelationChangeEventA   OnCreateGuild;        // 创建行会
    pfnGuildRelationChangeEventA   OnJoinGuild;          // 加入行会
    pfnGuildRelationChangeEventA   OnQuitGuild;          // 退出行会
    pfnChangeRelationshipEventA    OnMarried;            // 结婚
    pfnChangeRelationshipEventA    OnDivorced;           // 离婚
    pfnChangeRelationshipEventA    OnJoinMaster;         // 拜师
    pfnChangeRelationshipEventA    OnLeaveMaster;        // 解释师徒关系
}IGWUserCommunityInterfaceA, *PIGWUserCommunityInterfaceA, *LPIGWUserCommunityInterfaceA;

typedef struct _IGWUserCommunityInterfaceW{
    pfnSetCharacterInfoW           SetCharacterInfo;     //设置角色信息
    pfnLevelUpEventW               OnLevelUp;            // 升级
    pfnGuildTitleChangeEventW      OnGuildTitleChanged;  // 行会头衔改变
    pfnSetGuildTitleEventW         OnSetGuildTitle;      // 获得行会头衔
    pfnChangeRelationshipEventW    OnAddFriend;          // 增加好友
    pfnChangeRelationshipEventW    OnRemoveFriend;       // 删除好友
    pfnGuildRelationChangeEventW   OnCreateGuild;        // 创建行会
    pfnGuildRelationChangeEventW   OnJoinGuild;          // 加入行会
    pfnGuildRelationChangeEventW   OnQuitGuild;          // 退出行会
    pfnChangeRelationshipEventW    OnMarried;            // 结婚
    pfnChangeRelationshipEventW    OnDivorced;           // 离婚
    pfnChangeRelationshipEventW    OnJoinMaster;         // 拜师
    pfnChangeRelationshipEventW    OnLeaveMaster;        // 解释师徒关系
}IGWUserCommunityInterfaceW, *PIGWUserCommunityInterfaceW, *LPIGWUserCommunityInterfaceW;

// 位置信息接口
typedef struct _IGWLocationInterfaceA{
    PFNChangeLocationA Enter;       // 进入位置（房间）
}IGWLocationInterfaceA, *PIGWLocationInterfaceA, *LPIGWLocationInterfaceA;

typedef struct _IGWLocationInterfaceW{
    PFNChangeLocationW Enter;       // 进入位置（房间）
}IGWLocationInterfaceW, *PIGWLocationInterfaceW, *LPIGWLocationInterfaceW;


// 定制的扩展接口类型定义
#define igwWindowsNone		0      // 窗口没有被创建
#define igwWindowsShow		1      // 窗口显示状态
#define igwWindowsHide		2      // 窗口隐藏
// 窗口类型
#define igwWindowsType_None		0	// 未知类型
#define igwWindowsType_ToolBar	1	// 工具栏
#define igwWindowsType_Widget	2  // Widget窗口

// 窗口属性结构定义
typedef struct _IGWWinProperty{
    int Type;          // 窗口类型(只用于标识窗口类型，设置无效)
    int Status;        // 窗口状态
    // 以下是位置、宽高信息
    int Left;
    int Top;
    int Width;
    int Height;	
} IGWWinProperty, *PIGWWinProperty, *LPIGWWinProperty;


typedef DECLSPEC_IMPORT int (WINAPI* PFNGetWinPropertyA)(LPCSTR WinName, PIGWWinProperty WinProperty);
typedef DECLSPEC_IMPORT int (WINAPI* PFNSetWinPropertyA)(LPCSTR WinName, PIGWWinProperty WinProperty);
typedef DECLSPEC_IMPORT int (WINAPI* PFNGetWinPropertyW)(LPCWSTR WinName, PIGWWinProperty WinProperty);
typedef DECLSPEC_IMPORT int (WINAPI* PFNSetWinPropertyW)(LPCWSTR WinName, PIGWWinProperty WinProperty);

typedef DECLSPEC_IMPORT int (WINAPI* PFNOpenWidgetA)(LPCSTR WidgetNameSpace);
typedef DECLSPEC_IMPORT int (WINAPI* PFNOpenWidgetW)(LPCWSTR WidgetNameSpace);
typedef DECLSPEC_IMPORT int (WINAPI* PFNCloseWidgetA)(LPCSTR WidgetNameSpace);
typedef DECLSPEC_IMPORT int (WINAPI* PFNCloseWidgetW)(LPCWSTR WidgetNameSpace);


typedef struct _IGWCustomizeInterfaceA{
    PFNGetWinPropertyA		GetWinProperty;
    PFNSetWinPropertyA		SetWinProperty;
    PFNOpenWidgetA			OpenWidget;
    PFNCloseWidgetA			CloseWidget;
} IGWCustomizeInterfaceA, *PIGWCustomizeInterfaceA, *LPIGWCustomizeInterfaceA;

typedef struct _IGWCustomizeInterfaceW{
    PFNGetWinPropertyW		GetWinProperty;
    PFNSetWinPropertyW		SetWinProperty;
    PFNOpenWidgetW			OpenWidget;
    PFNCloseWidgetW			CloseWidget;
} IGWCustomizeInterfaceW, *PIGWCustomizeInterfaceW, *LPIGWCustomizeInterfaceW;


// 注意音量值的有效范围在0-100
typedef DECLSPEC_IMPORT void (WINAPI* PFNNodifyAudioVolumeChanged)();
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetAudioSoundVolume)(DWORD NewVolume);
typedef DECLSPEC_IMPORT DWORD (WINAPI* PFNGetAudioSoundVolume)();
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetAudioSoundVolume)(DWORD NewVolume);
typedef DECLSPEC_IMPORT DWORD (WINAPI* PFNGetAudioEffectVolume)();
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetAudioEffectVolume)(DWORD NewVolume);

typedef struct _IGWGameAudioSetting{
    PFNNodifyAudioVolumeChanged NodifyAudioVolumeChanged;
    PFNGetAudioSoundVolume		GetAudioSoundVolume;
    PFNSetAudioSoundVolume		SetAudioSoundVolume;
    PFNGetAudioEffectVolume		GetAudioEffectVolume;
    PFNSetAudioEffectVolume		SetAudioEffectVolume;
} IGWGameAudioSetting, *PIGWGameAudioSetting, *LPIGWGameAudioSetting;


#pragma pack(pop)







