
// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment : 
// Creation : metalgeni 2004-08-23 ¿ÀÈÄ 4:48:46
//***************************************************************************
#pragma once

#ifndef INITCOLOR
#define INITCOLOR 0xffffffff
#endif

struct D3DXCOLOR;

class SPLine {
public:
	SPLine();
	~SPLine();

	HRESULT Init(D3DXCOLOR D3DColor = INITCOLOR);	//RGBA
	HRESULT OnLostDevice();
	HRESULT OnResetDevice();

	void	SetColor(const D3DXCOLOR Color = INITCOLOR);
	HRESULT	SetWidth (float fWidth)		{ return m_pD3DXLine->SetWidth(fWidth);	}
	FLOAT	GetWidth ()					{ return m_pD3DXLine->GetWidth();		}

	HRESULT	Begin()						{ return m_pD3DXLine->Begin();	} // Call Render Block Func is Optional
	HRESULT End()						{ return m_pD3DXLine->End();	}

	HRESULT DrawLine( RECT rect );
	HRESULT DrawRect( RECT rect );

protected:
	ID3DXLine*				m_pD3DXLine;
	D3DXCOLOR				m_D3DColor;
	bool					m_bShadow;	
	
};



