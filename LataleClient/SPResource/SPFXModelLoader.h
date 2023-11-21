// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-9    14:13 
//***************************************************************************

#pragma once

#include <string>

class SPFXModel;
//class SPFXFactory;
class SPFXModelBaseLoader;
class SPStream;

enum SPID_FX_MODEL;

class SPFXModelLoader
{
private:

	// {{ WORD CASE MAP
	DECLARE_WORDCASE_MAP( OnComment					);						//	코멘트 처리
	DECLARE_WORDCASE_MAP( OnFXType					);						//	FX타입 읽기
	DECLARE_WORDCASE_MAP( OnFXVersion				);						//	FX버전 읽기
	DECLARE_WORDCASE_MAP( OnFXId					);						//	FX아이디 읽기
	// }}

	SPStream*			m_pFileHandle;										//	파일 핸들
	SPFXModel*			m_pModel;											//	FX모델 데이터

	SPFXFactory< SPFXModelBaseLoader* , SPID_FX_MODEL > m_LoaderFactory;	//	ModelLoader	Factory
	SPFXFactory< SPFXModel* , SPID_FX_MODEL >			m_FXModelFactory;	//	SPFXModel	Factory

	SPID_FX_MODEL		m_eFXModelType;										//	FX 타입
	float				m_fFXModelVersion;									//	FX 버전

	std::string			m_strImgPath;										//	이미지 경로
	std::string			m_strImgName;										//	이미지 파일명

	bool	LoadFileASCII	( std::string strFileName = "" );				//	ASCII 방식으로.
	bool	LoadFileBinary	( std::string strFileName = "" );				//	BINARY 방식으로.

	bool	ExeSubLoader	( int iFXModelID );							//	FX_ID를 읽어 해당되는 로더를 호출한다.

public:
	SPFXModelLoader(void);
	virtual ~SPFXModelLoader(void);

	//	해당 파일 데이터를 읽어들어 FX모델에다 추가한 후 돌려준다.
	SPFXModel* LoadFile( std::string strFileName = "" );
	const char* GetImgPath()	{	return m_strImgPath.c_str();	}
	const char* GetImgName()	{	return m_strImgName.c_str();	}
};
