// SPUIManager.cpp
//---------------------------------------------------
#pragma comment (lib,"MSXML2.lib")
#pragma comment (lib,"comsuppwd.lib")

#include <Windows.h>

//---------------------------------------------------
#include "SPResourceDEF.h"
#include "SPResourceBaseDEF.h"
#include "SPResourceBase.h"
#include "SPWindowDEF.h"
//---------------------------------------------------


// Windows.
//---------------------------------------------------
// Base.
//---------------------------------------------------
#include "SPWindow_Base.h"
//--------------------------------------------------- Base end.

// Common controls.
//---------------------------------------------------
#include "SPWindow_Static.h"
#include "SPWindow_Button.h"
#include "SPWindow_Edit.h"
#include "SPWindow_Gauge.h"
#include "SPWindow_DigitBoard.h"
#include "SPWindow_ComboBox.h"
//--------------------------------------------------- Conmmon controls end.


// Dialogs.
//---------------------------------------------------
#include "SPWindow_Root.h"
#include "SPWindow_Login.h"
#include "SPWindow_StatPallet.h"
#include "SPWindow_Worldmap.h"
#include "SPWindow_MiniMap.h"
//--------------------------------------------------- Dialogs end.
//--------------------------------------------------- Windows end.
#include "SPUIManager.h"

#include "SPUtil.h"
#include "SPStream.h"
#include "SPDebug.h"

// Static functions list.
//---------------------------------------------------
//--------------------------------------------------- Static function list end.



SPUIManager* g_UIManager = 0; // Global Interface.



// CON & DIS.
//---------------------------------------------------
SPUIManager::SPUIManager()
{
	CoInitialize(NULL);

	m_ixml_Doc          = 0;
	m_cnt_XMLDocs		= 0;
	m_flg_UIFile        = FLAGS_NOTLOADED;
}


SPUIManager::~SPUIManager()
{
	CoUninitialize();
}
//--------------------------------------------------- CON & DIS end.



// Public:
//--------------------------------------------------- 
bool SPUIManager::Load(CString rec_cstr_FileName)
{
	if(m_cnt_XMLDocs > 0)
	{
		DXUTOutputDebugString("SPUIManager::Load() - 한개 이상의 DOM을 생성하려고 합니다.");
		return false;
	}

	if(g_bUsePackFile)
	{
		SPStream*	strm_XML	= 0; bool flg_IsFileLoaded =  SPResourceBase::GetInstance()->GetStreamData(rec_cstr_FileName.GetBuffer(),&strm_XML);
		INT			FileLength	= 0;	
		CHAR*		XMLString	= 0;	

		if(strm_XML == 0 || flg_IsFileLoaded == false)
		{
			DXUTOutputDebugString("SPUIManager::Load() - '%s'화일이 없습니다.",rec_cstr_FileName.GetBuffer());
			return false;
		}

		
		FileLength	= strm_XML->Length();
		XMLString	= new CHAR[FileLength];

		strm_XML->Read(XMLString,FileLength);
		Init(XMLString,FileLength);

		SAFE_RELEASE(strm_XML);
		SAFE_DELETE(XMLString);
	}
	else
	{
		/*
		//SPStream*	strm_XML	= 0; SPResourceBase::GetInstance()->GetStreamData(rec_cstr_FileName.GetBuffer(),&strm_XML);
		SPFileStream*  strm_XML = new SPFileStream(CString(GetAddResPath(rec_cstr_FileName.GetBuffer(),RES_WORK_RESOURCE_PATH)));
		INT			FileLength	= 0;	
		CHAR*		XMLString	= 0;	

		FileLength	= strm_XML->Length();
		XMLString	= new CHAR[FileLength];

		strm_XML->Read(XMLString,FileLength);
		Init(XMLString,FileLength);

		SAFE_RELEASE(strm_XML);
		SAFE_DELETE(XMLString);
		*/
		

		Init(CString(GetAddResPath(rec_cstr_FileName,RES_WORK_RESOURCE_PATH)));
	}

	if(m_flg_UIFile == FLAGS_LOADED)
	{
		m_cnt_XMLDocs++;
		return true;
	}
	else 
	{
		DXUTOutputDebugString("SPUIManager::Load() - '%s'을(를) 읽는데 실패 했습니다.",rec_cstr_FileName.GetBuffer());
		return false;
	}
}


