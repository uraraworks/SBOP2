/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MainFrame.h												 */
/* 内容			:サーバーメインフレーム 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/04													 */
/* ========================================================================= */

#pragma once

class CPacketBase;
class CMgrData;
class CUraraSockTCPSBO;
class CTextOutput;
class CUpdateServerInfo;
class CLibInfoAccount;
class CLibInfoCharSvr;
class CLibInfoMapBase;
class CLibInfoMapObject;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItemType;
class CLibInfoItem;
class CLibInfoItemWeapon;
class CLibInfoEffect;
class CLibInfoMotion;
class CLibInfoMotionType;
class CLibInfoEfcBalloon;
class CLibInfoSystem;
class CLibInfoSkill;
class CInfoCharBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CMainFrame
{
public:
			CMainFrame();											/* コンストラクタ */
	virtual ~CMainFrame();											/* デストラクタ */

	int		MainLoop	(HINSTANCE hInstance);								/* メインループ */

	/* 送信処理 */
	void	SendToScreenChar	(CInfoCharBase *pInfoChar, CPacketBase *pPacket);	/* 指定キャラと同じ画面のキャラへ送信 */
	void	SendToMapChar		(DWORD dwMapID, CPacketBase *pPacket);				/* 指定マップにいるキャラへ送信 */
	void	SendToAdminChar		(CPacketBase *pPacket);								/* 管理者権限を持っているキャラへ送信 */
	void	SendToClient		(DWORD dwSessionID, CPacketBase *pPacket);			/* セッションIDが有効な場合のみ送信 */


private:
	static	LRESULT CALLBACK WndProcEntry	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	/* メインウィンドウプロシージャ(エントリポイント) */
			LRESULT WndProc					(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	/* メインウィンドウプロシージャ */
	BOOL	OnCreate			(HWND hWnd, LPCREATESTRUCT lpCreateStruct);			/* メッセージハンドラ(WM_CREATE) */
	void	OnClose				(HWND hWnd);										/* メッセージハンドラ(WM_CLOSE) */
	void	OnDestroy			(HWND hWnd);										/* メッセージハンドラ(WM_DESTROY) */
	void	OnPaint				(HWND hWnd);										/* メッセージハンドラ(WM_PAINT) */
	void	OnTimer				(HWND hWnd, UINT id);								/* メッセージハンドラ(WM_TIMER) */
	void	OnCommand			(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);	/* メッセージハンドラ(WM_COMMAND) */
	void	OnAddClient			(DWORD dwSessionID);								/* メッセージハンドラ(WM_URARASOCK_ADDCLIENT) */
	void	OnDecClient			(DWORD dwSessionID);								/* メッセージハンドラ(WM_URARASOCK_DECCLIENT) */
	void	OnRecv				(PBYTE pData, DWORD dwSessionID);					/* メッセージハンドラ(WM_URARASOCK_RECV) */
	void	OnDisconnect		(DWORD dwSessionID);								/* メッセージハンドラ(WM_DISCONNECT) */
	void	TimerProc			(void);												/* 時間処理 */
	void	TimerProcKeepalive	(void);												/* 時間処理(生存確認チェック) */
	void	MyTextOut			(HDC hDC, int x, int y, LPCSTR pStr);				/* 文字列描画 */
	void	UpdateServerInfo	(BOOL bSend = TRUE, BOOL bUpload = FALSE);			/* サーバー情報を更新 */

	void	OnCommandACCOUNT_DELETEALL	(void);									/* メニューハンドラ(全アカウントを削除) */
	void	OnCommandUPDATE_RENEW		(void);									/* メニューハンドラ(アップデートファイル更新) */

	/* 受信処理(MainFrameRecvProcVERSION.cpp) */
	void	RecvProcVERSION						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* バージョン系 */
	void	RecvProcVERSION_REQ_VERSIONCHECK	(PBYTE pData, DWORD dwSessionID);					/* バージョンチェック要求 */
	void	RecvProcVERSION_REQ_FILELISTCHECK	(PBYTE pData, DWORD dwSessionID);					/* ファイルリストチェック要求 */
	void	RecvProcVERSION_REQ_FILELIST		(PBYTE pData, DWORD dwSessionID);					/* ファイルリスト要求 */
	void	RecvProcVERSION_REQ_FILE			(PBYTE pData, DWORD dwSessionID);					/* ファイル要求 */

	/* 受信処理(MainFrameRecvProcCONNECT.cpp) */
	void	RecvProcCONNECT						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* 接続系 */
	void	RecvProcCONNECT_REQ_LOGIN			(PBYTE pData, DWORD dwSessionID);					/* ログイン要求 */
	void	RecvProcCONNECT_REQ_PLAY			(PBYTE pData, DWORD dwSessionID);					/* ゲーム開始要求 */
	void	RecvProcCONNECT_KEEPALIVE			(PBYTE pData, DWORD dwSessionID);					/* 生存確認通知 */

	/* 受信処理(MainFrameRecvProcACCOUNT.cpp) */
	void	RecvProcACCOUNT						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* アカウント系 */
	void	RecvProcACCOUNT_REQ_ACCOUNTINFO		(PBYTE pData, DWORD dwSessionID);					/* アカウント情報要求 */
	void	RecvProcACCOUNT_REQ_MAKECHAR		(PBYTE pData, DWORD dwSessionID);					/* キャラ作成要求 */

	/* 受信処理(MainFrameRecvProcCHAR.cpp) */
	void	RecvProcCHAR						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* キャラ系 */
	void	RecvProcCHAR_REQ_CHARINFO			(PBYTE pData, DWORD dwSessionID);					/* キャラ情報要求 */
	void	RecvProcCHAR_MOVEPOS				(PBYTE pData, DWORD dwSessionID);					/* 移動通知 */
	void	RecvProcCHAR_STATE					(PBYTE pData, DWORD dwSessionID);					/* 状態通知 */
	void	RecvProcCHAR_REQ_CHAT				(PBYTE pData, DWORD dwSessionID);					/* チャット要求 */
	void	RecvProcCHAR_REQ_CHARINFO2			(PBYTE pData, DWORD dwSessionID);					/* キャラ情報要求(複数) */
	void	RecvProcCHAR_REQ_PUTGET				(PBYTE pData, DWORD dwSessionID);					/* アイテムを拾う置く要求 */
	void	RecvProcCHAR_REQ_USEITEM			(PBYTE pData, DWORD dwSessionID);					/* アイテムを使う要求 */
	void	RecvProcCHAR_REQ_DRAGITEM			(PBYTE pData, DWORD dwSessionID);					/* アイテム位置変更要求 */
	void	RecvProcCHAR_REQ_PUSH				(PBYTE pData, DWORD dwSessionID);					/* 押す要求 */
	void	RecvProcCHAR_REQ_TAIL				(PBYTE pData, DWORD dwSessionID);					/* 付いて行く要求 */
	void	RecvProcCHAR_REQ_MODIFY_PARAM		(PBYTE pData, DWORD dwSessionID);					/* パラメータ変更要求 */
	void	RecvProcCHAR_REQ_EQUIP				(PBYTE pData, DWORD dwSessionID);					/* 装備要求 */
	void	RecvProcCHAR_PROC_FISHING			(PBYTE pData, DWORD dwSessionID);					/* 釣り要求 */
	void	RecvProcCHAR_REQ_CHECKMAPEVENT		(PBYTE pData, DWORD dwSessionID);					/* マップイベントチェック要求 */
	void	RecvProcCHAR_STATE_CHARGE			(PBYTE pData, DWORD dwSessionID);					/* 溜め状態通知 */
	void	RecvProcCHAR_REQ_RECOVERY			(PBYTE pData, DWORD dwSessionID);					/* 気絶後復活要求 */

	/* 受信処理(MainFrameRecvProcMAP.cpp) */
	void	RecvProcMAP							(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* マップ系 */
	void	RecvProcMAP_REQ_MAPINFO				(PBYTE pData, DWORD dwSessionID);					/* マップ情報要求 */

	/* 受信処理(MainFrameRecvProcITEM.cpp) */
	void	RecvProcITEM						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* アイテム系 */
	void	RecvProcITEM_REQ_ITEMINFO			(PBYTE pData, DWORD dwSessionID);					/* アイテム情報要求 */
	void	RecvProcITEM_RENEWITEMINFO			(PBYTE pData, DWORD dwSessionID);					/* アイテム情報更新 */
	void	RecvProcITEM_REQ_ITEMTYPEINFO		(PBYTE pData, DWORD dwSessionID);					/* アイテム種別情報要求 */
	void	RecvProcITEM_RENEWITEMTYPEINFO		(PBYTE pData, DWORD dwSessionID);					/* アイテム種別情報更新 */
	void	RecvProcITEM_REQ_ITEMWEAPONINFO		(PBYTE pData, DWORD dwSessionID);					/* 武器情報要求 */

	/* 受信処理(MainFrameRecvProcADMIN.cpp) */
	void	RecvProcADMIN						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* 管理者系 */
	void	RecvProcADMIN_CHARINFO				(PBYTE pData, DWORD dwSessionID);					/* キャラ情報通知 */
	void	RecvProcADMIN_DELETECHARINFO		(PBYTE pData, DWORD dwSessionID);					/* キャラ情報削除 */
	void	RecvProcADMIN_MAP_RENEWMAPOBJECT	(PBYTE pData, DWORD dwSessionID);					/* マップオブジェクト更新 */
	void	RecvProcADMIN_MAP_RENEWOBJECTDATA	(PBYTE pData, DWORD dwSessionID);					/* マップオブジェクト配置データ更新 */
	void	RecvProcADMIN_MAP_DELETEOBJECTDATA	(PBYTE pData, DWORD dwSessionID);					/* マップオブジェクト配置データ削除 */
	void	RecvProcADMIN_RENEWMAPPARTS			(PBYTE pData, DWORD dwSessionID);					/* マップパーツ更新 */
	void	RecvProcADMIN_MAP_SETPARTS			(PBYTE pData, DWORD dwSessionID);					/* マップパーツ配置 */
	void	RecvProcADMIN_MAP_RENEWMAPSIZE		(PBYTE pData, DWORD dwSessionID);					/* マップサイズ更新 */
	void	RecvProcADMIN_MAP_DELETEPARTS		(PBYTE pData, DWORD dwSessionID);					/* マップパーツ削除 */
	void	RecvProcADMIN_MAP_COPYPARTS			(PBYTE pData, DWORD dwSessionID);					/* マップパーツコピー */
	void	RecvProcADMIN_MAP_SETMAPNAME		(PBYTE pData, DWORD dwSessionID);					/* マップ名変更 */
	void	RecvProcADMIN_RENEWMAPSHADOW		(PBYTE pData, DWORD dwSessionID);					/* マップ影更新 */
	void	RecvProcADMIN_MAP_SETMAPSHADOW		(PBYTE pData, DWORD dwSessionID);					/* マップ影配置 */
	void	RecvProcADMIN_MAP_DELETEMAPSHADOW	(PBYTE pData, DWORD dwSessionID);					/* マップ影削除 */
	void	RecvProcADMIN_MAP_RENEWEVENT		(PBYTE pData, DWORD dwSessionID);					/* マップイベント情報更新 */
	void	RecvProcADMIN_MAP_DELETEEVENT		(PBYTE pData, DWORD dwSessionID);					/* マップイベント情報削除 */
	void	RecvProcADMIN_MAP_ADD				(PBYTE pData, DWORD dwSessionID);					/* マップ追加 */
	void	RecvProcADMIN_REQ_ADMINILEVEL		(PBYTE pData, DWORD dwSessionID);					/* 管理者権限レベル要求 */
	void	RecvProcADMIN_RENEWADMINILEVEL		(PBYTE pData, DWORD dwSessionID);					/* 管理者権限レベル更新 */
	void	RecvProcADMIN_SERVER_SAVEINFO		(PBYTE pData, DWORD dwSessionID);					/* サーバー情報保存 */
	void	RecvProcADMIN_ITEM_ADD				(PBYTE pData, DWORD dwSessionID);					/* アイテム情報追加 */
	void	RecvProcADMIN_ITEM_COPY				(PBYTE pData, DWORD dwSessionID);					/* アイテム情報コピー */
	void	RecvProcADMIN_ITEM_DELETE			(PBYTE pData, DWORD dwSessionID);					/* アイテム情報削除 */
	void	RecvProcADMIN_ITEMTYPE_ADD			(PBYTE pData, DWORD dwSessionID);					/* アイテム種別情報追加 */
	void	RecvProcADMIN_ITEMTYPE_COPY			(PBYTE pData, DWORD dwSessionID);					/* アイテム種別情報コピー */
	void	RecvProcADMIN_ITEMTYPE_DELETE		(PBYTE pData, DWORD dwSessionID);					/* アイテム種別情報削除 */
	void	RecvProcADMIN_ITEMWEAPON_ADD		(PBYTE pData, DWORD dwSessionID);					/* 武器情報追加 */
	void	RecvProcADMIN_ITEMWEAPON_RENEW		(PBYTE pData, DWORD dwSessionID);					/* 武器情報更新 */
	void	RecvProcADMIN_CHAR_ADDNPC			(PBYTE pData, DWORD dwSessionID);					/* NPCの追加 */
	void	RecvProcADMIN_CHAR_MODIFYITEM		(PBYTE pData, DWORD dwSessionID);					/* 所持アイテムの変更 */
	void	RecvProcADMIN_CHAR_RENEWMOTION		(PBYTE pData, DWORD dwSessionID);					/* キャラモーション情報の更新 */
	void	RecvProcADMIN_CHAR_ADDMOTIONTYPE	(PBYTE pData, DWORD dwSessionID);					/* キャラモーション種別情報の追加 */
	void	RecvProcADMIN_CHAR_RENEWMOTIONTYPE	(PBYTE pData, DWORD dwSessionID);					/* キャラモーション種別情報の更新 */
	void	RecvProcADMIN_CHAR_RENEWSTATUS		(PBYTE pData, DWORD dwSessionID);					/* ステータス情報更新 */
	void	RecvProcADMIN_CHAR_REQ_STATUS		(PBYTE pData, DWORD dwSessionID);					/* ステータス情報要求 */
	void	RecvProcADMIN_CHAR_REQ_ONLINE		(PBYTE pData, DWORD dwSessionID);					/* オンライン中キャラ一覧要求 */
	void	RecvProcADMIN_EFC_RENEWBALLOON		(PBYTE pData, DWORD dwSessionID);					/* 噴出し情報の更新 */
	void	RecvProcADMIN_EFC_RENEWEFFECT		(PBYTE pData, DWORD dwSessionID);					/* エフェクト情報の更新 */
	void	RecvProcADMIN_REQ_PLAYSOUND			(PBYTE pData, DWORD dwSessionID);					/* 効果音の再生要求 */
	void	RecvProcADMIN_CHAR_REQ_ACCOUNT		(PBYTE pData, DWORD dwSessionID);					/* アカウント情報要求 */
	void	RecvProcADMIN_CHAR_RENEW_ACCOUNT	(PBYTE pData, DWORD dwSessionID);					/* アカウント情報更新 */
	void	RecvProcADMIN_RENEW_CLIENTVERSION	(PBYTE pData, DWORD dwSessionID);					/* クライアントバージョン更新 */
	void	RecvProcADMIN_SYSTEM_REQ_INFO		(PBYTE pData, DWORD dwSessionID);					/* システム情報要求 */
	void	RecvProcADMIN_SYSTEM_RENEWINFO		(PBYTE pData, DWORD dwSessionID);					/* システム情報の更新 */

	/* 受信処理(MainFrameRecvProcMSGCMD.cpp) */
	void	RecvProcMSGCMD						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* メッセージコマンド系 */
	void	RecvProcMSGCMD_CHGFACE				(PBYTE pData, DWORD dwSessionID);					/* 表情変更 */
	void	RecvProcMSGCMD_CHGHAIR				(PBYTE pData, DWORD dwSessionID);					/* 髪変更 */
	void	RecvProcMSGCMD_CHGCLOTH				(PBYTE pData, DWORD dwSessionID);					/* 服装変更 */
	void	RecvProcMSGCMD_CHGACCE				(PBYTE pData, DWORD dwSessionID);					/* アクセサリ変更 */
	void	RecvProcMSGCMD_CHGCOLOR				(PBYTE pData, DWORD dwSessionID);					/* 色変更 */
	void	RecvProcMSGCMD_CHGARMS				(PBYTE pData, DWORD dwSessionID);					/* 持ち物変更 */
	void	RecvProcMSGCMD_CHGSHIELD			(PBYTE pData, DWORD dwSessionID);					/* 盾変更 */
	void	RecvProcMSGCMD_MAKEITEM				(PBYTE pData, DWORD dwSessionID);					/* アイテム作成 */
	void	RecvProcMSGCMD_BALLOON				(PBYTE pData, DWORD dwSessionID);					/* 噴出し */
	void	RecvProcMSGCMD_DICE					(PBYTE pData, DWORD dwSessionID);					/* サイコロ */
	void	RecvProcMSGCMD_RND					(PBYTE pData, DWORD dwSessionID);					/* ランダム */
	void	RecvProcMSGCMD_EFFECT				(PBYTE pData, DWORD dwSessionID);					/* エフェクト */
	void	RecvProcMSGCMD_WHERE				(PBYTE pData, DWORD dwSessionID);					/* 最も集まっている場所 */

	/* 受信処理(MainFrameRecvProcEFFECT.cpp) */
	void	RecvProcEFFECT						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* エフェクト系 */
	void	RecvProcEFFECT_REQ_BALLOONINFO		(PBYTE pData, DWORD dwSessionID);					/* 噴出し情報要求 */
	void	RecvProcEFFECT_DELETEBALLOONINFO	(PBYTE pData, DWORD dwSessionID);					/* 噴出し情報削除 */

	/* 受信処理(MainFrameRecvProcBATTLE.cpp) */
	void	RecvProcBATTLE						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* 戦闘系 */
	void	RecvProcBATTLE_REQ_ATACK			(PBYTE pData, DWORD dwSessionID);					/* 攻撃要求 */


private:
	BYTE				m_byLastSendClock;				/* 最後にお知らせした時刻 */
	DWORD				m_dwServerStartTime,			/* サーバー起動時間 */
						m_dwLastKeepaliveCheck;			/* 最後に生存確認チェックした時間 */
	HWND				m_hWnd;							/* ウィンドウハンドル */
	HFONT				m_hFont;						/* サーバー状態の描画に使うフォント */

	CMgrData			*m_pMgrData;					/* データ管理 */
	CUraraSockTCPSBO	*m_pSock;						/* 通信マネージャ */
	CTextOutput			*m_pLog;						/* ログファイル */
	CUpdateServerInfo	*m_pUpdateServerInfo;			/* サーバー情報アップロード */

	CLibInfoAccount		*m_pLibInfoAccount;				/* アカウント情報ライブラリ */
	CLibInfoCharSvr		*m_pLibInfoChar;				/* キャラ情報ライブラリ */
	CLibInfoMapBase		*m_pLibInfoMap;					/* マップ情報ライブラリ */
	CLibInfoMapObject	*m_pLibInfoMapObject;			/* マップオブジェクト情報ライブラリ */
	CLibInfoMapParts	*m_pLibInfoMapParts;			/* マップパーツ情報ライブラリ */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;			/* マップ影情報ライブラリ */
	CLibInfoItemType	*m_pLibInfoItemType;			/* アイテム種別情報ライブラリ */
	CLibInfoItem		*m_pLibInfoItem;				/* アイテム情報ライブラリ */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;			/* 武器情報ライブラリ */
	CLibInfoEffect		*m_pLibInfoEffect;				/* エフェクト情報ライブラリ */
	CLibInfoMotion		*m_pLibInfoMotion;				/* モーション情報ライブラリ */
	CLibInfoMotionType	*m_pLibInfoMotionType;			/* モーション種別情報ライブラリ */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;			/* 噴出し情報ライブラリ */
	CLibInfoSystem		*m_pLibInfoSystem;				/* システム情報ライブラリ */
	CLibInfoSkill		*m_pLibInfoSkill;				/* スキル情報ライブラリ */
};

/* Copyright(C)URARA-works 2006 */
