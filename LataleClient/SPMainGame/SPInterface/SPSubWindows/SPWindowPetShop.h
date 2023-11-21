#pragma once

#define WIID_PETSHOP_TOP				(WIID_PETSHOP	+	1)		//스킨 TOP
#define WIID_PETSHOP_CENTER				(WIID_PETSHOP	+	2)		//스킨 CENTER
#define WIID_PETSHOP_BOTTOM				(WIID_PETSHOP	+	3)		//스킨 BOTTOM

#define WIID_PETSHOP_TITLE				(WIID_PETSHOP	+	11)
#define WIID_PETSHOP_CLOSE				(WIID_PETSHOP	+	12)

#define WIID_PETSHOP_BTN_SELL				(WIID_PETSHOP	+	20)		//아이템 판매 영역
#define WIID_PETSHOP_SELLSKIN			(WIID_PETSHOP	+	30)
#define WIID_PETSHOP_SELLHELP			(WIID_PETSHOP	+	31)

#define WIID_PETSHOP_ELY_TEXT_IMG		(WIID_PETSHOP	+	230)		//페이지 감소
#define WIID_PETSHOP_ELY				(WIID_PETSHOP	+	235)		//페이지 감소
#define WIID_PETSHOP_ELY_IMG			(WIID_PETSHOP	+	236)		//페이지 감소


class SPWindowPetShop : public SPWindow 
{
public:	
	SPWindowPetShop(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPetShop();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);	

	void SetSkillID(int iId = 0);
	void SetPetSlot(int iSlot = 0);

protected:
	void InitSubControl();

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnClose 		(WPARAM wParam, LPARAM lParam);	
	int OnItemSell 		(WPARAM wParam, LPARAM lParam);	

private:
	SPWindow*	m_pSell;
	SPWindow*	m_pEly;
	int			m_iSkillID;
	int			m_iPetSlot;
};



