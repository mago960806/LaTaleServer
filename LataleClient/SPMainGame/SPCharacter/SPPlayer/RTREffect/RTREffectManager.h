/**
 *	@brief RTR is the ab. of Real-Time Render
 */
//--------------------------------------------------
#pragma once
//--------------------------------------------------
class SPPlayer;
#include <vector>
#include <string>
using std::vector;
using std::string;
//--------------------------------------------------
enum RTRType
{
	RTRTYPE_NULL = 0,
	RTRTYPE_PHANTOM,
	RTRTYPE_TRANSFORM,
	RTRTYPE_TUMBLECLOUD,

	RTRTYPEENUMNUMBER,
	RTRTYPEFORCEENUMMAX = 1000000
};
struct RTREffectData
{
	int		itemID;
	string	strName;
	RTRType type;
	int		subtype;
	void reset( void )
	{
		itemID = -1;
		strName.clear();
		type = RTRTYPE_NULL;
		subtype = -1;
	}
	RTREffectData( void )
	{
		reset();
	}
};
class RTREffectCfg
{
public:
	static RTREffectCfg &getInstance();
	virtual ~RTREffectCfg( void );

	bool LoadLdt( void );
	bool ReloadLdt( void );

	/**
	 *	@brief get effect data from item id
	 *
	 *	@return return true if found, else false
	 */
	bool GetEffectFromItemID( int itemID, RTREffectData &data );

protected:
	RTREffectCfg( void );
	bool resetCfg( void );
	bool init( void );

	vector< RTREffectData > m_vecData;
};
//--------------------------------------------------
class IRTREffect
{
public:
	virtual ~IRTREffect( void ){};
	virtual void PreProcess( float fDeltaTime ) = 0;
	virtual void Process( float fDeltaTime ) = 0;
	virtual void Render( float fDeltaTime ) = 0;
	virtual void SetSubtype( int iSubtype ) = 0;
	virtual int GetEffectItemID( void ) const = 0;
};
//--------------------------------------------------
struct SPItemAttr;
class SPPlayer;
class RTREffectManager
{
public:
	RTREffectManager( void );
	RTREffectManager( SPPlayer *pParent );
	virtual ~RTREffectManager( void );

	bool Init( void );
	bool Clear( void );

	void PreProcess( float fDeltaTime );
	void Process( float fDeltaTime );
	void Render( float fDeltaTime );

	void AddItemRTREffect( int itemID );
	void DelItemRTREffect( int itemID );
protected:
	SPPlayer	*m_pParent;
	std::vector< IRTREffect* > m_vecRTREffect;
	static bool m_bInited;
};
//--------------------------------------------------
// end of file
