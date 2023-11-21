/*
* Copyright (c) 2008,上海盛趣信息技术（SDG事业部）
* All rights reserved.
* 
* 文件名称：SPWindowMesageBox.h
* 摘    要：对话框管理声明
* 
* 作    者：夏林
* 完成日期：2008年07月25日
*/

#ifndef __SPWINDOW_MESSAGEBOX_H__
#define __SPWINDOW_MESSAGEBOX_H__

#define		WIID_MESSAGEBOX_FRAME			(WIID_MESSAGEBOX + 500)
#define		WIID_MESSAGEBOX_GRN_BG			(WIID_MESSAGEBOX + 501)
#define		WIID_MESSAGEBOX_TITLE			(WIID_MESSAGEBOX + 502)
#define		WIID_MESSAGEBOX_TEXT			(WIID_MESSAGEBOX + 503)
#define		WIID_MESSAGEBOX_OK  			(WIID_MESSAGEBOX + 504)
#define		WIID_MESSAGEBOX_CANCEL			(WIID_MESSAGEBOX + 505)


class SPWindowMessageBox : public SPWindow
{
public:
	SPWindowMessageBox(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowMessageBox(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);

	virtual ~SPWindowMessageBox();

	virtual void Init();
	virtual void Clean();
	virtual void Process(float fTime);
	virtual void Render(float fTime);
	
	// 弹出对话框
	INSTANCE_ID		MessageBox(const char* pszMsgText, int iType = MES_OK, 
								INSTANCE_ID tMsgWnd = -1, SPIM eMsg = SPIM_NULL);

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnOK(WPARAM, LPARAM);
	virtual int OnCancel(WPARAM, LPARAM);
	
protected:
	int          m_iType;				// 对话框选项
	INSTANCE_ID  m_WndID;				// 回调窗口
	SPIM         m_eMsg;                // 回调消息

};

#endif