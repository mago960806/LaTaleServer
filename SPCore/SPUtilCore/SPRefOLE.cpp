
#include <ExDispId.h>

#include "SPRefOLE.h"



//////////////////////////////////////////////////////////////////////////
// SPIOleInPlaceFrame::

SPIOleInPlaceFrame::SPIOleInPlaceFrame()
: m_hWnd(NULL)
{}

void SPIOleInPlaceFrame::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::QueryInterface( REFIID riid, LPVOID FAR* ppvObj)
{
	NOTIMPLEMENTED;
}

ULONG STDMETHODCALLTYPE SPIOleInPlaceFrame::AddRef()
{
	return(1);
}

ULONG STDMETHODCALLTYPE SPIOleInPlaceFrame::Release()
{
	return(1);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::GetWindow( HWND FAR* lphwnd)
{	
	*lphwnd = m_hWnd;
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::ContextSensitiveHelp( BOOL fEnterMode)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::GetBorder( LPRECT lprectBorder)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::RequestBorderSpace( LPCBORDERWIDTHS pborderwidths)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::SetBorderSpace( LPCBORDERWIDTHS pborderwidths)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::SetActiveObject( IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::InsertMenus( HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::SetMenu( HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::RemoveMenus( HMENU hmenuShared)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::SetStatusText( LPCOLESTR pszStatusText)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::EnableModeless( BOOL fEnable)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceFrame::TranslateAccelerator( LPMSG lpmsg, WORD wID)
{
	NOTIMPLEMENTED;
}




//////////////////////////////////////////////////////////////////////////
// SPIOleInPlaceSite

SPIOleInPlaceSite::SPIOleInPlaceSite()
: m_pOleClientSite		(NULL)
, m_pOleInPlaceFrame	(NULL)
, m_pBrowserObject		(NULL)
, m_hWnd				(NULL)
{}

void SPIOleInPlaceSite::Init(IOleInPlaceFrame* pOleInPlaceFrame, HWND hWnd)
{
	m_pOleInPlaceFrame = pOleInPlaceFrame;
	m_hWnd = hWnd;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::QueryInterface( REFIID riid, void ** ppvObject)
{
	return m_pOleClientSite->QueryInterface(riid, ppvObject);
}

ULONG  STDMETHODCALLTYPE SPIOleInPlaceSite::AddRef()
{
	return 1;
}

ULONG  STDMETHODCALLTYPE SPIOleInPlaceSite::Release()
{
	return 1;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::GetWindow(HWND FAR* lphwnd)
{	
	*lphwnd = m_hWnd;
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::ContextSensitiveHelp( BOOL fEnterMode)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::CanInPlaceActivate()
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::OnInPlaceActivate()
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::OnUIActivate()
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::GetWindowContext( LPOLEINPLACEFRAME FAR* lplpFrame, LPOLEINPLACEUIWINDOW FAR* lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{	

	*lplpFrame = m_pOleInPlaceFrame;	
	*lplpDoc = 0;

	lpFrameInfo->fMDIApp = FALSE;	
	lpFrameInfo->hwndFrame = m_hWnd;
	lpFrameInfo->haccel = 0;
	lpFrameInfo->cAccelEntries = 0;

	// Give the browser the dimensions of where it can draw. We give it our entire window to fill
	//GetClientRect(lpFrameInfo->hwndFrame, lprcPosRect);
	//GetClientRect(lpFrameInfo->hwndFrame, lprcClipRect);

	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::Scroll( SIZE scrollExtent)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::OnUIDeactivate( BOOL fUndoable)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::OnInPlaceDeactivate()
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::DiscardUndoState()
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::DeactivateAndUndo()
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleInPlaceSite::OnPosRectChange( LPCRECT lprcPosRect)
{
	IOleInPlaceObject	*pInplace;
	if (!m_pBrowserObject->QueryInterface(IID_IOleInPlaceObject, (void**)&pInplace)) {
		pInplace->SetObjectRects(lprcPosRect, lprcPosRect);
	}
	return(S_OK);
}

void SPIOleInPlaceSite::SetBrowserObject(IOleObject* pOleObj)
{
	m_pBrowserObject = pOleObj;
}

void SPIOleInPlaceSite::SetClientSite(IOleClientSite* pOleClientSite)
{
	m_pOleClientSite = 	pOleClientSite;
}

//////////////////////////////////////////////////////////////////////////
// SPIOleClientSite

SPIOleClientSite::SPIOleClientSite()
: m_pInplaceSite(NULL)
, m_pDWebBrowserEvent2(NULL)
, m_pDocHostUIHandler(NULL)
{}

void SPIOleClientSite::Init(IOleInPlaceSite  *pInplaceSite, IDocHostUIHandler  *pDocHostUIHandler, DWebBrowserEvents2 *pDWebBrowserEvent2)
{
	m_pInplaceSite = pInplaceSite;
	m_pDWebBrowserEvent2 = pDWebBrowserEvent2;
	m_pDocHostUIHandler = pDocHostUIHandler; 
}

HRESULT STDMETHODCALLTYPE SPIOleClientSite::QueryInterface( REFIID riid, void ** ppvObject)
{
	if (IsEqualGUID(riid, IID_IUnknown) || IsEqualGUID(riid, IID_IOleClientSite)) 		
		*ppvObject = this;	
	else if (IsEqualGUID(riid, IID_IOleInPlaceSite)) 		
		*ppvObject = m_pInplaceSite;
	else if (IsEqualGUID(riid, IID_IDocHostUIHandler)) 		
		*ppvObject = m_pDocHostUIHandler;

	else if (IsEqualGUID(riid, DIID_DWebBrowserEvents2)) 
		*ppvObject = m_pDWebBrowserEvent2;

	else if (IsEqualGUID(riid, IID_IDispatch)) 
		*ppvObject = m_pDWebBrowserEvent2;

	else 
		*ppvObject = NULL;		
	
	if(*ppvObject == NULL)
		return(E_NOINTERFACE);

	return(S_OK);
	
}

ULONG STDMETHODCALLTYPE SPIOleClientSite::AddRef()
{
	return(1);
}

ULONG STDMETHODCALLTYPE SPIOleClientSite::Release()
{
	return(1);
}

HRESULT STDMETHODCALLTYPE SPIOleClientSite::SaveObject()
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleClientSite::GetMoniker( DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleClientSite::GetContainer( LPOLECONTAINER FAR* ppContainer)
{	
	*ppContainer = 0;
	return(E_NOINTERFACE);
}

HRESULT STDMETHODCALLTYPE SPIOleClientSite::ShowObject()
{
	return(NOERROR);
}

HRESULT STDMETHODCALLTYPE SPIOleClientSite::OnShowWindow( BOOL fShow)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIOleClientSite::RequestNewObjectLayout()
{
	NOTIMPLEMENTED;
}




//////////////////////////////////////////////////////////////////////////
// SPIStorage
HRESULT STDMETHODCALLTYPE SPIStorage::QueryInterface( REFIID riid, LPVOID FAR* ppvObj)
{
	NOTIMPLEMENTED;
}

ULONG STDMETHODCALLTYPE SPIStorage::AddRef()
{
	return(1);
}

ULONG STDMETHODCALLTYPE SPIStorage::Release()
{
	return(1);
}

HRESULT STDMETHODCALLTYPE SPIStorage::CreateStream( const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::OpenStream( const WCHAR * pwcsName, void *reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::CreateStorage( const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage **ppstg)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::OpenStorage( const WCHAR * pwcsName, IStorage * pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::CopyTo( DWORD ciidExclude, IID const *rgiidExclude, SNB snbExclude,IStorage *pstgDest)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::MoveElementTo( const OLECHAR *pwcsName,IStorage * pstgDest, const OLECHAR *pwcsNewName, DWORD grfFlags)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::Commit( DWORD grfCommitFlags)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::Revert()
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::EnumElements( DWORD reserved1, void * reserved2, DWORD reserved3, IEnumSTATSTG ** ppenum)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::DestroyElement( const OLECHAR *pwcsName)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::RenameElement( const WCHAR *pwcsOldName, const WCHAR *pwcsNewName)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::SetElementTimes( const WCHAR *pwcsName, FILETIME const *pctime, FILETIME const *patime, FILETIME const *pmtime)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::SetClass( REFCLSID clsid)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE SPIStorage::SetStateBits( DWORD grfStateBits, DWORD grfMask)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE SPIStorage::Stat( STATSTG * pstatstg, DWORD grfStatFlag)
{
	NOTIMPLEMENTED;
}


