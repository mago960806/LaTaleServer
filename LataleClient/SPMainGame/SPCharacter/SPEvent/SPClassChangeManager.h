#pragma once


/**
 * 클래스 체인지 시 추가로 클라이언트에서 체크해주어야 할 정보를 기억 <br> 
*/
struct SPClassChangeReqAttr {
	int				m_iID;						/**< Item Index <br> */
	int				m_iItemID;					/**< 필요 아이템 ID <br> */
	UINT64			m_iEly;						/**< 필요 Ely <br> */

	SPClassChangeReqAttr() {
		Clear();
	}

	~SPClassChangeReqAttr() {
		Clear();
	}

	void Clear() {
		m_iID				= 0;
		m_iItemID			= 0;
		m_iEly				= 0;
	}
};


/**
 * 클래스 변경후 추가 표현할 효과에 관한 정보를 기억 <br>
*/
struct SPClassChangeEffectAttr {
	int				m_iID;
	int				m_iFx1;
	int				m_iFx2;
	int				m_iSoundEffect;
	std::string		m_strMsg;
	UINT8			m_iRed;
	UINT8			m_iGreen;
	UINT8			m_iBlue;
	//std::string		m_strMotion;

	SPClassChangeEffectAttr() {
		Clear();
	}

	~SPClassChangeEffectAttr() {
		Clear();
	}

	void Clear(){
		m_iID					= 0;
		m_iFx1					= 0;
		m_iFx2					= 0;
		m_iSoundEffect			= 0;
		m_strMsg.clear();
		m_iRed					= 0;
		m_iGreen				= 0;
		m_iBlue					= 0;
		//m_strMotion.clear();
	}
};


/**
 * Level up 시 효과 Effect 정보를 지님 <br>
*/
struct SPLevelUpEffectAttr {
	int 			m_iID;	
	int 			m_iLevel;
	int 			m_iClass;
	int 			m_iFx;
	int				m_iSoundEffect;
	std::string		m_strMsg;
	UINT8			m_iRed;
	UINT8			m_iGreen;
	UINT8			m_iBlue;
	int				m_iGuideBook;

	SPLevelUpEffectAttr() {
		void Clear();
	}

	~SPLevelUpEffectAttr() {
		void Clear();
	}

	void Clear() {
		m_iID				= 0;		
		m_iLevel			= 0;
		m_iClass			= CLASS_NULL;
		m_iFx				= 0;
		m_iSoundEffect		= 0;
		m_strMsg.clear();
		m_iRed				= 0;
		m_iGreen			= 0;
		m_iBlue				= 0;
		m_iGuideBook		= 0;
	}

	int SetID(int iLevel, int iClass) {
		m_iClass = iClass;
		m_iLevel = iLevel;
		m_iID = iClass * 1000 + iLevel;
		
		return m_iID;
	}	
};


/**
* @class SPClassChangeManager
* @brief 클래스 변경, 레벨업시 효과에 관한 정보를 관리한다 
* @author Jinhee
* @version 00.00.01
* @date 2007.11.12
* @bug 없음
* @warning 
*/
class SPClassChangeManager {
public:
	SPClassChangeManager();
	~SPClassChangeManager();	

	/**
	 * 클래스 변경을 요청하는 패킷을 전송 <br> 
	 @param int iClassChangeID 변경하려는 클래스의 ID 
	 @return  bool
	*/
	bool SendClassChangeInfo(int iClassChangeID);

	/**
	 *
	 @param 
	 @return  
	*/
	bool OnClassChangeEffect(int iNewClassChangeType);
	
	/**
	 * 주어진 아이템의 추가로 체크할 정보를 가져온다 <br>
	 @param int iItemID
	 @return  SPClassChangeReqAttr*
	*/
	SPClassChangeReqAttr*		GetClassChangeReqAttr(int iItemID);
	
	/**
	 * 주어진 아이템의 클레스 변경 효과 정보를 가져온다 <br>
	 @param 
	 @return  
	*/
	SPClassChangeEffectAttr*	GetClassChangeEffectAttr(int iItemID);

	SPLevelUpEffectAttr*		GetLevelUpEffectAttr(int iItemID);
	SPLevelUpEffectAttr*		GetLevelUpEffectAttr(int iLevel, int iClass);

private:
	/**
	 * 모든 전직 리콰를 삭제한다 <br>
	 @param 
	 @return  
	*/
	void DeleteAllClassChangeAttr();
	
	/**
	 * 전직 리콰를 로드한다 <br>
	 @param 
	 @return  
	*/
	bool LoadClassChangeAttr();
	
	/**
	 * 전직 리콰를 데이터풀에 넣는다 <br>
	 @param 
	 @return  
	*/
	bool InsertClassChangeAttr(SPClassChangeReqAttr* pClassChangeReqAttr);
	
	/**
	 * 모든 클래스 변경 효과를 삭제한다 <br>
	 @param 
	 @return  
	*/
	void DeleteAllClassChangeEffectAttr();
	
	/**
	 * 클래스 변경 효과를 로드한다 <br>
	 @param 
	 @return  
	*/
	bool LoadClassChangeEffectAttr();
	
	/**
	 * 클래스 변경 효과를 데이터풀에 넣는다 <br>
	 @param 
	 @return  
	*/
	bool InsertClassChangeEffectAttr(SPClassChangeEffectAttr* pClassChangeEffectAttr);

	/**
	 *
	 @param 
	 @return  
	*/
	int GetLevelUpEffectID(int iLevel, int iClass);
	
	/**
	 *
	 @param 
	 @return  
	*/
	void DeleteAllLevelUpEffectAttr();
	
	/**
	 *
	 @param 
	 @return  
	*/
	bool LoadLevelUpEffectAttr();
	
	/**
	 *
	 @param 
	 @return  
	*/
	bool InsertLevelUpEffectAttr(SPLevelUpEffectAttr* pLevelUpEffectAttr);

private:
	std::map<unsigned int, SPClassChangeReqAttr*>		m_mClassChangeReqInfo;		/**< 클레스변경시 추가 필요 조건 리스트 <br> */
	std::map<unsigned int, SPClassChangeEffectAttr*>	m_mClassChangeEffectInfo;	/**< 클레스 변경시 클라이언트에서 보여줄 효과 <br> */
	std::map<unsigned int, SPLevelUpEffectAttr*>		m_mLevelUpEffectInfo;		/**< <br> */
};






