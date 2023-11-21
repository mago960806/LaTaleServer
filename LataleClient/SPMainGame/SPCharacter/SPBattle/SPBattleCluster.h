
#pragma once

#include <vector>
#include <hash_map>

using namespace std;

class SPCharacterAttack;
class SPMonsterAttack;
class SPCharacterBeAttacked;
class SPMonsterBeAttacked;

enum OBJECT_TYPE;
enum ANIMATION_INDEX;
enum FACING;

class SPBattleCluster
{
	static SPBattleCluster*	m_pInstance;

	stdext::hash_map< unsigned int, SPCharacterAttack > m_hmCharacterAttack;
	stdext::hash_map< unsigned int, SPCharacterBeAttacked > m_hmCharacterBeAttacked;

	stdext::hash_map< unsigned int, vector< SPMonsterAttack > > m_hmMonsterAttack;
	stdext::hash_map< unsigned int, vector< SPMonsterBeAttacked > > m_hmMonsterBeAttacked;

	float					m_fCurrCallTickTime;

public:
	SPBattleCluster();
	virtual ~SPBattleCluster();	

	static SPBattleCluster*	GetInstance();
	static void Release();

	OBJECT_TYPE	GetObjectType ( ANIMATION_INDEX );

	void	Clear();
	void	ClearCharacter();
	void	ClearMonster();
	void	ClearAttack();
	void	ClearBeAttacked();

	bool	IsCharacterAttackEmpty();
	bool	IsMonsterAttackEmpty();
	bool	IsCharacterBeAttackedEmpty();
	bool	IsMonsterBeAttackedEmpty();

	bool	SetAttack(unsigned int uiAniIndex, const SPCharacterAttack	);
	bool	SetAttack(const SPMonsterAttack, unsigned int uiClassID = 0);
	bool	SetAttack(unsigned int uiAniIndex, const SPCharacterBeAttacked);
	bool	SetAttack(const SPMonsterBeAttacked, unsigned int uiClassID = 0);

	bool	GetAttack(unsigned int uiAniIndex, SPCharacterAttack **) ;
	bool	GetAttack(ANIMATION_INDEX, SPMonsterAttack **, unsigned int uiClassID = 0) ;
	bool	GetAttack(unsigned int uiAniIndex, SPCharacterBeAttacked **) ;
	bool	GetAttack(ANIMATION_INDEX, SPMonsterBeAttacked **, unsigned int uiClassID = 0) ;

	float	GetCurrCallTickTime()	{ return m_fCurrCallTickTime; };
	void	SetCurrCallTickTime(float fData) { m_fCurrCallTickTime = fData; };

	bool	IsMonsterAttack(unsigned int uiClassID);
	bool	IsMonsterBeAttacked(unsigned int uiClassID);

	bool	IsCollision(const RECT& rcBeAttacked,
		const vector<ATTACKINFO>& vAttackPoints, const FACING eFacing);

#ifdef _CHARTOOL
	bool	GetAttack( unsigned int uiAniIndex, SPCharacterAttack &) ;
	bool	GetAttack( ANIMATION_INDEX, SPMonsterAttack &) ;
	bool	GetAttack( unsigned int uiAniIndex, SPCharacterBeAttacked &) ;
	bool	GetAttack( ANIMATION_INDEX, SPMonsterBeAttacked &) ;
	
	bool	SaveAttack(string strFilePath);
	bool	SaveBeAttacked(string strFilePath);

	bool	IsMonsterAttack();

	void	DeleteMOBAttack(ANIMATION_INDEX eIndex, unsigned int uiClassID = 0);
	void	DeleteMOBAttack(unsigned int uiClassID);
	void	DeleteMOBBeAttacked(ANIMATION_INDEX eIndex, unsigned int uiClassID = 0);
	void	DeleteMOBBeAttacked(unsigned int uiClassID);

	void	DeleteGOBAttack(unsigned int uiAniIndex);
	void	DeleteGOBBeAttacked(unsigned int uiAniIndex);

#endif

#ifdef _DEBUG
	// 상위 모듈에서 Dump() 호출을 위해 포인터를 얻고있음
	//vector<SPCharacterAttack>*		GetPCAttackerList(){return &m_vCharacterAttack;}
	//map<UINT,vector<SPMonsterAttack> >*		GetMOBAttackerList(){return &m_mMonsterAttack;}
	//vector<SPCharacterBeAttacked>*	GetPCAttackeeList(){return &m_vCharacterBeAttacked;}
	//map<UINT,vector<SPMonsterBeAttacked> >*	GetMOBAttackeeList(){return &m_mMonsterBeAttacked;}

	void Dump();
#endif

};

extern SPBattleCluster* g_pBattleCluster; //by metalgeni