// ***************************************************************
//  SPFXUtil   version:  1.0   ·  date: 09/29/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-9    10:17 
//***************************************************************************

#ifdef _LATALE_CLIENT
#include "SPCommon.h"	
#else 
#include "SPToolCommon.h"
#endif

#include "SPFXDEF.h"
#include "SPFXUtil.h"

//////////////////////////////////////////////////////////////////////////

void SPFX_UTIL::Lerp( float& fOut , float fStart , float fEnd , float fLerpTime )
{
	fOut	=	fStart	*	( 1.0f	-	fLerpTime )	+	fEnd	*	fLerpTime;
}

void SPFX_UTIL::Lerp( int& iOut , int iStart , int iEnd , float fLerpTime )
{
	float	fOut	=	0.0f;

	SPFX_UTIL::Lerp( fOut , (float)iStart , (float)iEnd , fLerpTime );

	iOut	=	(int)fOut;
}

void SPFX_UTIL::Lerp( RECT& rcOut , RECT rcStart , RECT rcEnd , float fLerpTime )
{
	float	fOut	=	0.0f;

	SPFX_UTIL::Lerp( fOut , (float)rcStart.left , (float)rcEnd.left , fLerpTime );
	rcOut.left	=	(int)fOut;

	SPFX_UTIL::Lerp( fOut , (float)rcStart.top , (float)rcEnd.top , fLerpTime );
	rcOut.top	=	(int)fOut;

	SPFX_UTIL::Lerp( fOut , (float)rcStart.right , (float)rcEnd.right , fLerpTime );
	rcOut.right	=	(int)fOut;

	SPFX_UTIL::Lerp( fOut , (float)rcStart.bottom , (float)rcEnd.bottom , fLerpTime );
	rcOut.bottom	=	(int)fOut;
}

void SPFX_UTIL::CalEllipse( float& fOutXPos , float& fOutYPos , float fXCenterPos , float fYCenterPos , float fRadiusX , float fRadiusY , float fAngle )
{
	fOutXPos	=	sin( D3DXToRadian( fAngle ) ) * fRadiusX + fXCenterPos;
	fOutYPos	=	cos( D3DXToRadian( fAngle ) ) * fRadiusY + fYCenterPos;
}

void SPFX_UTIL::CalAngle2D( float& fAngle , float fStartXPos , float fStartYPos , float fEndXPos , float fEndYPos )
{
	D3DXVECTOR2	v1( (FLOAT)fStartXPos	, (FLOAT)fStartYPos );
	D3DXVECTOR2	v2( (FLOAT)fEndXPos		, (FLOAT)fEndYPos );

	D3DXVec2Normalize( &v1 , &v1 );
	D3DXVec2Normalize( &v2 , &v2 );

	fAngle	=	D3DXVec2Dot( &v1 , &v2 );

	if( v1.y * v2.x + v1.x * v2.y > 0.0f )
		fAngle	=	(float)acos( fAngle );
	else
		fAngle	=	-(float)acos( fAngle );

	fAngle	*=	(180.0f / D3DX_PI);
}

float SPFX_UTIL::Distance2D( float fStartXPos , float fStartYPos , float fEndXPos , float fEndYPos )
{
	float	fOne	=	fEndXPos	-	fStartXPos;
	float	fTwo	=	fEndYPos	-	fStartYPos;

	return	(float)sqrt( fOne * fOne + fTwo * fTwo );
}

bool SPFX_UTIL::GetIntersection( float& fOutXPos , float& fOutYPos , float fLine1StartXPos , float fLine1StartYPos , float fLine1EndXPos , float fLine1EndYPos , float fLine2StartXPos , float fLine2StartYPos , float fLine2EndXPos , float fLine2EndYPos )
{
	D3DXVECTOR2	vLine1Start	( (FLOAT)fLine1StartXPos	, (FLOAT)fLine1StartYPos );
	D3DXVECTOR2	vLine1End	( (FLOAT)fLine1EndXPos		, (FLOAT)fLine1EndYPos );
	D3DXVECTOR2	vLine2Start	( (FLOAT)fLine2StartXPos	, (FLOAT)fLine2StartYPos );
	D3DXVECTOR2	vLine2End	( (FLOAT)fLine2EndXPos		, (FLOAT)fLine2EndYPos );

	D3DXVECTOR2	vA	=	vLine2End	-	vLine2Start;
	D3DXVECTOR2	vB	=	vLine1End	-	vLine1Start;
	D3DXVECTOR2 vC	=	vLine1Start	-	vLine2Start;

	D3DXVECTOR2	vAp( -vA.y , vA.x );
	D3DXVECTOR2	vBp( -vB.y , vB.x );
	D3DXVECTOR2	vCp( -vC.y , vC.x );

	float	fD	=	D3DXVec2Dot( &vC , &vAp );
	float	fE	=	D3DXVec2Dot( &vC , &vBp );
	float	fF	=	D3DXVec2Dot( &vA , &vBp );

	if( abs( fF ) > 0.0001f )
	{
		float	fT1	=	fD	/	fF;
		float	fT2	=	fE	/	fF;

		if( fT1 >= 0.0f && fT1 <= 1.0f )
		{
			if( fT2 >= 0.0f && fT2 <= 1.0f )
			{
				D3DXVECTOR2	vIntersection	=	vLine1Start	+	fT1	*	vB;

				fOutXPos	=	vIntersection.x;
				fOutYPos	=	vIntersection.y;

				return true;
			}
		}
	}

	fOutXPos	=	0.0f;
	fOutXPos	=	0.0f;

	return false;
}

