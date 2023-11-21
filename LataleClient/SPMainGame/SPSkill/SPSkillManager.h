#ifndef __SPSKILL_MANAGER_H__
#define __SPSKILL_MANAGER_H__

class SPSkill;
class SPMotionStatus;
class SPActionInterface;
class SPAttackCombo;

class SPSkillManager
{
public:

	static SPSkillManager* GetInstance();
	static void Release();
	
	bool Init();
	void Clear();
	bool AddSkill(int iID, SPSkill* pkSkill);
	bool RemoveSkill(int iID);

	SPSkill* GetSkill(int iID);
	SPSkill* GetSkill(const char* pszName);

	bool AddMotionStatus(int iID, SPMotionStatus* pkMotionStatus);
	bool RemoveMotionStatus(int iID);

	bool AddMOBMotionStatus(int iID, SPMotionStatus* pkMotionStatus);

	SPMotionStatus* GetMotionStatus(int iID);
	SPMotionStatus* GetMOBMotionStatus(int iID);

	bool AddActionInterface(int iID, SPActionInterface* pkActionInterface);
	SPActionInterface* GetFirstActionInterface();
	SPActionInterface* GetNextActionInterface();
	SPActionInterface* GetActionInterface(int iID);
	int GetActionInterfaceCount();

	bool AddAttackCombo(int iID, SPAttackCombo* pkAttackCombo);
	int GetNextAttackComboNo(int iID, int iAttackType);

	SKILL_UI_INFO* GetSkillSubID( int iSubID , int iIndex , SKILL_INVENTORY_TYPE eType );
	int	GetSkillSubIDPage( int iSubID , SKILL_INVENTORY_TYPE eType );

protected:
	SPSkillManager();
	virtual ~SPSkillManager();

	bool IsExistSkill(int iID);
	bool IsExistMotionStatus(int iID);
	bool IsExistMOBMotionStatus(int iID);
	bool IsExistActionInterface(int iID);

	bool LoadLDTFile();
	bool LoadLDTMotionFile();
	bool LoadMOBMotionFile();
	bool LoadLDTActionInterfaceFile();
	bool LoadLDTAttackComboFile();

	// SKILL SubID 와 Index키를 가지고 검색을 하기위한 자료 구조 [6/21/2006]
	typedef	std::map< int , SKILL_UI_INFO* >				STD_MAP_SKILL_FIND;

	struct SKILL_FIND_BUNDLE
	{
		int						m_iEndPage;
		STD_MAP_SKILL_FIND		m_mUIContent;
	};

	typedef	std::map< int , SKILL_FIND_BUNDLE >				STD_MAP_SKILL_FIND_BUNDLE;

	//////////////////////////////////////////////////////////////////////////

	SKILL_FIND_BUNDLE* GetSkillFindBundle( int iSubID , STD_MAP_SKILL_FIND_BUNDLE& mBundle );
	void AddSkillFind( SKILL_UI_INFO* pSkillUIInfo , STD_MAP_SKILL_FIND_BUNDLE& mBundle );
	void DelSkillFind( SKILL_UI_INFO* pSkillUIInfo , STD_MAP_SKILL_FIND_BUNDLE& mBundle );

protected:
	static SPSkillManager* m_pkInstance;
	
	int m_iCurIndex;

	stdext::hash_map< int, SPSkill* > m_hmSkillList;
	stdext::hash_map< int, SPMotionStatus* > m_hmMotionStatusList;
	stdext::hash_map< int, SPMotionStatus* > m_hmMOBMotionStatusList;
	stdext::hash_map< int, SPActionInterface* > m_hmActionInterfaceList;
	stdext::hash_map< int, SPActionInterface* > m_hmNullActionInterfaceList;
	stdext::hash_map< int, SPAttackCombo* > m_hmAttackComboList;

	// SKILL SubID 와 Index키를 가지고 검색을 하기위한 자료 구조 [6/21/2006]
	STD_MAP_SKILL_FIND_BUNDLE				m_mSkillListFind;
	STD_MAP_SKILL_FIND_BUNDLE				m_mActionInterfaceListFind;

	
};

#endif