/// @file GrpLayout.cpp
/// @brief スプライトレイアウト定義（Common/GrpLayout.h）の実体
/// @author 年がら年中春うらら(URARA-works)
/// @date 2026/08/26
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "GrpLayout.h"

#include <cstring>

namespace
{

// キャラ体の固定リソース名（body/earの組、NULL終端）
const char *const kBodyNames[] = {
	"IDP_BODY_BST", "IDP_BODY_BST_EAR",
	"IDP_BODY_DRK", "IDP_BODY_DRK_EAR",
	"IDP_BODY_ELF", "IDP_BODY_ELF_EAR",
	"IDP_BODY_HUM", NULL,
	NULL
};

// 2x2 キャラ体の固定リソース名
const char *const k2x2BodyNames[] = {
	"IDP_2X2_BODY_HUM", NULL,
	NULL
};

// ---------------------------------------------------------------------------
// レイアウト定義テーブル
//
// 並び順は管理画面のカテゴリ表示順（旧 ImageCatalogHandler.cpp の
// kCategories[] の順序）を維持すること。
// ---------------------------------------------------------------------------
const SGrpLayoutDef kGrpLayoutTable[] = {
	// idMain                  key            label                resPattern                fixedNames     cell  X   Y   first hintMax
	{ GRPIDMAIN_NONE,          "none",        "未設定",             NULL,                     NULL,          0,    0,  0,  0,    0   },
	{ GRPIDMAIN_CHAR,          "char",        "キャラ",             NULL,                     kBodyNames,    16,   32, 1,  0,    63  },
	{ 0,                       "cloth",       "服",                 "IDP_CLOTH_%02d",         NULL,          16,   1,  32, 1,    63  },
	{ 0,                       "eye",         "目",                 "IDP_EYE_%02d",           NULL,          16,   1,  32, 1,    191 },
	{ 0,                       "hairDown",    "髪",                 "IDP_HAIR_D%02d",         NULL,          16,   1,  32, 1,    415 },
	{ 0,                       "hairUp",      "髪(上)",             "IDP_HAIR_U%02d",         NULL,          16,   1,  32, 1,    415 },
	{ 0,                       "spCloth",     "特殊服",             "IDP_CLOTH_SP%02d",       NULL,          16,   1,  32, 1,    31  },
	{ 0,                       "acce",        "アクセサリ",          "IDP_ACCE_%02d",          NULL,          16,   1,  32, 1,    63  },
	{ 0,                       "npcRow",      "NPC(行)",            "IDP_NPC_%02d",           NULL,          16,   1,  32, 1,    255 },
	{ GRPIDMAIN_WEAPON,        "weapon",      "武器",               "IDP_WEAPON_%02d",        NULL,          32,   32, 18, 1,    63  },
	{ GRPIDMAIN_WEAPON_BOW,    "weaponBow",   "武器(弓)",           "IDP_WEAPON_BOW_%02d",    NULL,          32,   20, 18, 1,    63  },
	{ GRPIDMAIN_EFFECT32,      "effect32",    "エフェクト(32)",     "IDP_EFC_32_%02d",        NULL,          32,   16, 16, 1,    63  },
	{ GRPIDMAIN_WEAPON_GLOVE,  "weaponGlove", "武器(打撃)",         "IDP_WEAPON_GLOVE_%02d",  NULL,          32,   20, 18, 1,    63  },
	{ GRPIDMAIN_WEAPON_ETC,    "weaponEtc",   "武器(その他)",       "IDP_WEAPON_ETC_%02d",    NULL,          32,   20, 18, 1,    63  },
	{ GRPIDMAIN_NPC,           "npc",         "NPC",                "IDP_NPC_%02d",           NULL,          16,   16, 32, 1,    255 },
	{ GRPIDMAIN_EFCBALLOON,    "efcBalloon",  "噴出し",             "IDP_BALLOON_%02d",       NULL,          16,   10, 15, 1,    63  },
	{ GRPIDMAIN_2X2_CHAR,      "char2x2",     "キャラ(2x2)",        NULL,                     k2x2BodyNames, 32,   16, 8,  0,    255 },
	{ GRPIDMAIN_2X2_CLOTH,     "cloth2x2",    "服(2x2)",            "IDP_2X2_CLOTH_%02d",     NULL,          32,   16, 8,  0,    255 },
	{ GRPIDMAIN_2X2_EYE,       "eye2x2",      "目(2x2)",            "IDP_2X2_EYE_%02d",       NULL,          32,   12, 8,  1,    63  },
	{ GRPIDMAIN_2X2_HAIR,      "hair2x2",     "髪(2x2)",            "IDP_2X2_HAIR_%02d",      NULL,          32,   16, 8,  1,    255 },
	{ GRPIDMAIN_2X2_SPCLOTH,   "spCloth2x2",  "特殊服(2x2)",        "IDP_2X2_SP_CLOTH_%02d",  NULL,          32,   16, 8,  1,    255 },
	{ GRPIDMAIN_2X2_SPHAIR,    "spHair2x2",   "特殊髪(2x2)",        "IDP_2X2_SP_HAIR_%02d",   NULL,          32,   16, 8,  1,    255 },
	{ GRPIDMAIN_2X2_ARMS,      "arms2x2",     "持ち物(2x2)",        "IDP_2X2_ARMS_%02d",      NULL,          24,   12, 25, 1,    255 },
	{ GRPIDMAIN_2X2_SHIELD,    "shield2x2",   "盾(2x2)",            "IDP_2X2_SHIELD_%02d",    NULL,          16,   5,  25, 1,    255 },
	{ GRPIDMAIN_2X2_ARMSSP,    "armsSp2x2",   "特殊持ち物(2x2)",    "IDP_2X2_ARMS_SP_%02d",   NULL,          24,   11, 25, 1,    255 },
	{ GRPIDMAIN_2X2_BOW,       "bow2x2",      "弓(2x2)",            "IDP_2X2_BOW_%02d",       NULL,          24,   11, 25, 1,    255 },
	{ GRPIDMAIN_2X2_NPC,       "npc2x2",      "NPC(2x2)",           "IDP_2X2_NPC_%03d",       NULL,          32,   16, 8,  1,    255 },
	{ GRPIDMAIN_EFFECT64,      "effect64",    "エフェクト(64)",     "IDP_EFC_64_%02d",        NULL,          64,   8,  8,  1,    63  },
	{ GRPIDMAIN_ICON32,        "icon32",      "アイコン(32)",       "IDP_ICON",               NULL,          16,   20, 20, 0,    63  },
	// マップ系: GRPIDMAIN_* に列挙値が無いため idMain = 0 を使用
	{ 0,                       "mapParts",    "マップパーツ",        "IDP_MAP_%02d",           NULL,          16,   32, 32, 1,    63  },
	{ 0,                       "mapShadow",   "マップ影",           "IDP_MAPSHADOW_%02d",     NULL,          16,   32, 32, 1,    63  },
	{ 0,                       "item",        "アイテム地面",        "IDP_ITEM_%02d",          NULL,          16,   32, 32, 1,    63  },
};

const size_t kGrpLayoutTableCount = sizeof(kGrpLayoutTable) / sizeof(kGrpLayoutTable[0]);

} // namespace

