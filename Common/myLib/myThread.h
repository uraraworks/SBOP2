/// @file myThread.h
/// @brief スレッドクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/28
/// @copyright Copyright(C)URARA-works 2007

#pragma once

// クラス宣言

class CmyThread
{
public:
			CmyThread();				// コンストラクタ
	virtual ~CmyThread();				// デストラクタ

	virtual BOOL Create(void);			// 作成
	virtual void Destroy(void);			// 破棄


protected:
	static	UINT __stdcall ThreadEntry(void* pParam);	// エントリポイント
	virtual	DWORD	ThreadInit(void);					// 初期化
	virtual	void	ThreadMain(void);					// メイン処理
	virtual	void	ThreadExit(void);					// 終了処理


protected:
	BOOL		m_dwInitResult;		// 初期化結果
	HANDLE		m_hThread,			// スレッドハンドル
				m_hInitEvent,		// 初期化同期イベント
				m_hExitEvent;		// 終了要求イベント
};
