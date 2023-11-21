// SPWindow_ComboBox.h

#ifndef SPWindow_ComboBox_h
#define SPWindow_ComboBox_h

#define ID_CB_OFFSET_DROPDOWN_X (SPWindow_Base_MAX_ATTR + 1)
#define ID_CB_OFFSET_DROPDOWN_Y (SPWindow_Base_MAX_ATTR + 2)

#include "SPWindow_Base.h"

class SPWindow_ComboBox : public SPWindow_Base
{
public:
	enum TEXTURESET_CB
	{
		TEXSET_CB_ENABLE = 0			,  // DropDown...
		TEXSET_CB_HIT					,
		TEXSET_CB_PUSH					,

		TEXSET_CB_SCROLLUP_ENABLE		,
		TEXSET_CB_SCROLLUP_HIT			,
		TEXSET_CB_SCROLLUP_PUSH			,
		TEXSET_CB_SCROLLUP_DISABLE		,

		TEXSET_CB_SCROLLDOWN_ENABLE		,
		TEXSET_CB_SCROLLDOWN_HIT		,
		TEXSET_CB_SCROLLDOWN_PUSH		,
		TEXSET_CB_SCROLLDOWN_DISABLE	,

		TEXSET_CB_SLIDER_ENABLE			,
		TEXSET_CB_SLIDER_HIT			,
		TEXSET_CB_SLIDER_PUSH			,
		TEXSET_CB_SLIDER_DISABLE		,

		TEXSET_CB_SLIDERBG_ENABLE		,
		TEXSET_CB_SLIDERBG_HIT			,
		TEXSET_CB_SLIDERBG_PUSH			,
		TEXSET_CB_SLIDERBG_DISABLE		,

		TEXSET_CB_EDITBOX_ENABLE		,
		TEXSET_CB_EDITBOX_HIT			,
		TEXSET_CB_EDITBOX_PUSH			,

		TEXSET_CB_LISTBOX
	};

	enum OFFSET_CB
	{
		OFFSET_CB_DROPDOWN	= 0,
		OFFSET_CB_SCROLLUP,
		OFFSET_CB_SCROLLDOWN,
		OFFSET_CB_SLIDER,
		OFFSET_CB_SLIDER_BG,
		OFFSET_CB_EDITBOX,
		OFFSET_CB_LISTBOX
	};

