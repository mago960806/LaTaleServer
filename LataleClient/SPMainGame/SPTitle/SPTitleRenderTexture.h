// ***************************************************************
//  SPTitleRenderTexture   version:  1.0   ¡¤  date: 03/30/2006
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

class SPTitleRenderBase;

class SPTitleRenderTexture : public SPTitleRenderBase
{
public:

	SPTitleRenderTexture			();
	virtual	~SPTitleRenderTexture	();

	virtual	void	Init			();
	virtual	void	Clean			();
	virtual	void	Process			( float fTime );
	virtual	void	Render			( float fTime );

	BOOL			SetTexture		( const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );
	void			SetTextureMargin( int iLeft , int iTop , int iRight , int iBottom );

protected:

	SPTexture*						m_pTexture;
	RECT							m_rcTextureSrc;
	RECT							m_rcTexturePos;
	RECT							m_rcTextureMargin;

};
