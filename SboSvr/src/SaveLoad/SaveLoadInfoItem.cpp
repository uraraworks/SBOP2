/// @file SaveLoadInfoItem.cpp
/// @brief データ保存・読み込みクラス(アイテム情報) 実装ファイル
///        Item ライブラリを SQLite 正規化テーブル (sys_item) に移行
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/19
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoItem.h"
#include "LibInfoItem.h"
#include "SaveLoadInfoItem.h"

// テーブル名
static const char* s_pszTableName = "sys_item";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoItem::CSaveLoadInfoItem()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataItem.dat");
	SetName("Item");
}

CSaveLoadInfoItem::~CSaveLoadInfoItem()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   カラム順は InfoItem.cpp の s_aszName[] に準拠
//   POINT (m_ptPos, m_ptBackPack) は x/y を別カラムに展開
// ============================================================
void CSaveLoadInfoItem::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// ItemID を主キーとする（m_dwItemID はユニークなアイテムIDとして管理）
	const char* pszSql =
		"CREATE TABLE IF NOT EXISTS sys_item("
		"  ItemID       INTEGER PRIMARY KEY,"	// m_dwItemID (index=1)
		"  PutOn        INTEGER,"				// m_bPutOn (index=0)
		"  ItemTypeID   INTEGER,"				// m_dwItemTypeID (index=2)
		"  GrpID        INTEGER,"				// m_dwGrpID (index=3)
		"  IconGrpID    INTEGER,"				// m_dwIconGrpID (index=4)
		"  MapID        INTEGER,"				// m_dwMapID (index=5)
		"  CharID       INTEGER,"				// m_dwCharID (index=6)
		"  PosZ         INTEGER,"				// m_nPosZ (index=7)
		"  PosX         INTEGER,"				// m_ptPos.x (index=8)
		"  PosY         INTEGER,"				// m_ptPos.y (index=8)
		"  BackPackX    INTEGER,"				// m_ptBackPack.x (index=9)
		"  BackPackY    INTEGER,"				// m_ptBackPack.y (index=9)
		"  Name         TEXT,"					// m_strName (index=10)
		"  DropSoundID  INTEGER"				// m_dwDropSoundID (index=11)
		");";

	sqlite3_exec(s_pDb, pszSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全行を正規化テーブルに全行置換
//   DELETE + prepared INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoItem::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去
	sqlite3_exec(s_pDb, "DELETE FROM sys_item;", NULL, NULL, NULL);

	// prepared statement を使い回す
	const char* pszInsert =
		"INSERT INTO sys_item("
		"  ItemID, PutOn, ItemTypeID, GrpID, IconGrpID,"
		"  MapID, CharID, PosZ, PosX, PosY,"
		"  BackPackX, BackPackY, Name, DropSoundID"
		") VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?);";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsert, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoItem::SaveToNormalTable: prepare failed\n");
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoItem pInfo = (PCInfoItem)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// バインド（カラム順に合わせて）
		sqlite3_bind_int(pStmt,  1, (int)pInfo->m_dwItemID);
		sqlite3_bind_int(pStmt,  2, (int)pInfo->m_bPutOn);
		sqlite3_bind_int(pStmt,  3, (int)pInfo->m_dwItemTypeID);
		sqlite3_bind_int(pStmt,  4, (int)pInfo->m_dwGrpID);
		sqlite3_bind_int(pStmt,  5, (int)pInfo->m_dwIconGrpID);
		sqlite3_bind_int(pStmt,  6, (int)pInfo->m_dwMapID);
		sqlite3_bind_int(pStmt,  7, (int)pInfo->m_dwCharID);
		sqlite3_bind_int(pStmt,  8, (int)pInfo->m_nPosZ);
		sqlite3_bind_int(pStmt,  9, (int)pInfo->m_ptPos.x);
		sqlite3_bind_int(pStmt, 10, (int)pInfo->m_ptPos.y);
		sqlite3_bind_int(pStmt, 11, (int)pInfo->m_ptBackPack.x);
		sqlite3_bind_int(pStmt, 12, (int)pInfo->m_ptBackPack.y);

		// m_strName: SJIS バイト列をそのまま TEXT として格納
		LPCSTR pszName = (LPCSTR)pInfo->m_strName;
		sqlite3_bind_text(pStmt, 13, pszName, -1, SQLITE_TRANSIENT);

		sqlite3_bind_int(pStmt, 14, (int)pInfo->m_dwDropSoundID);

		sqlite3_step(pStmt);
		sqlite3_reset(pStmt);
	}

	sqlite3_finalize(pStmt);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
// ============================================================
BOOL CSaveLoadInfoItem::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	const char* pszSql =
		"SELECT ItemID, PutOn, ItemTypeID, GrpID, IconGrpID,"
		"       MapID, CharID, PosZ, PosX, PosY,"
		"       BackPackX, BackPackY, Name, DropSoundID"
		" FROM sys_item;";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSql, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		// 新規 CInfoItem を作成
		PCInfoItem pInfo = (PCInfoItem)pDst->GetNew();
		if (pInfo == NULL) continue;

		// ItemID を先に設定（Add() 内で 0 なら GetNewID() が呼ばれるため先セット）
		pInfo->m_dwItemID     = (DWORD)sqlite3_column_int(pStmt,  0);

		pInfo->m_bPutOn       = (BOOL) sqlite3_column_int(pStmt,  1);
		pInfo->m_dwItemTypeID = (DWORD)sqlite3_column_int(pStmt,  2);
		pInfo->m_dwGrpID      = (DWORD)sqlite3_column_int(pStmt,  3);
		pInfo->m_dwIconGrpID  = (DWORD)sqlite3_column_int(pStmt,  4);
		pInfo->m_dwMapID      = (DWORD)sqlite3_column_int(pStmt,  5);
		pInfo->m_dwCharID     = (DWORD)sqlite3_column_int(pStmt,  6);
		pInfo->m_nPosZ        = (int)  sqlite3_column_int(pStmt,  7);
		pInfo->m_ptPos.x      = (LONG) sqlite3_column_int(pStmt,  8);
		pInfo->m_ptPos.y      = (LONG) sqlite3_column_int(pStmt,  9);
		pInfo->m_ptBackPack.x = (LONG) sqlite3_column_int(pStmt, 10);
		pInfo->m_ptBackPack.y = (LONG) sqlite3_column_int(pStmt, 11);

		// Name: SJIS バイト列として格納されているので LegacyAnsiToTString で変換
		const char* pszName = (const char*)sqlite3_column_text(pStmt, 12);
		if (pszName != NULL) {
			pInfo->m_strName = (LPCTSTR)LegacyAnsiToTString(pszName);
		}

		pInfo->m_dwDropSoundID = (DWORD)sqlite3_column_int(pStmt, 13);

		// ライブラリに追加（m_dwItemID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmt);

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoItem::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'Item' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='Item';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoItem: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoItem::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoItem::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoItem: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoItem: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
