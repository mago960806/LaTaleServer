// ***************************************************************
//  SPFXFrameAnimationUnit   version:  1.0   ¡¤  date: 09/29/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPFXUnit;

class SPFXFrameAnimationUnit : public SPFXUnit
{
protected:
	int									m_iCurFrameNum;
	int									m_iMaxFrameCount;

public:
	SPFXFrameAnimationUnit				(void);
	virtual ~SPFXFrameAnimationUnit		(void);

	virtual void	SetNull				();
	virtual	void	Init				();
	virtual	bool	Process				( float fTime );
	virtual	bool	Render				( float fTime );
};
