/// @file GrpLayout.h
/// @brief スプライトレイアウト定義（セルサイズ/セル数/リソース名パターン）の単一ソース
/// @author 年がら年中春うらら(URARA-works)
/// @date 2026/08/26
/// @copyright Copyright(C)URARA-works 2006
///
/// SboCli の CMgrGrpData::GetGrpSize / GetGrpCountX / GetGrpCountY と、
/// SboSvr の SpriteSheetHandler / ImageCatalogHandler が個別に持っていた
/// 同一内容のレイアウトテーブルを、このファイルに一本化する。
/// 値を変更する場合はここだけを直せばよい。

#pragma once

#include <cstddef>

// ---------------------------------------------------------------------------
// スプライトレイアウト定義レコード
// ---------------------------------------------------------------------------
struct SGrpLayoutDef
{
	int         nIDMain;               // GRPIDMAIN_* 値（0 = 列挙値なしの疑似カテゴリ）
	const char *pszKey;                 // "char" / "hairDown" / "mapParts" 等
	const char *pszLabel;               // 管理画面表示用ラベル（UTF-8 日本語）
	const char *pszResPattern;          // "IDP_HAIR_D%02d"（NULL なら ppszFixedNames を使用）
	const char *const *ppszFixedNames;  // 固定名配列（NULL 終端）。pszResPattern が NULL の時のみ参照
	int         nCellSize;              // 1セルのピクセル幅・高さ
	int         nCountX;                // 横セル数
	int         nCountY;                // 縦セル数
	int         nFirstResourceIndex;    // sheetIndex=0 に対応するリソース番号
	int         nHintMaxSub;            // 管理画面スピナーの上限目安（-1 = 未設定）
};

// テーブル全体を取得する（*pnCount にエントリ数を格納）
const SGrpLayoutDef *GrpLayout_GetTable(size_t *pnCount);

// key からエントリを検索する（見つからなければ NULL）
const SGrpLayoutDef *GrpLayout_FindByKey(const char *pszKey);

// GRPIDMAIN_* からエントリを検索する。
// nIDMain == 0 (GRPIDMAIN_NONE / 列挙値なしの疑似カテゴリの共有値) は
// 複数エントリが同じ値を共有するため、常に NULL を返す。
const SGrpLayoutDef *GrpLayout_FindByIDMain(int nIDMain);

// GRPIDMAIN_* からレイアウト値を取得する（未知なら 0 を返す）。
// CMgrGrpData::GetGrpSize / GetGrpCountX / GetGrpCountY の委譲先。
int GrpLayout_GetCellSize(int nIDMain);
int GrpLayout_GetCountX(int nIDMain);
int GrpLayout_GetCountY(int nIDMain);

// ---------------------------------------------------------------------------
// リソース名 → ファイル名 対応表
//
// SboCli の CMgrGrpData::GetFileNameForResource が持っていた
// s_map[]（SboGrpData.rc のリソース定義に基づくマッピング）を一本化する。
// ---------------------------------------------------------------------------

// リソース名（"IDP_HAIR_D01"）から SboGrpData/res/ 配下の相対ファイル名を取得する。
// 返る文字列は Windows 形式の区切り（例 "hair\\hair_d01.png"）。
// 非 Windows 環境で使う場合は呼び出し側でスラッシュに変換すること。
// 見つからなければ NULL を返す。
const char *GrpLayout_GetResFileName(const char *pszResName);

// 対応表を列挙する（S5 のマニフェスト生成などで使う想定）。
// *pnCount にエントリ数を格納し、先頭要素へのポインタを返す。
struct SGrpResFileEntry
{
	const char *pszResName;   // "IDP_HAIR_D01"
	const char *pszFileName;  // "hair\\hair_d01.png"
};
const SGrpResFileEntry *GrpLayout_GetResFileTable(size_t *pnCount);
