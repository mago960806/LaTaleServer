// ***************************************************************
//  SPWindowGamble   version:  1.0   ¡¤  date: 10/29/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowGamble_h__
#define SPWindowGamble_h__

#pragma once

#define WIID_GAMBLE_CLOSE_ICON					( WIID_GAMBLE + 1 )

typedef std::vector< TRASH_ITEM >							STD_VECTOR_GAMBLE_TRASH_ITEM;
typedef std::vector< int >									STD_VECTOR_GAMBLE_REWARD;

struct GAMBLE_CONSUMPTION_ITEM
{
	int										m_iID;
	int										m_iCount;
	int										m_iErrorType;
	std::string								m_strErrorMsg;
};

typedef std::vector< GAMBLE_CONSUMPTION_ITEM >				STD_VECTOR_GAMBLE_CONSUMPTION_ITEM;

struct LDT_GAMBLE
{
	int										m_iKeyGamble;

	STD_VECTOR_GAMBLE_CONSUMPTION_ITEM		m_vConsumptionItemKey;
	STD_VECTOR_GAMBLE_CONSUMPTION_ITEM		m_vConsumptionItemBox;

	STD_VECTOR_GAMBLE_REWARD				m_vGambleReward;

	int m_iEquipSlotToken;
	int m_iConsumeSlotToken;
	int m_iEtcSlotToken;
	int m_iQuestSlotToken;
	int m_iPetSlotToken;
};

struct LDT_GAMBLE_REWARD
{
	int										m_iKeyGambleReward;
	int										m_iGambleID;

	int										m_iItemID;
	int										m_iItemCount;
	int										m_iItemPoint;
	int										m_iItemRare;

	int										m_iEly;

	int										m_iIcon;
	int										m_iIconIndex;

	std::string								m_strMessage_Item;
	std::string								m_strMessage_Ely;

};

typedef std::map< int , LDT_GAMBLE >						STD_MAP_GAMBLE;
typedef std::map< int , LDT_GAMBLE_REWARD >					STD_MAP_GAMBLE_REWARD;

class SPWindow;
class SPUIUnitManager;
class SPFXArchive;

class SPWindowGamble : public SPWindow 
{

public:
	SPWindowGamble								( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowGamble								( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowGamble						();

	virtual	void		Init					();
	virtual	void		Clean					();
	virtual void		Show					( bool bWithChild = true );
	virtual void		Hide					( bool bSendServer = true);

	virtual	void		Process					( float fTime );
	virtual	void		Render					( float fTime );

//	virtual void		RefreshRelationCoord	();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int					OnCursorEnter			( WPARAM wParam, LPARAM lParam );
	int					OnCursorOut				( WPARAM wParam, LPARAM lParam );
	int					OnCursorMove			( WPARAM wParam, LPARAM lParam );

	int					OnPurge					( WPARAM wParam, LPARAM lParam );
	int					OnItemUse				( WPARAM wParam, LPARAM lParam );
	int					OnResult				( WPARAM wParam, LPARAM lParam );
	int					OnCashShopMoveYes		( WPARAM wParam, LPARAM lParam );

	int					OnClose					( WPARAM wParam, LPARAM lParam );

	// }}

	//////////////////////////////////////////////////////////////////////////

	void				LoadGambleLDT			();
	void				LoadGambleRewardLDT		();
	void				InitItemUnit			();

	bool				RunGamble				( int iGambleID , UINT8 iBattingCount , STD_VECTOR_GAMBLE_REWARD* pvGambleRewardAutoSell );

	void				PrintErrorMsg			( GLOBAL_STRING_ID uiGlobalStringID , bool bChatWindow , D3DXCOLOR stColor , const char* pszName = NULL );
	void				PrintErrorMsg			( const char* pszGlobalString , bool bChatWindow , D3DXCOLOR stColor , const char* pszName = NULL );
	void				SetCashShopDlg			( const char* pszMsg );

	void				AddGamble				( LDT_GAMBLE& stGamble );
	LDT_GAMBLE*			GetGamble				( int iKey );
	void				LinkGamble				( int iGambleID , int iGambleRewardID );

	void				AddGambleReward			( LDT_GAMBLE_REWARD& stGambleReward );
	LDT_GAMBLE_REWARD*	GetGambleReward			( int iKey );

	bool				CheckPlayerState		( bool bErrorShow );
	bool				CheckConsumptionItem	( STD_VECTOR_GAMBLE_TRASH_ITEM* pvTrashItem , UINT8 iBattingCount , STD_VECTOR_GAMBLE_CONSUMPTION_ITEM& vItemKey , STD_VECTOR_GAMBLE_CONSUMPTION_ITEM& vItemBox , bool bErrorShow );
	bool				CheckConsumptionItem	( STD_VECTOR_GAMBLE_TRASH_ITEM* pvTrashItem , GAMBLE_CONSUMPTION_ITEM* pConsumptionItem , bool bIDZeroCheck );
	bool				CheckInventory			( int iSlotCount , bool bErrorShow );
	bool				CheckInventory			( const LDT_GAMBLE& gamble_info );

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	bool									m_bUse;

	SPUIUnitManager							m_UIUnitManager;

	int										m_iUnitCount;

	STD_MAP_GAMBLE							m_mGamble;
	STD_MAP_GAMBLE_REWARD					m_mGambleReward;

	int										m_iLastGambleID;

	//////////////////////////////////////////////////////////////////////////

	SPFXArchive*							m_pFXArchive;

	//////////////////////////////////////////////////////////////////////////
};

#endif // SPWindowGamble_h__