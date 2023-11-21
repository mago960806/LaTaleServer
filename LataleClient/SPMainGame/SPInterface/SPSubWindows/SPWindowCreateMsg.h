

#ifndef __SPWINDOW_CREATEMSG_H__
#define __SPWINDOW_CREATEMSG_H__

#define		WIID_CREATEMSG_FRAME			(WIID_CREATEMSG + 11)
#define		WIID_CREATEMSG_TITLE			(WIID_CREATEMSG + 12)
#define		WIID_CREATEMSG_TEXT				(WIID_CREATEMSG + 13)
#define		WIID_CREATEMSG_BUTTON_BG		(WIID_CREATEMSG + 14)
#define		WIID_CREATEMSG_CHECKTEXT		(WIID_CREATEMSG + 15)
#define		WIID_CREATEMSG_OK  				(WIID_CREATEMSG + 16)
#define		WIID_CREATEMSG_CANCEL			(WIID_CREATEMSG + 17)


class SPWindowCreateMsg : public SPWindow
{
public:
	SPWindowCreateMsg(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowCreateMsg(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);

	virtual ~SPWindowCreateMsg();

	virtual void Init();
	virtual void Clean();
	virtual void Process(float fTime);
	virtual void Render(float fTime);
	
	bool	  ParsingPosOK();

	

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnCreateMsgBoard( WPARAM, LPARAM );

	virtual int OnOK(WPARAM, LPARAM);
	virtual int OnCancel(WPARAM, LPARAM);

	virtual int OnBeginMove	( WPARAM, LPARAM );
	virtual int OnEndMove	( WPARAM, LPARAM );
	virtual int OnMove		( WPARAM, LPARAM );

	virtual int OnCreateRes	( WPARAM, LPARAM );

	int					OnEditSetFocus		( WPARAM wParam, LPARAM lParam );
	int					OnEditKillFocus		( WPARAM wParam, LPARAM lParam );
	
protected:
	GU_ID					m_itmeGUID;
	CONTAINER_SLOT		    m_itemSLOT;

	int						m_iPosX;
	int						m_iPosY;
	int						m_iType;

};

#endif