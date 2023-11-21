// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-9    13:17 
//***************************************************************************

#pragma once

#include <string>

struct SPFXGroup;

namespace {

#define DECLARE_WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool _CASE_MAP_FUNCTION_ (std::string strParam);

#define WORDCASE_MAP( _CLASS_ , _CASE_MAP_FUNCTION_ ) \
	bool _CLASS_::_CASE_MAP_FUNCTION_ (std::string strParam) 

#define EXCUTE_WORDCASE_MAP(_CASE_, _CASE_MAP_FUNCTION_) \
	if ( true == isCase( _CASE_ ,  str.c_str() ) )  { \
	if(_CASE_MAP_FUNCTION_(str)) \
	continue; \
		else \
		return false; \
	}
}

class SPFXGroupLoader
{
private:

	// {{ WORD CASE MAP
	DECLARE_WORDCASE_MAP( OnComment					);	//	코멘트 처리
	DECLARE_WORDCASE_MAP( OnGroupId					);	//	FX그룹 아이디 읽기
	DECLARE_WORDCASE_MAP( OnModelId					);	//	FX모델 아이디 읽기
	// }}

	SPFXGroup*							m_pSPFXGroup;					//	매니저에 추가될 그룹데이터

	bool	LoadFileASCII( std::string strFileName = "" );	//	ASCII 방식으로.
	bool	LoadFileBinary( std::string strFileName = "" );	//	BINARY 방식으로.

public:
	SPFXGroupLoader(void);
	virtual ~SPFXGroupLoader(void);

	//	해당 파일 데이터를 읽어들어 FX그룹에다 추가한 후 돌려준다.
	SPFXGroup*	LoadFile( std::string strFileName = "" );

};
