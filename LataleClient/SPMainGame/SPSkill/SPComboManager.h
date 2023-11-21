#ifndef __SPCOMBO_MANAGER_H__
#define __SPCOMBO_MANAGER_H__

struct SKILL_COMBO;

struct COMBO_INFO
{
	float fAccmulateTime;
	float fComboMinTime;
	float fComboMaxTime;
};

class SPComboManager
{
public:
	static SPComboManager* GetInstance();
	static void Release();

	void Clear();
	void Process(float fElapsedTime);

	bool AddCombo(int iID, const COMBO_INFO& Src);
	bool AddCombo(int iID, const SKILL_COMBO& Src);
	bool IsExist(int iID);
	bool CanConnection(int iID);

protected:
	SPComboManager();
	virtual ~SPComboManager();

protected:
	static SPComboManager* m_pkInstance;

	std::map< int, COMBO_INFO > m_mComboList;
};

#endif