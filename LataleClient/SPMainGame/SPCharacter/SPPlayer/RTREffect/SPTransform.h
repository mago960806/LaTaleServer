//--------------------------------------------------
#pragma once
//--------------------------------------------------
#include <wtypes.h>
#include <deque>
#include <vector>
#include <string>
#include <d3dx9math.h>
#include "RTREffectManager.h"
using std::deque;
using std::vector;
using std::string;
class SPPlayer;
class SPTransformMonster;
struct TMonsterPoolData
{
	int ID;
	SPTransformMonster *pMonster;
};
//--------------------------------------------------
struct transformCfg
{
	int iSubtype;
	int iMonsterID;

	transformCfg( void )
	{
		reset();
	}
	void reset( void )
	{
		iSubtype = -1;
		iMonsterID = 0;
	}
};
//--------------------------------------------------
class TransformLdtCfg
{
public:
	static TransformLdtCfg &getInstance( void )
	{
		static TransformLdtCfg __instance;
		return __instance;
	}
	virtual ~TransformLdtCfg( void );

	bool LoadLdt( void );
	bool ReloadLdt( void );

	bool GetCfgFromSubtype( int iSubtype, transformCfg &cfg );
protected:
	TransformLdtCfg( void );
	bool resetCfg( void );
	bool init( void );

	vector< transformCfg > m_vecLdtCfg;
};
//--------------------------------------------------
class TransformEffect : public IRTREffect
{
public:
	TransformEffect( void );
	TransformEffect( SPPlayer *pParent, int iItemID );
	~TransformEffect( void );

	void PreProcess( float fDeltaTime ){};
	void Process( float fDeltaTime );
	void Render( float fDeltaTime );
	
	void SetSubtype( int iSubtype );
	int GetEffectItemID( void ) const;

protected:
	SPPlayer	*m_pPlayer;
	TMonsterPoolData	m_pMonsterData;
	SPTransformMonster	*m_pMonster;
	int			m_itemID;
	int			m_iSubtype;
	transformCfg	m_cfg;
};
//--------------------------------------------------
class TransformMonsterPool
{
public:
	TransformMonsterPool( void );
	~TransformMonsterPool( void );

	static TransformMonsterPool &getInstance( void )
	{
		static TransformMonsterPool _instance;
		return _instance;
	}

	TMonsterPoolData	GetMonster( void );
	bool ReleaseMonster( int iID );
	bool Init( void );
protected:
	bool Destroy( void );
	int generateID( void );

	vector< TMonsterPoolData > m_vecFreeMonster;
	vector< TMonsterPoolData > m_vecUsedMonster;
	int m_iPoolSize;
};
//--------------------------------------------------
struct MonsterTypeInfo
{
	int iMonsterID;
	int iMonsterType;
};
class MonsterTypeManager
{
public:
	static MonsterTypeManager& getInstance( void )
	{
		static MonsterTypeManager _instance;
		return _instance;
	}
	MonsterTypeManager( void );
	~MonsterTypeManager( void );

	void Init( void );
	int GetMonsterType( int iMonsterID );

protected:
	std::vector< MonsterTypeInfo > m_vecMonsterType;
};
//--------------------------------------------------
