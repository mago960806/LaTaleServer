// SPWindow_Button.h

#pragma once

#include "SPWindow_Base.h"

class SPWindow_Button : public SPWindow_Base
{
public:
	SPWindow_Button();
	~SPWindow_Button();

	virtual void*	Get(int ID)
	{
		switch(ID)
		{
		case GET_SPWINDOW_BUTTON_FILENAME_DISABLE:		return m_cstr_Disable;
		case GET_SPWINDOW_BUTTON_FILENAME_HIT:			return m_cstr_Hit;
		case GET_SPWINDOW_BUTTON_FILENAME_PUSH:			return m_cstr_Push;
		case GET_SPWINDOW_BUTTON_FILENAME_CHECK:		return m_cstr_Check;

		case GET_SPWINDOW_BUTTON_DISABLE_X:				return &m_i_DisableX;
		case GET_SPWINDOW_BUTTON_DISABLE_Y:				return &m_i_DisableY;
		case GET_SPWINDOW_BUTTON_HIT_X:					return &m_i_HitX;
		case GET_SPWINDOW_BUTTON_HIT_Y:					return &m_i_HitY;
		case GET_SPWINDOW_BUTTON_PUSH_X:				return &m_i_PushX;
		case GET_SPWINDOW_BUTTON_PUSH_Y:				return &m_i_PushY;
		case GET_SPWINDOW_BUTTON_CHECK_X:				return &m_i_CheckX;
		case GET_SPWINDOW_BUTTON_CHECK_Y:				return &m_i_CheckY;


		default:
			return SPWindow_Base::Get(ID);
		}
	}

	virtual void Set(int ID,char* Value)
	{
		switch(ID)
		{
		case SET_SPWINDOW_BUTTON_FILENAME_DISABLE:		lstrcpy(m_cstr_Disable,	 Value); break;
		case SET_SPWINDOW_BUTTON_FILENAME_HIT:			lstrcpy(m_cstr_Hit,		 Value); break;
		case SET_SPWINDOW_BUTTON_FILENAME_PUSH:			lstrcpy(m_cstr_Push,     Value); break;
		case SET_SPWINDOW_BUTTON_FILENAME_CHECK:		lstrcpy(m_cstr_Check,    Value); break;

		case SET_SPWINDOW_BUTTON_DISABLE_X:				m_i_DisableX	= atoi(Value); break;
		case SET_SPWINDOW_BUTTON_DISABLE_Y:				m_i_DisableY	= atoi(Value); break;
		case SET_SPWINDOW_BUTTON_HIT_X:					m_i_HitX		= atoi(Value); break;
		case SET_SPWINDOW_BUTTON_HIT_Y:					m_i_HitY		= atoi(Value); break;
		case SET_SPWINDOW_BUTTON_PUSH_X:				m_i_PushX		= atoi(Value); break;
		case SET_SPWINDOW_BUTTON_PUSH_Y:				m_i_PushY		= atoi(Value); break;
		case SET_SPWINDOW_BUTTON_CHECK_X:				m_i_CheckX		= atoi(Value); break;
		case SET_SPWINDOW_BUTTON_CHECK_Y:				m_i_CheckY		= atoi(Value); break;
		default:
			SPWindow_Base::Set(ID,Value);
		}
	}


protected:
	int		m_i_DisableX,m_i_DisableY; 
	int		m_i_HitX	,m_i_HitY;
	int		m_i_PushX	,m_i_PushY;
	int		m_i_CheckX  ,m_i_CheckY;


	char	m_cstr_Check[HLP_MAX_STR_LENGTH];
	char	m_cstr_Disable[HLP_MAX_STR_LENGTH];
	char	m_cstr_Hit[HLP_MAX_STR_LENGTH];
	char	m_cstr_Push[HLP_MAX_STR_LENGTH];
protected:
private:
};

