// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-6-29    13:19 
//***************************************************************************

#pragma once

#include <vector>

class SPUIUnit;

class SPUIUnitManager
{
public:
	SPUIUnitManager								(void);
	virtual ~SPUIUnitManager					(void);

	void					AddUnit				( SPUIUnit* pUIUnit );
	SPUIUnit*				GetUnit				( int iNumber );

	BOOL					SetUnit				( SPUIUnit* pUIUnit , BOOL bDel );
	BOOL					SetUnit				( int iNumber , SPUIUnit* pUIUnit , BOOL bDel );

	void					SetBufferSize		( int iSize );
	int						GetBufferSize		();

	int						GetUnitCount		();

	void					Process				( float fTime );
	void					Render				();

	void					ClearAll			( BOOL bDel );

	SPUIUnit*				GetUnitMouseOver	( int iX , int iY );
	int						GetNumberMouseOver	( int iX , int iY );

	void					AddRect				( POINT& ptPos , POINT& ptSize );
	void					ReCalRect			();
	RECT					GetRect				();

protected:

	typedef	std::vector<SPUIUnit*>				STD_VECTOR_UIUNIT;

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	STD_VECTOR_UIUNIT							m_vBuffer;
	RECT										m_rtRect;

};
