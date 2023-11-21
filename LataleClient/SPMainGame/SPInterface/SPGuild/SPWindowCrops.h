// Copyright (C) DURAGON
//***************************************************************************
// System Name : Crops Window
// Comment     : 
// Creation    : DURAGON 2007-5-29    11:18 
//***************************************************************************

#pragma once

//------------------------------------------------------------------------------------
#define WIID_CROPS_TITLE			(WIID_CROPS + 1)
#define WIID_CROPS_NAME_BG			(WIID_CROPS + 2)
#define WIID_CROPS_ICON				(WIID_CROPS + 3)
#define WIID_CROPS_NAME				(WIID_CROPS + 4)
#define WIID_CROPS_MANURE			(WIID_CROPS + 5)
#define WIID_CROPS_HASTEN			(WIID_CROPS + 6)
#define WIID_CROPS_TIME_BAR			(WIID_CROPS + 7)
#define WIID_CROPS_GROWTH_TIME		(WIID_CROPS + 8)
#define WIID_CROPS_MANURE_TIME		(WIID_CROPS + 9)
#define WIID_CROPS_HASTEN_TIME		(WIID_CROPS + 10)
#define WIID_CROPS_ELIMINATE		(WIID_CROPS + 11)

#define WIID_CROPS_IMAGE			(WIID_CROPS + 12)
//------------------------------------------------------------------------------------

#define GUILD_CROP_IMG	17

class SPGOBModelUnit;
class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPItem ;
class SPTimer ;
class SPGuildArchive ;
class SPWindowGuildCrops : public SPWindow 
{

public:
	SPWindowGuildCrops(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowGuildCrops(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowGuildCrops();

	virtual	void	Init				();
	virtual	void	Clean				();
	virtual void	Show				(bool bWithChild = true);
	virtual void	Hide				(bool bSendServer = true);

	virtual	void	Process				( float fTime );
	virtual	void	Render				( float fTime );

	virtual void	RefreshRelationCoord();

private:
	SPItem*			GetDragItem() ;
	void			ReposOwnImage() ;

	//bool			AddCropMonster() ;
	//void			CleanCropMonster() ;
	//void			ChangeAnimation(UINT32 iCropImageID) ;

	void			ShowCropInfo() ;		// Show Crop Information
	void			UpdateCropTime() ;		// Crop Time Update
	void			UpdateCropImage(int iCropLevel) ;
	bool			ComputeTimeString(UINT32 iTime, char* &pstrTime) ;

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int				OnClose				( WPARAM wParam, LPARAM lParam );	// 닫기
	int				OnGuildCropUpdate	( WPARAM wParam, LPARAM lParam );	// Crop Update

	int				OnManureCursorEnter	( WPARAM wParam, LPARAM lParam );	// 비료
	int				OnManureCursorOut	( WPARAM wParam, LPARAM lParam );	//
	int				OnManureLButtonUp	( WPARAM wParam, LPARAM lParam );	//

	int				OnHastenCursorEnter	( WPARAM wParam, LPARAM lParam );	// 촉진제
	int				OnHastenCursorOut	( WPARAM wParam, LPARAM lParam );	//
	int				OnHastenLButtonUp	( WPARAM wParam, LPARAM lParam );	//

	int				OnCropEliminate		( WPARAM wParam, LPARAM lParam );	// 작물 제거

	int				OnCursorEnter		( WPARAM wParam, LPARAM lParam );
	int				OnCursorOut			( WPARAM wParam, LPARAM lParam );
	int				OnCursorMove		( WPARAM wParam, LPARAM lParam );
	int				OnLButtonUp			( WPARAM wParam, LPARAM lParam );
	int				OnLButtonDown		( WPARAM wParam, LPARAM lParam );
	int				OnRButtonUp			( WPARAM wParam, LPARAM lParam );

	int				OnClickTest( unsigned int iID, WPARAM wParam, LPARAM lParam ) ;
	// }}

	
private:

	//////////////////////////////////////////////////////////////////////////
	//	변수들
	SPTexture*		m_pBGTexture;

//	SPGOBModelUnit*	m_pkModelUnit;				// 작물 NPC
//	UINT32			m_iCropImageID;				// 작물 Object Image ID
//	float			m_fAnimationTime ;			// 작물 Animation

	SPItem*			m_pManureItem;	
	SPItem*			m_pHastenItem;	

	RECT			m_rcCropsBGSrc[GUILD_CROP_IMG] ;
	RECT			m_rcCropsBGDst[GUILD_CROP_IMG] ;
};