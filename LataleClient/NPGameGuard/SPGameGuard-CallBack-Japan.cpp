		switch (dwMsg)
		{
		case NPGAMEMON_COMM_ERROR:
		case NPGAMEMON_COMM_CLOSE:
			bAppExit = TRUE;	// 終了コード
			return FALSE;
		case NPGAMEMON_INIT_ERROR:
			wsprintf( msg , "GameGuard初期化エラー : %lu", dwArg);
			bAppExit = TRUE;	// 終了コード
			lpszMsg = msg;
			break;
		case NPGAMEMON_SPEEDHACK:
			wsprintf( msg , "スピードハックが感知されました。");
			bAppExit = TRUE;	// 終了コード
			lpszMsg = msg;
			break;
		case NPGAMEMON_GAMEHACK_KILLED:
			wsprintf( msg , "不正ツールが見つかりました。\r\n%s", SPGameGuard::GetInstance()->GetInfo() );
			bAppExit = TRUE;	// 終了コード
			lpszMsg = msg;
			break;
		case NPGAMEMON_GAMEHACK_DETECT:
			wsprintf( msg , "不正ツールが見つかりました。\r\n%s", SPGameGuard::GetInstance()->GetInfo() );
			bAppExit = TRUE;	// 終了コード
			lpszMsg = msg;
			break;
		case NPGAMEMON_GAMEHACK_DOUBT:
			wsprintf( msg , "ゲームやGameGuardが変造されました。");
			bAppExit = TRUE;	// 終了コード
			lpszMsg = msg;
			break;
		default:
			// 適切な終了メッセージ出力
			lpszMsg = "GameGuardの実行中エラーが発生しました。ゲームフォルダ内中のGameGuardフォルダにある *.erl ファイルを添付してgame2@inca.co.krにメールを送ってくださるようお願いします。";
			bAppExit = TRUE;
			break;
		}
