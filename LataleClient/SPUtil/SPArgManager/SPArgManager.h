// Copyright (C) kkandori
//***************************************************************************
// System Name : Argument Manager
// Comment     : 
// Creation    : kkandori 2004-10-5 ¿ÀÈÄ 4:05:24
//***************************************************************************

#pragma once

#include <list>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// TODO : Declare other manager , object or something else

struct tagARGSTRUCT
{
	string				m_strCommand;
	std::list<string>	m_Arguments;

	tagARGSTRUCT& operator=(const tagARGSTRUCT &src)
	{
		m_strCommand = src.m_strCommand;
		m_Arguments = src.m_Arguments;

		return *this;
	};

	~tagARGSTRUCT()
	{
		m_Arguments.clear();
	}
};

/// 
namespace {

#define DECLARE_WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool _CASE_MAP_FUNCTION_ (string strParam);

#define WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool SPArgManager::_CASE_MAP_FUNCTION_ (string strParam) 

#define EXCUTE_WORDCASE_MAP(_CASE_, _CASE_MAP_FUNCTION_) \
	if ( true == isCase( _strupr( _CASE_ ) ,  str.c_str() ) )  { \
		if(_CASE_MAP_FUNCTION_(str)) \
		{ \
			++Iter; \
			continue; \
		} \
		else \
			return false; \
	}
}
#define SET_ARGUMENT_HEADER()		\
	bool	bIsOnlyCommand = true;\
	std::list<tagARGSTRUCT>::iterator	IterCommand;\
	IterCommand = std::find_if( m_lArgList.begin(), m_lArgList.end(), std::bind2nd(IsEqualCommand, strParam));\
	if(IterCommand == m_lArgList.end())				return false;	\
	if((*IterCommand).m_Arguments.size()==0)	bIsOnlyCommand = true;\
	else										bIsOnlyCommand = false;\
	tagARGSTRUCT CommandLine = (*IterCommand);		\

class SPArgManager
{	
	//std::list<std::string>	m_lstrList;
	std::list<tagARGSTRUCT>		m_lArgList;
	void		ParsingArgument(string strArg);
protected:

	// TODO : Declare method
	DECLARE_WORDCASE_MAP( OnSetFullScreen );
	DECLARE_WORDCASE_MAP( OnSetWindowScreen );

	DECLARE_WORDCASE_MAP( OnSetLoginID );
	DECLARE_WORDCASE_MAP( OnSetChar );
	DECLARE_WORDCASE_MAP( OnSetServer );
	DECLARE_WORDCASE_MAP( OnSetLoginServer );
	DECLARE_WORDCASE_MAP( OnSetLobbyServer );
	DECLARE_WORDCASE_MAP( OnSetWorldID );

	DECLARE_WORDCASE_MAP( OnSetMoveDisable  );
	DECLARE_WORDCASE_MAP( OnGameMode );
	DECLARE_WORDCASE_MAP( OnNoSound );
	DECLARE_WORDCASE_MAP( OnRunApproval );
	DECLARE_WORDCASE_MAP( OnNoPack );
	DECLARE_WORDCASE_MAP( OnPack );
	DECLARE_WORDCASE_MAP( OnFileLog );
	DECLARE_WORDCASE_MAP( OnSetUserID );

	DECLARE_WORDCASE_MAP( OnSetLocale );
	DECLARE_WORDCASE_MAP( OnWebArg );
	DECLARE_WORDCASE_MAP( OnGameGuardPatch );
	DECLARE_WORDCASE_MAP( OnResPath );

	DECLARE_WORDCASE_MAP( OnFSDAdmin );

public:
	SPArgManager(char * arg);
	~SPArgManager(void);

	bool	ExcuteArgument();
};

