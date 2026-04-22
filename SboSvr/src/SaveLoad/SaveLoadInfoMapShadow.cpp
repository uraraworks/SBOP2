/// @file SaveLoadInfoMapShadow.cpp
/// @brief データ保存・読み込みクラス(マップ影情報) 実装ファイル
///        MapShadow ライブラリを SQLite 正規化テーブル (sys_map_shadow / sys_map_shadow_anime) に移行
///
/// テーブル構成:
///   sys_map_shadow       ─ マップ影基本情報（スカラーフィールド + POINT 展開）
///   sys_map_shadow_anime ─ アニメーションコマ m_paAnimeInfo の可変長配列
///                           (ShadowID, Slot, Wait, Level, GrpIDBase, GrpIDPile) の 6 カラム構成
///
/// BLOB 折衷なし: m_paAnimeInfo は CInfoAnime の可変長配列（要素数は最大 255）なので
///                完全正規化でサブテーブルに展開する。
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/06
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoMapShadow.h"
#include "InfoAnime.h"
#include "LibInfoMapShadow.h"
#include "SaveLoadInfoMapShadow.h"

// テーブル名
static const char* s_pszTableMain = "sys_map_shadow";
static const char* s_pszTableAnime = "sys_map_shadow_anime";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoMapShadow::CSaveLoadInfoMapShadow()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataMapShadow.dat");
	SetName("MapShadow");
}

CSaveLoadInfoMapShadow::~CSaveLoadInfoMapShadow()
{
}

