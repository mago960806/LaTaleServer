
// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPXMLParser.cpp
//	created:	2007/10/1   Time:12:51
//	purpose:	
//*********************************************************************/

#include "SPXMLParser.h"



//---------------------------------------------------------------------
SPXMLParser::SPXMLParser()
:m_pXDoc(NULL)
{
	Setup();
}




//---------------------------------------------------------------------
SPXMLParser::~SPXMLParser()
{
	Release();
}



//---------------------------------------------------------------------
BOOL SPXMLParser::Setup()
{
	m_pXDoc = NULL;
	CoInitialize(NULL);
	CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_ALL, IID_IXMLDOMDocument, (void**)&m_pXDoc );

	return TRUE;
}



//---------------------------------------------------------------------
VOID SPXMLParser::Release()
{
	if(m_pXDoc == NULL) return;

	m_pXDoc->Release();
	CoUninitialize();
}




//---------------------------------------------------------------------
HRESULT SPXMLParser::Open(LPCTSTR filename)
{
	HRESULT hr;

	VARIANT_BOOL bRes;
	VARIANT var;

	var.vt = VT_BSTR;
	var.bstrVal = (_bstr_t)filename;

	hr = m_pXDoc->load(var, &bRes);

	return hr;
}


//---------------------------------------------------------------------
// Open from Stream
HRESULT SPXMLParser::OpenXML(char* pData)
{
	HRESULT hr;

	VARIANT_BOOL bRes;
	//VARIANT var;

	_bstr_t bstrText = pData;

	hr = m_pXDoc->loadXML(bstrText, &bRes) ;

	return hr;
}


//---------------------------------------------------------------------
/**
	루트에서부터 상대path 로 요소(Element) 를 찾는다.
*/
XNodeList* SPXMLParser::FindElement(LPCTSTR ElementName)
{
	assert(m_pXDoc != NULL);
	IXMLDOMNodeList* pNodeList = NULL;
	m_pXDoc->selectNodes((_bstr_t)ElementName, &pNodeList); 
	return pNodeList;
}



//---------------------------------------------------------------------
/**
	주어진 노드의 하위 요소(Element) 를 찾는다.
	@param XNode* pNode,		: 찾을 요소의 부모노드.
	@param LPCTSTR ElementName	: 찾을 요소명
*/

XNodeList* SPXMLParser::FindElement(XNode* pNode, LPCTSTR ElementName )
{	
	assert(pNode != NULL);	 
	IXMLDOMNodeList* pChildNodeList = NULL;
	pNode->selectNodes((_bstr_t)ElementName, &pChildNodeList); 
	return pChildNodeList;
}


//---------------------------------------------------------------------
/**
	주어진 Element 에서 속성(Attribute)값을 읽어 온다.
	@param XNode* pNode			: 찾을 속성을 가지고 있는 노드, 즉 요소 : 입력
	@param const char* AttrName : 찾을 속성명:  입력
	@param char* AttrValue		: 속성값 : 출력(문자열)
*/
int SPXMLParser::GetAttrValue2Text(XNode* pNode, const char* AttrName, char* AttrValue)
{
	wchar_t	wAttrName[256];
	IXMLDOMNode*			pAttrNode = NULL;	//속성의 '노드'. 한 요소당 속성은 여러개가 있을 수 있다.
	IXMLDOMNamedNodeMap*	pMap = NULL;		//속성의 '노드'들을 '찾고','읽고'..하기 위한 Mapping 인터페이스.
	VARIANT	var;


	::mbstowcs(wAttrName, AttrName, sizeof(wAttrName));	//입력된 속성의 이름(MBS)을 유니코드(WCS)로 전환.


	pNode->get_attributes(&pMap);						//주어진 노드에 구성되어진 속성 배치(속성 멥 Attribute Map) 를 검색한다.
	pMap->getNamedItem(wAttrName,&pAttrNode);			//속성 멥에서 원하는 속성을 찾는다.
	pAttrNode->get_nodeValue(&var);						//속성값 얻기 (VARIANT 타입..적당하게 변환하여 사용 해야함.)

	//읽어 들인 데이터를 텍스트 형태로 전환, 리턴.
	::strcpy(AttrValue, (const char*)_bstr_t(var.bstrVal, FALSE));


	return TRUE;
}



//---------------------------------------------------------------------
// pNode 이하의 요소의 값을 읽어 온다.
//
// <Scene>  -- Scene 이하의 요소의 값을 읽어 온다. 속성은 제외.
//     <FileName>..
//     <FrameStart>..
//     <FrameEnd>..
//     ...
// </Scene>
//
int SPXMLParser::GetElementValue(XNode* pNode, const char* ElementName, char* ElementValue, long index /*= 0*/)
{
	BSTR bstr;
	XNodeList* pChildList = FindElement(pNode, ElementName);
	XNode* pChildNode = NULL;
	pChildList->get_item(index, &pChildNode);					//아이템 요소가 하나만있을 경우다..
	pChildNode->get_text(&bstr); 
	::strcpy(ElementValue,(const char*)_bstr_t(bstr, FALSE));		//@ fix.

	//m_SceneInfo.fFirstFrame = ::atoi(msg);

	return TRUE;

}
