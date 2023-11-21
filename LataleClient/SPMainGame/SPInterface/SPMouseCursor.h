// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************
#pragma once 



#include <WTypes.h>


#define MAX_MOUSE_INDEX 16



class SPTexture;
class SPMouseCursor {
public:

	SPMouseCursor();
	virtual ~SPMouseCursor();

	virtual void	Process(float);
	virtual	void	Render(float);
	virtual void	Init();

	void			SetCurPos(float, float);		
	void			SetMoveIconPos(SPTexture*, int, int );
	void			ClearMoveIcon();

	enum MOUSE_TYPE{
		MOUSE_TYPE_DEFAULT			= 	0x0000,
		MOUSE_TYPE_WAIT				= 	0x0002,
		MOUSE_TYPE_HRESIZE			= 	0x0003,
		MOUSE_TYPE_VRESIZE			= 	0x0004,
		MOUSE_TYPE_NPC_TALK			= 	0x0005,				//[2005/5/6] - jinhee //NPC위에 MouseOn 된 경우
		MOUSE_TYPE_PC_TALK			=	0x0006,				//[2005/7/13] - jinhee //PC위에 MouseOn 된 경우
		MOUSE_TYPE_CHAT_BOARD		=	0x0007,				//  [4/11/2008 AJJIYA]

		MOUSE_TYPE_WORLDMAP_SCROLL			=	0x01000,			// AJJIYA [4/10/2006]
		MOUSE_TYPE_WORLDMAP_LEFT			=	0x01001,			// AJJIYA [4/10/2006]
		MOUSE_TYPE_WORLDMAP_TOP				=	0x01002,			// AJJIYA [4/10/2006]
		MOUSE_TYPE_WORLDMAP_RIGHT			=	0x01003,			// AJJIYA [4/10/2006]
		MOUSE_TYPE_WORLDMAP_BOTTOM			=	0x01004,			// AJJIYA [4/10/2006]
		MOUSE_TYPE_WORLDMAP_LEFT_TOP		=	0x01005,			// AJJIYA [4/10/2006]
		MOUSE_TYPE_WORLDMAP_LEFT_BOTTOM		=	0x01006,			// AJJIYA [4/10/2006]
		MOUSE_TYPE_WORLDMAP_RIGHT_TOP		=	0x01007,			// AJJIYA [4/10/2006]
		MOUSE_TYPE_WORLDMAP_RIGHT_BOTTOM	=	0x01008,			// AJJIYA [4/10/2006]
		MOUSE_TYPE_WORLDMAP_DRAG			=	0x01010,			// AJJIYA [4/10/2006]

		MOUSE_TYPE_WORLDMAP_ETC				=	0x02000,			// AJJIYA [4/10/2006]
		MOUSE_TYPE_WORLDMAP_DETAIL			=	0x02001,			// AJJIYA [4/10/2006]
	};

	void			SetType(MOUSE_TYPE type = MOUSE_TYPE_DEFAULT);
	bool			IsSetType();
	void			SetAutoHide(bool bEnable);
	bool			IsShowCursor();							// Auto Hide 될때 툴팁쪽 Hide 시키기 위해 AJJIYA  [11/25/2005]

	MOUSE_TYPE		GetMouseType();
	void			ResetWaitData();

	SIZE			GetMouseSize();

protected:

	void			ProcessWait( float fTime );

	SPTexture*		m_pImage;
	SPTexture*		m_pMoveIconImage;	

	RECT			m_CurSrcRect;

	MOUSE_TYPE		m_MouseType;
	int				m_iCurInx;

	float			m_fX;
	float			m_fY;

	float			m_fPosOffsetX , m_fPosOffsetY;

	int				m_nSrcX, m_nSrcY;

	float			m_fAccumulateTime;
	const float		m_fAutoHideDelayTime;
	bool			m_bAutoHide;
	float			m_fHideAlpha;

	float			m_fAccumulateTimeWait;
	float			m_fAccumulateTimeLimitWait;
	int				m_iCurFrameWait;
	int				m_iMaxFrameWait;
	
};

extern SPMouseCursor* g_pMouseCursor;

