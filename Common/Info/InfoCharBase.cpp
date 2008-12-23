/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:InfoCharBase.cpp											 */
/* 内容			:キャラ情報基底クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_nMapX",					/* マップ座標(横) */
	"m_nMapY",					/* マップ座標(縦) */
	"m_nMoveState",				/* 移動状態 */
	"m_nMoveType",				/* 移動種別 */
	"m_nDirection",				/* 向き */
	"m_nGrpSize",				/* 画像サイズ */
	"m_nSex",					/* 性別 */
	"m_nMaxItemCount",			/* 最大アイテム所持数 */
	"m_nDropItemAverage",		/* アイテムドロップ率 */
	"m_nMoveAverage",			/* 移動確率 */
	"m_nMoveAverageBattle",		/* 戦闘時移動確率 */
	"m_wFamilyID",				/* 種族ID */
	"m_wGrpIDNPC",				/* 画像ID(NPC) */
	"m_wGrpIDCloth",			/* 画像ID(服) */
	"m_wGrpIDEye",				/* 画像ID(目) */
	"m_wGrpIDEyeColor",			/* 画像ID(目色) */
	"m_wGrpIDHairType",			/* 画像ID(髪) */
	"m_wGrpIDHairColor",		/* 画像ID(髪色) */
	"m_wGrpIDSP",				/* 画像ID(特殊服) */
	"m_wGrpIDTmpMain",			/* 画像ID(一時服:メイン) */
	"m_wGrpIDTmpSub",			/* 画像ID(一時服:サブ) */
	"m_wGrpIDAcce",				/* 画像ID(アクセサリ) */
	"m_wGrpIDArmsMain",			/* 画像ID(持ち物:メイン) */
	"m_wGrpIDArmsSub",			/* 画像ID(持ち物:サブ) */
	"m_wGrpIDArmsLeftMain",		/* 画像ID(盾:メイン) */
	"m_wGrpIDArmsLeftSub",		/* 画像ID(盾:サブ) */
	"m_dwCharID",				/* キャラID */
	"m_dwMapID",				/* マップID */
	"m_dwMotionTypeID",			/* モーション種別ID */
	"m_clName",					/* 名前の描画色 */
	"m_clSpeak",				/* 発言の描画色 */
	"m_strCharName",			/* キャラ名 */
	"m_strSpeak",				/* 発言内容 */
	"m_strTalk",				/* 会話データ */
	"m_adwItemID",				/* 所持アイテム */
	"m_adwSkillID",				/* 所持スキル */
	"m_bBlock",					/* ぶつかる判定 */
	"m_bPush",					/* 押せる判定 */
	"m_dwEquipItemIDCloth",		/* 装備アイテムID:服 */
	"m_dwEquipItemIDAcce1",		/* 装備アイテムID:アクセサリ1 */
	"m_dwEquipItemIDAcce2",		/* 装備アイテムID:アクセサリ2 */
	"m_dwEquipItemIDArmsRight",	/* 装備アイテムID:右手 */
	"m_dwEquipItemIDArmsLeft",	/* 装備アイテムID:左手 */
	"m_dwEquipItemIDHead",		/* 装備アイテムID:頭 */
	"m_wGrpIDInitNPC",			/* 初期画像ID(NPC) */
	"m_wGrpIDInitCloth",		/* 初期画像ID(服) */
	"m_wGrpIDInitEye",			/* 初期画像ID(目) */
	"m_wGrpIDInitEyeColor",		/* 初期画像ID(目色) */
	"m_wGrpIDInitHairType",		/* 初期画像ID(髪) */
	"m_wGrpIDInitHairColor",	/* 初期画像ID(髪色) */
	"m_wGrpIDInitSP",			/* 初期画像ID(特殊服) */
	"m_wLevel",					/* レベル */
	"m_wStamina",				/* スタミナ */
	"m_wPower",					/* 腕力 */
	"m_wStrength",				/* 体力 */
	"m_wMagic",					/* 魔力 */
	"m_wSkillful",				/* 器用 */
	"m_wAbillityAT",			/* 攻撃技能 */
	"m_wAbillityDF",			/* 防御技能 */
	"m_wPAtack",				/* 攻撃力 */
	"m_wPDefense",				/* 防御力 */
	"m_wPMagic",				/* 魔法力 */
	"m_wPMagicDefense",			/* 魔法防御力 */
	"m_wPHitAverage",			/* 命中率 */
	"m_wPAvoidAverage",			/* 回避率 */
	"m_wPCriticalAverage",		/* クリティカル率 */
	"m_wAttrFire",				/* 属性[火] */
	"m_wAttrWind",				/* 属性[風] */
	"m_wAttrWater",				/* 属性[水] */
	"m_wAttrEarth",				/* 属性[土] */
	"m_wAttrLight",				/* 属性[光] */
	"m_wAttrDark",				/* 属性[闇] */
	"m_dwMoveWait",				/* 移動待ち時間 */
	"m_dwMoveWaitBattle",		/* 戦闘時移動待ち時間 */
	"m_dwExp",					/* 経験値 */
	"m_dwHP",					/* HP */
	"m_dwMaxHP",				/* 最大HP */
	"m_dwSP",					/* SP */
	"m_dwMaxSP",				/* 最大SP */
	/* NPC発生 */
	"m_dwPutCycle",				/* 発生周期 */
	"m_nPutMoveType",			/* 発生させる移動種別 */
	"m_nMaxPutCount",			/* 同時発生数 */
	"m_nPutAverage",			/* 発生確率 */
	"m_ptPutArea",				/* 発生範囲(半径) */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoCharBase::CInfoCharBase									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CInfoCharBase::CInfoCharBase()
{
	m_bDelete				= FALSE;
	m_bBlock				= FALSE;
	m_bPush					= FALSE;
	m_nAnime				= 0;
	m_nDirectionBack		= 0;
	m_nMapX					= 1;
	m_nMapY					= 1;
	m_nMoveState			= CHARMOVESTATE_STAND;
	m_nProcState			= CHARPROCSTATEID_NORMAL;
	m_nMoveDirection		= -1;
	m_nLightLevel			= 0;				/* 灯りレベル */
	m_nMoveType				= CHARMOVETYPE_PC;
	m_nDirection			= 1;
	m_nGrpSize				= 16;
	m_nSex					= SEX_MALE;
	m_nMaxItemCount			= 25;
	m_nDropItemAverage		= 0;
	m_nMoveAverage			= 0;				/* 移動確率 */
	m_nMoveAverageBattle	= 0;				/* 戦闘時移動確率 */
	m_bNPC					= FALSE;
	m_bChargeAtack			= FALSE;			/* 溜め攻撃 */
	m_wFamilyID				= FAMILYTYPE_HUMAN;
	m_wGrpIDNPC				= 0;
	m_wGrpIDCloth			= 0;
	m_wGrpIDEye				= 1;
	m_wGrpIDEyeColor		= 1;
	m_wGrpIDHairType		= 1;
	m_wGrpIDHairColor		= 1;
	m_wGrpIDSP				= 0;
	m_wGrpIDTmpMain			= 0;
	m_wGrpIDTmpSub			= 0;
	m_wGrpIDAcce			= 0;
	m_wGrpIDArmsMain		= 0;
	m_wGrpIDArmsSub			= 0;
	m_wGrpIDArmsLeftMain	= 0;
	m_wGrpIDArmsLeftSub		= 0;
	m_wGrpIDInitNPC			= m_wGrpIDNPC;
	m_wGrpIDInitCloth		= m_wGrpIDCloth;
	m_wGrpIDInitEye			= m_wGrpIDEye;
	m_wGrpIDInitEyeColor	= m_wGrpIDEyeColor;
	m_wGrpIDInitHairType	= m_wGrpIDHairType;
	m_wGrpIDInitHairColor	= m_wGrpIDHairColor;
	m_wGrpIDInitSP			= m_wGrpIDSP;
	m_wAtackGauge			= 0;					/* アタックゲージ */
	m_wDefenseGauge			= 0;					/* ディフェンスゲージ */
	m_wLevel				= 1;					/* レベル */
	m_wStamina				= 0;					/* スタミナ */
	m_wPower				= 0;					/* 腕力 */
	m_wStrength				= 0;					/* 体力 */
	m_wMagic				= 0;					/* 魔力 */
	m_wSkillful				= 0;					/* 器用 */
	m_wAbillityAT			= 0;					/* 攻撃技能 */
	m_wAbillityDF			= 0;					/* 防御技能 */
	m_wPAtack				= 0;					/* 攻撃力 */
	m_wPDefense				= 0;					/* 防御力 */
	m_wPMagic				= 0;					/* 魔法力 */
	m_wPMagicDefense		= 0;					/* 魔法防御力 */
	m_wPHitAverage			= 0;					/* 命中率 */
	m_wPAvoidAverage		= 0;					/* 回避率 */
	m_wPCriticalAverage		= 0;					/* クリティカル率 */
	m_wAttrFire				= 0;					/* 属性[火] */
	m_wAttrWind				= 0;					/* 属性[風] */
	m_wAttrWater			= 0;					/* 属性[水] */
	m_wAttrEarth			= 0;					/* 属性[土] */
	m_wAttrLight			= 0;					/* 属性[光] */
	m_wAttrDark				= 0;					/* 属性[闇] */
	m_dwCharID				= 0;
	m_dwMapID				= 1;
	m_dwMotionTypeID		= 1;
	m_dwSessionID			= 0;
	m_dwAccountID			= 0;
	m_dwLastTimeSpeak		= 0;
	m_dwLastTimeMove		= 0;
	m_dwTailCharID			= 0;
	m_dwFrontCharID			= 0;
	m_dwParentCharID		= 0;
	m_dwTargetCharID		= 0;
	m_dwLightTime			= 0;					/* 灯り有効時間 */

	m_dwEquipItemIDCloth		= 0;
	m_dwEquipItemIDAcce1		= 0;
	m_dwEquipItemIDAcce2		= 0;
	m_dwEquipItemIDArmsRight	= 0;
	m_dwEquipItemIDArmsLeft		= 0;
	m_dwEquipItemIDHead			= 0;
	m_dwMoveWait				= MOVEWAIT;
	m_dwMoveWaitBattle			= 0;
	m_dwExp		= 0;
	m_dwHP		= 10;
	m_dwMaxHP	= 10;
	m_dwSP		= 0;
	m_dwMaxSP	= 0;

	ZeroMemory (m_adwMotionID, sizeof (m_adwMotionID));
	m_adwMotionID[CHARMOTIONID_STAND]		= CHARMOTIONLISTID_STAND_UP;		/* モーションID(立ち) */
	m_adwMotionID[CHARMOTIONID_WALK]		= CHARMOTIONLISTID_WALK_UP;			/* モーションID(歩き) */
	m_adwMotionID[CHARMOTIONID_SIT]			= CHARMOTIONLISTID_SIT_UP;			/* モーションID(座り) */
	m_adwMotionID[CHARMOTIONID_BATTLESTAND]	= CHARMOTIONLISTID_BATTLESTAND_UP;	/* モーションID(戦闘立ち) */
	m_adwMotionID[CHARMOTIONID_BATTLEWALK]	= CHARMOTIONLISTID_BATTLEWALK_UP;	/* モーションID(すり足) */
	m_adwMotionID[CHARMOTIONID_ATACK]		= CHARMOTIONLISTID_SWING_UP;		/* モーションID(攻撃) */

	m_clName	= RGB (255, 255, 255);
	m_clSpeak	= RGB (255, 255, 255);
	m_ptCharBack.x = m_ptCharBack.y = 0;
	m_ptViewCharPos.x = m_ptViewCharPos.y = 0;

	/* NPC発生 */
	m_dwPutCycle		= 0;				/* 発生周期 */
	m_nPutMoveType		= 0;				/* 発生させる移動種別 */
	m_nMaxPutCount		= 0;				/* 同時発生数 */
	m_nPutAverage		= 0;				/* 発生確率 */
	m_ptPutArea.x = m_ptPutArea.y = 0;		/* 発生範囲(半径) */

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::~CInfoCharBase									 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CInfoCharBase::~CInfoCharBase()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetElementNo									 */
/* 内容		:要素番号を取得													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

int CInfoCharBase::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetDataSize										 */
/* 内容		:データサイズを取得												 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoCharBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_bBlock);
	dwRet += sizeof (m_bPush);
	dwRet += sizeof (m_nMapX);
	dwRet += sizeof (m_nMapY);
	dwRet += sizeof (m_nMoveState);
	dwRet += sizeof (m_nMoveType);
	dwRet += sizeof (m_nDirection);
	dwRet += sizeof (m_nGrpSize);
	dwRet += sizeof (m_nSex);
	dwRet += sizeof (m_nMaxItemCount);
	dwRet += sizeof (m_nDropItemAverage);
	dwRet += sizeof (m_nMoveAverage),			/* 移動確率 */
	dwRet += sizeof (m_nMoveAverageBattle),		/* 戦闘時移動確率 */
	dwRet += sizeof (m_wFamilyID);
	dwRet += sizeof (m_wGrpIDNPC);
	dwRet += sizeof (m_wGrpIDCloth);
	dwRet += sizeof (m_wGrpIDEye);
	dwRet += sizeof (m_wGrpIDEyeColor);
	dwRet += sizeof (m_wGrpIDHairType);
	dwRet += sizeof (m_wGrpIDHairColor);
	dwRet += sizeof (m_wGrpIDSP);
	dwRet += sizeof (m_wGrpIDTmpMain);
	dwRet += sizeof (m_wGrpIDTmpSub);
	dwRet += sizeof (m_wGrpIDAcce);
	dwRet += sizeof (m_wGrpIDArmsMain);
	dwRet += sizeof (m_wGrpIDArmsSub);
	dwRet += sizeof (m_wGrpIDArmsLeftMain);
	dwRet += sizeof (m_wGrpIDArmsLeftSub);
	dwRet += sizeof (m_wGrpIDInitNPC);
	dwRet += sizeof (m_wGrpIDInitCloth);
	dwRet += sizeof (m_wGrpIDInitEye);
	dwRet += sizeof (m_wGrpIDInitEyeColor);
	dwRet += sizeof (m_wGrpIDInitHairType);
	dwRet += sizeof (m_wGrpIDInitHairColor);
	dwRet += sizeof (m_wGrpIDInitSP);
	dwRet += sizeof (m_wLevel);					/* レベル */
	dwRet += sizeof (m_wStamina);				/* スタミナ */
	dwRet += sizeof (m_wPower);					/* 腕力 */
	dwRet += sizeof (m_wStrength);				/* 体力 */
	dwRet += sizeof (m_wMagic);					/* 魔力 */
	dwRet += sizeof (m_wSkillful);				/* 器用 */
	dwRet += sizeof (m_wAbillityAT);			/* 攻撃技能 */
	dwRet += sizeof (m_wAbillityDF);			/* 防御技能 */
	dwRet += sizeof (m_wPAtack);				/* 攻撃力 */
	dwRet += sizeof (m_wPDefense);				/* 防御力 */
	dwRet += sizeof (m_wPMagic);				/* 魔法力 */
	dwRet += sizeof (m_wPMagicDefense);			/* 魔法防御力 */
	dwRet += sizeof (m_wPHitAverage);			/* 命中率 */
	dwRet += sizeof (m_wPAvoidAverage);			/* 回避率 */
	dwRet += sizeof (m_wPCriticalAverage);		/* クリティカル率 */
	dwRet += sizeof (m_wAttrFire);				/* 属性[火] */
	dwRet += sizeof (m_wAttrWind);				/* 属性[風] */
	dwRet += sizeof (m_wAttrWater);				/* 属性[水] */
	dwRet += sizeof (m_wAttrEarth);				/* 属性[土] */
	dwRet += sizeof (m_wAttrLight);				/* 属性[光] */
	dwRet += sizeof (m_wAttrDark);				/* 属性[闇] */
	dwRet += sizeof (m_dwCharID);
	dwRet += sizeof (m_dwMapID);
	dwRet += sizeof (m_dwMotionTypeID);
	dwRet += sizeof (m_dwEquipItemIDCloth);
	dwRet += sizeof (m_dwEquipItemIDAcce1);
	dwRet += sizeof (m_dwEquipItemIDAcce2);
	dwRet += sizeof (m_dwEquipItemIDArmsRight);
	dwRet += sizeof (m_dwEquipItemIDArmsLeft);
	dwRet += sizeof (m_dwEquipItemIDHead);
	dwRet += sizeof (m_dwMoveWait);				/* 移動待ち時間 */
	dwRet += sizeof (m_dwMoveWaitBattle);		/* 戦闘時移動待ち時間 */
	dwRet += sizeof (m_dwExp);					/* 経験値 */
	dwRet += sizeof (m_dwHP);
	dwRet += sizeof (m_dwMaxHP);
	dwRet += sizeof (m_dwSP);
	dwRet += sizeof (m_dwMaxSP);
	dwRet += sizeof (m_clName);
	dwRet += sizeof (m_clSpeak);
	dwRet += (m_strCharName.GetLength () + 1);
	dwRet += (m_strSpeak.GetLength () + 1);
	dwRet += (m_strTalk.GetLength () + 1);
	dwRet += ((m_adwItemID.GetSize () + 1) * sizeof (DWORD));
	dwRet += ((m_adwSkillID.GetSize () + 1) * sizeof (DWORD));
	/* NPC発生 */
	dwRet += sizeof (m_dwPutCycle);				/* 発生周期 */
	dwRet += sizeof (m_nPutMoveType);			/* 発生させる移動種別 */
	dwRet += sizeof (m_nMaxPutCount);			/* 同時発生数 */
	dwRet += sizeof (m_nPutAverage);			/* 発生確率 */
	dwRet += sizeof (m_ptPutArea);				/* 発生範囲(半径) */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoCharBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_nMapX);					break;
	case 1:		dwRet = sizeof (m_nMapY);					break;
	case 2:		dwRet = sizeof (m_nMoveState);				break;
	case 3:		dwRet = sizeof (m_nMoveType);				break;
	case 4:		dwRet = sizeof (m_nDirection);				break;
	case 5:		dwRet = sizeof (m_nGrpSize);				break;
	case 6:		dwRet = sizeof (m_nSex);					break;
	case 7:		dwRet = sizeof (m_nMaxItemCount);			break;
	case 8:		dwRet = sizeof (m_nDropItemAverage);		break;		/* アイテムドロップ率 */
	case 9:		dwRet = sizeof (m_nMoveAverage);			break;		/* 移動確率 */
	case 10:	dwRet = sizeof (m_nMoveAverageBattle);		break;		/* 戦闘時移動確率 */
	case 11:	dwRet = sizeof (m_wFamilyID);				break;
	case 12:	dwRet = sizeof (m_wGrpIDNPC);				break;
	case 13:	dwRet = sizeof (m_wGrpIDCloth);				break;
	case 14:	dwRet = sizeof (m_wGrpIDEye);				break;
	case 15:	dwRet = sizeof (m_wGrpIDEyeColor);			break;
	case 16:	dwRet = sizeof (m_wGrpIDHairType);			break;
	case 17:	dwRet = sizeof (m_wGrpIDHairColor);			break;
	case 18:	dwRet = sizeof (m_wGrpIDSP);				break;
	case 19:	dwRet = sizeof (m_wGrpIDTmpMain);			break;		/* 画像ID(一時服:メイン) */
	case 20:	dwRet = sizeof (m_wGrpIDTmpSub);			break;		/* 画像ID(一時服:サブ) */
	case 21:	dwRet = sizeof (m_wGrpIDAcce);				break;
	case 22:	dwRet = sizeof (m_wGrpIDArmsMain);			break;
	case 23:	dwRet = sizeof (m_wGrpIDArmsSub);			break;
	case 24:	dwRet = sizeof (m_wGrpIDArmsLeftMain);		break;
	case 25:	dwRet = sizeof (m_wGrpIDArmsLeftSub);		break;
	case 26:	dwRet = sizeof (m_dwCharID);				break;
	case 27:	dwRet = sizeof (m_dwMapID);					break;
	case 28:	dwRet = sizeof (m_dwMotionTypeID);			break;
	case 29:	dwRet = sizeof (m_clName);					break;
	case 30:	dwRet = sizeof (m_clSpeak);					break;
	case 31:	dwRet = (m_strCharName.	GetLength () + 1);	break;
	case 32:	dwRet = (m_strSpeak.	GetLength () + 1);	break;
	case 33:	dwRet = (m_strTalk.		GetLength () + 1);	break;
	case 34:	dwRet = ((m_adwItemID.GetSize () + 1) * sizeof (DWORD));	break;
	case 35:	dwRet = ((m_adwSkillID.GetSize () + 1) * sizeof (DWORD));	break;
	case 36:	dwRet = sizeof (m_bBlock);					break;
	case 37:	dwRet = sizeof (m_bPush);					break;
	case 38:	dwRet = sizeof (m_dwEquipItemIDCloth);		break;
	case 39:	dwRet = sizeof (m_dwEquipItemIDAcce1);		break;
	case 40:	dwRet = sizeof (m_dwEquipItemIDAcce2);		break;
	case 41:	dwRet = sizeof (m_dwEquipItemIDArmsRight);	break;
	case 42:	dwRet = sizeof (m_dwEquipItemIDArmsLeft);	break;
	case 43:	dwRet = sizeof (m_dwEquipItemIDHead);		break;
	case 44:	dwRet = sizeof (m_wGrpIDInitNPC);			break;
	case 45:	dwRet = sizeof (m_wGrpIDInitCloth);			break;
	case 46:	dwRet = sizeof (m_wGrpIDInitEye);			break;
	case 47:	dwRet = sizeof (m_wGrpIDInitEyeColor);		break;
	case 48:	dwRet = sizeof (m_wGrpIDInitHairType);		break;
	case 49:	dwRet = sizeof (m_wGrpIDInitHairColor);		break;
	case 50:	dwRet = sizeof (m_wGrpIDInitSP);			break;
	case 51:	dwRet = sizeof (m_wLevel);					break;	/* レベル */
	case 52:	dwRet = sizeof (m_wStamina);				break;	/* スタミナ */
	case 53:	dwRet = sizeof (m_wPower);					break;	/* 腕力 */
	case 54:	dwRet = sizeof (m_wStrength);				break;	/* 体力 */
	case 55:	dwRet = sizeof (m_wMagic);					break;	/* 魔力 */
	case 56:	dwRet = sizeof (m_wSkillful);				break;	/* 器用 */
	case 57:	dwRet = sizeof (m_wAbillityAT);				break;	/* 攻撃技能 */
	case 58:	dwRet = sizeof (m_wAbillityDF);				break;	/* 防御技能 */
	case 59:	dwRet = sizeof (m_wPAtack);					break;	/* 攻撃力 */
	case 60:	dwRet = sizeof (m_wPDefense);				break;	/* 防御力 */
	case 61:	dwRet = sizeof (m_wPMagic);					break;	/* 魔法力 */
	case 62:	dwRet = sizeof (m_wPMagicDefense);			break;	/* 魔法防御力 */
	case 63:	dwRet = sizeof (m_wPHitAverage);			break;	/* 命中率 */
	case 64:	dwRet = sizeof (m_wPAvoidAverage);			break;	/* 回避率 */
	case 65:	dwRet = sizeof (m_wPCriticalAverage);		break;	/* クリティカル率 */
	case 66:	dwRet = sizeof (m_wAttrFire);				break;	/* 属性[火] */
	case 67:	dwRet = sizeof (m_wAttrWind);				break;	/* 属性[風] */
	case 68:	dwRet = sizeof (m_wAttrWater);				break;	/* 属性[水] */
	case 69:	dwRet = sizeof (m_wAttrEarth);				break;	/* 属性[土] */
	case 70:	dwRet = sizeof (m_wAttrLight);				break;	/* 属性[光] */
	case 71:	dwRet = sizeof (m_wAttrDark);				break;	/* 属性[闇] */
	case 72:	dwRet = sizeof (m_dwMoveWait);				break;	/* 移動待ち時間 */
	case 73:	dwRet = sizeof (m_dwMoveWaitBattle);		break;	/* 戦闘時移動待ち時間 */
	case 74:	dwRet = sizeof (m_dwExp);					break;	/* 経験値 */
	case 75:	dwRet = sizeof (m_dwHP);					break;
	case 76:	dwRet = sizeof (m_dwMaxHP);					break;
	case 77:	dwRet = sizeof (m_dwSP);					break;
	case 78:	dwRet = sizeof (m_dwMaxSP);					break;
	/* NPC発生 */
	case 79:	dwRet = sizeof (m_dwPutCycle);				break;	/* 発生周期 */
	case 80:	dwRet = sizeof (m_nPutMoveType);			break;	/* 発生させる移動種別 */
	case 81:	dwRet = sizeof (m_nMaxPutCount);			break;	/* 同時発生数 */
	case 82:	dwRet = sizeof (m_nPutAverage);				break;	/* 発生確率 */
	case 83:	dwRet = sizeof (m_ptPutArea);				break;	/* 発生範囲(半径) */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetName											 */
