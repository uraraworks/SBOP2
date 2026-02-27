/* Copyright(C)URARA-works 2025 */
/* ========================================================================= */
/* ファイル名	:IGameLoopHost.h											 */
/* 内容			:ゲームループホスト インターフェース 定義					 */
/* 作成開始日	:2025/06/01													 */
/* 説明			:SDLApp から呼び出される純粋仮想インターフェース。			 */
/*				 CMainFrame が実装し、SDL基盤に依存しない形でゲームロジックを */
/*				 駆動する。													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* インターフェース宣言														 */
/* ========================================================================= */

class IGameLoopHost
{
public:
	virtual ~IGameLoopHost() {}

	/* SDLウィンドウのHWNDを受け取ってゲーム側を初期化する */
	/* OnCreate / OnInitEnd 相当の処理をここで行う         */
	/* 失敗時はFALSEを返す                                 */
	virtual BOOL OnSDLInit(HWND hWnd) = 0;

	/* 1フレーム分のゲーム更新を行う                       */
	/* TimerProc + KeyProc 相当                            */
	/* 描画が必要なフレームならTRUEを返す                  */
	virtual BOOL OnFrame() = 0;

	/* 描画を行う（GDI: MgrDraw::Draw(HDC) に相当）        */
	virtual void OnDraw(HDC hDC) = 0;

	/* ゲーム終了判定（TRUEなら終了）                      */
	virtual BOOL IsQuit() = 0;

	/* ゲーム後片付け（OnDestroy 相当）                    */
	virtual void OnSDLDestroy() = 0;
};

/* Copyright(C)URARA-works 2025 */
