#pragma once

struct SPNpc;
struct SPNpcQuestList;
class SPQuestAttr;



// AJJIYA [4/18/2006]
struct MAPEVENT_INFO_EXT
{
	std::string		m_strDesc;
	int				m_iIcon;
	int				m_iIconIndex;
};

//[xialin 2008/11/25]add
struct MAPINFO_LDT
{
	UINT			m_iID;
	std::string		m_strMapID;
	std::string		m_strMapName;
	int				m_TimeScan;
	int				m_TimeOpen[5];
	int				m_TimeOff[5];
};

//[xialin 2008/11/25]add
struct EVENTPORTAL_INFO
{
	UINT			m_iID;
	int				m_iStageID;
	int				m_iMapGroupID;
	int				m_iMapID;
	float			m_fPosX;
	float			m_fPosY;
};


class SPEventArchive {
public:
	SPEventArchive();
	~SPEventArchive();
	void Init();
	void Clean();
	
	void DeleteAllEvent();
	bool LoadEventLDT();
	MAPEVENT_INFO* GetEvent(int iItemID);
	bool FindEvent( std::vector<MAPEVENT_INFO*>* pvDataOut , int iStage );
	MAPEVENT_INFO_EXT* GetEventExt( UINT32 uiItemID );

	void DeleteAllMapInfo();
	bool LoadMapInfoLDT();
	bool isMapOpenTime(UINT32 iEventID);
	MAPINFO_LDT* GetMapInfo(int iStageID, int iMapGroupID);

	void DeleteAllEvevtPortal();
	bool LoadEvevtPortalLDT();
	EVENTPORTAL_INFO* GetEvevtPortal(UINT32 iEventID);

	void DeleteAllNpc();
	bool LoadNpcLDT();
	SPNpc*	GetNpc(int iItemID);

	void DeleteAllQuestList();
	bool LoadQuestListLDT();
	SPNpcQuestList*	GetQuestList(int iItemID);

	void DeleteAllQuestAttr();
	bool LoadQuestAttrLDT();
	SPQuestAttr* GetQuestAttr(int iItemID);

	bool LoadCoreEventInfoLDT();
	SPCoreEventInfo*	GetCorEventInfo(int iItemID);	
	void DeleteAllCoreEventInfo();

	bool LoadCoreRequireAttrLDT();	
	SPCoreRequireAttr*	GetCoreRequireAttr(int iItemID);
	bool InsertCoreRequireAttr(SPCoreRequireAttr& iRequireAttr);

	//
	bool LoadCutInTalkLDT();
	void DeleteAllCutInTalk();

	//void DeleteAllSellItem();						//사용 보류
	//bool LoadSellItemLDT();						//사용 보류
	//SPSellItem*	GetSellItem(int iItemID);		//사용 보류

	SPCutInTalk*		GetCutInTalk(int iItemID);

	bool LoadGlobalSysLDT();
	
	/**
	 * 데스 패널티 감소 아이템 <br>
	 @param
	 @return
	*/
	int	 GetDeathItem()			{return m_iDeathItem; }

	/**
	 * 서버 외치기 아이템 <br>
	 @param
	 @return
	*/
	int	 GetMicServerItem()		{return m_iMicServer; }

	/**
	 * 채널 외치기 아이템 <br>
	 @param
	 @return
	*/
	int	 GetMicChannelItem()	{return m_iMicChannel; }

	/**
	 * 우편 요금 <br>
	 @param
	 @return
	*/
	int	 GetMailFee()			{return m_iMailFee; }

	/**
	 * 우표 아이템 ID <br>
	 @param
	 @return
	*/
	int	 GetMailStampItem()		{return m_iMailStampItem; }


protected:
	//bool	InsertItem(SPID_FX id, LPARAM lParam = 0);

protected:	
	std::vector<MAPEVENT_INFO*>					m_vpEventInfo;
	std::map< UINT32 , MAPEVENT_INFO_EXT >		m_mEventInfoExt;
	std::vector<MAPINFO_LDT*>					m_vpMapInfoLdt;
	std::map< UINT32, EVENTPORTAL_INFO*>		m_mEventPortal;

	std::vector<SPNpc*>	m_vpNpcInfo;

	std::vector<SPNpcQuestList*> m_vpQuestListInfo;

	std::vector<SPQuestAttr*>	m_vpQuestAttrInfo;
	
	std::vector<SPCoreEventInfo*>			m_vpCoreEventInfo;
	std::map<int, SPCoreRequireAttr>		m_mCoreEventReqInfo;

	std::vector<SPCutInTalk*>					m_vpCutInTalk;

	std::vector<SPSellItem*>	m_vpSellItem;		//사용 보류...

	int							m_iDeathItem;		/**< 데스 패널티 무효 아이템 ID <br> */
	int							m_iMicServer;		/**< 서버 외치기 아이템 ID <br> */
	int							m_iMicChannel;		/**< 채널 외치기 아이템 ID <br> */
	int							m_iMailFee;			/**< 우편 요금 <br> */
	int							m_iMailStampItem;	/**< 우표 아이템 ID <br> */
};