void SPUIManager::Release()
{
	ITR_MAP_SPWINDOW_BASE itr_SPWindow = m_map_SPWindow.begin();

	for(;itr_SPWindow != m_map_SPWindow.end();)
	{
		SAFE_DELETE(itr_SPWindow->second);

		itr_SPWindow = m_map_SPWindow.erase(itr_SPWindow);
	}

	m_map_SPWindow.		clear();
	m_map_DefSet.		clear();
	m_map_ClassID.		clear();
	m_map_InstanceID.	clear();
	
	SAFE_RELEASE(m_ixml_Doc); m_ixml_Doc = 0;

	
	if(m_flg_UIFile == FLAGS_LOADED)
	{
		m_flg_UIFile = FLAGS_NOTLOADED;
		m_cnt_XMLDocs--;
	}
}


void SPUIManager::Init(CString rec_cstr_Filename)
{
	CoCreateInstance(CLSID_FreeThreadedDOMDocument,NULL,CLSCTX_INPROC_SERVER,IID_IXMLDOMDocument,(void**)&m_ixml_Doc);

	if(m_ixml_Doc == 0)
	{
		DXUTOutputDebugString("SPUIManager::Init() - XML인스턴스를 생성하지 못했습니다.\n");
		return;
	}

	variant_t	 var_Filename(rec_cstr_Filename.GetBuffer());
	VARIANT_BOOL var_Out		= VARIANT_FALSE;
	VARIANT_BOOL var_b_Async    = VARIANT_FALSE;

	m_ixml_Doc->load(var_Filename,&var_Out);
	m_ixml_Doc->put_async(var_b_Async);

	if(var_Out == VARIANT_TRUE)
	{
		LoadAttrName();
		LoadClassID();
		LoadInstanceID();
		LoadWindows();

		m_flg_UIFile = FLAGS_LOADED; 
	}
	else
	{
		m_ixml_Doc->Release();
		m_flg_UIFile = FLAGS_NOTLOADED; 
	}
}


void SPUIManager::Init(CHAR* rec_sz_XMLString,INT rec_i_Length) //- 한곳에서만 리소스를 위한 메모리 할당을 시도하도록 수정합니다... 부모호출쪽 확인할것....
{
	// Remarks.
	//--------------------------------------------------------------------
	// Calling load or loadXML on an existing document immediately discards the content of the document.
	// The loadXML() method will work only with UTF-16 or UCS-2 encodings.
	//--------------------------------------------------------------------
	m_ixml_Doc = 0;

	CoCreateInstance(CLSID_FreeThreadedDOMDocument,NULL,CLSCTX_INPROC_SERVER,IID_IXMLDOMDocument,(void**)&m_ixml_Doc);

	if(m_ixml_Doc == 0)
	{
		DXUTOutputDebugString("SPUIManager::Init() - XML인스턴스를 생성하지 못했습니다.");
		return;
	}
    

	BSTR bstr_XML = SysAllocStringByteLen(&rec_sz_XMLString[2],rec_i_Length - 2); //<-- (FF FE)Have to be removed in stream.
	//BSTR bstr_XML = SysAllocStringByteLen(rec_sz_XMLString,rec_i_Length); //<-- (FF FE)Have to be removed in stream.


	VARIANT_BOOL var_Out			= VARIANT_FALSE;
	VARIANT_BOOL var_b_Async		= VARIANT_FALSE;
	VARIANT_BOOL var_b_Validation	= VARIANT_FALSE;

	m_ixml_Doc->put_validateOnParse(var_b_Validation);
	m_ixml_Doc->put_async(var_b_Async);
	m_ixml_Doc->loadXML(bstr_XML,&var_Out);

	if(var_Out == VARIANT_TRUE)
	{
		LoadAttrName();
		LoadClassID();
		LoadInstanceID();
		LoadWindows();

		m_flg_UIFile = FLAGS_LOADED; 
	}
	else
	{
		m_ixml_Doc->Release();
		m_flg_UIFile = FLAGS_NOTLOADED; 
	}

	SysFreeString(bstr_XML);
}


SPWindow_Base* SPUIManager::GetWindow(INT rec_cstr_InstanceID)
{
	ITR_MAP_SPWINDOW_BASE itr_Window = m_map_SPWindow.find(rec_cstr_InstanceID);

	if(itr_Window == m_map_SPWindow.end()) 
	{
		DXUTOutputDebugString("UI_SPWINDOW_DEFAULT_BASE - 로딩실패 : (%d)해당되는 윈도우가 없습니다.\n",rec_cstr_InstanceID);		
		return 0;
	}
	else
	{
		return itr_Window->second;
	}
}


SPUIManager::SPUIMGR_FLAGS SPUIManager::GetFlags(SPUIMGR_FLAGS rec_flg_Section)
{
	switch(rec_flg_Section)
	{
	case FLAGS_SEC_UIFILE:	return m_flg_UIFile;
	default:
		return FLAGS_ERROR; 
	}
}


