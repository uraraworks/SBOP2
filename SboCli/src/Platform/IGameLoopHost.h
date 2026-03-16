/* Copyright(C)URARA-works 2025 */
/* ========================================================================= */
/* ファイル名	:IGameLoopHost.h											 */
/* 内容			:ゲームループホスト インターフェース 定義					 */
/* 作成開始日	:2025/06/01													 */
/* 説明			:SDLApp から呼び出される純粋仮想インターフェース。			 */
/*				 CMainFrame が実装し、SDL基盤に依存しない形でゲームロジックを */
/*				 駆動する。													 */
/* 変更履歴		:Phase 3: OnDraw を HDC → SDL_Renderer* に変更				 */
/* ========================================================================= */

#pragma once

/* SDL_Renderer の前方宣言（SDL2 の typedef に合わせる）    */
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Window SDL_Window;

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
	virtual BOOL OnSDLInit(SDL_Window *pWindow) = 0;
	virtual void OnSDLFocusChanged(BOOL bActive) = 0;

	/* 1フレーム分のゲーム更新を行う                       */
	/* TimerProc + KeyProc 相当                            */
	/* 描画が必要なフレームならTRUEを返す                  */
	virtual BOOL OnFrame() = 0;
	virtual void OnSDLKeyUp(int vk) = 0;
	virtual void OnSDLMouseMove(int x, int y) = 0;
	virtual void OnSDLMouseLeftButtonDown(int x, int y, BOOL bDoubleClick) = 0;
	virtual void OnSDLMouseRightButtonDown(int x, int y, BOOL bDoubleClick) = 0;
	virtual void OnSDLMouseRightButtonDoubleClick(int x, int y) = 0;

	/* 描画を行う（SDL: MgrDraw::Draw(SDL_Renderer*) に相当）*/
	/* SDL_RenderClear は呼び出し元(SDLApp)が行う          */
	/* SDL_RenderPresent は実装側(MgrDraw::Draw)が行う     */
	virtual void OnDraw(SDL_Renderer *pRenderer) = 0;

	/* ゲーム終了判定（TRUEなら終了）                      */
	virtual BOOL IsQuit() = 0;

	/* ゲーム後片付け（OnDestroy 相当）                    */
	virtual void OnSDLDestroy() = 0;
};

/* Copyright(C)URARA-works 2025 */
