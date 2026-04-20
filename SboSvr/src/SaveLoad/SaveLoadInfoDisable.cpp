/// @file SaveLoadInfoDisable.cpp
/// @brief データ保存・読み込みクラス(拒否情報) 実装ファイル
///        Disable ライブラリを SQLite 正規化テーブル (sys_disable) に移行
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/11
/// @copyright Copyright(C)URARA-works 2009

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoDisable.h"
#include "LibInfoDisable.h"
#include "SaveLoadInfoDisable.h"

// テーブル名
static const char* s_pszTableName = "sys_disable";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoDisable::CSaveLoadInfoDisable()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataDisable.dat");
	SetName("Disable");
}

CSaveLoadInfoDisable::~CSaveLoadInfoDisable()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
// ============================================================
void CSaveLoadInfoDisable::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// DisableID はユニークな主キー（GetNewID() で重複なく払い出し済み）
	const char* pszSql =
		"CREATE TABLE IF NOT EXISTS sys_disable("
		"  DisableID  INTEGER PRIMARY KEY,"
		"  MacAddress TEXT"
		");";

	sqlite3_exec(s_pDb, pszSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全行を正規化テーブルに全行置換
//   DELETE + INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoDisable::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去
	sqlite3_exec(s_pDb, "DELETE FROM sys_disable;", NULL, NULL, NULL);

	// 行ごとに INSERT
	const char* pszInsert =
		"INSERT INTO sys_disable(DisableID, MacAddress) VALUES(?, ?);";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsert, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoDisable::SaveToNormalTable: prepare failed\n");
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoDisable pInfo = (PCInfoDisable)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// DisableID をバインド
		sqlite3_bind_int(pStmt, 1, (int)pInfo->m_dwDisableID);

		// MacAddress: SJIS バイト列をそのまま TEXT としてバインド（-1 で NULL 終端まで）
		LPCSTR pszMac = (LPCSTR)pInfo->m_strMacAddress;
		sqlite3_bind_text(pStmt, 2, pszMac, -1, SQLITE_TRANSIENT);

		sqlite3_step(pStmt);
		sqlite3_reset(pStmt);
	}

	sqlite3_finalize(pStmt);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
// ============================================================
BOOL CSaveLoadInfoDisable::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	const char* pszSql =
		"SELECT DisableID, MacAddress FROM sys_disable;";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSql, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		// 新規 CInfoDisable を作成
		PCInfoDisable pInfo = (PCInfoDisable)pDst->GetNew();
		if (pInfo == NULL) continue;

		// DisableID を直接設定（Add() で重複チェック後に m_dwDisableID が 0 なら振り直すので事前に設定）
		pInfo->m_dwDisableID = (DWORD)sqlite3_column_int(pStmt, 0);

		// MacAddress: SJIS バイト列として格納されているので LPCSTR で取得
		const char* pszMac = (const char*)sqlite3_column_text(pStmt, 1);
		if (pszMac != NULL) {
			// LegacyAnsiToTString と同様に SJIS → TCHAR 変換して格納
			pInfo->m_strMacAddress = (LPCTSTR)LegacyAnsiToTString(pszMac);
		}

		// ライブラリに追加（DisableID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmt);

	// ID 検索マップを更新
	pDst->RenewIDPtr();

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoDisable::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'Disable' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='Disable';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoDisable: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoDisable::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoDisable::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoDisable: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoDisable: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
