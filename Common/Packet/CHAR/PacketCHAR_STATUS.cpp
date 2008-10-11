/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_STATUS.cpp										 */
/* 内容			:コマンド(キャラ系:ステータス情報通知) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/29													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketCHAR_STATUS.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_STATUS::CPacketCHAR_STATUS							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

CPacketCHAR_STATUS::CPacketCHAR_STATUS()
{
	m_byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	m_byCmdSub	= SBOCOMMANDID_SUB_CHAR_STATUS;

	m_wAtackGauge			= 0;	/* アタックゲージ */
	m_wDefenseGauge			= 0;	/* ディフェンスゲージ */
	m_wStamina				= 0;	/* スタミナ */
	m_wPower				= 0;	/* 腕力 */
	m_wStrength				= 0;	/* 体力 */
	m_wMagic				= 0;	/* 魔力 */
	m_wSkillful				= 0;	/* 器用 */
	m_wAbillityAT			= 0;	/* 攻撃技能 */
	m_wAbillityDF			= 0;	/* 防御技能 */
	m_wPAtack				= 0;	/* 攻撃力 */
	m_wPDefense				= 0;	/* 防御力 */
	m_wPMagic				= 0;	/* 魔法力 */
	m_wPMagicDefense		= 0;	/* 魔法防御力 */
	m_wPHitAverage			= 0;	/* 命中率 */
	m_wPAvoidAverage		= 0;	/* 回避率 */
	m_wPCriticalAverage		= 0;	/* クリティカル率 */
	m_wAttrFire				= 0;	/* 属性[火] */
	m_wAttrWind				= 0;	/* 属性[風] */
	m_wAttrWater			= 0;	/* 属性[水] */
	m_wAttrEarth			= 0;	/* 属性[土] */
	m_wAttrLight			= 0;	/* 属性[光] */
	m_wAttrDark				= 0;	/* 属性[闇] */
	m_dwCharID				= 0;	/* キャラID */
	m_dwMoveWait			= 0;	/* 移動待ち時間 */
	m_dwMoveWaitBattle		= 0;	/* 戦闘時移動待ち時間 */
	m_dwHP					= 0;	/* HP */
	m_dwMaxHP				= 0;	/* 最大HP */
	m_dwSP					= 0;	/* SP */
	m_dwMaxSP				= 0;	/* 最大SP */
	m_nDropItemAverage		= 0;	/* アイテムドロップ率 */
	m_nMoveAverage			= 0;	/* 移動確率 */
	m_nMoveAverageBattle	= 0;	/* 戦闘時移動確率 */
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_STATUS::~CPacketCHAR_STATUS						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

CPacketCHAR_STATUS::~CPacketCHAR_STATUS()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_STATUS::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

void CPacketCHAR_STATUS::Make(
	CInfoCharBase *pInfo)		/* [in] キャラ情報 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)				+
			 sizeof (pInfo->m_dwCharID)			+
			 sizeof (m_wAtackGauge)				+	/* アタックゲージ */
			 sizeof (m_wDefenseGauge)			+	/* ディフェンスゲージ */
			 sizeof (m_wStamina)				+	/* スタミナ */
			 sizeof (m_wPower)					+	/* 腕力 */
			 sizeof (m_wStrength)				+	/* 体力 */
			 sizeof (m_wMagic)					+	/* 魔力 */
			 sizeof (m_wSkillful)				+	/* 器用 */
			 sizeof (m_wAbillityAT)				+	/* 攻撃技能 */
			 sizeof (m_wAbillityDF)				+	/* 防御技能 */
			 sizeof (m_wPAtack)					+	/* 攻撃力 */
			 sizeof (m_wPDefense)				+	/* 防御力 */
			 sizeof (m_wPMagic)					+	/* 魔法力 */
			 sizeof (m_wPMagicDefense)			+	/* 魔法防御力 */
			 sizeof (m_wPHitAverage)			+	/* 命中率 */
			 sizeof (m_wPAvoidAverage)			+	/* 回避率 */
			 sizeof (m_wPCriticalAverage)		+	/* クリティカル率 */
			 sizeof (m_wAttrFire)				+	/* 属性[火] */
			 sizeof (m_wAttrWind)				+	/* 属性[風] */
			 sizeof (m_wAttrWater)				+	/* 属性[水] */
			 sizeof (m_wAttrEarth)				+	/* 属性[土] */
			 sizeof (m_wAttrLight)				+	/* 属性[光] */
			 sizeof (m_wAttrDark)				+	/* 属性[闇] */
			 sizeof (pInfo->m_dwMoveWait)		+
			 sizeof (pInfo->m_dwMoveWaitBattle)	+
			 sizeof (pInfo->m_dwHP)				+
			 sizeof (pInfo->m_dwMaxHP)			+
			 sizeof (pInfo->m_dwSP)				+
			 sizeof (pInfo->m_dwMaxSP)			+
			 sizeof (pInfo->m_nDropItemAverage)	+
			 sizeof (pInfo->m_nMoveAverage)		+
			 sizeof (pInfo->m_nMoveAverageBattle);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= m_byCmdMain;
	pPacketBase->byCmdSub	= m_byCmdSub;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwCharID,				sizeof (m_dwCharID),			pDataTmp);	/* キャラID */
