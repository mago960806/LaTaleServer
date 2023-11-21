//
// HtGEPD.h
//

#ifndef _HTGEPD_H_
#define _HTGEPD_H_

typedef void (WINAPI* HtGEPDFnCallback)();

#ifdef __cplusplus
extern "C"{
#endif

extern BOOL HtGEPDSetCallback( HtGEPDFnCallback fnCallback );

#ifdef __cplusplus
}
#endif

#endif // _HTGEPD_H_
