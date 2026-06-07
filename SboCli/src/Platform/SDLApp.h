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
#if !defined(__EMSCRIPTEN__)
#include "ImGuiSubWindow.h"
struct ImGuiContext;
class CImGuiMsgLog;
#endif

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

#if !defined(__EMSCRIPTEN__)
	// デバッグ/ログのサブウィンドウを生成・表示する（StateProcMAP::Init から呼ぶ）
	void	ShowImGuiSubWindows(void);
	// デバッグ/ログのサブウィンドウを破棄する（StateProcMAP デストラクタから呼ぶ）
	void	HideImGuiSubWindows(void);
	// グローバルインスタンス取得（StateProcMAP などから使用）
	static CSDLApp *GetInstance(void) { return s_pInstance; }
	// メッセージログポインタを設定する（ShowImGuiSubWindows / HideImGuiSubWindows から呼ぶ）
	void SetMsgLogPtr(CImGuiMsgLog *p) { m_pMsgLog = p; }
	// サブ窓 (デバッグ/ログ) のいずれかが OS キーボードフォーカスを持つか
	bool	IsSubWindowKeyboardFocused(void) const;
#endif

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
	ULONGLONG	m_ullUpdateAccumulated;	// 固定更新残り時間(ms * FPS)
	DWORD		m_dwTimeLast;		// 前回時刻
	DWORD		m_dwLastRenderTime;	// 前回描画時刻
	DWORD		m_dwTimeStart;		// FPS計測開始時刻
	BOOL		m_bDrawPending;		// 描画保留
	BOOL		m_bQuit;			// 終了要求
	BOOL		m_bDestroyCalled;	// Destroy通知済み
	BOOL		m_bImGuiInitialized;	// ImGui初期化済みフラグ
#if !defined(__EMSCRIPTEN__)
	ImGuiContext   *m_pMainCtx;		// メイン ImGuiContext
	CImGuiSubWindow *m_pSubDbg;		// デバッグサブウィンドウ
	CImGuiSubWindow *m_pSubLog;		// メッセージログサブウィンドウ
	CImGuiMsgLog   *m_pMsgLog;		// メッセージログ入力通知先
	static CSDLApp *s_pInstance;	// グローバルインスタンスポインタ
#endif
	DWORD		m_dwMainLoopCallCount;	// 1秒内の MainLoopThunk 呼出し回数カウント
	DWORD		m_dwOnFrameCallCount;	// 1秒内の OnFrame 呼出し回数カウント
	DWORD		m_dwMaxRunFrameThisSec;	// 1秒内の最大 RunFrame 時間（ms）
	DWORD		m_dwMaxDrawThisSec;	// 1秒内の最大 OnDraw 時間（ms）
};
