	if (hDLL == NULL)
	{
		strcpy( szBuf , "LaTaleClientが正常に終了されませんでした。DBGHELP.DLL ファイルがないので情報を保存できません。" );

		pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
		pLogFile->Release();

		::MessageBox( NULL , szBuf , "LaTaleClient" , MB_ICONWARNING );

		return retval;
	}

	// DLL ｳｻｺﾎｿ｡ｼｭ MiniDumpWriteDump APIｸｦ ﾃ｣ｴﾂｴﾙ.
	MINIDUMPWRITEDUMP pfnMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDLL, "MiniDumpWriteDump");

	// ｹﾌｴﾏｴ�ﾇﾁ ﾇﾔｼ�ｸｦ ﾃ｣ﾀｻ ｼ・ｾ�ｴﾙｸ・ｴ・ﾀﾌｻ・ﾁ�ﾇ猊ﾒ ｼ・ｾ�ｴﾙ.
	if (pfnMiniDumpWriteDump == NULL)
	{
		strcpy( szBuf , "LaTaleClientが正常に終了されませんでした。 DBGHELP.DLLのバージョンが低いため情報を保存できません。." );

		pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
		pLogFile->Release();

		::MessageBox( NULL , szBuf , "LaTaleClient" , MB_ICONWARNING );

		return retval;
	}

	// ｴ�ﾇﾁ ﾆﾄﾀﾏ ﾀﾌｸｧ += ﾈｮﾀ蠡ﾚ
	::lstrcat(szDumpPath, ".DMP");

	// ﾆﾄﾀﾏﾀｻ ｻ�ｼｺﾇﾑｴﾙ.
	HANDLE hFile = ::CreateFile(
		szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// ﾆﾄﾀﾏﾀｻ ｻ�ｼｺﾇﾒ ｼ・ｾ�ｴﾙｸ・ｴ・ﾀﾌｻ・ﾁ�ﾇ猊ﾒ ｼ・ｾ�ｴﾙ.
	if (hFile == INVALID_HANDLE_VALUE)
	{
//		sprintf( szBuf , "Failed to create dump file '%s'", szDumpPath );
		sprintf( szBuf , "LaTaleClientが正常に終了されませんでした。 %s ファイルを生成できません。" , szDumpPath );

		pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
		pLogFile->Release();

		::MessageBox( NULL , szBuf , "LaTaleClient" , MB_ICONWARNING );

		return retval;
	}

	MINIDUMP_EXCEPTION_INFORMATION ExceptionParam;

	ExceptionParam.ThreadId = ::GetCurrentThreadId();
	ExceptionParam.ExceptionPointers = pExPtr;
	ExceptionParam.ClientPointers = FALSE;

	// ｿﾉｼﾇｿ｡ ｵ�ｶ・ｴ�ﾇﾁ ﾆﾄﾀﾏﾀｻ ｻ�ｼｺﾇﾑｴﾙ. 
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
	BOOL	bIsLaTaleErrorExits	=	FALSE;		// LaTaleError.EXE ｰ｡ ﾀﾖｴﾂﾁ・ﾈｮﾀﾎﾇﾑｴﾙ.
	TCHAR   szLaTaleErrorFullPath[_MAX_PATH] = {0,};
	TCHAR   szLaTaleErrorPath[_MAX_PATH] = {0,};
	TCHAR   szLaTaleErrorName[_MAX_PATH] = {0,};
	TCHAR   szLaTaleErrorCmdLine[_MAX_PATH] = {0,};

	// ｴ�ﾇﾁ ﾆﾄﾀﾏ ｻ�ｼｺ ｰ皺�ｸｦ ｷﾎｱﾗ ﾆﾄﾀﾏｿ｡ｴﾙ ｱ箙ﾏﾇﾑｴﾙ.
	if (bResult)
	{
//		TCHAR szMessage[8192] = {0,};
//		lstrcat(szMessage, "Saved dump file to '");
//		lstrcat(szMessage, szDumpPath);
//		lstrcat(szMessage, "'.\nFault Reason : ");
//		lstrcat(szMessage, GetFaultReason(pExPtr));

		retval = EXCEPTION_EXECUTE_HANDLER;

		// LaTaleError.EXE ｰ｡ ﾀﾖｴﾂﾁ・ﾈｮﾀﾎﾇﾑｴﾙ.
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

		_snprintf( szBuf , chDIMOF( szBuf ) - 1 , "LaTaleClient Version : %d.%d\r\n\r\nLaTaleClientが Error : [%s] 問題で正常に終了されませんでした。\r\n\r\nhttp://www.latale.jpのお問い合わせはこちら\r\n\r\n %s \r\n %s \r\n\r\n" , VER_MAJOR , VER_MINOR , GetFaultReason(pExPtr) , szDumpPath , szFilePath );

		if( bIsLaTaleErrorExits == TRUE )
		{
			::lstrcat( szBuf , "上記の２つのファイルと追加でお客様PCのDirect X情報を\r\n\r\n" );
			::lstrcat( szBuf , "LaTaleErrorを利用して情報ファイルを添付して送っていただけますでしょうか？ \r\n" );
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
			::lstrcat( szBuf , "上記の２つのファイルと追加でお客様PCのDirect X情報が必要なので\r\n" );
			::lstrcat( szBuf , "スタート > ファイル名を指定して実行  から dxdiag を入力して確認ボタンを押せば\r\n" );
			::lstrcat( szBuf , "Direct X 診断が行われますので全ての情報を保存をクリックして保存した後、\r\n\r\n" );
			::lstrcat( szBuf , "( スタート -> ファイル名を指定して実行 -> dxdiag 入力 -> 全ての情報を保存  )\r\n\r\n" );
			::lstrcat( szBuf , "そのファイルを添付してメールの件名に [DMPエラー関連 dxdiagファイル添付]で作成していただいて\r\n" );
			::lstrcat( szBuf , "送ってくださるようにお願いいたします。\r\n\r\n" );
			::lstrcat( szBuf , "メールに添付する容量は2メガまでですので、サイズの確認をしてから\r\n" );
			::lstrcat( szBuf , "横にある添付ボタンを押して下段の添付リストに\r\n" );
			::lstrcat( szBuf , "ファイルが添付されているかを確認してください。" );
		}
	}
	else
	{
//		sprintf( szBuf , "Failed to save dump file to '%s' (error %d)", szDumpPath, ::GetLastError() );
		sprintf( szBuf , "LaTaleClientが正常に終了されませんでした。%s 情報ファイルを保存することができませんでした。\r\n Error Code : [ %d ]" , szDumpPath , ::GetLastError() );

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
				::MessageBox( NULL , "見つけることができません。" , "LaTaleClient" , MB_ICONWARNING );
			if( res == ERROR_PATH_NOT_FOUND )
				::MessageBox( NULL , "プログラムへのアクセスが正しくありません。" , "LaTaleClient" , MB_ICONWARNING );
			if( res == ERROR_ACCESS_DENIED )
				::MessageBox( NULL , "プログラムへのアクセスが拒否されました。" , "LaTaleClient" , MB_ICONWARNING );
		}
	}