void SPUIManager::SetFlags(SPUIMGR_FLAGS rec_flg_Section,SPUIMGR_FLAGS rec_flg_Value)
{
	switch(rec_flg_Section)
	{
	case FLAGS_SEC_UIFILE:	m_flg_UIFile = rec_flg_Value; break;
	default:
		break; 
	}
}
//---------------------------------------------------------------------- Public: end.



// Protected:
//---------------------------------------------------------------------- 
void SPUIManager::LoadAttrName()
{
	LONG					cnt_Node    = 0;
	LONG					cnt_Attr	= 0;
	CONST LONG				ATTR_NAME   = 0;
	CONST LONG				ATTR_VALUE  = 1;
	
	CString					xpath("//ATTR");
	BSTR					bstr_xpath = xpath.AllocSysString();

	IXMLDOMNodeList*		ndlst_AttrID;
	IXMLDOMNode*			nd_AttrID;
	IXMLDOMNamedNodeMap*	nd_map_AttrID;

	m_ixml_Doc->selectNodes(bstr_xpath,&ndlst_AttrID);  SysFreeString(bstr_xpath);
	ndlst_AttrID->get_length(&cnt_Node);
    	
	for(INT idx_Node = 0;idx_Node < cnt_Node;++idx_Node)
	{
		LONG	l_Value	= 0;
		CString	cstr_ID("");

		ndlst_AttrID	->get_item(idx_Node,&nd_AttrID);
		nd_AttrID		->get_attributes(&nd_map_AttrID);
		nd_map_AttrID	->get_length(&cnt_Attr);

		for(INT idx_Attr = 0;idx_Attr < cnt_Attr;++idx_Attr)
		{
			IXMLDOMNode* nd_Temp;
			VARIANT      var_Temp;

			nd_map_AttrID->get_item(idx_Attr,&nd_Temp);
			nd_Temp->get_nodeValue(&var_Temp);

			CString cstr_Got = (CString)var_Temp;

			if(idx_Attr == ATTR_NAME ) cstr_ID = cstr_Got;						else
			if(idx_Attr == ATTR_VALUE) l_Value = atoi(cstr_Got.GetBuffer());

			nd_Temp->Release();
		}

		m_map_DefSet.insert(make_pair(cstr_ID,l_Value));

		nd_AttrID->Release();
		nd_map_AttrID->Release();
	}

	ndlst_AttrID->Release();
}


void SPUIManager::LoadClassID()
{
	LONG					cnt_Node		= 0;
	LONG					cnt_Attr		= 0;
	CONST LONG				ATTR_CLASSNAME  = 0;
	CONST LONG				ATTR_CLASSID	= 1;

	CString					xpath("//CLASSID");
	BSTR					bstr_xpath = xpath.AllocSysString();

	IXMLDOMNodeList*		ndlst_ClassID;
	IXMLDOMNode*			nd_AttrID;
	IXMLDOMNamedNodeMap*	nd_map_AttrID;

	m_ixml_Doc->selectNodes(bstr_xpath,&ndlst_ClassID);  SysFreeString(bstr_xpath);
	ndlst_ClassID->get_length(&cnt_Node);

	for(INT idx_Node = 0;idx_Node < cnt_Node;++idx_Node)
	{
		LONG	l_Value	= 0;
		CString	cstr_ID("");

		ndlst_ClassID	->get_item(idx_Node,&nd_AttrID);
		nd_AttrID		->get_attributes(&nd_map_AttrID);
		nd_map_AttrID	->get_length(&cnt_Attr);

		for(INT idx_Attr = 0;idx_Attr < cnt_Attr;++idx_Attr)
		{
			IXMLDOMNode* nd_Temp;
			VARIANT      var_Temp;

			nd_map_AttrID->get_item(idx_Attr,&nd_Temp);
			nd_Temp->get_nodeValue(&var_Temp);

			CString cstr_Got = (CString)var_Temp;

			if(idx_Attr == ATTR_CLASSNAME) cstr_ID = cstr_Got; else
			if(idx_Attr == ATTR_CLASSID)   l_Value = ConvertToDec(cstr_Got); 

			nd_Temp->Release();
		}

		m_map_ClassID.insert(make_pair(cstr_ID,l_Value));

		nd_AttrID->Release();
		nd_map_AttrID->Release();
	}

	ndlst_ClassID->Release();
}