//	CopyMemoryRenew (pDataTmp, &pInfo->m_wAtackGauge,			sizeof (m_wAtackGauge),			pDataTmp);	/* アタックゲージ */
//	CopyMemoryRenew (pDataTmp, &pInfo->m_wDefenseGauge,			sizeof (m_wDefenseGauge),		pDataTmp);	/* ディフェンスゲージ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wStamina,				sizeof (m_wStamina),			pDataTmp);	/* スタミナ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPower,				sizeof (m_wPower),				pDataTmp);	/* 腕力 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wStrength,				sizeof (m_wStrength),			pDataTmp);	/* 体力 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wMagic,				sizeof (m_wMagic),				pDataTmp);	/* 魔力 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wSkillful,				sizeof (m_wSkillful),			pDataTmp);	/* 器用 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAbillityAT,			sizeof (m_wAbillityAT),			pDataTmp);	/* 攻撃技能 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAbillityDF,			sizeof (m_wAbillityDF),			pDataTmp);	/* 防御技能 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPAtack,				sizeof (m_wPAtack),				pDataTmp);	/* 攻撃力 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPDefense,				sizeof (m_wPDefense),			pDataTmp);	/* 防御力 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPMagic,				sizeof (m_wPMagic),				pDataTmp);	/* 魔法力 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPMagicDefense,		sizeof (m_wPMagicDefense),		pDataTmp);	/* 魔法防御力 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPHitAverage,			sizeof (m_wPHitAverage),		pDataTmp);	/* 命中率 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPAvoidAverage,		sizeof (m_wPAvoidAverage),		pDataTmp);	/* 回避率 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPCriticalAverage,		sizeof (m_wPCriticalAverage),	pDataTmp);	/* クリティカル率 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrFire,				sizeof (m_wAttrFire),			pDataTmp);	/* 属性[火] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrWind,				sizeof (m_wAttrWind),			pDataTmp);	/* 属性[風] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrWater,			sizeof (m_wAttrWater),			pDataTmp);	/* 属性[水] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrEarth,			sizeof (m_wAttrEarth),			pDataTmp);	/* 属性[土] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrLight,			sizeof (m_wAttrLight),			pDataTmp);	/* 属性[光] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrDark,				sizeof (m_wAttrDark),			pDataTmp);	/* 属性[闇] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMoveWait,			sizeof (m_dwMoveWait),			pDataTmp);	/* 移動待ち時間 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMoveWaitBattle,		sizeof (m_dwMoveWaitBattle),	pDataTmp);	/* 戦闘時移動待ち時間 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwHP,					sizeof (m_dwHP),				pDataTmp);	/* HP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMaxHP,				sizeof (m_dwMaxHP),				pDataTmp);	/* 最大HP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwSP,					sizeof (m_dwSP),				pDataTmp);	/* SP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMaxSP,				sizeof (m_dwMaxSP),				pDataTmp);	/* 最大SP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_nDropItemAverage,		sizeof (m_nDropItemAverage),	pDataTmp);	/* アイテムドロップ率 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_nMoveAverage,			sizeof (m_nMoveAverage),		pDataTmp);	/* 移動確率 */
	CopyMemoryRenew (pDataTmp, &pInfo->m_nMoveAverageBattle,	sizeof (m_nMoveAverageBattle),	pDataTmp);	/* 戦闘時移動確率 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_STATUS::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

PBYTE CPacketCHAR_STATUS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,				pDataTmp, sizeof (m_dwCharID),				pDataTmp);	/* キャラID */
//	CopyMemoryRenew (&m_wAtackGauge,			pDataTmp, sizeof (m_wAtackGauge),			pDataTmp);	/* アタックゲージ */
//	CopyMemoryRenew (&m_wDefenseGauge,			pDataTmp, sizeof (m_wDefenseGauge),			pDataTmp);	/* ディフェンスゲージ */
	CopyMemoryRenew (&m_wStamina,				pDataTmp, sizeof (m_wStamina),				pDataTmp);	/* スタミナ */
	CopyMemoryRenew (&m_wPower,					pDataTmp, sizeof (m_wPower),				pDataTmp);	/* 腕力 */
	CopyMemoryRenew (&m_wStrength,				pDataTmp, sizeof (m_wStrength),				pDataTmp);	/* 体力 */
	CopyMemoryRenew (&m_wMagic,					pDataTmp, sizeof (m_wMagic),				pDataTmp);	/* 魔力 */
	CopyMemoryRenew (&m_wSkillful,				pDataTmp, sizeof (m_wSkillful),				pDataTmp);	/* 器用 */
	CopyMemoryRenew (&m_wAbillityAT,			pDataTmp, sizeof (m_wAbillityAT),			pDataTmp);	/* 攻撃技能 */
	CopyMemoryRenew (&m_wAbillityDF,			pDataTmp, sizeof (m_wAbillityDF),			pDataTmp);	/* 防御技能 */
	CopyMemoryRenew (&m_wPAtack,				pDataTmp, sizeof (m_wPAtack),				pDataTmp);	/* 攻撃力 */
	CopyMemoryRenew (&m_wPDefense,				pDataTmp, sizeof (m_wPDefense),				pDataTmp);	/* 防御力 */
	CopyMemoryRenew (&m_wPMagic,				pDataTmp, sizeof (m_wPMagic),				pDataTmp);	/* 魔法力 */
	CopyMemoryRenew (&m_wPMagicDefense,			pDataTmp, sizeof (m_wPMagicDefense),		pDataTmp);	/* 魔法防御力 */
	CopyMemoryRenew (&m_wPHitAverage,			pDataTmp, sizeof (m_wPHitAverage),			pDataTmp);	/* 命中率 */
	CopyMemoryRenew (&m_wPAvoidAverage,			pDataTmp, sizeof (m_wPAvoidAverage),		pDataTmp);	/* 回避率 */
	CopyMemoryRenew (&m_wPCriticalAverage,		pDataTmp, sizeof (m_wPCriticalAverage),		pDataTmp);	/* クリティカル率 */
	CopyMemoryRenew (&m_wAttrFire,				pDataTmp, sizeof (m_wAttrFire),				pDataTmp);	/* 属性[火] */
	CopyMemoryRenew (&m_wAttrWind,				pDataTmp, sizeof (m_wAttrWind),				pDataTmp);	/* 属性[風] */
	CopyMemoryRenew (&m_wAttrWater,				pDataTmp, sizeof (m_wAttrWater),			pDataTmp);	/* 属性[水] */
	CopyMemoryRenew (&m_wAttrEarth,				pDataTmp, sizeof (m_wAttrEarth),			pDataTmp);	/* 属性[土] */
	CopyMemoryRenew (&m_wAttrLight,				pDataTmp, sizeof (m_wAttrLight),			pDataTmp);	/* 属性[光] */
	CopyMemoryRenew (&m_wAttrDark,				pDataTmp, sizeof (m_wAttrDark),				pDataTmp);	/* 属性[闇] */
	CopyMemoryRenew (&m_dwMoveWait,				pDataTmp, sizeof (m_dwMoveWait),			pDataTmp);	/* 移動待ち時間 */
	CopyMemoryRenew (&m_dwMoveWaitBattle,		pDataTmp, sizeof (m_dwMoveWaitBattle),		pDataTmp);	/* 戦闘時移動待ち時間 */
	CopyMemoryRenew (&m_dwHP,					pDataTmp, sizeof (m_dwHP),					pDataTmp);	/* HP */
	CopyMemoryRenew (&m_dwMaxHP,				pDataTmp, sizeof (m_dwMaxHP),				pDataTmp);	/* 最大HP */
	CopyMemoryRenew (&m_dwSP,					pDataTmp, sizeof (m_dwSP),					pDataTmp);	/* SP */
	CopyMemoryRenew (&m_dwMaxSP,				pDataTmp, sizeof (m_dwMaxSP),				pDataTmp);	/* 最大SP */
	CopyMemoryRenew (&m_nDropItemAverage,		pDataTmp, sizeof (m_nDropItemAverage),		pDataTmp);	/* アイテムドロップ率 */
	CopyMemoryRenew (&m_nMoveAverage,			pDataTmp, sizeof (m_nMoveAverage),			pDataTmp);	/* 移動確率 */
	CopyMemoryRenew (&m_nMoveAverageBattle,		pDataTmp, sizeof (m_nMoveAverageBattle),	pDataTmp);	/* 戦闘時移動確率 */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_STATUS::SetParam									 */
