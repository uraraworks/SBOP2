/// @file MainFrame.h
/// @brief サーバーメインフレーム 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/04
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include <deque>

class CPacketBase;
class CMgrData;
class CUraraSockTCPSBO;
class CTextOutput;
class CHttpServer;
class CWebSocketBridge;
class CLibInfoAccount;
class CLibInfoCharSvr;
class CLibInfoDisable;
class CLibInfoMapBase;
class CLibInfoMapObject;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItemType;
class CLibInfoItem;
class CLibInfoItemWeapon;
class CLibInfoEffect;
class CLibInfoMotion;
class CLibInfoMotionType;
class CLibInfoEfcBalloon;
class CLibInfoSystem;
class CLibInfoSkill;
class CLibInfoTalkEvent;
class CInfoCharBase;

// ソケット通知の種別

enum SOCKNOTIFY_TYPE {
	SOCKNOTIFY_ADDCLIENT = 0,	// クライアントが接続した
	SOCKNOTIFY_DECCLIENT,	// クライアントが切断した
	SOCKNOTIFY_RECV,	// 受信
	SOCKNOTIFY_DISCONNECT,	// こちらから切断する
};

// ソケット通知キューの要素
//
// 通信ライブラリは専用スレッドから通知してくるため、ここへ積んで
// メインスレッド(TimerProc)でまとめて捌く。直接ハンドラを呼ぶと
// ゲームロジックを別スレッドが触ることになるので厳禁。

struct SOCKNOTIFYINFO {
	SOCKNOTIFY_TYPE	Type;	// 種別
	DWORD	dwSessionID;	// セッションID
	PBYTE	pData;	// 受信データ(SOCKNOTIFY_RECV のみ有効。所有権はキューが持つ)
};

// プロセスの終了コード
//
// ヘッドレス運用では起動スクリプトが判定に使うため、
// 0 = 正常、それ以外 = 異常とする。

#define SBOSVR_EXIT_OK	(0)	// 正常終了
#define SBOSVR_EXIT_ERROR	(1)	// 初期化失敗、または停止対象が見つからない
#define SBOSVR_EXIT_ALREADY_RUNNING	(2)	// 同じポートのサーバーが既に起動している

// サーバー状態表示の項目
//
// 描画やウィンドウに依存しない形で状態を取り出すための入れ物。
// OnPaint 以外(コンソール出力やログ)からも同じ内容を使えるようにする。

struct SERVERSTATEITEM {
	CmyString	strLabel;	// 項目名
	CmyString	strValue;	// 値
};

#define SERVERSTATEITEM_MAX	(4)	// サーバー状態表示の項目数

// クラス宣言

class CMainFrame
{
public:
			CMainFrame();	// コンストラクタ
	virtual ~CMainFrame();	// デストラクタ

	int	MainLoop(HINSTANCE hInstance, BOOL bHeadless = FALSE);	// メインループ

	// 送信処理
	void	SendToScreenChar(CInfoCharBase *pInfoChar, CPacketBase *pPacket, BOOL bExcludeSelf = FALSE);	// 指定キャラと同じ画面のキャラへ送信
	void	SendToMapChar(DWORD dwMapID, CPacketBase *pPacket);	// 指定マップにいるキャラへ送信
	void	SendToAdminChar(CPacketBase *pPacket);	// 管理者権限を持っているキャラへ送信
	void	SendToClient(DWORD dwSessionID, CPacketBase *pPacket);	// セッションIDが有効な場合のみ送信

	void	RequestDisconnect(DWORD dwSessionID);	// 切断を予約する(即時ではなく次の TimerProc で処理)

	static	BOOL	RequestStopRunningServer(void);	// 稼働中のヘッドレスサーバーへ停止を要求する
	static	void	WriteConsoleMessage(LPCTSTR pszFormat, ...);	// 接続したコンソールへ出力する

private:
	int	MainLoopWindow(HINSTANCE hInstance);	// メインループ(ウィンドウあり)
	int	MainLoopHeadless(void);	// メインループ(ヘッドレス)

	BOOL	InitServer(void);	// サーバー初期化(ウィンドウに依存しない)
	void	TermServer(void);	// サーバー終了処理(ウィンドウに依存しない)
	void	RequestQuit(void)	{ m_bQuit = TRUE;	}	// 終了を要求する(別スレッドから呼ばれ得る)

