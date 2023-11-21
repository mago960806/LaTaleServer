// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-7-15    18:31 
//***************************************************************************

#pragma once

#define ITEM_COUNT_MAX	999
#define ITEM_LENGTH_MAX	3

class SPUIUnit;

class SPUIItemUnit : public SPUIUnit
{
public:
	SPUIItemUnit								(void);
	virtual ~SPUIItemUnit						(void);

	//	기본 인터페이스
	virtual	void			Init				();
	virtual	void			Clear				();
	virtual	void			Process				( float fTime );
	virtual	void			Render				();
	virtual	BOOL			Use					();

	//	아이템 아이디 인터페이스
	void					SetItemID			( int iItemID );
	int						GetItemID			();

	//	아이템 타입 인터페이스
	void					SetItemType			( CONTAINER_TYPE eType );
	CONTAINER_TYPE			GetItemType			();

	//	자기 자신 복사 인터페이스
	virtual SPUIUnit*		ClonePtr			();
	virtual BOOL			Copy				( SPUIUnit** ppDestUIUnit );

	//	아이디로 리소스 재 로드
	virtual	BOOL			ReResourceLoad		();

	//	아이템 백그라운드 텍스처 인터페이스
	void					SetBGTexture		( SPTexture* pTexture );
	BOOL					SetBGTexture		( const char* pstrTexture );
	SPTexture*				GetBGTexture		();
	
	//	아이템 백그라운드 텍스처 영역 인터페이스
	void					SetBGRectSrc		( RECT rtSrc );
	void					SetBGRectSrc		( int ixLeft , int iyTop , int ixRight , int iyBottom );
	RECT					GetBGRectSrc		();

	//	아이템 갯수 체크 인터페이스
	void					CheckCount			();
	void					SetCount			( int iCount );
	int						GetCount			();

	//	아이템 아이콘 텍스처 인터페이스
	void					SetIconTexture		( SPTexture* pTexture );
	BOOL					SetIconTexture		( const char* pstrTexture );
	SPTexture*				GetIconTexture		();

	//	아이템 갯수 프로세서 인터페이스
	void					ProcessCount		();

	//	아이템 기간 표시 유무
	void					SetDuration			( BOOL bDuration );
	BOOL					GetDuration			();

protected:

	//	아이템 갯수 렌더 인터페이스
	void					RenderCount			();

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	int						m_iItemID;
	CONTAINER_TYPE			m_eItemType;

	SPTexture*				m_pBGTexture;
	RECT					m_rtBGSrc;

	int						m_iCount;

	int						m_iCountLength;

	RECT					m_rtCountSrc[ ITEM_LENGTH_MAX ];
	POINT					m_ptCountPos[ ITEM_LENGTH_MAX ];

	SPTexture*				m_pIconTexture;
	RECT					m_rtIconSrc;

	BOOL					m_bDuration;

};
