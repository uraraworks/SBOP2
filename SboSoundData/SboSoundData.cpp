/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SboSoundData.cpp											 */
/* 内容			:音関連DLL 実装ファイル										 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include <stdlib.h>
#include "resource.h"
#define SOUNDDATA_EXPORTS
#include "SboSoundData.h"

/* ========================================================================= */
/* 構造体の定義																 */
/* ========================================================================= */

/* 効果音情報 */
typedef struct _SOUNDDATAINFO {
	DWORD	dwSoundID;			/* 効果音ID */
	int		nResourceID;		/* リソースID */
	LPCSTR	pszSoundName;		/* 効果音名 */
} SOUNDDATAINFO, *PSOUNDDATAINFO;


/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

static SOUNDDATAINFO g_aSoundDataTbl[] = {
	SOUNDID_OK_PI73,			IDR_WAVE_PI73,			"決定音(pi73.wav)",
	SOUNDID_FOOT_ASI_2,			IDR_WAVE_ASI_2,			"足音(asi_2.wav)",
	SOUNDID_CURSORMOVE,			IDR_WAVE_SELECT,		"カーソル移動",
	SOUNDID_CANCEL,				IDR_WAVE_PUSH51_C,		"キャンセル",
	SOUNDID_CHARI14_A,			IDR_WAVE_CHARI14_A,		"戦闘開始",
	SOUNDID_SWING24_C,			IDR_WAVE_SWING24_C,		"武器振り",
	SOUNDID_SWING25_C,			IDR_WAVE_SWING25_C,		"武器突き",
	SOUNDID_HIT0,				IDR_WAVE_HIT0,			"ヒット0",
	SOUNDID_HIT1,				IDR_WAVE_HIT1,			"ヒット1",
	SOUNDID_DROP_MONEY,			IDR_WAVE_MONEY,			"ドロップ:お金",
	SOUNDID_DROP_ITEM_SMALL,	IDR_WAVE_ITEM_SMALL,	"ドロップ:小アイテム",
	SOUNDID_DROP_ITEM_MIDIUM,	IDR_WAVE_ITEM_MIDIUM,	"ドロップ:中アイテム",
	SOUNDID_DROP_ITEM_LARGE,	IDR_WAVE_ITEM_LARGE,	"ドロップ:大アイテム",
	SOUNDID_DROP_MATERIAL,		IDR_WAVE_MATERIAL,		"ドロップ:素材",
	SOUNDID_DROP_WEAPON,		IDR_WAVE_WEAPON,		"ドロップ:武器",
	SOUNDID_DROP_CLOTH,			IDR_WAVE_CLOTH,			"ドロップ:服・ローブ",
	SOUNDID_CRASH20_D,			IDR_WAVE_CRASH20_D,		"樽が壊れる音",
	SOUNDID_KUSA,				IDR_WAVE_KUSA,			"草を刈る音",
	SOUNDID_KAI,				IDR_WAVE_KAI,			"貝が壊れる音",
	SOUNDID_CHAT,				IDR_WAVE_CHAT,			"チャット",
	SOUNDID_W_CHAT,				IDR_WAVE_W_CHAT,		"アナウンス",
	SOUNDID_CLAP02,				IDR_WAVE_CLAP02,		"拍手2",
	SOUNDID_CLAP03,				IDR_WAVE_CLAP03,		"拍手3",
	SOUNDID_CLAP0203,			IDR_WAVE_CLAP02,		"拍手2か拍手3",
	SOUNDID_BALLOON1,			IDR_WAVE_BALLOON1,		"噴出し",
	SOUNDID_BIKKURI,			IDR_WAVE_BIKKURI,		"びっくり",
	SOUNDID_WHISTLES,			IDR_WAVE_WHISTLES,		"ホイッスルＳ",
	SOUNDID_WHISTLEL,			IDR_WAVE_WHISTLEL,		"ホイッスルＬ",
	SOUNDID_GIMON,				IDR_WAVE_GIMON,			"疑問",
	SOUNDID_NIKU,				IDR_WAVE_NIKU,			"肉",
	SOUNDID_KIZUKU,				IDR_WAVE_KIZUKU,		"気付く",
	SOUNDID_PINPONPANPON,		IDR_PINPONPANPON,		"放送前のお知らせ音",
	SOUNDID_SWOON,				IDR_WAVE_SWOON,			"気絶",
	SOUNDID_URARA_WORKS,		IDR_WAVE_URARA_WORKS,	"ウララワークス",
	SOUNDID_PUYON_ATACK,		IDR_WAVE_PUYON_ATACK,	"プヨン攻撃",
	SOUNDID_LIQUID,				IDR_WAVE_LIQUID,		"液体",
	SOUNDID_BOW_ATACK,			IDR_WAVE_BOW_ATACK,		"弓攻撃",
	SOUNDID_MEAL,				IDR_WAVE_MEAL,			"食事",
	SOUNDID_DEFENSE,			IDR_WAVE_DEFENSE,		"防御",
	SOUNDID_SHIELD,				IDR_WAVE_SHIELD,		"盾構え",
	SOUNDID_PUYON_MOVE,			IDR_WAVE_PUYON_MOVE,	"プヨン移動",
	SOUNDID_PUYON_DEAD,			IDR_WAVE_PUYON_DEAD,	"プヨン死亡",
	SOUNDID_EXIST,				IDR_WAVE_EXIST,			"消滅",
	SOUNDID_TRASH,				IDR_WAVE_TRASH,			"ゴミ箱開く・捨てる",
	SOUNDID_DROP_BIN,			IDR_WAVE_DROP_BIN,		"ドロップ：ビン",
	SOUNDID_OPEN_BAG,			IDR_WAVE_OPEN_BAG,		"バッグ開く",
	SOUNDID_MISS,				IDR_WAVE_MISS,			"ミス",
	SOUNDID_LVUP,				IDR_WAVE_LVUP,			"レベルアップ",
	SOUNDID_CRITICAL1,			IDR_WAVE_CRITICAL1,		"斬撃クリティカル",
	SOUNDID_HIT2,				IDR_WAVE_HIT2,			"斬撃ヒット１",
	SOUNDID_HIT3,				IDR_WAVE_HIT3,			"斬撃ヒット２",
	SOUNDID_OPEN_WINDOW,		IDR_WAVE_OPEN_WINDOW,	"窓開く",
	SOUNDID_CRITICAL2,			IDR_WAVE_CRITICAL2,		"打撃クリティカル",
	SOUNDID_CRITICAL3,			IDR_WAVE_CRITICAL3,		"矢クリティカル",
	SOUNDID_HIT4,				IDR_WAVE_HIT4,			"矢ヒット",
	SOUNDID_CHARGE,				IDR_WAVE_CHARGE,		"溜め",
	SOUNDID_CHARGE2,			IDR_WAVE_CHARGE2,		"溜め２",
	SOUNDID_DR_BASS01,			IDR_WAVE_DR_BASS01,		"ベース１",
	SOUNDID_DR_BASS02,			IDR_WAVE_DR_BASS02,		"ベース２",
	SOUNDID_DR_BASS05,			IDR_WAVE_DR_BASS05,		"ベース５",
	SOUNDID_DR_CLAP01,			IDR_WAVE_DR_CLAP01,		"クラップ１",
	SOUNDID_DR_SNE02,			IDR_WAVE_DR_SNE02,		"スネア２",
	SOUNDID_KIE_ATACK,			IDR_WAVE_KIE_ATACK,		"キー攻撃",
	SOUNDID_KIE_DEAD,			IDR_WAVE_KIE_DEAD,		"キー死亡",
	SOUNDID_FLY,				IDR_WAVE_FLY,			"羽ばたき",
	SOUNDID_STEP03,				IDR_WAVE_STEP03,		"階段移動",
	0, 0, ""
};


