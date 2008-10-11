/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoMotion.h												 */
/* 内容			:モーション情報クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/29													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* モーションリストID */
enum {
	CHARMOTIONLISTID_NONE = 0,
	CHARMOTIONLISTID_STAND_UP,					/* 立ち(上) */
	CHARMOTIONLISTID_STAND_DOWN,				/* 立ち(下) */
	CHARMOTIONLISTID_STAND_LEFT,				/* 立ち(左) */
	CHARMOTIONLISTID_STAND_RIGHT,				/* 立ち(右) */
	CHARMOTIONLISTID_WALK_UP,					/* 歩き(上) */
	CHARMOTIONLISTID_WALK_DOWN,					/* 歩き(下) */
	CHARMOTIONLISTID_WALK_LEFT,					/* 歩き(左) */
	CHARMOTIONLISTID_WALK_RIGHT,				/* 歩き(右) */
	CHARMOTIONLISTID_SIT_UP,					/* 座り(上) */
	CHARMOTIONLISTID_SIT_DOWN,					/* 座り(下) */
	CHARMOTIONLISTID_SIT_LEFT,					/* 座り(左) */
	CHARMOTIONLISTID_SIT_RIGHT,					/* 座り(右) */
	CHARMOTIONLISTID_BATTLESTAND_UP,			/* 戦闘立ち(上) */
	CHARMOTIONLISTID_BATTLESTAND_DOWN,			/* 戦闘立ち(下) */
	CHARMOTIONLISTID_BATTLESTAND_LEFT,			/* 戦闘立ち(左) */
	CHARMOTIONLISTID_BATTLESTAND_RIGHT,			/* 戦闘立ち(右) */
	CHARMOTIONLISTID_BATTLEWALK_UP,				/* すり足(上) */
	CHARMOTIONLISTID_BATTLEWALK_DOWN,			/* すり足(下) */
	CHARMOTIONLISTID_BATTLEWALK_LEFT,			/* すり足(左) */
	CHARMOTIONLISTID_BATTLEWALK_RIGHT,			/* すり足(右) */
	CHARMOTIONLISTID_SWING_UP,					/* 振り(上) */
	CHARMOTIONLISTID_SWING_DOWN,				/* 振り(下) */
	CHARMOTIONLISTID_SWING_LEFT,				/* 振り(左) */
	CHARMOTIONLISTID_SWING_RIGHT,				/* 振り(右) */
	CHARMOTIONLISTID_POKE_UP,					/* 突き(上) */
	CHARMOTIONLISTID_POKE_DOWN,					/* 突き(下) */
	CHARMOTIONLISTID_POKE_LEFT,					/* 突き(左) */
	CHARMOTIONLISTID_POKE_RIGHT,				/* 突き(右) */
	CHARMOTIONLISTID_BOWWALK_UP,				/* 弓用すり足(上) */
	CHARMOTIONLISTID_BOWWALK_DOWN,				/* 弓用すり足(下) */
	CHARMOTIONLISTID_BOWWALK_LEFT,				/* 弓用すり足(左) */
	CHARMOTIONLISTID_BOWWALK_RIGHT,				/* 弓用すり足(右) */
	CHARMOTIONLISTID_BOW_UP,					/* 弓(上) */
	CHARMOTIONLISTID_BOW_DOWN,					/* 弓(下) */
	CHARMOTIONLISTID_BOW_LEFT,					/* 弓(左) */
	CHARMOTIONLISTID_BOW_RIGHT,					/* 弓(右) */
	CHARMOTIONLISTID_GLOVEWALK_UP,				/* 打撃用すり足(上) */
	CHARMOTIONLISTID_GLOVEWALK_DOWN,			/* 打撃用すり足(下) */
	CHARMOTIONLISTID_GLOVEWALK_LEFT,			/* 打撃用すり足(左) */
	CHARMOTIONLISTID_GLOVEWALK_RIGHT,			/* 打撃用すり足(右) */
	CHARMOTIONLISTID_GLOVE_UP,					/* 打撃(上) */
	CHARMOTIONLISTID_GLOVE_DOWN,				/* 打撃(下) */
	CHARMOTIONLISTID_GLOVE_LEFT,				/* 打撃(左) */
	CHARMOTIONLISTID_GLOVE_RIGHT,				/* 打撃(右) */
	CHARMOTIONLISTID_FISHINGWALK_UP,			/* 釣り用すり足(上) */
	CHARMOTIONLISTID_FISHINGWALK_DOWN,			/* 釣り用すり足(下) */
	CHARMOTIONLISTID_FISHINGWALK_LEFT,			/* 釣り用すり足(左) */
	CHARMOTIONLISTID_FISHINGWALK_RIGHT,			/* 釣り用すり足(右) */
	CHARMOTIONLISTID_FISHING_UP,				/* 釣り(上) */
	CHARMOTIONLISTID_FISHING_DOWN,				/* 釣り(下) */
	CHARMOTIONLISTID_FISHING_LEFT,				/* 釣り(左) */
	CHARMOTIONLISTID_FISHING_RIGHT,				/* 釣り(右) */
	CHARMOTIONLISTID_FISHING_HIT_UP,			/* 釣りヒット(上) */
	CHARMOTIONLISTID_FISHING_HIT_DOWN,			/* 釣りヒット(下) */
	CHARMOTIONLISTID_FISHING_HIT_LEFT,			/* 釣りヒット(左) */
	CHARMOTIONLISTID_FISHING_HIT_RIGHT,			/* 釣りヒット(右) */
	CHARMOTIONLISTID_BOWBATTLESTAND_UP,			/* 弓用戦闘立ち(上) */
	CHARMOTIONLISTID_BOWBATTLESTAND_DOWN,		/* 弓用戦闘立ち(下) */
	CHARMOTIONLISTID_BOWBATTLESTAND_LEFT,		/* 弓用戦闘立ち(左) */
	CHARMOTIONLISTID_BOWBATTLESTAND_RIGHT,		/* 弓用戦闘立ち(右) */
	CHARMOTIONLISTID_GLOVEBATTLESTAND_UP,		/* 打撃用戦闘立ち(上) */
	CHARMOTIONLISTID_GLOVEBATTLESTAND_DOWN,		/* 打撃用戦闘立ち(下) */
	CHARMOTIONLISTID_GLOVEBATTLESTAND_LEFT,		/* 打撃用戦闘立ち(左) */
	CHARMOTIONLISTID_GLOVEBATTLESTAND_RIGHT,	/* 打撃用戦闘立ち(右) */
	CHARMOTIONLISTID_FISHINGBATTLESTAND_UP,		/* 釣り用戦闘立ち(上) */
	CHARMOTIONLISTID_FISHINGBATTLESTAND_DOWN,	/* 釣り用戦闘立ち(下) */
	CHARMOTIONLISTID_FISHINGBATTLESTAND_LEFT,	/* 釣り用戦闘立ち(左) */
	CHARMOTIONLISTID_FISHINGBATTLESTAND_RIGHT,	/* 釣り用戦闘立ち(右) */
	CHARMOTIONLISTID_STAND,						/* 立ち */
	CHARMOTIONLISTID_STAND_SIMPLE,				/* 立ち(上下左右1コマずつ) */
	CHARMOTIONLISTID_WALK,						/* 歩き(方向無し) */
	CHARMOTIONLISTID_DESTROY,					/* 破壊 */
	CHARMOTIONLISTID_SIMPLE,					/* 16コマ連続 */
	CHARMOTIONLISTID_DEFENSE_UP,				/* 防御(上) */
	CHARMOTIONLISTID_DEFENSE_DOWN,				/* 防御(下) */
	CHARMOTIONLISTID_DEFENSE_LEFT,				/* 防御(左) */
	CHARMOTIONLISTID_DEFENSE_RIGHT,				/* 防御(右) */
	CHARMOTIONLISTID_DEFENSE_SUCCESS_UP,		/* 防御成功(上) */
	CHARMOTIONLISTID_DEFENSE_SUCCESS_DOWN,		/* 防御成功(下) */
	CHARMOTIONLISTID_DEFENSE_SUCCESS_LEFT,		/* 防御成功(左) */
	CHARMOTIONLISTID_DEFENSE_SUCCESS_RIGHT,		/* 防御成功(右) */
	CHARMOTIONLISTID_BOWDEFENSE_UP,				/* 弓用防御(上) */
	CHARMOTIONLISTID_BOWDEFENSE_DOWN,			/* 弓用防御(下) */
	CHARMOTIONLISTID_BOWDEFENSE_LEFT,			/* 弓用防御(左) */
	CHARMOTIONLISTID_BOWDEFENSE_RIGHT,			/* 弓用防御(右) */
	CHARMOTIONLISTID_DAMAGE_UP,					/* ダメージ(上) */
	CHARMOTIONLISTID_DAMAGE_DOWN,				/* ダメージ(下) */
	CHARMOTIONLISTID_DAMAGE_LEFT,				/* ダメージ(左) */
	CHARMOTIONLISTID_DAMAGE_RIGHT,				/* ダメージ(右) */
	CHARMOTIONLISTID_DEAD_UP,					/* 死亡(上) */
	CHARMOTIONLISTID_DEAD_DOWN,					/* 死亡(下) */
	CHARMOTIONLISTID_DEAD_LEFT,					/* 死亡(左) */
	CHARMOTIONLISTID_DEAD_RIGHT,				/* 死亡(右) */
	CHARMOTIONLISTID_MAX
};

