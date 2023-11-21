#pragma once


#define WIID_NPCTALK_NPCNAME		(WIID_NPCTALK	+	20 )	/**< NPC 이름 출력 영역 <br> */
#define WIID_NPCTALK_LINE			(WIID_NPCTALK	+	30 )	/**< NPC 이름 경계선 <br> */
#define WIID_NPCTALK_CLOSE			(WIID_NPCTALK	+	40 )	/**< 대화창 종료 버튼 <br> */
#define WIID_NPCTALK_MESSAGE		(WIID_NPCTALK	+	50 )	/**< 메시지 영역 소 <br> */
#define WIID_NPCTALK_SKIN			(WIID_NPCTALK	+	100 )


enum TALK_STATE {
	TALK_STATE_NULL		= 0,				/**< 현재 아무런 상태가 아님 <br> 또는 SavePont의 마지막 결과 메시지 <br>*/
	TALK_STATE_MENU		= 1,				/**< 메뉴를 보여주고 있음 <br> */
	TALK_STATE_START	= 2,				/**< Npc 대화 시작 <br> */
	TALK_STATE_TALK		= 3,				/**< Npc 대화 중 <br> */
	TALK_STATE_END		= 4,				/**< Npc 대화 끝 <br> */
};

const int MAX_NEXT_FRAME		= 2;		/**< 다음 표시에 사용되는 에니메이션 프레임 <br> */

class SPLDTFile;
class SPWindow;
class SPWindowButton;
class SPWindowTypeWriter;

#include "SPEventDEF.h"
struct SPNpcTalk;
struct SPSavePoint;
enum NPC_EVENT_TYPE;

enum TALK_FADE {
	TALK_FADE_IN,
	TALK_FADE_STOP,
	TALK_FADE_OUT,	
};

enum BOX_VIBRATE {
	BOX_VIBRATE_NULL,
	BOX_VIBRATE_LEFT,
	BOX_VIBRATE_RIGHT,
};



