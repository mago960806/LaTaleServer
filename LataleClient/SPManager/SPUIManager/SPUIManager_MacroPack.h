// SPUIManager_MacroPack.h
//--------------------------------------------------------
//Info 
// - Administrator: 조준영.
//--------------------------------------------------------

#ifndef SPUIManager_h
#define SPUIManager_h

//-	모든 매크로를 해당 클래스안으로 옮깁니다.... 오직 생성만 책입집니다...
//-----------------------------------------------------------------------
#include "SPDebug.h"


static CHAR				Buffer_INT[4];
static DWORD			WrittenByteSize;

#define RADIX_1         1
#define RADIX_2        16
#define RADIX_3	      256
#define RADIX_4	     4096
#define RADIX_5	    65536
#define RADIX_6	  1048576
#define RADIX_7	 16777216
#define RADIX_8	268435456

#ifndef VOID
typedef void VOID;
#endif

#ifndef INT
typedef int INT;
#endif

#ifndef CHAR	
typedef char CHAR;
#endif

#ifndef TYPEDEF	
#define TYPEDEF typedef
#endif;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif


#define UI_INT(Attr,Option)		   *(int*)(##Attr->Get(Option))
#define UI_STRING(Attr,Option)	   (char*)(##Attr->Get(Option))
#define UI_FLOAT(Attr,Option)	   *(float*)(##Attr->Get(Option))
#define UI_IMAGERECT(Attr,Option)  *(s_ImageRect4*)(##Attr->Get(Option))

