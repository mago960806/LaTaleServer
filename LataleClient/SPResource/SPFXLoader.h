// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-4-20    15:37 
//***************************************************************************

#pragma once

#include <string>

class SPFXGroupLoader;
class SPFXModelLoader;
class SPFXModel;

struct SPFXGroup;

class SPFXLoader
{
private:
	SPFXGroupLoader		m_FXGroupLoader;
	SPFXModelLoader		m_FXModelLoader;

public:
	SPFXLoader(void);
	virtual ~SPFXLoader(void);

	//	FX관련된 파일을 로드한다.
	//	내부에서 new해서 돌려주기 때문에 사용하는곳에선 delete가 필수적이다!!!
	SPFXGroup*	LoadGroupFile( std::string strFileName = "" );	//	FXG 파일을 로드해서 데이터를 돌려준다.
	SPFXModel*	LoadModelFile( std::string strFileName = "" );	//	FXM 파일을 로드해서 데이터를 돌려준다.

	//	FXModelLoader에서 캐릭터툴에 사용할 파일패스를 가져온다.
	const char* GetFXMImgPath()	{	return m_FXModelLoader.GetImgPath();	}
	const char* GetFXMImgName()	{	return m_FXModelLoader.GetImgName();	}
};
