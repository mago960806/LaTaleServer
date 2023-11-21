// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPFXArchive
// Comment     : 
// Creation    : metalgeni 2005-1-26 오전 10:26:19
// Modified	   : AJJIYA 2005-9-1    16:26 
//***************************************************************************

#pragma once

class SPFXGroupUnit;

typedef	std::multimap< UINT , SPFXGroupUnit* >	STD_MULTIMAP_SPFXGROUPUNIT;
typedef	std::vector< GU_ID >					STD_VECTOR_NEXTGOBID;

class SPFXArchive
{
public:
	SPFXArchive					( SPGameObject* pParent );
	virtual	~SPFXArchive		();

	void	SetNULL				();
	void	Init				();
	void	Clean				();
	void	Process				( float fTime );
	void	Render				( float fTime , SP_FXM_RENDER_LAYER eRenderLayer = FXM_RENDER_LAYER_ALL );
	bool	IsEmpty				();

	//	기본 인자들
	void	SetFXGroup			( UINT uiFXGroupID );
	void	SetLPARAM			( LPARAM lParam );
	void	SetSrcPosX			( float fSrcPosX );
	void	SetSrcPosY			( float fSrcPosY );
	void	SetDestPosX			( float fDestPosX );
	void	SetDestPosY			( float fDestPosY );
	void	SetLoop				( BOOL bLoop );
	void	SetContinue			( BOOL bContinue );
    void	SetVelocity			( float fVelocity );
	void	SetLifeTime			( float fLifeTime );
	void	SetTargetGOBID		( GU_ID tTargetGOBID );
	void	SetFollowParent		( int iFollowParent );
	void	SetDealyTime		( float fDealyTime );
	void	SetParentPos		( float fPosX , float fPosY );
	void	SetOffsetPos		( float fPosX , float fPosY );
	void	AddNextFXGroupID	( UINT uiNextFXGroupID );
	void	AddNextTargetGOBID	( GU_ID tTargetGOBID );

	//	데미지에서 사용할 인자들
	void	SetGOBBox			( POINT	ptGOBBox );
	void	SetDamageType		( FXUNIT_DAMAGE_TYPE eDamageType );
	void	SetDamage			( int iDamage );

	//	크리티컬에서 사용할 인자들
	void	SetCritical			( bool bCritical );

	//	생성하자!
	void	InsertFXGroup		( UINT uiFXGroupID = 0 );
	void	InsertFXGroup		( SPID_FX_GROUP eFXGroupID = IDFXGR_NULL );
	void	InsertFXGroup		( UINT uiFXGroupID, SPFXGroup *pFXGroup );

	//	삭제하자!
	void	DeleteFXGroup		( UINT uiFXGroupID );

	//	FXG 존재 확인
	bool	IsFXGroup			( UINT uiOriginalFXGroupID , UINT uiChangeFXGroupID = 0 );

private:
	void	ClearData			();

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	//	기본인자 변수들
	SPGameObject*							m_pParentGOB;
	GU_ID									m_tTargetGOBID;
	UINT									m_uiFXGroupID;
	LPARAM									m_tlParam;
	float									m_fSrcPosX;
	float									m_fSrcPosY;
	float									m_fDestPosX;
	float									m_fDestPosY;
	BOOL									m_bLoop;
	BOOL									m_bContinue;
	float									m_fVelocity;
	float									m_fLifeTime;
	BOOL									m_bEnumType;
	int										m_iFollowParent;
	float									m_fDealyTime;
	float									m_fParentPosX;
	float									m_fParentPosY;
	float									m_fOffsetPosX;
	float									m_fOffsetPosY;

	STD_MULTIMAP_SPFXGROUPUNIT				m_mpFXGroupUnit;


	STD_VECTOR_NEXTFXGROUPID				m_vNextFXGroupIDOrder;
	STD_VECTOR_NEXTGOBID					m_vNextGOBID;
	STD_MULTIMAP_NEXTFXINFO					m_mNextFXInfo;


	//	데미지 인자 변수들
	POINT									m_ptGOBBox;
	FXUNIT_DAMAGE_TYPE						m_eDamageType;
	int										m_iDamage;

	//	크리티컬 인자 변수들
	bool									m_bCritical;

	//	임시 변수
	STD_MULTIMAP_SPFXGROUPUNIT::iterator	m_IterKey;
	STD_MULTIMAP_SPFXGROUPUNIT::iterator	m_IterEnd;
	int										m_iIndex;
	float									m_fPosX;
	float									m_fPosY;
	FACING									m_eFacing;
	SPFXGroupUnit*							m_pFXGroupUnit;
};
