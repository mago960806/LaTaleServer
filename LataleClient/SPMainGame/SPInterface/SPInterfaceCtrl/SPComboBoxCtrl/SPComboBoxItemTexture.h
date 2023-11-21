// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-11-9    12:01 
//***************************************************************************

#pragma once

class SPComboBoxItemBase;

class SPComboBoxItemTexture : public SPComboBoxItemBase
{
public:

	SPComboBoxItemTexture			();
	virtual	~SPComboBoxItemTexture	();

	virtual	void	Init			();
	virtual	void	Clean			();
	virtual	void	Process			( float fTime );
	virtual	void	Render			( float fTime );

	BOOL			SetTexture		( COMBOBOX_DATA_TEXTURE_LABEL eLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );
	void			SetTextureMargin( COMBOBOX_DATA_TEXTURE_LABEL eLabel , int iLeft , int iTop , int iRight , int iBottom );

protected:

	SPTexture*						m_pTexture			[ COMBOBOX_DATA_TEXTURE_COUNT ];
	RECT							m_rcTextureSrc		[ COMBOBOX_DATA_TEXTURE_COUNT ];
	RECT							m_rcTexturePos		[ COMBOBOX_DATA_TEXTURE_COUNT ];
	RECT							m_rcTextureMargin	[ COMBOBOX_DATA_TEXTURE_COUNT ];

};
