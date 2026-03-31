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

private:
	// Win32 メッセージをポーリングする（WM_MAINFRAME等の独自メッセージ用）
	void	PollWin32Messages(void);

private:
	CSDLWindow	m_Window;		// SDLウィンドウ
	BOOL		m_bInitialized;	// SDL初期化済みフラグ
};