/* 内容		:要素名を取得													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

LPCSTR CInfoCharBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetWriteData									 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

PBYTE CInfoCharBase::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pSrc, pTmp;
	DWORD dwSize, dwTmp;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_nMapX;					break;
	case 1:		pSrc = (PBYTE)&m_nMapY;					break;
	case 2:		pSrc = (PBYTE)&m_nMoveState;			break;
	case 3:		pSrc = (PBYTE)&m_nMoveType;				break;
	case 4:		pSrc = (PBYTE)&m_nDirection;			break;
	case 5:		pSrc = (PBYTE)&m_nGrpSize;				break;
	case 6:		pSrc = (PBYTE)&m_nSex;					break;
	case 7:		pSrc = (PBYTE)&m_nMaxItemCount;			break;
	case 8:		pSrc = (PBYTE)&m_nDropItemAverage;		break;		/* アイテムドロップ率 */
	case 9:		pSrc = (PBYTE)&m_nMoveAverage;			break;		/* 移動確率 */
	case 10:	pSrc = (PBYTE)&m_nMoveAverageBattle;	break;		/* 先頭時移動確率 */
	case 11:	pSrc = (PBYTE)&m_wFamilyID;				break;
	case 12:	pSrc = (PBYTE)&m_wGrpIDNPC;				break;
	case 13:	pSrc = (PBYTE)&m_wGrpIDCloth;			break;
	case 14:	pSrc = (PBYTE)&m_wGrpIDEye;				break;
	case 15:	pSrc = (PBYTE)&m_wGrpIDEyeColor;		break;
	case 16:	pSrc = (PBYTE)&m_wGrpIDHairType;		break;
	case 17:	pSrc = (PBYTE)&m_wGrpIDHairColor;		break;
	case 18:	pSrc = (PBYTE)&m_wGrpIDSP;				break;
	case 19:	pSrc = (PBYTE)&m_wGrpIDTmpMain;			break;		/* 画像ID(一時服:メイン) */
	case 20:	pSrc = (PBYTE)&m_wGrpIDTmpSub;			break;		/* 画像ID(一時服:サブ) */
	case 21:	pSrc = (PBYTE)&m_wGrpIDAcce;			break;
	case 22:	pSrc = (PBYTE)&m_wGrpIDArmsMain;		break;
	case 23:	pSrc = (PBYTE)&m_wGrpIDArmsSub;			break;
	case 24:	pSrc = (PBYTE)&m_wGrpIDArmsLeftMain;	break;
	case 25:	pSrc = (PBYTE)&m_wGrpIDArmsLeftSub;		break;
	case 26:	pSrc = (PBYTE)&m_dwCharID;				break;
	case 27:	pSrc = (PBYTE)&m_dwMapID;				break;
	case 28:	pSrc = (PBYTE)&m_dwMotionTypeID;		break;
	case 29:	pSrc = (PBYTE)&m_clName;				break;
	case 30:	pSrc = (PBYTE)&m_clSpeak;				break;
	case 31:	pSrc = (PBYTE)(LPCSTR)m_strCharName;	break;
	case 32:	pSrc = (PBYTE)(LPCSTR)m_strSpeak;		break;
	case 33:	pSrc = (PBYTE)(LPCSTR)m_strTalk;		break;
	case 34:
		pTmp	= pRet;
		nCount	= m_adwItemID.GetSize ();

		for (i = 0; i < nCount; i ++) {
			dwTmp = m_adwItemID[i];
			CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		}
		dwTmp = 0;
		CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		break;
	case 35:
		pTmp	= pRet;
		nCount	= m_adwSkillID.GetSize ();

		for (i = 0; i < nCount; i ++) {
			dwTmp = m_adwSkillID[i];
			CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		}
		dwTmp = 0;
		CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		break;
	case 36:	pSrc = (PBYTE)&m_bBlock;					break;
	case 37:	pSrc = (PBYTE)&m_bPush;						break;
	case 38:	pSrc = (PBYTE)&m_dwEquipItemIDCloth;		break;
	case 39:	pSrc = (PBYTE)&m_dwEquipItemIDAcce1;		break;
	case 40:	pSrc = (PBYTE)&m_dwEquipItemIDAcce2;		break;
	case 41:	pSrc = (PBYTE)&m_dwEquipItemIDArmsRight;	break;
	case 42:	pSrc = (PBYTE)&m_dwEquipItemIDArmsLeft;		break;
	case 43:	pSrc = (PBYTE)&m_dwEquipItemIDHead;			break;
	case 44:	pSrc = (PBYTE)&m_wGrpIDInitNPC;				break;
	case 45:	pSrc = (PBYTE)&m_wGrpIDInitCloth;			break;
	case 46:	pSrc = (PBYTE)&m_wGrpIDInitEye;				break;
	case 47:	pSrc = (PBYTE)&m_wGrpIDInitEyeColor;		break;
	case 48:	pSrc = (PBYTE)&m_wGrpIDInitHairType;		break;
	case 49:	pSrc = (PBYTE)&m_wGrpIDInitHairColor;		break;
	case 50:	pSrc = (PBYTE)&m_wGrpIDInitSP;				break;
	case 51:	pSrc = (PBYTE)&m_wLevel;					break;	/* レベル */
	case 52:	pSrc = (PBYTE)&m_wStamina;					break;	/* スタミナ */
	case 53:	pSrc = (PBYTE)&m_wPower;					break;	/* 腕力 */
	case 54:	pSrc = (PBYTE)&m_wStrength;					break;	/* 体力 */
	case 55:	pSrc = (PBYTE)&m_wMagic;					break;	/* 魔力 */
	case 56:	pSrc = (PBYTE)&m_wSkillful;					break;	/* 器用 */
	case 57:	pSrc = (PBYTE)&m_wAbillityAT;				break;	/* 攻撃技能 */
	case 58:	pSrc = (PBYTE)&m_wAbillityDF;				break;	/* 防御技能 */
	case 59:	pSrc = (PBYTE)&m_wPAtack;					break;	/* 攻撃力 */
	case 60:	pSrc = (PBYTE)&m_wPDefense;					break;	/* 防御力 */
	case 61:	pSrc = (PBYTE)&m_wPMagic;					break;	/* 魔法力 */
	case 62:	pSrc = (PBYTE)&m_wPMagicDefense;			break;	/* 魔法防御力 */
	case 63:	pSrc = (PBYTE)&m_wPHitAverage;				break;	/* 命中率 */
	case 64:	pSrc = (PBYTE)&m_wPAvoidAverage;			break;	/* 回避率 */
	case 65:	pSrc = (PBYTE)&m_wPCriticalAverage;			break;	/* クリティカル率 */
	case 66:	pSrc = (PBYTE)&m_wAttrFire;					break;	/* 属性[火] */
	case 67:	pSrc = (PBYTE)&m_wAttrWind;					break;	/* 属性[風] */
	case 68:	pSrc = (PBYTE)&m_wAttrWater;				break;	/* 属性[水] */
	case 69:	pSrc = (PBYTE)&m_wAttrEarth;				break;	/* 属性[土] */
	case 70:	pSrc = (PBYTE)&m_wAttrLight;				break;	/* 属性[光] */
	case 71:	pSrc = (PBYTE)&m_wAttrDark;					break;	/* 属性[闇] */
	case 72:	pSrc = (PBYTE)&m_dwMoveWait;				break;	/* 移動待ち時間 */
	case 73:	pSrc = (PBYTE)&m_dwMoveWaitBattle;			break;	/* 先頭時移動待ち時間 */
	case 74:	pSrc = (PBYTE)&m_dwExp;						break;	/* 経験値 */
	case 75:	pSrc = (PBYTE)&m_dwHP;						break;
	case 76:	pSrc = (PBYTE)&m_dwMaxHP;					break;
	case 77:	pSrc = (PBYTE)&m_dwSP;						break;
	case 78:	pSrc = (PBYTE)&m_dwMaxSP;					break;
	/* NPC発生 */
	case 79:	pSrc = (PBYTE)&m_dwPutCycle;				break;	/* 発生周期 */
	case 80:	pSrc = (PBYTE)&m_nPutMoveType;				break;	/* 発生させる移動種別 */
	case 81:	pSrc = (PBYTE)&m_nMaxPutCount;				break;	/* 同時発生数 */
	case 82:	pSrc = (PBYTE)&m_nPutAverage;				break;	/* 発生確率 */
	case 83:	pSrc = (PBYTE)&m_ptPutArea;					break;	/* 発生範囲(半径) */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::ReadElementData									 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoCharBase::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst, pTmp;
	DWORD dwSize, dwTmp;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_nMapX;					dwSize = sizeof (m_nMapX);				break;
	case 1:		pDst = (PBYTE)&m_nMapY;					dwSize = sizeof (m_nMapY);				break;
	case 2:												dwSize = sizeof (m_nMoveState);			break;
	case 3:		pDst = (PBYTE)&m_nMoveType;				dwSize = sizeof (m_nMoveType);			break;
	case 4:		pDst = (PBYTE)&m_nDirection;			dwSize = sizeof (m_nDirection);			break;
	case 5:		pDst = (PBYTE)&m_nGrpSize;				dwSize = sizeof (m_nGrpSize);			break;
	case 6:		pDst = (PBYTE)&m_nSex;					dwSize = sizeof (m_nSex);				break;
	case 7:		pDst = (PBYTE)&m_nMaxItemCount;			dwSize = sizeof (m_nMaxItemCount);		break;
	case 8:		pDst = (PBYTE)&m_nDropItemAverage;		dwSize = sizeof (m_nDropItemAverage);	break;	/* アイテムドロップ率 */
	case 9:		pDst = (PBYTE)&m_nMoveAverage;			dwSize = sizeof (m_nMoveAverage);		break;	/* 移動確率 */
	case 10:	pDst = (PBYTE)&m_nMoveAverageBattle;	dwSize = sizeof (m_nMoveAverageBattle);	break;	/* 戦闘時移動確率 */
	case 11:	pDst = (PBYTE)&m_wFamilyID;				dwSize = sizeof (m_wFamilyID);			break;
	case 12:	pDst = (PBYTE)&m_wGrpIDNPC;				dwSize = sizeof (m_wGrpIDNPC);			break;
	case 13:	pDst = (PBYTE)&m_wGrpIDCloth;			dwSize = sizeof (m_wGrpIDCloth);		break;
	case 14:	pDst = (PBYTE)&m_wGrpIDEye;				dwSize = sizeof (m_wGrpIDEye);			break;
	case 15:	pDst = (PBYTE)&m_wGrpIDEyeColor;		dwSize = sizeof (m_wGrpIDEyeColor);		break;
	case 16:	pDst = (PBYTE)&m_wGrpIDHairType;		dwSize = sizeof (m_wGrpIDHairType);		break;
	case 17:	pDst = (PBYTE)&m_wGrpIDHairColor;		dwSize = sizeof (m_wGrpIDHairColor);	break;
	case 18:	pDst = (PBYTE)&m_wGrpIDSP;				dwSize = sizeof (m_wGrpIDSP);			break;
	case 19:	pDst = (PBYTE)&m_wGrpIDTmpMain;			dwSize = sizeof (m_wGrpIDTmpMain);		break;	/* 画像ID(一時服:メイン) */
	case 20:	pDst = (PBYTE)&m_wGrpIDTmpSub;			dwSize = sizeof (m_wGrpIDTmpSub);		break;	/* 画像ID(一時服:サブ) */
	case 21:	pDst = (PBYTE)&m_wGrpIDAcce;			dwSize = sizeof (m_wGrpIDAcce);			break;
	case 22:	pDst = (PBYTE)&m_wGrpIDArmsMain;		dwSize = sizeof (m_wGrpIDArmsMain);		break;
	case 23:	pDst = (PBYTE)&m_wGrpIDArmsSub;			dwSize = sizeof (m_wGrpIDArmsSub);		break;
	case 24:	pDst = (PBYTE)&m_wGrpIDArmsLeftMain;	dwSize = sizeof (m_wGrpIDArmsLeftMain);	break;
	case 25:	pDst = (PBYTE)&m_wGrpIDArmsLeftSub;		dwSize = sizeof (m_wGrpIDArmsLeftSub);	break;
	case 26:	pDst = (PBYTE)&m_dwCharID;				dwSize = sizeof (m_dwCharID);			break;
	case 27:	pDst = (PBYTE)&m_dwMapID;				dwSize = sizeof (m_dwMapID);			break;
	case 28:	pDst = (PBYTE)&m_dwMotionTypeID;		dwSize = sizeof (m_dwMotionTypeID);		break;
	case 29:	pDst = (PBYTE)&m_clName;				dwSize = sizeof (m_clName);				break;
	case 30:	pDst = (PBYTE)&m_clSpeak;				dwSize = sizeof (m_clSpeak);			break;
	case 31:
		m_strCharName = (LPCSTR)pSrc;
		dwSize = m_strCharName.GetLength () + 1;
		break;
	case 32:
		m_strSpeak = (LPCSTR)pSrc;
		dwSize = m_strSpeak.GetLength () + 1;
		break;
	case 33:
		m_strTalk = (LPCSTR)pSrc;
		dwSize = m_strTalk.GetLength () + 1;
		break;
	case 34:
		pTmp	= pSrc;
		dwTmp	= 0;

		while (1) {
			CopyMemoryRenew (&dwTmp, pTmp, sizeof (DWORD), pTmp);
			if (dwTmp == 0) {
				break;
			}
			m_adwItemID.Add (dwTmp);
		}
		dwSize = (m_adwItemID.GetSize () + 1) * sizeof (DWORD);
		break;
	case 35:
		pTmp	= pSrc;
		dwTmp	= 0;

		while (1) {
			CopyMemoryRenew (&dwTmp, pTmp, sizeof (DWORD), pTmp);
			if (dwTmp == 0) {
				break;
			}
			m_adwSkillID.Add (dwTmp);
		}
		dwSize = (m_adwSkillID.GetSize () + 1) * sizeof (DWORD);
		break;
	case 36:	pDst = (PBYTE)&m_bBlock;					dwSize = sizeof (m_bBlock);					break;
	case 37:	pDst = (PBYTE)&m_bPush;						dwSize = sizeof (m_bPush);					break;
	case 38:	pDst = (PBYTE)&m_dwEquipItemIDCloth;		dwSize = sizeof (m_dwEquipItemIDCloth);		break;
	case 39:	pDst = (PBYTE)&m_dwEquipItemIDAcce1;		dwSize = sizeof (m_dwEquipItemIDAcce1);		break;
	case 40:	pDst = (PBYTE)&m_dwEquipItemIDAcce2;		dwSize = sizeof (m_dwEquipItemIDAcce2);		break;
	case 41:	pDst = (PBYTE)&m_dwEquipItemIDArmsRight;	dwSize = sizeof (m_dwEquipItemIDArmsRight);	break;
	case 42:	pDst = (PBYTE)&m_dwEquipItemIDArmsLeft;		dwSize = sizeof (m_dwEquipItemIDArmsLeft);	break;
	case 43:	pDst = (PBYTE)&m_dwEquipItemIDHead;			dwSize = sizeof (m_dwEquipItemIDHead);		break;
	case 44:	pDst = (PBYTE)&m_wGrpIDInitNPC;				dwSize = sizeof (m_wGrpIDInitNPC);			break;
	case 45:	pDst = (PBYTE)&m_wGrpIDInitCloth;			dwSize = sizeof (m_wGrpIDInitCloth);		break;
	case 46:	pDst = (PBYTE)&m_wGrpIDInitEye;				dwSize = sizeof (m_wGrpIDInitEye);			break;
	case 47:	pDst = (PBYTE)&m_wGrpIDInitEyeColor;		dwSize = sizeof (m_wGrpIDInitEyeColor);		break;
	case 48:	pDst = (PBYTE)&m_wGrpIDInitHairType;		dwSize = sizeof (m_wGrpIDInitHairType);		break;
	case 49:	pDst = (PBYTE)&m_wGrpIDInitHairColor;		dwSize = sizeof (m_wGrpIDInitHairColor);	break;
	case 50:	pDst = (PBYTE)&m_wGrpIDInitSP;				dwSize = sizeof (m_wGrpIDInitSP);			break;
	case 51:	pDst = (PBYTE)&m_wLevel;					dwSize = sizeof (m_wLevel);					break;	/* レベル */
	case 52:	pDst = (PBYTE)&m_wStamina;					dwSize = sizeof (m_wStamina);				break;	/* スタミナ */
	case 53:	pDst = (PBYTE)&m_wPower;					dwSize = sizeof (m_wPower);					break;	/* 腕力 */
	case 54:	pDst = (PBYTE)&m_wStrength;					dwSize = sizeof (m_wStrength);				break;	/* 体力 */
	case 55:	pDst = (PBYTE)&m_wMagic;					dwSize = sizeof (m_wMagic);					break;	/* 魔力 */
	case 56:	pDst = (PBYTE)&m_wSkillful;					dwSize = sizeof (m_wSkillful);				break;	/* 器用 */
	case 57:	pDst = (PBYTE)&m_wAbillityAT;				dwSize = sizeof (m_wAbillityAT);			break;	/* 攻撃技能 */
	case 58:	pDst = (PBYTE)&m_wAbillityDF;				dwSize = sizeof (m_wAbillityDF);			break;	/* 防御技能 */
	case 59:	pDst = (PBYTE)&m_wPAtack;					dwSize = sizeof (m_wPAtack);				break;	/* 攻撃力 */
	case 60:	pDst = (PBYTE)&m_wPDefense;					dwSize = sizeof (m_wPDefense);				break;	/* 防御力 */
	case 61:	pDst = (PBYTE)&m_wPMagic;					dwSize = sizeof (m_wPMagic);				break;	/* 魔法力 */
	case 62:	pDst = (PBYTE)&m_wPMagicDefense;			dwSize = sizeof (m_wPMagicDefense);			break;	/* 魔法防御力 */
	case 63:	pDst = (PBYTE)&m_wPHitAverage;				dwSize = sizeof (m_wPHitAverage);			break;	/* 命中率 */
	case 64:	pDst = (PBYTE)&m_wPAvoidAverage;			dwSize = sizeof (m_wPAvoidAverage);			break;	/* 回避率 */
	case 65:	pDst = (PBYTE)&m_wPCriticalAverage;			dwSize = sizeof (m_wPCriticalAverage);		break;	/* クリティカル率 */
	case 66:	pDst = (PBYTE)&m_wAttrFire;					dwSize = sizeof (m_wAttrFire);				break;	/* 属性[火] */
	case 67:	pDst = (PBYTE)&m_wAttrWind;					dwSize = sizeof (m_wAttrWind);				break;	/* 属性[風] */
	case 68:	pDst = (PBYTE)&m_wAttrWater;				dwSize = sizeof (m_wAttrWater);				break;	/* 属性[水] */
	case 69:	pDst = (PBYTE)&m_wAttrEarth;				dwSize = sizeof (m_wAttrEarth);				break;	/* 属性[土] */
	case 70:	pDst = (PBYTE)&m_wAttrLight;				dwSize = sizeof (m_wAttrLight);				break;	/* 属性[光] */
	case 71:	pDst = (PBYTE)&m_wAttrDark;					dwSize = sizeof (m_wAttrDark);				break;	/* 属性[闇] */
	case 72:	pDst = (PBYTE)&m_dwMoveWait;				dwSize = sizeof (m_dwMoveWait);				break;	/* 移動待ち時間 */
	case 73:	pDst = (PBYTE)&m_dwMoveWaitBattle;			dwSize = sizeof (m_dwMoveWaitBattle);		break;	/* 戦闘時移動待ち時間 */
	case 74:	pDst = (PBYTE)&m_dwExp;						dwSize = sizeof (m_dwExp);					break;	/* 経験値 */
	case 75:	pDst = (PBYTE)&m_dwHP;						dwSize = sizeof (m_dwHP);					break;
	case 76:	pDst = (PBYTE)&m_dwMaxHP;					dwSize = sizeof (m_dwMaxHP);				break;
	case 77:	pDst = (PBYTE)&m_dwSP;						dwSize = sizeof (m_dwSP);					break;
	case 78:	pDst = (PBYTE)&m_dwMaxSP;					dwSize = sizeof (m_dwMaxSP);				break;
	/* NPC発生 */
	case 79:	pDst = (PBYTE)&m_dwPutCycle;				dwSize = sizeof (m_dwPutCycle);				break;	/* 発生周期 */
	case 80:	pDst = (PBYTE)&m_nPutMoveType;				dwSize = sizeof (m_nPutMoveType);			break;	/* 発生させる移動種別 */
	case 81:	pDst = (PBYTE)&m_nMaxPutCount;				dwSize = sizeof (m_nMaxPutCount);			break;	/* 同時発生数 */
	case 82:	pDst = (PBYTE)&m_nPutAverage;				dwSize = sizeof (m_nPutAverage);			break;	/* 発生確率 */
	case 83:	pDst = (PBYTE)&m_ptPutArea;					dwSize = sizeof (m_ptPutArea);				break;	/* 発生範囲(半径) */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
