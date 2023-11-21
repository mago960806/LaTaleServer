// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-29 ¿ÀÈÄ 2:32:54 
//***************************************************************************


#pragma once


//!!!/////////////////////////////////////////////////////////////////////////////////////////
//
// caution : certainly marking version changed date, to version control 
//
// Latest Version assigned 2004/03/31 by metalgeni.
//
//const float CURRENT_BG_FORMAT_VERSION	= 0.1f;
//
//////////////////////////////////////////////////////////////////////////////////////////////


namespace {

#define DECLARE_WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool _CASE_MAP_FUNCTION_ (string strParam);

#define WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool SPTerrainFormLoader::_CASE_MAP_FUNCTION_ (string strParam) 


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

struct CONVERT_FORM_INFO
{
	//int iInstance;
	INT64 iInstance;
	float fX;
	float fY;
	int iArg0;
	int iArg1;
	int iArg2;
	int iArg3;
	float fScaleX;
	float fScaleY;

	void Clear()
	{
		iInstance = 0;
		fX = 0.0f;
		fY = 0.0f;
		iArg0 = 0;
		iArg1 = 0;
		iArg2 = 0;
		iArg3 = 0;
		fScaleX = 1.0f;
		fScaleY = 1.0f;
	}

	CONVERT_FORM_INFO& operator = (const CONVERT_FORM_INFO& formInfo)
	{
		iInstance = formInfo.iInstance;
		fX = formInfo.fX;
		fY = formInfo.fY;
		iArg0 = formInfo.iArg0;
		iArg1 = formInfo.iArg1;
		iArg2 = formInfo.iArg2;
		iArg3 = formInfo.iArg3;
		fScaleX = formInfo.fScaleX;
		fScaleY = formInfo.fScaleY;

		return *this;
	}
};

class SPTerrainFormLoader
{
public:

	SPTerrainFormLoader();
	virtual ~SPTerrainFormLoader();		
	bool LoadDataText(const char* pszFormFile, 
		std::vector<SPTerrainUnit*>* pvpTerrainUnit1,
		std::vector<SPTerrainUnit*>* pvpTerrainUnit2,
		std::vector<SPTerrainUnit*>* pvpTerrainUnit3,
		std::vector<SPTerrainModel*>* pvpTerrainModel,
		float fStartPosX = 0.0f,			//[2005/1/20]
		float fStartPosY = 0.0f,
		const char* strPath = NULL);		//jinhee -04.10.16
	bool LoadBinary(const char* pszFormFile, 
		std::vector<SPTerrainUnit*>* pvpTerrainUnit1,
		std::vector<SPTerrainUnit*>* pvpTerrainUnit2,
		std::vector<SPTerrainUnit*>* pvpTerrainUnit3,
		std::vector<SPTerrainModel*>* pvpTerrainModel,
		float fStartPosX = 0.0f,
		float fStartPosY = 0.0f,
		const char* strPath = NULL);


	float GetCXTerrain()	{ return m_fCXTerrain; }	
	float GetCYTerrain()	{ return m_fCYTerrain; }	

	void  SetCommonPallet(std::vector<SPTerrainModel*>* pvpTerrainCommonModel);	


protected:

	DECLARE_WORDCASE_MAP( OnComment				);	
	DECLARE_WORDCASE_MAP( OnBlockBegin			);
	DECLARE_WORDCASE_MAP( OnBlockEnd			);

	DECLARE_WORDCASE_MAP( OnTerrainformHeader	);
	DECLARE_WORDCASE_MAP( OnCommonPallet		);
	DECLARE_WORDCASE_MAP( OnLocalPallet			);
	DECLARE_WORDCASE_MAP( OnTerrainLayerCX		);
	DECLARE_WORDCASE_MAP( OnTerrainLayerCY		);
	DECLARE_WORDCASE_MAP( OnTerrainLayer1		);
	DECLARE_WORDCASE_MAP( OnTerrainLayer2		);
	DECLARE_WORDCASE_MAP( OnTerrainLayer3		);
	DECLARE_WORDCASE_MAP( OnTile				);

	//SPTerrainModel* IsExistModel(int iInstance, std::vector<SPTerrainModel*>* ppvLocalModel);
	SPTerrainModel* IsExistModel(INT64 iInstance, std::vector<SPTerrainModel*>* ppvLocalModel);

protected:

	float			m_fCXTerrain;
	float			m_fCYTerrain;
	int				m_iDeep;
	int				m_iCurLayer;	// For Error Log

	float			m_fStartPosX;	//[2005/1/20]
	float			m_fStartPosY;

	std::vector<SPTerrainModel*>	m_vpTerrainLocalModel;
	std::vector<SPTerrainModel*>*	m_pvpTerrainCommonModel;

	std::vector<SPTerrainUnit*>*	m_pvpCurLayer;
	
	std::vector<SPTerrainUnit*>		m_vpTerrainLayer1;
	std::vector<SPTerrainUnit*>		m_vpTerrainLayer2;
	std::vector<SPTerrainUnit*>		m_vpTerrainLayer3;

};



