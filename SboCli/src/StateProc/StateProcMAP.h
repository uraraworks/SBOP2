/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:StateProcMAP.h												 */
/* 内容			:状態処理クラス(マップ画面) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"

class CInfoCharCli;
class CInfoMapBase;
class CLibInfoCharCli;
class CLibInfoMapBase;
class CLibInfoItem;
class CDlgMsgLog;
class CAdminWindow;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CStateProcMAP : public CStateProcBase
{
public:
			CStateProcMAP();										/* コンストラクタ */
	virtual ~CStateProcMAP();										/* デストラクタ */

	void Create				(CMgrData *pMgrData, CUraraSockTCPSBO *pSock);	/* 作成 */
	void Init				(void);											/* 初期化 */
	void GetMsgLogRect		(RECT &rcDst);									/* メッセージログウィンドウの矩形を取得 */
	BOOL TimerProc			(void);											/* 時間処理 */
	void KeyProc			(BYTE byCode, BOOL bDown);						/* キー処理 */
	void OnWindowMsg		(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_WINDOWMSG) */
	void OnLButtonDown		(int x, int y);									/* メッセージハンドラ(WM_LBUTTONDOWN) */
	void OnRButtonDown		(int x, int y);									/* メッセージハンドラ(WM_RBUTTONDOWN) */
	void OnRButtonDblClk	(int x, int y);									/* メッセージハンドラ(WM_RBUTTONDBLCLK) */
	void OnMouseMove		(int x, int y);									/* メッセージハンドラ(WM_MOUSEMOVE) */
	void OnMainFrame		(DWORD dwCommand, DWORD dwParam);				/* メッセージハンドラ(WM_MAINFRAME) */
	void OnAdminMsg			(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */


protected:
	void	TimerProcSleepTimer			(void);											/* 時間処理(おひるねタイマー) */
	void	TimerProcChargeAtack		(void);											/* 時間処理(溜め攻撃) */
	void	TimerProcGauge				(void);											/* 時間処理(ゲージ回復) */
	BOOL	IsKeyInputEnable			(void);											/* キー入力を許可するか判定 */
	BOOL	OnUp						(BOOL bDown);									/* キーハンドラ(↑) */
	BOOL	OnDown						(BOOL bDown);									/* キーハンドラ(↓) */
	BOOL	OnLeft						(BOOL bDown);									/* キーハンドラ(←) */
	BOOL	OnRight						(BOOL bDown);									/* キーハンドラ(→) */
	BOOL	OnX							(BOOL bDown);									/* キーハンドラ(X) */
	BOOL	OnZ							(BOOL bDown);									/* キーハンドラ(Z) */
	BOOL	OnS							(BOOL bDown);									/* キーハンドラ(S) */
	BOOL	OnR							(BOOL bDown);									/* キーハンドラ(R) */
	BOOL	OnB							(BOOL bDown);									/* キーハンドラ(B) */
	BOOL	OnN							(BOOL bDown);									/* キーハンドラ(N) */
	BOOL	OnAt						(BOOL bDown);									/* キーハンドラ(@) */
	BOOL	OnEnter						(BOOL bDown);									/* キーハンドラ(Enter) */
	BOOL	OnEscape					(BOOL bDown);									/* キーハンドラ(Escape) */
	BOOL	OnTab						(BOOL bDown);									/* キーハンドラ(Tab) */
	BOOL	OnShift						(BOOL bDown);									/* キーハンドラ(Shift) */
	BOOL	OnCtrl						(BOOL bDown);									/* キーハンドラ(Ctrl) */
	BOOL	OnSpace						(BOOL bDown);									/* キーハンドラ(Space) */
	void	OnMgrDrawSTART_FADEIN		(DWORD dwPara);									/* フェードイン開始 */
	BOOL	MoveProc					(int x, int y, int xx, int yy, int nDirection);	/* 移動処理 */
	BOOL	OnWindowMsgCHAT				(DWORD dwPara);									/* チャット入力 */
	BOOL	OnWindowMsgSYSTEMMENU		(DWORD dwPara);									/* システムメニュー */
	BOOL	OnWindowMsgSETCOLOR			(DWORD dwPara);									/* 名前と発言色の設定 */
	BOOL	OnWindowMsgSETDRAWMODE		(DWORD dwPara);									/* 表示内容の設定 */
	BOOL	OnWindowMsgITEMMENU			(DWORD dwPara);									/* アイテムメニュー */
	BOOL	OnWindowMsgITEMMENU_SELECT	(DWORD dwPara);									/* アイテムどうするかメニュー */
	BOOL	OnWindowMsgOPTION			(DWORD dwPara);									/* オプション */
	BOOL	OnWindowMsgOPTION_VIEWSET	(DWORD dwPara);									/* オプション-表示設定 */
	BOOL	OnWindowMsgOPTION_INPUTSET	(DWORD dwPara);									/* オプション-入力設定 */
	BOOL	OnWindowMsgOPTION_TASKBAR	(DWORD dwPara);									/* オプション(発言時にタスクバーチカチカ) */
	BOOL	OnWindowMsgOPTION_INPUTSET_SETDEVICE	(DWORD dwPara);						/* オプション-入力設定-入力デバイスの設定 */
	BOOL	OnWindowMsgOPTION_ACTIONSET	(DWORD dwPara);									/* オプション-動作設定 */
	BOOL	OnWindowMsgOPTION_ACTIONSET_SLEEPTIMER	(DWORD dwPara);						/* オプション-動作設定-おひるねタイマーの設定 */
	BOOL	OnWindowMsgCOMMANDMENU		(DWORD dwPara);									/* コマンドメニュー */
	void	OnMainFrameRENEWITEMINFO	(DWORD dwItemID);								/* メッセージハンドラ(WM_MAINFRAME)[アイテム情報更新] */
	BOOL	OnXChar						(DWORD dwCharID);								/* キャラにXキーを押した時の処理 */
	void	AddSystemMsg				(BOOL bAddLog, LPCSTR pszMsg, COLORREF cl);		/* システムメッセージを追加 */
	void	DefenseOff					(void);											/* 防御解除 */


protected:
	BOOL			m_bChgScrollMode,					/* スクロールモードの変更に入れるかチェック */
					m_bSendCheckMapEvent;				/* マップイベントチェックを送信したか判定 */
	int				m_nScrollMode;						/* スクロールモード */
	DWORD			m_dwLastTimeMove,					/* 最後に移動した時間 */
					m_dwLastTimeKeepAlive,				/* 最後に生存確認通知を送信した時間 */
					m_dwLastTimeGauge,					/* ゲージ回復時間 */
					m_dwLastBalloonID,					/* 最後に使った噴出しID */
					m_dwLastKeyInput,					/* 最後にキー入力した時間 */
					m_dwStartChargeTime;				/* 溜め開始時間 */

	CAdminWindow	*m_pAdminWindow;					/* 管理者ウィンドウ */
	CInfoCharCli	*m_pPlayerChar;						/* 操作中のキャラ情報 */
	CInfoMapBase	*m_pMap;							/* 表示中のマップ情報 */

	CLibInfoCharCli	*m_pLibInfoChar;					/* キャラ情報ライブラリ */
	CLibInfoMapBase	*m_pLibInfoMap;						/* マップ情報ライブラリ */
	CLibInfoItem	*m_pLibInfoItem;					/* アイテム情報ライブラリ */
	CDlgMsgLog		*m_pDlgMsgLog;						/* メッセージログウィンドウ */
} CStateProcMAP, *PCStateProcMAP;

/* Copyright(C)URARA-works 2006 */