void CSaveLoadInfoMapShadow::SetHeaderInfo(PCInfoBase pInfo)
{
	int i;
	LPCSTR pszName;
	CStringA strHeader;
	CInfoAnime AnimeTmp;

	CSaveLoadInfoBase::SetHeaderInfo(pInfo);

	for (i = 0; ; i++) {
		pszName = AnimeTmp.GetName(i);
		if (pszName == NULL) {
			break;
		}
		strHeader.Format("%s%s", PREFIX_INFOANIME, pszName);
		AddHeaderInfo(strHeader);
	}
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   sys_map_shadow:
//     ShadowID を主キーとする基本情報テーブル
//     POINT (m_ptViewPos) は ViewPosX / ViewPosY の 2 カラムに展開
//   sys_map_shadow_anime:
//     (ShadowID, Slot) を複合主キーとするアニメーションコマテーブル
// ============================================================
void CSaveLoadInfoMapShadow::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// メインテーブル
	const char* pszMainSql =
		"CREATE TABLE IF NOT EXISTS sys_map_shadow("
		"  ShadowID   INTEGER PRIMARY KEY,"	// m_dwShadowID
		"  ViewType   INTEGER,"				// m_byViewType  (表示種別)
		"  AnimeType  INTEGER,"				// m_byAnimeType (アニメーション種別)
		"  AnimeCount INTEGER,"				// m_byAnimeCount (アニメーションコマ数)
		"  Level      INTEGER,"				// m_byLevel (透明度)
		"  Light      INTEGER,"				// m_bLight (透明度を明度として使う)
		"  GrpID      INTEGER,"				// m_wGrpID (グラフィックID)
		"  ViewPosX   INTEGER,"				// m_ptViewPos.x
		"  ViewPosY   INTEGER"				// m_ptViewPos.y
		");";

	sqlite3_exec(s_pDb, pszMainSql, NULL, NULL, NULL);

	// サブテーブル: アニメーションコマ (m_paAnimeInfo の可変長配列)
	//   ShadowID + Slot を複合主キーとし、配列インデックスを Slot に格納
	const char* pszAnimeSql =
		"CREATE TABLE IF NOT EXISTS sys_map_shadow_anime("
		"  ShadowID  INTEGER NOT NULL,"
		"  Slot      INTEGER NOT NULL,"
		"  Wait      INTEGER,"				// m_byWait  (待ち時間 ×１０ミリ秒)
		"  Level     INTEGER,"				// m_byLevel (透明度)
		"  GrpIDBase INTEGER,"				// m_wGrpIDBase (グラフィックID 下地)
		"  GrpIDPile INTEGER,"				// m_wGrpIDPile (グラフィックID 重ね合わせ用)
		"  PRIMARY KEY (ShadowID, Slot)"
		");";

	sqlite3_exec(s_pDb, pszAnimeSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全マップ影情報を正規化テーブルに全置換
//   DELETE + prepared INSERT ループ（外側トランザクションに乗る）
//   サブテーブル (sys_map_shadow_anime) も同様に DELETE して再 INSERT する
// ============================================================
void CSaveLoadInfoMapShadow::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去（サブテーブルも先に消す）
	sqlite3_exec(s_pDb, "DELETE FROM sys_map_shadow;",       NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_map_shadow_anime;", NULL, NULL, NULL);

	// メインテーブル INSERT 文
	const char* pszInsertMain =
		"INSERT INTO sys_map_shadow("
		"  ShadowID, ViewType, AnimeType, AnimeCount,"
		"  Level, Light, GrpID, ViewPosX, ViewPosY"
		") VALUES(?,?,?,?,?,?,?,?,?);";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsertMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMapShadow::SaveToNormalTable: prepare(main) failed\n");
		return;
	}

	// サブテーブル INSERT 文（影ごとに複数回使い回す）
	const char* pszInsertAnime =
		"INSERT INTO sys_map_shadow_anime(ShadowID, Slot, Wait, Level, GrpIDBase, GrpIDPile)"
		" VALUES(?,?,?,?,?,?);";

	sqlite3_stmt* pStmtAnime = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszInsertAnime, -1, &pStmtAnime, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMapShadow::SaveToNormalTable: prepare(anime) failed\n");
		sqlite3_finalize(pStmtMain);
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoMapShadow pInfo = (PCInfoMapShadow)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// ---- メインテーブルへの INSERT ----
		sqlite3_bind_int(pStmtMain, 1, (int)pInfo->m_dwShadowID);
		sqlite3_bind_int(pStmtMain, 2, (int)pInfo->m_byViewType);
		sqlite3_bind_int(pStmtMain, 3, (int)pInfo->m_byAnimeType);
		sqlite3_bind_int(pStmtMain, 4, (int)pInfo->m_byAnimeCount);
		sqlite3_bind_int(pStmtMain, 5, (int)pInfo->m_byLevel);
		sqlite3_bind_int(pStmtMain, 6, (int)pInfo->m_bLight);
		sqlite3_bind_int(pStmtMain, 7, (int)pInfo->m_wGrpID);
		sqlite3_bind_int(pStmtMain, 8, (int)pInfo->m_ptViewPos.x);
		sqlite3_bind_int(pStmtMain, 9, (int)pInfo->m_ptViewPos.y);

		sqlite3_step(pStmtMain);
		sqlite3_reset(pStmtMain);

		// ---- サブテーブルへの INSERT (m_paAnimeInfo ループ) ----
		//   Slot = 配列インデックス (0-based) をそのまま使う
		int nAnimeCount = pInfo->GetAnimeCount();
		for (int j = 0; j < nAnimeCount; j++) {
			PCInfoAnime pAnime = pInfo->GetAnimePtr(j);
			if (pAnime == NULL) continue;

			sqlite3_bind_int(pStmtAnime, 1, (int)pInfo->m_dwShadowID);
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
//   メインテーブルを全行 SELECT し、各影ごとにサブテーブルを
//   prepared statement を reset して再利用しながら取得する
// ============================================================
BOOL CSaveLoadInfoMapShadow::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	// メインテーブル SELECT
	const char* pszSelectMain =
		"SELECT"
		"  ShadowID, ViewType, AnimeType, AnimeCount,"
		"  Level, Light, GrpID, ViewPosX, ViewPosY"
		" FROM sys_map_shadow;";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSelectMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	// サブテーブル SELECT（外側で一度 prepare し、各影ごとに bind+reset で使い回す）
	//   ORDER BY Slot を必ず付けて配列インデックス順を保証する
	const char* pszSelectAnime =
		"SELECT Slot, Wait, Level, GrpIDBase, GrpIDPile"
		" FROM sys_map_shadow_anime WHERE ShadowID=? ORDER BY Slot;";

	sqlite3_stmt* pStmtAnime = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszSelectAnime, -1, &pStmtAnime, NULL);
	if (nRet != SQLITE_OK) {
		sqlite3_finalize(pStmtMain);
		return FALSE;
	}

	int nRowCount = 0;
	while (sqlite3_step(pStmtMain) == SQLITE_ROW) {
		// 新規 CInfoMapShadow を作成
		PCInfoMapShadow pInfo = (PCInfoMapShadow)pDst->GetNew();
		if (pInfo == NULL) continue;

		// m_dwShadowID を先にセットしてから Add() を呼ぶ
		//   ← Add() 内で 0 なら GetNewID() が呼ばれてしまうため
		DWORD dwShadowID = (DWORD)sqlite3_column_int(pStmtMain, 0);
		pInfo->m_dwShadowID   = dwShadowID;
		pInfo->m_byViewType   = (BYTE)sqlite3_column_int(pStmtMain, 1);
		pInfo->m_byAnimeType  = (BYTE)sqlite3_column_int(pStmtMain, 2);
		pInfo->m_byAnimeCount = (BYTE)sqlite3_column_int(pStmtMain, 3);
		pInfo->m_byLevel      = (BYTE)sqlite3_column_int(pStmtMain, 4);
		pInfo->m_bLight       = (BOOL)sqlite3_column_int(pStmtMain, 5);
		pInfo->m_wGrpID       = (WORD)sqlite3_column_int(pStmtMain, 6);
		pInfo->m_ptViewPos.x  = (LONG)sqlite3_column_int(pStmtMain, 7);
		pInfo->m_ptViewPos.y  = (LONG)sqlite3_column_int(pStmtMain, 8);

		// ---- サブテーブルからアニメーションコマを復元 ----
		//   prepared statement を bind + reset で再利用
		pInfo->DeleteAllAnime();
		sqlite3_bind_int(pStmtAnime, 1, (int)dwShadowID);
		while (sqlite3_step(pStmtAnime) == SQLITE_ROW) {
			// Slot は連番のはずだが ORDER BY Slot で取得するので Add 順序保証
			PCInfoAnime pAnime = new CInfoAnime;
			pAnime->m_byWait    = (BYTE)sqlite3_column_int(pStmtAnime, 1);
			pAnime->m_byLevel   = (BYTE)sqlite3_column_int(pStmtAnime, 2);
			pAnime->m_wGrpIDBase = (WORD)sqlite3_column_int(pStmtAnime, 3);
			pAnime->m_wGrpIDPile = (WORD)sqlite3_column_int(pStmtAnime, 4);
			pInfo->m_paAnimeInfo->Add(pAnime);
		}
		sqlite3_reset(pStmtAnime);

		// m_byAnimeCount をコマ数実値に合わせる（DB 値と一致するはずだが念のため）
		pInfo->m_byAnimeCount = (BYTE)pInfo->GetAnimeCount();

		// ライブラリに追加（m_dwShadowID が 0 でないので GetNewID() は呼ばれない）
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
BOOL CSaveLoadInfoMapShadow::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'MapShadow' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='MapShadow';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoMapShadow: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

BOOL CSaveLoadInfoMapShadow::HasBrokenAnimeRows(void)
{
	const char* pszSql =
		"SELECT CASE"
		"  WHEN EXISTS (SELECT 1 FROM sys_map_shadow_anime)"
		"   AND NOT EXISTS ("
		"     SELECT 1 FROM sys_map_shadow_anime"
		"      WHERE IFNULL(GrpIDBase, 0) <> 0 OR IFNULL(GrpIDPile, 0) <> 0"
		"   )"
		"  THEN 1"
		"  ELSE 0"
		"END;";
	sqlite3_stmt* pStmt = NULL;
	BOOL bRet = FALSE;

	if (s_pDb == NULL) {
		return FALSE;
	}

	if (sqlite3_prepare_v2(s_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
		return FALSE;
	}

	if (sqlite3_step(pStmt) == SQLITE_ROW) {
		bRet = sqlite3_column_int(pStmt, 0) ? TRUE : FALSE;
	}
	sqlite3_finalize(pStmt);

	return bRet;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoMapShadow::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoMapShadow::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 旧 .dat からの移行時にアニメコマの grpID だけ欠落した DB を自動修復する
	if (HasBrokenAnimeRows()) {
		OutputDebugStringA("SaveLoadInfoMapShadow: 破損した正規化アニメ行を検出 → BLOB/.dat から再マイグレーション\n");
		MigrateFromBlob(pDst);
		return;
	}

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoMapShadow: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoMapShadow: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
