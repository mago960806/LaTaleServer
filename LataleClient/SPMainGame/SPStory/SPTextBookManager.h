

#pragma once

struct TEXTBOOK_FACTOR
{
	std::string strText;		// 스토리 내용
	std::string strImage;		// 삽화
	std::string strBGM;			// 배경음
	int			iPrePage ;		// 이전 페이지 Index (없으면 첫페이지)
	int			iNextPage ;		// 다음 페이지 Index (없으면 마지막)
	int			iImageIndex ;	// 삽화내 이미지 Index

	void Clear()
	{
		strText.clear();
		strImage.clear();
		strBGM.clear();
		iPrePage	= 0 ;
		iNextPage	= 0 ;
		iImageIndex = 0 ;
	}
	TEXTBOOK_FACTOR()
	{
		Clear();
	}
	~TEXTBOOK_FACTOR()
	{
		Clear() ;
	}
};


class SPTextBookManager
{
public:
	static SPTextBookManager* GetInstance();
	static void Release();

	bool Init();
	
	TEXTBOOK_FACTOR* GetStoryFactor(int iID);
	void SetTextBookID(int iTextBookID)		{	m_iTextBookID = iTextBookID ;	}
	int GetTextBookID()							{	return m_iTextBookID ;	}
	int	GetImageSrc(int iIndex, int& iPosX, int& iPosY);

private:
	SPTextBookManager();
	~SPTextBookManager();

	bool LoadLDTFile();

private:
	static SPTextBookManager* m_pkInstance;
	int	 m_iTextBookID ;

	std::map< int, TEXTBOOK_FACTOR > m_mStoryFactor;
};

extern SPTextBookManager* g_pkBookManger;

