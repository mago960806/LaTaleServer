
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPRefOLE
// Comment     : refer to "Embed an HTML control in your own window using plain C" by Jeff Glatt 
//					URL - http://www.codeproject.com/com/cwebpage.asp
// Creation    : metalgeni 2007-11-12 
//***************************************************************************




#include <string>

#include <assert.h>
#include <map>
#include <comutil.h>

#include <OleIdl.h>

#include <windows.h>
#include <ExDisp.h>
#include <mshtml.h>
#include <mshtmhst.h>
#include <oaidl.h>



#define NOTIMPLEMENTED assert(0); return(E_NOTIMPL)



class SPIOleInPlaceFrame
	: public IOleInPlaceFrame 
{
public:
	SPIOleInPlaceFrame();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, LPVOID FAR* ppvObj);
	virtual ULONG  STDMETHODCALLTYPE AddRef();
	virtual ULONG  STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE GetWindow( HWND FAR* lphwnd);
	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp( BOOL fEnterMode);
	virtual HRESULT STDMETHODCALLTYPE GetBorder( LPRECT lprectBorder);
	virtual HRESULT STDMETHODCALLTYPE RequestBorderSpace( LPCBORDERWIDTHS pborderwidths);
	virtual HRESULT STDMETHODCALLTYPE SetBorderSpace( LPCBORDERWIDTHS pborderwidths);
	virtual HRESULT STDMETHODCALLTYPE SetActiveObject( IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName);
	virtual HRESULT STDMETHODCALLTYPE InsertMenus( HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths);
	virtual HRESULT STDMETHODCALLTYPE SetMenu( HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject);
	virtual HRESULT STDMETHODCALLTYPE RemoveMenus( HMENU hmenuShared);
	virtual HRESULT STDMETHODCALLTYPE SetStatusText( LPCOLESTR pszStatusText);
	virtual HRESULT STDMETHODCALLTYPE EnableModeless( BOOL fEnable);
	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( LPMSG lpmsg, WORD wID);	

	void Init(HWND hWnd);

protected:
	HWND m_hWnd;
};



class SPIOleInPlaceSite
	: public IOleInPlaceSite
{
public:
	SPIOleInPlaceSite();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, void ** ppvObject);
	virtual ULONG  STDMETHODCALLTYPE AddRef();
	virtual ULONG  STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE GetWindow( HWND FAR* lphwnd);
	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp( BOOL fEnterMode);
	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate();
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate();
	virtual HRESULT STDMETHODCALLTYPE OnUIActivate();
	virtual HRESULT STDMETHODCALLTYPE GetWindowContext( LPOLEINPLACEFRAME FAR* lplpFrame,LPOLEINPLACEUIWINDOW FAR* lplpDoc,LPRECT lprcPosRect,LPRECT lprcClipRect,LPOLEINPLACEFRAMEINFO lpFrameInfo);
	virtual HRESULT STDMETHODCALLTYPE Scroll( SIZE scrollExtent);
	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate( BOOL fUndoable);
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate();
	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState();
	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo();
	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange( LPCRECT lprcPosRect);
	
	void Init(IOleInPlaceFrame* pOleInPlaceFrame, HWND hWnd);
	void SetBrowserObject	(IOleObject* pOleObj);
	void SetClientSite		(IOleClientSite* pOleClientSite);	


protected:
	IOleClientSite		*m_pOleClientSite;
	IOleInPlaceFrame	*m_pOleInPlaceFrame;
	IOleObject			*m_pBrowserObject;
	HWND                m_hWnd;
};


class SPIOleClientSite 
	: virtual public IOleClientSite
{
public:
	SPIOleClientSite();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, void ** ppvObject);
	virtual ULONG  STDMETHODCALLTYPE AddRef();
	virtual ULONG  STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE SaveObject();
	virtual HRESULT STDMETHODCALLTYPE GetMoniker( DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk);
	virtual HRESULT STDMETHODCALLTYPE GetContainer( LPOLECONTAINER FAR* ppContainer);
	virtual HRESULT STDMETHODCALLTYPE ShowObject();
	virtual HRESULT STDMETHODCALLTYPE OnShowWindow( BOOL fShow);
	virtual HRESULT STDMETHODCALLTYPE RequestNewObjectLayout();

	void Init(IOleInPlaceSite  *pInplaceSite, IDocHostUIHandler  *pDocHostUIHandler, DWebBrowserEvents2 *pDWebBrowserEvent2);

protected:
	IOleInPlaceSite       *m_pInplaceSite;
	IDocHostUIHandler     *m_pDocHostUIHandler;
	DWebBrowserEvents2    *m_pDWebBrowserEvent2;

}; 



class SPIStorage 
	: public virtual IStorage 
{
public:	

	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, LPVOID FAR* ppvObj);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE CreateStream( const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm);
	virtual HRESULT STDMETHODCALLTYPE OpenStream( const WCHAR * pwcsName, void *reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm);
	virtual HRESULT STDMETHODCALLTYPE CreateStorage( const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage **ppstg);
	virtual HRESULT STDMETHODCALLTYPE OpenStorage( const WCHAR * pwcsName, IStorage * pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg);
	virtual HRESULT STDMETHODCALLTYPE CopyTo( DWORD ciidExclude, IID const *rgiidExclude, SNB snbExclude,IStorage *pstgDest);
	virtual HRESULT STDMETHODCALLTYPE MoveElementTo( const OLECHAR *pwcsName,IStorage * pstgDest, const OLECHAR *pwcsNewName, DWORD grfFlags);
	virtual HRESULT STDMETHODCALLTYPE Commit( DWORD grfCommitFlags);
	virtual HRESULT STDMETHODCALLTYPE Revert();
	virtual HRESULT STDMETHODCALLTYPE EnumElements( DWORD reserved1, void * reserved2, DWORD reserved3, IEnumSTATSTG ** ppenum);
	virtual HRESULT STDMETHODCALLTYPE DestroyElement( const OLECHAR *pwcsName);
	virtual HRESULT STDMETHODCALLTYPE RenameElement( const WCHAR *pwcsOldName, const WCHAR *pwcsNewName);
	virtual HRESULT STDMETHODCALLTYPE SetElementTimes( const WCHAR *pwcsName, FILETIME const *pctime, FILETIME const *patime, FILETIME const *pmtime);
	virtual HRESULT STDMETHODCALLTYPE SetClass( REFCLSID clsid);
	virtual HRESULT STDMETHODCALLTYPE SetStateBits( DWORD grfStateBits, DWORD grfMask);
	virtual HRESULT STDMETHODCALLTYPE Stat( STATSTG * pstatstg, DWORD grfStatFlag);

};