/**
* @class SPWindowNpcTalk
* @brief NPC에 대화 창
* @author Jinhee
* @version 00.00.01
* @date 2005.5.
* @bug 없음
* @warning 
*/
class SPWindowNpcTalk : public SPWindow {

public:	
	SPWindowNpcTalk(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowNpcTalk();

	virtual void Init();
	virtual void Clean();
	
	virtual void Show(bool bWithChild = true);
	void ShowComplete();
	
	virtual void Hide(bool bSendServer = true);
	
	
	virtual void Process(float fTime);
	virtual void Render(float fTime);
	

public:
	

protected:

	/**
	 * 하부 컨트롤들을 생성, 초기화 한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void InitSubControl();	

	/**
	 * 해당 index의 NpcTalkData를 NpcHandler에서 로드해 설정하도록 한다 <br>
	 * 내부에서 SetNpcTalk를 호출 <br> 
	 @param 
	 @return NONE
	*/
	void SetNpcTalkData(int iTalkIndex = 0);
	
	/**
	 * 로드한 NpcTalkData의 BoxPos에 의한 다음 NpcTalk동작을 지시 <br>
	 @param 
	 @return 
	*/
	void SetNpcTalk();	

	/**
	 * SavePointData에서 현재 Page의 대화 내용을 가져와 설정한다 <br> 
	 @param int iTalkIndex = 1 대화 인덱스는 SCROOL_END 상태에서 SavePoint 의 msgInfo[index] 의 정보임
	 @return
	*/	
	void SetSavePointTalk(int iTalkIndex = 0);

	/**
	 *
	 @param 
	 @return  
	*/
	bool LoadAttachImage(int iImageIndex = 0);


protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnSetNpcData		( WPARAM wParam, LPARAM lParam );	//처음 SPNpc를 이용하여 UI를 설정한다 
	int OnSetSavePoint		( WPARAM wParam, LPARAM lParam );	//Save Point 이용하여 UI를 설정한 경우
	int OnSetNpcTalk		( WPARAM wParam, LPARAM lParam );	/**< 외부에서 NPCTalk를 로드 하도록 요청한 경우 <br> */
	int OnNpcTalkClose		( WPARAM wParam, LPARAM lParam );	/**< Close Button <br> */
	int OnSetBoxType		( WPARAM wParam, LPARAM lParam );	/**< Box Type Setting <br> */

	int OnHideStart			(WPARAM wParam, LPARAM lParam);		/**< Faid In 작업을 시작한다 <br> */
	int OnTypeWriteEnd		(WPARAM wParam, LPARAM lParam);		/**< 타입라이터의 문자가 다 찍힌 경우 <br> */
	int OnTypeWriteClick	(WPARAM wParam, LPARAM lParam);		/**< 다음동작을 하는 경우 <br> */


private:
	SPWindow*	m_pParent;
	std::string m_strMessage;					/**< 창에 뛰울 Message <br> */	

	SPNpcTalk* m_pNpcTalk;						/**< Npc Talk 내용 Data <br> */		
	SPSavePoint* m_pSavePoint;					/**< Save Point Talk 내용 Data <br> */
	int m_iTalkIndex;							/**< 현재 Save Point Talk 중 인 Page <br> */

	TALK_STATE m_iTalkState;					/**< 현재 윈도우가 표현하는 대화 상황 <br> */
	char	m_szMsg[512];						/**< <br> */

	SPTexture*	m_pWindowSkin[NPC_TALK_WINDOW_MODE_MAX];	/**< 바닥 스킨으로 사용된 텍스쳐 <br> */
	RECT		m_rtSkinSrc[NPC_TALK_WINDOW_MODE_MAX];		/**< 바닥 스킨 소스 <br> */
	
	int			m_iEndHeight;					/**< Skin Faid 작업시 최대높이 비교용 <br> */
	int			m_iEndWidth;					/**< Skin Faid 작업시 최대넓이 비교용  <br> */
	int			m_iMoveSkinHeight;				/**< Skin Faid시 높이에 적용할 값 <br> */
	int			m_iMoveSkinWidth;				/**< Skin Faid시 넓이에 적용할 값 <br> */
	
	int			m_iSkinMoveX;					/**< m_iMoveSkiWidth에 영향을 주는 값 <br> */
	int			m_iSkinMoveY;					/**< m_iMoveSkinHeight에 영향을 주는 값 <br> */

	int			m_iSkinMoveUnitX;				/**< m_iSkinMoveX에 영향을 주는 값 <br> */
	int			m_iSkinMoveUnitY;				/**< m_iSkinMoveY에 영향을 주는 값 <br> */
	
	RECT		m_rtSkinBaseDest;				/**< 생성시 윈도의 영역을 기억 <br> */
	RECT		m_rtSkinDest;					/**< 스킨 렌더 영역용 <br> */

	TALK_FADE	m_iTalkFadeType;				/**< 스킨 패이드 타입 <br> */
	float		m_fFadeAlpha;					/**< 스킨 적용 알파 <br> */
	float		m_fFadeAlphaUnit;				/**< m_fFadeAlpha 영향을 주는 값 <br> */

	float		m_fAccumulateRender;			/**< 렌더링 시간 계산 <br> */
	float		m_fCurDelay;					/**< 프레임 간격 시간 <br> */

	SPWindow*	m_pNpcName;						/**< NPC 이름 <br> */
	SPWindowTypeWriter* m_pTypeWriter;			/**< 대사 <br> */
	bool		m_bTypeWriteEnd;				/**< <br> */
	SPWindowButton* m_pClose;					/**< 종료 버튼 <br> */

	bool		m_bShowAttach;					/**< 부가 이미지를 보여줘야 하는지 <br> */
	RECT		m_rtAttachSkinDest;				/**< 부가 이미지 스킨 출력 영역 <br> */
	RECT		m_rtAttachSkinSrc;				/**< 부가 이미지 스킨 소스  <br> */
	float		m_fAttachSkinAlpha;				/**< 부가 이미지에 적용할 알파  <br> */	
	//ATTACH_FADE	m_iAttachFade;
	
	SPLDTFile*	m_pAttachImageLDTFile;
	RECT		m_rtAttachDest;
	RECT		m_rtAttachSrc;
	SPTexture*	m_pAttachImage;

	NPC_TALK_WINDOW_MODE m_iTalkWindowMode;		/**< 현재 대화창 스킨 종류 <br> */
	BOX_VIBRATE	m_iVibrate;						/**< 스킨 진동 방향 <br> */
	float		m_fVibrateTotalRender;			/**< 총 렌더링 시간 계산 <br> */
	float		m_fVibrateTotalDelay;			/**< 총 프레임 간격 시간 <br> */
	float		m_fVibrateRender;				/**< 프레임 시간 계산 <br> */
	float		m_fVibrateDelay;				/**< 프레임 간격 시간 <br> */

	bool		m_bFocusLose;					/**< 포커스를 읽은 경우 표현 <br> */
	SPTexture*	m_pFocusLoseSkin;				/**< 포커스를 잃은 경우 추가 스킨 <br> */
	RECT		m_rtFocusLoseSkinSrc[NPC_TALK_WINDOW_MODE_MAX];	/**< 포커스를 잃은 경우 추가 스킨 소스 <br> */

	bool		m_bShowNext;					/**< 다음 클릭 표현 <br> */
	SPTexture*	m_pNextSkin;					/**< 다음 클릭 스킨 <br> */
	RECT		m_rtNextSkinDest;				/**< 다음 클릭 출력 영역<br> */
	RECT		m_rtNextSkinSrc[MAX_NEXT_FRAME];/**< 다음 클릭 스킨 소스 <br> */
	int			m_iNextFrame;					/**< 출력 플레임 <br> */
	float		m_fNextRender;					/**< 프레임 시간 계산 <br> */
	float		m_fNextDelay;					/**< 프레임 간격 시간 <br> */
};
