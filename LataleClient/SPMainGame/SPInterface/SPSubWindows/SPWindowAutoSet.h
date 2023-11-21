#pragma once

#define WIID_AUTO_FRAME			(WIID_AUTOSET + 1)
#define WIID_AUTO_PIC			(WIID_AUTOSET + 2)
#define WIID_AUTO_EXIT			(WIID_AUTOSET + 3)


class SPWindow;
class SPWindowAutoSet : public SPWindow
{
public:
	SPWindowAutoSet(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowAutoSet(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowAutoSet();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int			OnExitClick		( WPARAM wParam, LPARAM lParam );

};
