// SPWindow_Static.h

#pragma once

#include "SPWindow_Base.h"

class SPWindow_Static : public SPWindow_Base
{
public:
	SPWindow_Static();
	virtual ~SPWindow_Static();
public:
	virtual void* Get(int ID)
	{
		switch(ID)
		{
		case GET_SPWINDOW_STATIC_NORMALBODY:	return &m_imgrc4_Body;

			// SPWindow_Base와 동일 기능.
			// GET_SPWINDOW_IMAGE_FILENAME
			// GET_SPWINDOW_IMAGE_X
			// GET_SPWINDOW_IMAGE_Y
			//----------------------------------------------
		case GET_SPWINDOW_STATIC_FILENAME:      return m_imgrc2_Texture.sz_FileName; 
		case GET_SPWINDOW_STATIC_X:				return &m_imgrc2_Texture.i_X;
		case GET_SPWINDOW_STATIC_Y:				return &m_imgrc2_Texture.i_Y;
		case GET_SPWINDOW_STATIC_TEXT:			return m_sz_Text;

			//----------------------------------------------

		default:
			return SPWindow_Base::Get(ID);
		}
	}

	virtual void Set(int ID,char* Value)
	{
		switch(ID)
		{
		case SET_SPWINDOW_STATIC_NORMALBODY_FILENAME:   lstrcpy(m_imgrc4_Body.FileName,Value);		break;
		case SET_SPWINDOW_STATIC_NORMALBODY_X:		    m_imgrc4_Body.x        = atoi(Value);		break;
		case SET_SPWINDOW_STATIC_NORMALBODY_Y:		    m_imgrc4_Body.y        = atoi(Value);		break;
		case SET_SPWINDOW_STATIC_NORMALBODY_CX:		    m_imgrc4_Body.cx       = atoi(Value);		break;
		case SET_SPWINDOW_STATIC_NORMALBODY_CY:		    m_imgrc4_Body.cy       = atoi(Value);		break;
		case SET_SPWINDOW_STATIC_TEXT:					lstrcpy(m_sz_Text,Value);					break;

		default:
			SPWindow_Base::Set(ID,Value);
		}
	}

	s_ImageRect4	m_imgrc4_Head;
	s_ImageRect4	m_imgrc4_Body;
	s_ImageRect4	m_imgrc4_Tail;
	CHAR			m_sz_Text[40];
protected:
private:
};

