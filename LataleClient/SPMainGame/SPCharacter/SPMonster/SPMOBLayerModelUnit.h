
#pragma once

enum ANIMATION_INDEX;

struct SPGOBModel;
struct SPMOBModel;
struct SPMOBLayerModel;

class SPGameObject;
class SPGOBModelUnit;

#include <string>
using namespace std;

class SPMOBLayerModelUnit : public SPGOBModelUnit
{
public:
	SPMOBLayerModelUnit(SPGameObject* pParent);
	virtual ~SPMOBLayerModelUnit();
	virtual void	Init();
	virtual LRESULT	SPSendMessage(MODEL_VALUE_TYPE mvMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual void	Render(float fTime);
	virtual void	Process(float fTime);

	RECT GetRenderRgn();
	virtual float GetRenderPosX();
	virtual float GetRenderPosY();

#if defined(_CHARTOOL)
	SPMOBLayerModel* GetMOBLayerModel(int iLayer);
	void SetCurAniFrame(int iFrame);
#endif

	void SetLayerTexture(const char* pszImagePath);
#if defined(_USED_TEXTURE_TABLE)
	void SetLayerTexturebyTable(const char* pszImagePath);
#endif

	void SetNewPet( unsigned int ClassID );

protected:
	void ProcessImage(const float fPosX, const float fPosY, float fTime);
	void FrameInit();

	void SetAccumulateAction(float fAccumulateAction);
	void SetAnimationIndex(ANIMATION_INDEX AniInx, int iForce = 0);
	void SetAnimationLoop(bool bLoop);

	float GetAniAccumulateTime();

	void SetFramePos(int iPosX, int iPosY, FACING eFacing);
#if defined(_USED_TEXTURE_TABLE)
	void SetFramePosbyTable(int iPosX, int iPosY, FACING eFacing);
#endif

	void ResetLayerModel();
	void ResetTexture();
	void ReleaseTexture();
	SPTexture* GetTexture(int iLayerNo);

protected:

	TCHAR* m_strImagePath;
	SPMOBLayerModel* m_pMOBLayerModel;
	int m_iLayerNum;

	ANIMATION_INDEX m_eAniIndex;
	ANIMATION_INDEX m_eLastIndex;
	int m_iCurAniFrame;

	float m_fRenderPosX;
	float m_fRenderPosY;
	float m_fDefaultRot;

	float m_fAccumulateAction;
	float m_fAccumulateRenderTime;

	bool m_bMustQuery;
	bool m_bAnimationLoop;
	RECT m_rcRenderRgn;

	std::map< int, SPTexture* > m_mTextureList;
	unsigned int m_uiNewPetClassID;
};


