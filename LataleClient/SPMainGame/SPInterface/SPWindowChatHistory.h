// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

class SPWindow;
class SPWindowChatHistory : public SPWindow {

public:
	SPWindowChatHistory(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowChatHistory(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowChatHistory();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

public:


protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int OnCursorEnter	( WPARAM, LPARAM );
	int OnCursorOut		( WPARAM, LPARAM );
	int OnCursorLDown	( WPARAM, LPARAM );
	int OnCursorLUp		( WPARAM, LPARAM );
	// }}

protected:
	

};




