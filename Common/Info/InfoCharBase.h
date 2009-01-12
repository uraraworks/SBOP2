/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:InfoCharBase.h												 */
/* 内容			:キャラ情報基底クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"
#include "InfoMotion.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* モーション種別ID */
enum {
	CHARMOTIONID_STAND = 0,		/* 立ち */
	CHARMOTIONID_WALK,			/* 歩き */
	CHARMOTIONID_SIT,			/* 座り */
	CHARMOTIONID_BATTLESTAND,	/* 戦闘立ち */
	CHARMOTIONID_BATTLEWALK,	/* すり足 */
	CHARMOTIONID_ATACK,			/* 攻撃 */
	CHARMOTIONID_BATTLEDEFENSE,	/* 防御 */
	CHARMOTIONID_ANIME,			/* アニメーション */
	CHARMOTIONID_INTERRUUPT,	/* 割り込み再生 */
	CHARMOTIONID_MAX
};

/* 行動状態 */
enum {
	CHARPROCSTATEID_NORMAL = 0,		/* 通常状態 */
	CHARPROCSTATEID_FISHING,		/* 釣り状態 */
	CHARPROCSTATEID_MAX
};

/* キャラ移動種別 */
enum {
	CHARMOVETYPE_PC = 0,							/* プレイヤーキャラ */
	CHARMOVETYPE_STAND,								/* 移動しない */
	CHARMOVETYPE_BALL,								/* ボール */
	CHARMOVETYPE_SCORE,								/* 得点 */
	CHARMOVETYPE_STYLECOPY_PUT,						/* 容姿コピー(取り込み) */
	CHARMOVETYPE_ATACKANIME,						/* 攻撃受けるとアニメーション */
	CHARMOVETYPE_MOVEATACK,							/* 移動して攻撃 */
	CHARMOVETYPE_STYLECOPY_GET,						/* 容姿コピー(反映) */
	CHARMOVETYPE_PUTNPC,							/* NPC発生 */
	CHARMOVETYPE_BATTLE1,							/* 戦闘1 */
	CHARMOVETYPE_MAX
};

/* キャラ移動状態 */
enum {
	CHARMOVESTATE_STAND = 0,						/* 立ち */
	CHARMOVESTATE_SIT,								/* 座り中 */
	CHARMOVESTATE_MOVE,								/* 移動中 */
	CHARMOVESTATE_DELETEREADY,						/* 消去準備 */
	CHARMOVESTATE_DELETE,							/* 消去 */
	CHARMOVESTATE_BATTLE,							/* 戦闘中 */
	CHARMOVESTATE_BATTLEMOVE,						/* 戦闘移動中 */
	CHARMOVESTATE_BATTLEATACK,						/* 戦闘攻撃中 */
	CHARMOVESTATE_BATTLEATACK_WAIT,					/* 戦闘攻撃後の待ち時間 */
	CHARMOVESTATE_ANIME,							/* アニメーション */
	CHARMOVESTATE_WALKANIME,						/* 足踏み */
	CHARMOVESTATE_SWOON,							/* 気絶 */
	CHARMOVESTATE_DAMAGE,							/* ダメージ */
	CHARMOVESTATE_BATTLE_DEFENSE,					/* 防御中 */
	CHARMOVESTATE_SLEEPTIMER,						/* おやすみタイマー中 */
	CHARMOVESTATE_MAX
};

