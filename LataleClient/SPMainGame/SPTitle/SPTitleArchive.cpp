// ***************************************************************
//  SPTitleArchive   version:  1.0   ·  date: 03/30/2006
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
#include <algorithm>

#include "SPTitleAttr.h"
#include "SPTitleArchive.h"

/**
* 주어진 ID의 TitleAttr 검사 <br>
*/
struct _IsEqualTitle : binary_function<SPTitleData, int, bool> {
	bool operator() (SPTitleData pTitleAttr, int iTitleID) const
	{
		if(pTitleAttr.m_uiTitleID == iTitleID)
			return true;
		return false;
	}
} IsEqualTitle;


SPTitleArchive::SPTitleArchive(void)
{
	Init();
}

SPTitleArchive::~SPTitleArchive(void)
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
//
//

void SPTitleArchive::Init()
{
	m_vTitleData.clear();
}

void SPTitleArchive::Destroy()
{
	DelAllTitleData();
}

void SPTitleArchive::AddTitleData( SPTitleData& stTitleData )
{
	m_vTitleData.push_back( stTitleData );
}

SPTitleData* SPTitleArchive::GetTitleData( int iNumber )
{
	if( iNumber < 0 || iNumber >= (int)m_vTitleData.size() )
		return NULL;

	return	&( m_vTitleData.at( iNumber ) );
}

void SPTitleArchive::DelAllTitleData()
{
	m_vTitleData.clear();
}

int SPTitleArchive::GetCount()
{
	return (int)m_vTitleData.size();
}


bool SPTitleArchive::FindTitle(int iTitleID)
{
	std::vector<SPTitleData>::iterator iter;
	iter = std::find_if(m_vTitleData.begin(), m_vTitleData.end(),
		std::bind2nd(IsEqualTitle, iTitleID));	

	if(iter == m_vTitleData.end())
		return false;
	else 
		return true;

	return false;
}
