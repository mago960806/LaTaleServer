#pragma once
//--------------------------------------------------
#define WIID_VISUAL_BUGLE_BG		(WIID_VISUAL_BUGLE + 1)
#define WIID_VISUAL_BUGLE_MSG_LEFT	(WIID_VISUAL_BUGLE + 2)
#define WIID_VISUAL_BUGLE_MSG_MID	(WIID_VISUAL_BUGLE + 3)
#define WIID_VISUAL_BUGLE_MSG_RIGHT	(WIID_VISUAL_BUGLE + 4)
#define WIID_VISUAL_BUGLE_MSG		(WIID_VISUAL_BUGLE + 5)
//--------------------------------------------------
#include "SPWindowBugleSelect.h"
class SPWindow;
class SPPlayer;
class SPImageProcessor;
//--------------------------------------------------
struct VisualBuglePlayerInfo
{
	vector< FIGURE_ITEM > vItems;
	UINT16 usItemNum;
	VisualBuglePlayerInfo( void )
	{
		vItems.clear();
		usItemNum = 0;
	}
};
//--------------------------------------------------
struct VisualBugleInfo
{
	SPPlayer	*m_pPlayer;
	BugleInfo	m_info;
	std::string m_strMsg;
	bool		m_bPreview;
	bool		m_bShowed;
	static UINT64	iCount;
	UINT64		m_iIndex;
	bool		m_bAutoDeletePlayer;
	VisualBuglePlayerInfo m_vbPlayerInfo;
	VisualBugleInfo( void )
	{
		m_pPlayer = NULL;
		m_strMsg.clear();
		m_bPreview = false;
		m_bShowed = true;
		iCount++;
		m_iIndex = iCount;
		m_bAutoDeletePlayer = true;
	}
	~VisualBugleInfo( void )
	{
		m_pPlayer = NULL;
	}
	void reset( void );
};
//--------------------------------------------------
// VB is tha ab. of VisualBugle
enum VisualBugleStatus
{
	VB_WAIT = 0,
	VB_FADEIN,
	VB_SHOW,
	VB_PREFADEOUT,
	VB_FADEOUT,
	VB_OTHER
};
//--------------------------------------------------
class SPWindowVisualBugle : public SPWindow
{
public:
	SPWindowVisualBugle( WND_ID wndClassID, INSTANCE_ID instanceID );
	SPWindowVisualBugle( INSTANCE_ID instanceID, 
		int iX, int iY, int iCX, int iCY, 
		SPWindow *pParent = NULL );
	virtual ~SPWindowVisualBugle( void );

	virtual void Init( void );
	virtual void Clean( void );
	virtual void Show( bool bWithChild = true );
	virtual void Hide( bool bSendServer = true );
	virtual void Process( float fDeltaTime );
	virtual void Render( float fDeltaTime );
	virtual void RefreshRelationCoord( void );

public:
	bool ShowVisualBugle( VisualBuglePlayerInfo &playerInfo,
		const std::string &strMsg,
		const BugleInfo &bugleInfo,
		bool bPreview );
	bool ShowVisualBugleImme( SPPlayer *pPlayer,
		const std::string &strMsg,
		const BugleInfo &bugleInfo,
		bool bPreview );
	bool UnshowImme( void );

public:
	bool IsQueueReadyForNew( void );

private:
	bool _initGUI( void );
	bool showBugle( void );

protected:
	SPPlayer	*m_pPlayer;
	std::string	m_strMsg;
	BugleInfo	m_bugleInfo;
	bool		m_bPreview;
	float		m_fShowTime;
	float		m_fAccuTime;

	float		m_fBGMovieTime;
	float		m_fAvatarDelay;
	float		m_fMsgDelay;

	SPWindowStatic		*m_pImgBG;
	SPWindowStatic		*m_pStrMsg;
	SPImageProcessor	*m_pkImageProcessor;

	std::vector< VisualBugleInfo > m_vecBugleQueue;
	VisualBugleInfo		m_curBugle;
	VisualBugleStatus	m_status;
};
//--------------------------------------------------
// end of file
//