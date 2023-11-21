#ifndef __SPEFFECT_H__
#define __SPEFFECT_H__

enum EFFECT_TYPE
{
	ET_NULL = 0, ET_INSTANT, ET_KEEP, ET_TOGGLE, ET_PASSIVE
};

enum EFFECT_MISSILE_TYPE
{
	EMT_NULL = 0, EMT_MISSILE
};

enum EFFECT_STATUS_TYPE
{
	EST_NULL = 0, EST_PHYSICAL, EST_MAGIC, EST_STAT, EST_EVENT
};

enum EFFECT_STATUS_KIND_TYPE
{
	DSKT_NULL					= 0,
	DSKT_PHYSICAL_ATTACK		= 10001,
	DSKT_PHYSICAL_MIGHT_ATTACK	= 10002,
	DSKT_MAGIC_FIRE				= 20001,
	DSKT_MAGIC_WATER			= 20002,
	DSKT_MAGIC_WIND				= 20003,
	DSKT_MAGIC_EARTH			= 20004,

	DSKT_CLASS					= 1,
	DSKT_GENDER,
	DSKT_LV,
	DSKT_MONEY,
	DSKT_EXP,
	DSKT_HP,
	DSKT_SP,
	DSKT_CURR_FAME,
	DSKT_BASE_FAME,
	DSKT_ADD_FAME,
	DSKT_RATIO_FAME,
	DSKT_CURR_ATK,
	DSKT_BASE_ATK,
	DSKT_ADD_ATK,
	DSKT_RATIO_ATK,
	DSKT_CURR_LUCK,
	DSKT_BASE_LUCK,
	DSKT_ADD_LUCK,
	DSKT_RATIO_LUCK,
	DSKT_CURR_ELE,
	DSKT_BASE_ELE,
	DSKT_ADD_ELE,
	DSKT_RATIO_ELE,
	DSKT_CURR_DEF,
	DSKT_BASE_DEF,
	DSKT_ADD_DEF,
	DSKT_RATIO_DEF,
	DSKT_CURR_MHP,
	DSKT_BASE_MHP,
	DSKT_ADD_MHP,
	DSKT_RATIO_MHP,
	DSKT_CURR_MSP,
	DSKT_BASE_MSP,
	DSKT_ADD_MSP,
	DSKT_RATIO_MSP,
	DSKT_CURR_WATER,
	DSKT_BASE_WATER,
	DSKT_ADD_WATER,
	DSKT_RATIO_WATER,
	DSKT_CURR_WIND,
	DSKT_BASE_WIND,
	DSKT_ADD_WIND,
	DSKT_RATIO_WIND,
	DSKT_CURR_FIRE,
	DSKT_BASE_FIRE,
	DSKT_ADD_FIRE,
	DSKT_RATIO_FIRE,
	DSKT_CURR_EARTH,
	DSKT_BASE_EARTH,
	DSKT_ADD_EARTH,
	DSKT_RATIO_EARTH,
	DSKT_CURR_RECV_HP,
	DSKT_BASE_RECV_HP,
	DSKT_ADD_RECV_HP,
	DSKT_RATIO_RECV_HP,
	DSKT_CURR_RECV_SP,
	DSKT_BASE_RECV_SP,
	DSKT_ADD_RECV_SP,
	DSKT_RATIO_RECV_SP,
	DSKT_CURR_HEAVY,
	DSKT_BASE_HEAVY,
	DSKT_ADD_HEAVY,
	DSKT_RATIO_HEAVY,
	DSKT_CURR_NORMAL,
	DSKT_BASE_NORMAL,
	DSKT_ADD_NORMAL,
	DSKT_RATIO_NORMAL,
	DSKT_CURR_MP_WATER,
	DSKT_BASE_MP_WATER,
	DSKT_CURR_MP_WIND,
	DSKT_BASE_MP_WIND,
	DSKT_CURR_MP_FIRE,
	DSKT_BASE_MP_FIRE,
	DSKT_CURR_MP_EARTH,
	DSKT_BASE_MP_EARTH,
	DSKT_ADD_MP_ELEMENTAL,
	DSKT_RATIO_MP_ELEMENTAL,
	DSKT_CURR_AP_HEAVY,
	DSKT_BASE_AP_HEAVY,
	DSKT_CURR_AP_NORMAL,
	DSKT_BASE_AP_NORMAL,
	DSKT_ADD_AP_ATK,
	DSKT_RATIO_AP_ATK,

	DSKT_RECOVERY_RATIO_HP = 2001,
	DSKT_RECOVERY_RATIO_SP = 2002,

	DSKT_RECOVERY_HP	= 40001,
	DSKT_RECOVERY_SP	= 40002,
};

struct EFFECT_BUFF
{
	int iID;
	int iLevel;
	int iLevelElv;
	int iIcon;
	int iIconIndex;
	std::string strName;
	std::string strDescription;
};

struct EFFECT_MISSILE
{
	EFFECT_MISSILE_TYPE eType;
	int iTargetFX;
	int iSpeed;
};

struct EFFECT_FX_SOUND
{
	int iTargetFX;
	int iTargetSound;

	int iCasterFX;
	int iCasterSound;
};

struct EFFECT_REQUIRE
{
	int iEquipType;
	int iItemKind;
	__int64 iMotionType;
	int iStandCheck;
};

struct EFFECT_STATUS
{
	EFFECT_STATUS_TYPE eType;
	EFFECT_STATUS_KIND_TYPE eKindType;
	float fDelayTime;
	float fDelayTimeElv;
	
	int iParameter[6];

};

class SPEffect
{
public:
	SPEffect();
	SPEffect(int iID, EFFECT_TYPE eType, float fApplyTime, float fApplyTimeElv);
	~SPEffect();

	void Init();
	
	void SetBuffInfo(const EFFECT_BUFF& Src);
	void SetMissileInfo(const EFFECT_MISSILE& Src);
	void SetFXSoundInfo(const EFFECT_FX_SOUND& Src);
	void SetRequireInfo(const EFFECT_REQUIRE& Src);
	void SetStatusInfo(const EFFECT_STATUS& Src);
	void SetAttachFXGroup( int iAttachFXGroup );
	void SetTransparency(int iTransparency);
	void SetName(const char* pszName);
	void SetDescription(const char* pszDescription);

	int GetID();
	int GetAttachFXGroup();
	int GetTransparency();
	
	float GetApplyTime();
	float GetApplyTimeElv();

	EFFECT_TYPE GetType();
	EFFECT_STATUS* GetStatusInfo();
	EFFECT_MISSILE* GetMissileInfo();
	EFFECT_FX_SOUND* GetFXSoundInfo();
	EFFECT_BUFF* GetBuff();
	EFFECT_REQUIRE* GetRequireInfo();

	const char* GetName();
	const char* GetDescription();
	

protected:


protected:
	int m_iID;
	EFFECT_TYPE m_eType;

	float m_fApplyTime;
	float m_fApplyTimeElv;

	EFFECT_BUFF m_BuffInfo;
	EFFECT_MISSILE m_MissileInfo;
	EFFECT_FX_SOUND m_FXSoundInfo;
	EFFECT_REQUIRE m_RequireInfo;
	EFFECT_STATUS m_StatusInfo;
	int	m_iAttachFXGroup;
	int m_iTransparency;

	std::string	m_strName;
	std::string m_strDescription;
};

#include "SPEffect.inl"

#endif