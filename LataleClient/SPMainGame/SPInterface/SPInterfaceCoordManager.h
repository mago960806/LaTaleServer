// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2006-6-28 
//***************************************************************************

#pragma once


class SPInterfaceCoordManager {
public:	
	virtual ~SPInterfaceCoordManager();
	void Init();
	static SPInterfaceCoordManager* GetInstance();
	static void DelInstance();

	void SetDeault();
	void Load();
	void Save();

	bool GetWindowPT(IN INSTANCE_ID wiid, OUT POINT& pt);
	bool SetWindowPT(IN INSTANCE_ID wiid, IN POINT pt);

private:
	SPInterfaceCoordManager();

private:
	static SPInterfaceCoordManager* m_pInstance;
	std::map<INSTANCE_ID, POINT*> m_mStartPoint;

};



