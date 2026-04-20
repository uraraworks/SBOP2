/// @file SaveLoadInfoEfcBalloon.cpp
/// @brief データ保存・読み込みクラス(噴出し情報) 実装ファイル
///        EfcBalloon ライブラリを SQLite 正規化テーブル (sys_efc_balloon) に移行
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/12/24
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoEfcBalloon.h"
#include "LibInfoEfcBalloon.h"
#include "SaveLoadInfoEfcBalloon.h"

// テーブル名
static const char* s_pszTableName = "sys_efc_balloon";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoEfcBalloon::CSaveLoadInfoEfcBalloon()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataEfcBalloon.dat");
	SetName("EfcBalloon");
}

CSaveLoadInfoEfcBalloon::~CSaveLoadInfoEfcBalloon()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   カラム順は InfoEfcBalloon.cpp の s_aszName[] に準拠
//   (Wait, Loop, EfcBalloonID, ListID, AnimeID, SoundID, GrpID, Name)
// ============================================================
void CSaveLoadInfoEfcBalloon::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// EfcBalloonID を主キーとする（GetNewID() で重複なく払い出し済み）
	const char* pszSql =
		"CREATE TABLE IF NOT EXISTS sys_efc_balloon("
		"  EfcBalloonID INTEGER PRIMARY KEY,"	// m_dwEfcBalloonID
		"  Wait         INTEGER,"				// m_byWait  (×１０ミリ秒)
		"  Loop         INTEGER,"				// m_bLoop
		"  ListID       INTEGER,"				// m_dwListID
		"  AnimeID      INTEGER,"				// m_dwAnimeID
		"  SoundID      INTEGER,"				// m_dwSoundID
		"  GrpID        INTEGER,"				// m_dwGrpID
		"  Name         TEXT"					// m_strName
		");";

	sqlite3_exec(s_pDb, pszSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全行を正規化テーブルに全行置換
//   DELETE + prepared INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoEfcBalloon::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去
	sqlite3_exec(s_pDb, "DELETE FROM sys_efc_balloon;", NULL, NULL, NULL);

	// prepared statement を使い回す
	const char* pszInsert =
		"INSERT INTO sys_efc_balloon("
		"  EfcBalloonID, Wait, Loop, ListID, AnimeID, SoundID, GrpID, Name"
		") VALUES(?,?,?,?,?,?,?,?);";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsert, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoEfcBalloon::SaveToNormalTable: prepare failed\n");
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoEfcBalloon pInfo = (PCInfoEfcBalloon)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// バインド（カラム順に合わせて）
		sqlite3_bind_int(pStmt, 1, (int)pInfo->m_dwEfcBalloonID);
		sqlite3_bind_int(pStmt, 2, (int)pInfo->m_byWait);
		sqlite3_bind_int(pStmt, 3, (int)pInfo->m_bLoop);
		sqlite3_bind_int(pStmt, 4, (int)pInfo->m_dwListID);
		sqlite3_bind_int(pStmt, 5, (int)pInfo->m_dwAnimeID);
		sqlite3_bind_int(pStmt, 6, (int)pInfo->m_dwSoundID);
		sqlite3_bind_int(pStmt, 7, (int)pInfo->m_dwGrpID);

		// m_strName: SJIS バイト列をそのまま TEXT として格納
		LPCSTR pszName = (LPCSTR)pInfo->m_strName;
		sqlite3_bind_text(pStmt, 8, pszName, -1, SQLITE_TRANSIENT);

		sqlite3_step(pStmt);
		sqlite3_reset(pStmt);
	}

	sqlite3_finalize(pStmt);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
// ============================================================
BOOL CSaveLoadInfoEfcBalloon::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	const char* pszSql =
		"SELECT EfcBalloonID, Wait, Loop, ListID, AnimeID, SoundID, GrpID, Name"
		" FROM sys_efc_balloon;";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSql, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		// 新規 CInfoEfcBalloon を作成
		PCInfoEfcBalloon pInfo = (PCInfoEfcBalloon)pDst->GetNew();
		if (pInfo == NULL) continue;

		// EfcBalloonID を先に設定（Add() 内で 0 なら GetNewID() が呼ばれるため先セット）
		pInfo->m_dwEfcBalloonID = (DWORD)sqlite3_column_int(pStmt, 0);

		pInfo->m_byWait    = (BYTE) sqlite3_column_int(pStmt, 1);
		pInfo->m_bLoop     = (BOOL) sqlite3_column_int(pStmt, 2);
		pInfo->m_dwListID  = (DWORD)sqlite3_column_int(pStmt, 3);
		pInfo->m_dwAnimeID = (DWORD)sqlite3_column_int(pStmt, 4);
		pInfo->m_dwSoundID = (DWORD)sqlite3_column_int(pStmt, 5);
		pInfo->m_dwGrpID   = (DWORD)sqlite3_column_int(pStmt, 6);

		// Name: SJIS バイト列として格納されているので LegacyAnsiToTString で変換
		const char* pszName = (const char*)sqlite3_column_text(pStmt, 7);
		if (pszName != NULL) {
			pInfo->m_strName = (LPCTSTR)LegacyAnsiToTString(pszName);
		}

		// ライブラリに追加（m_dwEfcBalloonID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmt);

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoEfcBalloon::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'EfcBalloon' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='EfcBalloon';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoEfcBalloon: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoEfcBalloon::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoEfcBalloon::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoEfcBalloon: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoEfcBalloon: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
