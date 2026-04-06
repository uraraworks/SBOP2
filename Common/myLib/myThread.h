/// @file myThread.h
/// @brief スレッドクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/28
/// @copyright Copyright(C)URARA-works 2007

#pragma once

// 非Windows環境向けSDL型前方宣言
// （SDL.hはmyThread.cppのみでインクルード、ヘッダはtypedefのみ）
#ifndef _WIN32
struct SDL_Thread;
struct SDL_semaphore;
typedef struct SDL_semaphore SDL_sem;
#endif

// クラス宣言

class CmyThread
{
public:
			CmyThread();				// コンストラクタ
	virtual ~CmyThread();				// デストラクタ

	virtual BOOL Create(void);			// 作成
	virtual void Destroy(void);			// 破棄


protected:
	// スレッドエントリポイント（プラットフォーム別呼び出し規約）
	static
#ifdef _WIN32
	UINT __stdcall
#else
	int
#endif
	ThreadEntry(void* pParam);
	virtual	DWORD	ThreadInit(void);					// 初期化
	virtual	void	ThreadMain(void);					// メイン処理
	virtual	void	ThreadExit(void);					// 終了処理


protected:
	BOOL		m_dwInitResult;		// 初期化結果
#ifdef _WIN32
	HANDLE		m_hThread,			// スレッドハンドル
				m_hInitEvent,		// 初期化同期イベント
				m_hExitEvent;		// 終了要求イベント
#else
	SDL_Thread*	m_hThread;			// SDLスレッドハンドル
	SDL_sem*	m_hInitEvent;		// 初期化同期セマフォ（イベント代替）
	SDL_sem*	m_hExitEvent;		// 終了要求セマフォ（イベント代替）
#endif
};
