// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-29 ¿ÀÈÄ 2:32:54 
//***************************************************************************


#pragma once


namespace {

#define DECLARE_WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool _CASE_MAP_FUNCTION_ (std::string strParam);

#define WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool SPTerrainAttributeLoader::_CASE_MAP_FUNCTION_ (std::string strParam) 


#define CHECK_BLOCK_INVALID( DEEP ) \
	if( m_iDeep != DEEP ) { \
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

struct CONVERT_ATTRIBUTE_INFO
{
	int iDiffX;
	int iDiffY;

	unsigned char ucType;

	void Clear()
	{
		iDiffX = 0;
		iDiffY = 0;
		ucType = 0;
	}

	CONVERT_ATTRIBUTE_INFO& operator = (const CONVERT_ATTRIBUTE_INFO& attrInfo)
	{
		iDiffX = attrInfo.iDiffX;
		iDiffY = attrInfo.iDiffY;
		ucType = attrInfo.ucType;

		return *this;
	}
};


class SPTerrainAttributeLoader
{
public:

	SPTerrainAttributeLoader();
	virtual ~SPTerrainAttributeLoader();		
	bool LoadDataText(const char* pszFile, 
		BYTE** ppLayer1, 
		BYTE** ppLayer2, 
		BYTE** ppLayer3,							//jinhee - 04.11.23
		int& iSize,	
		long lGroupWidth = 0,						//jinhee - [2005/1/17]
		long lGroupHeight = 0,						//iinhee - [2005/1/17]
		const char* strPath = NULL);				//jinhee - 04.11.22

	bool LoadBinary(const char* pszFile, 
		BYTE** ppLayer1, 
		BYTE** ppLayer2, 
		BYTE** ppLayer3,
		int& iSize,	
		long lGroupWidth = 0,
		long lGroupHeight = 0,
		const char* strPath = NULL);

	int GetCXTerrain()	{ return m_iCXTerrain; }	
	int GetCYTerrain()	{ return m_iCYTerrain; }	


protected:

	DECLARE_WORDCASE_MAP( OnComment				);	
	DECLARE_WORDCASE_MAP( OnBlockBegin			);
	DECLARE_WORDCASE_MAP( OnBlockEnd			);

	DECLARE_WORDCASE_MAP( OnHeader				);
	DECLARE_WORDCASE_MAP( OnCommonPallet		);
	DECLARE_WORDCASE_MAP( OnLocalPallet			);
	DECLARE_WORDCASE_MAP( OnTerrainLayerCX		);
	DECLARE_WORDCASE_MAP( OnTerrainLayerCY		);
	DECLARE_WORDCASE_MAP( OnTerrainAttrLayer1	);
	DECLARE_WORDCASE_MAP( OnTerrainAttrLayer2	);
	DECLARE_WORDCASE_MAP( OnTerrainAttrLayer3	);	//jinhee - 04.11.23
	DECLARE_WORDCASE_MAP( OnAttr				);


protected:

	int				m_iCXTerrain;
	int				m_iCYTerrain;
	int				m_iDeep;

	BYTE**			m_ppCurLayer;

	BYTE*			m_pAttrLayer1;
	BYTE*			m_pAttrLayer2;
	BYTE*			m_pAttrLayer3;					//jinhee - 04.11.23

	long			m_lGroupWidth;					//jinhee - [2005/1/17] //ÁÂÇ¥ º¸Á¤
	long			m_lGroupHeight;					//jinhee - [2005/1/17] //ÁÂÇ¥ º¸Á¤

	int				m_iAttrSize;

};






