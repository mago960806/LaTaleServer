#ifndef __SPINDUN_MANAGER_H__
#define __SPINDUN_MANAGER_H__

#define INDUN_REQUIRE_NUM			5
#define FORCE_EXIT_CHECK_DELAY		500

struct SPItemRequire;
//enum INDUN_TYPE ;

enum INDUN_TYPE ;
//{
//	IDT_NULL = 0,
//	IDT_GUILD,
//	IDT_PARTY,
//	IDT_SINGLE,
//	IDT_EVENT,
//	IDT_GINDUN,
//};

struct INDUN_INFO
{
	INDUN_TYPE eType;
	int iMapInStage;
	int iMapInGroup;
	int iMapInPosX;
	int iMapInPosY;
	int iMapOutStage;
	int iMapOutGroup;
	int iMapOutPosX;
	int iMapOutPosY;
	std::string strIndunName;
	SPItemRequire stRequire[INDUN_REQUIRE_NUM];

	void Clear()
	{
		eType = IDT_NULL;

		iMapInStage = 0;
		iMapInGroup = 0;
		iMapInPosX = 0;
		iMapInPosY = 0;
		iMapOutStage = 0;
		iMapOutGroup = 0;
		iMapOutPosX = 0;
		iMapOutPosY = 0;
		strIndunName.clear();

		ZeroMemory(stRequire, sizeof(SPItemRequire) * INDUN_REQUIRE_NUM);
	}
	INDUN_INFO& operator = (const INDUN_INFO& indunInfo)
	{
		eType			= indunInfo.eType;
		iMapInStage		= indunInfo.iMapInStage;
		iMapInGroup		= indunInfo.iMapInGroup;
		iMapInPosX		= indunInfo.iMapInPosX;
		iMapInPosY		= indunInfo.iMapInPosY;
		iMapOutStage	= indunInfo.iMapOutStage;
		iMapOutGroup	= indunInfo.iMapOutGroup;
		iMapOutPosX		= indunInfo.iMapOutPosX;
		iMapOutPosY		= indunInfo.iMapOutPosY;
		strIndunName	= indunInfo.strIndunName;
		for( int i = 0 ; i < INDUN_REQUIRE_NUM; i++ )
		{
			stRequire[i] = indunInfo.stRequire[i] ;
		}
		return *this;
	}
};


class SPIndun ;
class SPIndunManager
{
public:

	static SPIndunManager* GetInstance();
	static void Release();

	bool Init();
	void Clear();
	void Process(float fTime);
	void Render();

	bool IsInIndun()		{	return (m_pCurIndun == NULL) ? false : true ;	}
	
	bool EnterIndun(UINT32 iInstanceID, int iIndunID = 0) ;
	void ClearIndunInfo() ;
	void SetIndunForceExit(bool bExit)	{	m_bForceExit = bExit;	}		// 인던 강제 퇴장 설정
	bool GetIndunForceExit()			{	return m_bForceExit;	}

	SPIndun*	GetCurrentIndun()					{	return m_pCurIndun ;	}
	void	SetIndunLdtID(UINT32 iLdtID)			{	m_uiCurIndunLdtID = iLdtID ;			}
	int		GetIndunLdtID()							{	return (m_pCurIndun) ? m_pCurIndun->GetIndunID() : 0 ;			}
	UINT32	GetIndunInstanceID()					{	return (m_pCurIndun) ? m_pCurIndun->GetInstanceID() : 0 ;		}
	bool	FindIndunInfo(int iIndunID, INDUN_INFO& indunInfo) ;
	bool	GetIndunName(int iIndunID, std::string& strIndunName) ;

	void DisplayIndunInfo() ;

protected:
	SPIndunManager();
	virtual ~SPIndunManager();

	bool LoadLDTFile();

protected:
	static SPIndunManager* m_pkInstance;

	std::map< int, INDUN_INFO > m_mIndunData;
	SPIndun*	m_pCurIndun ;				// 현재 인던
	UINT32		m_uiCurIndunLdtID ;			// LDT ID

	bool		m_bForceExit ;				// 인던 강제 퇴장
};

extern SPIndunManager* g_pkIndunManager;

#endif