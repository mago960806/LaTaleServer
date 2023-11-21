// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPAvatarModelUnit
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

enum ANIMATION_INDEX;

struct SPGOBModel;
struct SPEquipLayerPool;
struct SPGOBInfoSequencePart;

class SPGameObject;
class SPGOBModelUnit;

#include <string>
using namespace std;

class SPGOBModelUnit;
class PhantomEffect;
class SPAvatarModelUnit : public SPGOBModelUnit {
	friend class PhantomEffect;
public:
	SPAvatarModelUnit(SPGameObject* pParent);
	virtual ~SPAvatarModelUnit();
	virtual void	Init();
	virtual LRESULT	SPSendMessage(MODEL_VALUE_TYPE mvMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual void	Render(float fTime);
	virtual void	Process(float fTime);

	void ForceRender(int iPosX, int iPosY, FACING eFaing, int iWeaponItem, unsigned char iClassType = 0);

	// added by kkandori (11.10.2004)
#ifdef _CHARTOOL
	SPGOBModel*		GetCurGOBModel(void) const 				{ return m_pGOBModel; };
	void			SetCurGOBModel(SPGOBModel *pGOBModel, int nCurrFrame)	
	{ 
		m_pGOBModel = pGOBModel;
		m_iCurAniFrame = nCurrFrame;
	};
	int				GetCurFrame(void)						{ return m_iCurAniFrame; };
	void			SetCurFrame(int iCurAniFrame)			{ m_iCurAniFrame = iCurAniFrame; };
	SPGOBAnimation*	GetAnimation(const unsigned int uiAniInx);
	void			SetAnimation(const unsigned int uiAniInx, SPGOBAnimation* pGOBAnimation);		
#endif

	TCHAR*			GetImagePath()							{ return m_strImagePath; }
	void			SetImagePath(TCHAR* strImagePath)		{ ::lstrcpy(m_strImagePath , strImagePath);}

	RECT GetRenderRgn()			{ return m_rcRenderRgn; }
	virtual float GetRenderPosX()		{ return m_fRenderPosX; }
	virtual float GetRenderPosY()		{ return m_fRenderPosY; }

	void SetTransparencyValue( float value)	
	{ 
		if (value > 1.0f)
			value = 1.0f;
		if (value < 0.0f)
			value = 0.0f;
		m_fTransparencyValue = value;
	}
	float GetTransparencyValue( void ) const
	{
		return m_fTransparencyValue;
	}

protected:	
	bool	SetPartLayerInfo(SPEquipLayerPool* pEquipLayerPool, PART_LAYER PartLayer);
	void	ProcessImageLayer(const float RenderPosX, const float RenderPosY, float fTime );
	void	FrameInit();
	float	GetAccumulateAction() { return m_fAccumulateAction; }
	void	SetAccumulateAction(float fAccumulateAction); 
	void	SetAnimationIndex(unsigned int uiAniInx);
	float	GetAniAccumulateTime();

	void	SetLayerPos(SPGOBModel* pGOBModel, int iPosX, int iPosY,
		FACING eFacing, bool AnimateFace, bool bMainProcess = true, bool bOnlyFacing = false);

#if defined(_USED_TEXTURE_TABLE)
	void	SetLayerPosbyTable(SPGOBModel* pGOBModel, int iPosX, int iPosY,
		FACING eFacing, bool AnimateFace, bool bMainProcess = true,  bool bOnlyFacing = false);
#endif
	
	/**
	 *	@brief re-calulate each layers' position
	 */
	void	ReCalcLayerPos( int iPosXChange, int iPosYChange );

	void	SetAnimationLoop(bool bLoop);
	SPGOBInfoSequencePart* GetSequencePart(SPGOBModel* pGOBModel, PART_LAYER eLayer);
	
public:
	D3DXCOLOR getPhantomColor( void ) const
	{
		return m_colorPhantom;
	}
	bool setPhantomColor( D3DXCOLOR color )
	{
		m_colorPhantom = color;
		return true;
	}

protected:

	TCHAR*				m_strImagePath;

	unsigned int 		m_uiAniInx;
	unsigned int 		m_uiLastAniInx;
	int					m_iCurAniFrame;

	SPGOBModel*			m_pGOBModel;
	float				m_fRenderPosX;
	float				m_fRenderPosY;
	float				m_fBeforeRenderPosX;
	float				m_fBeforeRenderPosY;
	FACING				m_iBeforeFacing;
	RECT				m_rcRenderRgn;
	bool				m_bAllLayerProcess;

	float				m_fAccumulateAction;
	float				m_fAccumulateRenderTime;

	// Rear Effect
	float				m_fAccumulateEffectRearRender;	
	int					m_iSeqRearEffect;	

	// Face Effect
	float				m_fAccumulateEffectFaceRender;
	float				m_fNextEffectFaceRender;
	int					m_iSeqFace;	
	int					m_iBeforeFaceIndex;
	bool				m_bBeforeFaceFlip;
	bool				m_bPartLayerChange;

	bool				m_bMustQuery;
	int					m_iTransparency;
	float				m_fTransparencyValue;
	float				m_fTransparencyAccmulateTime;
	
	// phantom effect
	D3DXCOLOR			m_colorPhantom;

	bool m_bAnimationLoop;
	SPGOBModel* m_pGOBForceModel;
};