// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-8-31    14:21 
//***************************************************************************

#pragma once

typedef	std::vector< UINT >						STD_VECTOR_NEXTFXGROUPID;
typedef	std::multimap< UINT , GU_ID >			STD_MULTIMAP_NEXTFXINFO;

enum FXUNIT_DAMAGE_TYPE;

class SPFXGroupUnit
{
public:
	SPFXGroupUnit							();
	virtual	~SPFXGroupUnit					();

	void			SetNull					();
	BOOL			Init					();
	void			Destory					();

	void			Process					( float fTime );
	void			Render					( float fTime , SP_FXM_RENDER_LAYER eRenderLayer );

	FXUNIT_STATE	GetState				();

	void							SetNextFXGroupIDOrder	( STD_VECTOR_NEXTFXGROUPID& vNextFXGroupID );
	STD_VECTOR_NEXTFXGROUPID&		GetNextFXGroupIDOrder	();

	void							SetNextFXInfo			( STD_MULTIMAP_NEXTFXINFO& vNextFXInfo );
	STD_MULTIMAP_NEXTFXINFO&		GetNextFXInfo			();

	//	기본 인자들
	void			SetParentPos			( float fPosX , float fPosY );
	void			SetParentFacing			( FACING eFacing );
	void			SetFXGroup				( UINT uiFXGroupID );
	void			SetFXGroup				( SPFXGroup* pFXGroup );
	UINT			GetFXGroup				();
	void			SetLPARAM				( LPARAM lParam );
	void			SetSrcPosX				( float fSrcPosX );
	void			SetSrcPosY				( float fSrcPosY );
	void			SetDestPosX				( float fDestPosX );
	void			SetDestPosY				( float fDestPosY );
	void			SetLoop					( BOOL bLoop );
	void			SetContinue				( BOOL bContinue );
	void			SetVelocity				( float fVelocity );
	void			SetLifeTime				( float fLifeTime );
	void			SetTargetGOBID			( GU_ID tTargetGOBID );
	void			SetEnumType				( BOOL bEnumType );
	void			SetFollowParent			( int iFollowParent );
	void			SetDealyTime			( float fDealyTime );
	float			GetDealyTime			();
	void			SetNotDel				( BOOL bNotDel );
	void			SetOffsetPos			( float fPosX , float fPosY );

	//	데미지에서 사용할 인자들
	void			SetDamageFX				( int iDamage , POINT ptGOBBox , FXUNIT_DAMAGE_TYPE eDamageType );
	RECT			GetRenderRect			();

	//	크리티컬에서 사용할 인자들
	void			SetCritical				( POINT ptGOBBox );

private:

	BOOL			InsertFXModel			( UINT uiFXModelID );

	void			SetDamageString			( const char strNumber , int istrLength );
//	BOOL			ProcessDealy			( float fTime );

//	void			AddDamageRenderRect		( RECT rcRender );

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	FXUNIT_STATE							m_eState;
	BOOL									m_bEnumType;

	STD_VECTOR_SPFXUNIT						m_vpFXUnit[ FXM_RENDER_LAYER_COUNT ];

	STD_VECTOR_NEXTFXGROUPID				m_vNextFXGroupIDOrder;
	STD_MULTIMAP_NEXTFXINFO					m_mNextFXInfo;

	//	기본 인자 변수들
	float									m_fParentPosX;
	float									m_fParentPosY;
	FACING									m_eParentFacing;
	SPFXGroup*								m_pFXGroup;
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
	int										m_iFollowParent;
	float									m_fDealyTime;
	BOOL									m_bDealyInit;
	BOOL									m_bNotDel;
	float									m_fOffsetPosX;
	float									m_fOffsetPosY;

	//	데미지 인자 변수들
	POINT									m_ptGOBBox;
	FXUNIT_DAMAGE_TYPE						m_eDamageType;
	int										m_iDamage;
	std::string								m_strNumber;
	int										m_istrLength;
	RECT									m_rcRender;

	//	크리티컬 인자 변수들
	bool									m_bCritical;

	//	임시 변수
	STD_VECTOR_SPFXUNIT::iterator			m_IterKey;
	STD_VECTOR_SPFXUNIT::iterator			m_IterEnd;
	int										m_iIndex;
	int										m_iCount;
	std::vector<UINT>::iterator				m_IterFXIDKey;
	std::vector<UINT>::iterator				m_IterFXIDEnd;
	char									m_szBuf[ _MAX_PATH ];
	SPFXUnit*								m_pFXUnit;
	SP_FXM_RENDER_LAYER						m_eRenderLayerStart;
	SP_FXM_RENDER_LAYER						m_eRenderLayerEnd;

};
