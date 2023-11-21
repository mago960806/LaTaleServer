#pragma once


/**
 * PvpLevel당 기본 정보를 지닌다 <br>
*/
struct SPPvpGloryLevelAttr {
	int					m_iID;						/**< ItemID <br> LevelIndex <br> */
	UINT64				m_iMinPoint;				/**< 레벨구간 최저 포인트 <br> */
	UINT64				m_iMaxPoint;				/**< 레벨구간 최고 포인트 <br> */
	std::string			m_strLevel;					/**< 레벨표현 문자 <br> */
	int					m_iIconIndex;				/**< 레벨표현 아이콘 인덱스 <br> */

	SPPvpGloryLevelAttr(){
		Clear();
	}

	~SPPvpGloryLevelAttr(){
		Clear();
	}

	void Clear() {
		m_iID					= 0;
		m_iMinPoint				= 0;
		m_iMaxPoint				= 0;
		m_strLevel.clear();
		m_iIconIndex			= 0;		
	}
};



/**
* @class SPPvpGloryLevelArchive
* @brief Pvp명예 레벨데이터 관리
* @author Jinhee
* @version 00.00.01
* @date 2007.11.27
* @bug 없음
* @warning 
*/
class SPPvpGloryLevelArchive {
public:
	SPPvpGloryLevelArchive();
	~SPPvpGloryLevelArchive();

	/**
	* 주어진 PvpPoint 에 해당하는 구간의 SPPvpGloryLevelAttr 를 확인한다 <br>
	@param 
	@return  
	*/
	SPPvpGloryLevelAttr* GetPvpGloryLevel(UINT64 iPvpPoint);

	/**
	* 주어진 레벨의 SPPvpGloryLevelAttr를 확인한다 <br>
	* 실제 인덱스는 0베이스이므로 1을 빼서 가져온다 <br>
	@param 
	@return  
	*/
	SPPvpGloryLevelAttr* GetPvpGloryLevel(int iPvpLevel);

	/**
	 *
	 @param 
	 @return  
	*/
	std::string GetPvpGloryLevelString(int iPvpLevel);

private:
	/**
	 *
	 @param 
	 @return  
	*/
	bool LoadPvpGloryLevelAttr();
	
	/**
	 *
	 @param 
	 @return  
	*/
	void DeleteAllLevelInfo();	


private:
	std::vector<SPPvpGloryLevelAttr*>		m_vpPvpGloryLevelInfo;
};



