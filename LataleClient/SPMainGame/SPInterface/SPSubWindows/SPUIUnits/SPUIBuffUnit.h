// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-10-10    16:08 
//***************************************************************************

#pragma once

#define TIME_COUNT_MAX		9999
#define TIME_LENGTH_MAX		4

class SPUIUnit;
class SPEffect;

class SPUIBuffUnit : public SPUIUnit
{
public:
	enum BUFF_FONT_COLOR
	{
		BUFF_FONT_WIDTH				=	9	,
		BUFF_FONT_HEIGHT			=	10	,
		BUFF_FONT_COLOR_WHITE		=	0	,
		BUFF_FONT_COLOR_YELLOW				,
		BUFF_FONT_COLOR_GREEN				,
	};

	SPUIBuffUnit								(void);
	virtual ~SPUIBuffUnit						(void);

	//	기본 인터페이스
	virtual	void			Init				();
	virtual	void			Clear				();
	virtual	void			Process				( float fTime );
	virtual	void			Render				();
	virtual	BOOL			Use					();

	//	자기 자신 복사 인터페이스
	virtual SPUIUnit*		ClonePtr			();
	virtual BOOL			Copy				( SPUIUnit** ppDestUIUnit );

	//	리소스 재 로드
//	virtual	BOOL			ReResourceLoad		();

	//	버프 액티비티 유니크 아이디 인터페이스
	void					SetBuffID			( unsigned int uiBuffID );
	unsigned int			GetBuffID			();

	//	버프 이펙트 세팅 인터페이스
	BOOL					SetEffect			( SPEffect* pEffect );

	//	타임 체크 여부 인터페이스
	void					SetTimeCheck		( BOOL bTimeCheck );
	BOOL					GetTimeCheck		();

	//	타임 리소스 정보 만들기
	void					SetTimeData			( int iTime );
	int						GetTime				();

protected:

	//	타임 렌더 인터페이스
	void					RenderTime			();

	//	이펙트 프로세서 인터페이스
	void					ProcessEffect		( float fTime );

	//	이펙트 동작 인터페이스
	void					RunEffect			();

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	unsigned int			m_uiBuffID;

	BOOL					m_bTimeCheck;

	int						m_iTime;
	int						m_iTimeLength;
	RECT					m_rtTimeSrc[ TIME_LENGTH_MAX ];
	POINT					m_ptTimePos[ TIME_LENGTH_MAX ];

	BOOL					m_bEffect;
	float					m_fLimitTime;					//	깜빡임을 위해서
	float					m_fAccmulateTime;				//	깜빡임을 위해서

};
