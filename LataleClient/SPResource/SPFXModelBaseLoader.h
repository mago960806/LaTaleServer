// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-9    13:39 
//***************************************************************************

#pragma once

#include <string>

class SPFXModel;
class SPTexture;
class SPStream;

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

//	FX 공용 바이너리 파일헤더

#define FXM_VERSION		3.2f

struct SPFXStreamHeader {
	enum { BINARY_CHECK = 415 };

	char			szDescription[255];
	int				iBinary;
	float			fVersion;
	unsigned long	ulExpansion;

	SPFXStreamHeader()
	{
		memset( szDescription , 0 , sizeof( szDescription ) );
		iBinary		=	0;
		fVersion	=	0.0f;
		ulExpansion	=	0;
	}
};

//	Ver 1.0 - > 1.1	변경사항
//		Loop , ScreenRender 항목을 iFollowParent에 같이 삽입
//	Ver 1.1 - > 2.0	변경사항
//		bool			bLightMap;	-> TEXTURE_EFFECT	eTextureEffect; 으로 변경
//	Ver 2.0 - > 2.1	변경사항
//		SPFXFrameData클래스내	fFrameScale	-> fFrameScaleX , fFrameScaleY 추가.
//	Ver 2.1 - > 3.0 변경사항
//		파티클 완전 리뉴얼 ( 2.1도 로드를 하나 3.0으로 데이터 변환은 하지 않음 )
//	Ver 3.0 - > 3.1 변경사항
//		SPFXStreamDefaultData 클래스내 Model ID , Type 추가 , fDelayTime 값 삭제
//	Ver 3.1 - > 3.2 변경사항
//		SPFXStreamDefaultData 클래스내 FXM Render Layer 추가

//	FX Model 기본 데이터

struct SPFXStreamDefaultData {	// AJJIYA Ver.Now!!! [10/6/2008 AJJIYA]
	TEXTURE_EFFECT	eTextureEffect;
	int				iFollowParent;
	float			fLifeTime;
	char			szPath[255];
	UINT			uiFXModelID;
	SPID_FX_MODEL	eFXModelType;
	int				iFXMRenderLayer;

	SPFXStreamDefaultData()
	{
		memset( szPath , 0 , sizeof( szPath ) );
		eTextureEffect	=	TEXTURE_EFFECT_NULL;
		iFollowParent	=	0;
		fLifeTime		=	0.0;
		uiFXModelID		=	0;
		eFXModelType	=	IDFXM_NULL;
		iFXMRenderLayer	=	0;
	}
};

struct SPFXStreamDefaultData31 {	// AJJIYA Ver3.1 [12/1/2005]
	TEXTURE_EFFECT	eTextureEffect;
	int				iFollowParent;
	float			fLifeTime;
	char			szPath[255];
	UINT			uiFXModelID;
	SPID_FX_MODEL	eFXModelType;

	SPFXStreamDefaultData31()
	{
		memset( szPath , 0 , sizeof( szPath ) );
		eTextureEffect	=	TEXTURE_EFFECT_NULL;
		iFollowParent	=	0;
		fLifeTime		=	0.0;
		uiFXModelID		=	0;
		eFXModelType	=	IDFXM_NULL;
	}
};

struct SPFXStreamDefaultData30 {	// AJJIYA Ver3.0 [12/1/2005]
	TEXTURE_EFFECT	eTextureEffect;
	int				iFollowParent;
	float			fLifeTime;
	float			fDelayTime;
	char			szPath[255];

	SPFXStreamDefaultData30()
	{
		memset( szPath , 0 , sizeof( szPath ) );
		eTextureEffect	=	TEXTURE_EFFECT_NULL;
		iFollowParent	=	0;
		fLifeTime		=	0.0;
		fDelayTime		=	0.0;
	}
};

struct SPFXStreamDefaultData11 {	// AJJIYA Ver1.1 [12/1/2005]
	bool			bLightMap;
	int				iFollowParent;
	float			fLifeTime;
	float			fDelayTime;
	char			szPath[255];

	SPFXStreamDefaultData11()
	{
		memset( szPath , 0 , sizeof( szPath ) );
		bLightMap		=	false;
		iFollowParent	=	0;
		fLifeTime		=	0.0;
		fDelayTime		=	0.0;
	}
};

class SPFXModelBaseLoader
{
protected:

	// {{ WORD CASE MAP
	DECLARE_WORDCASE_MAP ( OnComment				);		//	코맨트 처리
	DECLARE_WORDCASE_MAP ( OnPath					);		//	텍스처 파일 경로
	DECLARE_WORDCASE_MAP ( OnLifeTime				);		//	지속 시간 읽기
	DECLARE_WORDCASE_MAP ( OnFollowParent			);		//	부모를 따라 다닐때의 옵션
	DECLARE_WORDCASE_MAP ( OnLightMap				);		//	라이맵 사용 유무
	DECLARE_WORDCASE_MAP ( OnModelType				);		//	모델 타입
	DECLARE_WORDCASE_MAP ( OnRenderLayer			);		//	렌더 레이어
	// }}

	SPFXModel*		m_pModel;								//	상위에서 받아온 모델 데이터
	std::string		m_strImgPath;							//	캐릭터툴에서 사용할 이미지 경로
	std::string		m_strImgName;							//	캐릭터툴에서 사용할 이미지 파일명

	//	경로를 주어 텍스처를 받아온다.
	bool			GetTexture( const char* pszPath , SPTexture** ppTexture );

public:
	SPFXModelBaseLoader(void);
	virtual ~SPFXModelBaseLoader(void);

	//	하위 자식들이 알아서 로드한다.
	virtual	bool	LoadASCII	( SPFXModel* pModel , SPStream* pFile ) = 0;
	virtual	bool	LoadBinary	( SPFXModel* pModel , SPStream* pFile , SPFXStreamDefaultData& DefaultData , float fVersion );

	const char*	GetImgPath()	{	return m_strImgPath.c_str();	}
	const char* GetImgName()	{	return m_strImgName.c_str();	}
};

extern	bool		IsASCIIFile	( const char* pszFileName );
extern	bool		StrToChar	( char* pszOut , std::string *strLine );
extern	int			StrToInt	( std::string *strLine );
extern	bool		StrToBool	( std::string *strLine );
extern	float		StrToFloat	( std::string *strLine );
extern	POINT		StrToPoint	( std::string *strLine );
extern	RECT		StrToRect	( std::string *strLine );
extern	D3DXCOLOR	StrToColor	( std::string *strLine );