//		if (nNo == 5) m_nGrpSize /= 2;
//		if (nNo == 7) m_nMaxItemCount = 25;
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetSendDataSize									 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2006/11/06														 */
/* ========================================================================= */

DWORD CInfoCharBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwCharID);
	dwRet += sizeof (m_dwAccountID);
	dwRet += sizeof (m_dwTailCharID);
	dwRet += sizeof (m_dwFrontCharID);
	dwRet += sizeof (m_dwParentCharID);
	dwRet += sizeof (m_dwMapID);
	dwRet += sizeof (m_dwMotionTypeID);
	dwRet += sizeof (m_bBlock);
	dwRet += sizeof (m_bPush);
	dwRet += sizeof (m_nAnime);
	dwRet += sizeof (m_nMapX);
	dwRet += sizeof (m_nMapY);
	dwRet += sizeof (m_nMoveState);
	dwRet += sizeof (m_nMoveType);
	dwRet += sizeof (m_nDirection);
	dwRet += sizeof (m_nSex);
	dwRet += sizeof (m_nMaxItemCount);
	dwRet += sizeof (m_nDropItemAverage);		/* アイテムドロップ率 */
	dwRet += sizeof (m_nMoveAverage);			/* 移動確率 */
	dwRet += sizeof (m_nMoveAverageBattle);		/* 戦闘時移動確率 */
	dwRet += sizeof (m_wFamilyID);
	dwRet += sizeof (m_wGrpIDNPC);
	dwRet += sizeof (m_wGrpIDCloth);
	dwRet += sizeof (m_wGrpIDEye);
	dwRet += sizeof (m_wGrpIDEyeColor);
	dwRet += sizeof (m_wGrpIDHairType);
	dwRet += sizeof (m_wGrpIDHairColor);
	dwRet += sizeof (m_wGrpIDSP);
	dwRet += sizeof (m_wGrpIDTmpMain);			/* 画像ID(一時服:メイン) */
	dwRet += sizeof (m_wGrpIDTmpSub);			/* 画像ID(一時服:サブ) */
	dwRet += sizeof (m_wGrpIDAcce);
	dwRet += sizeof (m_wGrpIDArmsMain);
	dwRet += sizeof (m_wGrpIDArmsSub);
	dwRet += sizeof (m_wGrpIDArmsLeftMain);
	dwRet += sizeof (m_wGrpIDArmsLeftSub);
	dwRet += sizeof (m_wGrpIDInitNPC);
	dwRet += sizeof (m_wGrpIDInitCloth);
	dwRet += sizeof (m_wGrpIDInitEye);
	dwRet += sizeof (m_wGrpIDInitEyeColor);
	dwRet += sizeof (m_wGrpIDInitHairType);
	dwRet += sizeof (m_wGrpIDInitHairColor);
	dwRet += sizeof (m_wGrpIDInitSP);
	dwRet += sizeof (m_wAtackGauge);			/* アタックゲージ */
	dwRet += sizeof (m_wDefenseGauge);			/* ディフェンスゲージ */
	dwRet += sizeof (m_wLevel);					/* レベル */
	dwRet += sizeof (m_wStamina);				/* スタミナ */
	dwRet += sizeof (m_wPower);					/* 腕力 */
	dwRet += sizeof (m_wStrength);				/* 体力 */
	dwRet += sizeof (m_wMagic);					/* 魔力 */
	dwRet += sizeof (m_wSkillful);				/* 器用 */
	dwRet += sizeof (m_wAbillityAT);			/* 攻撃技能 */
	dwRet += sizeof (m_wAbillityDF);			/* 防御技能 */
	dwRet += sizeof (m_wPAtack);				/* 攻撃力 */
	dwRet += sizeof (m_wPDefense);				/* 防御力 */
	dwRet += sizeof (m_wPMagic);				/* 魔法力 */
	dwRet += sizeof (m_wPMagicDefense);			/* 魔法防御力 */
	dwRet += sizeof (m_wPHitAverage);			/* 命中率 */
	dwRet += sizeof (m_wPAvoidAverage);			/* 回避率 */
	dwRet += sizeof (m_wPCriticalAverage);		/* クリティカル率 */
	dwRet += sizeof (m_wAttrFire);				/* 属性[火] */
	dwRet += sizeof (m_wAttrWind);				/* 属性[風] */
	dwRet += sizeof (m_wAttrWater);				/* 属性[水] */
	dwRet += sizeof (m_wAttrEarth);				/* 属性[土] */
	dwRet += sizeof (m_wAttrLight);				/* 属性[光] */
	dwRet += sizeof (m_wAttrDark);				/* 属性[闇] */
	dwRet += sizeof (m_dwEquipItemIDCloth);
	dwRet += sizeof (m_dwEquipItemIDAcce1);
	dwRet += sizeof (m_dwEquipItemIDAcce2);
	dwRet += sizeof (m_dwEquipItemIDArmsRight);
	dwRet += sizeof (m_dwEquipItemIDArmsLeft);
	dwRet += sizeof (m_dwEquipItemIDHead);
	dwRet += sizeof (m_dwMoveWait);				/* 移動待ち時間 */
	dwRet += sizeof (m_dwMoveWaitBattle);		/* 戦闘時移動待ち時間 */
	dwRet += sizeof (m_dwExp);					/* 経験値 */
	dwRet += sizeof (m_dwHP);
	dwRet += sizeof (m_dwMaxHP);
	dwRet += sizeof (m_dwSP);
	dwRet += sizeof (m_dwMaxSP);
	dwRet += sizeof (m_clName);
	dwRet += sizeof (m_clSpeak);
	dwRet += (m_strCharName.GetLength () + 1);
	dwRet += (m_strSpeak.	GetLength () + 1);
	dwRet += (m_strTalk.	GetLength () + 1);
	dwRet += (m_abyMark.	GetSize () + 1);
	dwRet += ((m_adwItemID.	GetSize () + 1) * sizeof (DWORD));
	dwRet += ((m_adwSkillID.GetSize () + 1) * sizeof (DWORD));
	/* NPC発生 */
	dwRet += sizeof (m_dwPutCycle);				/* 発生周期 */
	dwRet += sizeof (m_nPutMoveType);			/* 発生させる移動種別 */
	dwRet += sizeof (m_nMaxPutCount);			/* 同時発生数 */
	dwRet += sizeof (m_nPutAverage);			/* 発生確率 */
	dwRet += sizeof (m_ptPutArea);				/* 発生範囲(半径) */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2006/11/06														 */
