// 
// HtGECACln.h
//

#ifndef _HTGECACLN_H_
#define _HTGECACLN_H_

#pragma pack(push)
#pragma pack(1)
typedef struct {
	WORD iLen;		// The actual length of the data
	WORD iMax;		// The biggest length of the data
	BYTE abData[1];	// data instance (variable length array)
} HtGECAData, *PHtGECAData;
#pragma pack(pop)

#ifdef __cplusplus
extern "C"{
#endif

BOOL HtGECAAuthorize( PHtGECAData pDataA, PHtGECAData pDataQ );

#ifdef __cplusplus
}
#endif

#endif // _HTGECACLN_H_