#define UI_MAKE_WINDOW(wndattr_Base,WindowType,Buffer)			\
	wndattr_Base  = new (##WindowType);							\
	*((##WindowType*)wndattr_Base) = *(##WindowType*)Buffer;


#define UI_READ_INT(Stream,Var)									\
	(##Stream)->Read(Buffer_INT,4);								\
	(##Var) = *(int*)Buffer_INT;


#define UI_READ_WINDOW(Stream,WindowToLoad,ClassID,ClassSize)	\
	(##WindowToLoad) = GetMatchedWindow(ClassID);				\
	(##Stream)->Read(WindowToLoad,ClassSize);		

#define UI_WAPI_READ_INT(FileHandle,Var)						\
	ReadFile(FileHandle,Buffer_INT,4,&WrittenByteSize,0);	    \
	(##Var) = *(int*)Buffer_INT;


// SPWINDOW_Base.
//---------------------------------------------------
#define UI_SPWINDOW_DEFAULT_BASE(InstanceID,WindowType,CurrentWindow,AttributeMap,ParentWindow)																				\
	(##AttributeMap)	= g_UIManager->GetWindow(InstanceID);																												\
	if((##AttributeMap))																																					\
	{																																										\
	(##CurrentWindow)	= new (##WindowType)(InstanceID,UI_INT(AttributeMap,GET_SPWINDOW_X) ,UI_INT(AttributeMap,GET_SPWINDOW_Y),											\
	UI_INT(AttributeMap,GET_SPWINDOW_CX),UI_INT(AttributeMap,GET_SPWINDOW_CY),ParentWindow);																				\
	(##CurrentWindow)->InitFromXML(AttributeMap);																															\
	}																																										\
	

#define UI_CREATE(InstanceID,WindowType,CurrentWindow,AttributeMap,ParentWindow)																							\
	(##AttributeMap)	= g_UIManager->GetWindow(InstanceID);																												\
	if((##AttributeMap))																																					\
{																																											\
	(##CurrentWindow)	= new (##WindowType)(InstanceID,UI_INT(AttributeMap,GET_SPWINDOW_X) ,UI_INT(AttributeMap,GET_SPWINDOW_Y),											\
	UI_INT(AttributeMap,GET_SPWINDOW_CX),UI_INT(AttributeMap,GET_SPWINDOW_CY),ParentWindow);																				\
	(##CurrentWindow)->InitFromXML(AttributeMap);																															\
}																																											\



#define UI_SPWINDOW_DEFAULT_BASE_WITH_CLASSID(ClassID,InstanceID,WindowType,CurrentWindow,AttributeMap,ParentWindow)														\
	(##AttributeMap)	= g_UIManager->GetWindow(InstanceID);																												\
	(##CurrentWindow)	= new (##WindowType)(ClassID,InstanceID,UI_INT(AttributeMap,GET_SPWINDOW_X) ,UI_INT(AttributeMap,GET_SPWINDOW_Y),									\
	UI_INT(AttributeMap,GET_SPWINDOW_CX),UI_INT(AttributeMap,GET_SPWINDOW_CY),ParentWindow)

#define UI_SPWINDOW_DEFAULT_SETWINDOWTYPE(CurrentWindow,AttributeMap)																										\
	(##CurrentWindow)->SetWindowType((WND_TYPE)(UI_INT(AttributeMap,GET_SPWINDOW_WINDOWTYPE)))																				\


#define UI_SPWINDOW_DEFAULT_SETIMAGE(CurrentWindow,AttributeMap)																											\
	(##CurrentWindow)->SetImage(UI_STRING(AttributeMap,GET_SPWINDOW_IMAGE_FILENAME),UI_INT(AttributeMap,GET_SPWINDOW_IMAGE_X),UI_INT(AttributeMap,GET_SPWINDOW_IMAGE_Y));   \
	(##CurrentWindow)->SetRectSize(UI_INT(AttributeMap,GET_SPWINDOW_REALSIZE_X),UI_INT(AttributeMap,GET_SPWINDOW_REALSIZE_Y))


#define UI_SPWINDOW_DEFAULT_FONTTYPE(CurrentWindow,AttributeMap)																											\
	(##CurrentWindow)->SetFont(static_cast<FONT_ENUMERATE>(UI_INT(AttributeMap,GET_SPWINDOW_FONTTYPE)))																		\

#define UI_SPWINDOW_DEFAULT_SETFONTCOLOR(CurrentWindow,AttributeMap)																										\
	(##CurrentWindow)->SetFontColor(D3DXCOLOR(UI_FLOAT(AttributeMap,GET_SPWINDOW_FONT_R),																					\
	UI_FLOAT(AttributeMap,GET_SPWINDOW_FONT_G),																					\
	UI_FLOAT(AttributeMap,GET_SPWINDOW_FONT_B),																					\
	UI_FLOAT(AttributeMap,GET_SPWINDOW_FONT_A)))																					

#define UI_SPWINDOW_DEFAULT_SHOW(CurrentWindow) (##CurrentWindow)->Show()  
#define UI_SPWINDOW_DEFAULT_HIDE(CurrentWindow) (##CurrentWindow)->Hide()  



//	SPWINDOW_Static.
//---------------------------------------------------
#define UI_SPWINDOW_STATIC_SETNORMALBODY(CurrentWindow,AttributeMap,Imagerc)																								\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_STATIC_NORMALBODY)));																				\
	(##CurrentWindow)->SetImageNormalBody((##Imagerc)->FileName,(##Imagerc)->x,(##Imagerc)->y,(##Imagerc)->cx,(##Imagerc)->cy)		




// SPWINDOW_Button.
//---------------------------------------------------
#define UI_SPWINDOW_BUTTON_SETIMAGES(CurrentWindow,AttributeMap) \
	(##CurrentWindow)->SetImageDisable(UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_DISABLE),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_DISABLE_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_DISABLE_Y)); \
	(##CurrentWindow)->SetImageHit	  (UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_HIT),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_HIT_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_HIT_Y));             \
	(##CurrentWindow)->SetImagePush	  (UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_PUSH),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_PUSH_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_PUSH_Y))

#define UI_SPWINDOW_BUTTON_SETIMAGECHECK(CurrentWindow,AttributeMap) \
	(##CurrentWindow)->SetImageCheck(UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_CHECK),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_CHECK_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_CHECK_Y))			\

#define UI_SPWINDOW_BUTTON_SETIMAGES_HITANDPUSH(CurrentWindow,AttributeMap) \
	(##CurrentWindow)->SetImageHit	  (UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_HIT),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_HIT_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_HIT_Y));             \
	(##CurrentWindow)->SetImagePush	  (UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_PUSH),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_PUSH_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_PUSH_Y))

#define UI_SPWINDOW_BUTTON_SETIMAGES_HIT_PUSH_CHECK(CurrentWindow,AttributeMap) \
	(##CurrentWindow)->SetImageHit	  (UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_HIT),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_HIT_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_HIT_Y));             \
	(##CurrentWindow)->SetImagePush	  (UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_PUSH),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_PUSH_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_PUSH_Y));          \
	(##CurrentWindow)->SetImageCheck  (UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_CHECK),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_CHECK_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_CHECK_Y)) 


#define UI_SPWINDOW_BUTTON_SETIMAGES_ORI_HIT_PUSH(CurrentWindow,AttributeMap) \
	(##CurrentWindow)->SetImage		  (UI_STRING(AttributeMap,GET_SPWINDOW_IMAGE_FILENAME),UI_INT(AttributeMap,GET_SPWINDOW_IMAGE_X),UI_INT(AttributeMap,GET_SPWINDOW_IMAGE_Y));							\
	(##CurrentWindow)->SetImageHit	  (UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_HIT),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_HIT_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_HIT_Y));             \
	(##CurrentWindow)->SetImagePush	  (UI_STRING(AttributeMap,GET_SPWINDOW_BUTTON_FILENAME_PUSH),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_PUSH_X),UI_INT(AttributeMap,GET_SPWINDOW_BUTTON_PUSH_Y))          




//	SPWINDOW_Gauge.
//---------------------------------------------------
#define UI_SPWINDOW_GAUGE_SETALLSTATE(CurrentWindow,AttributeMap,Rect,Imagerc)														\
	(##CurrentWindow)->SetWindowType((WND_TYPE)(UI_INT(AttributeMap,GET_SPWINDOW_WINDOWTYPE)));										\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_SET)));												\
	(##Rect).left	= (##Imagerc)->x;																								\
	(##Rect).top	= (##Imagerc)->y;																								\
	(##Rect).right	= (##Imagerc)->cx;																								\
	(##Rect).bottom	= (##Imagerc)->cy;																								\
	(##CurrentWindow)->SetImageGauge((##Imagerc)->FileName,(##Rect));   															\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_EDGE_SET)));										\
	(##Rect).left	= (##Imagerc)->x;																								\
	(##Rect).top	= (##Imagerc)->y;																								\
	(##Rect).right	= (##Imagerc)->cx;																								\
	(##Rect).bottom	= (##Imagerc)->cy;																								\
	(##CurrentWindow)->SetImageEdge((##Imagerc)->FileName,(##Rect));																\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_DECO_SET)));										\
	(##Rect).left	= (##Imagerc)->x;																								\
	(##Rect).top	= (##Imagerc)->y;																								\
	(##Rect).right	= (##Imagerc)->cx;																								\
	(##Rect).bottom	= (##Imagerc)->cy;																								\
	(##CurrentWindow)->SetImageDeco((##Imagerc)->FileName,(##Rect));																\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_FRAMEHEAD_SET)));									\
	(##CurrentWindow)->SetImageFrameHead((##Imagerc)->FileName,(##Imagerc)->x,(##Imagerc)->y,(##Imagerc)->cx,(##Imagerc)->cy);		\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_FRAMEBODY_SET)));									\
	(##CurrentWindow)->SetImageFrameBody((##Imagerc)->FileName,(##Imagerc)->x,(##Imagerc)->y,(##Imagerc)->cx,(##Imagerc)->cy);		\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_FRAMETAIL_SET)));									\
	(##CurrentWindow)->SetImageFrameTail((##Imagerc)->FileName,(##Imagerc)->x,(##Imagerc)->y,(##Imagerc)->cx,(##Imagerc)->cy);		\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_FLICKER_SET)));										\
	(##CurrentWindow)->SetImageFlickerGauge((##Imagerc)->FileName,(##Imagerc)->x,(##Imagerc)->y,(##Imagerc)->cx,(##Imagerc)->cy);	\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_FLICKER_EDGE_SET)));								\
	(##CurrentWindow)->SetImageFlickerEdge((##Imagerc)->FileName,(##Imagerc)->x,(##Imagerc)->y,(##Imagerc)->cx,(##Imagerc)->cy);	\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_FLICKER_DECO_SET)));								\
	(##CurrentWindow)->SetImageFlickerDeco((##Imagerc)->FileName,(##Imagerc)->x,(##Imagerc)->y,(##Imagerc)->cx,(##Imagerc)->cy)	


#define UI_SPWINDOW_GAUGE_SETSTATE_NOFLICKER(CurrentWindow,AttributeMap,Rect,Imagerc)												\
	(##CurrentWindow)->SetWindowType((WND_TYPE)(UI_INT(AttributeMap,GET_SPWINDOW_WINDOWTYPE)));										\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_SET)));												\
	(##Rect).left	= (##Imagerc)->x;																								\
	(##Rect).top	= (##Imagerc)->y;																								\
	(##Rect).right	= (##Imagerc)->cx;																								\
	(##Rect).bottom	= (##Imagerc)->cy;																								\
	(##CurrentWindow)->SetImageGauge((##Imagerc)->FileName,(##Rect));   															\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_EDGE_SET)));										\
	(##Rect).left	= (##Imagerc)->x;																								\
	(##Rect).top	= (##Imagerc)->y;																								\
	(##Rect).right	= (##Imagerc)->cx;																								\
	(##Rect).bottom	= (##Imagerc)->cy;																								\
	(##CurrentWindow)->SetImageEdge((##Imagerc)->FileName,(##Rect));																\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_DECO_SET)));										\
	(##Rect).left	= (##Imagerc)->x;																								\
	(##Rect).top	= (##Imagerc)->y;																								\
	(##Rect).right	= (##Imagerc)->cx;																								\
	(##Rect).bottom	= (##Imagerc)->cy;																								\
	(##CurrentWindow)->SetImageDeco((##Imagerc)->FileName,(##Rect));																\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_FRAMEHEAD_SET)));									\
	(##CurrentWindow)->SetImageFrameHead((##Imagerc)->FileName,(##Imagerc)->x,(##Imagerc)->y,(##Imagerc)->cx,(##Imagerc)->cy);		\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_FRAMEBODY_SET)));									\
	(##CurrentWindow)->SetImageFrameBody((##Imagerc)->FileName,(##Imagerc)->x,(##Imagerc)->y,(##Imagerc)->cx,(##Imagerc)->cy);		\
	(##Imagerc) = &((s_ImageRect4)(UI_IMAGERECT(AttributeMap,GET_SPWINDOW_GAUGE_FRAMETAIL_SET)));									\
	(##CurrentWindow)->SetImageFrameTail((##Imagerc)->FileName,(##Imagerc)->x,(##Imagerc)->y,(##Imagerc)->cx,(##Imagerc)->cy)		


#endif