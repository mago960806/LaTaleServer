#ifndef __SPEFFECT_MANAGER_H__
#define __SPEFFECT_MANAGER_H__

class SPEffect;

class SPEffectManager
{
public:
	static SPEffectManager* GetInstance();
	static void Release();

	bool Init();
	void Clear();
	bool AddEffect(int iID, SPEffect* pkEffect);

	SPEffect* GetEffect(int iID);

protected:
	SPEffectManager();
	virtual ~SPEffectManager();

	bool IsExistEffect(int iID);
	bool LoadLDTFile();

protected:
	static SPEffectManager* m_pkInstance;
	stdext::hash_map< int, SPEffect* > m_hmEffectList;

};

#endif