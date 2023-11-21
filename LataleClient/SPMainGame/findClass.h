// CFindClass.cpp : Defines the entry point for the console application.
// 作者:  xialin 
// 日期:  2008/07/17

#pragma once
#include "stdafx.h"
#include "string.h"
#include <utility>



template<class T, class I>
class  CFindClass
{
public:
	CFindClass(I data, int nOffset)             //所有支持==运算符的类型
	{
		m_bString = false;
		m_findData = data;
		m_lpString = NULL;
		m_nOffset = nOffset;
	}
	CFindClass(const char * data, int nOffset)         //字符串类型(不支持==运算符)
	{
		m_bString = true;
		int nLenght = static_cast<int>(strlen(data) + 1);
		m_lpString = new char[nLenght];
		strcpy_s(m_lpString, nLenght, data);
		m_nOffset = nOffset;
	}
	CFindClass(const CFindClass<T,I> & src)        //拷贝构造
	{
		m_findData = src.m_findData;
		m_nOffset = src.m_nOffset;
		m_bString = src.m_bString;
		if (src.m_lpString)
		{
			int nLenght = static_cast<int>(strlen(src.m_lpString)+1);
			m_lpString = new char[nLenght];
			strcpy(m_lpString, src.m_lpString);
		}
		else
		{
			m_lpString = NULL;
		}
	}
	~CFindClass()
	{
		if (m_lpString)
		{
			delete m_lpString;
			m_lpString = NULL;
		}
	}

	void operator = (const CFindClass<T,I> & src)
	{
		CFindClass(src);
	}

	bool operator ()(const T & item)		   //查找
	{
		char * pData = (char *)&item;
		pData += m_nOffset;
		if (m_bString)
		{
			return !strcmp(m_lpString, pData);
		}
		return *((I*)pData) == m_findData;
	}

	bool operator ()(const T * item)          //指针类型查找
	{
		char * pData = (char *)item;
		pData += m_nOffset;

		if (m_bString)
		{
			return !strcmp(m_lpString, pData);
		}
		return *((I*)pData) == m_findData;
	}

protected:
	I		m_findData;
	int		m_nOffset;
	char *	m_lpString;
	bool	m_bString;
};

//关联容器类查找
template<class T, class I>
class CPairFind : public CFindClass<T,I>
{
public:
	typedef CFindClass<T,I>  _Mybase;

	CPairFind(I data, int nOffset) : _Mybase(data, nOffset) {}
	CPairFind(const char * data, int nOffset) : _Mybase(data, nOffset) {}
	CPairFind(const CPairFind & src) : _Mybase((CFindClass)src) {}
	~CPairFind() { _Mybase::~CFindClass(); }

	void operator = (const CPairFind & src) { CPairFind(src); }
	bool operator ()(const std::pair<char, T> & item)
	{
		return _Mybase::operator ()(item.second);
	}
};

//关联容器指针类查找
template<class T, class I>
class CPairPtrFind : public CFindClass<T,I>
{
public:
	typedef CFindClass<T,I>  _Mybase;

	CPairPtrFind(I data, int nOffset) : _Mybase(data, nOffset) {}
	CPairPtrFind(const char * data, int nOffset) : _Mybase(data, nOffset) {}
	CPairPtrFind(const CPairPtrFind & src) : _Mybase((CFindClass)src) {}
	~CPairPtrFind() { _Mybase::~CFindClass(); }

	void operator = (const CPairPtrFind & src) { CPairPtrFind(src); }
	bool operator ()(const std::pair<char, T*> & item)
	{
		return _Mybase::operator ()(item.second);
	}
};

#define FindNode(i,s,m,c)			CFindClass<s, c>(i, offsetof(s,m))
#define FindNode_s(i,s,m)			CFindClass<s, char*>(i, offsetof(s,m))
#define FindNode_pair(i,s,m,c)			CPairFind<s, c>(i, offsetof(s,m))
#define FindNode_pair_s(i,s,m)			CPairFind<s, char*>(i, offsetof(s,m))
#define FindNode_pairPtr(i,s,m,c)			CPairPtrFind<s, c>(i, offsetof(s,m))
#define FindNode_pairPtr_s(i,s,m)			CPairPtrFind<s, char*>(i, offsetof(s,m))