bool SPFX_UTIL::Collision_Triangle_Point_2D( float fTri0XPos , float fTri0YPos , float fTri1XPos , float fTri1YPos , float fTri2XPos , float fTri2YPos , float fPointXPos , float fPointYPos )
{
	float	fAXPos	=	fTri1XPos	-	fTri0XPos;
	float	fAYPos	=	fTri1YPos	-	fTri0YPos;

	float	fBXPos	=	fTri2XPos	-	fTri0XPos;
	float	fBYPos	=	fTri2YPos	-	fTri0YPos;

	float	fCXPos	=	fPointXPos	-	fTri0XPos;
	float	fCYPos	=	fPointYPos	-	fTri0YPos;

	float	fD		=	fAXPos * fBYPos - fAYPos * fBXPos;

	if( fabs( fD ) <= 0.00001f )
		return false;

	float	fInvD	=	1.0f / fD;
	float	fAlpha	=	fInvD * ( fCXPos * fBYPos - fCYPos * fBXPos );

	if( fAlpha < 0.0f || fAlpha > 1.0f )
		return false;

	float	fBeta	=	fInvD * ( fCYPos * fAXPos - fCXPos * fAYPos );

	if( fBeta < 0.0f || fBeta > 1.0f )
		return false; 

	float	fTemp	=	fAlpha + fBeta;

	if( fTemp < 0.0f || fTemp > 1.0f )
		return false;

	return true;
}

bool SPFX_UTIL::Collision_Rectangle_Point_2D( float fTri0XPos , float fTri0YPos , float fTri1XPos , float fTri1YPos , float fTri2XPos , float fTri2YPos , float fTri3XPos , float fTri3YPos , float fPointXPos , float fPointYPos )
{
	if( Collision_Triangle_Point_2D( fTri0XPos , fTri0YPos , fTri1XPos , fTri1YPos , fTri2XPos , fTri2YPos , fPointXPos , fPointYPos ) == true ||
		Collision_Triangle_Point_2D( fTri2XPos , fTri2YPos , fTri3XPos , fTri3YPos , fTri0XPos , fTri0YPos , fPointXPos , fPointYPos ) == true )
	{
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

bool SPFX_UTIL::CalMapParticleValue( float& fStartValueOut , float& fEndValueOut , float& fStartTimeOut , float& fEndTimeOut , STD_MAP_PARTICLE_VALUE* pmParticleValue , float fTime )
{
	if( pmParticleValue == NULL )
		return false;

	if( pmParticleValue->empty() == true )
		return false;

	//	순차 검색 버전
	STD_MAP_PARTICLE_VALUE::iterator			mIter	=	pmParticleValue->begin();

	while( mIter != pmParticleValue->end() )
	{
		if( mIter->first <= fTime )
		{
			fStartTimeOut	=	mIter->first;
			fStartValueOut	=	mIter->second;
		}
		else if( mIter->first > fTime )
		{
			fEndTimeOut		=	mIter->first;
			fEndValueOut	=	mIter->second;
			break;
		}
		
		++mIter;
	}

	if( fEndTimeOut <= 0.0f )
	{
		fEndTimeOut		=	fTime;
		fEndValueOut	=	fStartValueOut;
	}

	return true;
}

void SPFX_UTIL::CalMapParticleValueLerp( float& fCalOut , float fStartValue , float fEndValue , float fStartTime , float fEndTime , float fTime )
{
	float fLerpTime;

	fEndTime	-=	fStartTime;
	fStartTime	=	fTime	-	fStartTime;

	if( fStartTime < 0.001f || fEndTime < 0.001f )
		fLerpTime	=	0.0f;
	else
		fLerpTime	=	fStartTime	/	fEndTime;

	SPFX_UTIL::Lerp( fCalOut , fStartValue , fEndValue ,  fLerpTime );
}
