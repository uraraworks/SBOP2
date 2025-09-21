/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_STATUS.h										 */
/* 内容			:コマンド(キャラ系:ステータス情報通知) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/29													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_STATUS : public CPacketBase
{
public:
			CPacketCHAR_STATUS();						/* コンストラクタ */
	virtual ~CPacketCHAR_STATUS();						/* デストラクタ */

	void	Make		(CInfoCharBase *pInfo);					/* パケットを作成 */
	PBYTE	Set			(PBYTE pPacket);						/* パケットを設定 */
	void	SetParam	(CInfoCharBase *pInfo);					/* 指定キャラ情報へパラメータを反映 */


public:
	BYTE	m_byCmdMain,				/* コマンドIDメイン */
			m_byCmdSub;					/* コマンドIDサブ */
	WORD	m_wAtackGauge,				/* アタックゲージ */
			m_wDefenseGauge,			/* ディフェンスゲージ */
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
	DWORD	m_dwCharID,					/* キャラID */
			m_dwMoveWait,				/* 移動待ち時間 */
			m_dwMoveWaitBattle,			/* 戦闘時移動待ち時間 */
			m_dwExp,					/* 経験値 */
			m_dwHP,						/* HP */
			m_dwMaxHP,					/* 最大HP */
			m_dwSP,						/* SP */
			m_dwMaxSP,					/* 最大SP */
			m_dwLightTime;				/* 灯り継続時間 */
	int		m_nDropItemAverage,			/* アイテムドロップ率 */
			m_nMoveAverage,				/* 移動確率 */
			m_nMoveAverageBattle,		/* 戦闘時移動確率 */
			m_nLightLevel;				/* 灯りレベル */
	SIZE	m_sizeSearchDistance;		/* 策敵範囲 */
} CPacketCHAR_STATUS, *PCPacketCHAR_STATUS;

/* Copyright(C)URARA-works 2008 */
