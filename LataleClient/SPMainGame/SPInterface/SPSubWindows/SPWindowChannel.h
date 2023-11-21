#ifndef __SPWINDOW_CHANNEL_H__
#define __SPWINDOW_CHANNEL_H__

#define WIID_CHANNEL_SKIN_UPPER				(WIID_CHANNEL_CHANGE + 1)
#define WIID_CHANNEL_SKIN_MIDDLE			(WIID_CHANNEL_CHANGE + 2)
#define WIID_CHANNEL_SKIN_LOWER				(WIID_CHANNEL_CHANGE + 3)
#define WIID_CHANNEL_TITLE_TEXT				(WIID_CHANNEL_CHANGE + 4)
#define WIID_CHANNEL_CLOSE					(WIID_CHANNEL_CHANGE + 5)
#define WIID_CHANNEL_SEPERATE_LINE1			(WIID_CHANNEL_CHANGE + 6)
#define WIID_CHANNEL_SEPERATE_LINE2			(WIID_CHANNEL_CHANGE + 7)
#define WIID_CHANNEL_SUB_BG					(WIID_CHANNEL_CHANGE + 8)
#define WIID_CHANNEL_OK						(WIID_CHANNEL_CHANGE + 9)

#define WIID_CHANNEL_LIST					(WIID_CHANNEL_CHANGE + 10)		// +3

class SPWindow;
class SPWindowChannel : public SPWindow
{
public:
	SPWindowChannel(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowChannel(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowChannel();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnCursorEnter(WPARAM, LPARAM);
	virtual int OnCursorOut(WPARAM, LPARAM);
	virtual int OnSetChannel(WPARAM wParam, LPARAM lParam);
	virtual int OnExit(WPARAM wParam, LPARAM lParam);
	virtual int OnOK(WPARAM wParam, LPARAM lParam);
	virtual int OnListBoxLButton(WPARAM wParam, LPARAM lParam);
	virtual int OnListBoxDBLButton(WPARAM wParam, LPARAM lParam);

	void InitListBox(SPWindow* pkParent);
	void ClearChannelList();

protected:

	int m_iCurSelIndex;
	RECT m_rcSrcStatus[4];
	SERVER_ID m_iCurServerID;
	std::vector< CHANNEL_INFO > m_vChannelList;
};

#endif