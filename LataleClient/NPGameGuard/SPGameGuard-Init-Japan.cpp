			// 「6.主要エラーコード」を参考して状況に合うメッセージを出力します。
			switch (dwResult)
			{
			case NPGAMEMON_ERROR_EXIST:
				lpszMsg = "GameGuardが実行中です。しばらく後または再起動の後、再実行してください。";
				break;
			case NPGAMEMON_ERROR_GAME_EXIST:
				lpszMsg = "ゲームが重複実行されたとか既にGameGuardが実行中です。ゲーム終了の後再実行してください。";
				break;
			case NPGAMEMON_ERROR_INIT:
				lpszMsg = "GameGuard初期化エラーです。再起動の後、再度実行してみるとか衝突の可能性がある他のプログラムを終了した後、実行してください。";
				break;
			case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
			case NPGAMEMON_ERROR_NFOUND_GG:
			case NPGAMEMON_ERROR_AUTH_INI:
			case NPGAMEMON_ERROR_NFOUND_INI:
				lpszMsg = "GameGuardファイルがないか変造されました。GameGuardセットアップファイルをインストールしてください。";
				break;
			case NPGAMEMON_ERROR_CRYPTOAPI:
				lpszMsg = "ウィンドウの一部システムファイルが損傷されました。インターネットエクスプローラ(IE)を再インストールしてください。";
				break;
			case NPGAMEMON_ERROR_EXECUTE:
				lpszMsg = "GameGuard実行に失敗しました。GameGuardセットアップファイルを再インストールしてください。";
				break;
			case NPGAMEMON_ERROR_ILLEGAL_PRG:
				lpszMsg = "不法プログラムが見つかりました。不要なプログラムを終了した後、再実行してください。";
				break;
			case NPGMUP_ERROR_ABORT:
				lpszMsg = "GameGuardアップデートをキャンセルしました。続けて接続できない場合、インターネット及び個人ファイアウォール設定を調整してください。";
				break;
			case NPGMUP_ERROR_CONNECT:
			case HOOK_TIMEOUT:
				lpszMsg = "ウィルスやSpywareによってHookingに失敗しました。最新ワクチンをタウンロードしてコンピューター全体検査をしてください。";
				break;
			case NPGAMEMON_ERROR_GAMEGUARD:
				lpszMsg = "GameGuard初期かエラー又は旧バージョンのGameGuardファイルです。 GameGuardセットアップファイルを再インストールしてゲームを実行してください。";
				break;
			case NPGMUP_ERROR_PARAM:
				lpszMsg = "iniファイルがないとか変造されました。GameGuardセットアップファイルを再インストールしてください。";
				break;
			case NPGMUP_ERROR_INIT:
				lpszMsg = "npgmup.des初期化エラーです。GameGuardフォルダを削除した後ゲームを再起動してください。";
				break;
			//case ???:
			//	lpszMsg = "検証されないWindows XP x64バージョンを実行した場合です。最新バージョンでインストールした後ゲーム実行をしてください。";
			//	break;
			case NPGMUP_ERROR_DOWNCFG:
				lpszMsg = "GameGuardのアップデートサーバ接続に失敗しました。しばらく後再実行するか、個人ファイアウォールがあれば設定を調整してください。";
				break;
			case NPGMUP_ERROR_AUTH:
				lpszMsg = "GameGuardのアップデートが完了されませんでした。ウィルスワクチンを一時中止させた後、再実行するか、PC管理プログラムを使えば設定を調整してください。";
				break;
			case NPGAMEMON_ERROR_NPSCAN:
				lpszMsg = "ウィルス及び不正ツール検査モジュールローディングに失敗しました。メモリー不足またはウィルスによる感染の可能性があります。";
				break;
			default:
				// 適切な終了メッセージ出力
				lpszMsg = "GameGuardの実行中エラーが発生しました。ゲームフォルダ内中のGameGuardフォルダにある *.erl ファイルを添付してgame2@inca.co.krにメールを送ってくださるようお願いします。";
				break;
			}
			wsprintf( msg, "GameGuardエラー : %lu" , dwResult );