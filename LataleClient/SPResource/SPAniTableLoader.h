// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-29 ¿ÀÈÄ 2:32:54 
//***************************************************************************


#pragma once

#include <string>

using namespace std;


struct SPGOBModel;

struct SPMOBAniSeqPart;
struct SPMOBAnimation;

//!!!/////////////////////////////////////////////////////////////////////////////////////////
//
// caution : certainly marking version changed date, to version control 
//
// Latest Version assigned 2004/10/12 by metalgeni.
//
const float CURRENT_ANI_TABLE_VERSION	= 2.0f;
//
//////////////////////////////////////////////////////////////////////////////////////////////


namespace {

#define DECLARE_WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool _CASE_MAP_FUNCTION_ (std::string strParam);

#define WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool SPAniTableLoader::_CASE_MAP_FUNCTION_ (std::string strParam) 

#define CHECK_BLOCK_INVALID_SEQ_PART() \
	if( m_iDeep != 2 ) { \
		return false; \
	}

#define CHECK_BLOCK_INVALID_SEQ() \
	if( m_iDeep != 1 ) { \
		return false; \
	}

#define CHECK_BLOCK_INVALID_ANI() \
	if( m_iDeep != 0 ) { \
		return false; \
	}

#define EXCUTE_WORDCASE_MAP(_CASE_, _CASE_MAP_FUNCTION_) \
	if ( true == isCase( _CASE_ ,  str.c_str() ) )  { \
		if(_CASE_MAP_FUNCTION_(str)) \
			continue; \
		else \
			return false; \
	}
}




class SPAniTableLoader
{
public:

	SPAniTableLoader();
	virtual ~SPAniTableLoader();
	
	bool LoadAniPallet(SPGOBAnimation** ppAnimation, string strResource="");	
	bool LoadBinaryAni(SPGOBAnimation** ppAnimation, string strResource);

	bool AssignPartLayerValue( const std::string strParam, SPGOBAniSeqPart& AniSeqPart);
	bool AdjustPartLayer();

protected:	

	// {{ WORD CASE MAP
	DECLARE_WORDCASE_MAP( OnComment					);	
	DECLARE_WORDCASE_MAP( OnBlockBegin				);
	DECLARE_WORDCASE_MAP( OnBlockEnd				);
	DECLARE_WORDCASE_MAP( OnTextHeader				);	
	DECLARE_WORDCASE_MAP( OnAniIndex);

	DECLARE_WORDCASE_MAP( OnSeq						);
	DECLARE_WORDCASE_MAP( OnAccumulateTime			);

	DECLARE_WORDCASE_MAP( OnSeqPart	);
	// }} 	

protected:

	int								m_iDeep;

	SPGOBAniSeqPart					m_AniSeqPart;
	SPGOBAniSeq						m_AniSeq;
	SPGOBAnimation*					m_pAnimation;
	std::vector<SPGOBAnimation*>	m_vpAnimation;	 

	std::map<string, PART_LAYER>	m_mStrToLayer;
};