/* 行動ID */
enum {
	CHARMOTIONPROCID_NONE = 0,					/* 何もしない */
	CHARMOTIONPROCID_ATACK,						/* 攻撃 */
	CHARMOTIONPROCID_FISHING,					/* 釣り */
	CHARMOTIONPROCID_MAX
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMotion : public CInfoBase
{
public:
			CInfoMotion();									/* コンストラクタ */
	virtual ~CInfoMotion();									/* デストラクタ */

	int		GetElementCount		(void);								/* 要素数を取得 */
	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	void	Copy				(CInfoMotion *pSrc);				/* コピー */


public:
	int			m_nElementCount;				/* 要素数 */

	/* 保存しないデータ */

	/* 保存するデータ */
	BYTE		m_byWait,						/* 待ち時間(×１０ミリ秒) */
				m_byLevel1,						/* 透明度1 */
				m_byLevel2,						/* 透明度2 */
				m_byLevel3;						/* 透明度3 */
	BOOL		m_bPile,						/* 重ね画像を先に描画 */
				m_bRedrawHand,					/* 手を手前に描画 */
				m_bLoop;						/* ループ再生 */
	WORD		m_wGrpIDMainBase,				/* グラフィックIDメイン(下地) */
				m_wGrpIDMainPile1,				/* グラフィックIDメイン(重ね合わせ1) */
				m_wGrpIDMainPile2,				/* グラフィックIDメイン(重ね合わせ2) */
				m_wGrpIDMainPile3,				/* グラフィックIDメイン(重ね合わせ3) */
				m_wGrpIDSubBase,				/* グラフィックIDサブ(下地) */
				m_wGrpIDSubPile1,				/* グラフィックIDサブ(重ね合わせ1) */
				m_wGrpIDSubPile2,				/* グラフィックIDサブ(重ね合わせ2) */
				m_wGrpIDSubPile3;				/* グラフィックIDサブ(重ね合わせ3) */
	DWORD		m_dwMotionID,					/* モーションID */
				m_dwMotionTypeID,				/* モーション種別ID */
				m_dwMotionListID,				/* モーションリストID */
				m_dwSoundID,					/* 効果音ID */
				m_dwProcID;						/* 行動ID */
	POINT		m_ptDrawPosPile0,				/* 描画位置(下地) */
				m_ptDrawPosPile1,				/* 描画位置(重ね合わせ1) */
				m_ptDrawPosPile2,				/* 描画位置(重ね合わせ2) */
				m_ptDrawPosPile3;				/* 描画位置(重ね合わせ3) */
	ARRAYINT	m_anDrawList;					/* 描画順リスト */
} CInfoMotion, *PCInfoMotion;
typedef CmyArray<PCInfoMotion, PCInfoMotion>	   ARRAYMOTIONINFO;
typedef CmyArray<PCInfoMotion, PCInfoMotion>	 *PARRAYMOTIONINFO;

/* Copyright(C)URARA-works 2007 */
