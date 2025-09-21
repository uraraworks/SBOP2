/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:StateProcBase.h											 */
/* 内容			:状態処理基底クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#pragma once

class CUraraSockTCPSBO;
class CMgrData;
class CMgrSound;
class CMgrDraw;
class CMgrLayer;
class CMgrWindow;
class CMgrKeyInput;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CStateProcBase
{
public:
			CStateProcBase();							/* コンストラクタ */
	virtual ~CStateProcBase();							/* デストラクタ */

	virtual void Create			(CMgrData *pMgrData, CUraraSockTCPSBO *pSock);	/* 作成 */

	virtual void Init				(void);								/* 初期化 */
	virtual BOOL TimerProc			(void);								/* 時間処理 */
	virtual void KeyProc			(BYTE byCode, BOOL bDown);			/* キー処理 */
	virtual void OnLButtonDown		(int x, int y);						/* メッセージハンドラ(WM_LBUTTONDOWN) */
	virtual void OnRButtonDown		(int x, int y);						/* メッセージハンドラ(WM_RBUTTONDOWN) */
	virtual void OnRButtonDblClk	(int x, int y);						/* メッセージハンドラ(WM_RBUTTONDBLCLK) */
	virtual void OnMouseMove		(int x, int y);						/* メッセージハンドラ(WM_MOUSEMOVE) */
	virtual void OnKeyDown			(UINT vk);							/* メッセージハンドラ(WM_KEYDOWN) */
	virtual void OnWindowMsg		(int nType, DWORD dwPara);			/* メッセージハンドラ(WM_WINDOWMSG) */
	virtual void OnMainFrame		(DWORD dwCommand, DWORD dwParam);	/* メッセージハンドラ(WM_MAINFRAME) */
	virtual void OnAdminMsg			(int nType, DWORD dwPara);			/* メッセージハンドラ(WM_ADMINMSG) */
			void OnMgrDraw			(int nCode, DWORD dwPara);			/* メッセージハンドラ(WM_MGRDRAW) */


protected:
	virtual BOOL OnUp		(BOOL bDown);							/* キーハンドラ(↑) */
	virtual BOOL OnDown		(BOOL bDown);							/* キーハンドラ(↓) */
	virtual BOOL OnLeft		(BOOL bDown);							/* キーハンドラ(←) */
	virtual BOOL OnRight	(BOOL bDown);							/* キーハンドラ(→) */
	virtual BOOL OnX		(BOOL bDown);							/* キーハンドラ(X) */
	virtual BOOL OnZ		(BOOL bDown);							/* キーハンドラ(Z) */
	virtual BOOL OnS		(BOOL bDown);							/* キーハンドラ(S) */
	virtual BOOL OnA		(BOOL bDown);							/* キーハンドラ(A) */
	virtual BOOL OnB		(BOOL bDown);							/* キーハンドラ(B) */
	virtual BOOL OnR		(BOOL bDown);							/* キーハンドラ(R) */
	virtual BOOL OnI		(BOOL bDown);							/* キーハンドラ(I) */
	virtual BOOL OnN		(BOOL bDown);							/* キーハンドラ(N) */
	virtual BOOL OnV		(BOOL bDown);							/* キーハンドラ(V) */
	virtual BOOL OnF		(BOOL bDown);							/* キーハンドラ(F) */
	virtual BOOL OnJ		(BOOL bDown);							/* キーハンドラ(J) */
	virtual BOOL OnL		(BOOL bDown);							/* キーハンドラ(L) */
	virtual BOOL OnAt		(BOOL bDown);							/* キーハンドラ(@) */
	virtual BOOL OnEnter	(BOOL bDown);							/* キーハンドラ(Enter) */
	virtual BOOL OnEscape	(BOOL bDown);							/* キーハンドラ(Escape) */
	virtual BOOL OnTab		(BOOL bDown);							/* キーハンドラ(Tab) */
	virtual BOOL OnShift	(BOOL bDown);							/* キーハンドラ(Shift) */
	virtual BOOL OnCtrl		(BOOL bDown);							/* キーハンドラ(Ctrl) */
	virtual BOOL OnSpace	(BOOL bDown);							/* キーハンドラ(Space) */
	virtual void OnMgrDrawSTART_FADEIN	(DWORD dwPara) {}			/* フェードイン開始 */
	virtual void OnMgrDrawSTART_FADEOUT	(DWORD dwPara) {}			/* フェードアウト開始 */
	virtual void OnMgrDrawEND_FADEIN	(DWORD dwPara) {}			/* フェードイン完了 */
	virtual void OnMgrDrawEND_FADEOUT	(DWORD dwPara) {}			/* フェードアウト完了 */


protected:
	HWND			m_hWndMain,							/* メインフレームのウィンドウハンドル */
					m_hWndAdmin;						/* 管理者ウィンドウのウィンドウハンドル */

	CUraraSockTCPSBO	*m_pSock;						/* 通信マネージャ */
	CMgrData			*m_pMgrData;					/* データ管理 */
	CMgrSound			*m_pMgrSound;					/* サウンド管理 */
	CMgrDraw			*m_pMgrDraw;					/* 描画管理 */
	CMgrLayer			*m_pMgrLayer;					/* レイヤー管理 */
	CMgrWindow			*m_pMgrWindow;					/* ウィンドウ管理 */
	CMgrKeyInput		*m_pMgrKeyInput;				/* キー入力管理 */
} CStateProcBase, *PCStateProcBase;

/* Copyright(C)URARA-works 2006 */
