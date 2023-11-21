// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-6-29    13:20 
//***************************************************************************

#include "SPCommon.h"

#include "SPUIUnit.h"

#include "SPUIUnitManager.h"

SPUIUnitManager::SPUIUnitManager()
{
	ClearAll( TRUE );
}

SPUIUnitManager::~SPUIUnitManager()
{
	ClearAll( TRUE );
}

void SPUIUnitManager::AddUnit( SPUIUnit* pUIUnit )
{
	AddRect( pUIUnit->GetPos() , pUIUnit->GetSize() );
	m_vBuffer.push_back( pUIUnit );
}

SPUIUnit* SPUIUnitManager::GetUnit( int iNumber )
{
	if( iNumber >= GetUnitCount() || iNumber < 0 )
		return NULL;

	return m_vBuffer.at( iNumber );
}

BOOL SPUIUnitManager::SetUnit( SPUIUnit* pUIUnit , BOOL bDel )
{
	STD_VECTOR_UIUNIT::iterator		Iter	=	m_vBuffer.begin();

	while( Iter != m_vBuffer.end() )
	{
		if( pUIUnit == (*Iter) )
		{
			Iter	=	m_vBuffer.insert( Iter , pUIUnit );
			++Iter;

			if( Iter != m_vBuffer.end() )
			{
				if( bDel == TRUE )
				{
					delete (*Iter);
				}

				m_vBuffer.erase( Iter );
			}

			ReCalRect();
			return TRUE;
		}

		++Iter;
	}

	return FALSE;
}

BOOL SPUIUnitManager::SetUnit( int iNumber , SPUIUnit* pUIUnit , BOOL bDel )
{
	STD_VECTOR_UIUNIT::iterator		Iter	=	m_vBuffer.begin() + iNumber;

	if( Iter == m_vBuffer.end() )
		return FALSE;

	Iter	=	m_vBuffer.insert( Iter , pUIUnit );
	++Iter;

	if( Iter != m_vBuffer.end() )
	{
		if( bDel == TRUE )
		{
			delete (*Iter);
		}

		m_vBuffer.erase( Iter );
	}

	ReCalRect();
	return TRUE;
}

void SPUIUnitManager::SetBufferSize( int iSize )
{
	m_vBuffer.reserve( iSize );
}

int SPUIUnitManager::GetBufferSize()
{
	return (int)( m_vBuffer.capacity() );
}

int SPUIUnitManager::GetUnitCount()
{
	return (int)( m_vBuffer.size() );
}

void SPUIUnitManager::Process( float fTime )
{
	STD_VECTOR_UIUNIT::iterator		Iter	=	m_vBuffer.begin();
	SPUIUnit*						pUIUnit;

	while( Iter != m_vBuffer.end() )
	{
		pUIUnit	=	(SPUIUnit*)(*Iter);

		if( pUIUnit != NULL )
		{
			pUIUnit->Process( fTime );
		}

		++Iter;
	}
}

void SPUIUnitManager::Render()
{
	g_pVideo->Flush();

	STD_VECTOR_UIUNIT::iterator		Iter	=	m_vBuffer.begin();
	SPUIUnit*						pUIUnit;

	while( Iter != m_vBuffer.end() )
	{
		pUIUnit	=	(SPUIUnit*)(*Iter);

		if( pUIUnit != NULL )
		{
			pUIUnit->Render();
		}

		++Iter;
	}

	g_pVideo->Flush();
}

void SPUIUnitManager::ClearAll( BOOL bDel )
{
	if( bDel == TRUE )
	{
		STD_VECTOR_UIUNIT::iterator		Iter	=	m_vBuffer.begin();

		while( Iter != m_vBuffer.end() )
		{
			delete (*Iter);
			++Iter;
		}
	}

	m_vBuffer.clear();

	m_rtRect.left	=	LONG_MAX;
	m_rtRect.top	=	LONG_MAX;
	m_rtRect.right	=	LONG_MIN;
	m_rtRect.bottom	=	LONG_MIN;
}

SPUIUnit* SPUIUnitManager::GetUnitMouseOver( int iX , int iY )
{
	STD_VECTOR_UIUNIT::iterator		Iter	=	m_vBuffer.begin();
	SPUIUnit*						pUIUnit;

	while( Iter != m_vBuffer.end() )
	{
		pUIUnit	=	(SPUIUnit*)(*Iter);

		if( pUIUnit != NULL )
		{
			if( pUIUnit->MouseOver( iX , iY ) == TRUE )
			{
				return pUIUnit;
			}
		}

		++Iter;
	}

	return NULL;
}

int SPUIUnitManager::GetNumberMouseOver( int iX , int iY )
{
	if( m_rtRect.left > iX || m_rtRect.right < iX || m_rtRect.top > iY || m_rtRect.bottom < iY )
		return -2;

	int			iUnitCount	=	GetUnitCount();
	SPUIUnit*	pUIUnit;

	for ( int i = 0 ; i < iUnitCount ; ++i )
	{
		pUIUnit	=	GetUnit( i );

		if( pUIUnit == NULL)
			continue;

		if( pUIUnit->MouseOver( iX , iY ) == TRUE )
		{
			return i;
		}
	}

	return -1;
}

void SPUIUnitManager::AddRect( POINT& ptPos , POINT& ptSize )
{
	if( m_rtRect.left > ptPos.x )
		m_rtRect.left	=	ptPos.x;

	if( m_rtRect.top > ptPos.y )
		m_rtRect.top	=	ptPos.y;

	ptPos.x	+=	ptSize.x;
	ptPos.y	+=	ptSize.y;

	if( m_rtRect.right < ptPos.x )
		m_rtRect.right	=	ptPos.x;

	if( m_rtRect.bottom < ptPos.y )
		m_rtRect.bottom	=	ptPos.y;
}

void SPUIUnitManager::ReCalRect()
{
	m_rtRect.left	=	LONG_MAX;
	m_rtRect.top	=	LONG_MAX;
	m_rtRect.right	=	LONG_MIN;
	m_rtRect.bottom	=	LONG_MIN;

	STD_VECTOR_UIUNIT::iterator		Iter	=	m_vBuffer.begin();
	SPUIUnit*						pUIUnit;

	while( Iter != m_vBuffer.end() )
	{
		pUIUnit	=	(SPUIUnit*)(*Iter);

		if( pUIUnit != NULL )
		{
			AddRect( pUIUnit->GetPos() , pUIUnit->GetSize() );
		}
		++Iter;
	}
}

RECT SPUIUnitManager::GetRect()
{
	return m_rtRect;
}
