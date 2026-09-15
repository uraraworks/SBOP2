/// @file InfoCharSvr.h
/// @brief キャラ情報サーバークラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/14
/// @copyright Copyright(C)URARA-works 2007

#pragma once
#include <vector>

#include "InfoCharBase.h"

class CInfoMapBase;
class CLibInfoCharSvr;

// 定数定義

// 行動ID
enum {
	CHARPROCID_NONE = 0,
	CHARPROCID_FISHING,	// 釣り
	CHARPROCID_FISHING_HIT,	// 釣り(ヒット)
	CHARPROCID_MAPMOVEIN,	// マップ内移動
	CHARPROCID_MAPMOVEOUT,	// マップ外移動
	CHARPROCID_SWOON,	// 気絶
	CHARPROCID_INVINCIBLE,	// 無敵
	CHARPROCID_MAX
};

// 発火済みマップイベントタイル集合の最大数
#define MAPEVENT_FIRED_MAX 8

// 行動情報
typedef struct _CHARPROCINFO {
	DWORD	dwProcID,	// 行動ID
			dwProcSetTime,	// 処理設定時間
			dwProcStartTime,	// 処理開始時間
			dwPara;	// パラメータ
} CHARPROCINFO, *PCHARPROCINFO;

// クラス宣言

typedef class CInfoCharSvr : public CInfoCharBase
{
public:
			CInfoCharSvr();	// コンストラクタ
	virtual ~CInfoCharSvr();	// デストラクタ

			void SetSpeak(LPCSTR pszSpeak);	// 発言内容を更新
	virtual void SetMoveState(int nMoveState);	// 移動状態を変更
	virtual void SetProcState(int nProcState);	// 行動状態を変更
			void SetMap(CInfoMapBase *pInfoMap);	// マップ情報を設定
			void SetLibInfoChar(CLibInfoCharSvr *pLibInfoChar);	// キャラ情報ライブラリを設定
			void SetEfcBalloon(DWORD dwEfcBalloonID);	// 噴出しを設定
			void SetMotion(DWORD dwMotionID);	// モーションを設定

	virtual BOOL  IsEnableBattle(void);	// 戦闘状態に遷移できるか判定
	virtual BOOL  IsEnableMove(void);	// 移動できる状態か判定
	virtual void  CopyAll(CInfoCharSvr *pSrc);	// 派生先の情報も全てコピー
	virtual void  Copy(CInfoCharBase *pSrc);	// コピー
	virtual void  ProcAtack(void);	// 処理(攻撃した時)
	virtual BOOL  ProcHit(CInfoCharSvr *pInfoChar);	// 処理(攻撃を受けた時)
	virtual BOOL  TimerProc(DWORD dwTime);	// 時間処理
	virtual BOOL  TimerProcMOVE(DWORD dwTime);	// 時間処理(移動)
			void  ProcEjectMove(DWORD dwTime);	// [押せる物側] 自走(eject)を1周期ぶん進める(docs/push-object-redesign.md 2章7項)
	virtual BOOL  IsAtackTarget(void);	// 攻撃対象となるか判定
	virtual DWORD GetHitEffectID(void);	// ヒット時に相手に表示するエフェクトIDを取得
	virtual DWORD GetDamage(void);	// ダメージ値を取得

			void Proc(DWORD dwTime);	// 行動処理
	virtual BOOL ProcFISHING(DWORD dwPara);	// 行動処理(釣り)
	virtual BOOL ProcFISHING_HIT(DWORD dwPara);	// 行動処理(釣り(ヒット))
	virtual BOOL ProcMAPMOVEIN(DWORD dwPara);	// 行動処理(マップ内移動)
	virtual BOOL ProcMAPMOVEOUT(DWORD dwPara);	// 行動処理(マップ外移動)
	virtual BOOL ProcSWOON(DWORD dwPara);	// 行動処理(気絶)
	virtual BOOL ProcINVINCIBLE(DWORD dwPara);	// 行動処理(無敵)

	void DeleteProcInfo(int nNo);	// 行動情報を削除
	void DeleteProcInfo(DWORD dwProcID);	// 行動情報を削除
	void DeleteAllProcInfo(void);	// 行動情報を全て削除
	void AddProcInfo(DWORD dwProcID, DWORD dwStartTime, DWORD dwPara);	// 行動情報を追加

	// マップイベント発火済み集合の管理
	void ClearFiredMapEvent();	// 発火済み集合をクリア
	void AddFiredMapEvent(DWORD dwMapID, int nTileX, int nTileY);	// 発火済みタイルを追加
	BOOL IsFiredMapEventInRect(DWORD dwMapID, int nLeft, int nTop, int nRight, int nBottom);	// 矩形内に発火済みタイルがあるか

	// 移動種別による処理
	virtual void IncPutCount(void);	// 発生NPC数を増加
	virtual void DecPutCount(void);	// 発生NPC数を減少

public:
	int	m_nReserveChgEfect,	// エフェクト変更予約
			m_nReserveChgMoveState,	// 移動種別変更予約
			m_nMoveCount;	// 移動歩数
	BOOL	m_bChgPos,	// 座標が変更された
			m_bChgMap,	// マップが変更された
			m_bChgUpdatePos,	// クライアント側で自キャラの座標を更新する
			m_bChgSpeak,	// 発言内容が変更された
			m_bChgInfo,	// キャラ情報が変更された
			m_bChgMoveState,	// 移動状態が変更された
			m_bChgProcState,	// 行動状態が変更された
			m_bChgScreenPos,	// 画面位置を変更する
			m_bChgPosRenew,	// 座標が変更された
			m_bChgGrp,	// 画像が変更された
			m_bChgEfcBalloon,	// 噴出しが変更された
			m_bChgMotion,	// モーションが変更された
			m_bChgStatus,	// ステータスが変更された
			m_bChgFishingHit,	// 釣れた
			m_bChgMoveCount,	// 指定歩数の移動
			m_bChgPutNpc,	// NPC発生
			m_bChgTargetChar,	// 近くのキャラにターゲット変更
			m_bWaitCheckMapEvent,	// マップイベントチェック待ち
			m_bDropItem,	// アイテムドロップ
			m_bAtack,	// 攻撃
			m_bRenewTargetPos,	// ターゲット座標更新
			m_bProcMoveMapIn,	// マップ内移動
			m_bProcMoveMapOut,	// マップ外移動
			m_bProcMoveMarkPos,	// 記録位置へ移動
			m_bProcSwoon,	// 気絶
			m_bProcInvincible,	// 無敵
			m_bStateFadeInOut,	// フェードイン・アウト中
			m_bStatusInvincible;	// 無敵中
	DWORD	m_dwLastTimeChg,	// 状態変更時間
			m_dwChgWait,	// 状態変更待ち時間
			m_dwEfcBalloonID,	// 噴出しID
			m_dwMotionID,	// モーションID
			m_dwMoveCount,	// 移動歩数
			m_dwLastMoveSyncSendTime,	// 最終移動同期送信時刻
			m_dwFiredMapEventMapID,	// 発火済みイベント集合の対象マップID
			m_dwLastRecvMoveTime,	// 最終移動受信時刻(ms)
			m_dwLastRecvMovePacketTime,	// 最終移動受信パケット時刻(ms)
			m_dwLastMoveRejectSyncTime,	// 移動速度超過拒否時に権威座標を送り返した最終時刻(ms)。連続拒否時の送信頻度を抑制するため
			m_dwLastTalkEventNPCID,	// 最後に会話イベントを要求したNPCのID(REQ_ADDSKILLの正当性確認用。保存・送信対象外)
			m_dwLastTalkEventTime,	// 上記を記録した時刻(ms)
			m_dwLastPushedTime,	// [押せる物側] 最後に押された時刻(ms)。150ms経過で停止させる(保存・送信対象外)
			m_dwPushingCharID,	// [押せる物側] 押している本人のキャラID。0なら押されていない(保存・送信対象外)
			m_dwLastPushClientTime,	// [押す本人側] 最後に受理したREQ_PUSHのクライアント時刻(ms)。速度検証の基準(保存・送信対象外)
			m_dwSwapOwnerSessionID,	// [押せる物側] 入れ替わり(SWAP)を専有している本人のセッションID。改造クライアント対策の照合用(保存・送信対象外)
			m_dwLastSwapReqTime,	// [押せる物側] 専有者から最後にSWAP/PUSH要求を受けた時刻(ms)。PUSH_SWAP_OWNER_TIMEOUT_MS途絶えたら打ち切る(保存・送信対象外)
			m_dwEjectOwnerCharID,	// [押せる物側] 自走(eject)を開始した専有者のキャラID。ログアウトで自走を終了する判定に使う(保存・送信対象外)
			m_dwLastEjectProcTime,	// [押せる物側] 自走(eject)を最後に進めた時刻(ms)。50ms未満はまとめて進める(保存・送信対象外)
			m_dwLastPushRejectSyncTime,	// [押す本人側] 押し要求を却下してRES_PUSHで補正送信した最終時刻(ms)。連続拒否時の送信頻度を抑制する(保存・送信対象外)
			m_dwLastPushRejectLogTime,	// [押す本人側] 押し要求却下ログを最後に出力した時刻(ms)。連続拒否時のログ出力頻度を抑制する(保存・送信対象外)
			m_dwLastPushAcceptLogTime,	// [押す本人側] 診断用。押し要求受理ログを最後に出力した時刻(ms)。原因特定後に無効化(保存・送信対象外)
			m_dwLastPushDiagLogTime,	// [押す本人側] 診断用。REQ_PUSH入口ログ・判定前の黙殺returnログの共通抑制(1秒1回)。原因特定後に無効化(保存・送信対象外)
			m_dwLastPushDecideLogTime,	// [押す本人側] 診断用。DecidePush直前ログの抑制(1秒1回)。原因特定後に無効化(保存・送信対象外)
			m_dwLastAtackAcceptedTime,	// 最後に受理した攻撃要求の時刻(ms)。攻撃間隔チェックの基準(保存・送信対象外)
			m_dwPrevAtackAcceptedTime,	// 前々回に受理した攻撃要求の時刻(ms)。通信の揺らぎ許容の基準(保存・送信対象外)
			m_dwLastAtackRejectLogTime;	// 攻撃要求却下ログを最後に出力した時刻(ms)。連続拒否時のログ出力頻度を抑制する(保存・送信対象外)
	int	m_nFiredMapEventCount,	// 発火済みイベント数
			m_nLastMoveSyncDirection,	// 最終移動同期向き
			m_nPushRejectSuppressedCount,	// [押す本人側] ログ抑制中に却下したが出力しなかった件数(保存・送信対象外)
			m_nAtackRejectSuppressedCount;	// ログ抑制中に却下したが出力しなかった攻撃要求の件数(保存・送信対象外)
	int	m_nFiredMapEventTileX[MAPEVENT_FIRED_MAX];	// 発火済みイベントのタイルX（集合）
	int	m_nFiredMapEventTileY[MAPEVENT_FIRED_MAX];	// 発火済みイベントのタイルY（集合）
	BOOL	m_bMoveSyncActive,	// 移動同期中
			m_bPendingMapEvent,	// イベント自動歩行フェーズ待機中
			m_bSwapActive,	// [押せる物側] 入れ替わり(SWAP)中か(保存・送信対象外)
			m_bEjectActive;	// [押せる物側] 入れ替わり終了後の自走(eject)中か(保存・送信対象外)
	int	m_nPendingEventTileX,	// 待機中イベントのタイルX
			m_nPendingEventTileY,	// 待機中イベントのタイルY
			m_nSwapDir,	// [押せる物側] 入れ替わり開始時の(本人の)向き(保存・送信対象外)
			m_nEjectDir;	// [押せる物側] 自走(eject)の向き(保存・送信対象外)
	POINT	m_ptSwapP0,	// [押せる物側] 入れ替わり開始時の本人位置(保存・送信対象外)
			m_ptSwapB0;	// [押せる物側] 入れ替わり開始時のボール位置(保存・送信対象外)
	double	m_dEjectPxRemainder;	// [押せる物側] 自走(eject)のpx端数。50ms単位で進める際の取りこぼし防止(保存・送信対象外)

	CInfoMapBase	*m_pInfoMap;	// マップ情報
	CLibInfoCharSvr	*m_pLibInfoCharSvr;	// キャラ情報ライブラリ
	std::vector<PCHARPROCINFO>	m_apProcInfo;	// 行動情報
} CInfoCharSvr, *PCInfoCharSvr;
