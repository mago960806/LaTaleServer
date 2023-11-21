#ifndef __SPLOOTINGBAG_H__
#define __SPLOOTINGBAG_H__

class SPGameObject;
class SPFXArchive;

class SPLootingBag : public SPGameObject
{
public:
	SPLootingBag();
	SPLootingBag(INSTANCE_ID instanceID, CLASS_ID ClassID);
	virtual ~SPLootingBag();

	virtual void Init();
	virtual void Destroy();
	virtual void Process(float fTime);
	virtual void Render(float fTime);
	virtual void RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer );

	float OnGetPosX();
	float OnGetPosY();

	void ProcessImage(float fTime);

	void SetState(GOB_STATE_TYPE state, bool bForce = false);
	void SetAction(UINT64 uiAction, bool bForce = false);

	void SetOwerID(GU_ID id);
	void AddItem(CONTAINER_ITEM item);
	void DeleteItem(CONTAINER_ITEM item);

	bool GetEnablePickup();

	std::vector< CONTAINER_ITEM >* GetItemList();
	bool IsValidItem(CONTAINER_ITEM* pItemInfo);

	int		GetCurStageID();
	void	SetCurStageID(int iStageID);
	int		GetCurMapGroupID();
	void	SetCurMapGroupID(int iMapGroupID);

protected:
	SPGMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnGetPosX(LPARAM lParam);
	virtual int OnGetPosY(LPARAM lParam);
	virtual int OnGetFacing(LPARAM lParam);
	virtual int OnGetStandlayer(LPARAM lParam);
	virtual int OnSetLPPosotion(LPARAM lParam);
	virtual int OnLootingItem(LPARAM lParam);
	virtual int OnSetActionCommand(LPARAM lParam);
	virtual int OnSetActionCompleteCommand(LPARAM lParam);

protected:
	// State
	GOB_STATE_TYPE m_eCurState;
	GOB_STATE_TYPE m_eLastState;

	// Action
	UINT64 m_uiCurAction;
	UINT64 m_uiLastAction;
	
	bool m_bEnablePickup;
	float m_fActionCompleteTime;

	float m_fCamX;
	float m_fCamY;

	GU_ID m_OwerID;
	std::vector< CONTAINER_ITEM > m_vItemList;

	SPGOBModelUnit*	m_pkModelUnit;
	SPGOBCoordPhysics* m_pkCoordPhysics;
	SPFXArchive* m_pkFXArchive;
};

#endif