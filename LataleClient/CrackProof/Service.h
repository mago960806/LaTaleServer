///
/// Service.h
///

#ifndef _Service_h_
#define _Service_h_

#define	_Service_ns_	HtpecService
namespace _Service_ns_ { }
namespace Service = _Service_ns_;
namespace _Service_ns_ {


///
/// ŠO•”•Ï”éŒ¾
///

extern DWORD iLibErr;
extern DWORD iSysErr;
extern PSTR  szDriverFileQry;
extern PSTR  szDisplayNameQry;


///
/// ŠO•”ŠÖ”éŒ¾
///

extern BOOL DriverLoad  (PSTR szDriverName, PSTR szDriverFile, PSTR szDisplayName);
extern BOOL DriverUnload(PSTR szDriverName);
extern BOOL DriverIsRun (PSTR szDriverName);
extern BOOL DriverQuery (PSTR szDriverName);


} // namespace _Service_ns_

#endif // _Service_h_