///////////////////////////////////////////查找示例//////////////////////////////////////////////
//struct  info
//{
//	int   nId;
//	char  szName[32];
//	int   nLevel;
//};
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	///////////////////////////////////支持顺序容器与关联容器///////////////////////////////////
//	info    in;
//    std::vector<info>  vecInfo;
//	std::map<int, info> mapInfo;
//	memset(&in, 0x00, sizeof(in));
//
//	in.nId = 1;
//	in.nLevel = 1;
//	strcpy_s(in.szName, "lin");
//	vecInfo.push_back(in);
//	mapInfo[1] = in;
//
//	in.nId = 2;
//	in.nLevel = 2;
//	strcpy_s(in.szName, "456");
//	vecInfo.push_back(in);
//	mapInfo[2] = in;
//
//	//vector
//	std::vector<info>::iterator  it = find_if(vecInfo.begin(), vecInfo.end(), FindNode(1, info, nId, int));
//	if (it != vecInfo.end())
//	{
//		int  nId = it->nId;
//	}
//
//	std::vector<info>::iterator  itName = find_if(vecInfo.begin(), vecInfo.end(), FindNode_s("456", info, szName));
//	if (itName != vecInfo.end())
//	{
//		int  nId = itName->nId;
//	}
//
//	//map
//	std::map<int, info>::iterator  itMap = find_if(mapInfo.begin(), mapInfo.end(), FindNode_pair(1, info, nId, int));
//	if (itMap != mapInfo.end())
//	{
//		int  nId = itMap->second.nId;
//	}
//
//	std::map<int, info>::iterator  itMapName = find_if(mapInfo.begin(), mapInfo.end(), FindNode_pair_s("456", info, szName));
//	if (itMapName != mapInfo.end())
//	{
//		int  nId = itMapName->second.nId;
//	}
//
//	/////////////////////////////////////支持指针///////////////////////////////////
//    std::vector<info*>  vecInfoPtr;
//	std::map<int, info*> mapInfoPtr;
//
//	info  * pIn = new info;
//	memset(pIn, 0x00, sizeof(info));
//	pIn->nId = 1;
//	pIn->nLevel = 1;
//	strcpy_s(pIn->szName, "xia");
//	vecInfoPtr.push_back(pIn);
//	mapInfoPtr[1] = pIn;
//
//	pIn = new info;
//	memset(pIn, 0x00, sizeof(info));
//	pIn->nId = 2;
//	pIn->nLevel = 2;
//	strcpy_s(pIn->szName, "123");
//	vecInfoPtr.push_back(pIn);
//	mapInfoPtr[2] = pIn;
//
//	//vector
//	std::vector<info*>::iterator  itPtr = find_if(vecInfoPtr.begin(), vecInfoPtr.end(), FindNode(2, info, nId, int));
//	if (itPtr != vecInfoPtr.end())
//	{
//		int  nId = (*itPtr)->nId;
//	}
//
//	std::vector<info*>::iterator  itPtrName = find_if(vecInfoPtr.begin(), vecInfoPtr.end(), FindNode_s("xia", info, szName));
//	if (itPtrName != vecInfoPtr.end())
//	{
//		int  nId = (*itPtrName)->nId;
//	}
//
//	//map
//	std::map<int, info*>::iterator  itMapPtr = find_if(mapInfoPtr.begin(), mapInfoPtr.end(), FindNode_pairPtr(2, info, nId, int));
//	if (itMapPtr != mapInfoPtr.end())
//	{
//		int  nId = itMapPtr->second->nId;
//	}
//
//	std::map<int, info*>::iterator  itMapPtrName = find_if(mapInfoPtr.begin(), mapInfoPtr.end(), FindNode_pairPtr_s("xia", info, szName));
//	if (itMapPtrName != mapInfoPtr.end())
//	{
//		int  nId = itMapPtrName->second->nId;
//	}
//
//	system("Pause");
//	return 0;
//}

