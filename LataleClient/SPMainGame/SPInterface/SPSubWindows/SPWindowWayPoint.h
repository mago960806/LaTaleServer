#pragma once


class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPWindowTreeBox;
struct TREEBOX_ITEM;

#define WIID_WAYPOINT_TOP						(WIID_WAYPOINT	+	1)
#define WIID_WAYPOINT_CENTER					(WIID_WAYPOINT	+	2)
#define WIID_WAYPOINT_BOTTOM					(WIID_WAYPOINT	+	3)

#define WIID_WAYPOINT_TITLE						(WIID_WAYPOINT	+	5)
#define WIID_WAYPOINT_CLOSE						(WIID_WAYPOINT	+	6)

#define WIID_WAYPOINT_LINE1						(WIID_WAYPOINT	+	7)
#define WIID_WAYPOINT_LINE2						(WIID_WAYPOINT	+	8)
#define WIID_WAYPOINT_LINE3						(WIID_WAYPOINT	+	9)

#define WIID_WAYPOINT_SKIN_TREE					(WIID_WAYPOINT	+	10)		//트리 컨트롤 스킨
#define WIID_WAYPOINT_TREE						(WIID_WAYPOINT	+	20)		//트리 컨트롤

#define WIID_WAYPOINT_SKIN_DESC					(WIID_WAYPOINT	+	100)	//설명 스킨
#define WiID_WAYPOINT_DESC						(WIID_WAYPOINT	+	110)	//설명

#define WIID_WAYPOINT_IMAGE						(WIID_WAYPOINT	+	150)	//이리스 영상 버튼
#define WIID_WAYPOINT_POSITION					(WIID_WAYPOINT	+	160)	//위치 버튼
#define WIID_WAYPOINT_MOVE						(WIID_WAYPOINT	+	170)	//이동 버튼

class SPTreeBoxItemQuest;

class SPWindowWayPoint : public SPWindow 
{
public:
	SPWindowWayPoint( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowWayPoint();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);
	

protected:
	/**
	 * 
	 @param 
	 @return
	*/
	void InitSubWindow();
	
	/**
	 * 부모 노드를 업데이트 한다 <br> 
	 @param 
	 @return
	*/
	bool UpdateGroup();
	
	/**
	 * 실 웨이포인트 노드를 업데이트 한다 <br>
	 @param 
	 @return
	*/
	bool UpdateWayPoint();

	SPTreeBoxItemQuest* CreateTreeBoxItem( bool bMenuType );
	
	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnUpdateWayPoint	(WPARAM wParam, LPARAM lParam);
	int OnClose				(WPARAM wParam, LPARAM lParam);
	int OnImage				(WPARAM wParam, LPARAM lParam);
	int OnPosition			(WPARAM wParam, LPARAM lParam);
	int OnMovement			(WPARAM wParam, LPARAM lParam);
	int OnTreeBoxLButtonUp	(WPARAM wParam, LPARAM lParam);
	int OnMoveResult		(WPARAM wParam, LPARAM lParam);			//이동 결과 패킷을 받으면 이동 버튼을 풀어준다

private:
	SPWindowTreeBox*		m_pTreeBox;
	D3DXCOLOR				m_cStringColor;
	bool					m_bClickNotify;
	SPWindow*				m_pDesc;
	SPWindowButton*			m_pImage;				/**< 이리스 영상 <br> */
	SPWindowButton*			m_pPosition;			/**< 위치 정보 <br> */
	SPWindowButton*			m_pMove;				/**< 이동 <br> */

	bool	m_bMoveRequest;			/**< 이동 요청 버튼을 눌렀는지 <br> */
	float	m_fDeley;				/**< 이동 요청 버튼 비활성 시간 <br> */
	float	m_fAccumulate;			/**< 이동 요청 시간 갱신용 <br> */

	std::string				m_strNotSaveInfo;		/**< 저장되지 않은 WayPoint의 경우 트리 아이템 이름에 추가될 문자열 <br> */
	std::string				m_strNotSave;			/**< 저장되지 않은 WayPoint의 경우 Desc영역에 출력될 문자열 <br> */

	std::vector<TREEBOX_ITEM*> m_vGroupItem;		/**< 부모노드들의 정보를 기억하는 백터 <br> */
};



