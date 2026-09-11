/// @file MainFrameWindowNone.cpp
/// @brief サーバーメインフレーム GUI(GDI/USER32)実装ファイル(非Windows 用の何もしない版)
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/04
/// @copyright Copyright(C)URARA-works 2006
///
/// MainFrameWindow.cpp(Windows 専用の GUI 実装)と対になるファイル。
/// ビルド側(.vcxproj / 移植チェックスクリプト)がどちらか一方だけを
/// コンパイル対象に選ぶ構成にしており、このファイル自体に #ifdef は書かない。
///
/// ここには CreateStateFont() / DestroyStateFont() / RefreshStateDisplay() /
/// MainLoopWindow() の4つだけを実装する。WndProc() や OnPaint() など残りの
/// ウィンドウメッセージハンドラは、ウィンドウ経路(WndProcEntry 経由)からしか
/// 呼ばれず、非Windows ビルドではウィンドウそのものを作らない
/// (MainLoopWindow が起動を拒否する)ためリンク時に参照されない。
/// そのためここでは何もしない版を用意しない。

#include "StdAfx.h"
#include "MainFrame.h"

// 状態表示用フォントを作成する
//
// ウィンドウが無い(=状態を描画する先が無い)ので何もしない。

void CMainFrame::CreateStateFont(void)
{
}

// 状態表示用フォントを破棄する
//
// CreateStateFont() が何も作らないので、破棄するものも無い。

void CMainFrame::DestroyStateFont(void)
{
}

// 状態表示の再描画を要求する
//
// 描画するウィンドウが無いので何もしない。

void CMainFrame::RefreshStateDisplay(void)
{
}

// メインループ(ウィンドウあり)
//
// ウィンドウ版はこのプラットフォームでは提供しない。--headless を
// 指定してヘッドレス経路(MainLoopHeadless)を使ってもらう。

int CMainFrame::MainLoopWindow(HINSTANCE hInstance)
{
	WriteConsoleMessage(_T("ウィンドウ版はこのプラットフォームでは使えません。--headless を指定してください"));
	return SBOSVR_EXIT_ERROR;
}
