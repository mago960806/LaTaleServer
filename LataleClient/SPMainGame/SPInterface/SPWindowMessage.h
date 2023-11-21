#pragma once

#define WIID_MESSAGE_TOP			(WIID_MESSAGE	+	1 )
#define WIID_MESSAGE_CENTER			(WIID_MESSAGE	+	2 )
#define WIID_MESSAGE_BOTTOM			(WIID_MESSAGE	+	3 )

#define WIID_MESSAGE_MESSAGE		(WIID_MESSAGE	+	10 )	//메시지 영역
#define WIID_MESSAGE_INFO			(WIID_MESSAGE	+	20 )	//Info 영역

enum FRAME_IMAGE_TYPE {
	FRAME_IMAGE_TYPE_NULL = 0,				/**< Frame Image Auto Load <br> */
	FRAME_IMAGE_TYPE_ROOF = 1,				/**< Frame Image Roof <br> */
	FRAME_IMAGE_TYPE_CENTER = 2,			/**< Frame Image Center <br> */
	FRAME_IMAGE_TYPE_BOTTOM = 3,			/**< Frame Image Bottom <br> */
};


class SPWindow;

class SPWindowMessage : public SPWindow {

public:	
	SPWindowMessage(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowMessage();

	virtual void Init();
	virtual void Clean();
	virtual void SetFocus();
	virtual void Process(float fTime);
	virtual void Render(float fTime);
	virtual void RenderText();

	//virtual void Show(bool bWithChild = true);	
	//virtual bool IsShow();
	//virtual void Hide();  // 서브게임 내에서 만 쓰임..

public:


protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnMessageShow		( WPARAM wParam, LPARAM lParam );
	virtual int OnMessageTextSet	( WPARAM wParam, LPARAM lParam );

protected:
	std::string m_strMessage;					/**< 창에 뛰울 Message <br> */
	std::string m_strInfo;						/**< 창 하단에 표실될 행동 정보 <br> */

	SPWindow* m_pMessage;						/**< Static Window Message <br> */
	SPWindow* m_pInfo;							/**< Static Window Info <br> */

protected:


};