#define MAX_ATACKGAUGE		100						/* アタックゲージ最大値 */
#define MAX_DEFENSEGAUGE	100						/* ガードゲージ最大値 */


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharBase : public CInfoBase
{
public:
			CInfoCharBase();								/* コンストラクタ */
	virtual ~CInfoCharBase();								/* デストラクタ */

	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データを設定 */

	BOOL	IsLogin				(void);								/* ログイン中か判定 */
	BOOL	IsStateBattle		(void);								/* 戦闘モードか判定 */
	BOOL	IsStateMove			(void);								/* 移動中か判定 */
	BOOL	IsViewArea			(DWORD dwMapID, POINT *pptPos);		/* 見える範囲か判定 */
	BOOL	IsNPC				(void);								/* NPCか判定 */
	BOOL	CheckSessionID		(DWORD dwSessionID);				/* セッションIDをチェック */
	void	GetFrontPos			(POINT &ptDst, int nDirection = -1, BOOL bMove=FALSE);	/* 一歩前のキャラ座標を取得 */
	void	GetFrontPos			(CmyArray<POINT, POINT> &aptPos, int nDirection = -1);	/* 一歩前のキャラ座標を取得 */
	void	GetFrontMapPos		(POINT &ptDst, int nDirection = -1);					/* 一歩前のマップ座標を取得 */
	int		GetDirection		(int x, int y);						/* 指定座標の向きを取得 */
	int		GetAtackType		(void);								/* 攻撃種別を取得 */
	void	SetAtackMotion		(int nMotionID);					/* 攻撃モーションの設定 */
	int		GetDrawDirection	(int nDirection = -1);				/* 描画用に4方向で向きを取得 */
	int		GetBackDirection	(void);								/* 反対方向を取得 */
	DWORD	GetMoveWait			(void);								/* 状態に応じた移動速度を取得 */

	void		SetItem			(ARRAYDWORD *padwItemID);						/* アイテム情報を設定 */
	ARRAYDWORD	*GetItem		(void)	{ return &m_adwItemID; }				/* アイテム情報を取得 */
	BOOL		IsItemAdd		(void);											/* アイテムを追加できるか判定 */
	void		DeleteItem		(DWORD dwItemID);								/* 所持アイテムを削除 */
	BOOL		HaveItem		(DWORD dwItemID);								/* 指定アイテムを持っているか判定 */
	void		GetTargetPos	(POINT *ptTarget, POINT &ptDst, int nCount);	/* 指定座標を取得 */
	void		SetTarget		(CInfoCharBase *pCharTarget);					/* ターゲットキャラを設定 */
	void		SetSkill		(ARRAYDWORD *padwSkillID);						/* スキル情報を設定 */
	ARRAYDWORD	*GetSkill		(void)	{ return &m_adwSkillID; }				/* スキル情報を取得 */
	BOOL		HaveSkill		(DWORD dwSkillID);								/* 指定スキルを持っているか判定 */

	virtual BOOL IsMove				(void);							/* 移動中か判定 */
	virtual int  SetPos				(int x, int y, BOOL bBack = FALSE);	/* 座標を指定 */
	virtual void SetDirection		(int nDirection);				/* 向きを指定 */
	virtual void SetMoveState		(int nMoveState);				/* 移動状態を変更 */
	virtual void SetProcState		(int nProcState);				/* 行動状態を変更 */
	virtual void SetName			(LPCSTR pszName);				/* キャラ名を更新 */
	virtual void SetSpeak			(LPCSTR pszSpeak);				/* 発言内容を更新 */
	virtual void Copy				(CInfoCharBase *pSrc);			/* コピー */
	virtual BOOL TimerProc			(DWORD dwTime);					/* 時間処理 */
	virtual void RenewBlockMapArea	(int x, int y, int nDirection, BOOL bMoveOut = FALSE);	/* 当たり判定対象のマップ座標を更新 */
	virtual BOOL IsHitCharPos		(int x, int y, SIZE *psize = NULL);						/* キャラ座標との当たり判定 */
	virtual void GetViewCharPos		(POINT &ptDst);					/* 表示用の座標補正値を取得 */
	virtual void GetCharSize		(SIZE &sizeDst);				/* キャラの幅と高さを取得 */
	virtual BOOL IsEnableMove		(void);							/* 移動できる状態か判定 */
	virtual BOOL IsEnableAtack		(void);							/* 攻撃できる状態か判定 */
	virtual BOOL IsAtackTarget		(void);							/* 攻撃対象となるか判定 */
	virtual BOOL IsLogoutDelete		(void);							/* ログアウト時に削除するか判定 */


public:
	/* 保存しないデータ */
	DWORD		m_dwSessionID,						/* 操作中のセッションID */
				m_dwAccountID,						/* アカウントID */
				m_dwLastTimeSpeak,					/* 最終発言時間 */
				m_dwLastTimeMove,					/* 最終移動処理時間 */
				m_dwTailCharID,						/* 付いて来ているキャラID */
				m_dwFrontCharID,					/* 付いているキャラID */
				m_dwParentCharID,					/* 親のキャラID */
				m_dwTargetCharID,					/* ターゲットキャラID */
				m_adwMotionID[CHARMOTIONID_MAX],	/* モーションID */
				m_dwLightTime;						/* 灯り有効時間 */
	WORD		m_wAtackGauge,						/* アタックゲージ */
				m_wDefenseGauge;					/* ガードゲージ */
	POINT		m_ptCharBack,						/* 前回のキャラ座標 */
				m_ptViewCharPos,					/* 表示用の座標補正値 */
				m_ptTargetPos;						/* ターゲット座標 */
	int			m_nAnime,							/* アニメーション番号 */
				m_nDirectionBack,					/* 前回の向き */
				m_nProcState,						/* 行動状態 */
				m_nMoveDirection,					/* 移動方向 */
				m_nLightLevel,						/* 灯りレベル */
				m_nAtackTarget;						/* 攻撃対象 */
	BOOL		m_bDelete,							/* 削除フラグ */
				m_bNPC,								/* NPC */
				m_bChargeAtack,						/* 溜め攻撃 */
				m_bParentInfo;						/* 親の情報を参照する */
	ARRAYBYTE	m_abyMark;							/* 名前の前に表示するマーク番号 */
	CmyArray<POINT, POINT> m_aposBockMapArea;		/* 当たり判定対象のマップ座標 */

	/* 保存するデータ */
	BOOL		m_bBlock,					/* ぶつかる判定 */
				m_bPush;					/* 押せる判定 */
	int			m_nMapX,					/* マップ座標(横) */
				m_nMapY,					/* マップ座標(縦) */
				m_nMoveState,				/* 移動状態 */
				m_nMoveType,				/* 移動種別 */
				m_nDirection,				/* 向き */
				m_nGrpSize,					/* 画像サイズ */
				m_nSex,						/* 性別 */
				m_nMaxItemCount,			/* 最大アイテム所持数 */
				m_nDropItemAverage,			/* アイテムドロップ率 */
				m_nMoveAverage,				/* 移動確率 */
				m_nMoveAverageBattle;		/* 戦闘時移動確率 */
	WORD		m_wFamilyID,				/* 種族ID */
				m_wGrpIDNPC,				/* 画像ID(NPC) */
				m_wGrpIDCloth,				/* 画像ID(服) */
				m_wGrpIDEye,				/* 画像ID(目) */
				m_wGrpIDEyeColor,			/* 画像ID(目色) */
				m_wGrpIDHairType,			/* 画像ID(髪) */
				m_wGrpIDHairColor,			/* 画像ID(髪色) */
				m_wGrpIDSP,					/* 画像ID(特殊服) */
				m_wGrpIDTmpMain,			/* 画像ID(一時服:メイン) */
				m_wGrpIDTmpSub,				/* 画像ID(一時服:サブ) */
				m_wGrpIDAcce,				/* 画像ID(アクセサリ) */
				m_wGrpIDArmsMain,			/* 画像ID(持ち物:メイン) */
				m_wGrpIDArmsSub,			/* 画像ID(持ち物:サブ) */
				m_wGrpIDArmsLeftMain,		/* 画像ID(盾:メイン) */
				m_wGrpIDArmsLeftSub,		/* 画像ID(盾:サブ) */
				m_wGrpIDInitNPC,			/* 初期画像ID(NPC) */
				m_wGrpIDInitCloth,			/* 初期画像ID(服) */
				m_wGrpIDInitEye,			/* 初期画像ID(目) */
				m_wGrpIDInitEyeColor,		/* 初期画像ID(目色) */
				m_wGrpIDInitHairType,		/* 初期画像ID(髪) */
				m_wGrpIDInitHairColor,		/* 初期画像ID(髪色) */
				m_wGrpIDInitSP,				/* 初期画像ID(特殊服) */
				m_wLevel,					/* レベル */
				m_wStamina,					/* スタミナ */
				m_wPower,					/* 腕力 */
				m_wStrength,				/* 体力 */
				m_wMagic,					/* 魔力 */
				m_wSkillful,				/* 器用 */
				m_wAbillityAT,				/* 攻撃技能 */
				m_wAbillityDF,				/* 防御技能 */
				m_wPAtack,					/* 攻撃力 */
				m_wPDefense,				/* 防御力 */
				m_wPMagic,					/* 魔法力 */
				m_wPMagicDefense,			/* 魔法防御力 */
				m_wPHitAverage,				/* 命中率 */
				m_wPAvoidAverage,			/* 回避率 */
				m_wPCriticalAverage,		/* クリティカル率 */
				m_wAttrFire,				/* 属性[火] */
				m_wAttrWind,				/* 属性[風] */
				m_wAttrWater,				/* 属性[水] */
				m_wAttrEarth,				/* 属性[土] */
				m_wAttrLight,				/* 属性[光] */
				m_wAttrDark;				/* 属性[闇] */
	DWORD		m_dwCharID,					/* キャラID */
				m_dwMapID,					/* マップID */
				m_dwMotionTypeID,			/* モーション種別ID */
				m_dwEquipItemIDCloth,		/* 装備アイテムID:服 */
				m_dwEquipItemIDAcce1,		/* 装備アイテムID:アクセサリ1 */
				m_dwEquipItemIDAcce2,		/* 装備アイテムID:アクセサリ2 */
				m_dwEquipItemIDArmsRight,	/* 装備アイテムID:右手 */
				m_dwEquipItemIDArmsLeft,	/* 装備アイテムID:左手 */
				m_dwEquipItemIDHead,		/* 装備アイテムID:頭 */
				m_dwMoveWait,				/* 移動待ち時間 */
				m_dwMoveWaitBattle,			/* 戦闘時移動待ち時間 */
				m_dwExp,					/* 経験値 */
				m_dwHP,						/* HP */
				m_dwMaxHP,					/* 最大HP */
				m_dwSP,						/* SP */
				m_dwMaxSP;					/* 最大SP */
	COLORREF	m_clName,					/* 名前の描画色 */
				m_clSpeak;					/* 発言の描画色 */
	CmyString	m_strCharName,				/* キャラ名 */
				m_strSpeak,					/* 発言内容 */
				m_strTalk;					/* 会話データ */
	ARRAYDWORD	m_adwItemID,				/* 所持アイテム */
				m_adwSkillID;				/* 所持スキル */

	/* 移動種別による個別情報 */

	/* NPC発生 */
	DWORD		m_dwPutCycle;				/* 発生周期 */
	int			m_nPutMoveType;				/* 発生させる移動種別 */
	int			m_nMaxPutCount;				/* 同時発生数 */
	int			m_nPutAverage;				/* 発生確率 */
	POINT		m_ptPutArea;				/* 発生範囲(半径) */
} CInfoCharBase, *PCInfoCharBase;
typedef CmyArray<PCInfoCharBase, PCInfoCharBase>	  ARRAYINFOCHARBASE;
typedef CmyArray<PCInfoCharBase, PCInfoCharBase>	*PARRAYINFOCHARBASE;

/* Copyright(C)URARA-works 2006 */
