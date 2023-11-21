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

#include "SPCommon.h"

#include "SPComboBoxDEF.h"
#include "SPComboBoxItemBase.h"

#include "SPTitleRenderDEF.h"
#include "SPTitleRenderBase.h"

#include "SPComboBoxItemTitle.h"

//////////////////////////////////////////////////////////////////////////

SPComboBoxItemTitle::SPComboBoxItemTitle()
{
	Init();
}

SPComboBoxItemTitle::~SPComboBoxItemTitle()
{
	Clean();
}

void SPComboBoxItemTitle::Init()
{
	m_pTitleBase	=	NULL;
}

void SPComboBoxItemTitle::Clean()
{
	SAFE_DELETE( m_pTitleBase );
}

void SPComboBoxItemTitle::Process( float fTime )
{
	if( IsProcess() == FALSE )
		return;

	SPComboBoxItemBase::Process( fTime );

	if( m_pTitleBase != NULL )
	{
		m_pTitleBase->SetPos( m_pstRenderInfo->m_rcRenderPos.left , m_pstRenderInfo->m_rcRenderPos.top );
		m_pTitleBase->Process( fTime );
	}
}

void SPComboBoxItemTitle::Render( float fTime )
{
	//if( IsRender() == FALSE )
	//	return;

	if( m_pTitleBase != NULL )
	{
		m_pTitleBase->Render( fTime );
	}
}

void SPComboBoxItemTitle::SetTitleBase( SPTitleRenderBase* pTitleBase )
{
	m_pTitleBase	=	pTitleBase;

	if( m_pTitleBase != NULL )
	{
		SetSize( m_pTitleBase->GetSize().x , m_pTitleBase->GetSize().y );
	}
}

SPTitleRenderBase* SPComboBoxItemTitle::GetTitleBase()
{
	return m_pTitleBase;
}
