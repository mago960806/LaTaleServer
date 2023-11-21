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
class PhantomCfg
{
public:
	int		m_iMaxPhantomNbr;
	float	m_fFirstAlpha;
	float	m_fDelayTime;
	D3DXCOLOR m_color;
	int		m_iMultiColor;	// 0 for single color, 1 for multi color
	std::vector< D3DXCOLOR > m_vecColors;

	PhantomCfg( void )
		:m_color( 1.0f, 1.0f, 1.0f, 1.0f )
	{
		m_iMaxPhantomNbr = 8;
		m_fDelayTime = 0.07f;
		m_fFirstAlpha = 0.8f;
		m_iMultiColor = 0;
		m_vecColors.clear();
	}
};
extern PhantomCfg *g_pPhantomCfg;
bool initPhantomCfg( void );
//--------------------------------------------------
//--------------------------------------------------
// Phantom LDT config
struct phantomLdt
{
	int		iSubtype;
	string	name;
	int		phantomNumber;
	float	fDelayTime;
	float	fStartAlpha;
	int		iMultiColor;
	vector< D3DXCOLOR > vecColor;

	phantomLdt( void )
	{
		reset();
	}
	void reset( void )
	{
		iSubtype = -1;
		name = "unknown";
		phantomNumber = 0;
		fDelayTime = 0.0f;
		fStartAlpha = 0.0f;
		iMultiColor = 0;
		vecColor.clear();
	}
};
//--------------------------------------------------
class PhantomLDTCfg
{
public:
	static PhantomLDTCfg& getInstance()
	{
		static PhantomLDTCfg __instance;
		return __instance;
	}
	virtual ~PhantomLDTCfg( void );

	bool LoadLdt( void );
	bool ReloadLdt( void );

	bool GetCfgFromSubtype( int iSubtype, PhantomCfg &cfg );
protected:
	PhantomLDTCfg( void );
	bool resetCfg( void );
	bool init( void );

	vector< phantomLdt > m_vecLdtCfg;
};
//--------------------------------------------------
//--------------------------------------------------
struct PhantomData
{
	POINT pos;
	float fDeltaTime;
	PhantomData( void )
	{
		pos.x = 0;
		pos.y = 0;
		fDeltaTime = 0.0f;
	}
};
class PhantomAnim
{
public:
	struct replay_data
	{
		int frameIdx;
		float frameAlpha;
		replay_data( void )
		{
			frameIdx = -1;
			frameAlpha = 1.0f;
		}
	};
public:
	deque< PhantomData >	m_deqPhantomData;
	POINT	m_ptLastPos;
	float	m_fAccuTime;
	float	m_fPhantomAccuTime;
	vector<replay_data>	m_vecPhantomIdx;
	PhantomAnim( void )
	{
		m_ptLastPos.x = 0;
		m_ptLastPos.y = 0;
		m_fAccuTime = 0.0f;
		m_fPhantomAccuTime = 0.0f;
		m_deqPhantomData.clear();
		m_vecPhantomIdx.clear();
		m_vecPhantomIdx.resize( 20 );
	}
};
//--------------------------------------------------
class PhantomEffect : public IRTREffect
{
public:
	PhantomEffect( void );
	PhantomEffect( SPPlayer *pParent, int itemID );
	~PhantomEffect( void );

	void PreProcess( float fDeltaTime ){};
	void Process( float fDeltaTime );
	void Render( float fDeltaTime );

	void SetSubtype( int iSubtype );
	int GetEffectItemID( void ) const;

protected:
	SPPlayer	*m_pPlayer;
	int			m_itemID;
	int			m_iSubtype;
	PhantomCfg	m_cfg;
};
//--------------------------------------------------
