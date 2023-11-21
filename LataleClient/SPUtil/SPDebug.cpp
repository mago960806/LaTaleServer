

#include <WTypes.h>
#include <strsafe.h>

#include "SPDebug.h"




//--------------------------------------------------------------------------------------
// Outputs to the debug stream a formatted Unicode string with a variable-argument list.
//--------------------------------------------------------------------------------------
void DXUTOutputDebugStringW( LPCWSTR strMsg, ... )
{
#if defined(DEBUG) || defined(_DEBUG)
	WCHAR strBuffer[512];

	va_list args;
	va_start(args, strMsg);
	StringCchVPrintfW( strBuffer, 512, strMsg, args );
	strBuffer[511] = L'\0';
	va_end(args);

	OutputDebugStringW( strBuffer );
#else
	UNREFERENCED_PARAMETER(strMsg);
#endif
}


//--------------------------------------------------------------------------------------
// Outputs to the debug stream a formatted MBCS string with a variable-argument list.
//--------------------------------------------------------------------------------------
void DXUTOutputDebugString( LPCSTR strMsg, ... )
{
#if defined(DEBUG) || defined(_DEBUG)
	CHAR strBuffer[512];

	va_list args;
	va_start(args, strMsg);
	StringCchVPrintfA( strBuffer, 512, strMsg, args );
	strBuffer[511] = '\0';
	va_end(args);

	OutputDebugStringA( strBuffer );
#else
	UNREFERENCED_PARAMETER(strMsg);
#endif
}

