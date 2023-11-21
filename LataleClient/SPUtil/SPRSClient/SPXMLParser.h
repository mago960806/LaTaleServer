
// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPXMLParser.h
//	created:	2007/10/1   Time:12:50
//	purpose:	
//*********************************************************************/

#pragma once 

#include "msxml2.h"
#include "comdef.h"
#pragma comment(lib, "msxml2")

typedef IXMLDOMDocument	XDoc;
typedef IXMLDOMNode		XNode;
typedef IXMLDOMNodeList	XNodeList;

class SPXMLParser
{
protected:
	XDoc*	m_pXDoc;

protected: 
	BOOL Setup();									//XML DOM Setup.
	VOID Release();									//XML DOM Release

public:
	HRESULT Open(LPCTSTR filename);					//파일 열기.
	HRESULT OpenXML(char* pData);
	//VOID Close();									//파일 닫기.

	XNodeList*	FindElement(LPCTSTR ElementName);
	XNodeList*	FindElement(XNode* pNode, LPCTSTR ElementName);

	int			GetAttrValue2Text(XNode* pNode, const char* AttrName, char* pAttrValue);
	int			GetElementValue(XNode* pNode, const char* ElementName, char* ElementValue, long index=0);	//★


public:

	SPXMLParser();
	~SPXMLParser();
};

