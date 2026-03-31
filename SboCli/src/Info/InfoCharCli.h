/// @file InfoCharCli.h
/// @brief キャラ情報クライアントクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/10/01
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "InfoCharBase.h"
#include "InfoTextEffect.h"

class CMgrData;
class CMgrSound;
class CImg32;
class CInfoEffect;
class CUraraSockTCPSBO;

// 定数宣言

enum {
	INFOCHARCLI_VIEWSTATE_NONE = 0,
	INFOCHARCLI_VIEWSTATE_FADEIN,	// フェードイン
	INFOCHARCLI_VIEWSTATE_FADEOUT,	// フェードアウト
};

// 構造体宣言

// 座標変更キュー
typedef struct _MOVEPOSQUE {
	int	nState,	// 状態
			nDirection;	// 向き
	POINT	ptPos;	// 座標
} MOVEPOSQUE, *PMOVEPOSQUE;


// クラス宣言

typedef class CInfoCharCli : public CInfoCharBase
{
public:
			CInfoCharCli();	// コンストラクタ
	virtual ~CInfoCharCli();	// デストラクタ

	void Create	(CMgrData *pMgrData);	// 作成

			void	ChgDirection	(int nDirection);	// 向き変更
	virtual void	ChgMoveState	(int nMoveState);	// 移動状態変更
			BOOL	TimerProc	(DWORD dwTime);	// 時間処理
	virtual int	SetPos	(int x, int y, BOOL bBack = FALSE);	// 座標を指定

	void	SetMoveState	(int nMoveState);	// 移動状態を変更
	void	MakeCharGrp	(void);	// キャラ画像を作成
	void	SetName	(LPCSTR pszName);	// キャラ名を更新
	void	SetSpeak	(LPCSTR pszSpeak);	// 発言内容を更新
	void	SetSleepTimer	(BOOL bSleepTimer);	// おやすみタイマーを設定

	virtual void	SetViewState	(int nViewState);	// 表示状態設定
	virtual CInfoMotion *GetMotionInfo	(int *pnCount = NULL);	// モーション情報を取得
	virtual int	GetDrawDirection	(int nDirection = -1);	// 描画用に4方向で向きを取得

	BOOL	IsChgWait	(void);	// 状態変更待ちか判定
	void	SetEffectID	(DWORD dwEffectID);	// エフェクトIDを指定
	void	SetBalloonID	(DWORD dwBalloonID);	// 噴出しIDを指定
	void	GetGrpID	(WORD &wGrpIDMain, WORD &wGrpIDSub); // グラフィックIDを取得
	void	InitMotionInfo	(DWORD dwMotionID);	// モーション情報の初期化

	void	GetEfcGrpPos	(POINT &ptDst);	// エフェクト画像の開始座標を取得
	CImg32	*GetEfcImg	(void);	// 描画元のエフェクト画像イメージを取得
	int	GetEfcLevel	(void);	// エフェクト画像の透過レベルを取得
	int	GetEfcGrpSize	(void);	// エフェクト画像のサイズを取得
	void	GetEfcDrawPos	(POINT &ptDst);	// エフェクト画像の描画補正幅を取得
	void	GetDrawMapPos	(POINT &ptDst) const;	// 描画用の補間座標を取得

	DWORD	GetBalloonGrpID	(void);	// 噴出し画像IDを取得
	BOOL	IsEnableMove	(void);	// 移動できる状態か判定
	BOOL	IsDamage	(void);	// ダメージ受け中か判定
	void	SetDrawDirectionOverride(int nDirection);	// 描画用向きの上書き
	void	ClearDrawDirectionOverride(void);	// 描画用向きの上書きを解除

	void	SetChgWait	(BOOL bChgWait)	{ m_bChgWait = bChgWait; }	// 状態変更待ちか設定

	// 文字エフェクト関連
	int	GetTextEffectCount	(void);	// 登録数を取得
	void	AddTextEffect	(DWORD dwData, int nColor, int nSize);	// 追加
	void	AddTextEffect	(LPCSTR pszData, int nColor, int nSize); // 追加
	void	DeleteAllTextEffect	(void);	// 全て削除
	int	GetTextEffectGrpCount	(int nIndex);	// 画像数を取得
	int	GetTextEffectGrpNo	(int nIndex, int nNo);	// 指定番号の数値を取得
	BOOL	GetTextEffectDrawPos	(int nIndex, int nNo, POINT &ptDst, BYTE &byLevel);	// 指定番号の表示位置補正値を取得
	DWORD	GetTextEffectData	(int nIndex);	// 数値データを取得
	CInfoTextEffect	*GetTextEffect	(int nIndex);	// 文字エフェクトを取得

	// 座標変更キュー関連
	void	AddMovePosQue	(int nState, int nDirection, int x, int y);	// 追加
	void	DeleteMovePosQue	(int nNo);	// 削除
	void	DeleteAllMovePosQue	(void);	// 全て削除

	// Phase 6: Dead Reckoning（受信側予測）
	void	StartPredictedMove	(int nDirection, int x, int y, DWORD dwRecvTime);	// 予測移動開始
	void	StopPredictedMove	(int x, int y);	// 予測移動停止
	void	UpdatePredictedPos	(DWORD dwNowTime);	// 予測座標更新


protected:
	virtual BOOL RenewAnime	(DWORD dwTime, int nAdd = 1);	// アニメーションの更新

	void GetDrawMapPosDouble	(double &dDstX, double &dDstY, DWORD dwNowTime) const;	// 描画用補間座標を取得
	void ResetDrawMoveSegment	(int x, int y, DWORD dwNowTime);	// 描画補間を即時反映
	DWORD GetDrawMoveDuration	(int nSrcX, int nSrcY, int nDstX, int nDstY);	// 描画補間時間を取得
	BOOL TimerProcMove	(DWORD dwTime);	// 移動処理
	BOOL TimerProcViewState	(DWORD dwTime);	// 表示状態処理
	BOOL TimerProcAtack	(DWORD dwTime);	// 攻撃処理
	BOOL TimerProcBalloon	(DWORD dwTime);	// 噴出し処理
	void MotionProc	(DWORD dwProcID);	// モーション処理


public:
	BOOL	m_bRedraw,	// 再描画させる
			m_bChgWait,	// 状態変更待ち
			m_bSkipMove,	// 移動が間に合わない場合は待ち時間を無視する
			m_bWaitCheckMapEvent,	// マップイベントチェック待ち
			m_bMotionDirection,	// 向きに応じてモーションを切り替える
			m_bMotionInterrupt,	// モーション割り込み再生
			m_bModeSleepTimer;	// おやすみタイマー
	int	m_nAnimeBack,	// 前回のアニメーションコマ番号
			m_nViewState,	// 表示状態
			m_nFadeLevel,	// 透過レベル
			m_nMoveStateReserve,	// モーション終わった後に変更する移動種別
			m_nMoveStateBack;	// 前回の移動状態
	POINT	m_ptMove;	// 移動中の座標
	DWORD	m_dwMoveWaitOnce,	// 一度だけの移動待ち時間
			m_dwLastTimeAnime,	// 最終アニメーション処理時間
			m_dwLastTimeViewState,	// 最終表示時間
			m_dwBalloonID,	// 噴出しID
			m_dwBalloonGrpID,	// 噴出し画像ID
			m_dwBalloonAnimeID,	// 噴出しコマ番号
			m_dwLastTimeBalloon,	// 最終噴出し更新時間
			m_dwLastTimeDamage;	// 最終ダメージ受けた時間
	HFONT	m_hFont;	// 描画に使うフォント

	CMgrData	*m_pMgrData;	// データマネージャ
	CMgrSound	*m_pMgrSound;	// サウンド管理
	CImg32	*m_pDibChar,	// キャラ画像
						*m_pDibName,	// キャラ名画像
						*m_pDibSpeak,	// 発言内容画像
						*m_pDibBattle;	// 戦闘用画像
	CInfoEffect	*m_pInfoEffect;	// エフェクト情報
	CUraraSockTCPSBO	*m_pSock;	// 通信マネージャ
	ARRAYMOTIONINFO	m_aMotion[CHARMOTIONID_MAX][4];	// モーション情報
	ARRAYTEXTEFFECT	m_aTextEffect;	// 文字エフェクト
	CStdArray<PMOVEPOSQUE, PMOVEPOSQUE>	m_apMovePosQue;	// 座標変更キュー
	BOOL	m_bPredictedMove;	// 予測移動中か
	int	m_nPredictBaseX,	// 予測開始X(px)
			m_nPredictBaseY,	// 予測開始Y(px)
			m_nPredictDirection,	// 予測向き
			m_nPredictSpeed,	// 予測速度(px/sec基準)
			m_nDrawDirectionOverride;	// 描画用の向き上書き
	DWORD	m_dwPredictRecvTime,	// 受信タイムスタンプ
			m_dwDrawMoveStartTime,	// 描画補間開始時刻
			m_dwDrawMoveEndTime;	// 描画補間終了時刻
	double	m_dDrawMoveStartX,	// 描画補間開始X
			m_dDrawMoveStartY,	// 描画補間開始Y
			m_dDrawMoveEndX,	// 描画補間終了X
			m_dDrawMoveEndY;	// 描画補間終了Y
} CInfoCharCli, *PCInfoCharCli;

