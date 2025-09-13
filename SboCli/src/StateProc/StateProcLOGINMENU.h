/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:StateProcLOGINMENU.h										 */
/* 内容			:状態処理クラス(メニュー画面) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"

class CInfoAccount;
class CWindowLOGINMENU;
class CWindowSTATUS;
class CWindowSTYLESELECT;
class CWindowNAMEINPUT;
class CInfoCharCli;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CStateProcLOGINMENU : public CStateProcBase
{
public:
			CStateProcLOGINMENU();						/* コンストラクタ */
	virtual ~CStateProcLOGINMENU();						/* デストラクタ */

	void Init			(void);									/* 初期化 */
	void OnWindowMsg	(int nType, DWORD dwPara);				/* メッセージハンドラ(WM_WINDOWMSG) */
	void OnMainFrame	(DWORD dwCommand, DWORD dwParam);		/* メッセージハンドラ(WM_MAINFRAME) */


protected:
	BOOL	OnLeft							(BOOL bDown);			/* キーハンドラ(←) */
	BOOL	OnRight							(BOOL bDown);			/* キーハンドラ(→) */
	BOOL	OnX								(BOOL bDown);			/* キーハンドラ(X) */
	BOOL	OnZ								(BOOL bDown);			/* キーハンドラ(Z) */
	void	OnMgrDrawEND_FADEOUT			(DWORD dwPara);			/* フェードアウト完了 */
	void	OnWindowMsgLOGINMENU			(DWORD dwPara);			/* メニューウィンドウからの通知 */
	void	OnWindowMsgFAMILYTYPE			(DWORD dwPara);			/* 種族選択ウィンドウからの通知 */
	void	OnWindowMsgSTYLESELECT			(DWORD dwPara);			/* 容姿選択ウィンドウからの通知 */
	void	OnWindowMsgNAMEINPUT			(DWORD dwPara);			/* 名前入力ウィンドウからの通知 */


protected:
	DWORD	m_dwLastTimeKey;					/* 前回のキー処理時間 */
	int		m_nCountKeyRepeate,					/* キーリピート回数 */
			m_nSelect,							/* 選択位置 */
			m_nState;							/* 状態 */

	CInfoAccount		*m_pInfoAccount;		/* アカウント情報 */
	CWindowLOGINMENU	*m_pWindowLOGINMENU;	/* ログインメニューウィンドウ */
	CWindowSTATUS		*m_pWindowSTATUS;		/* ステータスウィンドウ */
	CWindowSTYLESELECT	*m_pWindowSTYLESELECT;	/* 容姿選択ウィンドウ */
	CWindowNAMEINPUT	*m_pWindowNAMEINPUT;	/* 名前入力 */

	CInfoCharCli		*m_pInfoCharCli;		/* 作成中のキャラ情報 */
} CStateProcLOGINMENU, *PCStateProcLOGINMENU;

/* Copyright(C)URARA-works 2006 */
