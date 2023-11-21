// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPMonsterModelUnit
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

enum ANIMATION_INDEX;

struct SPGOBModel;
struct SPMOBModel;
struct SPEquipLayerPool;

class SPGameObject;
class SPGOBModelUnit;

#include <string>
using namespace std;

class SPMonsterModelUnit : public SPGOBModelUnit {
public:
	SPMonsterModelUnit(SPGameObject* pParent);
	virtual ~SPMonsterModelUnit();
	virtual void	Init();
	virtual LRESULT	SPSendMessage(MODEL_VALUE_TYPE mvMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual void	Render(float fTime);
	virtual void	Process(float fTime);

	RECT GetRenderRgn();
	virtual float GetRenderPosX();
	virtual float GetRenderPosY();

	void SetNPC(unsigned int uiClassID);
	void SetNewPet( unsigned int uiClassID );

#if defined(_CHARTOOL)
	SPMOBModel* GetMOBModel();
	void SetCurAniFrame(int iFrame);
#endif

protected:
	void ProcessImage(const float fPosX, const float fPosY, float fTime);
	void FrameInit();

	void SetAccumulateAction(float fAccumulateAction);
	void SetAnimationIndex(ANIMATION_INDEX AniInx, int iForce = 0);
	void SetAnimationLoop(bool bLoop);

	float GetAniAccumulateTime();

	void SetFramePos(SPMOBModel* pMOBModel, int iPosX, int iPosY, FACING eFacing);
#if defined(_USED_TEXTURE_TABLE)
	void SetFramePosbyTable(SPMOBModel* pMOBModel, int iPosX, int iPosY, FACING eFacing);
#endif

protected:

	TCHAR* m_strImagePath;
	SPMOBModel*	m_pMOBModel;

	ANIMATION_INDEX m_eAniIndex;
	ANIMATION_INDEX m_eLastIndex;
	int m_iCurAniFrame;

	float m_fRenderPosX;
	float m_fRenderPosY;
	float m_fDefaultRot;

	float m_fAccumulateAction;
	float m_fAccumulateRenderTime;

	bool m_bMonster;
	bool m_bMustQuery;
	bool m_bAnimationLoop;

	unsigned int m_uiNPCSeqClassID;
	unsigned int m_uiNewPetClassID;
};


