#ifndef __SPMOB_CLUSTER__
#define __SPMOB_CLUSTER__

class SPGOBManager;
struct SPMOBModel;
struct SPMOBAnimation;
struct SPMOBUIInfo;
struct SPMOBLayerInfo;
struct SPMOBLayerModel;
struct SPMOBLayerAnimation;
struct SPMOBLayerAniFrameInfo;

enum MONSTER_TYPE;

struct TALK_MESSAGE
{
	int iAnimationIndex;
	int iProbability;

	std::vector< std::string > vMessage;

	~TALK_MESSAGE()
	{
		vMessage.clear();
	}
};

class SPMOBCluster
{
public:
#if defined(_LATALE_CLIENT)
	SPMOBCluster(SPGOBManager* pkParent);
#else
	SPMOBCluster();
#endif

	~SPMOBCluster();

	bool Init(string strFilePath="");
	void Clean();
	void CleanStageTexture();
	void CleanDefaultTexture();

#ifdef _LATALE_CLIENT
	bool LoadLDTFile(string strFileName, string strTablePath);
#endif

	bool Load(unsigned int uiClassID, string strFilePath = "");
	bool LoadBinary(unsigned int uiClassID, string strFileName, string strImageName);

	bool LoadMOBLayer(unsigned int uiClassID, string strFilePath = "");
	bool LoadBinaryMOBLayer(unsigned int uiClassID, string strFilePath = "");

	bool QueryAnimationSequence(SPMOBModel** ppMOBModel, 
		unsigned int uiClassID, ANIMATION_INDEX AniInx, int& iAniFrame, bool bLoop = true);
	bool QueryAnimationSequence(SPMOBLayerModel* pMOBLayerModel, int iLayerNum,
		unsigned int uiClassID, ANIMATION_INDEX AniInx, int& iAniFrame, bool bLoop = true);

#if defined(_CHARTOOL)
	bool InsertAnimation(unsigned int uiClassID, SPMOBAnimation* pkAnimation);
	void DeleteAnimation(unsigned int uiClassID, ANIMATION_INDEX AniIndex);
#endif

	SPMOBAnimation* GetAnimation(unsigned int uiClassID, ANIMATION_INDEX AniIndex);
	const char* GetTalkMessage(unsigned int uiClassID, ANIMATION_INDEX eAniIndex);

	float GetAnimationTime(unsigned int uiClassID,
		const ANIMATION_INDEX AniInx);
	float GetAnimationTimeLayer(unsigned int uiClassID,
		const ANIMATION_INDEX AniInx);

	SPMOBUIInfo*	GetMOBUIInfo( unsigned int uiClassID );

	SPTexture* GetIconTexture(int iIndex, RECT& rcSrc);
	int GetIconIndex(unsigned int uiClassID);

#if defined(_USED_TEXTURE_TABLE)
	bool LoadTextureTableMetaFile( string strFileName , string strTablePath );
	bool LoadTextureTable(string strFileName, string strTablePath);
	TEXTURE_TABLE_INFO* GetTextureTable(string strID);
	FRAME_INFO* GetFrame(TEXTURE_TABLE_INFO* pTableInfo, int iFrame);
	FRAME_INFO* GetFrame(string strID, int iFrame);

	void PreDefaultTextureLoading();
	void PreTextureLoading(int iStage);
	void PreTextureLoadingbyID(unsigned int uiClassID, bool bDefault = false, bool bLayer = false);

#endif

	bool InsertLayer(unsigned int uiClassID, SPMOBLayerInfo* pLayerInfo);
	void DeleteLayer(unsigned int uiClassID, const char* pszLayerName);

	SPMOBLayerInfo* GetMOBLayerInfo(unsigned int uiClassID, const char* pszLayer);
	SPMOBLayerInfo* GetMOBLayerInfo(unsigned int uiClassID, int iNo);
	int GetMOBLayerSize(unsigned int uiClassID);
	int GetMOBLayerNo(unsigned int uiClassID, const char* pszLayer);
	
	SPMOBLayerAnimation* GetMOBLayerAnimation(unsigned int uiClassID, int iNo, ANIMATION_INDEX eIndex);
	void InsertMOBLayerAnimation(unsigned int uiClassID, SPMOBLayerAnimation ani);
	bool DeleteMOBLayerAnimation(unsigned int uiClassID, ANIMATION_INDEX eIndex);

	SPMOBLayerAniFrameInfo* GetMOBLayerAniFrame(unsigned int uiClassID,
		int iNo, ANIMATION_INDEX eIndex, int iFrameNo);
	SPMOBLayerAniFrameInfo* FindMOBLayerZOrder(unsigned int uiClassID,
		ANIMATION_INDEX eIndex, int iFrameNo, int iZOrder);

	MONSTER_TYPE GetMonsterType(unsigned int uiClassID);

protected:

	bool IsEqualClassID(unsigned int uiClassID, std::vector< SPMOBAnimation* >** ppvAnimation);
	bool IsEqualStage( unsigned int uiClassID, int iStage );

	bool ConvertStringToStage( stdext::hash_map< std::string , bool >& hmOutData , const char* pszString );

	std::map< unsigned int, std::vector< SPMOBAnimation* > > m_mAnimation;
	std::map< unsigned int, std::vector< SPMOBLayerInfo* > > m_mLayer;
	SPGOBManager* m_pkParent;
	SPTexture* m_pkIconTexture;

	ANIMATION_INDEX m_eAniDefaultIndex;

	std::vector< SPTexture* > m_vStageTexture;

	std::map< unsigned int , stdext::hash_map< std::string , bool > >	m_mStagePalette;

	std::map< unsigned int, std::vector< TALK_MESSAGE > > m_mTalkMessage;
	std::map< unsigned int , SPMOBUIInfo >		m_mMOBUIInfo;
	std::map< unsigned int, int > m_mMarkIcon;

	std::vector< SPTexture* > m_vDefaultTexture;
	std::map< unsigned int, MONSTER_TYPE > m_mMonsterType;

#if defined(_USED_TEXTURE_TABLE)
	stdext::hash_map< std::string, TEXTURE_TABLE_INFO > m_hmTextureTableList;
#endif
};

extern SPMOBCluster* g_pMOBCluster;

#endif