	static	void	RequestQuitStatic(void);	// SboPlatform::InstallStopSignalHandler へ渡す: 終了要求を立てる
	static	bool	IsQuittingDoneStatic(void);	// SboPlatform::InstallStopSignalHandler へ渡す: 終了処理が完了したか

	void	LoadWindowPos(HWND hWnd);	// ウィンドウ位置を復元(ウィンドウありのみ)
	void	SaveWindowPos(HWND hWnd);	// ウィンドウ位置を保存(ウィンドウありのみ)

	void	CreateStateFont(void);	// 状態表示用フォントを作成(コンストラクタから呼ぶ。GUI 実装は MainFrameWindow.cpp / MainFrameWindowNone.cpp)
	void	DestroyStateFont(void);	// 状態表示用フォントを破棄(デストラクタから呼ぶ)
	void	RefreshStateDisplay(void);	// 状態表示の再描画を要求する(ウィンドウが無ければ何もしない)

	static	LRESULT CALLBACK WndProcEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	// メインウィンドウプロシージャ(エントリポイント)
			LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	// メインウィンドウプロシージャ
	BOOL	OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);	// メッセージハンドラ(WM_CREATE)
	void	OnClose(HWND hWnd);	// メッセージハンドラ(WM_CLOSE)
	void	OnDestroy(HWND hWnd);	// メッセージハンドラ(WM_DESTROY)
	void	OnPaint(HWND hWnd);	// メッセージハンドラ(WM_PAINT)
	void	OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);	// メッセージハンドラ(WM_COMMAND)
	void	OnAddClient(DWORD dwSessionID);	// 通知ハンドラ(クライアント接続)
	void	OnDecClient(DWORD dwSessionID);	// 通知ハンドラ(クライアント切断)
	void	OnRecv(PBYTE pData, DWORD dwSessionID);	// 通知ハンドラ(受信)
	void	OnDisconnect(DWORD dwSessionID);	// 通知ハンドラ(こちらから切断)

	static	void	OnSocketNotifyThunk(void *pUserData, UINT uMsgOffset, WPARAM wParam, LPARAM lParam);	// ソケット通知コールバック(ソケットスレッドから呼ばれる)
	void	PushSockNotify(const SOCKNOTIFYINFO &Info);	// ソケット通知をキューへ積む
	void	ProcSockNotify(void);	// 溜まったソケット通知を捌く(メインスレッド)
	void	ClearSockNotify(void);	// ソケット通知キューを破棄

	void	TimerProc(void);	// 時間処理
	void	TimerProcKeepalive(void);	// 時間処理(生存確認チェック)
	void	TimerProcClock(void);	// 時間処理(時報と状態表示の更新)
	void	TimerProcSave(void);	// 時間処理(定期保存)
	int	GetServerStateItem(SERVERSTATEITEM *paItem, int nMax);	// サーバー状態の表示項目を取得(描画に依存しない)
	void	MyTextOut(HDC hDC, int x, int y, LPCTSTR pStr);	// 文字列描画
	void	NotifyOnlineCount(void);	// オンライン数を全クライアントへ通知

	void	OnCommandUPDATE_RENEW(void);	// メニューハンドラ(アップデートファイル更新)

	// 受信処理(MainFrameRecvProcVERSION.cpp)
	void	RecvProcVERSION(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	// バージョン系
	void	RecvProcVERSION_REQ_VERSIONCHECK(PBYTE pData, DWORD dwSessionID);	// バージョンチェック要求
	void	RecvProcVERSION_REQ_FILELISTCHECK(PBYTE pData, DWORD dwSessionID);	// ファイルリストチェック要求
	void	RecvProcVERSION_REQ_FILELIST(PBYTE pData, DWORD dwSessionID);	// ファイルリスト要求
	void	RecvProcVERSION_REQ_FILE(PBYTE pData, DWORD dwSessionID);	// ファイル要求

	// 受信処理(MainFrameRecvProcCONNECT.cpp)
	void	RecvProcCONNECT(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	// 接続系
	void	RecvProcCONNECT_REQ_LOGIN(PBYTE pData, DWORD dwSessionID);	// ログイン要求
	void	RecvProcCONNECT_REQ_PLAY(PBYTE pData, DWORD dwSessionID);	// ゲーム開始要求
	void	RecvProcCONNECT_KEEPALIVE(PBYTE pData, DWORD dwSessionID);	// 生存確認通知

	// 受信処理(MainFrameRecvProcACCOUNT.cpp)
	void	RecvProcACCOUNT(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	// アカウント系
	void	RecvProcACCOUNT_REQ_ACCOUNTINFO(PBYTE pData, DWORD dwSessionID);	// アカウント情報要求
	void	RecvProcACCOUNT_REQ_MAKECHAR(PBYTE pData, DWORD dwSessionID);	// キャラ作成要求
	void	RecvProcACCOUNT_REQ_DELETECHAR(PBYTE pData, DWORD dwSessionID);	// キャラ削除要求

	// 受信処理(MainFrameRecvProcCHAR.cpp)
	void	RecvProcCHAR(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	// キャラ系
	void	RecvProcCHAR_REQ_CHARINFO(PBYTE pData, DWORD dwSessionID);	// キャラ情報要求
	void	RecvProcCHAR_MOVEPOS(PBYTE pData, DWORD dwSessionID);	// 移動通知
	void	RecvProcCHAR_STATE(PBYTE pData, DWORD dwSessionID);	// 状態通知
	void	RecvProcCHAR_REQ_CHAT(PBYTE pData, DWORD dwSessionID);	// チャット要求
	void	RecvProcCHAR_REQ_CHARINFO2(PBYTE pData, DWORD dwSessionID);	// キャラ情報要求(複数)
	void	RecvProcCHAR_REQ_PUTGET(PBYTE pData, DWORD dwSessionID);	// アイテムを拾う置く要求
	void	RecvProcCHAR_REQ_USEITEM(PBYTE pData, DWORD dwSessionID);	// アイテムを使う要求
	void	RecvProcCHAR_REQ_DRAGITEM(PBYTE pData, DWORD dwSessionID);	// アイテム位置変更要求
	void	RecvProcCHAR_REQ_PUSH(PBYTE pData, DWORD dwSessionID);	// 押す要求
	void	RecvProcCHAR_REQ_TAIL(PBYTE pData, DWORD dwSessionID);	// 付いて行く要求
	void	RecvProcCHAR_REQ_MODIFY_PARAM(PBYTE pData, DWORD dwSessionID);	// パラメータ変更要求
	void	RecvProcCHAR_REQ_EQUIP(PBYTE pData, DWORD dwSessionID);	// 装備要求
	void	RecvProcCHAR_PROC_FISHING(PBYTE pData, DWORD dwSessionID);	// 釣り要求
	void	RecvProcCHAR_REQ_CHECKMAPEVENT(PBYTE pData, DWORD dwSessionID);	// マップイベントチェック要求
	void	RecvProcCHAR_STATE_CHARGE(PBYTE pData, DWORD dwSessionID);	// 溜め状態通知
	void	RecvProcCHAR_REQ_RECOVERY(PBYTE pData, DWORD dwSessionID);	// 気絶後復活要求
	void	RecvProcCHAR_REQ_TALKEVENT(PBYTE pData, DWORD dwSessionID);	// 会話イベント情報要求
	void	RecvProcCHAR_REQ_ADDSKILL(PBYTE pData, DWORD dwSessionID);	// スキル追加要求
	void	RecvProcCHAR_REQ_USESKILL(PBYTE pData, DWORD dwSessionID);	// スキルを使う要求

	// 受信処理(MainFrameRecvProcMAP.cpp)
	void	RecvProcMAP(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	// マップ系
	void	RecvProcMAP_REQ_MAPINFO(PBYTE pData, DWORD dwSessionID);	// マップ情報要求

	// 受信処理(MainFrameRecvProcITEM.cpp)
	void	RecvProcITEM(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	// アイテム系
	void	RecvProcITEM_REQ_ITEMINFO(PBYTE pData, DWORD dwSessionID);	// アイテム情報要求
	void	RecvProcITEM_RENEWITEMINFO(PBYTE pData, DWORD dwSessionID);	// アイテム情報更新
	void	RecvProcITEM_REQ_ITEMTYPEINFO(PBYTE pData, DWORD dwSessionID);	// アイテム種別情報要求
	void	RecvProcITEM_RENEWITEMTYPEINFO(PBYTE pData, DWORD dwSessionID);	// アイテム種別情報更新
	void	RecvProcITEM_REQ_ITEMWEAPONINFO(PBYTE pData, DWORD dwSessionID);	// 武器情報要求

	// 受信処理(MainFrameRecvProcADMIN.cpp)
	void	RecvProcADMIN(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	// 管理者系
	void	RecvProcADMIN_CHARINFO(PBYTE pData, DWORD dwSessionID);	// キャラ情報通知
	void	RecvProcADMIN_DELETECHARINFO(PBYTE pData, DWORD dwSessionID);	// キャラ情報削除
	void	RecvProcADMIN_MAP_RENEWMAPOBJECT(PBYTE pData, DWORD dwSessionID);	// マップオブジェクト更新
	void	RecvProcADMIN_MAP_RENEWOBJECTDATA(PBYTE pData, DWORD dwSessionID);	// マップオブジェクト配置データ更新
	void	RecvProcADMIN_MAP_DELETEOBJECTDATA(PBYTE pData, DWORD dwSessionID);	// マップオブジェクト配置データ削除
	void	RecvProcADMIN_RENEWMAPPARTS(PBYTE pData, DWORD dwSessionID);	// マップパーツ更新
	void	RecvProcADMIN_MAP_SETPARTS(PBYTE pData, DWORD dwSessionID);	// マップパーツ配置
	void	RecvProcADMIN_MAP_RENEWMAPSIZE(PBYTE pData, DWORD dwSessionID);	// マップサイズ更新
	void	RecvProcADMIN_MAP_DELETEPARTS(PBYTE pData, DWORD dwSessionID);	// マップパーツ削除
	void	RecvProcADMIN_MAP_COPYPARTS(PBYTE pData, DWORD dwSessionID);	// マップパーツコピー
	void	RecvProcADMIN_MAP_SETMAPNAME(PBYTE pData, DWORD dwSessionID);	// マップ名変更
	void	RecvProcADMIN_RENEWMAPSHADOW(PBYTE pData, DWORD dwSessionID);	// マップ影更新
	void	RecvProcADMIN_MAP_SETMAPSHADOW(PBYTE pData, DWORD dwSessionID);	// マップ影配置
	void	RecvProcADMIN_MAP_DELETEMAPSHADOW(PBYTE pData, DWORD dwSessionID);	// マップ影削除
	void	RecvProcADMIN_MAP_RENEWEVENT(PBYTE pData, DWORD dwSessionID);	// マップイベント情報更新
	void	RecvProcADMIN_MAP_DELETEEVENT(PBYTE pData, DWORD dwSessionID);	// マップイベント情報削除
	void	RecvProcADMIN_MAP_ADD(PBYTE pData, DWORD dwSessionID);	// マップ追加
	void	RecvProcADMIN_REQ_ADMINILEVEL(PBYTE pData, DWORD dwSessionID);	// 管理者権限レベル要求
	void	RecvProcADMIN_RENEWADMINILEVEL(PBYTE pData, DWORD dwSessionID);	// 管理者権限レベル更新
	void	RecvProcADMIN_SERVER_SAVEINFO(PBYTE pData, DWORD dwSessionID);	// サーバー情報保存
	void	RecvProcADMIN_ITEM_ADD(PBYTE pData, DWORD dwSessionID);	// アイテム情報追加
	void	RecvProcADMIN_ITEM_COPY(PBYTE pData, DWORD dwSessionID);	// アイテム情報コピー
	void	RecvProcADMIN_ITEM_DELETE(PBYTE pData, DWORD dwSessionID);	// アイテム情報削除
	void	RecvProcADMIN_ITEMTYPE_ADD(PBYTE pData, DWORD dwSessionID);	// アイテム種別情報追加
	void	RecvProcADMIN_ITEMTYPE_COPY(PBYTE pData, DWORD dwSessionID);	// アイテム種別情報コピー
	void	RecvProcADMIN_ITEMTYPE_DELETE(PBYTE pData, DWORD dwSessionID);	// アイテム種別情報削除
	void	RecvProcADMIN_ITEMWEAPON_ADD(PBYTE pData, DWORD dwSessionID);	// 武器情報追加
	void	RecvProcADMIN_ITEMWEAPON_RENEW(PBYTE pData, DWORD dwSessionID);	// 武器情報更新
	void	RecvProcADMIN_CHAR_ADDNPC(PBYTE pData, DWORD dwSessionID);	// NPCの追加
	void	RecvProcADMIN_CHAR_MODIFYITEM(PBYTE pData, DWORD dwSessionID);	// 所持アイテムの変更
	void	RecvProcADMIN_CHAR_MODIFYSKILL(PBYTE pData, DWORD dwSessionID);	// 所持スキルの変更
	void	RecvProcADMIN_CHAR_RENEWMOTION(PBYTE pData, DWORD dwSessionID);	// キャラモーション情報の更新
	void	RecvProcADMIN_CHAR_ADDMOTIONTYPE(PBYTE pData, DWORD dwSessionID);	// キャラモーション種別情報の追加
	void	RecvProcADMIN_CHAR_RENEWMOTIONTYPE(PBYTE pData, DWORD dwSessionID);	// キャラモーション種別情報の更新
	void	RecvProcADMIN_CHAR_RENEWSTATUS(PBYTE pData, DWORD dwSessionID);	// ステータス情報更新
	void	RecvProcADMIN_CHAR_REQ_STATUS(PBYTE pData, DWORD dwSessionID);	// ステータス情報要求
	void	RecvProcADMIN_CHAR_REQ_ONLINE(PBYTE pData, DWORD dwSessionID);	// オンライン中キャラ一覧要求
	void	RecvProcADMIN_EFC_RENEWBALLOON(PBYTE pData, DWORD dwSessionID);	// 噴出し情報の更新
	void	RecvProcADMIN_EFC_RENEWEFFECT(PBYTE pData, DWORD dwSessionID);	// エフェクト情報の更新
	void	RecvProcADMIN_REQ_PLAYSOUND(PBYTE pData, DWORD dwSessionID);	// 効果音の再生要求
	void	RecvProcADMIN_CHAR_REQ_ACCOUNT(PBYTE pData, DWORD dwSessionID);	// アカウント情報要求
	void	RecvProcADMIN_CHAR_RENEW_ACCOUNT(PBYTE pData, DWORD dwSessionID);	// アカウント情報更新
	void	RecvProcADMIN_CHAR_RENEW_TALKEVENT(PBYTE pData, DWORD dwSessionID);	// 会話イベント情報更新
	void	RecvProcADMIN_RENEW_CLIENTVERSION(PBYTE pData, DWORD dwSessionID);	// クライアントバージョン更新
	void	RecvProcADMIN_SYSTEM_REQ_INFO(PBYTE pData, DWORD dwSessionID);	// システム情報要求
	void	RecvProcADMIN_SYSTEM_RENEWINFO(PBYTE pData, DWORD dwSessionID);	// システム情報の更新
	void	RecvProcADMIN_SKILL_RENEWSKILL(PBYTE pData, DWORD dwSessionID);	// スキル情報更新
	void	RecvProcADMIN_ACCOUNT_REQ_ADD(PBYTE pData, DWORD dwSessionID);	// アカウントの追加要求
	void	RecvProcADMIN_DISABLE_REQ_INFO(PBYTE pData, DWORD dwSessionID);	// 拒否情報要求
	void	RecvProcADMIN_DISABLE_REQ_DELETE(PBYTE pData, DWORD dwSessionID);	// 拒否情報の削除要求
	void	RecvProcADMIN_DISABLE_RENEWINFO(PBYTE pData, DWORD dwSessionID);	// 拒否情報の更新
	void	RecvProcADMIN_MAP_SELECTPICK(PBYTE pData, DWORD dwSessionID);		// マップ選択ピック
	void	RecvProcADMIN_MAP_SELECTCLEAR(PBYTE pData, DWORD dwSessionID);	// マップ選択解除

	// 受信処理(MainFrameRecvProcMSGCMD.cpp)
	void	RecvProcMSGCMD(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	// メッセージコマンド系
	void	RecvProcMSGCMD_CHGFACE(PBYTE pData, DWORD dwSessionID);	// 表情変更
	void	RecvProcMSGCMD_CHGHAIR(PBYTE pData, DWORD dwSessionID);	// 髪変更
	void	RecvProcMSGCMD_CHGCLOTH(PBYTE pData, DWORD dwSessionID);	// 服装変更
	void	RecvProcMSGCMD_CHGACCE(PBYTE pData, DWORD dwSessionID);	// アクセサリ変更
	void	RecvProcMSGCMD_CHGCOLOR(PBYTE pData, DWORD dwSessionID);	// 色変更
	void	RecvProcMSGCMD_CHGARMS(PBYTE pData, DWORD dwSessionID);	// 持ち物変更
	void	RecvProcMSGCMD_CHGSHIELD(PBYTE pData, DWORD dwSessionID);	// 盾変更
	void	RecvProcMSGCMD_MAKEITEM(PBYTE pData, DWORD dwSessionID);	// アイテム作成
	void	RecvProcMSGCMD_BALLOON(PBYTE pData, DWORD dwSessionID);	// 噴出し
	void	RecvProcMSGCMD_DICE(PBYTE pData, DWORD dwSessionID);	// サイコロ
	void	RecvProcMSGCMD_RND(PBYTE pData, DWORD dwSessionID);	// ランダム
	void	RecvProcMSGCMD_EFFECT(PBYTE pData, DWORD dwSessionID);	// エフェクト
	void	RecvProcMSGCMD_WHERE(PBYTE pData, DWORD dwSessionID);	// 最も集まっている場所

	// 受信処理(MainFrameRecvProcEFFECT.cpp)
	void	RecvProcEFFECT(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	// エフェクト系
	void	RecvProcEFFECT_REQ_BALLOONINFO(PBYTE pData, DWORD dwSessionID);	// 噴出し情報要求
	void	RecvProcEFFECT_DELETEBALLOONINFO(PBYTE pData, DWORD dwSessionID);	// 噴出し情報削除

	// 受信処理(MainFrameRecvProcBATTLE.cpp)
	void	RecvProcBATTLE(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	// 戦闘系
	void	RecvProcBATTLE_REQ_ATACK(PBYTE pData, DWORD dwSessionID);	// 攻撃要求

private:
	BYTE	m_byLastSendClock;	// 最後にお知らせした時刻
	DWORD	m_dwServerStartTime,	// サーバー起動時間
						m_dwLastKeepaliveCheck,	// 最後に生存確認チェックした時間
						m_dwLastClockTime,	// 最後に時報処理を行った時間
						m_dwLastSaveTime;	// 最後に定期保存を行った時間
	HWND	m_hWnd;	// ウィンドウハンドル(ヘッドレス時は NULL)
	HFONT	m_hFont;	// サーバー状態の描画に使うフォント
	BOOL	m_bHeadless;	// ヘッドレス動作か
	volatile BOOL	m_bQuit;	// 終了要求(コンソールシグナルや停止通知から立てられる)

	static	CMainFrame	*s_pInstance;	// コンソールシグナルから参照する自身

	CmySection	m_SectSockNotify;	// ソケット通知キューの排他
	std::deque<SOCKNOTIFYINFO>	m_deqSockNotify;	// ソケット通知キュー

	CMgrData	*m_pMgrData;	// データ管理
	CUraraSockTCPSBO	*m_pSock;	// 通信マネージャ
	CTextOutput	*m_pLog;	// ログファイル
        CHttpServer             *m_pHttpServer;         // 管理用HTTPサーバー
        CWebSocketBridge        *m_pWebSocketBridge;    // WebSocket→TCPブリッジサーバー

	CLibInfoAccount	*m_pLibInfoAccount;	// アカウント情報ライブラリ
	CLibInfoCharSvr	*m_pLibInfoChar;	// キャラ情報ライブラリ
	CLibInfoDisable	*m_pLibInfoDisable;	// 拒否情報ライブラリ
	CLibInfoMapBase	*m_pLibInfoMap;	// マップ情報ライブラリ
	CLibInfoMapObject	*m_pLibInfoMapObject;	// マップオブジェクト情報ライブラリ
	CLibInfoMapParts	*m_pLibInfoMapParts;	// マップパーツ情報ライブラリ
	CLibInfoMapShadow	*m_pLibInfoMapShadow;	// マップ影情報ライブラリ
	CLibInfoItemType	*m_pLibInfoItemType;	// アイテム種別情報ライブラリ
	CLibInfoItem	*m_pLibInfoItem;	// アイテム情報ライブラリ
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	// 武器情報ライブラリ
	CLibInfoEffect	*m_pLibInfoEffect;	// エフェクト情報ライブラリ
	CLibInfoMotion	*m_pLibInfoMotion;	// モーション情報ライブラリ
	CLibInfoMotionType	*m_pLibInfoMotionType;	// モーション種別情報ライブラリ
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;	// 噴出し情報ライブラリ
	CLibInfoSystem	*m_pLibInfoSystem;	// システム情報ライブラリ
	CLibInfoSkill	*m_pLibInfoSkill;	// スキル情報ライブラリ
	CLibInfoTalkEvent	*m_pLibInfoTalkEvent;	// 会話イベント情報ライブラリ
};
