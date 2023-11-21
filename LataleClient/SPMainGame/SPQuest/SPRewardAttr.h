#pragma once

class SPQuestAttr;

/**
_Reward_EXP	경험치 보상치
_Reward_Ely	엘리 보상치
_Reward_Fame	명성치 보상치
_Reward_EffectID1	보상 효과1
_Reward_EffectID2	보상 효과2
_Reward_EffectID3	보상 효과3
_Reward_EffectID4	보상 효과4
_Reward_ItemID1	보상 아이템1
_Reward_StackCount1	보상 아이템 수1
_Reward_Rare1	보상 아이템 레어도1
_Reward_ItemID2	보상 아이템2
_Reward_StackCount2	보상 아이템 수2
_Reward_Rare2	보상 아이템 레어도2
_Reward_ItemID3	보상 아이템3
_Reward_StackCount3	보상 아이템 수3
_Reward_Rare3	보상 아이템 레어도3
_Reward_ItemID4	보상 아이템4
_Reward_StackCount4	보상 아이템 수4
_Reward_Rare4	보상 아이템 레어도4
_Reward_ItemID5	보상 아이템5
_Reward_StackCount5	보상 아이템 수5
_Reward_Rare5	보상 아이템 레어도5
_Reward_ItemID6	보상 아이템6
_Reward_StackCount6	보상 아이템 수6
_Reward_Rare6	보상 아이템 레어도6
_Reward_Gpoint	길드 포인트 보상치
*/

class SPRewardAttr {
public:
	SPRewardAttr();
	~SPRewardAttr();

	void SetRewardAttr(int iID, int iExp, int iEly, int iFame);

protected:
	void Clear();

public:
	int					m_iID;
	int					m_iExp;
	int					m_iEly;
	int					m_iFame;
	int					m_iEffect[QUESTREWARD_MAX_EFFECTID];
	SPQuestItem			m_Item[QUESTREWARD_MAX_ITEM];
	int					m_iGuildPoint;
};