/* ========================================================================= */
/* グローバル変数の定義														 */
/* ========================================================================= */

static int	g_nSoundDataCount;							/* 効果音数 */


/* ========================================================================= */
/* 関数の定義																 */
/* ========================================================================= */

SOUNDDATA_API	int		GetSoundCount		(void);					/* 効果音数を取得 */
SOUNDDATA_API	int		GetSoundResourceID	(int nNo);				/* 効果音のリソースIDを取得 */
SOUNDDATA_API	int		GetSoundNo			(DWORD dwSoundID);		/* 効果音のインデックスを取得 */
SOUNDDATA_API	LPCSTR	GetSoundName		(DWORD dwSoundID);		/* 効果音名を取得 */
static			void	Init				(void);					/* 初期化 */
static			PSOUNDDATAINFO	GetSoundDataPtr	(DWORD dwSoundID);	/* 効果音情報を取得 */


/* ========================================================================= */
/* 関数名	:DllMain														 */
/* 内容		:DLLメイン処理													 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	Init ();

    return TRUE;
}


/* ========================================================================= */
/* 関数名	:GetSoundCount													 */
/* 内容		:効果音数を取得													 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

SOUNDDATA_API int GetSoundCount(void)
{
	return g_nSoundDataCount;
}


/* ========================================================================= */
/* 関数名	:GetSoundResourceID												 */
/* 内容		:効果音のリソースIDを取得										 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

SOUNDDATA_API int GetSoundResourceID(int nNo)
{
	int nRet;
	PSOUNDDATAINFO pSoundData;

	nRet = 0;

	if (nNo >= g_nSoundDataCount) {
		goto Exit;
	}
	pSoundData = &g_aSoundDataTbl[nNo];

	nRet = pSoundData->nResourceID;
Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:GetSoundID														 */
/* 内容		:効果音IDを取得を取得											 */
/* 日付		:2007/09/19														 */
/* ========================================================================= */