/* 内容		:指定キャラ情報へパラメータを反映								 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

void CPacketCHAR_STATUS::SetParam(CInfoCharBase *pInfo)
{
	if (pInfo == NULL) {
		return;
	}

//	pInfo->m_wAtackGauge		= m_wAtackGauge;		/* アタックゲージ */
//	pInfo->m_wDefenseGauge		= m_wDefenseGauge;		/* ディフェンスゲージ */
	pInfo->m_wStamina			= m_wStamina;			/* スタミナ */
	pInfo->m_wPower				= m_wPower;				/* 腕力 */
	pInfo->m_wStrength			= m_wStrength;			/* 体力 */
	pInfo->m_wMagic				= m_wMagic;				/* 魔力 */
	pInfo->m_wSkillful			= m_wSkillful;			/* 器用 */
	pInfo->m_wAbillityAT		= m_wAbillityAT;		/* 攻撃技能 */
	pInfo->m_wAbillityDF		= m_wAbillityDF;		/* 防御技能 */
	pInfo->m_wPAtack			= m_wPAtack;			/* 攻撃力 */
	pInfo->m_wPDefense			= m_wPDefense;			/* 防御力 */
	pInfo->m_wPMagic			= m_wPMagic;			/* 魔法力 */
	pInfo->m_wPMagicDefense		= m_wPMagicDefense;		/* 魔法防御力 */
	pInfo->m_wPHitAverage		= m_wPHitAverage;		/* 命中率 */
	pInfo->m_wPAvoidAverage		= m_wPAvoidAverage;		/* 回避率 */
	pInfo->m_wPCriticalAverage	= m_wPCriticalAverage;	/* クリティカル率 */
	pInfo->m_wAttrFire			= m_wAttrFire;			/* 属性[火] */
	pInfo->m_wAttrWind			= m_wAttrWind;			/* 属性[風] */
	pInfo->m_wAttrWater			= m_wAttrWater;			/* 属性[水] */
	pInfo->m_wAttrEarth			= m_wAttrEarth;			/* 属性[土] */
	pInfo->m_wAttrLight			= m_wAttrLight;			/* 属性[光] */
	pInfo->m_wAttrDark			= m_wAttrDark;			/* 属性[闇] */
	pInfo->m_dwMoveWait			= m_dwMoveWait;			/* 移動待ち時間 */
	pInfo->m_dwMoveWaitBattle	= m_dwMoveWaitBattle;	/* 戦闘時移動待ち時間 */
	pInfo->m_dwHP				= m_dwHP;				/* HP */
	pInfo->m_dwMaxHP			= m_dwMaxHP;			/* 最大HP */
	pInfo->m_dwSP				= m_dwSP;				/* SP */
	pInfo->m_dwMaxSP			= m_dwMaxSP;			/* 最大SP */
	pInfo->m_nDropItemAverage	= m_nDropItemAverage;	/* アイテムドロップ率 */
	pInfo->m_nMoveAverage		= m_nMoveAverage;		/* 移動確率 */
	pInfo->m_nMoveAverageBattle	= m_nMoveAverageBattle;	/* 戦闘時移動確率 */
}

/* Copyright(C)URARA-works 2008 */
