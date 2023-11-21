#ifndef __SPSTORY_ACTOR_H__
#define __SPSTORY_ACTOR_H__

class SPStoryScene;

enum ACTION_COMPLETE_TYPE
{
	ACT_LOOP = 0,
	ACT_TIME,
	ACT_POS,
	ACT_SEQ,
};

struct ACTOR_ACTION
{
	UINT64 uiAction;
	int iSeqIndex;
	int iSkillID;
	
	float m_fStrPosX;
	float m_fStrPosY;
	float m_fDstPosX;
	float m_fDstPosY;
	float m_fCurPosX;
	float m_fCurPosY;

	float m_fAccmulateTime;
	float m_fCompleteTime;

	ACTION_COMPLETE_TYPE eCompleteType;
};

struct ACTOR_DEFAULT_LAYER
{
	int iClassType;
	int iGender;
	int iDefaultHair;
	int iDefaultExpression;
	int iDefaultSkin;
	int iDefaultBlouse;
	int iDefaultPants;
	int iCap;
	int iGlasses;
	int iEarring;
	int iBlouse;
	int iPants;
	int iCloak;
	int iGlove;
	int iFootwear;
	int iStocking;
	int iMakeup;
	int iWeapon;
	int iShield;
};

struct ACTOR_SPEAK
{
	int iNo;
	int iPage;
	int iImage;
	std::string strTalkMessage;
	std::string strHelpMessage;
};

class SPStoryActor
{
public:
	SPStoryActor(INSTANCE_ID iInstanceID, CLASS_ID iClassID,
		const char* pszName, float fPosX, float fPosY, float fVelocityX, float fJumpSpeed,
		bool bPlayer, int iFacing, SPStoryScene* pkScene);
	~SPStoryActor();

	void Clear();
	void Process(float fTime);
	void Render(float fTime);
	
	void StartScene(int SceneNo);
	void WaitAction();
	void ResumeAction();
	void AddAction(int iSceneNo, ACTOR_ACTION& Action);

	void ItemSetting(ACTOR_DEFAULT_LAYER* pLayer);
	void ItemSetting(FIGURE_ITEM ItemInfo);

	void SetRenderOrder(int iOrder);
	int GetRenderOrder();

	const char* GetName();
	int SendGOBMessage(SPGM uiMsg, LPARAM lParam);

private:
	bool ProcessbyTime(float fTime);
	bool ProcessbyPos(float fTime);
	bool ProcessbySeq(float fTime);

	void SendActionComplete();

private:
	SPGameObject* m_pkObject;
	std::string m_strName;
	bool m_bPlayer;

	int m_iRenderOrder;
	int m_iCurSceneNo;
	bool m_bPlayAction;
	SPStoryScene* m_pkScene;
	ACTOR_ACTION* m_pCurAction;
	
	std::map< int, ACTOR_ACTION > m_mActionList;
};

#endif