void SPUIManager::LoadInstanceID()
{
	LONG					cnt_Node			= 0;
	LONG					cnt_Attr			= 0;
	CONST LONG				ATTR_INSTANCENAME	= 0;
	CONST LONG				ATTR_INSTANCEID		= 1;

	CString					xpath("//INSTANCEID");
	BSTR					bstr_xpath = xpath.AllocSysString();

	IXMLDOMNodeList*		ndlst_ClassID;
	IXMLDOMNode*			nd_AttrID;
	IXMLDOMNamedNodeMap*	nd_map_AttrID;

	m_ixml_Doc->selectNodes(bstr_xpath,&ndlst_ClassID);  SysFreeString(bstr_xpath);
	ndlst_ClassID->get_length(&cnt_Node);

	for(INT idx_Node = 0;idx_Node < cnt_Node;++idx_Node)
	{
		LONG	l_Value	= 0;
		CString	cstr_ID("");

		ndlst_ClassID	->get_item(idx_Node,&nd_AttrID);
		nd_AttrID		->get_attributes(&nd_map_AttrID);
		nd_map_AttrID	->get_length(&cnt_Attr);

		for(INT idx_Attr = 0;idx_Attr < cnt_Attr;++idx_Attr)
		{
			IXMLDOMNode* nd_Temp;
			VARIANT      var_Temp;

			nd_map_AttrID->get_item(idx_Attr,&nd_Temp);
			nd_Temp->get_nodeValue(&var_Temp);

			CString cstr_Got = (CString)var_Temp;

			if(idx_Attr == ATTR_INSTANCENAME) cstr_ID = cstr_Got; else
			if(idx_Attr == ATTR_INSTANCEID)   l_Value = ConvertToDec(cstr_Got); 

			nd_Temp->Release();
		}

		m_map_InstanceID.insert(make_pair(cstr_ID,l_Value));

		if(l_Value == 0x30019524)
		{
			int Debug = 0;
		}

		nd_AttrID->Release();
		nd_map_AttrID->Release();
	}

	ndlst_ClassID->Release();
}


void SPUIManager::LoadWindows()
{
	LONG					cnt_Window		= 0;
	LONG					cnt_Attr_Window = 0;
	LONG                    cnt_Attr_Sub    = 0;
	CString					xpath_Window("//WINDOW");
	BSTR					bstr_xpath_Window = xpath_Window.AllocSysString();
	CONST LONG				ATTR_CLASSID	= 0;
	CONST LONG				ATTR_INSTANCEID	= 1;


	IXMLDOMNodeList*		ndlst_Window;
	IXMLDOMNode*			nd_Window;
	IXMLDOMNode*			nd_SubAttr;
	IXMLDOMNamedNodeMap*	ndmap_Window;
	IXMLDOMNamedNodeMap*	ndmap_SubAttr;

	m_ixml_Doc->selectNodes(bstr_xpath_Window,&ndlst_Window); SysFreeString(bstr_xpath_Window);
	ndlst_Window->get_length(&cnt_Window);

	for(INT idx_Window = 0;idx_Window < cnt_Window;++idx_Window)
	{
		CString			cstr_ClassID("");
		CString			cstr_InstanceID("");
		CString			cstr_AttrName("");
		CString			cstr_Value("");	
		SPWindow_Base*	spwnd_Base		= 0;
		
		ndlst_Window	->get_item(idx_Window,&nd_Window);
		nd_Window		->get_attributes(&ndmap_Window);
		ndmap_Window	->get_length(&cnt_Attr_Window);
		
		for(INT idx_Attr_Window = 0;idx_Attr_Window < cnt_Attr_Window;++idx_Attr_Window)
		{
			IXMLDOMNode* nd_Temp;
			VARIANT      var_Temp;

			ndmap_Window->get_item(idx_Attr_Window,&nd_Temp);
			nd_Temp		->get_nodeValue(&var_Temp);

			CString cstr_Got = (CString)var_Temp;

			if(idx_Attr_Window == ATTR_CLASSID)		cstr_ClassID	= cstr_Got; else
			if(idx_Attr_Window == ATTR_INSTANCEID)	cstr_InstanceID	= cstr_Got;	
			
			nd_Temp->Release();
        }

		
		ITR_MAP_INT	itr_ClassID = m_map_ClassID.find(cstr_ClassID);

		if(itr_ClassID != m_map_ClassID.end()) 	
		{
            spwnd_Base = GetMatchedWindow(static_cast<INT>(itr_ClassID->second));
			if (!spwnd_Base)
				continue;
		}


		// 부가 속성 정보 엘리먼트.
		// 모든 속성이 하나의 엘리먼트로 통합 됐습니다. 순서와 상관 없이 삽입가능 합니다.
		//-------------------------------------------------
		nd_Window		->get_firstChild(&nd_SubAttr);
		nd_SubAttr		->get_attributes(&ndmap_SubAttr);
		ndmap_SubAttr	->get_length(&cnt_Attr_Sub);

	
		for(INT idx_Attr_Sub = 0;idx_Attr_Sub < cnt_Attr_Sub;++idx_Attr_Sub)
		{
			IXMLDOMNode*	nd_Temp;
			BSTR			bstr_NodeName;
			VARIANT			var_Temp;

			ndmap_SubAttr->get_item(idx_Attr_Sub,&nd_Temp);
			
			nd_Temp->get_nodeName(&bstr_NodeName);	cstr_AttrName	= BSTRToCString(bstr_NodeName);
			nd_Temp->get_nodeValue(&var_Temp);		cstr_Value		= (CString)var_Temp;

			
			ITR_MAP_INT itr_AttrDEF
							= m_map_DefSet.find(cstr_AttrName);

			if(itr_AttrDEF == m_map_DefSet.end()) 
			{
			#ifdef _DEBUG
				assert("해당되는 속성 아이디가 없습니다.");
			#endif
				nd_Temp->Release();
				return;
			}

			spwnd_Base->Set(itr_AttrDEF->second,cstr_Value.GetBuffer());

			nd_Temp->Release();
		}
		//------------------------------------------------- 부가 속성 정보 엘리먼트 end.


		//- 현재 선택된 윈도우를 컨테이너에 저장합니다.
		//-------------------------------------------------
		ITR_MAP_INT itr_InsID =  m_map_InstanceID.find(cstr_InstanceID);
		
		

		if(itr_InsID != m_map_InstanceID.end())
		{
			m_map_SPWindow.insert(make_pair(static_cast<INT>(itr_InsID->second),spwnd_Base)); 
		}
		//------------------------------------------------- 컨테이너 저장 끝.

		nd_Window->Release();
		nd_SubAttr->Release();

		ndmap_Window->Release();
		ndmap_SubAttr->Release();
	}

	ndlst_Window->Release();
}


