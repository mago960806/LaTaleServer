#ifndef __SPSTORY_MANAGER_H__
#define __SPSTORY_MANAGER_H__

struct STORY_FACTOR
{
	std::string strBGM;
	std::vector< std::string > vstrImage;

	void Clear()
	{
		strBGM = "";
		vstrImage.clear();
	}

	~STORY_FACTOR()
	{
		vstrImage.clear();
	}
};

class SPStoryManager
{
public:
	static SPStoryManager* GetInstance();
	static void Release();

	bool Init();
	
	STORY_FACTOR* GetStoryFactor(int iID);
	void SetStoryViewID(int iStoryViewID)		{	m_iStoryViewID = iStoryViewID ;	}
	int GetStoryViewID()						{	return m_iStoryViewID ;	}

private:
	SPStoryManager();
	~SPStoryManager();

	bool LoadLDTFile();

private:
	static SPStoryManager* m_pkInstance;
	int	 m_iStoryViewID ;

	std::map< int, STORY_FACTOR > m_mStoryFactor;
};

extern SPStoryManager* g_pkStoryManger;

#endif