#ifndef __SPMOB_ANITABLE_LOALDER__
#define __SPMOB_ANITABLE_LOALDER__

#pragma once

#include <string>

using namespace std;

struct SPGOBModel;

namespace {

#define DECLARE_WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool _CASE_MAP_FUNCTION_ (std::string strParam);

#define WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool SPMOBAniTableLoader::_CASE_MAP_FUNCTION_ (std::string strParam) 

#define CHECK_BLOCK_INVALID_FRAME() \
	if( m_iDeep != 2 ) { \
		return false; \
	}

#define CHECK_BLOCK_INVALID_SIZE() \
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

class SPMOBAniTableLoader
{
public:
	SPMOBAniTableLoader();
	~SPMOBAniTableLoader();

	bool LoadAniPallet(std::vector<SPMOBAnimation*>& vpMobAnimation, string strResource);
	bool LoadBinaryAni(std::vector<SPMOBAnimation*>& vpMobAnimation,
		string strFileName, string strImageName);
	
	ANIMATION_INDEX	GetCurAniIndex() {return m_eAniIndex;} 

protected:
	
	// {{ WORD CASE MAP
	DECLARE_WORDCASE_MAP(OnComment);	
	DECLARE_WORDCASE_MAP(OnBlockBegin);
	DECLARE_WORDCASE_MAP(OnBlockEnd);
	DECLARE_WORDCASE_MAP(OnTextHeader);	

	DECLARE_WORDCASE_MAP(OnSize);
	DECLARE_WORDCASE_MAP(OnFrame);

protected:
	int	m_iDeep;
	
	ANIMATION_INDEX m_eAniIndex;
	MONSTER_INDEX m_eMonsterIndex;
	
	SPMOBAniFrameInfo m_AniFrameInfo;
	SPMOBAnimation* m_pAnimation;
	std::vector<SPMOBAnimation*> m_vpAnimation;
};

#endif








