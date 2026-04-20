/// @file SaveLoadInfoMapParts.cpp
/// @brief データ保存・読み込みクラス(マップパーツ情報) 実装ファイル
///        MapParts ライブラリを SQLite 正規化テーブル (sys_map_parts) に移行
///        サブ配列 m_paAnimeInfo → sys_map_parts_anime (PartsID, Slot, Wait, Level, GrpIDBase, GrpIDPile)
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoMapParts.h"
#include "LibInfoMapParts.h"
#include "SaveLoadInfoMapParts.h"

// テーブル名
static const char* s_pszTableMain  = "sys_map_parts";
static const char* s_pszTableAnime = "sys_map_parts_anime";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoMapParts::CSaveLoadInfoMapParts()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataMapParts.dat");
	SetName("MapParts");
}

CSaveLoadInfoMapParts::~CSaveLoadInfoMapParts()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   メインテーブル (sys_map_parts) と
//   アニメコマ サブテーブル (sys_map_parts_anime) を同時に作成する
// ============================================================
void CSaveLoadInfoMapParts::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// メインテーブル: マップパーツ基本情報（アニメコマ以外）
	//   カラム順は InfoMapParts.cpp の保存フィールドに準拠
	//   ViewType, AnimeType, AnimeCount, Level, BlockDirection, MoveDirection,
	//   GrpIDBase, GrpIDPile, PartsID(PK), PartsType, ViewPosX, ViewPosY
	//   ※ CInfoMapParts には m_strName フィールドが存在しないため Name カラムなし
	const char* pszMainSql =
		"CREATE TABLE IF NOT EXISTS sys_map_parts("
		"  PartsID         INTEGER PRIMARY KEY,"	// m_dwPartsID
		"  ViewType        INTEGER,"				// m_byViewType
		"  AnimeType       INTEGER,"				// m_byAnimeType
		"  AnimeCount      INTEGER,"				// m_byAnimeCount
		"  Level           INTEGER,"				// m_byLevel (透明度)
		"  BlockDirection  INTEGER,"				// m_byBlockDirection
		"  MoveDirection   INTEGER,"				// m_byMoveDirection
		"  GrpIDBase       INTEGER,"				// m_wGrpIDBase
		"  GrpIDPile       INTEGER,"				// m_wGrpIDPile
		"  PartsType       INTEGER,"				// m_dwPartsType
		"  ViewPosX        INTEGER,"				// m_ptViewPos.x
		"  ViewPosY        INTEGER"				// m_ptViewPos.y
		");";

	sqlite3_exec(s_pDb, pszMainSql, NULL, NULL, NULL);

	// サブテーブル: アニメーションコマ情報 (m_paAnimeInfo の可変長配列)
	//   PartsID + Slot を複合主キーとし、配列インデックスを Slot に格納する
	const char* pszAnimeSql =
		"CREATE TABLE IF NOT EXISTS sys_map_parts_anime("
		"  PartsID   INTEGER NOT NULL,"
		"  Slot      INTEGER NOT NULL,"
		"  Wait      INTEGER,"				// m_byWait  (×１０ミリ秒)
		"  Level     INTEGER,"				// m_byLevel (透明度)
		"  GrpIDBase INTEGER,"				// m_wGrpIDBase
		"  GrpIDPile INTEGER,"				// m_wGrpIDPile
		"  PRIMARY KEY (PartsID, Slot)"
		");";

	sqlite3_exec(s_pDb, pszAnimeSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全マップパーツ情報を正規化テーブルに全行置換
//   DELETE + INSERT ループ（外側トランザクションに乗る）
//   アニメサブテーブルも同様に DELETE して再 INSERT する
// ============================================================
void CSaveLoadInfoMapParts::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去（サブテーブルも先に消す）
	sqlite3_exec(s_pDb, "DELETE FROM sys_map_parts;",       NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_map_parts_anime;", NULL, NULL, NULL);

	// メインテーブル INSERT 文
	const char* pszInsertMain =
		"INSERT INTO sys_map_parts("
		"  PartsID, ViewType, AnimeType, AnimeCount, Level, BlockDirection, MoveDirection,"
		"  GrpIDBase, GrpIDPile, PartsType, ViewPosX, ViewPosY"
		") VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsertMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMapParts::SaveToNormalTable: prepare(main) failed\n");
		return;
	}

	// アニメサブテーブル INSERT 文
	const char* pszInsertAnime =
		"INSERT INTO sys_map_parts_anime(PartsID, Slot, Wait, Level, GrpIDBase, GrpIDPile)"
		" VALUES(?, ?, ?, ?, ?, ?);";

	sqlite3_stmt* pStmtAnime = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszInsertAnime, -1, &pStmtAnime, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMapParts::SaveToNormalTable: prepare(anime) failed\n");
		sqlite3_finalize(pStmtMain);
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoMapParts pInfo = (PCInfoMapParts)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// ---- メインテーブルへの INSERT ----
		sqlite3_bind_int(pStmtMain,  1, (int)pInfo->m_dwPartsID);
		sqlite3_bind_int(pStmtMain,  2, (int)pInfo->m_byViewType);
		sqlite3_bind_int(pStmtMain,  3, (int)pInfo->m_byAnimeType);
		sqlite3_bind_int(pStmtMain,  4, (int)pInfo->m_byAnimeCount);
		sqlite3_bind_int(pStmtMain,  5, (int)pInfo->m_byLevel);
		sqlite3_bind_int(pStmtMain,  6, (int)pInfo->m_byBlockDirection);
		sqlite3_bind_int(pStmtMain,  7, (int)pInfo->m_byMoveDirection);
		sqlite3_bind_int(pStmtMain,  8, (int)pInfo->m_wGrpIDBase);
		sqlite3_bind_int(pStmtMain,  9, (int)pInfo->m_wGrpIDPile);
		sqlite3_bind_int(pStmtMain, 10, (int)pInfo->m_dwPartsType);
		sqlite3_bind_int(pStmtMain, 11, (int)pInfo->m_ptViewPos.x);
		sqlite3_bind_int(pStmtMain, 12, (int)pInfo->m_ptViewPos.y);

		sqlite3_step(pStmtMain);
		sqlite3_reset(pStmtMain);

		// ---- サブテーブルへの INSERT: アニメーションコマ ----
		//   Slot = 配列インデックス (0-based) をそのまま使う
		int nAnimeCount = pInfo->GetAnimeCount();
		for (int j = 0; j < nAnimeCount; j++) {
			PCInfoAnime pAnime = pInfo->GetAnimePtr(j);
			if (pAnime == NULL) continue;

			sqlite3_bind_int(pStmtAnime, 1, (int)pInfo->m_dwPartsID);
			sqlite3_bind_int(pStmtAnime, 2, j);
			sqlite3_bind_int(pStmtAnime, 3, (int)pAnime->m_byWait);
			sqlite3_bind_int(pStmtAnime, 4, (int)pAnime->m_byLevel);
			sqlite3_bind_int(pStmtAnime, 5, (int)pAnime->m_wGrpIDBase);
			sqlite3_bind_int(pStmtAnime, 6, (int)pAnime->m_wGrpIDPile);

			sqlite3_step(pStmtAnime);
			sqlite3_reset(pStmtAnime);
		}
	}

	sqlite3_finalize(pStmtAnime);
	sqlite3_finalize(pStmtMain);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