	SPWindow_ComboBox(){};
	virtual ~SPWindow_ComboBox(){};
public:
	virtual void* Get(int ID)
	{
		switch(ID)
		{

			// TexPos...
			//{
		case GET_SPWINDOW_CB_TEXPOS_DROPDOWN_ENABLE:		return &m_pt_TexPos[TEXSET_CB_ENABLE];
		case GET_SPWINDOW_CB_TEXPOS_DROPDOWN_HIT:			return &m_pt_TexPos[TEXSET_CB_HIT];
		case GET_SPWINDOW_CB_TEXPOS_DROPDOWN_PUSH:			return &m_pt_TexPos[TEXSET_CB_PUSH];

		case GET_SPWINDOW_CB_TEXPOS_SCROLLUP_ENABLE:		return &m_pt_TexPos[TEXSET_CB_SCROLLUP_ENABLE];
		case GET_SPWINDOW_CB_TEXPOS_SCROLLUP_HIT:			return &m_pt_TexPos[TEXSET_CB_SCROLLUP_HIT];
		case GET_SPWINDOW_CB_TEXPOS_SCROLLUP_PUSH:			return &m_pt_TexPos[TEXSET_CB_SCROLLUP_PUSH];
		case GET_SPWINDOW_CB_TEXPOS_SCROLLUP_DISABLE:		return &m_pt_TexPos[TEXSET_CB_SCROLLUP_DISABLE];

		case GET_SPWINDOW_CB_TEXPOS_SCROLLDOWN_ENABLE:		return &m_pt_TexPos[TEXSET_CB_SCROLLDOWN_ENABLE];
		case GET_SPWINDOW_CB_TEXPOS_SCROLLDOWN_HIT:			return &m_pt_TexPos[TEXSET_CB_SCROLLDOWN_HIT];
		case GET_SPWINDOW_CB_TEXPOS_SCROLLDOWN_PUSH:		return &m_pt_TexPos[TEXSET_CB_SCROLLDOWN_PUSH];
		case GET_SPWINDOW_CB_TEXPOS_SCROLLDOWN_DISABLE:		return &m_pt_TexPos[TEXSET_CB_SCROLLDOWN_DISABLE];

		case GET_SPWINDOW_CB_TEXPOS_SLIDER_ENABLE:			return &m_pt_TexPos[TEXSET_CB_SLIDER_ENABLE];
		case GET_SPWINDOW_CB_TEXPOS_SLIDER_HIT:				return &m_pt_TexPos[TEXSET_CB_SLIDER_HIT];
		case GET_SPWINDOW_CB_TEXPOS_SLIDER_PUSH:			return &m_pt_TexPos[TEXSET_CB_SLIDER_PUSH];
		case GET_SPWINDOW_CB_TEXPOS_SLIDER_DISABLE:			return &m_pt_TexPos[TEXSET_CB_SLIDER_DISABLE];

		case GET_SPWINDOW_CB_TEXPOS_SLIDERBG_ENABLE:		return &m_pt_TexPos[TEXSET_CB_SLIDERBG_ENABLE];
		case GET_SPWINDOW_CB_TEXPOS_SLIDERBG_HIT:			return &m_pt_TexPos[TEXSET_CB_SLIDERBG_HIT];
		case GET_SPWINDOW_CB_TEXPOS_SLIDERBG_PUSH:			return &m_pt_TexPos[TEXSET_CB_SLIDERBG_PUSH];
		case GET_SPWINDOW_CB_TEXPOS_SLIDERBG_DISABLE:		return &m_pt_TexPos[TEXSET_CB_SLIDERBG_DISABLE];

		case GET_SPWINDOW_CB_TEXPOS_EDITBOX_ENABLE:			return &m_pt_TexPos[TEXSET_CB_EDITBOX_ENABLE];
		case GET_SPWINDOW_CB_TEXPOS_EDITBOX_HIT:			return &m_pt_TexPos[TEXSET_CB_EDITBOX_HIT];
		case GET_SPWINDOW_CB_TEXPOS_EDITBOX_PUSH:			return &m_pt_TexPos[TEXSET_CB_EDITBOX_PUSH];

		case GET_SPWINDOW_CB_TEXPOS_LISTBOX_ENABLE:			return &m_pt_TexPos[TEXSET_CB_LISTBOX];
			//}
		default:
			return SPWindow_Base::Get(ID);
		}
	}

