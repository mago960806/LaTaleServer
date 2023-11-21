/*
* Copyright (c) 2008,上海盛趣信息技术（SDG事业部）
* All rights reserved.
* 
* 文件名称：SPWindowBeExact.h
* 摘    要：被索取框体声明
* 
* 作    者：夏林
* 完成日期：2008年07月30日
*/

#include "SPItemAttr.h"

#ifndef __SPWINDOW_BEEXACT_H__
#define __SPWINDOW_BEEXACT_H__

#define WIID_BEEXACT_EXACT					(WIID_BEEXACT + 1)
#define WIID_BEEXACT_EXACT_FRAME			(WIID_BEEXACT + 2)
#define WIID_BEEXACT_EXACT_CHECK			(WIID_BEEXACT + 20) //+ 20
#define WIID_BEEXACT_EXACT_NAME				(WIID_BEEXACT + 40) //+ 20
#define WIID_BEEXACT_EXACT_PRICE			(WIID_BEEXACT + 60) //+ 20
#define WIID_BEEXACT_EXACT_OZ_ICON			(WIID_BEEXACT + 80) //+ 20
#define WIID_BEEXACT_EXACT_PRICE_TEXT		(WIID_BEEXACT + 100)
#define WIID_BEEXACT_EXACT_PRICE_SUM		(WIID_BEEXACT + 101)
#define WIID_BEEXACT_EXACT_OZICON_SUM		(WIID_BEEXACT + 102)
#define WIID_BEEXACT_EXACT_MESSAGE_GRN_BG	(WIID_BEEXACT + 103)
#define WIID_BEEXACT_EXACT_RECEIVE_IMG		(WIID_BEEXACT + 104)
#define WIID_BEEXACT_EXACT_MESSAGE_IMG		(WIID_BEEXACT + 105)
#define WIID_BEEXACT_EXACT_RECEIVE_BG		(WIID_BEEXACT + 106)
#define WIID_BEEXACT_EXACT_MESSAGE_BG		(WIID_BEEXACT + 107)
#define WIID_BEEXACT_EXACT_MESSAGE			(WIID_BEEXACT + 108)
#define WIID_BEEXACT_EXACT_RECEIVE			(WIID_BEEXACT + 109)
#define WIID_BEEXACT_EXACT_TEXT             (WIID_BEEXACT + 110)
#define WIID_BEEXACT_EXACT_OK				(WIID_BEEXACT + 111)
#define WIID_BEEXACT_EXACT_CANCEL			(WIID_BEEXACT + 112)

#define WIID_BEEXACT_RESULT					(WIID_BEEXACT + 200)
#define WIID_BEEXACT_RESULT_ITEM_SLOT		(WIID_BEEXACT + 201)
#define WIID_BEEXACT_RESULT_ITEM			(WIID_BEEXACT + 202)
#define WIID_BEEXACT_RESULT_ITEM_NAME		(WIID_BEEXACT + 203)
#define WIID_BEEXACT_RESULT_PRICE_BG		(WIID_BEEXACT + 204)
#define WIID_BEEXACT_RESULT_OZ_ICON			(WIID_BEEXACT + 205)
#define WIID_BEEXACT_RESULT_PRICE			(WIID_BEEXACT + 206)
#define WIID_BEEXACT_RESULT_BUY_OZ_TEXT		(WIID_BEEXACT + 207)
#define WIID_BEEXACT_RESULT_MY_OZ_TEXT		(WIID_BEEXACT + 208)
#define WIID_BEEXACT_RESULT_BUY_OZ_ICON		(WIID_BEEXACT + 209)
#define WIID_BEEXACT_RESULT_MY_OZ_ICON		(WIID_BEEXACT + 210)
#define WIID_BEEXACT_RESULT_BUY_OZ			(WIID_BEEXACT + 211)
#define WIID_BEEXACT_RESULT_MY_OZ			(WIID_BEEXACT + 212)
#define WIID_BEEXACT_RESULT_OK				(WIID_BEEXACT + 213)
#define WIID_BEEXACT_RESULT_MSG				(WIID_BEEXACT + 214)

#define WIID_BEEXACT_BUYWAIT				(WIID_BEEXACT + 215)
#define WIID_BEEXACT_BUYWAIT_ITEM			(WIID_BEEXACT + 216)
#define WIID_BEEXACT_BUYWAIT_PLAYER			(WIID_BEEXACT + 217)
#define WIID_BEEXACT_BUYWAIT_TEXT			(WIID_BEEXACT + 218)
#define WIID_BEEXACT_BUYWAIT_ANI			(WIID_BEEXACT + 219)	// +14



struct EVENT_ANIMATION
{
	float fAccmulateTime;
	float fDelayTime;
	int iTotalFrame;
	int iCurFrame;
	int iIncDir;

	RECT rcDst;
	RECT rcSrc[2];
};


class SPWindowBeExact : public SPWindow
{
public:
	SPWindowBeExact(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowBeExact(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);

	virtual ~SPWindowBeExact();

	virtual void Init();
	virtual void Clean();
	virtual void Process(float fTime);
	virtual void Render(float fTime);

	bool   IsGifting() { return m_bGift; }
	

	void InitResult();
	void InitBuyWait();


	void RefreshFrame();
	void UpdateFrame();

	virtual int OnBeExact(WPARAM wParam, LPARAM lParam);
	virtual int OnGift(WPARAM wParam, LPARAM lParam);

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnOK(WPARAM, LPARAM);
	virtual int OnCancel(WPARAM, LPARAM);
	virtual int OnResultOK(WPARAM wParam, LPARAM lParam);

	virtual int OnCheck(unsigned int iID, WPARAM wParam, LPARAM lParam);

	int					OnEditSetFocus		( WPARAM wParam, LPARAM lParam );
	int					OnEditKillFocus		( WPARAM wParam, LPARAM lParam );

	void ProcessBuyWaitAni(float fTime);
	void ShowResultDlg(int iItemID, const char* pszTargetName = NULL);
	void ShowBuyWaitDlg();
	void HideBuyWaitDlg();
	void InitBuyWaitAni(SPWindow* pkParentWindow);


	bool Gift();
	
protected:
	std::vector<int>			m_vecPackageID;
	std::string					m_strRequesterName;	    // 被请求玩家名字
	std::string					m_strMsg;				// 消息
	int                         m_iCahsierPrice;
	bool                        m_bGift;

	std::vector<SPCashItemAttr*>  m_vCashierItemAttr;   //[xialin 2008/08/01]edit

	UINT						m_uiCurOZ;
	UINT						m_uiCurMileage;

	std::string                m_strGiftFriendName;		//[xialin 2008/08/05]add
	std::string                m_strGiftFriendMsg;		//[xialin 2008/08/05]add

	bool m_bBuyRecv;
	bool m_bOnceAni;
	UINT m_uiBuyItem;
	string m_strTargetName;
	float m_fBuyWaitAccmulateTime;
	EVENT_ANIMATION m_BuyAni[14];

};

#endif