// ***************************************************************
//  SPComboBoxItemTitle   version:  1.0   ¡¤  date: 03/30/2006
//  -------------------------------------------------------------
//  
//	AJJIYA
//
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPComboBoxItemBase;

class SPTitleRenderBase;

class SPComboBoxItemTitle : public SPComboBoxItemBase
{
public:

	SPComboBoxItemTitle					();
	virtual	~SPComboBoxItemTitle		();

	virtual	void		Init			();
	virtual	void		Clean			();
	virtual	void		Process			( float fTime );
	virtual	void		Render			( float fTime );

	void				SetTitleBase	( SPTitleRenderBase* pTitleBase );
	SPTitleRenderBase*	GetTitleBase	();


protected:

	SPTitleRenderBase*				m_pTitleBase;

};
