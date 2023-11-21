/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/04/11
	created:	2006:4:11:   12:12
	filename: 	SPWindowWorldMapShortcut.h
	
	purpose:	
*********************************************************************/

#pragma once

class SPWindow;

class SPWindowWorldMapShortcut : public SPWindow
{

public:
	SPWindowWorldMapShortcut(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowWorldMapShortcut(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowWorldMapShortcut();

	virtual void Init();

	void	UpdatePos();

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int					OnWorldMap			( WPARAM wParam, LPARAM lParam );
	int					OnStageMap			( WPARAM wParam, LPARAM lParam );

	// }}

	//////////////////////////////////////////////////////////////////////////

	enum SHORTCUT_INSTANCE_ID
	{
		WIID_WORLDMAP_SHORTCUT_STAGE					=	WIID_WORLDMAP_SHORTCUT	+	1			,
		WIID_WORLDMAP_SHORTCUT_MAP																	,
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

};
