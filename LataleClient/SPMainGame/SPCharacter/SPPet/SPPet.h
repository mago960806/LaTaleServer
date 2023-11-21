
#pragma once 
#include <vector>
#include <string>

#define PET_DISTANCE_X		100.0f
#define PET_DISTANCE_Y		50.0f
#define PET_DISTANCE_PORTAL	400.0f
#define	PET_WALK_VELOCITY	100.0f
#define	PET_RUN_VELOCITY	300.0f
#define	PET_JUMP_VELOCITY	-800.0f


enum PART_LAYER;
enum FXUNIT_DAMAGE_TYPE;
enum SKILL_SPECIAL_TYPE;

struct SPGOBModel;

class SPGameObject;
class SPMonsterCoordPhysics;
class SPSoundArchive;
class SPFXArchive;
class SPGOBModelUnit;
class SPMonsterStatusModel;

enum TARGET_DIRECTION
{
	TD_OUT		= 0,
	TD_IN_X		= 1 << 0,
	TD_IN_Y		= 1 << 1,
	TD_LEFT		= 1 << 2,
	TD_RIGHT	= 1 << 3,
	TD_UP		= 1 << 4,
	TD_DOWN		= 1 << 5,
};

const int BETWEEN_PET = 40;

class SPPet : public SPGameObject 
{
public:
	SPPet();
	SPPet(INSTANCE_ID instanceID, CLASS_ID ClassID, SPGameObject* pkParent);
	virtual ~SPPet();

	virtual void Process(float fTime);
	virtual	void Render(float fTime);
	virtual void Init();	
	virtual void Destroy();
	virtual void RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer );

	void ProcessImage(float fTime);
	void ProcessImageJustRender(float fTime);

	virtual void SetState(GOB_STATE_TYPE state, bool bForce = false);
	virtual void SetAction(UINT64 uiAction, bool bForce = false);
	virtual void SetLootingFX(int iID = 0)	{}
		
	void SetPos(float fX, float fY);
	void SetRePos(int iStageID, int iMapGroupID, float fPosX, float fPosY);
	void SetJustRenderPos(int iX, int iY);
	void SetJustRenderAnimation(int iAniIndex, bool bLoop);
	
	void SetSpecialSkill(SKILL_SPECIAL_TYPE eSkillType, int iSkillID, bool bEnable, bool bToggle);
	bool IsEnableSpecialSkill(SKILL_SPECIAL_TYPE eType);
	bool IsToggleOnSpecialSkill(SKILL_SPECIAL_TYPE eType);
	
	std::vector<int> &GetSpecialSkillID();
	std::vector<SKILL_SPECIAL_TYPE> &GetSpecialSkillType();

	void SetSkillAnimation(ANIMATION_INDEX eIndex);

	int	GetCurStageID();
	void SetCurStageID(int iStageID);
	int	GetCurMapGroupID();
	void SetCurMapGroupID(int iMapGroupID);

	void SetGravity(float fGravity, float fMaxDropSpeed);
	void SetJumpSpeed(float fSpeed);
	void SetJustRender(bool bRender);
	void SetName(const char* pszName, D3DXCOLOR* pColor);
	void SetLocal(bool bLocal);
	void SetPetGageRgn(int iCurSP, int iMaxSP, INT64 iCurXP, INT64 iMaxXP);

	void SetChatMsg( const std::vector< std::string > &vecChatMsg );
	void SendChat( void );

	void	SetItemNoID( ITEMNO itemID ) { m_iItemNoID = itemID; }
	ITEMNO	GetItemNoID( void ) { return m_iItemNoID; }

	bool	GetOpenShopStatus( void )	{ return m_bOpenShop; }
	void	SetOpenShopStatus( bool bOpenShop );

	bool	SetPetAttr( t_PetAttr_Info *info );
	t_PetAttr_Info *GetPetAttr( void )
	{
		return &m_petInfo;
	}
	
	BG_ID GetStandLayer();

	void SetPetPlace( int place )
	{
		m_iPetPlace = place;
	}
	void SetTalkBalloon( const char *pszString );
	
	virtual bool IsPreRendering();

	enum PATTERN_TYPE
	{
		PT_WAIT,
		PT_TRACE,
		PT_SELF_POS,
	};

	//--------------------------------------------------
	void SetShopStyle( int iShopStyle );
	void SetShopGUID( GU_ID petShopGUID );
	void SetShopName( std::string strShopName );
	void SetHostName( std::string strHostName )
	{
		m_strPetHostName = strHostName;
	}
	std::string GetHostName( void )
	{
		return m_strPetHostName;
	}
	void SetSkill( std::vector< t_Pet_Skill_To_Client > &vecSkill );
	std::vector< t_Pet_Skill_To_Client > GetSkill( void )
	{
		return m_vecSkill;
	}

	void AddSkill( t_Pet_Skill_To_Client &skill );
	void SetPetGeneration( bool bNewGeneration )
	{
		m_bNewPet = bNewGeneration;
	}
	bool IsNewGenerationPet( void )
	{
		return m_bNewPet;
	}

	void SetAutoChat( bool bAutoChat )
	{
		m_bAutoChat = bAutoChat;
	}
	bool GetAutoChat( void )
	{
		return m_bAutoChat;
	}
	bool ReloadPetClass( void );

	void SetShopUncloseError( bool bErr )
	{
		m_bShopUnclosedError = true;
	}
	bool GetShopUncloseError( void )
	{
		return m_bShopUnclosedError;
	}

	void SetItemID( int iItemID )
	{
		m_iItemID = iItemID;
	}
	int GetItemID( void )
	{
		return m_iItemID;
	}

	void SwitchSkillStatus( SKILL_SPECIAL_TYPE eSpecialType )
	{
		m_SkillStatus[eSpecialType] = !m_SkillStatus[eSpecialType];
	}
	void SetSkillStatus( SKILL_SPECIAL_TYPE eSpecialType, bool bOn )
	{
		if (bOn)
			m_SkillStatus[eSpecialType] = 1;
		else
			m_SkillStatus[eSpecialType] = 0;
	}
	bool GetSkillStatus( SKILL_SPECIAL_TYPE eSpecialType )
	{
		return (m_SkillStatus[eSpecialType] == 1) ? true : false;
	}

	GU_ID GetShopGUID( void );
	virtual bool	IsCursorIn(int iX, int iY);
	virtual bool	IsCursorIn( RECT &rect );
	virtual int		PerformMouseMove(int iX, int iY);
	virtual int		PerformMouseLDown(int iX, int iY);
	virtual int		PerformMouseLUp(int iX, int iY);
	//--------------------------------------------------

