// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

#define WIID_EXIT_SKIN_UPPER				(WIID_EXIT + 1)
#define WIID_EXIT_SKIN_MIDDLE				(WIID_EXIT + 2)
#define WIID_EXIT_SKIN_LOWER				(WIID_EXIT + 3)

#define WIID_EXIT_TITLE_TEXT				(WIID_EXIT + 4)
#define WIID_EXIT_ENG_TITLE_TEXT			(WIID_EXIT + 5)

#define WIID_EXIT_CLOSE						(WIID_EXIT + 6)
#define WIID_EXIT_SERVER_CHANGE				(WIID_EXIT + 7)
#define WIID_EXIT_CHANNEL_CHANGE			(WIID_EXIT + 8)
#define WIID_EXIT_GOTO_LOGIN				(WIID_EXIT + 9)

class SPWindow;
class SPWindowExit : public SPWindow {
public:	
	SPWindowExit(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowExit();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);
	

protected:

	SPIMESSAGE_OBJ_MAP_DECLARE()
	
	virtual int OnKeyReturn				( WPARAM, LPARAM );
	virtual int OnConfirmExit			( WPARAM, LPARAM );
	virtual int OnConfirmServerChange	( WPARAM, LPARAM );
	virtual int OnConfirmChannelChange	( WPARAM, LPARAM );
	virtual int OnEnableButton			( WPARAM, LPARAM );
	virtual int OnChannelEnable			( WPARAM, LPARAM );

protected:
	int OnClearFindMessage();
};
