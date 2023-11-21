// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-6-28    15:24 
//***************************************************************************

#pragma once

enum SKILL_INVENTORY_TYPE;
enum EFFECT_STATUS_KIND_TYPE;

#define GS_SKILL						1200000
#define GS_SKILL_LEVEL					( GS_SKILL + 1000 )
#define GS_SKILL_GET_LEVEL				( GS_SKILL + 2000 )
#define GS_SKILL_MAX_LEVEL				( GS_SKILL + 2001 )
#define GS_SKILL_TYPE					( GS_SKILL + 3000 )
#define GS_SKILL_REQUIRE_STAT			( GS_SKILL + 4000 )
#define GS_SKILL_CONSUMPTION_STAT		( GS_SKILL + 5000 )
#define GS_SKILL_REQUIREITEM_KIND		( GS_SKILL + 6000 )
#define GS_SKILL_COOLTIME				( GS_SKILL + 7000 )
#define GS_SKILL_INVENTORY_TYPE			( GS_SKILL + 8000 )
#define GS_SKILL_LEVEL_UP_POINT			( GS_SKILL + 9000 )
#define GS_SKILL_UPREQUIRE_STAT			( GS_SKILL + 10000 )

class SPUIUnit;
class SPSkill;
class SPSkillActivity;

class SPUISkillUnit : public SPUIUnit
{
public:
	SPUISkillUnit								(void);
	virtual ~SPUISkillUnit						(void);

	//	기본 인터페이스
	virtual	void			Init				();
	virtual	void			Clear				();
	virtual	void			Process				( float fTime );
	virtual	void			Render				();
	virtual	BOOL			Use					();

	//	스킬 아이디 인터페이스
	void					SetSkillID			( int iSkillID );
	int						GetSkillID			();

	//	스킬 인벤토리 타입 인터페이스
	void					SetType				( SKILL_INVENTORY_TYPE eType );
	SKILL_INVENTORY_TYPE	GetType				();

	//	아이템 아이디 인터페이스
	void					SetItemID			( int iItemID );
	int						GetItemID			();

	//	CONTAINER_SLOT 인터페이스 - 아이템 스킬일 경우 CONTAINER_SLOT를 세팅해야 한다.
	void					SetContainerSlot	( CONTAINER_SLOT stContainerSlot );
	CONTAINER_SLOT			GetContainerSlot	();

	//	아이템 정보 로드 인터페이스
	void					ReItemInfoLoad		();

	//	자기 자신 복사 인터페이스
	virtual SPUIUnit*		ClonePtr			();
	virtual BOOL			Copy				( SPUIUnit** ppDestUIUnit );

	//	아이디로 리소스 재 로드
	virtual	BOOL			ReResourceLoad		();

	//	콤보 텍스처 인터페이스
	void					SetComboTexture		( SPTexture* pTexture );
	BOOL					SetComboTexture		( const char* pstrTexture );
	SPTexture*				GetComboTexture		();

	//	콤보 텍스처 영역 인터페이스
	void					SetComboRectSrc		( RECT rtSrc );
	void					SetComboRectSrc		( int ixLeft , int iyTop , int ixRight , int iyBottom );
	RECT					GetComboRectSrc		();

	//	콤보 세팅 인터페이스
	void					SetCombo			( BOOL bCombo );
	BOOL					GetCombo			();

	//	콤보 표시 시간 인터페이스
	void					SetComboLimitTime	( float fLimitTime );
	float					GetComboLimitTime	();

	//	토글 텍스처 인터페이스
	void					SetToggleTexture	( SPTexture* pTexture );
	BOOL					SetToggleTexture	( const char* pstrTexture );
	SPTexture*				GetToggleTexture	();

	//	토클 스킬 인터페이스
	void					SetToggle			( BOOL bToggle );
	BOOL					GetToggle			();

protected:

	//	콤보 표시 프로세서
	void					ProcessCombo		( float fTime );

	//	Toggle 표시 프로세서
	void					ProcessToggle		( float fTime );

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	int						m_iSkillID;
	SKILL_INVENTORY_TYPE	m_eType;
	int						m_iItemID;
	CONTAINER_SLOT			m_stContainerSlot;

	SPTexture*				m_pComboTexture;
	RECT					m_rtComboSrc;

	BOOL					m_bCombo;
	D3DXCOLOR				m_stComboColor;

	float					m_fComboLimitTime;
	float					m_fComboAccmulateTime;

	BOOL					m_bToggle;
	int						m_iCurToggleFrame;
	int						m_iMaxToggleFrame;
	float					m_fToggleLimitTime;
	float					m_fToggleAccmulateTime;
	SPTexture*				m_pToggleTexture;
	RECT					m_rtToggleSrc;
	POINT					m_ptToogleRenderOffset;
	POINT					m_ptToggleSize;

};