	virtual void Set(int ID,char* Value)
	{
		//- 툴에선 XML에서 로딩할때 이 부분을 호출 합니다..... Get과는 다르게 꼭 설정 해줘야 하는 부분입니다...
		//	XML --> Specified window.....

		switch(ID)
		{
		case SET_SPWINDOW_CB_OFFSET_DROPDOWN_X:			m_pt_Offset[OFFSET_CB_DROPDOWN].x	= atoi(Value); break;
		case SET_SPWINDOW_CB_OFFSET_DROPDOWN_Y:			m_pt_Offset[OFFSET_CB_DROPDOWN].y	= atoi(Value); break;
		case SET_SPWINDOW_CB_SIZE_DROPDOWN_X:
			{
				m_pt_Size[OFFSET_CB_DROPDOWN].x		= atoi(Value); 

				m_pt_TexPos[TEXSET_CB_ENABLE].x2	= m_pt_TexPos[TEXSET_CB_ENABLE].x	+ m_pt_Size[OFFSET_CB_DROPDOWN].x;
				m_pt_TexPos[TEXSET_CB_HIT].x2		= m_pt_TexPos[TEXSET_CB_HIT].x		+ m_pt_Size[OFFSET_CB_DROPDOWN].x;
				m_pt_TexPos[TEXSET_CB_PUSH].x2		= m_pt_TexPos[TEXSET_CB_PUSH].x		+ m_pt_Size[OFFSET_CB_DROPDOWN].x;
			}
			break;
		case SET_SPWINDOW_CB_SIZE_DROPDOWN_Y:
			{
				m_pt_Size[OFFSET_CB_DROPDOWN].y		= atoi(Value); 

				m_pt_TexPos[TEXSET_CB_ENABLE].y2	= m_pt_TexPos[TEXSET_CB_ENABLE].y	+ m_pt_Size[OFFSET_CB_DROPDOWN].y;
				m_pt_TexPos[TEXSET_CB_HIT].y2		= m_pt_TexPos[TEXSET_CB_HIT].y		+ m_pt_Size[OFFSET_CB_DROPDOWN].y;
				m_pt_TexPos[TEXSET_CB_PUSH].y2		= m_pt_TexPos[TEXSET_CB_PUSH].y		+ m_pt_Size[OFFSET_CB_DROPDOWN].y;
			}
			break;
		case SET_SPWINDOW_CB_POS_DROPDOWN_ENABLE_X:		m_pt_TexPos[TEXSET_CB_ENABLE].x				= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_DROPDOWN_ENABLE_Y:		m_pt_TexPos[TEXSET_CB_ENABLE].y				= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_DROPDOWN_HIT_X:		m_pt_TexPos[TEXSET_CB_HIT].x				= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_DROPDOWN_HIT_Y:		m_pt_TexPos[TEXSET_CB_HIT].y				= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_DROPDOWN_PUSH_X:		m_pt_TexPos[TEXSET_CB_PUSH].x				= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_DROPDOWN_PUSH_Y:		m_pt_TexPos[TEXSET_CB_PUSH].y				= atoi(Value); break;

		case SET_SPWINDOW_CB_POS_EDITBOX_ENABLE_X:		m_pt_TexPos[TEXSET_CB_EDITBOX_ENABLE].x		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_ENABLE_Y:		m_pt_TexPos[TEXSET_CB_EDITBOX_ENABLE].y		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_ENABLE_CX:		m_pt_TexPos[TEXSET_CB_EDITBOX_ENABLE].x2	= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_ENABLE_CY:		m_pt_TexPos[TEXSET_CB_EDITBOX_ENABLE].y2	= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_HIT_X:			m_pt_TexPos[TEXSET_CB_EDITBOX_HIT].x		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_HIT_Y:			m_pt_TexPos[TEXSET_CB_EDITBOX_HIT].y		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_HIT_CX:		m_pt_TexPos[TEXSET_CB_EDITBOX_HIT].x2		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_HIT_CY:		m_pt_TexPos[TEXSET_CB_EDITBOX_HIT].y2		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_PUSH_X:		m_pt_TexPos[TEXSET_CB_EDITBOX_PUSH].x		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_PUSH_Y:		m_pt_TexPos[TEXSET_CB_EDITBOX_PUSH].y		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_PUSH_CX:		m_pt_TexPos[TEXSET_CB_EDITBOX_PUSH].x2		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_EDITBOX_PUSH_CY:		m_pt_TexPos[TEXSET_CB_EDITBOX_PUSH].y2		= atoi(Value); break;

		case SET_SPWINDOW_CB_POS_SCROLLUP_ENABLE_X:		m_pt_TexPos[TEXSET_CB_SCROLLUP_ENABLE].x	= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLUP_ENABLE_Y:		m_pt_TexPos[TEXSET_CB_SCROLLUP_ENABLE].y	= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLUP_HIT_X:		m_pt_TexPos[TEXSET_CB_SCROLLUP_HIT].x		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLUP_HIT_Y:		m_pt_TexPos[TEXSET_CB_SCROLLUP_HIT].y		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLUP_PUSH_X:		m_pt_TexPos[TEXSET_CB_SCROLLUP_PUSH].x		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLUP_PUSH_Y:		m_pt_TexPos[TEXSET_CB_SCROLLUP_PUSH].y		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLUP_DISABLE_X:	m_pt_TexPos[TEXSET_CB_SCROLLUP_DISABLE].x	= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLUP_DISABLE_Y:	m_pt_TexPos[TEXSET_CB_SCROLLUP_DISABLE].y	= atoi(Value); break;

		case SET_SPWINDOW_CB_POS_SCROLLDOWN_ENABLE_X:	m_pt_TexPos[TEXSET_CB_SCROLLDOWN_ENABLE].x	= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLDOWN_ENABLE_Y:	m_pt_TexPos[TEXSET_CB_SCROLLDOWN_ENABLE].y	= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLDOWN_HIT_X:		m_pt_TexPos[TEXSET_CB_SCROLLDOWN_HIT].x		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLDOWN_HIT_Y:		m_pt_TexPos[TEXSET_CB_SCROLLDOWN_HIT].y		= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLDOWN_PUSH_X:		m_pt_TexPos[TEXSET_CB_SCROLLDOWN_PUSH].x	= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLDOWN_PUSH_Y:		m_pt_TexPos[TEXSET_CB_SCROLLDOWN_PUSH].y	= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLDOWN_DISABLE_X:  m_pt_TexPos[TEXSET_CB_SCROLLDOWN_DISABLE].x	= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_SCROLLDOWN_DISABLE_Y:	m_pt_TexPos[TEXSET_CB_SCROLLDOWN_DISABLE].y	= atoi(Value); break;

		case SET_SPWINDOW_CB_POS_LIST_X:				m_pt_TexPos[TEXSET_CB_LISTBOX].x			= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_LIST_Y:				m_pt_TexPos[TEXSET_CB_LISTBOX].y			= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_LIST_CX:				m_pt_TexPos[TEXSET_CB_LISTBOX].x2			= atoi(Value); break;
		case SET_SPWINDOW_CB_POS_LIST_CY:				m_pt_TexPos[TEXSET_CB_LISTBOX].y2			= atoi(Value); break;

		case SET_SPWINDOW_CB_SIZE_SCROLLUP_X:
			{
				INT Width = m_pt_Size[OFFSET_CB_SCROLLUP].x = atoi(Value); 

				m_pt_TexPos[TEXSET_CB_SCROLLUP_ENABLE].x2	= m_pt_TexPos[TEXSET_CB_SCROLLUP_ENABLE].x	+ Width;
				m_pt_TexPos[TEXSET_CB_SCROLLUP_HIT].x2		= m_pt_TexPos[TEXSET_CB_SCROLLUP_HIT].x		+ Width;
				m_pt_TexPos[TEXSET_CB_SCROLLUP_PUSH].x2		= m_pt_TexPos[TEXSET_CB_SCROLLUP_PUSH].x	+ Width;
			}
			break;
		case SET_SPWINDOW_CB_SIZE_SCROLLUP_Y:			
			{
				INT Height = m_pt_Size[OFFSET_CB_SCROLLUP].y = atoi(Value); 

				m_pt_TexPos[TEXSET_CB_SCROLLUP_ENABLE].y2	= m_pt_TexPos[TEXSET_CB_SCROLLUP_ENABLE].y	+ Height;
				m_pt_TexPos[TEXSET_CB_SCROLLUP_HIT].y2		= m_pt_TexPos[TEXSET_CB_SCROLLUP_HIT].y		+ Height;
				m_pt_TexPos[TEXSET_CB_SCROLLUP_PUSH].y2		= m_pt_TexPos[TEXSET_CB_SCROLLUP_PUSH].y	+ Height;

			}
			break;
		case SET_SPWINDOW_CB_SIZE_SCROLDOWN_X:
			{
				INT Width = m_pt_Size[OFFSET_CB_SCROLLDOWN].x = atoi(Value); 

				m_pt_TexPos[TEXSET_CB_SCROLLDOWN_ENABLE].x2	= m_pt_TexPos[TEXSET_CB_SCROLLDOWN_ENABLE].x	+ Width;
				m_pt_TexPos[TEXSET_CB_SCROLLDOWN_HIT].x2		= m_pt_TexPos[TEXSET_CB_SCROLLDOWN_HIT].x	+ Width;
				m_pt_TexPos[TEXSET_CB_SCROLLDOWN_PUSH].x2		= m_pt_TexPos[TEXSET_CB_SCROLLDOWN_PUSH].x	+ Width;
			}
			break;
		case SET_SPWINDOW_CB_SIZE_SCROLDOWN_Y:		
			{
				INT Height = m_pt_Size[OFFSET_CB_SCROLLDOWN].y = atoi(Value); 

				m_pt_TexPos[TEXSET_CB_SCROLLDOWN_ENABLE].y2	= m_pt_TexPos[TEXSET_CB_SCROLLDOWN_ENABLE].y	+ Height;
				m_pt_TexPos[TEXSET_CB_SCROLLDOWN_HIT].y2		= m_pt_TexPos[TEXSET_CB_SCROLLDOWN_HIT].y	+ Height;
				m_pt_TexPos[TEXSET_CB_SCROLLDOWN_PUSH].y2		= m_pt_TexPos[TEXSET_CB_SCROLLDOWN_PUSH].y	+ Height;

			}
			break;
		default:
			SPWindow_Base::Set(ID,Value);
		}
	}

	INT			m_flg_SetScroll;
	INT			m_flg_ShowEditBox;
	INT			m_flg_RenderItemCountMAX;
	INT			m_flg_ShowToolTip;

	s_Point4	m_pt_Offset[5];
	s_Point4	m_pt_TexPos[23];

	//- 데이터로 남길 정보만 따로 뽑아내서 정리 합니다....
	//---------------------------------------------------------------
	s_Point		m_pt_Size[5];
private:
protected:
};

#endif