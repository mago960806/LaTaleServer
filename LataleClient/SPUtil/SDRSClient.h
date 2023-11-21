/*!
!module 组件名称
!moduletype 组件类型
组件介绍

!inc SDRSClient.h
*/ 

// SDRSClient.h  组件的头文件 
#pragma once
#include "SDBase.h"


namespace SGDP {

// 组件名称
const char SDRSCLIENT_MODULENAME[] = "SDRSClient";

// 组件版本号
const SSGDPVersion	SDRSCLIENT_VERSION = {0,5,0,1};

/*!
!macro SDRSCLIENT_
精灵窗口类型
!inc SDRSClient.h
*/
#define SDRSCLIENT_INGAME		1		// 模拟成游戏内部窗口
#define SDRSCLIENT_POPUP		2		// 表现为漂浮在游戏外部的窗口

/*!
!marco SDRSCLIENT_ERROR_
SDRSClient组件错误代码
!inc SDRSClient.h
*/

#define SDRSCLIENT_ERROR_OK						0				//>=0表示执行成功
#define SDRSCLIENT_ERROR_LOADCONFIG				-1				// -1 表示加载配置文件资源失败	检查配置文件是否存在　格式是否正确
#define SDRSCLIENT_ERROR_NET					-2				// -2 初始化服务器连接时失败　检查DLL是否存在 服务器地址设置是否正确
#define SDRSCLIENT_ERROR_WIN32					-3				// -3 创建Win32窗口时失败了　检查图片资源是否存在

/*!
!marco SDRSCLIENT_ACTION_
SDRSClient事件类型
!inc SDRSClient.h
*/

#define SDRSCLIENT_ACTION_SENDBTN				1				//触发发送问题按钮事件
#define SDRSCLIENT_ACTION_EXITBTN				2				//触发离开按钮事件
#define SDRSCLIENT_ACTION_HIGHBTN				3				//触发满意按钮事件
#define SDRSCLIENT_ACTION_LOWBTN				4				//触发不满意按钮事件
#define SDRSCLIENT_ACTION_SLIDER_UP				5				//滑块向上
#define SDRSCLIENT_ACTION_SLIDER_DOWN			6				//滑块向下

//===========================================================================================================
/*!cls
彩虹精灵统一客户端组件类
!inc SDRSClient.h
*/
class ISDRSClient : public ISDBase
{
public:
	/**************************************************************************************************/
	//!group 初始化函数
	/*!func
	此函数用来创建彩虹精灵窗口
	!param [in] 游戏渲染窗口句柄
	!param [in] 窗口布局配置文件
	!return 错误代码　参见SDRSCLIENT_ERROR_
	*/
	// 创建彩虹精灵窗口
	virtual int SDAPI CreateRSWindow(HWND hMainWnd = NULL, const char * strConfigFile = NULL) = 0;
	
	/*!func
	设置游戏相关信息(不从配置文件中读取,而是由游戏提供,必须在创建窗口成功后调用)
	!param [in] 游戏种类(参见游戏的数字编码:例如传世 1 梦幻 2)
	!param [in] 玩家所在区数字编码
	!param [in] 帐号种类 "0" 数字帐号 "1"　盛大通行证
	!param [in] 帐号
	!return 目前不会失败
	*/
	virtual bool SDAPI SetGameInfo( int nGameType, int nAreaCode, char* szIDType, char* szUserID) = 0;
	

	/*!func
	// 设置窗口的模式 必须在创建主窗口之间调用　否则无效
	!param [in] 窗口种类　参见SDRSCLIENT_
	!return 成功为tue 失败 false
	*/
	virtual bool SDAPI SetMode(int iMode) = 0;


	/****************************************************************************************************/
	//!group 窗口控制函数
	/*!func
	// 重设彩虹精灵窗口状态，恢复到窗口创建时的状态
	!return 目前不会失败
	*/
	virtual bool SDAPI Reset() = 0;
	/*!func 
	// 显示或隐藏彩虹精灵窗口
	!param [in] 显示与否
	!return 目前不会失败
	*/
	virtual bool SDAPI Show(bool bShow = true) = 0;

	/*!func
	// 设置窗口的坐标(注意在POPUP模式坐标为相对于桌面坐标)
	!param [in]　x
	!param [in] y
	!return 目前不会失败
	*/
	virtual bool SDAPI SetPos(int iX, int iY) = 0;

	/*!func
	// 设置窗口的尺寸
	//不接受程序中改变窗口的大小,窗口的大小需要事先设计,从图片资源中读取但保留该接口
	!param [in] 窗口宽
	!param [in] 窗口高
	!return 目前必失败
	*/
	virtual bool SDAPI SetSize(int iWidth, int iHeight) = 0;

	/*!func
	// 获得窗口的位置
	!param [in] x
	!param [in] y
	*/
	virtual bool SDAPI GetPos(int & iX, int & iY) = 0;

	/*!func
	// 获得窗口的尺寸
	!param [in] 窗口宽
	!param [in] 窗口高
	!return 目前不会失败
	*/
	virtual bool SDAPI GetSize(int & iWidth, int & iHeight) = 0;

	/*!func
	// 获得窗口的模式
	!return 参见SDRSClIENT_
	*/

	virtual int  SDAPI GetMode() = 0;
	/*!func
	// 判断窗口是否在显示状态
	!return 显示　true 隐藏false
	*/
	virtual bool SDAPI IsShow() = 0;
	
	/*!func
	//设置是否允许窗口移动
	!param [in] 是事允许窗口移动
	!return 目前不会失败
	*/
	virtual bool SDAPI SetMoveable( bool bCanMove = true) = 0;

	/*!func
	//判断窗口是否可移动
	!return 可移动 true 不可　false
	*/
	virtual bool SDAPI GetMoveable( ) = 0;

	/*!func
	//用来提供给手柄用记模拟窗口中发生的一些事件
	!param [in] 事件　参见　SDRSCLINET_ACTION
	!return 成功　true 失败　false
	*/
	virtual bool SDAPI ExecuteAction( int iAction) = 0;
};


/*!func
此函数返回SDRSClient组件的实例。
!param [in] 用户要求的组件最低版本号，见SSGDPVersion。
!return 如果成功，返回此组件的示例；如果识别返回空指针。
!inc SDRSClient.h
*/
ISDRSClient * SDAPI SDRSClientGetModule(const SSGDPVersion *pstVersion);

/*!typedef
SDRSClientGetModule 函数的类型定义。
!inc SDRSClient.h
*/
typedef ISDRSClient * (SDAPI *PFN_SDRSClientGetModule)(const SSGDPVersion *pstVersion);

}