SPWindow_Base* SPUIManager::GetMatchedWindow(INT rec_i_ClassID)
{
	switch(rec_i_ClassID)
	{
	case WNDID_INVALID:
	case WNDID_CTRL_NULL: 
	case WNDID_CTRL_BUTTON_CHECK:
	case WNDID_CTRL_LISTBOX:
	case WNDID_TOOLTIP:
	case WNDID_HELP:
	case WNDID_SHOPTALK:
	case WNDID_MENU: 
		return (new SPWindow_Base());
		//-------------------------------------------------------- 여기까지 별다른 속성이 필요없는 클래스가 필요없는  윈도우를 만들어 줍니다.
		//                                                         아래는 특수화된 윈도우를 만듭니다.
	case WNDID_CTRL_BUTTON:			return (new SPWindow_Button());
	case WNDID_CTRL_STATIC:			return (new SPWindow_Static());
	case WNDID_CTRL_EDIT:			return (new SPWindow_Edit());
	case WNDID_CTRL_DIGITBOARD:     return (new SPWindow_DigitBoard());
	case WNDID_CTRL_GAUGE:          return (new SPWindow_Gauge());
	case WNDID_CTRL_COMBOBOX:		return (new SPWindow_ComboBox());
	
	case WNDID_ROOT:				return (new SPWindow_Root());
	case WNDID_LOGIN:				return (new SPWindow_Login());

	case WNDID_CENTERSTATUS:		return (new SPWindow_StatPallet());
	case WNDID_MINIMAP:				return (new SPWindow_MiniMap());
	case WNDID_WORLDMAP:			return (new SPWindow_WorldMap());
		

	default:
		#ifdef _DEBUG
		assert(0 && "해당되는 윈도우가 없습니다");
		#endif
		break;
	}
	return 0;
}


INT SPUIManager::ConvertToDec(CString rec_cstr_ToConvert)
{
	try	
	{
		int result = 0; 
		result = (int)(::strtol( (const char*)rec_cstr_ToConvert.GetString(), NULL, 16 ));
		return result;
	}
	catch(...)
	{
		assert( false && "ConverToDec Failed" );
		return 0;
	}
}


CString SPUIManager::BSTRToCString(BSTR rec_bstr)
{
	CHAR* pbstr;

	USES_CONVERSION;		//convert를 위한 매크로
	pbstr=OLE2A(rec_bstr);	//bstr을 CHAR로 convert
	
	return CString(pbstr);
} 
//---------------------------------------------------------------------- Protected: end.








