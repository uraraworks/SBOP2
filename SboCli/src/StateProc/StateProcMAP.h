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
class CImGuiMsgLog;
class CImGuiDbg;

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
	void OnPushSwapRejected(DWORD dwObjCharID);                // 入れ替わり中のボールがRES_PUSHで却下された時の後始末(S5)
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
	void  TimerProcAtackRepeat(void);                                                          // 時間処理(Xキー押しっぱなしの連続攻撃。docs/battle-redesign.md S2)
	void  TimerProcAtackAutoOff(void);                                                         // 時間処理(無操作で戦闘状態を自動解除。docs/battle-redesign.md S2)
	BOOL  StartLocalAtack(void);                                                               // 攻撃モーションをローカルで即時開始する(docs/battle-redesign.md S2)
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
	BOOL  TryMoveOrPushDirection(CInfoMapBase *pMap, int nDirection, int nPushDir, int nMovePixel, DWORD &dwPushObjCharIDOut); // 1方向の移動可否判定(押せる物なら押し予測を試みる。S3b。nMovePixelは今回の移動量px、入れ替わり継続判定に使う。S5)
	BOOL  TryPushObject(CInfoMapBase *pMap, DWORD dwObjCharID, int nPushDir);                // 押せる物をローカルで1px押せるか試す(予測。S3b)
	void  SendReqPush(DWORD dwObjCharID, int nPushDir, CInfoCharCli *pInfoObj, BOOL bRelease = FALSE, int nPushType = 0); // REQ_PUSHを送信する(S3b。bRelease=TRUEは押すのをやめた最後の送信。S4。nPushTypeはPUSHTYPE_*。S5)
	void  EndPushPredict(BOOL bSendFinal);                                                    // 押し予測を終える(S3b)
	BOOL  TryStartPushSwap(CInfoMapBase *pMap, CInfoCharCli *pInfoObj, int nDirection);        // ボールとの入れ替わり開始を試す(S5)
	void  EndPushSwapOnDirectionChange(int nNewPushDir);                                       // 入れ替わり中に本人が向きを変えた時の終了処理(S5)
	void  DiscardPushSwapState(BOOL bClearOwnership, LPCSTR pszReason = "");                    // 入れ替わり状態を消す(完了/向き変更/マップ切替/却下/タイムアウトの共通後始末。S5。
	                                                                                            // bClearOwnershipはボールのm_bPushPredicting/所有者/m_dwPushPredictEndTimeも一緒に消すか
	                                                                                            // pszReasonは診断ログ用の理由(done/dirchg/map/reject/timeout)
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
	BOOL  IsEnemyChar(CInfoCharCli *pInfoChar);                                                // NPCで攻撃対象になり得るか判定(docs/battle-redesign.md S3。PCはPvPマップ設定ができるまで敵扱いしない)
	DWORD GetFrontEnemyCharID(DWORD dwCharID, int nDirection);                                 // 攻撃の届く範囲の正面にいる敵キャラIDを取得(docs/battle-redesign.md S3)
	void  ProcAutoWalkToEvent(void);                                                           // イベントタイルへの自動歩行処理
	void  ResetPlayerMoveSyncState(void);                                                      // 自キャラ移動同期状態をリセット


public:
	void StartAutoWalkToEvent(int nTileX, int nTileY); // イベントタイルへの自動歩行開始
	void DrawImGui(void);              // ImGuiウィジェット描画（ブラウザ版）
	void DrawImGuiSub(int kind);       // ImGuiウィジェット描画（ネイティブサブウィンドウ）
#if defined(__EMSCRIPTEN__)
	void BrowserChatSubmit(const char *pszText, int nType); // DOM→C++ チャット送信ブリッジ
#endif


protected:
	BOOL  m_bChgScrollMode,        // スクロールモードの変更に入れるかチェック
	      m_bSendCheckMapEvent;    // マップイベントチェックを送信したか判定
	int   m_nScrollMode;           // スクロールモード
	int   m_nLastNotifiedBattleMode; // 最後にJSへ通知した戦闘モード状態(-1:未通知, 0/1)
	DWORD m_dwLastTimeMove,        // 最後に移動した時間
	      m_dwLastTimeKeepAlive,   // 最後に生存確認通知を送信した時間
	      m_dwLastTimeMoveSyncSend, // 最後に移動同期を送信した時間
	      m_dwLastBalloonID,       // 最後に使った噴出しID
	      m_dwLastKeyInput,        // 最後にキー入力した時間
	      m_dwLastAtackTime;       // 最後に攻撃を開始した時間(無操作5秒でBATTLE解除に使う。docs/battle-redesign.md S2)
	BOOL  m_bAtackKeyAutoRepeat;   // 今回のXキー押下が攻撃で始まったか(押しっぱなし連続攻撃の対象か。docs/battle-redesign.md S2)
	BOOL  m_bZKeyDefenseActive;   // 今回のZキー押下が防御を開始したか(離した時にDefenseOffするか付いて行い要求を送るかの判定に使う。docs/battle-redesign.md S3)
	BOOL  m_bMoveSyncActive;       // Dead Reckoning送信中か
	int   m_nMoveSyncDirection;    // 最後に送信した移動方向
	// S3b: 押し予測(REQ_PUSH)送信状態。docs/push-object-redesign.md 4章
	BOOL  m_bPushSyncActive;          // 押している最中(REQ_PUSH送信対象を持っている)か
	DWORD m_dwPushSyncObjCharID;      // 押している押せる物のCharID
	int   m_nPushSyncDirection;       // 最後に送信した押す向き(-1:未送信)
	DWORD m_dwLastTimePushSyncSend;   // 最後にREQ_PUSHを送信した時刻
	DWORD m_dwLastTimePushContact;    // 最後に押せる物へ接触して押せていた時刻(離れ検出の基準)
	// S3b: 接触喪失時の即時送信で重複送信しないための直近送信済み目標座標
	// (docs/push-object-redesign.md 4章。斜め移動で接触が切れた瞬間に毎フレーム
	// 送ってしまわないよう、同じ座標なら送らない)
	int   m_nLastPushSyncSentX,       // 直近に送信したREQ_PUSHの目標座標X
	      m_nLastPushSyncSentY;       // 直近に送信したREQ_PUSHの目標座標Y
	BOOL  m_bLastPushSyncSentValid;   // 上記が有効か(未送信/リセット直後はFALSE)
	// S4: 離した印(m_bRelease)を1回だけ送るためのフラグ。docs/push-object-redesign.md。
	// 座標が同じで送信を間引く場合でも、離した印はまだ送っていなければ1回は送る
	BOOL  m_bPushReleaseSent;
	// S5: 入れ替わり(SWAP。ボールのみ)予測状態。docs/push-object-redesign.md 2章7項
	BOOL  m_bPushSwapActive;       // 入れ替わり中か
	DWORD m_dwPushSwapObjCharID;   // 入れ替わり中のボールのCharID
	int   m_nPushSwapDirection;    // 開始時の本人の向きd(0-3)。以後この向きのまま
	POINT m_ptPushSwapP0;          // 開始時の本人座標P0
	POINT m_ptPushSwapB0;          // 開始時のボール座標B0
	DWORD m_dwLastTimePushSwapSend; // 最後にSWAP種別のREQ_PUSHを送信した時刻(1800ms放置で手放す。親レビュー指摘)
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
	CImGuiMsgLog    *m_pImGuiMsgLog;  // ImGui版メッセージログ
	CImGuiDbg       *m_pImGuiDbg;     // ImGui版デバッグ
} CStateProcMAP, *PCStateProcMAP;
