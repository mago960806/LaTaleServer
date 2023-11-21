
#pragma once

#include <string>
#include <math.h>

using namespace std;

#include "SPBattle.h"

class SPBattleCluster;

enum MONSTER_INDEX;
enum ANIMATION_INDEX;

class SPStream;
class SPBattleLoader
{
	//FILE*	m_fptrAseFile;

	SPStream* m_pStream;
	char*	m_strLineData;

	char*	ReadLine();
	void	ReadMonHead(const char* pszString);
	
	void	ReadCharacterAttack();
	void	ReadMonsterAttack(unsigned int uiClassID);
	void	ReadMOBLayerAttack(unsigned int uiClassID);
	void	ReadAttackBody(SPAttack *attack);

	void	ReadCharacterBeAttacked();
	void	ReadMonsterBeAttacked(unsigned int uiClassID);
	void	ReadMOBLayerBeAttacked(unsigned int uiClassID);

	void	ReadValue( int& ,							const char* );
	void	ReadValue( int& , float& ,					const char* );
	void	ReadValue( int& , float& , float& ,	int&,	const char* );
	void	ReadValue( int& , float&, int&, char&,		const char* );
	void	ReadValue( int& , float&, int&,				const char* );
	void	ReadValue( ATTACKINFO&,						const char* );

	void	ReadValue( APCBEATTACKEDINFO&,	MONSTER_INDEX,	const char* );
	void	ReadValue( APCBEATTACKEDINFO&,	bool,			const char* );
	void	ReadValue( APCBEATTACKEDINFO&,	const char* );

	bool LoadBinaryCharAttack(unsigned int uiCharAniIndex);
	bool LoadBinaryCharBeAttacked(unsigned int uiCharAniIndex);
	bool LoadBinaryMonsterAttack(unsigned int uiMOBClassID);
	bool LoadBinaryMonsterBeAttacked(unsigned int uiMOBClassID);
	
public:
	SPBattleLoader();
	~SPBattleLoader();

	unsigned int m_uiCharIndex;
	ANIMATION_INDEX					m_CurAniIndex;	
	
	bool	LoadBattleFile(string strFileName, bool bCharacter, bool bAttack,
		unsigned int uiCharAniIndex = 0, unsigned int uiClassID = 0);
	bool	LoadBinaryBattleFile(string strFileName, bool bCharacter, bool bAttack,
		unsigned int uiCharAniIndex = 0, unsigned int uiClassID = 0);
	
	bool	LoadMOBLayerBattleFile(string strFileName, bool bAttack,
		unsigned int uiClassID);

	bool LoadMOBBattleTableFromFolder(string strFilePath);

	
};
