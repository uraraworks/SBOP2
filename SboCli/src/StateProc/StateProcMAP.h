/// @file StateProcMAP.h
/// @brief 状態処理クラス(マップ画面) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/10/01
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "StateProcBase.h"
#include "AdminApi/AdminUiLoader.h"

class CInfoCharCli;
class CInfoMapBase;
class CLibInfoCharCli;
class CLibInfoMapBase;
class CLibInfoItem;
#ifdef _WIN32
class CDlgMsgLog;
class CDlgDbg;
#endif

// クラス宣言

typedef class CStateProcMAP : public CStateProcBase
{
public:
			CStateProcMAP();  // コンストラクタ
	virtual ~CStateProcMAP(); // デストラクタ

	void Create(CMgrData *pMgrData, CUraraSockTCPSBO *pSock); // 作成
	void Init(void);                                           // 初期化
	void GetMsgLogRect(RECT &rcDst);                           // メッセージログウィンドウの矩形を取得
	void SyncLastEventTile(DWORD dwMapID, int x, int y);       // イベント再発火抑止用の基準タイルを同期
	void ResetMapEventCheckSendState(void);                    // マップイベント送信状態をリセット
	BOOL TimerProc(void);                                      // 時間処理
	void KeyProc(BYTE byCode, BOOL bDown);                     // キー処理
	void OnWindowMsg(int nType, DWORD dwPara);                 // メッセージハンドラ(WM_WINDOWMSG)
	void OnLButtonDown(int x, int y);                          // メッセージハンドラ(WM_LBUTTONDOWN)
	void OnRButtonDown(int x, int y);                          // メッセージハンドラ(WM_RBUTTONDOWN)
	void OnRButtonDblClk(int x, int y);                        // メッセージハンドラ(WM_RBUTTONDBLCLK)
	void OnMouseMove(int x, int y);                            // メッセージハンドラ(WM_MOUSEMOVE)
	void OnMainFrame(DWORD dwCommand, DWORD dwParam);          // メッセージハンドラ(WM_MAINFRAME)
	void OnAdminMsg(int nType, DWORD dwPara);                  // メッセージハンドラ(WM_ADMINMSG)


protected:
	void  TimerProcSleepTimer(void);                                                           // 時間処理(おひるねタイマー)
	void  TimerProcChargeAtack(void);                                                          // 時間処理(溜め攻撃)
	void  TimerProcGauge(void);                                                                // 時間処理(ゲージ回復)
	BOOL  IsKeyInputEnable(void);                                                              // キー入力を許可するか判定
	BOOL  OnUp(BOOL bDown);                                                                    // キーハンドラ(↑)
	BOOL  OnDown(BOOL bDown);                                                                  // キーハンドラ(↓)
	BOOL  OnLeft(BOOL bDown);                                                                  // キーハンドラ(←)
	BOOL  OnRight(BOOL bDown);                                                                 // キーハンドラ(→)
	BOOL  OnX(BOOL bDown);                                                                     // キーハンドラ(X)
	BOOL  OnZ(BOOL bDown);                                                                     // キーハンドラ(Z)
	BOOL  OnS(BOOL bDown);                                                                     // キーハンドラ(S)
	BOOL  OnR(BOOL bDown);                                                                     // キーハンドラ(R)
	BOOL  OnB(BOOL bDown);                                                                     // キーハンドラ(B)
	BOOL  OnN(BOOL bDown);                                                                     // キーハンドラ(N)
	BOOL  OnV(BOOL bDown);                                                                     // キーハンドラ(V)
	BOOL  OnF(BOOL bDown);                                                                     // キーハンドラ(F)
	BOOL  OnJ(BOOL bDown);                                                                     // キーハンドラ(J)
	BOOL  OnL(BOOL bDown);                                                                     // キーハンドラ(L)
	BOOL  OnAt(BOOL bDown);                                                                    // キーハンドラ(@)
	BOOL  OnEnter(BOOL bDown);                                                                 // キーハンドラ(Enter)
	BOOL  OnEscape(BOOL bDown);                                                                // キーハンドラ(Escape)
	BOOL  OnTab(BOOL bDown);                                                                   // キーハンドラ(Tab)
	BOOL  OnShift(BOOL bDown);                                                                 // キーハンドラ(Shift)
	BOOL  OnCtrl(BOOL bDown);                                                                  // キーハンドラ(Ctrl)
	BOOL  OnSpace(BOOL bDown);                                                                 // キーハンドラ(Space)
	int   GetPlayerMoveStep(DWORD dwNowTime, int &nAccumOut, DWORD &dwLastStepTimeOut);        // 自キャラの1更新あたり移動量を取得
	int   GetSmoothedMoveDirection(int nTargetDirection, DWORD dwNowTime);                     // 慣性付きの移動方向を取得
	void  OnMgrDrawSTART_FADEIN(DWORD dwPara);                                                 // フェードイン開始
	void  OnMgrDrawEND_FADEIN(DWORD dwPara);                                                   // フェードイン完了
	BOOL  MoveProc(int x, int y, int xx, int yy, int nDirection, BOOL bSyncSend = TRUE);      // 移動処理
	BOOL  OnWindowMsgCHAT(DWORD dwPara);                                                      // チャット入力
	BOOL  OnWindowMsgSYSTEMMENU(DWORD dwPara);                                                // システムメニュー
	BOOL  OnWindowMsgSETCOLOR(DWORD dwPara);                                                  // 名前と発言色の設定
	BOOL  OnWindowMsgSETDRAWMODE(DWORD dwPara);                                               // 表示内容の設定
	BOOL  OnWindowMsgITEMMENU(DWORD dwPara);                                                  // アイテムメニュー
	BOOL  OnWindowMsgITEMMENU_SELECT(DWORD dwPara);                                           // アイテムどうするかメニュー
	BOOL  OnWindowMsgOPTION(DWORD dwPara);                                                    // オプション
	BOOL  OnWindowMsgOPTION_VIEWSET(DWORD dwPara);                                            // オプション-表示設定
	BOOL  OnWindowMsgOPTION_INPUTSET(DWORD dwPara);                                           // オプション-入力設定
	BOOL  OnWindowMsgOPTION_TASKBAR(DWORD dwPara);                                            // オプション(発言時にタスクバーチカチカ)
	BOOL  OnWindowMsgOPTION_INPUTSET_SETDEVICE(DWORD dwPara);                                 // オプション-入力設定-入力デバイスの設定
	BOOL  OnWindowMsgOPTION_ACTIONSET(DWORD dwPara);                                          // オプション-動作設定
	BOOL  OnWindowMsgOPTION_ACTIONSET_SLEEPTIMER(DWORD dwPara);                               // オプション-動作設定-おひるねタイマーの設定
	BOOL  OnWindowMsgCOMMANDMENU(DWORD dwPara);                                               // コマンドメニュー
	BOOL  OnWindowMsgSWOON(DWORD dwPara);                                                     // 気絶メニュー
	BOOL  OnWindowMsgSKILLMENU(DWORD dwPara);                                                 // スキルメニュー
	void  OnMainFrameRENEWITEMINFO(DWORD dwItemID);                                           // メッセージハンドラ(WM_MAINFRAME)[アイテム情報更新]
	void  OnMainFrameRENEWTALKEVENT(DWORD dwParam);                                           // メッセージハンドラ(WM_MAINFRAME)[会話イベント情報更新]
	void  CreateAdminUi(void);                                                                 // 管理UIを作成
	void  DestroyAdminUi(void);                                                                // 管理UIを破棄
	void  PostAdminUiMessage(UINT message, WPARAM wParam, LPARAM lParam);                      // 管理UIへメッセージ通知
	BOOL  OnXChar(DWORD dwCharID);                                                             // キャラにXキーを押した時の処理
	void  AddSystemMsg(BOOL bAddLog, LPCSTR pszMsg, COLORREF cl);                              // システムメッセージを追加
	void  DefenseOff(void);                                                                    // 防御解除
	DWORD GetTalkCharID(DWORD dwCharID, int nDirection);                                       // 会話できるキャラIDを取得
	void  ProcAutoWalkToEvent(void);                                                           // イベントタイルへの自動歩行処理
	void  ResetPlayerMoveSyncState(void);                                                      // 自キャラ移動同期状態をリセット


public:
	void StartAutoWalkToEvent(int nTileX, int nTileY); // イベントタイルへの自動歩行開始


protected:
	BOOL  m_bChgScrollMode,        // スクロールモードの変更に入れるかチェック
	      m_bSendCheckMapEvent;    // マップイベントチェックを送信したか判定
	int   m_nScrollMode;           // スクロールモード
	DWORD m_dwLastTimeMove,        // 最後に移動した時間
	      m_dwLastTimeKeepAlive,   // 最後に生存確認通知を送信した時間
	      m_dwLastTimeGauge,       // ゲージ回復時間
	      m_dwLastTimeMoveSyncSend, // 最後に移動同期を送信した時間
	      m_dwLastBalloonID,       // 最後に使った噴出しID
	      m_dwLastKeyInput,        // 最後にキー入力した時間
	      m_dwStartChargeTime;     // 溜め開始時間
	BOOL  m_bMoveSyncActive;       // Dead Reckoning送信中か
	int   m_nMoveSyncDirection;    // 最後に送信した移動方向
	int   m_nMoveSpeedAccum;       // 自キャラ速度のサブピクセル累積
	DWORD m_dwLastPlayerMoveStepTime; // 自キャラ速度計算の前回時刻
	DWORD m_dwLastPlayerMoveTurnTime; // 自キャラの回頭計算の前回時刻
	DWORD m_dwLastEventMapID;      // 前回イベント判定マップID
	BOOL  m_bHasLastEventTile;     // 前回イベント判定タイルを保持しているか
	BOOL  m_bHasPlayerMoveHeading; // 自キャラの移動方位を保持しているか
	BOOL  m_bNeedIdleMapEventCheck; // 停止直後のイベント判定が必要か
	int   m_nLastEventTileX,       // 前回イベント判定タイルX
	      m_nLastEventTileY;       // 前回イベント判定タイルY
	double m_dPlayerMoveHeading;   // 自キャラの現在移動方位(rad)
	BOOL  m_bAutoWalkToEvent;      // イベントタイルへ自動歩行中
	int   m_nAutoWalkTargetX,      // 自動歩行目標X（ピクセル）
	      m_nAutoWalkTargetY;      // 自動歩行目標Y（ピクセル）

	CAdminUiLoader  m_AdminUi;         // 管理者UIモジュール
	CInfoCharCli   *m_pPlayerChar;     // 操作中のキャラ情報
	CInfoMapBase   *m_pMap;            // 表示中のマップ情報

	CLibInfoCharCli *m_pLibInfoChar;   // キャラ情報ライブラリ
	CLibInfoMapBase *m_pLibInfoMap;    // マップ情報ライブラリ
	CLibInfoItem    *m_pLibInfoItem;   // アイテム情報ライブラリ
#ifdef _WIN32
	CDlgMsgLog      *m_pDlgMsgLog;    // メッセージログウィンドウ
	CDlgDbg         *m_pDlgDbg;       // デバッグウィンドウ
#endif
} CStateProcMAP, *PCStateProcMAP;