/* ========================================================================= */

PBYTE CInfoCharBase::GetSendData(void)
{
	BYTE byTmp;
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwSize, dwTmp;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	pDataTmp	= pData;

	CopyMemoryRenew (pDataTmp, &m_dwCharID,					sizeof (m_dwCharID),				pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &m_dwAccountID,				sizeof (m_dwAccountID),				pDataTmp);	/* アカウントID */
	CopyMemoryRenew (pDataTmp, &m_dwTailCharID,				sizeof (m_dwTailCharID),			pDataTmp);	/* 付いて来ているキャラID */
	CopyMemoryRenew (pDataTmp, &m_dwFrontCharID,			sizeof (m_dwFrontCharID),			pDataTmp);	/* 付いているキャラID */
	CopyMemoryRenew (pDataTmp, &m_dwParentCharID,			sizeof (m_dwParentCharID),			pDataTmp);	/* 親のキャラID */
	CopyMemoryRenew (pDataTmp, &m_bBlock,					sizeof (m_bBlock),					pDataTmp);	/* ぶつかる判定 */
	CopyMemoryRenew (pDataTmp, &m_bPush,					sizeof (m_bPush),					pDataTmp);	/* 押せる判定 */
	CopyMemoryRenew (pDataTmp, &m_dwMapID,					sizeof (m_dwMapID),					pDataTmp);	/* マップID */
	CopyMemoryRenew (pDataTmp, &m_dwMotionTypeID,			sizeof (m_dwMotionTypeID),			pDataTmp);	/* モーション種別ID */
	CopyMemoryRenew (pDataTmp, &m_nAnime,					sizeof (m_nAnime),					pDataTmp);	/* アニメーション番号 */
	CopyMemoryRenew (pDataTmp, &m_nMapX,					sizeof (m_nMapX),					pDataTmp);	/* マップ座標(横) */
	CopyMemoryRenew (pDataTmp, &m_nMapY,					sizeof (m_nMapY),					pDataTmp);	/* マップ座標(縦) */
	CopyMemoryRenew (pDataTmp, &m_nMoveState,				sizeof (m_nMoveState),				pDataTmp);	/* 移動状態 */
	CopyMemoryRenew (pDataTmp, &m_nMoveType,				sizeof (m_nMoveType),				pDataTmp);	/* 移動種別 */
	CopyMemoryRenew (pDataTmp, &m_nDirection,				sizeof (m_nDirection),				pDataTmp);	/* 向き */
	CopyMemoryRenew (pDataTmp, &m_nSex,						sizeof (m_nSex),					pDataTmp);	/* 性別 */
	CopyMemoryRenew (pDataTmp, &m_nMaxItemCount,			sizeof (m_nMaxItemCount),			pDataTmp);	/* 最大アイテム所持数 */
	CopyMemoryRenew (pDataTmp, &m_nDropItemAverage,			sizeof (m_nDropItemAverage),		pDataTmp);	/* アイテムドロップ率 */
	CopyMemoryRenew (pDataTmp, &m_nMoveAverage,				sizeof (m_nMoveAverage),			pDataTmp);	/* 移動確率 */
	CopyMemoryRenew (pDataTmp, &m_nMoveAverageBattle,		sizeof (m_nMoveAverageBattle),		pDataTmp);	/* 戦闘時移動確率 */
	CopyMemoryRenew (pDataTmp, &m_wFamilyID,				sizeof (m_wFamilyID),				pDataTmp);	/* 種族ID */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDNPC,				sizeof (m_wGrpIDNPC),				pDataTmp);	/* 画像ID(NPC) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDCloth,				sizeof (m_wGrpIDCloth),				pDataTmp);	/* 画像ID(服) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDEye,				sizeof (m_wGrpIDEye),				pDataTmp);	/* 画像ID(目) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDEyeColor,			sizeof (m_wGrpIDEyeColor),			pDataTmp);	/* 画像ID(目色) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDHairType,			sizeof (m_wGrpIDHairType),			pDataTmp);	/* 画像ID(髪) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDHairColor,			sizeof (m_wGrpIDHairColor),			pDataTmp);	/* 画像ID(髪色) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDSP,					sizeof (m_wGrpIDSP),				pDataTmp);	/* 画像ID(特殊服) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDTmpMain,			sizeof (m_wGrpIDTmpMain),			pDataTmp);	/* 画像ID(一時服:メイン) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDTmpSub,				sizeof (m_wGrpIDTmpSub),			pDataTmp);	/* 画像ID(一時服:サブ) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDAcce,				sizeof (m_wGrpIDAcce),				pDataTmp);	/* 画像ID(アクセサリ) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDArmsMain,			sizeof (m_wGrpIDArmsMain),			pDataTmp);	/* 画像ID(持ち物:メイン) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDArmsSub,			sizeof (m_wGrpIDArmsSub),			pDataTmp);	/* 画像ID(持ち物:サブ) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDArmsLeftMain,		sizeof (m_wGrpIDArmsLeftMain),		pDataTmp);	/* 画像ID(盾:メイン) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDArmsLeftSub,		sizeof (m_wGrpIDArmsLeftSub),		pDataTmp);	/* 画像ID(盾:サブ) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitNPC,			sizeof (m_wGrpIDInitNPC),			pDataTmp);	/* 初期画像ID(NPC) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitCloth,			sizeof (m_wGrpIDInitCloth),			pDataTmp);	/* 初期画像ID(服) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitEye,			sizeof (m_wGrpIDInitEye),			pDataTmp);	/* 初期画像ID(目) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitEyeColor,		sizeof (m_wGrpIDInitEyeColor),		pDataTmp);	/* 初期画像ID(目色) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitHairType,		sizeof (m_wGrpIDInitHairType),		pDataTmp);	/* 初期画像ID(髪) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitHairColor,		sizeof (m_wGrpIDInitHairColor),		pDataTmp);	/* 初期画像ID(髪色) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitSP,				sizeof (m_wGrpIDInitSP),			pDataTmp);	/* 初期画像ID(特殊服) */
	CopyMemoryRenew (pDataTmp, &m_wAtackGauge,				sizeof (m_wAtackGauge),				pDataTmp);	/* アタックゲージ */
	CopyMemoryRenew (pDataTmp, &m_wDefenseGauge,			sizeof (m_wDefenseGauge),			pDataTmp);	/* ディフェンスゲージ */
	CopyMemoryRenew (pDataTmp, &m_wLevel,					sizeof (m_wLevel),					pDataTmp);	/* レベル */
	CopyMemoryRenew (pDataTmp, &m_wStamina,					sizeof (m_wStamina),				pDataTmp);	/* スタミナ */
	CopyMemoryRenew (pDataTmp, &m_wPower,					sizeof (m_wPower),					pDataTmp);	/* 腕力 */
	CopyMemoryRenew (pDataTmp, &m_wStrength,				sizeof (m_wStrength),				pDataTmp);	/* 体力 */
	CopyMemoryRenew (pDataTmp, &m_wMagic,					sizeof (m_wMagic),					pDataTmp);	/* 魔力 */
	CopyMemoryRenew (pDataTmp, &m_wSkillful,				sizeof (m_wSkillful),				pDataTmp);	/* 器用 */
	CopyMemoryRenew (pDataTmp, &m_wAbillityAT,				sizeof (m_wAbillityAT),				pDataTmp);	/* 攻撃技能 */
	CopyMemoryRenew (pDataTmp, &m_wAbillityDF,				sizeof (m_wAbillityDF),				pDataTmp);	/* 防御技能 */
	CopyMemoryRenew (pDataTmp, &m_wPAtack,					sizeof (m_wPAtack),					pDataTmp);	/* 攻撃力 */
	CopyMemoryRenew (pDataTmp, &m_wPDefense,				sizeof (m_wPDefense),				pDataTmp);	/* 防御力 */
	CopyMemoryRenew (pDataTmp, &m_wPMagic,					sizeof (m_wPMagic),					pDataTmp);	/* 魔法力 */
	CopyMemoryRenew (pDataTmp, &m_wPMagicDefense,			sizeof (m_wPMagicDefense),			pDataTmp);	/* 魔法防御力 */
	CopyMemoryRenew (pDataTmp, &m_wPHitAverage,				sizeof (m_wPHitAverage),			pDataTmp);	/* 命中率 */
	CopyMemoryRenew (pDataTmp, &m_wPAvoidAverage,			sizeof (m_wPAvoidAverage),			pDataTmp);	/* 回避率 */
	CopyMemoryRenew (pDataTmp, &m_wPCriticalAverage,		sizeof (m_wPCriticalAverage),		pDataTmp);	/* クリティカル率 */
	CopyMemoryRenew (pDataTmp, &m_wAttrFire,				sizeof (m_wAttrFire),				pDataTmp);	/* 属性[火] */
	CopyMemoryRenew (pDataTmp, &m_wAttrWind,				sizeof (m_wAttrWind),				pDataTmp);	/* 属性[風] */
	CopyMemoryRenew (pDataTmp, &m_wAttrWater,				sizeof (m_wAttrWater),				pDataTmp);	/* 属性[水] */
	CopyMemoryRenew (pDataTmp, &m_wAttrEarth,				sizeof (m_wAttrEarth),				pDataTmp);	/* 属性[土] */
	CopyMemoryRenew (pDataTmp, &m_wAttrLight,				sizeof (m_wAttrLight),				pDataTmp);	/* 属性[光] */
	CopyMemoryRenew (pDataTmp, &m_wAttrDark,				sizeof (m_wAttrDark),				pDataTmp);	/* 属性[闇] */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDCloth,		sizeof (m_dwEquipItemIDCloth),		pDataTmp);	/* 装備アイテムID:服 */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDAcce1,		sizeof (m_dwEquipItemIDAcce1),		pDataTmp);	/* 装備アイテムID:アクセサリ1 */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDAcce2,		sizeof (m_dwEquipItemIDAcce2),		pDataTmp);	/* 装備アイテムID:アクセサリ2 */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDArmsRight,	sizeof (m_dwEquipItemIDArmsRight),	pDataTmp);	/* 装備アイテムID:右手 */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDArmsLeft,	sizeof (m_dwEquipItemIDArmsLeft),	pDataTmp);	/* 装備アイテムID:左手 */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDHead,		sizeof (m_dwEquipItemIDHead),		pDataTmp);	/* 装備アイテムID:頭 */
	CopyMemoryRenew (pDataTmp, &m_dwMoveWait,				sizeof (m_dwMoveWait),				pDataTmp);	/* 移動待ち時間 */
	CopyMemoryRenew (pDataTmp, &m_dwMoveWaitBattle,			sizeof (m_dwMoveWaitBattle),		pDataTmp);	/* 戦闘時移動待ち時間 */
	CopyMemoryRenew (pDataTmp, &m_dwExp,					sizeof (m_dwExp),					pDataTmp);	/* 経験値 */
	CopyMemoryRenew (pDataTmp, &m_dwHP,						sizeof (m_dwHP),					pDataTmp);	/* HP */
	CopyMemoryRenew (pDataTmp, &m_dwMaxHP,					sizeof (m_dwMaxHP),					pDataTmp);	/* 最大HP */
	CopyMemoryRenew (pDataTmp, &m_dwSP,						sizeof (m_dwSP),					pDataTmp);	/* SP */
	CopyMemoryRenew (pDataTmp, &m_dwMaxSP,					sizeof (m_dwMaxSP),					pDataTmp);	/* 最大SP */
	CopyMemoryRenew (pDataTmp, &m_clName,					sizeof (m_clName),					pDataTmp);	/* 名前の描画色 */
	CopyMemoryRenew (pDataTmp, &m_clSpeak,					sizeof (m_clSpeak),					pDataTmp);	/* 発言の描画色 */
	strcpyRenew ((LPSTR)pDataTmp, m_strCharName, pDataTmp);													/* キャラ名 */
	strcpyRenew ((LPSTR)pDataTmp, m_strSpeak, pDataTmp);													/* 発言内容 */
	strcpyRenew ((LPSTR)pDataTmp, m_strTalk,  pDataTmp);													/* 会話データ */

	nCount = m_abyMark.GetSize ();
	for (i = 0; i < nCount; i ++) {
		byTmp = m_abyMark[i];
		CopyMemoryRenew (pDataTmp, &byTmp, sizeof (byTmp), pDataTmp);	/* 名前の前に表示するマーク番号 */
	}
	byTmp = 0;
	CopyMemoryRenew (pDataTmp, &byTmp, sizeof (byTmp), pDataTmp);
	nCount = m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_adwItemID[i];
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);	/* 所持アイテム */
	}
	dwTmp = 0;
	CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);
	nCount = m_adwSkillID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_adwSkillID[i];
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);	/* 所持スキル */
	}
	dwTmp = 0;
	CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);

	/* NPC発生 */
	CopyMemoryRenew (pDataTmp, &m_dwPutCycle,	sizeof (m_dwPutCycle),		pDataTmp);		/* 発生周期 */
	CopyMemoryRenew (pDataTmp, &m_nPutMoveType,	sizeof (m_nPutMoveType),	pDataTmp);		/* 発生させる移動種別 */
	CopyMemoryRenew (pDataTmp, &m_nMaxPutCount,	sizeof (m_nMaxPutCount),	pDataTmp);		/* 同時発生数 */
	CopyMemoryRenew (pDataTmp, &m_nPutAverage,	sizeof (m_nPutAverage),		pDataTmp);		/* 発生確率 */
	CopyMemoryRenew (pDataTmp, &m_ptPutArea,	sizeof (m_ptPutArea),		pDataTmp);		/* 発生範囲(半径) */

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetSendData										 */
/* 内容		:送信データを設定												 */
/* 日付		:2006/11/06														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CInfoCharBase::SetSendData(PBYTE pSrc)
{
	BYTE byTmp;
	DWORD dwTmp;
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	m_abyMark.RemoveAll ();
	m_adwItemID.RemoveAll ();
	m_adwSkillID.RemoveAll ();

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwCharID,				pDataTmp, sizeof (m_dwCharID),					pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwAccountID,			pDataTmp, sizeof (m_dwAccountID),				pDataTmp);	/* アカウントID */
	CopyMemoryRenew (&m_dwTailCharID,			pDataTmp, sizeof (m_dwTailCharID),				pDataTmp);	/* 付いて来ているキャラID */
	CopyMemoryRenew (&m_dwFrontCharID,			pDataTmp, sizeof (m_dwFrontCharID),				pDataTmp);	/* 付いているキャラID */
	CopyMemoryRenew (&m_dwParentCharID,			pDataTmp, sizeof (m_dwParentCharID),			pDataTmp);	/* 親のキャラID */
	CopyMemoryRenew (&m_bBlock,					pDataTmp, sizeof (m_bBlock),					pDataTmp);	/* ぶつかる判定 */
	CopyMemoryRenew (&m_bPush,					pDataTmp, sizeof (m_bPush),						pDataTmp);	/* 押せる判定 */
	CopyMemoryRenew (&m_dwMapID,				pDataTmp, sizeof (m_dwMapID),					pDataTmp);	/* マップID */
	CopyMemoryRenew (&m_dwMotionTypeID,			pDataTmp, sizeof (m_dwMotionTypeID),			pDataTmp);	/* モーション種別ID */
	CopyMemoryRenew (&m_nAnime,					pDataTmp, sizeof (m_nAnime),					pDataTmp);	/* アニメーション番号 */
	CopyMemoryRenew (&m_nMapX,					pDataTmp, sizeof (m_nMapX),						pDataTmp);	/* マップ座標(横) */
	CopyMemoryRenew (&m_nMapY,					pDataTmp, sizeof (m_nMapY),						pDataTmp);	/* マップ座標(縦) */
	CopyMemoryRenew (&m_nMoveState,				pDataTmp, sizeof (m_nMoveState),				pDataTmp);	/* 移動状態 */
	CopyMemoryRenew (&m_nMoveType,				pDataTmp, sizeof (m_nMoveType),					pDataTmp);	/* 移動種別 */
	CopyMemoryRenew (&m_nDirection,				pDataTmp, sizeof (m_nDirection),				pDataTmp);	/* 向き */
	CopyMemoryRenew (&m_nSex,					pDataTmp, sizeof (m_nSex),						pDataTmp);	/* 性別 */
	CopyMemoryRenew (&m_nMaxItemCount,			pDataTmp, sizeof (m_nMaxItemCount),				pDataTmp);	/* 最大アイテム所持数 */
	CopyMemoryRenew (&m_nDropItemAverage,		pDataTmp, sizeof (m_nDropItemAverage),			pDataTmp);	/* アイテムドロップ率 */
	CopyMemoryRenew (&m_nMoveAverage,			pDataTmp, sizeof (m_nMoveAverage),				pDataTmp);	/* 移動確率 */
	CopyMemoryRenew (&m_nMoveAverageBattle,		pDataTmp, sizeof (m_nMoveAverageBattle),		pDataTmp);	/* 戦闘時移動確率 */
	CopyMemoryRenew (&m_wFamilyID,				pDataTmp, sizeof (m_wFamilyID),					pDataTmp);	/* 種族ID */
	CopyMemoryRenew (&m_wGrpIDNPC,				pDataTmp, sizeof (m_wGrpIDNPC),					pDataTmp);	/* 画像ID(NPC) */
	CopyMemoryRenew (&m_wGrpIDCloth,			pDataTmp, sizeof (m_wGrpIDCloth),				pDataTmp);	/* 画像ID(服) */
	CopyMemoryRenew (&m_wGrpIDEye,				pDataTmp, sizeof (m_wGrpIDEye),					pDataTmp);	/* 画像ID(目) */
	CopyMemoryRenew (&m_wGrpIDEyeColor,			pDataTmp, sizeof (m_wGrpIDEyeColor),			pDataTmp);	/* 画像ID(目色) */
	CopyMemoryRenew (&m_wGrpIDHairType,			pDataTmp, sizeof (m_wGrpIDHairType),			pDataTmp);	/* 画像ID(髪) */
	CopyMemoryRenew (&m_wGrpIDHairColor,		pDataTmp, sizeof (m_wGrpIDHairColor),			pDataTmp);	/* 画像ID(髪色) */
	CopyMemoryRenew (&m_wGrpIDSP,				pDataTmp, sizeof (m_wGrpIDSP),					pDataTmp);	/* 画像ID(特殊服) */
	CopyMemoryRenew (&m_wGrpIDTmpMain,			pDataTmp, sizeof (m_wGrpIDTmpMain),				pDataTmp);	/* 画像ID(一時服:メイン) */
	CopyMemoryRenew (&m_wGrpIDTmpSub,			pDataTmp, sizeof (m_wGrpIDTmpSub),				pDataTmp);	/* 画像ID(一時服:サブ) */
	CopyMemoryRenew (&m_wGrpIDAcce,				pDataTmp, sizeof (m_wGrpIDAcce),				pDataTmp);	/* 画像ID(アクセサリ) */
	CopyMemoryRenew (&m_wGrpIDArmsMain,			pDataTmp, sizeof (m_wGrpIDArmsMain),			pDataTmp);	/* 画像ID(持ち物:メイン) */
	CopyMemoryRenew (&m_wGrpIDArmsSub,			pDataTmp, sizeof (m_wGrpIDArmsSub),				pDataTmp);	/* 画像ID(持ち物:サブ) */
	CopyMemoryRenew (&m_wGrpIDArmsLeftMain,		pDataTmp, sizeof (m_wGrpIDArmsLeftMain),		pDataTmp);	/* 画像ID(盾:メイン) */
	CopyMemoryRenew (&m_wGrpIDArmsLeftSub,		pDataTmp, sizeof (m_wGrpIDArmsLeftSub),			pDataTmp);	/* 画像ID(盾:サブ) */
	CopyMemoryRenew (&m_wGrpIDInitNPC,			pDataTmp, sizeof (m_wGrpIDInitNPC),				pDataTmp);	/* 初期画像ID(NPC) */
	CopyMemoryRenew (&m_wGrpIDInitCloth,		pDataTmp, sizeof (m_wGrpIDInitCloth),			pDataTmp);	/* 初期画像ID(服) */
	CopyMemoryRenew (&m_wGrpIDInitEye,			pDataTmp, sizeof (m_wGrpIDInitEye),				pDataTmp);	/* 初期画像ID(目) */
	CopyMemoryRenew (&m_wGrpIDInitEyeColor,		pDataTmp, sizeof (m_wGrpIDInitEyeColor),		pDataTmp);	/* 初期画像ID(目色) */
	CopyMemoryRenew (&m_wGrpIDInitHairType,		pDataTmp, sizeof (m_wGrpIDInitHairType),		pDataTmp);	/* 初期画像ID(髪) */
	CopyMemoryRenew (&m_wGrpIDInitHairColor,	pDataTmp, sizeof (m_wGrpIDInitHairColor),		pDataTmp);	/* 初期画像ID(髪色) */
	CopyMemoryRenew (&m_wGrpIDInitSP,			pDataTmp, sizeof (m_wGrpIDInitSP),				pDataTmp);	/* 初期画像ID(特殊服) */
	CopyMemoryRenew (&m_wAtackGauge,			pDataTmp, sizeof (m_wAtackGauge),				pDataTmp);	/* アタックゲージ */
	CopyMemoryRenew (&m_wDefenseGauge,			pDataTmp, sizeof (m_wDefenseGauge),				pDataTmp);	/* ディフェンスゲージ */
	CopyMemoryRenew (&m_wLevel,					pDataTmp, sizeof (m_wLevel),					pDataTmp);	/* レベル */
	CopyMemoryRenew (&m_wStamina,				pDataTmp, sizeof (m_wStamina),					pDataTmp);	/* スタミナ */
	CopyMemoryRenew (&m_wPower,					pDataTmp, sizeof (m_wPower),					pDataTmp);	/* 腕力 */
	CopyMemoryRenew (&m_wStrength,				pDataTmp, sizeof (m_wStrength),					pDataTmp);	/* 体力 */
	CopyMemoryRenew (&m_wMagic,					pDataTmp, sizeof (m_wMagic),					pDataTmp);	/* 魔力 */
	CopyMemoryRenew (&m_wSkillful,				pDataTmp, sizeof (m_wSkillful),					pDataTmp);	/* 器用 */
	CopyMemoryRenew (&m_wAbillityAT,			pDataTmp, sizeof (m_wAbillityAT),				pDataTmp);	/* 攻撃技能 */
	CopyMemoryRenew (&m_wAbillityDF,			pDataTmp, sizeof (m_wAbillityDF),				pDataTmp);	/* 防御技能 */
	CopyMemoryRenew (&m_wPAtack,				pDataTmp, sizeof (m_wPAtack),					pDataTmp);	/* 攻撃力 */
	CopyMemoryRenew (&m_wPDefense,				pDataTmp, sizeof (m_wPDefense),					pDataTmp);	/* 防御力 */
	CopyMemoryRenew (&m_wPMagic,				pDataTmp, sizeof (m_wPMagic),					pDataTmp);	/* 魔法力 */
	CopyMemoryRenew (&m_wPMagicDefense,			pDataTmp, sizeof (m_wPMagicDefense),			pDataTmp);	/* 魔法防御力 */
	CopyMemoryRenew (&m_wPHitAverage,			pDataTmp, sizeof (m_wPHitAverage),				pDataTmp);	/* 命中率 */
	CopyMemoryRenew (&m_wPAvoidAverage,			pDataTmp, sizeof (m_wPAvoidAverage),			pDataTmp);	/* 回避率 */
	CopyMemoryRenew (&m_wPCriticalAverage,		pDataTmp, sizeof (m_wPCriticalAverage),			pDataTmp);	/* クリティカル率 */
	CopyMemoryRenew (&m_wAttrFire,				pDataTmp, sizeof (m_wAttrFire),					pDataTmp);	/* 属性[火] */
	CopyMemoryRenew (&m_wAttrWind,				pDataTmp, sizeof (m_wAttrWind),					pDataTmp);	/* 属性[風] */
	CopyMemoryRenew (&m_wAttrWater,				pDataTmp, sizeof (m_wAttrWater),				pDataTmp);	/* 属性[水] */
	CopyMemoryRenew (&m_wAttrEarth,				pDataTmp, sizeof (m_wAttrEarth),				pDataTmp);	/* 属性[土] */
	CopyMemoryRenew (&m_wAttrLight,				pDataTmp, sizeof (m_wAttrLight),				pDataTmp);	/* 属性[光] */
	CopyMemoryRenew (&m_wAttrDark,				pDataTmp, sizeof (m_wAttrDark),					pDataTmp);	/* 属性[闇] */
	CopyMemoryRenew (&m_dwEquipItemIDCloth,		pDataTmp, sizeof (m_dwEquipItemIDCloth),		pDataTmp);	/* 装備アイテムID:服 */
	CopyMemoryRenew (&m_dwEquipItemIDAcce1,		pDataTmp, sizeof (m_dwEquipItemIDAcce1),		pDataTmp);	/* 装備アイテムID:アクセサリ1 */
	CopyMemoryRenew (&m_dwEquipItemIDAcce2,		pDataTmp, sizeof (m_dwEquipItemIDAcce2),		pDataTmp);	/* 装備アイテムID:アクセサリ2 */
	CopyMemoryRenew (&m_dwEquipItemIDArmsRight,	pDataTmp, sizeof (m_dwEquipItemIDArmsRight),	pDataTmp);	/* 装備アイテムID:右手 */
	CopyMemoryRenew (&m_dwEquipItemIDArmsLeft,	pDataTmp, sizeof (m_dwEquipItemIDArmsLeft),		pDataTmp);	/* 装備アイテムID:左手 */
	CopyMemoryRenew (&m_dwEquipItemIDHead,		pDataTmp, sizeof (m_dwEquipItemIDHead),			pDataTmp);	/* 装備アイテムID:頭 */
	CopyMemoryRenew (&m_dwMoveWait,				pDataTmp, sizeof (m_dwMoveWait),				pDataTmp);	/* 移動待ち時間 */
	CopyMemoryRenew (&m_dwMoveWaitBattle,		pDataTmp, sizeof (m_dwMoveWaitBattle),			pDataTmp);	/* 戦闘時移動待ち時間 */
	CopyMemoryRenew (&m_dwExp,					pDataTmp, sizeof (m_dwExp),						pDataTmp);	/* 経験値 */
	CopyMemoryRenew (&m_dwHP,					pDataTmp, sizeof (m_dwHP),						pDataTmp);	/* HP */
	CopyMemoryRenew (&m_dwMaxHP,				pDataTmp, sizeof (m_dwMaxHP),					pDataTmp);	/* 最大HP */
	CopyMemoryRenew (&m_dwSP,					pDataTmp, sizeof (m_dwSP),						pDataTmp);	/* SP */
	CopyMemoryRenew (&m_dwMaxSP,				pDataTmp, sizeof (m_dwMaxSP),					pDataTmp);	/* 最大SP */
	CopyMemoryRenew (&m_clName,					pDataTmp, sizeof (m_clName),					pDataTmp);	/* 名前の描画色 */
	CopyMemoryRenew (&m_clSpeak,				pDataTmp, sizeof (m_clSpeak),					pDataTmp);	/* 発言の描画色 */
	StoreRenew (m_strCharName,	(LPCSTR)pDataTmp, pDataTmp);												/* キャラ名 */
	StoreRenew (m_strSpeak,		(LPCSTR)pDataTmp, pDataTmp);												/* 発言内容 */
	StoreRenew (m_strTalk,		(LPCSTR)pDataTmp, pDataTmp);												/* 会話データ */

	SetName (m_strCharName);
	SetSpeak (m_strSpeak);

	while (1) {
		CopyMemoryRenew (&byTmp, pDataTmp, sizeof (byTmp), pDataTmp);	/* 名前の前に表示するマーク番号 */
		if (byTmp == 0) {
			break;
		}
		m_abyMark.Add (byTmp);
	}
	while (1) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);	/* 所持アイテム */
		if (dwTmp == 0) {
			break;
		}
		m_adwItemID.Add (dwTmp);
	}
	while (1) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);	/* 所持スキル */
		if (dwTmp == 0) {
			break;
		}
		m_adwSkillID.Add (dwTmp);
	}

	/* NPC発生 */
	CopyMemoryRenew (&m_dwPutCycle,		pDataTmp, sizeof (m_dwPutCycle),	pDataTmp);		/* 発生周期 */
	CopyMemoryRenew (&m_nPutMoveType,	pDataTmp, sizeof (m_nPutMoveType),	pDataTmp);		/* 発生させる移動種別 */
	CopyMemoryRenew (&m_nMaxPutCount,	pDataTmp, sizeof (m_nMaxPutCount),	pDataTmp);		/* 同時発生数 */
	CopyMemoryRenew (&m_nPutAverage,	pDataTmp, sizeof (m_nPutAverage),	pDataTmp);		/* 発生確率 */
	CopyMemoryRenew (&m_ptPutArea,		pDataTmp, sizeof (m_ptPutArea),		pDataTmp);		/* 発生範囲(半径) */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsLogin											 */
