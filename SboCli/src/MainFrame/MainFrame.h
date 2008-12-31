/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MainFrame.h												 */
/* 内容			:クライアントメインフレーム 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CUraraSockTCPSBO;
class CMgrGrpData;
class CMgrDraw;
class CMgrLayer;
class CMgrWindow;
class CMgrKeyInput;
class CMgrSound;
class CLibInfoCharCli;
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
class CStateProcBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CMainFrame
{
public:
			CMainFrame();									/* コンストラクタ */
	virtual ~CMainFrame();									/* デストラクタ */

	int		MainLoop		(HINSTANCE hInstance);							/* メインループ */
	void	DisConnectProc	(int nID);										/* 切断処理 */
	void	ChgMoveState	(BOOL bChgBGM);									/* プレイヤー移動状態変更処理 */
	void	RenewItemArea	(void);											/* 画面内のアイテム情報を更新 */
	void	SendChat		(int nType, LPCSTR pszMsg, DWORD *pdwDst=NULL);	/* チャット発言 */


private:
	static LRESULT CALLBACK WndProcEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	/* メインウィンドウプロシージャ(エントリポイント) */
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);							/* メインウィンドウプロシージャ */
	BOOL	OnCreate		(HWND hWnd, LPCREATESTRUCT lpCreateStruct);							/* メッセージハンドラ(WM_CREATE) */
	void	OnInitEnd		(HWND hWnd);														/* メッセージハンドラ(WM_INITEND) */
	void	OnClose			(HWND hWnd);														/* メッセージハンドラ(WM_CLOSE) */
	void	OnDestroy		(HWND hWnd);														/* メッセージハンドラ(WM_DESTROY) */
	void	OnPaint			(HWND hWnd);														/* メッセージハンドラ(WM_PAINT) */
	void	OnTimer			(HWND hWnd, UINT id);												/* メッセージハンドラ(WM_TIMER) */
	void	OnCommand		(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);					/* メッセージハンドラ(WM_COMMAND) */
	void	OnActivate		(HWND hWnd, UINT state, HWND hwndActDeact, BOOL fMinimized);		/* メッセージハンドラ(WM_ACTIVATE) */
	void	OnKeyUp			(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);			/* メッセージハンドラ(WM_KEYDOWN) */
	void	OnLButtonDown	(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);		/* メッセージハンドラ(WM_LBUTTONDOWN) */
	void	OnRButtonDown	(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);		/* メッセージハンドラ(WM_RBUTTONDOWN) */
	void	OnRButtonDblClk	(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);		/* メッセージハンドラ(WM_RBUTTONDBLCLK) */
	void	OnMouseMove		(HWND hWnd, int x, int y, UINT keyFlags);							/* メッセージハンドラ(WM_MOUSEMOVE) */
	void	OnMgrDraw		(int nType, DWORD dwPara);											/* メッセージハンドラ(WM_MGRDRAW) */
	void	OnWindowMsg		(int nType, DWORD dwPara);											/* メッセージハンドラ(WM_WINDOWMSG) */
	void	OnMainFrame		(DWORD dwCommand, DWORD dwParam);									/* メッセージハンドラ(WM_MAINFRAME) */
	void	OnAdminMsg		(int nType, DWORD dwPara);											/* メッセージハンドラ(WM_ADMINMSG) */
	void	OnRecvCommand	(DWORD dwMain, DWORD dwSub);										/* メッセージハンドラ(WM_RECVCOMMAND) */
	void	OnConnect		(void);																/* メッセージハンドラ(WM_URARASOCK_CONNECT) */
	void	OnDisConnect	(void);																/* メッセージハンドラ(WM_URARASOCK_DISCONNECT) */
	void	OnRecv			(PBYTE pData);														/* メッセージハンドラ(WM_URARASOCK_RECV) */

	BOOL	TimerProc		(void);								/* 時間処理 */
	void	KeyProc			(void);								/* キー入力処理 */
	void	ChgGameState	(int nGameState);					/* ゲーム状態を切り替える */
	void	Connect			(void);								/* 接続 */
	void	FlashMainWindow	(void);								/* ウィンドウを点滅させる */
	int		GetMsgCmdType	(LPCSTR pszText);					/* メッセージコマンド種別を取得 */

	/* 受信処理(MainFrameRecvProcVERSION.cpp) */
	void	RecvProcVERSION						(BYTE byCmdSub, PBYTE pData);	/* バージョン系 */
	void	RecvProcVERSION_RES_VERSIONCHECK	(PBYTE pData);					/* バージョンチェック応答 */
	void	RecvProcVERSION_RES_FILELISTCHECK	(PBYTE pData);					/* ファイルリストチェック応答 */

	/* 受信処理(MainFrameRecvProcCONNECT.cpp) */
	void	RecvProcCONNECT						(BYTE byCmdSub, PBYTE pData);	/* 接続系 */
	void	RecvProcCONNECT_RES_LOGIN			(PBYTE pData);					/* ログイン応答 */
	void	RecvProcCONNECT_RES_PLAY			(PBYTE pData);					/* ゲーム開始応答 */

	/* 受信処理(MainFrameRecvProcACCOUNT.cpp) */
	void	RecvProcACCOUNT						(BYTE byCmdSub, PBYTE pData);	/* アカウント系 */
	void	RecvProcACCOUNT_RES_ACCOUNTINFO		(PBYTE pData);					/* アカウント情報応答 */
	void	RecvProcACCOUNT_RES_MAKECHAR		(PBYTE pData);					/* キャラ作成応答 */

	/* 受信処理(MainFrameRecvProcCHAR.cpp) */
	void	RecvProcCHAR						(BYTE byCmdSub, PBYTE pData);	/* キャラ系 */
	void	RecvProcCHAR_RES_CHARINFO			(PBYTE pData);					/* キャラ情報応答 */
	void	RecvProcCHAR_CHARINFO				(PBYTE pData);					/* キャラ情報通知 */
	void	RecvProcCHAR_MOVEPOS				(PBYTE pData);					/* 移動通知 */
	void	RecvProcCHAR_STATE					(PBYTE pData);					/* 状態通知 */
	void	RecvProcCHAR_PROCSTATE				(PBYTE pData);					/* 行動状態通知 */
	void	RecvProcCHAR_CHAT					(PBYTE pData);					/* チャット通知 */
	void	RecvProcCHAR_CHARID					(PBYTE pData);					/* キャラID通知 */
	void	RecvProcCHAR_RES_PUTGET				(PBYTE pData);					/* アイテムを拾う置く応答 */
	void	RecvProcCHAR_SET_EFFECT				(PBYTE pData);					/* エフェクトを指定 */
	void	RecvProcCHAR_ITEMINFO				(PBYTE pData);					/* アイテム情報通知 */
	void	RecvProcCHAR_RES_TAIL				(PBYTE pData);					/* 付いて行く応答 */
	void	RecvProcCHAR_MODIFY_PARAM			(PBYTE pData);					/* パラメータ変更通知 */
	void	RecvProcCHAR_GRP					(PBYTE pData);					/* 画像通知 */
	void	RecvProcCHAR_MOTION					(PBYTE pData);					/* モーション情報通知 */
	void	RecvProcCHAR_MOTIONTYPE				(PBYTE pData);					/* モーション種別情報通知 */
	void	RecvProcCHAR_SET_EFCBALLOON			(PBYTE pData);					/* 噴出しを指定 */
	void	RecvProcCHAR_SET_MOTION				(PBYTE pData);					/* モーションを指定 */
	void	RecvProcCHAR_RES_CHECKMAPEVENT		(PBYTE pData);					/* マップイベントチェック応答 */
	void	RecvProcCHAR_STATUS					(PBYTE pData);					/* ステータス情報通知 */
	void	RecvProcCHAR_TEXTEFFECT				(PBYTE pData);					/* 文字エフェクト通知 */
	void	RecvProcCHAR_STATE_CHARGE			(PBYTE pData);					/* 溜め状態通知 */
	void	RecvProcCHAR_RES_TALKEVENT			(PBYTE pData);					/* 会話イベント情報応答 */
	void	RecvProcCHAR_SKILLINFO				(PBYTE pData);					/* スキル情報通知 */

	/* 受信処理(MainFrameRecvProcMAP.cpp) */
	void	RecvProcMAP							(BYTE byCmdSub, PBYTE pData);	/* マップ系 */
	void	RecvProcMAP_RES_MAPINFO				(PBYTE pData);					/* マップ情報応答 */
	void	RecvProcMAP_ONLINE					(PBYTE pData);					/* オンライン数通知 */
	void	RecvProcMAP_SYSTEMMSG				(PBYTE pData);					/* システムメッセージ通知 */
	void	RecvProcMAP_FORMATMSG				(PBYTE pData);					/* フォーマットメッセージ通知 */
	void	RecvProcMAP_MAPOBJECT				(PBYTE pData);					/* マップオブジェクト情報通知 */
	void	RecvProcMAP_MAPOBJECTDATA			(PBYTE pData);					/* マップオブジェクト配置データ通知 */
	void	RecvProcMAP_DELETEMAPOBJECTDATA		(PBYTE pData);					/* マップオブジェクト配置データ削除 */
	void	RecvProcMAP_MAPPARTS				(PBYTE pData);					/* マップパーツ情報通知 */
	void	RecvProcMAP_SETPARTS				(PBYTE pData);					/* マップパーツ配置 */
	void	RecvProcMAP_RENEWMAPSIZE			(PBYTE pData);					/* マップサイズ更新 */
	void	RecvProcMAP_DELETEPARTS				(PBYTE pData);					/* マップパーツ削除 */
	void	RecvProcMAP_MAPSHADOW				(PBYTE pData);					/* マップ影情報通知 */
	void	RecvProcMAP_SETMAPSHADOW			(PBYTE pData);					/* マップ影配置 */
	void	RecvProcMAP_DELETEMAPSHADOW			(PBYTE pData);					/* マップ影削除 */
	void	RecvProcMAP_MAPNAME					(PBYTE pData);					/* マップ名通知 */
	void	RecvProcMAP_MAPEVENT				(PBYTE pData);					/* マップイベント情報通知 */
	void	RecvProcMAP_DELETEEVENT				(PBYTE pData);					/* マップイベント情報削除 */
	void	RecvProcMAP_FADEINOUT				(PBYTE pData);					/* フェードイン/アウト通知 */

	/* 受信処理(MainFrameRecvProcITEM.cpp) */
	void	RecvProcITEM						(BYTE byCmdSub, PBYTE pData);	/* アイテム系 */
	void	RecvProcITEM_RES_ITEMINFO			(PBYTE pData);					/* アイテム情報応答 */
	void	RecvProcITEM_ITEMINFO				(PBYTE pData);					/* アイテム情報通知 */
	void	RecvProcITEM_DELETEITEMINFO			(PBYTE pData);					/* アイテム情報削除 */
	void	RecvProcITEM_RES_ITEMTYPEINFO		(PBYTE pData);					/* アイテム種別情報応答 */
	void	RecvProcITEM_ITEMTYPEINFO			(PBYTE pData);					/* アイテム種別情報通知 */
	void	RecvProcITEM_DELETEITEMTYPEINFO		(PBYTE pData);					/* アイテム種別情報削除 */
	void	RecvProcITEM_RES_ITEMWEAPONINFO		(PBYTE pData);					/* 武器情報応答 */
	void	RecvProcITEM_ITEMWEAPONINFO			(PBYTE pData);					/* 武器情報通知 */

	/* 受信処理(MainFrameRecvProcADMIN.cpp) */
	void	RecvProcADMIN						(BYTE byCmdSub, PBYTE pData);	/* 管理者系 */
	void	RecvProcADMIN_RES_ADMINLEVEL		(PBYTE pData);					/* 管理者レベル応答 */
	void	RecvProcADMIN_RENEWADMINLEVEL		(PBYTE pData);					/* 管理者レベル更新 */
	void	RecvProcADMIN_PLAYSOUND				(PBYTE pData);					/* 効果音の再生 */
	void	RecvProcADMIN_CHAR_RES_ACCOUNT		(PBYTE pData);					/* アカウント情報応答 */
	void	RecvProcADMIN_CHAR_RES_ONLINE		(PBYTE pData);					/* オンライン中キャラ一覧応答 */

	/* 受信処理(MainFrameRecvProcEFFECT.cpp) */
	void	RecvProcEFFECT						(BYTE byCmdSub, PBYTE pData);	/* エフェクト系 */
	void	RecvProcEFFECT_RES_EFFECTINFO		(PBYTE pData);					/* エフェクト情報応答 */
	void	RecvProcEFFECT_EFFECTINFO			(PBYTE pData);					/* エフェクト情報通知 */
	void	RecvProcEFFECT_DELETEEFFECTINFO		(PBYTE pData);					/* エフェクト情報削除 */
	void	RecvProcEFFECT_BALLOONINFO			(PBYTE pData);					/* 噴出し情報通知 */

	/* 受信処理(MainFrameRecvProcSYSTEM.cpp) */
	void	RecvProcSYSTEM						(BYTE byCmdSub, PBYTE pData);	/* システム系 */
	void	RecvProcSYSTEM_INFO					(PBYTE pData);					/* システム情報通知 */

	/* 受信処理(MainFrameRecvProcSKILL.cpp) */
	void	RecvProcSKILL						(BYTE byCmdSub, PBYTE pData);	/* スキル系 */
	void	RecvProcSKILL_SKILLINFO				(PBYTE pData);					/* スキル情報通知 */


