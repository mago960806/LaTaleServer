// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-6-28    14:41 
//***************************************************************************

#pragma once


class SPUIUnit
{
public:
	enum STATUS_UI_UNIT
	{
		STATUS_UI_UNIT_NONE			=	0x0000	,
		STATUS_UI_UNIT_RENDER		=	0x0100	,
		STATUS_UI_UNIT_DRAGNDROP	=	0x0200	,
		STATUS_UI_UNIT_COOLTIME		=	0x0400	,
		STATUS_UI_UNIT_NO_DRAGNDROP	=	0x0800	,
		STATUS_UI_UNIT_NO_TOOLTIP	=	0x1000	,
		STATUS_UI_UNIT_MASK			=	0xFF00	,
		STATUS_UI_UNIT_ENABLE		=	0x0001	,
		STATUS_UI_UNIT_DISABLE		=	0x0002	,
	};

	SPUIUnit								(void);
	virtual ~SPUIUnit						(void);

	//	기본 인터페이스
	virtual	void		Init				();
	virtual	void		Clear				();
	virtual	void		Process				( float fTime );
	virtual	void		Render				();
	virtual	BOOL		Use					();
	virtual	BOOL		MouseOver			( int iX , int iY );

	//	InstanceID 인터페이스
	void				SetInstanceID		( INSTANCE_ID iID );
	INSTANCE_ID			GetInstanceID		();

	//	Status 인터페이스
	void				SetStatus			( DWORD dwStatus );
	void				AddStatus			( DWORD dwStatus );
	void				DelStatus			( DWORD dwStatus );
	BOOL				GetStatus			( DWORD dwStatus );
	DWORD				GetStatus			();

	//	리소스 세팅 인터페이스
	BOOL				SetResource			( int iResID , int iResIndex );

	//	텍스쳐 렌더 영역 인터페이스
	void				SetRectSrc			( RECT rtSrc );
	void				SetRectSrc			( int ixLeft , int iyTop , int ixRight , int iyBottom );
	RECT				GetRectSrc			();

	//	텍스쳐 렌더 위치 인터페이스
	void				SetPos				( POINT ptPos );
	void				SetPos				( LONG lX , LONG lY );
	POINT				GetPos				();

	//	텍스쳐 사이즈 인터페이스
	void				SetSize				( POINT ptSize );
	void				SetSize				( LONG lX , LONG lY );
	POINT				GetSize				();

	//	렌더 텍스쳐 인터페이스
	void				SetTexture			( SPTexture* pTexture );
	BOOL				SetTexture			( const char* pstrTexture );
	SPTexture*			GetTexture			();

	//	비활성화 텍스쳐 렌더 영역 인터페이스
	void				SetDisableRectSrc	( RECT rtSrc );
	void				SetDisableRectSrc	( int ixLeft , int iyTop , int ixRight , int iyBottom );
	RECT				GetDisableRectSrc	();

	//	비활성화 텍스쳐 인터페이스
	void				SetDisableTexture	( SPTexture* pTexture );
	BOOL				SetDisableTexture	( const char* pstrTexture );
	SPTexture*			GetDisableTexture	();

	//	비활성화 컬러 인터페이스
	void				SetDisableColor		( D3DXCOLOR cColor );
	D3DXCOLOR			GetDisableColor		();

	//	자기 자신 복사 인터페이스
	virtual SPUIUnit*	ClonePtr			();
	virtual BOOL		Copy				( SPUIUnit** ppDestUIUnit );

	//	아이디로 리소스 재 로드
	virtual	BOOL		ReResourceLoad		();

	//	CoolTime ID 인터페이스
	void				SetCoolTimeID		( int iID );
	int					GetCoolTimeID		();

	//	CoolTime 텍스처 인터페이스
	void				SetCoolTimeTexture	( SPTexture* pTexture );
	BOOL				SetCoolTimeTexture	( const char* pstrTexture );
	SPTexture*			GetCoolTimeTexture	();

	//	CoolTime 텍스처 렌더 영역 인터페이스
	void				SetCoolTimeRectSrc	( RECT rtSrc );
	void				SetCoolTimeRectSrc	( int ixLeft , int iyTop , int ixRight , int iyBottom );
	RECT				GetCoolTimeRectSrc	();

protected:

	//	기타 인터페이스
	BOOL				IsRender			();
	BOOL				IsTexture			();
	BOOL				IsDisableTexture	();
	BOOL				IsCoolTimeTexture	();
	BOOL				IsRectIn			( int iX , int iY );

	//	프로세서 인터페이스
	virtual void		DisablePorcess		();
	virtual	void		CoolTimeProcess		( float fTime );

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	INSTANCE_ID			m_iInstanceID;			//	현재 인스턴스 아이디( SPWindow랑 같음 )
	DWORD				m_dwStatus;				//	현재 Unit의 상태
	
	SPTexture*			m_pTexture;				//	렌더 텍스쳐
	POINT				m_ptPos;				//	텍스쳐 렌더 위치
	POINT				m_ptSize;				//	텍스쳐 크기
	RECT				m_rtSrc;				//	텍스쳐 영역

	SPTexture*			m_pDisableTexture;		//	비활성화시 사용할 텍스쳐
	RECT				m_rtDisableSrc;			//	비활성화시 텍스쳐 영역
	RECT				m_rtDisableTarget;		//	비활성화시 렌더 영역
	D3DXCOLOR			m_cDisableRGBA;			//	비활성화시 사용하는 컬러값

	D3DXCOLOR			m_cRGBA;				//	드래그 상태일때 투명하게 보여주자~

	int					m_iCoolTimeID;			//	쿨타임 아이디
	SPTexture*			m_pCoolTimeTexture;		//	쿨타임 텍스처
	RECT				m_rtCoolTimeSrc;		//	쿨타임 텍스처 영역
	RECT				m_rtCoolTimeTarget;		//	쿨타임 텍스처 타겟 영역

};
