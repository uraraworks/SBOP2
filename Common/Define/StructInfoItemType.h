#pragma once

/* ========================================================================= */
/* 構造体定義																 */
/* ========================================================================= */

/* アイテム種別情報構造体(持ち物) */
struct stInfoItemTypeArms {
	DWORD		dwWeaponInfoID;				/* 武器情報ID */
	DWORD		dwValue;					/* 攻撃力 */
	DWORD		dwMoveWait;					/* 速度 */
	DWORD		dwMoveCount;				/* 飛距離 */

	stInfoItemTypeArms()
	: dwWeaponInfoID(0)
	, dwValue(0)
	, dwMoveWait(0)
	, dwMoveCount(0)
	{}
};

/* アイテム種別情報構造体(HP増減) */
struct stInfoItemTypeHp {
	DWORD		dwMin;						/* 効果(最小) */
	DWORD		dwMax;						/* 効果(最大) */

	stInfoItemTypeHp()
	: dwMin(0)
	, dwMax(0)
	{}
};

/* アイテム種別情報構造体(灯り) */
struct stInfoItemTypeLight {
	DWORD		dwLevel;					/* 灯りレベル */
	DWORD		dwTime;						/* 持続時間 */

	stInfoItemTypeLight()
	: dwLevel(0)
	, dwTime(0)
	{}
};

/* アイテム種別情報構造体 */
struct stInfoItemType {
	BYTE		byDelAverage;				/* 使ってなくなる確率 */
	BYTE		byTarget;					/* 使用対象 */
	BYTE		byArea;						/* 使用範囲 */
	BOOL		bPutOn;						/* 上に置けるか判定 */
	WORD		wGrpIDMain;					/* 画像IDメイン */
	WORD		wGrpIDSub;					/* 画像IDサブ */
	DWORD		dwTypeID;					/* 種別ID */
	DWORD		dwItemTypeID;				/* アイテム種別ID */
	DWORD		dwGrpID;					/* 地面にある時の画像ID */
	DWORD		dwIconGrpID;				/* バックパック内の画像ID */
	DWORD		dwDropSoundID;				/* 落ちたときの効果音ID */
	DWORD		dwUseEffectID;				/* 使った時に再生するエフェクトID */
	DWORD		dwUseSoundID;				/* 使った時に再生する効果音ID */
	CmyString	strName;					/* アイテム名 */

	union Type {
		stInfoItemTypeArms	Arms;			/* 持ち物 */
		stInfoItemTypeHp	Hp;				/* HP増減 */
		stInfoItemTypeLight	Light;			/* 灯り */
	};

	stInfoItemType()
	: byDelAverage(0)
	, byTarget(0)
	, byArea(0)
	, bPutOn(0)
	, wGrpIDMain(0)
	, wGrpIDSub(0)
	, dwTypeID(0)
	, dwItemTypeID(0)
	, dwGrpID(0)
	, dwIconGrpID(0)
	, dwDropSoundID(0)
	, dwUseEffectID(0)
	, dwUseSoundID(0)
	{}
}
