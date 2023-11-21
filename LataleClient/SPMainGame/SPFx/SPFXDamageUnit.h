// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-20    13:55 
//***************************************************************************

#pragma once

#include <string>

class SPFXFrameAnimationUnit;

enum FXUNIT_DAMAGE_EFFECT_TYPE
{
	FDET_POSION			=	40007	,	//	µ∂
	FDET_BLEEDING		=	40008	,	//	√‚«˜
	FDET_CURSE			=	40009	,	//	¿˙¡÷
};

enum FXUNIT_DAMAGE_TYPE
{
	FDT_ATTACK			=	0	,
	FDT_BEATTACK				,
	FDT_POSION					,
	FDT_HEALING					,
};

#define SPFXDAMAGE_NUMBER_COUNT 10

class SPFXDamageUnit : public SPFXFrameAnimationUnit
{
public:
	SPFXDamageUnit			();
	virtual	~SPFXDamageUnit	();

	virtual void	SetNull	();
	virtual void	Init	();

	virtual bool	Process	( float fTime );
	virtual bool	Render	( float fTime );

	void	InitSrcRect		();

	void	SetDamage		( const char* pstrNumber , int iStrLength );
	void	CalStrPos		( int iStrLength );

	void	SetGOBBox		( POINT ptBox );

	void	SetDamageType	( FXUNIT_DAMAGE_TYPE eDamageType );

	RECT	GetRenderRect	();

private:

	void	CalRenderPos	( int& iXPos , int& iYPos );


	std::string				m_strDamage;
	RECT					m_rtNumberSrc[ SPFXDAMAGE_NUMBER_COUNT ];
	POINT					m_ptNumberSize;

	POINT					m_ptGOBBox;

	FXUNIT_DAMAGE_TYPE		m_eDamageType;

	POINT					m_ptOffSet;

	RECT					m_rcRender;
};
