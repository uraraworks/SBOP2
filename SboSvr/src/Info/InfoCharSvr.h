/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharSvr.h												 */
/* 内容			:キャラ情報サーバークラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/14													 */
/* ========================================================================= */

#pragma once

#include "InfoCharBase.h"

class CInfoMapBase;

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* 行動ID */
enum {
	CHARPROCID_NONE = 0,
	CHARPROCID_FISHING,								/* 釣り */
	CHARPROCID_FISHING_HIT,							/* 釣り(ヒット) */
	CHARPROCID_MAPMOVEIN,							/* マップ内移動 */
	CHARPROCID_MAPMOVEOUT,							/* マップ外移動 */
	CHARPROCID_SWOON,								/* 気絶 */
	CHARPROCID_INVINCIBLE,							/* 無敵 */
	CHARPROCID_MAX
};

/* 行動情報 */
typedef struct _CHARPROCINFO {
	DWORD	dwProcID,								/* 行動ID */
			dwProcSetTime,							/* 処理設定時間 */
			dwProcStartTime,						/* 処理開始時間 */
			dwPara;									/* パラメータ */
} CHARPROCINFO, *PCHARPROCINFO;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharSvr : public CInfoCharBase
{
public:
			CInfoCharSvr();								/* コンストラクタ */
	virtual ~CInfoCharSvr();							/* デストラクタ */

			void SetSpeak		(LPCSTR pszSpeak);				/* 発言内容を更新 */
	virtual void SetMoveState	(int nMoveState);				/* 移動状態を変更 */
	virtual void SetProcState	(int nProcState);				/* 行動状態を変更 */
			void SetMap			(CInfoMapBase *pInfoMap);		/* マップ情報を設定 */
			void SetEfcBalloon	(DWORD dwEfcBalloonID);			/* 噴出しを設定 */
			void SetMotion		(DWORD dwMotionID);				/* モーションを設定 */

	virtual BOOL IsEnableBattle	(void);							/* 戦闘状態に遷移できるか判定 */
	virtual BOOL IsEnableMove	(void);							/* 移動できる状態か判定 */
	virtual void CopyAll		(CInfoCharSvr *pSrc);			/* 派生先の情報も全てコピー */
	virtual void Copy			(CInfoCharBase *pSrc);			/* コピー */
	virtual void ProcAtack		(void);							/* 処理(攻撃した時) */
	virtual BOOL ProcHit		(CInfoCharSvr *pInfoChar);		/* 処理(攻撃を受けた時) */
	virtual BOOL TimerProc		(DWORD dwTime);					/* 時間処理 */
	virtual BOOL TimerProcMOVE	(DWORD dwTime);					/* 時間処理(移動) */
	virtual BOOL IsAtackTarget	(void);							/* 攻撃対象となるか判定 */

			void Proc				(DWORD dwTime);				/* 行動処理 */
	virtual BOOL ProcFISHING		(DWORD dwPara);				/* 行動処理(釣り) */
	virtual BOOL ProcFISHING_HIT	(DWORD dwPara);				/* 行動処理(釣り(ヒット)) */
	virtual BOOL ProcMAPMOVEIN		(DWORD dwPara);				/* 行動処理(マップ内移動) */
	virtual BOOL ProcMAPMOVEOUT		(DWORD dwPara);				/* 行動処理(マップ外移動) */
	virtual BOOL ProcSWOON			(DWORD dwPara);				/* 行動処理(気絶) */
	virtual BOOL ProcINVINCIBLE		(DWORD dwPara);				/* 行動処理(無敵) */

	void DeleteProcInfo		(int nNo);							/* 行動情報を削除 */
	void DeleteProcInfo		(DWORD dwProcID);					/* 行動情報を削除 */
	void DeleteAllProcInfo	(void);								/* 行動情報を全て削除 */
	void AddProcInfo		(DWORD dwProcID, DWORD dwStartTime, DWORD dwPara);	/* 行動情報を追加 */

	/* 移動種別による処理 */
	virtual void IncPutCount	(void);							/* 発生NPC数を増加 */
	virtual void DecPutCount	(void);							/* 発生NPC数を減少 */


public:
	int		m_nReserveChgEfect,			/* エフェクト変更予約 */
			m_nReserveChgMoveState,		/* 移動種別変更予約 */
			m_nMoveCount;				/* 移動歩数 */
	BOOL	m_bChgPos,					/* 座標が変更された */
			m_bChgMap,					/* マップが変更された */
			m_bChgUpdatePos,			/* クライアント側で自キャラの座標を更新する */
			m_bChgSpeak,				/* 発言内容が変更された */
			m_bChgInfo,					/* キャラ情報が変更された */
			m_bChgMoveState,			/* 移動状態が変更された */
			m_bChgProcState,			/* 行動状態が変更された */
			m_bChgScreenPos,			/* 画面位置を変更する */
			m_bChgPosRenew,				/* 座標が変更された */
			m_bChgGrp,					/* 画像が変更された */
			m_bChgEfcBalloon,			/* 噴出しが変更された */
			m_bChgMotion,				/* モーションが変更された */
			m_bChgStatus,				/* ステータスが変更された */
			m_bChgFishingHit,			/* 釣れた */
			m_bChgMoveCount,			/* 指定歩数の移動 */
			m_bChgPutNpc,				/* NPC発生 */
			m_bWaitCheckMapEvent,		/* マップイベントチェック待ち */
			m_bDropItem,				/* アイテムドロップ */
			m_bAtack,					/* 攻撃 */
			m_bRenewTargetPos,			/* ターゲット座標更新 */
			m_bProcMoveMapIn,			/* マップ内移動 */
			m_bProcMoveMapOut,			/* マップ外移動 */
			m_bProcMoveMarkPos,			/* 記録位置へ移動 */
			m_bProcSwoon,				/* 気絶 */
			m_bProcInvincible,			/* 無敵 */
			m_bStateFadeInOut,			/* フェードイン・アウト中 */
			m_bStatusInvincible;		/* 無敵中 */
	DWORD	m_dwLastTimeChg,			/* 状態変更時間 */
			m_dwChgWait,				/* 状態変更待ち時間 */
			m_dwEfcBalloonID,			/* 噴出しID */
			m_dwMotionID,				/* モーションID */
			m_dwMoveCount;				/* 移動歩数 */

	CInfoMapBase	*m_pInfoMap;		/* マップ情報 */
	CmyArray<PCHARPROCINFO, PCHARPROCINFO>	m_apProcInfo;	/* 行動情報 */
} CInfoCharSvr, *PCInfoCharSvr;

/* Copyright(C)URARA-works 2007 */