//   メインテーブルを全行 SELECT し、各パーツごとにサブテーブルを
//   prepared statement を reset して再利用しながら取得する
// ============================================================
BOOL CSaveLoadInfoMapParts::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	// メインテーブル SELECT
	const char* pszSelectMain =
		"SELECT PartsID, ViewType, AnimeType, AnimeCount, Level, BlockDirection, MoveDirection,"
		"       GrpIDBase, GrpIDPile, PartsType, ViewPosX, ViewPosY"
		" FROM sys_map_parts;";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSelectMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	// アニメサブテーブル SELECT
	//   ORDER BY Slot を必ず付けて配列インデックス順を保証する
	const char* pszSelectAnime =
		"SELECT Slot, Wait, Level, GrpIDBase, GrpIDPile"
		" FROM sys_map_parts_anime WHERE PartsID=? ORDER BY Slot;";

	sqlite3_stmt* pStmtAnime = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszSelectAnime, -1, &pStmtAnime, NULL);
	if (nRet != SQLITE_OK) {
		sqlite3_finalize(pStmtMain);
		return FALSE;
	}

	int nRowCount = 0;
	while (sqlite3_step(pStmtMain) == SQLITE_ROW) {
		// 新規 CInfoMapParts を作成
		PCInfoMapParts pInfo = (PCInfoMapParts)pDst->GetNew();
		if (pInfo == NULL) continue;

		// PartsID を事前にセットしてから Add() を呼ぶ
		//   ← Add() 内で m_dwPartsID == 0 なら GetNewID() で上書きされてしまうため
		DWORD dwPartsID = (DWORD)sqlite3_column_int(pStmtMain, 0);
		pInfo->m_dwPartsID = dwPartsID;

		pInfo->m_byViewType       = (BYTE)sqlite3_column_int(pStmtMain, 1);
		pInfo->m_byAnimeType      = (BYTE)sqlite3_column_int(pStmtMain, 2);
		// AnimeCount は AddAnime() で自動更新されるため直接セットしない
		pInfo->m_byLevel          = (BYTE)sqlite3_column_int(pStmtMain, 4);
		pInfo->m_byBlockDirection = (BYTE)sqlite3_column_int(pStmtMain, 5);
		pInfo->m_byMoveDirection  = (BYTE)sqlite3_column_int(pStmtMain, 6);
		pInfo->m_wGrpIDBase       = (WORD)sqlite3_column_int(pStmtMain, 7);
		pInfo->m_wGrpIDPile       = (WORD)sqlite3_column_int(pStmtMain, 8);
		pInfo->m_dwPartsType      = (DWORD)sqlite3_column_int(pStmtMain, 9);
		pInfo->m_ptViewPos.x      = (LONG)sqlite3_column_int(pStmtMain, 10);
		pInfo->m_ptViewPos.y      = (LONG)sqlite3_column_int(pStmtMain, 11);

		// ---- サブテーブルからアニメーションコマを復元 ----
		//   prepared statement を bind + reset で再利用
		pInfo->DeleteAllAnime();
		sqlite3_bind_int(pStmtAnime, 1, (int)dwPartsID);
		while (sqlite3_step(pStmtAnime) == SQLITE_ROW) {
			// AddAnime() で空コマを追加してからフィールドを上書き
			pInfo->AddAnime();
			int nSlot = pInfo->GetAnimeCount() - 1;
			PCInfoAnime pAnime = pInfo->GetAnimePtr(nSlot);
			if (pAnime == NULL) continue;

			pAnime->m_byWait     = (BYTE)sqlite3_column_int(pStmtAnime, 1);
			pAnime->m_byLevel    = (BYTE)sqlite3_column_int(pStmtAnime, 2);
			pAnime->m_wGrpIDBase = (WORD)sqlite3_column_int(pStmtAnime, 3);
			pAnime->m_wGrpIDPile = (WORD)sqlite3_column_int(pStmtAnime, 4);
		}
		sqlite3_reset(pStmtAnime);	// 次のパーツのために reset

		// ライブラリに追加（m_dwPartsID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmtAnime);
	sqlite3_finalize(pStmtMain);

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoMapParts::MigrateFromBlob(PCLibInfoBase pDst)
{
	// 親クラスの Load() を呼んで BLOB / .dat を読む
	CSaveLoadInfoBase::Load(pDst);

	// 読み込み結果を確認
	if (pDst->GetCount() <= 0) {
		// BLOB にも行がなかった（新規データなし）
		return FALSE;
	}

	// 正規化テーブルへ書き込み（マイグレーション）
	m_pLibInfoBase = pDst;
	SaveToNormalTable();

	// sbo_data から 'MapParts' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='MapParts';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoMapParts: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoMapParts::Save(PCLibInfoBase pSrc)
{
	if (pSrc == NULL) return;

	m_pLibInfoBase = pSrc;

	// テーブルが無ければ作成
	EnsureTable();

	// 正規化テーブルへ保存（sbo_data には書かない）
	SaveToNormalTable();
}

// ============================================================
// Load のオーバーライド: 正規化テーブルから読む（マイグレーション付き）
// ============================================================
void CSaveLoadInfoMapParts::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoMapParts: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoMapParts: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
