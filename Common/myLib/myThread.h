/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:myThread.h													 */
/* 内容			:スレッドクラス 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/28													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CmyThread
{
public:
			CmyThread();					/* コンストラクタ */
	virtual ~CmyThread();					/* デストラクタ */

	virtual BOOL Create		(void);					/* 作成 */
	virtual void Destroy	(void);					/* 破棄 */


protected:
	static	UINT __stdcall ThreadEntry(void* pParam);	/* エントリポイント */
	virtual	DWORD	ThreadInit(void);					/* 初期化 */
	virtual	void	ThreadMain(void);					/* メイン処理 */
	virtual	void	ThreadExit(void);					/* 終了処理 */


protected:
	BOOL		m_dwInitResult;			/* 初期化結果 */
	HANDLE		m_hThread,				/* スレッドハンドル */
				m_hInitEvent,			/* 初期化同期イベント */
				m_hExitEvent;			/* 終了要求イベント */
};

/* Copyright(C)URARA-works 2007 */
