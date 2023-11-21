// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowListDEF.h
//	created:	2007/10/31   Time:17:49
//	purpose:	for SPWindowList, SPWindowRSList, SPWindowRSStatic
//*********************************************************************/


#pragma once

//////////////////////////////////////////////////////////////////////////
// SubSpim List
#define SUB_SPIM_SCROLL_UP			0
#define SUB_SPIM_SCROLL_DOWN		1
#define SUB_SPIM_SCROLL_LATEST		2
#define SUB_SPIM_SCROLL_FIRST		3
#define SUB_SPIM_SCROLL_SET_LINE	4

//////////////////////////////////////////////////////////////////////////


struct LIST_RETURNVALUE
{
	std::string					m_strReturnValue;		//	돌려줄 문자열
	int							m_iReturnValue;			//	돌려줄 숫자

	void	Clear()
	{
		m_strReturnValue.clear();
		m_iReturnValue	=	-1;
	}

	LIST_RETURNVALUE()
	{
		Clear();
	}
	LIST_RETURNVALUE& operator = (const LIST_RETURNVALUE& Src)
	{
		m_strReturnValue	= Src.m_strReturnValue ;
		m_iReturnValue		= Src.m_iReturnValue;
		return *this;
	}
};