private:
	HWND				m_hWnd;					/* ウィンドウハンドル */
	BOOL				m_bWindowActive,		/* ウィンドウのアクティブ状態 */
						m_bRenewCharInfo;		/* キャラ情報更新 */
	int					m_nGameState,			/* ゲーム状態 */
						m_nDrawCount,			/* 秒間表示回数 */
						m_nFPS;					/* 1フレームの時間 */
	DWORD				m_dwLastTimeCheck,		/* 前回のチェック時間 */
						m_dwDrawTime;			/* 描画に使った時間 */
	HRESULT				m_hCom;
	SYSTEMTIME			m_stSystemTime;			/* 前回のチェック時刻 */

	CMgrData			*m_pMgrData;			/* データマネージャ */
	CUraraSockTCPSBO	*m_pSock;				/* 通信マネージャ */
	CMgrGrpData			*m_pMgrGrpData;			/* グラフィックデータマネージャ */
	CMgrDraw			*m_pMgrDraw;			/* 描画マネージャ */
	CMgrLayer			*m_pMgrLayer;			/* レイヤーマネージャ */
	CMgrWindow			*m_pMgrWindow;			/* ウィンドウマネージャ */
	CMgrKeyInput		*m_pMgrKeyInput;		/* キー入力マネージャ */
	CMgrSound			*m_pMgrSound;			/* サウンド管理 */
	CLibInfoCharCli		*m_pLibInfoChar;		/* キャラ情報ライブラリ */
	CLibInfoMapBase		*m_pLibInfoMap;			/* マップ情報ライブラリ */
	CLibInfoMapObject	*m_pLibInfoMapObject;	/* マップオブジェクト情報ライブラリ */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* マップパーツ情報ライブラリ */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;	/* マップ影情報ライブラリ */
	CLibInfoItemType	*m_pLibInfoItemType;	/* アイテム種別情報ライブラリ */
	CLibInfoItem		*m_pLibInfoItem;		/* アイテム情報ライブラリ */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	/* 武器情報ライブラリ */
	CLibInfoEffect		*m_pLibInfoEffect;		/* エフェクト情報ライブラリ */
	CLibInfoMotion		*m_pLibInfoMotion;		/* モーション情報ライブラリ */
	CLibInfoMotionType	*m_pLibInfoMotionType;	/* モーション種別情報ライブラリ */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;	/* 噴出し情報ライブラリ */
	CLibInfoSystem		*m_pLibInfoSystem;		/* システム情報ライブラリ */
	CLibInfoSkill		*m_pLibInfoSkill;		/* スキル情報ライブラリ */
	CStateProcBase		*m_pStateProc;			/* 状態処理 */
};

/* Copyright(C)URARA-works 2005 */
