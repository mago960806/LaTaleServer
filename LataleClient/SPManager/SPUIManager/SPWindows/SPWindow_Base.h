// SPWindow_Base.h

#pragma warning (disable:4065)

#pragma once

#include <ATLSTR.h>
#include <Windows.h>
#include "def_Window_Key.h"
#include "def_Window_HelperStructure.h"

class SPWindow_Base
{
public:
	SPWindow_Base();
	virtual ~SPWindow_Base();

protected:
	void	ExtractARGB();	
	float	ExtractColor(CString rec_cstr_Value);

public:
	virtual void*	Get(int ID)
	{
		switch(ID)
		{
		case GET_SPWINDOW_WINDOWTYPE:       return &m_i_WindowType;
		case GET_SPWINDOW_IMAGE_FILENAME:	return m_imgrc2_Texture.sz_FileName;
		case GET_SPWINDOW_IMAGE_X:			return &m_imgrc2_Texture.i_X;
		case GET_SPWINDOW_IMAGE_Y:			return &m_imgrc2_Texture.i_Y;
		case GET_SPWINDOW_X:				return &m_i_X;
		case GET_SPWINDOW_Y:				return &m_i_Y;
		case GET_SPWINDOW_CX:				return &m_i_CX;
		case GET_SPWINDOW_CY:				return &m_i_CY;
		case GET_SPWINDOW_FONTTYPE:			return &m_i_FontType;
		case GET_SPWINDOW_FONTSIZE:			return &m_i_FontSize;
		case GET_SPWINDOW_FONT_R:			return &m_f_R;
		case GET_SPWINDOW_FONT_G:			return &m_f_G;
		case GET_SPWINDOW_FONT_B:			return &m_f_B;
		case GET_SPWINDOW_FONT_A:			return &m_f_A;
		case GET_SPWINDOW_ALIGN:            return &m_i_Align;
		case GET_SPWINDOW_REALSIZE_X:		return &m_i_RealSizeX;	
		case GET_SPWINDOW_REALSIZE_Y:		return &m_i_RealSizeY;	
			//case GET_SPWINDOW_FONTCOLOR:		return &m_d3dcolor_Font;
		default:
			return 0;
		}
	}

	virtual void	Set(int ID,char* Value)
	{
		switch(ID)
		{
		case SET_SPWINDOW_WINDOWTYPE:		m_i_WindowType			= atoi(Value);			break;
		case SET_SPWINDOW_IMAGE_FILENAME:	lstrcpy(m_imgrc2_Texture.sz_FileName,Value);	break;
		case SET_SPWINDOW_IMAGE_X:			m_imgrc2_Texture.i_X	= atoi(Value);			break;
		case SET_SPWINDOW_IMAGE_Y:			m_imgrc2_Texture.i_Y	= atoi(Value);			break;
		case SET_SPWINDOW_X:				m_i_X					= atoi(Value);			break;
		case SET_SPWINDOW_Y:				m_i_Y					= atoi(Value);			break;
		case SET_SPWINDOW_CX:				m_i_CX					= atoi(Value);			break; 
		case SET_SPWINDOW_CY:				m_i_CY					= atoi(Value);			break;
		case SET_SPWINDOW_FONTTYPE: 		m_i_FontType			= atoi(Value);			break;
		case SET_SPWINDOW_FONTSIZE: 		m_i_FontSize			= atoi(Value);			break;
		case SET_SPWINDOW_FONT_R:			m_f_R					= (float)atof(Value);	break;
		case SET_SPWINDOW_FONT_G:			m_f_G					= (float)atof(Value);	break;
		case SET_SPWINDOW_FONT_B:			m_f_B					= (float)atof(Value);	break;
		case SET_SPWINDOW_FONT_A:			m_f_A					= (float)atof(Value);	break;
		case SET_SPWINDOW_ALIGN:			m_i_Align				= atoi(Value);			break;
		case SET_SPWINDOW_REALSIZE_X:		m_i_RealSizeX			= atoi(Value);			break;
		case SET_SPWINDOW_REALSIZE_Y:		m_i_RealSizeY			= atoi(Value);			break;
		case SET_SPWINDOW_FONTCOLOR:
			{
				lstrcpy(m_FontColor,Value);		

				ExtractARGB();
			}
			break;
		default:
			break;
		}
	}
protected:
	// XML.
	//----------------------------------------------
	INT             m_i_InstanceID;
	INT             m_i_ClassID;
	INT				m_i_WindowType;
	INT             m_i_Movable;
	s_ImageRect2    m_imgrc2_Texture;
	INT				m_i_X;
	INT				m_i_Y;
	INT				m_i_CX; 
	INT				m_i_CY;
	INT				m_i_FontType;
	INT             m_i_FontSize;
	FLOAT			m_f_R;
	FLOAT			m_f_G;
	FLOAT			m_f_B;
	FLOAT			m_f_A;
	INT             m_i_Align;
	INT				m_i_RealSizeX;
	INT				m_i_RealSizeY;
	CHAR			m_FontColor[9];
	//---------------------------------------------- XML end.

private:
};


