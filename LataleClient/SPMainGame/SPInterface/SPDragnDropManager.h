// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-7-1    10:22 
//***************************************************************************

#pragma once

class SPTexture;
class SPWindow;
class SPUIUnit;

class SPDragnDropManager
{
private:
	SPDragnDropManager			();

	static	SPDragnDropManager*	m_pDragnDropManagerInstance;

public:
	virtual ~SPDragnDropManager	();

	static	SPDragnDropManager*	GetInstance(void);
	static	void				DelInstance(void);

	enum DRAGNDROP_STATUS_TYPE
	{
		DRAGNDROP_STATUS_ENABLE		=	0	,
		DRAGNDROP_STATUS_DISABLE			,
		DRAGNDROP_STATUS_ISDRAGGING			,
		DRAGNDROP_STATUS_MAX				,
	};

	//	초기화
	void						Init				();

	//	드래그 종료
	void						End					();

	//	매니저 상태 인터페이스
	BOOL						IsDragging			();
	BOOL						IsEnable			();

	//	드래그앤 드랍을 사용하기 위한 세팅 인터페이스
	BOOL						Set					( SPWindow* pWindow , SPUIUnit* pUIUnit );
	BOOL						Get					( SPWindow** ppWindow , SPUIUnit** ppUIUnit );

protected:

	//	드래그앤 드랍을 사용하기 위한 세팅 인터페이스
	BOOL						SetSPWindow			( SPWindow*		pWindow			);
	BOOL						SetUIUnit			( SPUIUnit*		pUIUnit			);

	//	드래그앤 드랍을 사용하기 위한 가져오기 인터페이스
	SPWindow*					GetSPWindow			();
	SPUIUnit*					GetUIUnit			();


	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	DRAGNDROP_STATUS_TYPE		m_eStatus;				//		매니저의 현재 상태

	SPWindow*					m_pWindow;				//		드래그 앤 드랍을 사용 하는 윈도우
	SPUIUnit*					m_pUIUnit;				//		드래그 앤 드랍되는 Unit

};
