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
//--------------------------------------------------
struct tc_ActionData
{/* in uiAction, use sequence_id animation, add fx_id effect */
	UINT64 uiAction;
	int sequence_id;
	int fx_id;
};

//--------------------------------------------------
const int MAX_TC_ACTION = 8;
struct tumbleCloudCfg
{
	int iSubtype;
	int offsetX;
	int offsetY;
	tc_ActionData	action_data[MAX_TC_ACTION];

	tumbleCloudCfg( void )
	{
		reset();
	}
	void reset( void )
	{
		iSubtype = -1;
		offsetX = 0;
		offsetY = 0;
		memset( &action_data[0], 0, MAX_TC_ACTION * sizeof(tc_ActionData) );
	}
};

//--------------------------------------------------
class TumbleCloudLdtCfg
{
public:
	static TumbleCloudLdtCfg &getInstance( void )
	{
		static TumbleCloudLdtCfg __instance;
		return __instance;
	}
	virtual ~TumbleCloudLdtCfg( void );

	bool LoadLdt( void );
	bool ReloadLdt( void );

	bool GetCfgFromSubtype( int iSubtype, tumbleCloudCfg &cfg );

protected:
	TumbleCloudLdtCfg( void );
	bool resetCfg( void );
	bool init( void );

	vector< tumbleCloudCfg > m_vecLdtCfg;
};

//--------------------------------------------------
class TumbleCloudEffect : public IRTREffect
{
	//--------------------------------------------------
public:
	TumbleCloudEffect( void );
	TumbleCloudEffect( SPPlayer *pParent, int itemID );
	~TumbleCloudEffect( void );

	void PreProcess( float fDeltaTime );
	void Process( float fDeltaTime );
	void Render( float fDeltaTime );

	void SetSubtype( int iSubtype );
	int GetEffectItemID( void ) const;

protected:
	int GetFxIDByAction( UINT64 &uiAction );
	bool GetActionData( UINT64 uiAction, tc_ActionData &actionData );

protected:
	SPPlayer	*m_pPlayer;
	int			m_itemID;
	int			m_iSubtype;
	//--------------------------------------------------
	tumbleCloudCfg	m_cfg;
	UINT64	m_lastAction;
	int		m_restoreSeq;
};
//--------------------------------------------------