/* 内容		:ログイン中か判定												 */
/* 日付		:2007/02/12														 */
/* 戻り値	:TRUE:ログイン中 FALS:ログアウト中								 */
/* ========================================================================= */

BOOL CInfoCharBase::IsLogin(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nMoveType != CHARMOVETYPE_PC) {
		return TRUE;
	}
	if (m_bNPC) {
		bRet = TRUE;
		goto Exit;
	}
	if (m_dwAccountID == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsStateBattle									 */
/* 内容		:戦闘モードか判定												 */
/* 日付		:2007/07/21														 */
/* 戻り値	:TRUE:戦闘モード中												 */
/* ========================================================================= */

BOOL CInfoCharBase::IsStateBattle(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nMoveState) {
	case CHARMOVESTATE_BATTLE:			/* 戦闘中 */
	case CHARMOVESTATE_BATTLEATACK:		/* 戦闘攻撃中 */
	case CHARMOVESTATE_BATTLEMOVE:		/* 戦闘移動中 */
	case CHARMOVESTATE_BATTLE_DEFENSE:	/* 防御中 */
		break;
	default:
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsStateMove										 */
/* 内容		:移動中か判定													 */
/* 日付		:2008/08/13														 */
/* 戻り値	:TRUE:移動中													 */
/* ========================================================================= */

BOOL CInfoCharBase::IsStateMove(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nMoveState) {
	case CHARMOVESTATE_MOVE:			/* 移動中 */
	case CHARMOVESTATE_BATTLEMOVE:		/* 戦闘移動中 */
		break;
	default:
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsViewArea										 */
/* 内容		:見える範囲か判定												 */
/* 日付		:2007/09/08														 */
/* 戻り値	:TRUE:見える範囲												 */
/* ========================================================================= */

BOOL CInfoCharBase::IsViewArea(DWORD dwMapID, POINT *pptPos)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_dwMapID != dwMapID) {
		goto Exit;
	}
	if (!((abs (pptPos->x - m_nMapX) <= (DRAW_PARTS_X * 2) + 1) &&
		(abs (pptPos->y - m_nMapY) <= (DRAW_PARTS_Y * 2) + 1))) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsNPC											 */
/* 内容		:NPCか判定														 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

BOOL CInfoCharBase::IsNPC(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nMoveType == CHARMOVETYPE_PC) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::CheckSessionID									 */
/* 内容		:セッションIDをチェック											 */
/* 日付		:2008/01/05														 */
/* ========================================================================= */

BOOL CInfoCharBase::CheckSessionID(DWORD dwSessionID)
{
	BOOL bRet;

	bRet = FALSE;

	if (dwSessionID != m_dwSessionID) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetFrontPos										 */
/* 内容		:一歩前のキャラ座標を取得										 */
/* 日付		:2007/07/27														 */
/* ========================================================================= */

void CInfoCharBase::GetFrontPos(
	POINT &ptDst,					/* [ou] 取得先 */
	int nDirection	/*-1*/,			/* [in] 向き */
	BOOL bMove		/*FALSE*/)		/* [in] TRUE:移動先用のキャラ座標 */
{
	int nFrontPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, nFrontPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	SIZE sizeChar;

	if (nDirection == -1) {
		nDirection = m_nDirection;
	}

	sizeChar.cx = sizeChar.cy = 1;
	if (bMove == FALSE) {
		GetCharSize (sizeChar);
	}
	ptDst.x = m_nMapX + (nFrontPosX[nDirection] * sizeChar.cx);
	ptDst.y = m_nMapY + (nFrontPosY[nDirection] * sizeChar.cy);
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetFrontPos										 */
/* 内容		:一歩前のキャラ座標を取得										 */
/* 日付		:2008/05/01														 */
/* ========================================================================= */

void CInfoCharBase::GetFrontPos(
	CmyArray<POINT, POINT> &aptPos,		/* [ou] 取得先 */
	int nDirection	/*-1*/)				/* [in] 向き */
{
	int i, nCount;
	SIZE sizeChar;
	POINT ptFront, ptTmp;
	int nFrontPosX[] = {0, 0, 0, 1, 1, 1, 0, 0}, nFrontPosY[] = {0, 1, 0, 0, 0, 1, 1, 0};

	aptPos.RemoveAll ();
	if (nDirection == -1) {
		nDirection = m_nDirection;
	}
	GetFrontPos (ptFront, nDirection, FALSE);

	sizeChar.cx = sizeChar.cy = 1;
	GetCharSize (sizeChar);

	ptTmp = ptFront;
	switch (nDirection) {
	case 0:
	case 1:
	case 2:
	case 3:
		nCount = sizeChar.cx;
		if (nDirection == 3) {
			ptTmp.x -= sizeChar.cx;
		}
		if (nDirection < 2) {
			ptTmp.x -= (sizeChar.cx - 1);
			nCount ++;
		}
		ptTmp.x += nFrontPosX[nDirection];
		ptTmp.y += nFrontPosY[nDirection];
		for (i = 0; i < nCount; i ++) {
			aptPos.Add (ptTmp);
			ptTmp.x ++;
		}
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		ptTmp  = ptFront;
		nCount = sizeChar.cx;
		ptTmp.x --;
		ptTmp.y += nFrontPosY[nDirection];
		for (i = 0; i < nCount + 1; i ++) {
			aptPos.Add (ptTmp);
			ptTmp.x ++;
		}
		ptTmp  = ptFront;
		nCount = sizeChar.cy;
		ptTmp.x += nFrontPosX[nDirection];
		for (i = 0; i < nCount; i ++) {
			aptPos.Add (ptTmp);
			ptTmp.y ++;
		}
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetFrontMapPos									 */
/* 内容		:一歩前のマップ座標を取得										 */
/* 日付		:2008/05/24														 */
/* ========================================================================= */

void CInfoCharBase::GetFrontMapPos(
	POINT &ptDst,				/* [ou] 取得先 */
	int nDirection	/*-1*/)		/* [in] 向き */
{
	int x, y, nFrontPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, nFrontPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	SIZE sizeChar;

	if (nDirection == -1) {
		nDirection = m_nDirection;
	}

	sizeChar.cx = sizeChar.cy = 1;
	GetCharSize (sizeChar);
	if (sizeChar.cx > 1) {
		sizeChar.cx --;
	}
	if (sizeChar.cy > 1) {
		sizeChar.cy --;
	}
	x = m_nMapX;
	y = m_nMapY;
	if (nFrontPosX[nDirection] == 1) {
		x += sizeChar.cx;
	}
	if (nFrontPosY[nDirection] == 1) {
		y += sizeChar.cy;
	}
	if (x % 2) {
		x -= (nFrontPosX[nDirection] * sizeChar.cx);
	}
	if (y % 2) {
		y -= (nFrontPosY[nDirection] * sizeChar.cy);
	}
	ptDst.x = x / 2 + nFrontPosX[nDirection];
	ptDst.y = y / 2 + nFrontPosY[nDirection];
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetDirection									 */
/* 内容		:指定座標の向きを取得											 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

int CInfoCharBase::GetDirection(
	int x,		/* [in] 移動先の座標(X) */
	int y)		/* [in] 移動先の座標(Y) */
{
	int nRet;

	nRet = -1;

	if (y < m_nMapY) {
		if (x < m_nMapX) {
			nRet = 7;
		} else if (x > m_nMapX) {
			nRet = 4;
		} else {
			nRet = 0;
		}

	} else if (y > m_nMapY) {
		if (x < m_nMapX) {
			nRet = 6;
		} else if (x > m_nMapX) {
			nRet = 5;
		} else {
			nRet = 1;
		}
	} else {
		if (x < m_nMapX) {
			nRet = 2;
		} else if (x > m_nMapX) {
			nRet = 3;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetAtackType									 */
/* 内容		:攻撃種別を取得													 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

int CInfoCharBase::GetAtackType(void)
{
	int nRet;

	nRet = 0;

//Todo:画像ではなく武器の属性を見るように
	switch (m_wGrpIDArmsMain) {
	case 0:		/* 無し */
		nRet = WEAPONTYPE_SWING;
		break;
//	case 1:		/* 振り・突き */
//		nRet = WEAPONTYPE_POKE;
//		break;
	case 1:		/* 弓 */
		nRet = WEAPONTYPE_BOW;
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetAtackMotion									 */
/* 内容		:攻撃モーションの設定											 */
/* 日付		:2007/12/05														 */
/* ========================================================================= */

void CInfoCharBase::SetAtackMotion(
	int nMotionID)		/* [in] モーションID(4方向の先頭) */
{
	m_adwMotionID[CHARMOTIONID_ATACK] = nMotionID;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetDrawDirection								 */
/* 内容		:描画用に4方向で向きを取得										 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

int CInfoCharBase::GetDrawDirection(
	int nDirection/*-1*/)		/* [in] 向き */
{
	int anDirection[] = {0, 1, 2, 3, 0, 1, 1, 0};

	if (nDirection == -1) {
		nDirection = m_nDirection;
	}

	return anDirection[nDirection];
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetBackDirection								 */
/* 内容		:反対方向を取得													 */
/* 日付		:2008/11/24														 */
/* ========================================================================= */

int CInfoCharBase::GetBackDirection(void)
{
	int anDirection[] = {1, 0, 3, 2, 6, 7, 5, 4};

	return anDirection[m_nDirection];
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetMoveWait										 */
/* 内容		:状態に応じた移動速度を取得										 */
/* 日付		:2008/08/09														 */
/* ========================================================================= */

DWORD CInfoCharBase::GetMoveWait(void)
{
	DWORD dwRet;

	dwRet = m_dwMoveWait;
	if (IsStateBattle ()) {
		if (m_dwMoveWaitBattle == 0) {
			dwRet = (DWORD)((float)dwRet * 1.4f);
		} else {
			dwRet = m_dwMoveWaitBattle;
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetItem											 */
/* 内容		:アイテム情報を設定												 */
/* 日付		:2007/08/05														 */
/* ========================================================================= */

void CInfoCharBase::SetItem(ARRAYDWORD *padwItemID)
{
	m_adwItemID.Copy (padwItemID);
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsItemAdd										 */
/* 内容		:アイテムを追加できるか判定										 */
/* 日付		:2007/08/11														 */
/* ========================================================================= */

BOOL CInfoCharBase::IsItemAdd(void)
{
	BOOL bRet;
	int nCount;

	bRet = FALSE;

	nCount = m_adwItemID.GetSize ();
	/* もう持てない？ */
	if (nCount >= m_nMaxItemCount) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::DeleteItem										 */
/* 内容		:所持アイテムを削除												 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

void CInfoCharBase::DeleteItem(DWORD dwItemID)
{
	//Todo:装備品の解除などの処理を実装する
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::HaveItem										 */
/* 内容		:指定アイテムを持っているか判定									 */
/* 日付		:2008/06/10														 */
/* ========================================================================= */

BOOL CInfoCharBase::HaveItem(DWORD dwItemID)
{
	BOOL bRet;
	int i, nCount;

	bRet = FALSE;
	nCount = m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		if (m_adwItemID[i] == dwItemID) {
			break;
		}
	}
	if (i >= nCount) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetTargetPos									 */
/* 内容		:指定座標を取得													 */
/* 日付		:2008/08/09														 */
/* ========================================================================= */

void CInfoCharBase::GetTargetPos(
	POINT *ptTarget,		/* [in] 最終移動先 */
	POINT &ptDst,			/* [out] 受取先 */
	int nCount)				/* [in] 指定歩数 */
{
	int i, E, dx, dy, sx, sy, x0, x1, y0, y1, nCountTmp;

	ptDst.x = ptDst.y = -1;
	x0 = m_nMapX;
	y0 = m_nMapY;
	x1 = ptTarget->x;
	y1 = ptTarget->y;

	dx = (x1 > x0) ? x1 - x0 : x0 - x1;
	dy = (y1 > y0) ? y1 - y0 : y0 - y1;
	sx = (x1 > x0) ? 1 : -1;
	sy = (y1 > y0) ? 1 : -1;

	nCountTmp = 0;
	/* 傾きが1以下の場合 */
	if (dx >= dy) {
		E = -dx;
		for (i = 0 ; i <= dx ; i ++) {
			if (nCount >= 0) {
				if (nCountTmp >= nCount) {
					ptDst.x = x0;
					ptDst.y = y0;
					break;
				}
			}
			x0 += sx;
			E += 2 * dy;
			if (E >= 0) {
				y0 += sy;
				E -= 2 * dx;
			}
			nCountTmp ++;
		}

	/* 傾きが1より大きい場合 */
	} else {
		E = -dy;
		for (i = 0 ; i <= dy ; i ++) {
			if (nCount >= 0) {
				if (nCountTmp >= nCount) {
					ptDst.x = x0;
					ptDst.y = y0;
					break;
				}
			}
			y0 += sy;
			E += 2 * dx;
			if (E >= 0) {
				x0 += sx;
				E -= 2 * dy;
			}
			nCountTmp ++;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetTarget										 */
/* 内容		:ターゲットキャラを設定											 */
/* 日付		:2008/08/09														 */
/* ========================================================================= */

void CInfoCharBase::SetTarget(CInfoCharBase *pCharTarget)
{
	BOOL bClear;

	bClear = FALSE;
	if (pCharTarget == NULL) {
		bClear = TRUE;
	} else {
		if (pCharTarget->IsLogin () == FALSE) {
			bClear = TRUE;
		}
		if (pCharTarget->IsAtackTarget () == FALSE) {
			bClear = TRUE;
		}
		if (pCharTarget->m_dwMapID != m_dwMapID) {
			bClear = TRUE;
		}
	}

	if (bClear) {
		m_dwTargetCharID = 0;
		m_ptTargetPos.x = m_ptTargetPos.y = -1;
	} else {
		m_dwTargetCharID	= pCharTarget->m_dwCharID;
		m_ptTargetPos.x		= pCharTarget->m_nMapX;
		m_ptTargetPos.y		= pCharTarget->m_nMapY;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetPos											 */
/* 内容		:座標を指定														 */
/* 日付		:2008/04/29														 */
/* 戻り値	:移動方向														 */
/* ========================================================================= */

int CInfoCharBase::SetPos(int x, int y, BOOL bBack/*FALSE*/)
{
	int nRet;

	nRet = -1;
	if ((x == m_nMapX) && (y < m_nMapY)) {
		nRet = 0;
	} else if ((x == m_nMapX) && (y > m_nMapY)) {
		nRet = 1;
	} else if ((x < m_nMapX) && (y == m_nMapY)) {
		nRet = 2;
	} else if ((x > m_nMapX) && (y == m_nMapY)) {
		nRet = 3;
	} else if ((x > m_nMapX) && (y < m_nMapY)) {
		nRet = 4;
	} else if ((x > m_nMapX) && (y > m_nMapY)) {
		nRet = 5;
	} else if ((x < m_nMapX) && (y > m_nMapY)) {
		nRet = 6;
	} else if ((x < m_nMapX) && (y < m_nMapY)) {
		nRet = 7;
	}

	if ((m_nMapX != x) || (m_nMapY != y)) {
		m_ptCharBack.x = m_nMapX;
		m_ptCharBack.y = m_nMapY;
	}
	m_nMapX = x;
	m_nMapY = y;
	if (bBack) {
		m_ptCharBack.x = x;
		m_ptCharBack.y = y;
	}
	m_dwLastTimeMove = timeGetTime ();

	m_nMoveDirection = nRet;
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsMove											 */
/* 内容		:移動中か判定													 */
/* 日付		:2007/01/21														 */
/* 戻り値	:TRUE:移動中 FALSE:停止中										 */
/* ========================================================================= */

BOOL CInfoCharBase::IsMove(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nMoveState) {
	case CHARMOVESTATE_MOVE:		/* 移動中 */
	case CHARMOVESTATE_BATTLEMOVE:	/* 戦闘移動中 */
	case CHARMOVESTATE_BATTLEATACK:	/* 戦闘攻撃中 (移動できないという意味で) */
		bRet = TRUE;
		break;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetDirection									 */
/* 内容		:向きを指定														 */
/* 日付		:2008/04/29														 */
/* ========================================================================= */

void CInfoCharBase::SetDirection(int nDirection)
{
	if (m_nDirection != nDirection) {
		m_nDirectionBack = m_nDirection;
	}
	m_nDirection = nDirection;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetMoveState									 */
/* 内容		:移動状態を変更													 */
/* 日付		:2007/02/26														 */
/* ========================================================================= */

void CInfoCharBase::SetMoveState(int nMoveState)
{
	m_nMoveState = nMoveState;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetProcState									 */
/* 内容		:行動状態を変更													 */
/* 日付		:2008/06/11														 */
/* ========================================================================= */

void CInfoCharBase::SetProcState(int nProcState)
{
	m_nProcState = nProcState;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetName											 */
/* 内容		:キャラ名を更新													 */
/* 日付		:2007/01/28														 */
/* ========================================================================= */

void CInfoCharBase::SetName(LPCSTR pszName)
{
	m_strCharName = pszName;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetSpeak										 */
/* 内容		:発言内容を更新													 */
/* 日付		:2007/02/04														 */
/* ========================================================================= */

void CInfoCharBase::SetSpeak(LPCSTR pszSpeak)
{
	m_strSpeak = pszSpeak;

	if (m_strSpeak.IsEmpty ()) {
		m_dwLastTimeSpeak = 0;

	} else {
		m_dwLastTimeSpeak = timeGetTime ();
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::Copy											 */
/* 内容		:コピー															 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

void CInfoCharBase::Copy(CInfoCharBase *pSrc)
{
	int i, nCount;

	m_dwLightTime				= m_dwLightTime;				/* 灯り有効時間 */
	m_nLightLevel				= m_nLightLevel;				/* 灯りレベル */

	m_dwAccountID				= pSrc->m_dwAccountID;
	m_dwTailCharID				= pSrc->m_dwTailCharID;
	m_dwFrontCharID				= pSrc->m_dwFrontCharID;
	m_dwParentCharID			= pSrc->m_dwParentCharID;
	m_bBlock					= pSrc->m_bBlock;
	m_bPush						= pSrc->m_bPush;
	m_nAnime					= pSrc->m_nAnime;
	m_nMapX						= pSrc->m_nMapX;
	m_nMapY						= pSrc->m_nMapY;
	m_nMoveState				= pSrc->m_nMoveState;
	m_nMoveType					= pSrc->m_nMoveType;
	m_nDirection				= pSrc->m_nDirection;
	m_nGrpSize					= pSrc->m_nGrpSize;
	m_nSex						= pSrc->m_nSex;
	m_nMaxItemCount				= pSrc->m_nMaxItemCount;
	m_nDropItemAverage			= pSrc->m_nDropItemAverage;		/* アイテムドロップ率 */
	m_nMoveAverage				= pSrc->m_nMoveAverage;			/* 移動確率 */
	m_nMoveAverageBattle		= pSrc->m_nMoveAverageBattle;	/* 戦闘時移動確率 */
	m_bChargeAtack				= pSrc->m_bChargeAtack;			/* 溜め攻撃 */
	m_wFamilyID					= pSrc->m_wFamilyID;
	m_wGrpIDNPC					= pSrc->m_wGrpIDNPC;
	m_wGrpIDCloth				= pSrc->m_wGrpIDCloth;
	m_wGrpIDEye					= pSrc->m_wGrpIDEye;
	m_wGrpIDEyeColor			= pSrc->m_wGrpIDEyeColor;
	m_wGrpIDHairType			= pSrc->m_wGrpIDHairType;
	m_wGrpIDHairColor			= pSrc->m_wGrpIDHairColor;
	m_wGrpIDSP					= pSrc->m_wGrpIDSP;
	m_wGrpIDTmpMain				= pSrc->m_wGrpIDTmpMain;		/* 画像ID(一時服:メイン) */
	m_wGrpIDTmpSub				= pSrc->m_wGrpIDTmpSub;			/* 画像ID(一時服:サブ) */
	m_wGrpIDAcce				= pSrc->m_wGrpIDAcce;
	m_wGrpIDArmsMain			= pSrc->m_wGrpIDArmsMain;
	m_wGrpIDArmsSub				= pSrc->m_wGrpIDArmsSub;
	m_wGrpIDArmsLeftMain		= pSrc->m_wGrpIDArmsLeftMain;
	m_wGrpIDArmsLeftSub			= pSrc->m_wGrpIDArmsLeftSub;
	m_wGrpIDInitNPC				= pSrc->m_wGrpIDInitNPC;
	m_wGrpIDInitCloth			= pSrc->m_wGrpIDInitCloth;
	m_wGrpIDInitEye				= pSrc->m_wGrpIDInitEye;
	m_wGrpIDInitEyeColor		= pSrc->m_wGrpIDInitEyeColor;
	m_wGrpIDInitHairType		= pSrc->m_wGrpIDInitHairType;
	m_wGrpIDInitHairColor		= pSrc->m_wGrpIDInitHairColor;
	m_wGrpIDInitSP				= pSrc->m_wGrpIDInitSP;
	m_wAtackGauge				= pSrc->m_wAtackGauge;			/* アタックゲージ */
	m_wDefenseGauge				= pSrc->m_wDefenseGauge;		/* ディフェンスゲージ */
	m_wLevel					= pSrc->m_wLevel;				/* レベル */
	m_wStamina					= pSrc->m_wStamina;				/* スタミナ */
	m_wPower					= pSrc->m_wPower;				/* 腕力 */
	m_wStrength					= pSrc->m_wStrength;			/* 体力 */
	m_wMagic					= pSrc->m_wMagic;				/* 魔力 */
	m_wSkillful					= pSrc->m_wSkillful;			/* 器用 */
	m_wAbillityAT				= pSrc->m_wAbillityAT;			/* 攻撃技能 */
	m_wAbillityDF				= pSrc->m_wAbillityDF;			/* 防御技能 */
	m_wPAtack					= pSrc->m_wPAtack;				/* 攻撃力 */
	m_wPDefense					= pSrc->m_wPDefense;			/* 防御力 */
	m_wPMagic					= pSrc->m_wPMagic;				/* 魔法力 */
	m_wPMagicDefense			= pSrc->m_wPMagicDefense;		/* 魔法防御力 */
	m_wPHitAverage				= pSrc->m_wPHitAverage;			/* 命中率 */
	m_wPAvoidAverage			= pSrc->m_wPAvoidAverage;		/* 回避率 */
	m_wPCriticalAverage			= pSrc->m_wPCriticalAverage;	/* クリティカル率 */
	m_wAttrFire					= pSrc->m_wAttrFire;			/* 属性[火] */
	m_wAttrWind					= pSrc->m_wAttrWind;			/* 属性[風] */
	m_wAttrWater				= pSrc->m_wAttrWater;			/* 属性[水] */
	m_wAttrEarth				= pSrc->m_wAttrEarth;			/* 属性[土] */
	m_wAttrLight				= pSrc->m_wAttrLight;			/* 属性[光] */
	m_wAttrDark					= pSrc->m_wAttrDark;			/* 属性[闇] */
	m_dwCharID					= pSrc->m_dwCharID;
	m_dwMapID					= pSrc->m_dwMapID;
	m_dwMotionTypeID			= pSrc->m_dwMotionTypeID;
	m_dwEquipItemIDCloth		= pSrc->m_dwEquipItemIDCloth;
	m_dwEquipItemIDAcce1		= pSrc->m_dwEquipItemIDAcce1;
	m_dwEquipItemIDAcce2		= pSrc->m_dwEquipItemIDAcce2;
	m_dwEquipItemIDArmsRight	= pSrc->m_dwEquipItemIDArmsRight;
	m_dwEquipItemIDArmsLeft		= pSrc->m_dwEquipItemIDArmsLeft;
	m_dwEquipItemIDHead			= pSrc->m_dwEquipItemIDHead;
	m_dwMoveWait				= pSrc->m_dwMoveWait;			/* 移動待ち時間 */
	m_dwMoveWaitBattle			= pSrc->m_dwMoveWaitBattle;		/* 戦闘時移動待ち時間 */
	m_dwExp						= pSrc->m_dwExp;				/* 経験値 */
	m_dwHP						= pSrc->m_dwHP;
	m_dwMaxHP					= pSrc->m_dwMaxHP;
	m_dwSP						= pSrc->m_dwSP;
	m_dwMaxSP					= pSrc->m_dwMaxSP;
	m_clName					= pSrc->m_clName;
	m_clSpeak					= pSrc->m_clSpeak;
	m_strTalk					= pSrc->m_strTalk;				/* 会話データ */

	m_abyMark.RemoveAll ();
	nCount = pSrc->m_abyMark.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_abyMark.Add (pSrc->m_abyMark[i]);
	}
	m_adwItemID.RemoveAll ();
	nCount = pSrc->m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_adwItemID.Add (pSrc->m_adwItemID[i]);
	}
	m_adwSkillID.RemoveAll ();
	nCount = pSrc->m_adwSkillID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_adwSkillID.Add (pSrc->m_adwSkillID[i]);
	}

	SetName (pSrc->m_strCharName);
	SetSpeak (pSrc->m_strSpeak);

	m_dwPutCycle		= pSrc->m_dwPutCycle;		/* 発生周期 */
	m_nPutMoveType		= pSrc->m_nPutMoveType;		/* 発生させる移動種別 */
	m_nMaxPutCount		= pSrc->m_nMaxPutCount;		/* 同時発生数 */
	m_nPutAverage		= pSrc->m_nPutAverage;		/* 発生確率 */
	m_ptPutArea			= pSrc->m_ptPutArea;		/* 発生範囲(半径) */
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CInfoCharBase::TimerProc(DWORD dwTime)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_dwLastTimeSpeak > 0) {
		if (timeGetTime () - m_dwLastTimeSpeak > 1000 * 10) {
			/* １０秒経過したので消去 */
			SetSpeak ("");
			bRet = TRUE;
		}
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::RenewBlockMapArea								 */
/* 内容		:当たり判定対象のマップ座標を更新								 */
/* 日付		:2008/04/13														 */
/* ========================================================================= */

void CInfoCharBase::RenewBlockMapArea(
	int x,				/* [in] 移動後のキャラ座標(横) */
	int y,				/* [in] 移動後のキャラ座標(縦) */
	int nDirection,		/* [in] 調べる向き */
	BOOL bMoveOut)		/* [in] TRUE:外への移動用に偶数時のみ対象とする */
{
	int anPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, anPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	POINT ptTmp, ptTmpBack;

	m_aposBockMapArea.RemoveAll ();
	if (nDirection < 0) {
		/* クリアのみ */
		return;
	}

//todo:キャラサイズ

	if (bMoveOut) {
		if (nDirection < 4) {
			if (anPosY[nDirection] != 0) {
				if (y % 2) {
					return;
				}
			} else if (anPosX[nDirection] != 0) {
				if (x % 2) {
					return;
				}
			}
		} else {
			if (anPosY[nDirection] != 0) {
				if (y % 2) {
					if (anPosX[nDirection] != 0) {
						if (x % 2) {
							return;
						}
					}
				}
			}
		}
	}

	ptTmp.x = x / 2;
	if (x < 0) {
		ptTmp.x = -1;
	}
	switch (nDirection) {
	case 3:
	case 4:
	case 5:
		if (x % 2) {
			ptTmp.x += anPosX[nDirection];
		}
		break;
	}
	if (y % 2) {
		y ++;
	}
	ptTmp.y = y / 2;
	if (y < 0) {
		ptTmp.y = -1;
	}
	switch (nDirection) {
	case 1:
	case 5:
	case 6:
		if (y % 2) {
			ptTmp.y += anPosY[nDirection];
		}
		break;
	}
	m_aposBockMapArea.Add (ptTmp);

	ptTmpBack = ptTmp;
	if (x % 2) {
		switch (nDirection) {
		case 0:
		case 1:
		case 4:
		case 5:
			ptTmp.x ++;
			m_aposBockMapArea.Add (ptTmp);
			break;
		}
	}
}

/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsHitCharPos									 */
/* 内容		:キャラ座標との当たり判定										 */
/* 戻り値	:TRUE:当たる													 */
/* 日付		:2008/04/29														 */
/* ========================================================================= */

BOOL CInfoCharBase::IsHitCharPos(
	int x,					/* [in] キャラ座標(横) */
	int y,					/* [in] キャラ座標(縦) */
	SIZE *psize/*NULL*/)	/* [in] キャラサイズ */
{
	BOOL bRet;
	int nPos;
	SIZE size;
	RECT rcSrc, rcTarget;

	bRet = FALSE;

	if (psize == NULL) {
		nPos = abs (x - m_nMapX);
		if (nPos != 0) {
			goto Exit;
		}
		nPos = y - m_nMapY;
		if (nPos != 0) {
			goto Exit;
		}

	} else {
		GetCharSize (size);
		SetRect (&rcSrc, m_nMapX, m_nMapY, m_nMapX + size.cx - 1, m_nMapY + size.cy - 1);
		SetRect (&rcTarget, x, y, x + psize->cx - 1, y + psize->cy - 1);
		if (!((rcSrc.left <= rcTarget.right) && (rcTarget.left <= rcSrc.right) &&
			(rcSrc.top <= rcTarget.bottom) && (rcTarget.top <= rcSrc.bottom))) {
				goto Exit;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetViewCharPos									 */
/* 内容		:表示用の座標補正値を取得										 */
/* 日付		:2008/05/01														 */
/* メモ		:表示する時に座標からずらす値を取得する							 */
/* ========================================================================= */

void CInfoCharBase::GetViewCharPos(POINT &ptDst)
{
	ptDst = m_ptViewCharPos;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::GetCharSize										 */
/* 内容		:キャラの幅と高さを取得											 */
/* 日付		:2008/05/01														 */
/* ========================================================================= */

void CInfoCharBase::GetCharSize(SIZE &sizeDst)
{
	sizeDst.cx = 2;
	sizeDst.cy = 1;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsEnableMove									 */
/* 内容		:移動できる状態か判定											 */
/* 日付		:2008/06/11														 */
/* 戻り値	:TRUE:移動可													 */
/* ========================================================================= */

BOOL CInfoCharBase::IsEnableMove(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_nProcState == CHARPROCSTATEID_FISHING) {
		goto Exit;
	}
	if (IsMove ()) {
		goto Exit;
	}
	/* ついていっている？ */
	if (m_dwFrontCharID) {
		goto Exit;
	}

	switch (m_nMoveState) {
	case CHARMOVESTATE_SWOON:				/* 気絶 */
	case CHARMOVESTATE_BATTLEATACK:			/* 戦闘攻撃中 */
	case CHARMOVESTATE_BATTLEATACK_WAIT:	/* 戦闘攻撃後の待ち時間 */
	case CHARMOVESTATE_BATTLE_DEFENSE:		/* 防御中 */
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsEnableAtack									 */
/* 内容		:攻撃できる状態か判定											 */
/* 日付		:2008/07/26														 */
/* 戻り値	:TRUE:攻撃可													 */
/* ========================================================================= */

BOOL CInfoCharBase::IsEnableAtack(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_dwHP == 0) {
		goto Exit;
	}
	switch (m_nMoveState) {
	case CHARMOVESTATE_SWOON:				/* 気絶 */
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsAtackTarget									 */
/* 内容		:攻撃対象となるか判定											 */
/* 日付		:2008/07/12														 */
/* 戻り値	:TRUE:攻撃可													 */
/* ========================================================================= */

BOOL CInfoCharBase::IsAtackTarget(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_dwHP == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::IsLogoutDelete									 */
/* 内容		:ログアウト時に削除するか判定									 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

BOOL CInfoCharBase::IsLogoutDelete(void)
{
	return m_bDelete;
}

/* Copyright(C)URARA-works 2006 */
