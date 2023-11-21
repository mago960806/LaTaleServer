// SPUIManager.h
//--------------------------------------------------------
//Info 
// - Administrator: 조준영.
//--------------------------------------------------------

#ifndef _SPUIManager_h
#define _SPUIManager_h

#include <ATLSTR.h>
#include <MsXml2.h>
#include <COMUtil.h>
#include <Map>

#include "def_Window_HelperStructure.h"
#include "def_Window_Key.h"
#include "SPWindow_Base.h"
#include "SPUIManager_MacroPack.h"

using std::map;
using std::make_pair;

class SPUIManager
{
public:
	enum SPUIMGR_FLAGS			
	{ 
		FLAGS_ERROR, 

		FLAGS_SEC_UIFILE,
		FLAGS_NOTLOADED,
		FLAGS_LOADED,		

		FLAGS_SEC_RESERVED,
		FLAGS_RESERVED_0,
		FLAGS_RESERVED_1	
	};

	TYPEDEF map<CString,INT>::iterator		
								ITR_MAP_INT;
	TYPEDEF map<INT,SPWindow_Base*>::iterator
								ITR_MAP_SPWINDOW_BASE;


								SPUIManager();
								~SPUIManager();

	VOID						Init(CString rec_cstr_Filename);
	VOID						Init(CHAR* rec_sz_XMLString,INT rec_i_Length);
	SPWindow_Base*				GetWindow(INT rec_cstr_InstanceID);
	SPUIMGR_FLAGS               GetFlags(SPUIMGR_FLAGS rec_flg_Section);
	VOID                        SetFlags(SPUIMGR_FLAGS rec_flg_Section,SPUIMGR_FLAGS rec_flg_Value);
	
	
    //- 분화작업을 위해 추가된 함수들. 
	//----------------------------------------------------
	//> Date : 09/30/2008 
	//> 1. XML별로 로딩 할수 있게 끔 합니다.
	//{
	bool						Load(CString rec_cstr_FileName);
	void						Release();
	//}
protected:
	int                         m_cnt_XMLDocs; //- 실행 시간중 단 하나만 존재 해야 합니다.
	IXMLDOMDocument*			m_ixml_Doc;
	SPUIMGR_FLAGS               m_flg_UIFile;

	map<CString,INT>			m_map_ClassID;
	map<CString,INT>			m_map_InstanceID;
	map<CString,INT>            m_map_DefSet;
	map<INT,SPWindow_Base*>     m_map_SPWindow;
	 
	VOID						LoadAttrName();
	VOID						LoadClassID();
	VOID						LoadInstanceID();
	VOID						LoadWindows();
	SPWindow_Base*				GetMatchedWindow(INT rec_i_ClassID);
	INT							ConvertToDec(CString rec_cstr_ToConvert);
	CString						BSTRToCString(BSTR bstr);
private:
};

extern SPUIManager* g_UIManager;

#endif