// ***************************************************************
//  SPFXCriticalUnit.h   version:  1.0   date:2006/01/24
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPFXFrameAnimationUnit;

class SPFXCriticalUnit : public SPFXFrameAnimationUnit
{
public:
	SPFXCriticalUnit			();
	virtual	~SPFXCriticalUnit	();

	virtual void	SetNull		();
	virtual void	Init		();

	virtual bool	Render		( float fTime );

	void			SetGOBBox	( POINT ptBox );

private:

	void			CalRenderPos( int& iXPos , int& iYPos );


	POINT			m_ptGOBBox;

};
