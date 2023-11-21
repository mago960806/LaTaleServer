
#pragma once


#define WIID_EQUIP_TOP		(WIID_EQUIP	+1)
#define WIID_EQUIP_CENTER	(WIID_EQUIP	+2)
#define WIID_EQUIP_BOTTOM 	(WIID_EQUIP	+3)

#define WIID_EQUIP_TITLE_IMAGE (WIID_EQUIP	+5)
#define WIID_EQUIP_TITLE	(WIID_EQUIP	+6)

#define WIID_EQUIP_SKIN		(WIID_EQUIP	+10)		//캐릭터및 바탕 아이콘 모양 스킨 (여러개가 될수도 있음...)

#define WIID_EQUIP_GROUP	(WIID_EQUIP	+20)

#define WIID_EQUIP_HIDE		(WIID_EQUIP	+30)
#define WIID_EQUIP_BATTLE	(WIID_EQUIP	+40)
#define WIID_EQUIP_FASHION	(WIID_EQUIP	+50)
#define WIID_EQUIP_PET		(WIID_EQUIP	+60)

#define WIID_EQUIP_GRID		(WIID_EQUIP	+70)
#define WIID_EQUIP_WEAPON1	(WIID_EQUIP	+80)
#define WIID_EQUIP_WEAPON2	(WIID_EQUIP	+90)

#define WIID_EQUIP_MARK1	(WIID_EQUIP	+100)
#define WIID_EQUIP_MARK2	(WIID_EQUIP	+110)

#define WIID_EQUIP_BACK1	(WIID_EQUIP	+120)

#define WIID_EQUIP_EFFECT	(WIID_EQUIP +130)

enum STORAGE_PLAYER;

class SPWindow;
class SPItemCluster;

class SPWindowEquip : public SPWindow
{

public:
	SPWindowEquip(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowEquip(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowEquip();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);
public:


protected:		
	//STORAGE_PLAYER	m_eCurrentPage;
	CONTAINER_TYPE	m_iCurrentPage;

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnCursorEnter	( WPARAM, LPARAM );
	virtual int OnCursorOut		( WPARAM, LPARAM );
	virtual int OnLButtonDown	( WPARAM, LPARAM );
	virtual int OnLButtonUp		( WPARAM, LPARAM );	
	virtual int OnToolTipEnter	( WPARAM, LPARAM );
	virtual int OnToolTipOut	( WPARAM, LPARAM );
	virtual int OnToolTipMove	( WPARAM, LPARAM );

	virtual int OnToolTipEnterWeaponSlot1( WPARAM , LPARAM );
	virtual int OnToolTipEnterWeaponSlot2( WPARAM , LPARAM );

	//virtual int OnGrid			( WPARAM, LPARAM );
	//virtual int OnGrid			( WPARAM, LPARAM );
	//virtual int OnGrid			( WPARAM, LPARAM );
	virtual int OnGridDown			( WPARAM, LPARAM );
	virtual int OnGridUp			( WPARAM, LPARAM );
	virtual int OnCursorEquipMove	( WPARAM, LPARAM );
	virtual int OnGridRUp			( WPARAM, LPARAM );

	virtual int OnWeapon1Down		( WPARAM, LPARAM );
	virtual int OnWeapon1Up			( WPARAM, LPARAM );
	virtual int OnCursorWeapon1Move	( WPARAM, LPARAM );
	virtual int OnWeapon1RUp		( WPARAM, LPARAM );
	
	virtual int OnWeapon2Down		( WPARAM, LPARAM );
	virtual int OnWeapon2Up			( WPARAM, LPARAM );
	virtual int OnCursorWeapon2Move	( WPARAM, LPARAM );
	virtual int OnWeapon2RUp		( WPARAM, LPARAM );

	virtual int OnEquipBattle	( WPARAM, LPARAM );
	virtual int OnEquipFashion	( WPARAM, LPARAM );
	virtual int OnEquipMedal	( WPARAM, LPARAM );
	virtual int OnEquipPet		( WPARAM, LPARAM );
	virtual int OnSetDefault	(WPARAM wParam, LPARAM lParam);			//[2005/11/27] 채널변경시 탭버튼 초기화

	virtual int OnEquipSetting  ( WPARAM, LPARAM );
	virtual int OnEquipClear	( WPARAM, LPARAM );
	virtual int OnEquipAllClear	( WPARAM, LPARAM );

	//virtual int OnWeapon1		( WPARAM wParam, LPARAM lParam );
	//virtual int OnWeapon2		( WPARAM wParam, LPARAM lParam );
	virtual int OnSettingWeapon1( WPARAM wParam, LPARAM lParam );
	virtual int OnSettingWeapon2( WPARAM wParam, LPARAM lParam );
	virtual int OnClearWeapon1	( WPARAM wParam, LPARAM lParam );
	virtual int OnClearWeapon2	( WPARAM wParam, LPARAM lParam );
	virtual int OnActiceSlot	( WPARAM wParam, LPARAM lParam );
	
	virtual int OnChangeWeapon	( WPARAM wParam, LPARAM lParam );

	int	OnEquipSelect			(WPARAM wParam, LPARAM lParam);	//[2005/7/8] 아이콘 Pick Up
	int	OnWeapon1Select			(WPARAM wParam, LPARAM lParam);
	int	OnWeapon2Select			(WPARAM wParam, LPARAM lParam);

	int	OnEquipDisable			(WPARAM wParam, LPARAM lParam);	//[2006/10/27] 아이콘 비활성
	int	OnWeapon1Disable		(WPARAM wParam, LPARAM lParam);
	int	OnWeapon2Disable		(WPARAM wParam, LPARAM lParam);
	
	int	OnHide			( WPARAM wParam, LPARAM lParam);			//[2005/7/8]
	int	OnMark1			( WPARAM wParam, LPARAM lParam);			//[2005/7/11]
	int	OnMark2			( WPARAM wParam, LPARAM lParam);			//[2005/7/11]
	int OnExitHide		( WPARAM wParam, LPARAM lParam);			//[2005/5/21] 닫음 버튼 숨김

	// }}


protected:
	
	int m_iActiveWeaponSlot;

	SPWindow* m_pHide;							/**< 숨김 버튼 <br> */

};
