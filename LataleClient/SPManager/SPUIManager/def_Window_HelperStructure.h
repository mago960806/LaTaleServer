// def_Window_HelperStructure.h
//--------------------------------------------------------
//Info 
// - Administrator: Á¶ÁØ¿µ.
//--------------------------------------------------------

#pragma once

#define HLP_MAX_STR_LENGTH 50

struct s_Point
{
	int x;
	int y;
};

struct s_Point4
{
	int x;
	int y;
	int x2;
	int y2;
};


struct s_WindowPosBuffer
{
	int x;
	int y;
	int MouseX;
	int MouseY;
};

struct s_WindowPosBufferExt
{
	int x;
	int y;
	int cx;
	int cy;
	int MouseX;
	int MouseY;
};

struct s_ImageRect2
{
	int  i_X,i_Y;
	char sz_FileName[HLP_MAX_STR_LENGTH];
};


struct s_ImageRect4
{
	int  x,y,cx,cy;
	char FileName[HLP_MAX_STR_LENGTH];
};

