#ifndef __SPTEAM_ARCHIVE_H__
#define __SPTEAM_ARCHIVE_H__

#define MAX_TEAM_NUM		4

class SPPlayer;

class SPTeamArchive
{
public:
	SPTeamArchive();
	~SPTeamArchive();

	void Clear();

	bool GetActive();
	bool EnableInvite();

	bool InsertTeamPlayer(const TEAM_MEMBER& MemberInfo);

	void EraseTeamPlayer(const GU_ID& CharGUID);
	void EraseTeamPlayer(const INT32& CharDBKey);

	void UpdateTeamPlayer(const TEAM_MEMBER& MemberInfo);

	void SetActCharName(const char* pszCharName);
	const char* GetActCharName();

	void SetMaster(bool bMaster);
	bool IsMaster();

	void ChangeMaster(GU_ID MasterID);
	void ChangeMaster(INT32 CharDBKey);

	bool IsMember(GU_ID iID);
	bool IsMember( GU_ID iID , UINT16 uiServerID );

	int GetTeamCount();
	std::vector< TEAM_MEMBER* >* GetTeamList();

	const char* GetCharName(const INT32& CharDBKey);
	INT32 GetCharDBKey(const char* pszCharName);

	GU_ID GetCharGUID(const char* pszCharName);
	const char* GetCharName(const GU_ID& CharID);

	TEAM_MEMBER*	GetCharInfo( const INT32& CharDBKey );

	const char* GetCharNameChannel( const GU_ID& CharID , UINT16 uiServerID );

private:
	bool m_bTeamActive;
	bool m_bMaster;
	int m_iLootingRule;

	std::string m_strActCharName;
	std::vector< TEAM_MEMBER* > m_vpTeamList;
};

#endif