protected:
	SPGMESSAGE_OBJ_MAP_DECLARE()
	
	virtual int OnGetPosX(LPARAM lParam);
	virtual int OnGetPosY(LPARAM lParam);
	virtual int OnGetFacing(LPARAM lParam);
	virtual int OnSetLPPosotion(LPARAM lParam);
	virtual int OnSetActionCommand(LPARAM lParam);
	virtual int OnSetActionCompleteCommand(LPARAM lParam);
	virtual int OnPetShout(LPARAM lParam);

	virtual void ProcessPattern(float fTime);
	virtual void SetPattern(PATTERN_TYPE eType);

	virtual void OnWaitPattern(float fTime)		{}
	virtual void OnSelfPosPattern(float fTime)	{}
	virtual void OnTracePattern(float fTime)	{}

	virtual void CaluSelfPos()	{}
	virtual void CheckTraceAction(int iDirection)	{}

	bool IsDistanceSquare(float fCompareDist, bool bIn);
	int GetTargetDirection(float fDistanceX, float fDistanceY);

	bool IsPortal();
	void SetPortal();
	void SetAnimationLoop();

	bool GetExceptionAction(UINT64& uiParentAction);
	bool IsExceptionAction(UINT64 uiAction);

	void SetShowStatus(bool bName, bool bHP, bool bShadow);
	
protected:		

	SPGameObject* m_pkParent;
	GOB_STATE_TYPE m_eCurState;
	GOB_STATE_TYPE m_eLastState;
	PATTERN_TYPE m_ePatternType;

	// Action
	UINT64 m_uiCurAction;
	UINT64 m_uiLastAction;
	ANIMATION_INDEX m_eCurAnimationIndex;
	ANIMATION_INDEX m_eSkillAnimationIndex;

	float m_fCamX;
	float m_fCamY;
	bool m_bJustRender;

	RECT m_RectPosition;

	bool m_bSetSelfPos;
	float m_fPatternAccmulateTime;

	bool m_bAutoLooting[2];
	bool m_bItemSell[2];
	bool m_bAutoItemUsed[2];
	bool m_btagAutoChat[2];
	bool m_btagOpenShop[2];
	
	std::vector<int> m_iSpecialSkillID;
	std::vector<SKILL_SPECIAL_TYPE> m_eSpecialSkillType;

	int m_iJustRenderPosX;
	int m_iJustRenderPosY;

	int m_iCurXP;
	int m_iCurSP;
	int m_iMaxXP;
	int m_iMaxSP;

	int m_iPetPlace;	//!< pet's place, first pet, second pet or third pet, etc.
	FACING	m_Facing;
	std::vector< std::string > m_vecChatMsg;
	ITEMNO	m_iItemNoID;	//!< itemid, UINT64, unique id in server side.
	bool	m_bOpenShop;	//!< is open shop or not.
	bool	m_bShopUnclosedError;	//!< if pet's shop didn't close last time, show a error msg.
	bool	m_bAutoChat;	//!< auto chat
	t_PetAttr_Info	m_petInfo;
	int	m_iShopStyle;
	GU_ID m_uiPetShopGUID;
	std::string m_strPetHostName;
	std::vector< t_Pet_Skill_To_Client > m_vecSkill;
	bool m_bNewPet;			//!< is this pet a new type
	bool m_bInitedAttr;		//!< is new pet attr received
	int m_iItemID;			//!< item id
	int	m_SkillStatus[64];

	// Render Model
	SPGOBModelUnit*			m_pkModelUnit;
	SPGOBCoordPhysics*		m_pkCoordPhysics;
	SPMonsterStatusModel*	m_pkStatusModel;
	SPFXArchive*			m_pkFXArchive;
};