SOUNDDATA_API DWORD GetSoundID(int nNo)
{
	DWORD dwRet;
	PSOUNDDATAINFO pSoundData;

	dwRet = 0;

	if (nNo >= g_nSoundDataCount) {
		goto Exit;
	}
	pSoundData = &g_aSoundDataTbl[nNo];

	dwRet = pSoundData->dwSoundID;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:GetSoundNo														 */
/* 内容		:効果音のインデックスを取得										 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

SOUNDDATA_API int GetSoundNo(DWORD dwSoundID)
{
	int i, nRet;
	PSOUNDDATAINFO pSoundData;

	nRet = -1;

	if (dwSoundID == SOUNDID_CLAP0203) {
		dwSoundID = ((rand () % 100) > 50) ? SOUNDID_CLAP02 : SOUNDID_CLAP03;
	}

	for (i = 0; i < g_nSoundDataCount; i ++) {
		pSoundData = &g_aSoundDataTbl[i];
		if (pSoundData->dwSoundID != dwSoundID) {
			continue;
		}
		nRet = i;
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:GetSoundName													 */
/* 内容		:効果音名を取得													 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

SOUNDDATA_API LPCSTR GetSoundName(DWORD dwSoundID)
{
	LPCSTR pszRet;
	PSOUNDDATAINFO pSoundData;

	pszRet = "";

	pSoundData = GetSoundDataPtr (dwSoundID);
	if (pSoundData == NULL) {
		goto Exit;
	}

	pszRet = pSoundData->pszSoundName;
Exit:
	return pszRet;
}


/* ========================================================================= */
/* 関数名	:Init															 */
/* 内容		:初期化															 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

void Init(void)
{
	int i;
	PSOUNDDATAINFO pSoundData;

	/* 効果音数を求める */
	g_nSoundDataCount = 0;
	for (i = 0; ; i ++) {
		pSoundData = &g_aSoundDataTbl[i];
		if (pSoundData->dwSoundID == 0) {
			break;
		}
		g_nSoundDataCount ++;
	}
}


/* ========================================================================= */
/* 関数名	:GetSoundDataPtr												 */
/* 内容		:効果音情報を取得												 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

PSOUNDDATAINFO GetSoundDataPtr(DWORD dwSoundID)
{
	int i;
	PSOUNDDATAINFO pRet, pSoundData;

	pRet = NULL;

	/* 効果音数を求める */
	for (i = 0; i < g_nSoundDataCount; i ++) {
		pSoundData = &g_aSoundDataTbl[i];
		if (pSoundData->dwSoundID != dwSoundID) {
			continue;
		}
		pRet = pSoundData;
		break;
	}

	return pRet;
}

/* Copyright(C)URARA-works 2007 */
