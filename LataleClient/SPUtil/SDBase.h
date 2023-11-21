/*!
!module SDBase组件基类
!moduletype common
此组件提供所有其它组件的基类ISDBase。
本组件只有静态链接模式，且不支持直接引用。
!inc SDBase.h
*/ 
#pragma once


namespace SGDP {

// 函数调用方式
#define SDAPI __stdcall


/*!
!macro SAFE_
一系列安全摧毁指针或对象的宏定义。
!inc SDBase.h
*/
#define SAFE_RELEASE(p)				{ if(p) { (p)->Release(); (p)=NULL; } }				//! 安全的释放对象
#define SAFE_DELETE(p)				{ if(p) { delete (p); (p)=NULL; } }					//! 安全的删除new出来的变量
#define SAFE_DELETE_ARRAY(p)		{ if(p) { delete [] (p); (p)=NULL; } }				//! 安全的删除new[]出来的数组
//#define SAFE_PURGE(p)				{ while ((p)->Release()) {} (p)=NULL; }				// ! 安全的彻底释放对象


/*!struct
SGDP组件版本号结构。
此结构标识一个SGDP组件的版本号。
!inc SDBase.h
*/
struct SSGDPVersion
{
	unsigned short	wMajorVersion;		//! 主版本号
	unsigned short	wMinorVersion;		//! 副版本号
	unsigned short	wMinimumVersion;	//! 最小版本号
	unsigned short	wBuildNumber;		//! Build序号
};


/*!cls
所有组件类的基类。
SGDP平台下所有组件输出的接口类都继承自ISDBase类。
!inc SDBase.h
*/
class ISDBase
{
public:
	ISDBase();


	//!group 引用计数
	/*!func
	增加本实例的引用计数。
	*/
	virtual void SDAPI AddRef();

	/*!func
	查询本实例当前的引用计数。
	!return 本实例当前的引用计数。
	*/
	virtual int  SDAPI QueryRef();

	/*!func
	释放类实例。
	每个组件类的实现都必须重载此函数，以完成组件的释放。
	为了实现单实例，此函数将减少引用计数而并不直接释放释放类实例。
	只有引用计数为0的时候，此类的实例才会真正的被释放掉。
	*/
	virtual void SDAPI Release();


	//!group 组件信息
	/*!func
	获取组件版本信息。
	每个组件都将重载此函数，以提供本组件的版本号。
	!return 组件版本号。见SSGDPVersion。
	*/
	virtual SSGDPVersion SDAPI GetVersion() = 0;

	/*!func
	获取组件名。
	每个组件都将重载此函数，以提供本组件的名称。
	!return 组件名。
	*/
	virtual const char * SDAPI GetModuleName() = 0;


protected:
	int m_iRef;
};

}