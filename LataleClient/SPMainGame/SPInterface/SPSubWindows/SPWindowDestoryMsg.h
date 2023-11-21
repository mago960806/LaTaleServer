

#ifndef __SPWINDOW_DESTORYMSG_H__
#define __SPWINDOW_DESTORYMSG_H__

#define		WIID_DESTORYMSG_FRAME			(WIID_DESTORYMSG + 11)
#define		WIID_DESTORYMSG_TITLE			(WIID_DESTORYMSG + 12)
#define		WIID_DESTORYMSG_TEXT			(WIID_DESTORYMSG + 13)
#define		WIID_DESTORYMSG_OK  			(WIID_DESTORYMSG + 14)
#define		WIID_DESTORYMSG_CANCEL			(WIID_DESTORYMSG + 15)


class SPWindowDestoryMsg : public SPWindow
{
public:
	SPWindowDestoryMsg(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowDestoryMsg(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);

	virtual ~SPWindowDestoryMsg();

	virtual void Init();
	virtual void Clean();
	virtual void Process(float fTime);
	virtual void Render(float fTime);

	void  DestoryMsgBoard(INSTANCE_ID instanceID , CLASS_ID ClassID, int iType);
	
protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnOK(WPARAM, LPARAM);
	virtual int OnCancel(WPARAM, LPARAM);

	virtual int OnDestoryRes( WPARAM, LPARAM );
	
protected:
	int						m_iType;
	GU_ID					m_instanceID;
	CONTAINER_SLOT		    m_itemSLOT;

};

#endif