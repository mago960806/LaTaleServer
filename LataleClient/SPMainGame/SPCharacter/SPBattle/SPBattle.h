
#pragma once

#include <vector>

enum MONSTER_INDEX;
enum ANIMATION_INDEX;

#define RECT_SIZE	( 35 )

/** Object Type */
enum	OBJECT_TYPE
{
	OBJECT_NULL = -1, OBJECT_APC, OBJECT_MONSTER, OBJECT_NPC
};

/**attack type index enum : normal, piercing, down(KO)*/
enum ATTACK_TYPE
{
	ATTACK_NORMAL, ATTACK_PIERCING, ATTACK_DOWN, ATTACK_BOTH
};

struct tagBATTLE
{
	int			m_nIndex;		// Attack : attack point index,		Be Attacked : frame index
	RECT		m_rtRange;		// Attack : 1 rect size,			Be Attacked : whole rect size
	tagBATTLE();
	tagBATTLE& operator=(const tagBATTLE &src);
};

/** attack cel ( position, attack type )*/
typedef struct tagATTACKINFO : public tagBATTLE
{
	ATTACK_TYPE	m_AttackType;
	tagATTACKINFO& operator=(const tagATTACKINFO &src);
}ATTACKINFO;

/** Be attacked cel ( position, attack type )*/
typedef struct tagBEATTACKEDINFO : public tagBATTLE
{
	float		m_fAccumulatedTickTime;
	tagBEATTACKEDINFO& operator=(const tagBEATTACKEDINFO &src);
}BEATTACKEDINFO;

/** Be attacked cel ( position, attack type )*/
typedef struct tagAPCBEATTACKEDINFO : public tagBEATTACKEDINFO
{
	//APC_BEATTACKED_INDEX	m_BeAttackedIndex;
	int m_iBeAttackedIndex;
	tagAPCBEATTACKEDINFO& operator=(const tagAPCBEATTACKEDINFO &src);
}APCBEATTACKEDINFO;

//////////////////////////////////////////////////////////////

/**
* @class	SPAttack
* @brief	Super class of Attack
*/
class SPAttack
{
public :
	unsigned int				m_uiAniIndex;
	int							m_nAttackPointNum;
	float						m_fCallTickTime;		/** called tick time ( have relation with delay time ) */
	float						m_fAccumulateTime;
	int							m_iAttackType;
	std::vector< ATTACKINFO >	m_AttackInfo;

	SPAttack() ;
	SPAttack(unsigned int index);
	SPAttack& operator=(const SPAttack &src);
	virtual ~SPAttack() { Clear(); }

	virtual void Clear();
#ifdef _DEBUG
	void	Dump();
#endif
};

/**
* @class	SPCharacterAttack
* @brief	Character attack range
**/
class SPCharacterAttack : public SPAttack
{
public:	
	SPCharacterAttack(){SPAttack();};
	SPCharacterAttack(unsigned int index) : SPAttack(index) {};
	virtual ~SPCharacterAttack() {}

	SPCharacterAttack& operator=(const SPCharacterAttack &src);
};

/**
* @class	SPMonsterAttack
* @brief	Monster attack
**/
class SPMonsterAttack : public SPAttack
{
public:
	SPMonsterAttack() {SPAttack();};
	SPMonsterAttack(unsigned int index) : SPAttack(index) {};
	virtual ~SPMonsterAttack() {}

	SPMonsterAttack& operator=(const SPMonsterAttack &src);

};

//////////////////////////////////////////////////////////////
/**
* @class	SPBeAttacked
* @brief	Super class of be attacked
**/
class SPBeAttacked
{	
public:
	unsigned int		m_uiAniIndex;  
	int					m_nTotalFrame;	
	float				m_fAccumulateTime;

	SPBeAttacked();
	SPBeAttacked(unsigned int index);
	SPBeAttacked& operator=(const SPBeAttacked &src);
	virtual ~SPBeAttacked() { Clear(); }
	
	virtual void Clear();
};

/**
* @class	SPCharacterBeAttacked
* @brief	character be attacked by somebody or else
*/
class SPCharacterBeAttacked : public SPBeAttacked
{	
public :
	std::vector< APCBEATTACKEDINFO >	m_BeAttackInfo;

	SPCharacterBeAttacked(){SPBeAttacked();};
	SPCharacterBeAttacked(unsigned int index) : SPBeAttacked(index) {};
	SPCharacterBeAttacked& operator=(const SPCharacterBeAttacked &src);
	virtual ~SPCharacterBeAttacked() { Clear(); }

	virtual void Clear();
#ifdef _DEBUG
	void	Dump();
#endif
};


/**
* @class	SPCharacterBeAttacked
* @brief	Monster be attacked by character or else
*/
class SPMonsterBeAttacked : public SPBeAttacked
{	
public :	
	std::vector< APCBEATTACKEDINFO > m_BeAttackInfo;
	MONSTER_INDEX					m_nMonsterSizeIndex ;

	SPMonsterBeAttacked() {SPBeAttacked();};
	SPMonsterBeAttacked( ANIMATION_INDEX index ) : SPBeAttacked(index) {};
	SPMonsterBeAttacked& operator=(const SPMonsterBeAttacked &src);
	virtual ~SPMonsterBeAttacked() { Clear(); }

	virtual void Clear() ;
#ifdef _DEBUG
	void	Dump();
#endif
};