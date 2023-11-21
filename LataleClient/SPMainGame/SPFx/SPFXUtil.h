// ***************************************************************
//  SPFXUtil   version:  1.0   ¡¤  date: 09/29/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

namespace SPFX_UTIL
{
	//////////////////////////////////////////////////////////////////////////

	void			Lerp							( float& fOut , float fStart , float fEnd , float fLerpTime );
	void			Lerp							( int& iOut , int iStart , int iEnd , float fLerpTime );
	void			Lerp							( RECT& rcOut , RECT rcStart , RECT rcEnd , float fLerpTime );

	void			CalEllipse						( float& fOutXPos , float& fOutYPos , float fXCenterPos , float fYCenterPos , float fRadiusX , float fRadiusY , float fAngle );
	void			CalAngle2D						( float& fAngle , float fStartXPos , float fStartYPos , float fEndXPos , float fEndYPos );

	float			Distance2D						( float fStartXPos , float fStartYPos , float fEndXPos , float fEndYPos );

	bool			GetIntersection					( float& fOutXPos , float& fOutYPos , float fLine1StartXPos , float fLine1StartYPos , float fLine1EndXPos , float fLine1EndYPos , float fLine2StartXPos , float fLine2StartYPos , float fLine2EndXPos , float fLine2EndYPos );

	bool			Collision_Triangle_Point_2D		( float fTri0XPos , float fTri0YPos , float fTri1XPos , float fTri1YPos , float fTri2XPos , float fTri2YPos , float fPointXPos , float fPointYPos );
	bool			Collision_Rectangle_Point_2D	( float fTri0XPos , float fTri0YPos , float fTri1XPos , float fTri1YPos , float fTri2XPos , float fTri2YPos , float fTri3XPos , float fTri3YPos , float fPointXPos , float fPointYPos );

	//////////////////////////////////////////////////////////////////////////

	bool			CalMapParticleValue				( float& fStartValueOut , float& fEndValueOut , float& fStartTimeOut , float& fEndTimeOut , STD_MAP_PARTICLE_VALUE* pmParticleValue , float fTime );
	void			CalMapParticleValueLerp			( float& fCalOut , float fStartValue , float fEndValue , float fStartTime , float fEndTime , float fTime );

}
