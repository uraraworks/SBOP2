/// @file SDLApp.h
/// @brief SDLアプリケーション 定義ファイル
/// @date 2025/06/01
/// @copyright Copyright(C)URARA-works 2025
/// @details SDL2 の初期化・イベントループ・タイミング制御を担う。
///          既存 CMainFrame::MainLoop の役割を引き継ぐ。
///          ゲームロジックは IGameLoopHost 経由で呼び出す。

#pragma once

#include "SDLWindow.h"
#include "IGameLoopHost.h"

// クラス宣言

class CSDLApp
{
public:
			CSDLApp();				// コンストラクタ
	virtual ~CSDLApp();				// デストラクタ

	// SDL初期化（Init後にRunを呼ぶ）
	BOOL	Init(void);

	// SDLを終了する
	void	Destroy(void);

	// メインループを実行する
	// pHost: IGameLoopHost実装（CMainFrame）
	// pszTitle: ウィンドウタイトル（ANSI）
	// nWidth, nHeight: クライアント領域サイズ
	int		Run(IGameLoopHost *pHost, const char *pszTitle, int nWidth, int nHeight);
#if defined(__EMSCRIPTEN__)
	void	RequestBrowserRedraw(void);
#endif
	BYTE	GetFps(void) const { return m_byFpsLast; }

private:
	void	RunFrame(void);
#if defined(__EMSCRIPTEN__)
	static void MainLoopThunk(void *pArg);
#endif

	// Win32 メッセージをポーリングする（未縮退のネイティブ通知用）
	void	PollWin32Messages(void);

	void	InitImGui(SDL_Renderer *pRenderer);
	void	ShutdownImGui(void);

private:
	CSDLWindow	m_Window;		// SDLウィンドウ
	BOOL		m_bInitialized;	// SDL初期化済みフラグ
	IGameLoopHost *m_pHost;		// 実行中ホスト
	BYTE		m_byFps;		// FPSカウンタ
	BYTE		m_byFpsLast;		// 直近1秒間のFPS値
	DWORD		m_dwUpdateInterval;	// 更新間隔
	DWORD		m_dwRenderInterval;	// 描画間隔
	DWORD		m_dwAccumulated;	// 固定更新残り時間
	DWORD		m_dwTimeLast;		// 前回時刻
	DWORD		m_dwLastRenderTime;	// 前回描画時刻
	DWORD		m_dwTimeStart;		// FPS計測開始時刻
	BOOL		m_bDrawPending;		// 描画保留
	BOOL		m_bQuit;			// 終了要求
	BOOL		m_bDestroyCalled;	// Destroy通知済み
	BOOL		m_bImGuiInitialized;	// ImGui初期化済みフラグ
};