const SGrpLayoutDef *GrpLayout_GetTable(size_t *pnCount)
{
	if (pnCount != NULL) {
		*pnCount = kGrpLayoutTableCount;
	}
	return kGrpLayoutTable;
}

const SGrpLayoutDef *GrpLayout_FindByKey(const char *pszKey)
{
	if (pszKey == NULL) {
		return NULL;
	}
	for (size_t i = 0; i < kGrpLayoutTableCount; ++i) {
		if (strcmp(kGrpLayoutTable[i].pszKey, pszKey) == 0) {
			return &kGrpLayoutTable[i];
		}
	}
	return NULL;
}

const SGrpLayoutDef *GrpLayout_FindByIDMain(int nIDMain)
{
	// nIDMain == 0 (GRPIDMAIN_NONE) は複数の疑似カテゴリが共有する値のため、
	// 一意に決まらない。常に NULL を返す（呼び出し側は default 相当の 0 扱い）。
	if (nIDMain == 0) {
		return NULL;
	}
	for (size_t i = 0; i < kGrpLayoutTableCount; ++i) {
		if (kGrpLayoutTable[i].nIDMain == nIDMain) {
			return &kGrpLayoutTable[i];
		}
	}
	return NULL;
}

int GrpLayout_GetCellSize(int nIDMain)
{
	const SGrpLayoutDef *pDef = GrpLayout_FindByIDMain(nIDMain);
	return (pDef != NULL) ? pDef->nCellSize : 0;
}

int GrpLayout_GetCountX(int nIDMain)
{
	const SGrpLayoutDef *pDef = GrpLayout_FindByIDMain(nIDMain);
	return (pDef != NULL) ? pDef->nCountX : 0;
}

int GrpLayout_GetCountY(int nIDMain)
{
	const SGrpLayoutDef *pDef = GrpLayout_FindByIDMain(nIDMain);
	return (pDef != NULL) ? pDef->nCountY : 0;
}
