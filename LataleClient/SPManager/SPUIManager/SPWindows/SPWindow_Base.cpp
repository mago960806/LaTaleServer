// c_Window_Base.cpp

#define RADIX_1         1
#define RADIX_2        16

#pragma warning (disable:4819)

#include "SPWindow_Base.h"

SPWindow_Base::SPWindow_Base()
{
	// REQ Save.
	//----------------------------------------------
	m_i_X				= 0;
	m_i_Y				= 0;
	m_i_CX				= 0;
	m_i_CY				= 0;
	m_i_WindowType		= 0;
	m_i_FontType		= 0;
	m_f_R				= 0.0f;
	m_f_G				= 0.0f;
	m_f_B				= 0.0f;
	m_f_A				= 0.0f;
	ZeroMemory(&m_imgrc2_Texture,sizeof(s_ImageRect2));
	ZeroMemory(m_FontColor,9);
	//---------------------------------------------- REQ Save end.
}


SPWindow_Base::~SPWindow_Base()
{
}


//- Protected:
//-------------------------------------------------------
//{
void SPWindow_Base::ExtractARGB()
{
	CString Value;

	Value += m_FontColor[0];
	Value += m_FontColor[1]; m_f_R =  ExtractColor(Value); Value = "";

	Value += m_FontColor[2];
	Value += m_FontColor[3]; m_f_G =  ExtractColor(Value); Value = "";

	Value += m_FontColor[4];
	Value += m_FontColor[5]; m_f_B =  ExtractColor(Value); Value = "";

	Value += m_FontColor[6];
	Value += m_FontColor[7]; m_f_A = 1.0f; // ExtractColor(Value); Value = "";
}


float SPWindow_Base::ExtractColor(CString rec_cstr_Value)
{

	INT		Radix_Unit[2]	= { RADIX_2,RADIX_1 };
	INT		cnt_Radix		= rec_cstr_Value.GetLength();
	INT		ConvertedValue  = 0;
	INT		RealVar         = 0;

	for(INT i = 0;i < cnt_Radix;++i)
	{
		CHAR Temp = rec_cstr_Value[i];

		if((INT)Temp > 57)
		{
			Temp	= toupper(Temp);
			RealVar = (INT)Temp - 55;
		}
		else
			RealVar = atoi(&Temp);

		ConvertedValue += (RealVar * Radix_Unit[i]);
	}

	return (float)((float)ConvertedValue / 255.0f);
}

//}

