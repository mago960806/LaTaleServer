// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 ¿ÀÀü 10:26:19
//***************************************************************************


#pragma once

class SPWindow;
class SPWindowDigitBoard :	public SPWindow {
public:
	SPWindowDigitBoard(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent);
	~SPWindowDigitBoard();

	virtual void Init();
	virtual void Clean();	
	virtual void Process(float fTime);	
	virtual void Render(float fTime);
	virtual void RenderText();

	virtual void SetWindowText(const char* pStr = NULL);
	virtual void RefreshRelationCoord();	

	bool SetDigit(const char* pszImage, char cDigit, RECT rcSrc);
	void Refresh();	
	void ReposOwnImage();

protected:
	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	// {{ Message Map	
	int OnCursorEnter	( WPARAM, LPARAM );
	int OnCursorOut		( WPARAM, LPARAM );			
	int OnWheelUp		( WPARAM wParam, LPARAM lParam);		//[2005/9/12]
	int OnWheelDown		( WPARAM wParam, LPARAM lParam);		//[2005/9/12]
	// }}

protected:
	struct ImageDigitboard {
		SPTexture*	pImage;
		RECT		rcSrc;
		ImageDigitboard()
			: pImage(NULL) 
		{
			ZeroMemory(&rcSrc, sizeof(RECT));
		}			 
	};

	struct ResultInstance {
		ImageDigitboard* pImageDigit;
		RECT rcTar;
	};

	std::map< char, ImageDigitboard > m_mImageList; 
	std::vector<ResultInstance> m_vResult;

};
