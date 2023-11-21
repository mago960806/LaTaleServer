// ***************************************************************
//  SPTreeBoxItemTexture.h   version:  1.0   date:2006/01/20
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPTreeBoxItemBase;

class SPTreeBoxItemTexture : public SPTreeBoxItemBase
{
public:
	SPTreeBoxItemTexture							();
	virtual	~SPTreeBoxItemTexture					();

	virtual	void					Init			();
	virtual	void					Clear			();

	virtual	void					Process			( float fTime );
	virtual	void					Render			( float fTime );

	virtual	int						GetNextPosX		();
	virtual	int						GetNextPosY		();

	virtual	bool					IsCursorIn		( int iX, int iY );

	virtual	void					SetPos			( int iX , int iY );
	virtual	RECT					GetPosRect		();

	virtual	void					SetStatus		( TREEBOX_DATA_STATUS_LABEL eStatus );
	virtual	void					AddStatus		( DWORD dwStatus );
	virtual	void					DelStatus		( DWORD dwStatus );

	TREEBOX_DATA_TEXTURE_LABEL		GetTextureLabel	();

	BOOL							SetTexture		( TREEBOX_DATA_TEXTURE_LABEL eLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );
	BOOL							SetTexture		( int iLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );
	void							SetTextureMargin( TREEBOX_DATA_TEXTURE_LABEL eLabel , int iLeft , int iTop , int iRight , int iBottom );
	void							SetTextureMargin( int iLabel , int iLeft , int iTop , int iRight , int iBottom );

protected:

	TREEBOX_DATA_TEXTURE_LABEL		m_eTextureLabel;
	SPTexture*						m_pTexture			[ DATA_TEXTURE_COUNT ];
	RECT							m_rcTextureSrc		[ DATA_TEXTURE_COUNT ];
	RECT							m_rcTexturePos		[ DATA_TEXTURE_COUNT ];
	RECT							m_rcTextureMargin	[ DATA_TEXTURE_COUNT ];
};
