#ifndef __SPMOB_LAYER_TABLE_LOALDER__
#define __SPMOB_LAYER_TABLE_LOALDER__

#pragma once

struct SPMOBLayerInfo;

namespace mobLayer{

#define DECLARE_WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool _CASE_MAP_FUNCTION_ (std::string strParam);

#define WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool SPMOBLayerTableLoader::_CASE_MAP_FUNCTION_ (std::string strParam) 

#define CHECK_BLOCK_INVALID_FRAME() \
	if( m_iDeep != 2 ) { \
	return false; \
	}

#define CHECK_BLOCK_INVALID_SIZE() \
	if( m_iDeep != 1 ) { \
	return false; \
	}

#define CHECK_BLOCK_INVALID_LAYERNO() \
	if( m_iDeep != 1 ) { \
	return false; \
	}

#define CHECK_BLOCK_INVALID_LAYER() \
	if( m_iDeep != 0 ) { \
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

class SPMOBLayerTableLoader
{
public:
	SPMOBLayerTableLoader();
	~SPMOBLayerTableLoader();

	bool LoadAniPallet(std::vector<SPMOBLayerInfo*>& vpMOBLayer, string strResource);
	bool LoadBinaryAni(std::vector<SPMOBLayerInfo*>& vpMOBLayer, string strResource);

protected:

	// {{ WORD CASE MAP
	DECLARE_WORDCASE_MAP(OnComment);	
	DECLARE_WORDCASE_MAP(OnBlockBegin);
	DECLARE_WORDCASE_MAP(OnBlockEnd);

	DECLARE_WORDCASE_MAP(OnLayer);

	DECLARE_WORDCASE_MAP(OnSize);
	DECLARE_WORDCASE_MAP(OnLayerNo);
	
	DECLARE_WORDCASE_MAP(OnName);
	DECLARE_WORDCASE_MAP(OnImage);
	DECLARE_WORDCASE_MAP(OnFrameWidth);
	DECLARE_WORDCASE_MAP(OnFrameHeight);
	DECLARE_WORDCASE_MAP(OnFrameCountX);
	DECLARE_WORDCASE_MAP(OnFrameCountY);
	DECLARE_WORDCASE_MAP(OnLightMap);
	DECLARE_WORDCASE_MAP(OnFrame);

	SPMOBLayerInfo* GetMOBLayerInfo(int iNo);

protected:
	int	m_iDeep;
	int m_iLayerNum;
	int m_iCurLayerNo;

	int m_iBlockType;

	ANIMATION_INDEX m_eAniIndex;
	SPMOBLayerInfo* m_pMOBLayerInfo;
	SPMOBLayerAnimation m_MOBLayerAnimation;

	std::vector<SPMOBLayerInfo*> m_vpMOBLayerInfo;
};

#endif