#ifndef __SPCOOLTIME_MANAGER_H__
#define __SPCOOLTIME_MANAGER_H__

struct COOLTIME_INFO
{
	float fAccmulateTime;
	float fCoolTime;
};

class SPCoolTimeManager
{
public:
	static SPCoolTimeManager* GetInstance();
	static void Release();

	void Clear();
	void Process(float fElapsedTime);

	bool AddCoolTime(int iID, const COOLTIME_INFO& Src);
	
	bool IsExistCoolTime(int iID);
	COOLTIME_INFO* GetCoolTime(int iID);

protected:
	SPCoolTimeManager();
	virtual ~SPCoolTimeManager();

protected:
	static SPCoolTimeManager* m_pkInstance;

	stdext::hash_map< int, COOLTIME_INFO > m_hmCoolTimeList;
};

#endif