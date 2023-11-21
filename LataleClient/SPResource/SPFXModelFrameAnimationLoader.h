// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-9    13:55 
//***************************************************************************

#pragma once

#include <string>

class SPFXModel;
class SPFXModelBaseLoader;
class SPStream;

enum SPID_FX;

struct SPFXStreamDefaultData;

class SPFXModelFrameAnimationLoader : public SPFXModelBaseLoader
{
private:

	// {{ WORD CASE MAP
	DECLARE_WORDCASE_MAP ( OnBlockBegin				);		//	블럭 시작
	DECLARE_WORDCASE_MAP ( OnTime					);		//	프레임당 지속시간
	DECLARE_WORDCASE_MAP ( OnScale					);		//	프레임당 크기 값 2.0 이하버전에서만 사용!!!
	DECLARE_WORDCASE_MAP ( OnScaleX					);		//	프레임당 X 크기 값
	DECLARE_WORDCASE_MAP ( OnScaleY					);		//	프레임당 Y 크기 값
	DECLARE_WORDCASE_MAP ( OnRadianDegree			);		//	프레임당 회전 각도
	DECLARE_WORDCASE_MAP ( OnLRSwap					);		//	프레임당 좌우 플립 값
	DECLARE_WORDCASE_MAP ( OnColor					);		//	프레임당 컬러 값
	DECLARE_WORDCASE_MAP ( OnSrcRect				);		//	프레임당 이미지 사각형
	DECLARE_WORDCASE_MAP ( OnAdjustRect				);		//	프레임당 렌더할때 보정하는 사각형
	DECLARE_WORDCASE_MAP ( OnBlockEnd				);		//	블럭 끝
	//

	int				m_iDeep;								//	블럭을 체크하기 위한 값
	SPFXFrameData	m_FrameData;							//	프레임 데이터

public:
	SPFXModelFrameAnimationLoader			(void);
	virtual ~SPFXModelFrameAnimationLoader	(void);

	virtual	bool	LoadASCII	( SPFXModel* pModel , SPStream* pFile );
	virtual	bool	LoadBinary	( SPFXModel* pModel , SPStream* pFile , SPFXStreamDefaultData& DefaultData , float fVersion );
};
