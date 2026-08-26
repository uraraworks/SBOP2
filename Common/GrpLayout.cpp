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

// ---------------------------------------------------------------------------
// リソース名 → ファイル名 対応表
//
// SboGrpData.rc のリソース定義に基づくマッピング。
// 旧 SboCli/src/MgrGrpData.cpp の GetFileNameForResource() 内 s_map[] を
// そのまま転記したもの（順序・内容とも変更なし）。
// ---------------------------------------------------------------------------
const SGrpResFileEntry kGrpResFileTable[] = {
		{ "IDP_MAP_01",             "map01.png" },
		{ "IDP_MAP_02",             "map02.png" },
		{ "IDP_MAP_03",             "map03.png" },
		{ "IDP_SYSTEM",             "system.png" },
		{ "IDP_LOGO",               "URARA-works-logo.png" },
		{ "IDP_TITLE_BACK",         "title_back.png" },
		{ "IDP_TITLE",              "title.png" },
		{ "IDP_BODY_HUM",           "body\\hum.png" },
		{ "IDP_BODY_BST",           "body\\bst.png" },
		{ "IDP_BODY_BST_EAR",       "body\\bst_ear.png" },
		{ "IDP_BODY_DRK",           "body\\drk.png" },
		{ "IDP_BODY_DRK_EAR",       "body\\drk_ear.png" },
		{ "IDP_BODY_ELF",           "body\\elf.png" },
		{ "IDP_BODY_ELF_EAR",       "body\\elf_ear.png" },
		{ "IDP_HAIR_D01",           "hair\\hair_d01.png" },
		{ "IDP_HAIR_D02",           "hair\\hair_d02.png" },
		{ "IDP_HAIR_D03",           "hair\\hair_d03.png" },
		{ "IDP_HAIR_D04",           "hair\\hair_d04.png" },
		{ "IDP_HAIR_D05",           "hair\\hair_d05.png" },
		{ "IDP_HAIR_D06",           "hair\\hair_d06.png" },
		{ "IDP_HAIR_D07",           "hair\\hair_d07.png" },
		{ "IDP_HAIR_D08",           "hair\\hair_d08.png" },
		{ "IDP_HAIR_D09",           "hair\\hair_d09.png" },
		{ "IDP_HAIR_D10",           "hair\\hair_d10.png" },
		{ "IDP_HAIR_D11",           "hair\\hair_d11.png" },
		{ "IDP_HAIR_D12",           "hair\\hair_d12.png" },
		{ "IDP_HAIR_D13",           "hair\\hair_d13.png" },
		{ "IDP_HAIR_U01",           "hair\\hair_u01.png" },
		{ "IDP_HAIR_U02",           "hair\\hair_u02.png" },
		{ "IDP_HAIR_U03",           "hair\\hair_u03.png" },
		{ "IDP_HAIR_U04",           "hair\\hair_u04.png" },
		{ "IDP_HAIR_U05",           "hair\\hair_u05.png" },
		{ "IDP_HAIR_U06",           "hair\\hair_u06.png" },
		{ "IDP_HAIR_U07",           "hair\\hair_u07.png" },
		{ "IDP_HAIR_U08",           "hair\\hair_u08.png" },
		{ "IDP_HAIR_U09",           "hair\\hair_u09.png" },
		{ "IDP_HAIR_U10",           "hair\\hair_u10.png" },
		{ "IDP_HAIR_U11",           "hair\\hair_u11.png" },
		{ "IDP_HAIR_U12",           "hair\\hair_u12.png" },
		{ "IDP_HAIR_U13",           "hair\\hair_u13.png" },
		{ "IDP_CLOTH_01",           "clothes\\cloth01.png" },
		{ "IDP_CLOTH_02",           "clothes\\cloth02.png" },
		{ "IDP_CLOTH_SP01",         "clothes\\sp01.png" },
		{ "IDP_EYE_01",             "eyes\\eye01.png" },
		{ "IDP_EYE_02",             "eyes\\eye02.png" },
		{ "IDP_EYE_03",             "eyes\\eye03.png" },
		{ "IDP_EYE_04",             "eyes\\eye04.png" },
		{ "IDP_EYE_05",             "eyes\\eye05.png" },
		{ "IDP_EYE_06",             "eyes\\eye06.png" },
		{ "IDP_CHARSELECT_BACK",    "char_back.png" },
		{ "IDP_SHADOW",             "shadow.png" },
		{ "IDP_MAPSHADOW_01",       "map_shadow01.png" },
		{ "IDP_ACCE_01",            "acce01.png" },
		{ "IDP_ACCE_02",            "acce02.png" },
		{ "IDP_ITEM_01",            "item.png" },
		{ "IDP_MARK",               "mark.png" },
		{ "IDP_WEAPON_01",          "weapons\\weapon01.png" },
		{ "IDP_WEAPON_02",          "weapons\\weapon02.png" },
		{ "IDP_WEAPON_BOW_01",      "weapons\\weapon_bow01.png" },
		{ "IDP_WEAPON_ARROW_01",    "weapons\\weapon_arrow01.png" },
		{ "IDP_WEAPON_GLOVE_01",    "weapons\\weapon_glove01.png" },
		{ "IDP_WEAPON_ETC_01",      "weapons\\weapon_etc01.png" },
		{ "IDP_BALLOON_01",         "balloon_01.png" },
		{ "IDP_NPC_01",             "NPC\\npc01.png" },
		{ "IDP_NPC_02",             "NPC\\npc02.png" },
		{ "IDP_TITLE_CLOUD",        "title_cloud.png" },
		{ "IDP_ICON",               "icon.png" },
		{ "IDP_2X2_BODY_HUM",       "2x2\\2x2_hum.png" },
		{ "IDP_2X2_HUM_HAIR_U01",   "2x2\\2x2_hum_hair_u01.png" },
		{ "IDP_2X2_EYE_01",         "2x2\\2x2_eye01.png" },
		{ "IDP_2X2_EYE_02",         "2x2\\2x2_eye02.png" },
		{ "IDP_2X2_EYE_03",         "2x2\\2x2_eye03.png" },
		{ "IDP_2X2_EYE_04",         "2x2\\2x2_eye04.png" },
		{ "IDP_2X2_EYE_05",         "2x2\\2x2_eye05.png" },
		{ "IDP_2X2_CLOTH_00",       "2x2\\2x2_cloth00.png" },
		{ "IDP_2X2_CLOTH_01",       "2x2\\2x2_cloth01.png" },
		{ "IDP_2X2_CLOTH_02",       "2x2\\2x2_cloth02.png" },
		{ "IDP_2X2_CLOTH_03",       "2x2\\2x2_cloth03.png" },
		{ "IDP_2X2_HAIR_01",        "2x2\\2x2_hair01.png" },
		{ "IDP_2X2_HAIR_02",        "2x2\\2x2_hair02.png" },
		{ "IDP_2X2_HAIR_03",        "2x2\\2x2_hair03.png" },
		{ "IDP_2X2_HAIR_04",        "2x2\\2x2_hair04.png" },
		{ "IDP_2X2_SP_CLOTH_01",    "2x2\\2x2_sp_cloth01.png" },
		{ "IDP_2X2_SP_CLOTH_02",    "2x2\\2x2_sp_cloth02.png" },
		{ "IDP_2X2_SP_HAIR_01",     "2x2\\2x2_sp_hair01.png" },
		{ "IDP_2X2_SP_HAIR_02",     "2x2\\2x2_sp_hair02.png" },
		{ "IDP_2X2_ARMS_01",        "2x2\\2x2_arms01.png" },
		{ "IDP_2X2_ARMS_02",        "2x2\\2x2_arms02.png" },
		{ "IDP_2X2_ARMS_03",        "2x2\\2x2_arms03.png" },
		{ "IDP_2X2_ARMS_SP_01",     "2x2\\2x2_arms_sp01.png" },
		{ "IDP_2X2_SHIELD_01",      "2x2\\2x2_shield01.png" },
		{ "IDP_2X2_BOW_01",         "2x2\\2x2_bow01.png" },
		{ "IDP_2X2_NPC_001",        "2x2\\2x2_npc001.png" },
		{ "IDP_2X2_NPC_002",        "2x2\\2x2_npc002.png" },
		{ "IDP_2X2_NPC_003",        "2x2\\2x2_npc003.png" },
		{ "IDP_2X2_NPC_004",        "2x2\\2x2_npc004.png" },
		{ "IDP_2X2_NPC_005",        "2x2\\2x2_npc005.png" },
		{ "IDP_2X2_NPC_006",        "2x2\\2x2_npc006.png" },
		{ "IDP_2X2_NPC_001_SHADOW", "2x2\\2x2_npc001_shadow.png" },
		{ "IDP_2X2_NPC_002_SHADOW", "2x2\\2x2_npc002_shadow.png" },
		{ "IDP_2X2_NPC_003_SHADOW", "2x2\\2x2_npc003_shadow.png" },
		{ "IDP_2X2_NPC_004_SHADOW", "2x2\\2x2_npc004_shadow.png" },
		{ "IDP_2X2_NPC_005_SHADOW", "2x2\\2x2_npc005_shadow.png" },
		{ "IDP_2X2_NPC_006_SHADOW", "2x2\\2x2_npc006_shadow.png" },
		{ "IDP_2X2_CHAR_SHADOW_01", "2x2\\2x2_char_shadow_01.png" },
		{ "IDP_NUM_S",              "num_s.png" },
		{ "IDP_NUM_M",              "num_m.png" },
		{ "IDP_NUM_L",              "num_l.png" },
		{ "IDP_EFC_32_01",          "efc_32_01.png" },
		{ "IDP_EFC_64_01",          "efc_64_01.png" },
};

const size_t kGrpResFileTableCount = sizeof(kGrpResFileTable) / sizeof(kGrpResFileTable[0]);

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

const char *GrpLayout_GetResFileName(const char *pszResName)
{
	if (pszResName == NULL) {
		return NULL;
	}
	for (size_t i = 0; i < kGrpResFileTableCount; ++i) {
		if (strcmp(kGrpResFileTable[i].pszResName, pszResName) == 0) {
			return kGrpResFileTable[i].pszFileName;
		}
	}
	return NULL;
}

const SGrpResFileEntry *GrpLayout_GetResFileTable(size_t *pnCount)
{
	if (pnCount != NULL) {
		*pnCount = kGrpResFileTableCount;
	}
	return kGrpResFileTable;
}
