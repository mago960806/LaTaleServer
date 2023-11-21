// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

class SPFXModel;

enum MODEL_VALUE_TYPE;

class SPFXModelFrameAnimation : public SPFXModel
{
public:	
	SPFXModelFrameAnimation				();
	virtual ~SPFXModelFrameAnimation	();

	virtual void	SetNull				();
	virtual void	Init				();
	virtual void	Destroy				();
	virtual bool	SetImage			( SPTexture** ppImage );
	virtual LRESULT	GetMemberValue		( MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0 );

protected:

	SPTexture*							m_pFrameImage;
	std::vector<SPFXFrameData*>			m_vpFrameData;

	int									m_iNoFrame;
	BOOL								m_BContinue;

};
