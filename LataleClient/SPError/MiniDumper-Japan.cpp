	if (hDLL == NULL)
	{
		strcpy( szBuf , "LaTaleClientÇ™ê≥èÌÇ…èIóπÇ≥ÇÍÇ‹ÇπÇÒÇ≈ÇµÇΩÅBDBGHELP.DLL ÉtÉ@ÉCÉãÇ™Ç»Ç¢ÇÃÇ≈èÓïÒÇï€ë∂Ç≈Ç´Ç‹ÇπÇÒÅB" );

		pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
		pLogFile->Release();

		::MessageBox( NULL , szBuf , "LaTaleClient" , MB_ICONWARNING );

		return retval;
	}

	// DLL ≥ª∫Œø°º≠ MiniDumpWriteDump API∏¶ √£¥¬¥Ÿ.
	MINIDUMPWRITEDUMP pfnMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDLL, "MiniDumpWriteDump");

	// πÃ¥œ¥˝«¡ «‘ºˆ∏¶ √£¿ª ºÅEæ¯¥Ÿ∏ÅE¥ÅE¿ÃªÅE¡¯«‡«“ ºÅEæ¯¥Ÿ.
	if (pfnMiniDumpWriteDump == NULL)
	{
		strcpy( szBuf , "LaTaleClientÇ™ê≥èÌÇ…èIóπÇ≥ÇÍÇ‹ÇπÇÒÇ≈ÇµÇΩÅB DBGHELP.DLLÇÃÉoÅ[ÉWÉáÉìÇ™í·Ç¢ÇΩÇﬂèÓïÒÇï€ë∂Ç≈Ç´Ç‹ÇπÇÒÅB." );

		pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
		pLogFile->Release();

		::MessageBox( NULL , szBuf , "LaTaleClient" , MB_ICONWARNING );

		return retval;
	}

	// ¥˝«¡ ∆ƒ¿œ ¿Ã∏ß += »Æ¿Â¿⁄
	::lstrcat(szDumpPath, ".DMP");

	// ∆ƒ¿œ¿ª ª˝º∫«—¥Ÿ.
	HANDLE hFile = ::CreateFile(
		szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// ∆ƒ¿œ¿ª ª˝º∫«“ ºÅEæ¯¥Ÿ∏ÅE¥ÅE¿ÃªÅE¡¯«‡«“ ºÅEæ¯¥Ÿ.
	if (hFile == INVALID_HANDLE_VALUE)
	{
//		sprintf( szBuf , "Failed to create dump file '%s'", szDumpPath );
		sprintf( szBuf , "LaTaleClientÇ™ê≥èÌÇ…èIóπÇ≥ÇÍÇ‹ÇπÇÒÇ≈ÇµÇΩÅB %s ÉtÉ@ÉCÉãÇê∂ê¨Ç≈Ç´Ç‹ÇπÇÒÅB" , szDumpPath );

		pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
		pLogFile->Release();

		::MessageBox( NULL , szBuf , "LaTaleClient" , MB_ICONWARNING );

		return retval;
	}

	MINIDUMP_EXCEPTION_INFORMATION ExceptionParam;

	ExceptionParam.ThreadId = ::GetCurrentThreadId();
	ExceptionParam.ExceptionPointers = pExPtr;
	ExceptionParam.ClientPointers = FALSE;

	// ø…º«ø° µ˚∂ÅE¥˝«¡ ∆ƒ¿œ¿ª ª˝º∫«—¥Ÿ. 
	BOOL bResult = FALSE;
	switch (s_DumpLevel)
	{
	case DUMP_LEVEL_0: // MiniDumpNormal
		bResult = pfnMiniDumpWriteDump(
			::GetCurrentProcess(), ::GetCurrentProcessId(), 
			hFile, MiniDumpNormal, &ExceptionParam, NULL, NULL);
		break;
	case DUMP_LEVEL_1: // MiniDumpWithDataSegs 
		bResult = pfnMiniDumpWriteDump(
			::GetCurrentProcess(), ::GetCurrentProcessId(), 
			hFile, MiniDumpWithDataSegs, &ExceptionParam, NULL, NULL);
		break;
	case DUMP_LEVEL_2: // MiniDumpWithFullMemory 
		bResult = pfnMiniDumpWriteDump(
			::GetCurrentProcess(), ::GetCurrentProcessId(), 
			hFile, (MINIDUMP_TYPE)( MiniDumpNormal | MiniDumpWithIndirectlyReferencedMemory ) , &ExceptionParam, NULL, NULL);
		break;
	case DUMP_LEVEL_3: // MiniDumpWithFullMemory 
		bResult = pfnMiniDumpWriteDump(
			::GetCurrentProcess(), ::GetCurrentProcessId(), 
			hFile, MiniDumpWithFullMemory, &ExceptionParam, NULL, NULL);
		break;
	default:
		bResult = pfnMiniDumpWriteDump(
			::GetCurrentProcess(), ::GetCurrentProcessId(), 
			hFile, MiniDumpNormal, &ExceptionParam, NULL, NULL);
		break;
	}

	if( bResult == FALSE )
	{
		bResult = pfnMiniDumpWriteDump( ::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, &ExceptionParam, NULL, NULL);
	}

	UINT	uiType	=	MB_ICONWARNING;
	BOOL	bIsLaTaleErrorExits	=	FALSE;		// LaTaleError.EXE ∞° ¿÷¥¬¡ÅE»Æ¿Œ«—¥Ÿ.
	TCHAR   szLaTaleErrorFullPath[_MAX_PATH] = {0,};
	TCHAR   szLaTaleErrorPath[_MAX_PATH] = {0,};
	TCHAR   szLaTaleErrorName[_MAX_PATH] = {0,};
	TCHAR   szLaTaleErrorCmdLine[_MAX_PATH] = {0,};

	// ¥˝«¡ ∆ƒ¿œ ª˝º∫ ∞·∞˙∏¶ ∑Œ±◊ ∆ƒ¿œø°¥Ÿ ±‚∑œ«—¥Ÿ.
	if (bResult)
	{
//		TCHAR szMessage[8192] = {0,};
//		lstrcat(szMessage, "Saved dump file to '");
//		lstrcat(szMessage, szDumpPath);
//		lstrcat(szMessage, "'.\nFault Reason : ");
//		lstrcat(szMessage, GetFaultReason(pExPtr));

		retval = EXCEPTION_EXECUTE_HANDLER;

		// LaTaleError.EXE ∞° ¿÷¥¬¡ÅE»Æ¿Œ«—¥Ÿ.
		if (::GetModuleFileName(NULL, szLaTaleErrorFullPath, _MAX_PATH))
		{
			::lstrcpy( szLaTaleErrorName , "LaTaleError.EXE" );

			TCHAR* pszSlash = ::strrchr( szLaTaleErrorFullPath , '\\' );

			if (pszSlash)
			{
				::lstrcpyn( szLaTaleErrorPath , szLaTaleErrorFullPath , (int)( pszSlash - szLaTaleErrorFullPath + 1 ) );
				::lstrcpy( pszSlash + 1, szLaTaleErrorName );

				if( ( _taccess( szLaTaleErrorFullPath , 0 ) ) != -1 )
					bIsLaTaleErrorExits	=	TRUE;
			}
		}

		_snprintf( szBuf , chDIMOF( szBuf ) - 1 , "LaTaleClient Version : %d.%d\r\n\r\nLaTaleClientÇ™ Error : [%s] ñ‚ëËÇ≈ê≥èÌÇ…èIóπÇ≥ÇÍÇ‹ÇπÇÒÇ≈ÇµÇΩÅB\r\n\r\nhttp://www.latale.jpÇÃÇ®ñ‚Ç¢çáÇÌÇπÇÕÇ±ÇøÇÁ\r\n\r\n %s \r\n %s \r\n\r\n" , VER_MAJOR , VER_MINOR , GetFaultReason(pExPtr) , szDumpPath , szFilePath );

		if( bIsLaTaleErrorExits == TRUE )
		{
			::lstrcat( szBuf , "è„ãLÇÃÇQÇ¬ÇÃÉtÉ@ÉCÉãÇ∆í«â¡Ç≈Ç®ãqólPCÇÃDirect XèÓïÒÇ\r\n\r\n" );
			::lstrcat( szBuf , "LaTaleErrorÇóòópÇµÇƒèÓïÒÉtÉ@ÉCÉãÇìYïtÇµÇƒëóÇ¡ÇƒÇ¢ÇΩÇæÇØÇ‹Ç∑Ç≈ÇµÇÂÇ§Ç©ÅH \r\n" );
			uiType	|=	MB_YESNO;

			TCHAR* pszFileSlash = ::strrchr( szFilePath , '\\' );
			TCHAR* pszDumpSlash = ::strrchr( szDumpPath , '\\' );

			if( pszFileSlash )
			{
				::lstrcpy( szFilePath , pszFileSlash + 1 );
			}

			if( pszDumpSlash )
			{
				::lstrcpy( szDumpPath , pszDumpSlash + 1 );
			}

			::lstrcat( szLaTaleErrorCmdLine , UNIQUE_LATALE_ERROR );
			::lstrcat( szLaTaleErrorCmdLine , szErrorUnique );
			::lstrcat( szLaTaleErrorCmdLine , " " );
			::lstrcat( szLaTaleErrorCmdLine , CMD_INFO );
			::lstrcat( szLaTaleErrorCmdLine , szFilePath );
			::lstrcat( szLaTaleErrorCmdLine , " " );
			::lstrcat( szLaTaleErrorCmdLine , CMD_DMP );
			::lstrcat( szLaTaleErrorCmdLine , szDumpPath );
			::lstrcat( szLaTaleErrorCmdLine , CMD_END );
		}
		else
		{
			::lstrcat( szBuf , "è„ãLÇÃÇQÇ¬ÇÃÉtÉ@ÉCÉãÇ∆í«â¡Ç≈Ç®ãqólPCÇÃDirect XèÓïÒÇ™ïKóvÇ»ÇÃÇ≈\r\n" );
			::lstrcat( szBuf , "ÉXÉ^Å[Ég > ÉtÉ@ÉCÉãñºÇéwíËÇµÇƒé¿çs  Ç©ÇÁ dxdiag Çì¸óÕÇµÇƒämîFÉ{É^ÉìÇâüÇπÇŒ\r\n" );
			::lstrcat( szBuf , "Direct X êfífÇ™çsÇÌÇÍÇ‹Ç∑ÇÃÇ≈ëSÇƒÇÃèÓïÒÇï€ë∂ÇÉNÉäÉbÉNÇµÇƒï€ë∂ÇµÇΩå„ÅA\r\n\r\n" );
			::lstrcat( szBuf , "( ÉXÉ^Å[Ég -> ÉtÉ@ÉCÉãñºÇéwíËÇµÇƒé¿çs -> dxdiag ì¸óÕ -> ëSÇƒÇÃèÓïÒÇï€ë∂  )\r\n\r\n" );
			::lstrcat( szBuf , "ÇªÇÃÉtÉ@ÉCÉãÇìYïtÇµÇƒÉÅÅ[ÉãÇÃåèñºÇ… [DMPÉGÉâÅ[ä÷òA dxdiagÉtÉ@ÉCÉãìYït]Ç≈çÏê¨ÇµÇƒÇ¢ÇΩÇæÇ¢Çƒ\r\n" );
			::lstrcat( szBuf , "ëóÇ¡ÇƒÇ≠ÇæÇ≥ÇÈÇÊÇ§Ç…Ç®äËÇ¢Ç¢ÇΩÇµÇ‹Ç∑ÅB\r\n\r\n" );
			::lstrcat( szBuf , "ÉÅÅ[ÉãÇ…ìYïtÇ∑ÇÈóeó ÇÕ2ÉÅÉKÇ‹Ç≈Ç≈Ç∑ÇÃÇ≈ÅAÉTÉCÉYÇÃämîFÇÇµÇƒÇ©ÇÁ\r\n" );
			::lstrcat( szBuf , "â°Ç…Ç†ÇÈìYïtÉ{É^ÉìÇâüÇµÇƒâ∫íiÇÃìYïtÉäÉXÉgÇ…\r\n" );
			::lstrcat( szBuf , "ÉtÉ@ÉCÉãÇ™ìYïtÇ≥ÇÍÇƒÇ¢ÇÈÇ©ÇämîFÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB" );
		}
	}
	else
	{
//		sprintf( szBuf , "Failed to save dump file to '%s' (error %d)", szDumpPath, ::GetLastError() );
		sprintf( szBuf , "LaTaleClientÇ™ê≥èÌÇ…èIóπÇ≥ÇÍÇ‹ÇπÇÒÇ≈ÇµÇΩÅB%s èÓïÒÉtÉ@ÉCÉãÇï€ë∂Ç∑ÇÈÇ±Ç∆Ç™Ç≈Ç´Ç‹ÇπÇÒÇ≈ÇµÇΩÅB\r\n Error Code : [ %d ]" , szDumpPath , ::GetLastError() );

		//filelog(NULL, "Failed to save dump file to '%s' (error %d,%s)", 
		//	szDumpPath, ::GetLastError(), GetLastErrorString().c_str());

		assert(false);
	}

	pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
	pLogFile->Release();

	::CloseHandle(hFile);
	int	iResult = ::MessageBox( NULL , szBuf , "LaTaleClient" , uiType );

	//	::MessageBox( NULL , szLaTaleErrorCmdLine , "LaTaleClient" , MB_ICONWARNING );

	if( bIsLaTaleErrorExits == TRUE && iResult == IDYES )
	{
		SHELLEXECUTEINFO	si;

		memset( &si, 0, sizeof(SHELLEXECUTEINFO) );

		si.cbSize		= sizeof(si);
		si.lpVerb		= _T("Open");
		si.nShow		|= SW_SHOWNA;
		si.lpDirectory	= (LPCSTR)szLaTaleErrorPath;
		si.lpFile		= (LPCSTR)szLaTaleErrorName;
		si.lpParameters	= (LPCSTR)szLaTaleErrorCmdLine;

		if( ShellExecuteEx( &si ) == FALSE )
		{   
			int res = GetLastError();

			if( res == ERROR_FILE_NOT_FOUND )
				::MessageBox( NULL , "å©Ç¬ÇØÇÈÇ±Ç∆Ç™Ç≈Ç´Ç‹ÇπÇÒÅB" , "LaTaleClient" , MB_ICONWARNING );
			if( res == ERROR_PATH_NOT_FOUND )
				::MessageBox( NULL , "ÉvÉçÉOÉâÉÄÇ÷ÇÃÉAÉNÉZÉXÇ™ê≥ÇµÇ≠Ç†ÇËÇ‹ÇπÇÒÅB" , "LaTaleClient" , MB_ICONWARNING );
			if( res == ERROR_ACCESS_DENIED )
				::MessageBox( NULL , "ÉvÉçÉOÉâÉÄÇ÷ÇÃÉAÉNÉZÉXÇ™ãëî€Ç≥ÇÍÇ‹ÇµÇΩÅB" , "LaTaleClient" , MB_ICONWARNING );
		}
	}