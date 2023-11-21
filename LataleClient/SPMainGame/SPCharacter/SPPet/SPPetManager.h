#pragma once
//--------------------------------------------------
#include "PacketID.h"
#include <vector>
#include <string>
enum PART_LAYER;
enum FXUNIT_DAMAGE_TYPE;
enum SKILL_SPECIAL_TYPE;
enum ANIMATION_INDEX; 
class SPPet;
//--------------------------------------------------
class SPPetManager
{
public:
	SPPetManager( void );
	virtual ~SPPetManager( void );

	void	Init( void );
	void	Destroy( void );
	void	Process( float fDeltaTime );
	void	Render( float fDeltaTime );

	SPPet	*GetPet( PET_PLACE place )
	{
		if (place < PET_PLACE_TYPE_NUMBER && place >= PET_PRIMARY)
			return m_vecPets[place];
		else
			return NULL;
	}
	SPPet	*GetPrimaryPet( void )
	{
		return m_vecPets[0];
	}
	SPPet	*GetPetFromITEMNO( ITEMNO itemNo );
	void	SetPet( PET_PLACE place, SPPet *pet );
	void	AddPet( SPPet *pet );
	void	DelPet( SPPet *pet );
	void	DelPet( PET_PLACE place );

	bool	IsPetOpenShop( SPPet *pet );

	//--------------------------------------------------
	/**
	 *	@brief find if there is a pet that has special skill
	 *	@return
			if found, return true and pass to pet parameter.
			else, return false.
	 */
	bool IsEnableSpecialSkill(SKILL_SPECIAL_TYPE eType, SPPet **pet = 0);
	bool IsToggleOnSpecialSkill(SKILL_SPECIAL_TYPE eType, SPPet	**pet = 0);

	/**
	 *	@brief judge if some pet need render
	 */
	bool JudgePreRender( bool forceRender = false );

	/**
	 *	@brief move pet to specify position
	 *
	 *	@note
	 *		if pet is in status of opening-shop, nothing will happened to it.
	 */
	void SetPos( float fX, float fY );

	/**
	 *	@brief when player change stage, synchronize pet's stage id.
	 *	@note if pet is in status of opening-shop, nothing will happened to it.
	 */
	void SetCurStageID( int iStageID );

	/**
	 *	@brief when player change map group, synchronize pet's map group id.
	 *	@note if pet is in status of opening-shop, nothing will happened to it.
	 */
	void SetCurMapGroupID( int iMapGroupID );

	void SetRePos( int iStageID, int iMapGroupID, float fPosX, float fPosY );
	void SetSkillAnimation( PET_PLACE place, ANIMATION_INDEX eIndex );
	void SetAction( PET_PLACE place, unsigned __int64 uiAction, bool bForce = false );
	void SetChatMsg( PET_PLACE place, std::vector< std::string > &vecChatMsg );
	//--------------------------------------------------
protected:
	SPPet	*m_vecPets[PET_PLACE_TYPE_NUMBER];
	bool	m_bNeedRender[PET_PLACE_TYPE_NUMBER];
	bool	m_bPetShopUncloseErr;
};
//--------------------------------------------------
