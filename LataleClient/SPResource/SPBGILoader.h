// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-29 오후 2:32:54 
//***************************************************************************


#pragma once

#include <string>

using namespace std;

struct SPRotateScrollImage;
extern BG_ID;


//!!!/////////////////////////////////////////////////////////////////////////////////////////
//
// caution : certainly marking version changed date, to version control 
//
// Latest Version assigned 2004/03/31 by metalgeni.
//
const float CURRENT_BG_FORMAT_VERSION	= 0.1f;
//
//////////////////////////////////////////////////////////////////////////////////////////////


namespace {

enum STATE_BLOCK { 
	STATE_BLOCK_NULL = 0,  
	STATE_BLOCK_BEGIN,  
	STATE_BLOCK_END,
};

#define EXCUTE_WORDCASE_MAP(_CASE_, _CASE_MAP_FUNCTION_) \
	if ( true == isCase( _CASE_ ,  str.c_str() ) )  { \
		if(_CASE_MAP_FUNCTION_(pScrollImage, str)) \
			continue; \
		else \
			return false; \
	}

#define DECLARE_WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool _CASE_MAP_FUNCTION_ (SPRotateScrollImage*& pScrollImage, string strParam);

#define WORDCASE_MAP( _CASE_MAP_FUNCTION_ ) \
	bool SPBGILoader::_CASE_MAP_FUNCTION_ (SPRotateScrollImage*& pScrollImage, string strParam) 

#define CHECK_BLOCK_INVALID_OUT() \
	if( m_StateBlock != STATE_BLOCK_BEGIN ) { \
		return false; \
	}

#define CHECK_BLOCK_INVALID_IN() \
	if( m_StateBlock != STATE_BLOCK_NULL ) { \
		return false; \
	}
}

struct CONVERT_BG_INFO
{
	float fX;
	float fY;
	float fSX;
	float fSY;
	float fDX;
	float fDY;
	float fFlowDX;
	float fFlowDY;
	float fRotateX;
	float fRotateY;
	float fScaleX;
	float fScaleY;
	int iLightMap;
	BG_ID eBGLayer;

	char szImageName[64];

	void Clear()
	{
		fX = 0.0f;
		fY = 0.0f;
		fSX = 0.0f;
		fSY = 0.0f;
		fDX = 0.0f;
		fDY = 0.0f;
		fFlowDX = 0.0f;
		fFlowDY = 0.0f;
		fRotateX = 0.0f;
		fRotateY = 0.0f;
		fScaleX = 0.0f;
		fScaleY = 0.0f;
		iLightMap = 0;
		eBGLayer = BG_ID_LAYER_NULL;

		szImageName[0] = '\0';
	}

	CONVERT_BG_INFO& operator = (const CONVERT_BG_INFO& bgInfo)
	{
		fX = bgInfo.fX;
		fY = bgInfo.fY;
		fSX = bgInfo.fSX;
		fSY = bgInfo.fSY;
		fDX = bgInfo.fDX;
		fDY = bgInfo.fDY;
		fFlowDX = bgInfo.fFlowDX;
		fFlowDY = bgInfo.fFlowDY;
		fRotateX = bgInfo.fRotateX;
		fRotateY = bgInfo.fRotateY;
		fScaleX = bgInfo.fScaleX;
		fScaleY = bgInfo.fScaleY;
		iLightMap = bgInfo.iLightMap;
		eBGLayer = bgInfo.eBGLayer;

		strcpy(szImageName, bgInfo.szImageName);
		return *this;
	}
};

class SPBGILoader
{

public:

	SPBGILoader();
	virtual ~SPBGILoader();

	//파라미터로 패스를 받을수 있도록 수정 04.10.14 - jinhee
	bool LoadBGDataText(std::vector<SPRotateScrollImage*>* pvpBackRotateImage,
		string strPath = "", string strFile = "");
	bool LoadBGBinary(std::vector<SPRotateScrollImage*>* pvpBackRotateImage,
		string strPath = "", string strFile = "");

	//float GetCXTerrain()	{ return m_fCXTerrain; }	
	//float GetCYTerrain()	{ return m_fCYTerrain; }	
	


protected:

	DECLARE_WORDCASE_MAP( OnComment				);	
	DECLARE_WORDCASE_MAP( OnBlockBegin			);
	DECLARE_WORDCASE_MAP( OnBlockEnd			);
	DECLARE_WORDCASE_MAP( OnTextHeader			);		
	DECLARE_WORDCASE_MAP( OnLayerBackground0	);	
	DECLARE_WORDCASE_MAP( OnLayerBackground1	);	
	DECLARE_WORDCASE_MAP( OnLayerBackground2	);		
	DECLARE_WORDCASE_MAP( OnLayerBackground3	);	
							
	DECLARE_WORDCASE_MAP( OnLayerTerrain0		);			
	DECLARE_WORDCASE_MAP( OnLayerTerrain1		);		
	DECLARE_WORDCASE_MAP( OnLayerTerrain2		);		

	DECLARE_WORDCASE_MAP( OnLayerForeground0	);		
	DECLARE_WORDCASE_MAP( OnLayerForeground1	);	
	DECLARE_WORDCASE_MAP( OnLayerForeground2	);	
	DECLARE_WORDCASE_MAP( OnLayerForeground3	);		
								
	DECLARE_WORDCASE_MAP( OnMemberFX			);	
	DECLARE_WORDCASE_MAP( OnMemberFY			);		
	DECLARE_WORDCASE_MAP( OnMemberFSX			);		
	DECLARE_WORDCASE_MAP( OnMemberFSY			);		
	DECLARE_WORDCASE_MAP( OnMemberFDX			);			
	DECLARE_WORDCASE_MAP( OnMemberFDY			);		
	DECLARE_WORDCASE_MAP( OnMemberFFLOWDX		);		
	DECLARE_WORDCASE_MAP( OnMemberFFLOWDY		);
	DECLARE_WORDCASE_MAP( OnMemberFROTATESTEPX	);
	DECLARE_WORDCASE_MAP( OnMemberFROTATESTEPY	);
	DECLARE_WORDCASE_MAP( OnMemberIMAGE			);	

	DECLARE_WORDCASE_MAP( OnMemberSCALEX		);	//jinhee - 04.11.03
	DECLARE_WORDCASE_MAP( OnMemberSCALEY		);	//jinhee - 04.11.03
	DECLARE_WORDCASE_MAP( OnMemberLIGHTMAP		);	//jinhee - 04.11.03

	//DECLARE_WORDCASE_MAP( OnMemberCXTerrain		);		
	//DECLARE_WORDCASE_MAP( OnMemberCYTerrain		);	



protected:

	STATE_BLOCK		m_StateBlock;
	BG_ID			m_bgStateImage;

	std::vector<SPRotateScrollImage*> m_vpBackRotateImageCopy;
	//float			m_fCXTerrain;
	//float			m_fCYTerrain;


};
