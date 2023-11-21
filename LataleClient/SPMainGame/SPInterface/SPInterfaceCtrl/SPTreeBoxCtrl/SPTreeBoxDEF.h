// ***************************************************************
//  SPTreeBoxDEF.h   version:  1.0   date:2006/01/20
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

enum TREEBOX_DATA_TEXTURE_LABEL
{
	DATA_TEXTURE_DISABLE				=	0						,
	DATA_TEXTURE_ENABLE												,
	DATA_TEXTURE_HIT												,
	DATA_TEXTURE_PUSH												,
	DATA_TEXTURE_SELECT												,
	DATA_TEXTURE_END												,
	DATA_TEXTURE_COUNT					=	DATA_TEXTURE_END * 2	,
};

enum TREEBOX_DATA_STATUS_LABEL
{
	DATA_STATUS_DISABLE					=	0x0001					,
	DATA_STATUS_ENABLE					=	0x0002					,
	DATA_STATUS_HIT						=	0x0004					,
	DATA_STATUS_PUSH					=	0x0008					,
	DATA_STATUS_SELECT					=	0x0010					,
	DATA_STATUS_PULLDOWN				=	0x1000					,
	DATA_STATUS_COUNT					=	6						,
};

enum TREEBOX_DATA_STRING_LABEL
{
	DATA_STRING_DISABLE					=	0						,
	DATA_STRING_ENABLE												,
	DATA_STRING_HIT													,
	DATA_STRING_PUSH												,
	DATA_STRING_SELECT												,
	DATA_STRING_END													,
	DATA_STRING_COUNT					=	DATA_STRING_END * 2		,
};

enum TREEBOX_TEXTURE
{
	TREEBOX_TEXTURE_LEFT_TOP		=	0	,
	TREEBOX_TEXTURE_CENTER_TOP				,
	TREEBOX_TEXTURE_RIGHT_TOP				,
	TREEBOX_TEXTURE_LEFT_MIDDLE				,
	TREEBOX_TEXTURE_CENTER_MIDDLE			,
	TREEBOX_TEXTURE_RIGHT_MIDDLE			,
	TREEBOX_TEXTURE_LEFT_BOTTOM				,
	TREEBOX_TEXTURE_CENTER_BOTTOM			,
	TREEBOX_TEXTURE_RIGHT_BOTTOM			,
	TREEBOX_TEXTURE_COUNT					,
};

class SPTreeBoxItemBase;

struct TREEBOX_ITEM
{
	TREEBOX_ITEM*									m_pParent;
	bool											m_bSelect;
	bool											m_bShow;
	SPTreeBoxItemBase*								m_pItemBase;

	std::vector< TREEBOX_ITEM* >					m_vpChild;
};
