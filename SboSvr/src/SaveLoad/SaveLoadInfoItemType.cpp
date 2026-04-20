/// @file SaveLoadInfoItemType.cpp
/// @brief データ保存・読み込みクラス(アイテム種別情報) 実装ファイル
///        ItemType ライブラリを SQLite 正規化テーブル (sys_item_type) に移行
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/02
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoItemTypeBase.h"
#include "LibInfoItemType.h"
#include "SaveLoadInfoItemType.h"

// テーブル名
static const char* s_pszTableName = "sys_item_type";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoItemType::CSaveLoadInfoItemType()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataItemType.dat");
	SetName("ItemType");
}

CSaveLoadInfoItemType::~CSaveLoadInfoItemType()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   カラム順は InfoItemTypeBase.cpp の s_aszName[] に準拠
// ============================================================
void CSaveLoadInfoItemType::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// TypeID を主キーとする（m_dwTypeID はユニークな種別IDとして管理）
	const char* pszSql =
		"CREATE TABLE IF NOT EXISTS sys_item_type("
		"  TypeID       INTEGER PRIMARY KEY,"	// m_dwTypeID (index=4)
		"  DelAverage   INTEGER,"				// m_byDelAverage (index=0)
		"  Target       INTEGER,"				// m_byTarget (index=1)
		"  Area         INTEGER,"				// m_byArea (index=2)
		"  PutOn        INTEGER,"				// m_bPutOn (index=3)
		"  ItemTypeID   INTEGER,"				// m_dwItemTypeID (index=5)
		"  GrpID        INTEGER,"				// m_dwGrpID (index=6)
		"  IconGrpID    INTEGER,"				// m_dwIconGrpID (index=7)
		"  Name         TEXT,"					// m_strName (index=8)
		"  DropSoundID  INTEGER,"				// m_dwDropSoundID (index=9)
		"  GrpIDMain    INTEGER,"				// m_wGrpIDMain (index=10)
		"  GrpIDSub     INTEGER,"				// m_wGrpIDSub (index=11)
		"  UseEffectID  INTEGER,"				// m_dwUseEffectID (index=12)
		"  UseSoundID   INTEGER,"				// m_dwUseSoundID (index=13)
		"  WeaponInfoID INTEGER,"				// m_dwWeaponInfoID (index=14)
		"  Value        INTEGER,"				// m_dwValue (index=15)
		"  MoveWait     INTEGER,"				// m_dwMoveWait (index=16)
		"  MoveCount    INTEGER,"				// m_dwMoveCount (index=17)
		"  Value2       INTEGER"				// m_dwValue2 (index=18)
		");";

	sqlite3_exec(s_pDb, pszSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全行を正規化テーブルに全行置換
//   DELETE + prepared INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoItemType::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去
	sqlite3_exec(s_pDb, "DELETE FROM sys_item_type;", NULL, NULL, NULL);

	// prepared statement を使い回す
	const char* pszInsert =
		"INSERT INTO sys_item_type("
		"  TypeID, DelAverage, Target, Area, PutOn,"
		"  ItemTypeID, GrpID, IconGrpID, Name, DropSoundID,"
		"  GrpIDMain, GrpIDSub, UseEffectID, UseSoundID,"
		"  WeaponInfoID, Value, MoveWait, MoveCount, Value2"
		") VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsert, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoItemType::SaveToNormalTable: prepare failed\n");
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoItemTypeBase pInfo = (PCInfoItemTypeBase)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// バインド（カラム順に合わせて）
		sqlite3_bind_int(pStmt,  1, (int)pInfo->m_dwTypeID);
		sqlite3_bind_int(pStmt,  2, (int)pInfo->m_byDelAverage);
		sqlite3_bind_int(pStmt,  3, (int)pInfo->m_byTarget);
		sqlite3_bind_int(pStmt,  4, (int)pInfo->m_byArea);
		sqlite3_bind_int(pStmt,  5, (int)pInfo->m_bPutOn);
		sqlite3_bind_int(pStmt,  6, (int)pInfo->m_dwItemTypeID);
		sqlite3_bind_int(pStmt,  7, (int)pInfo->m_dwGrpID);
		sqlite3_bind_int(pStmt,  8, (int)pInfo->m_dwIconGrpID);

		// m_strName: SJIS バイト列をそのまま TEXT として格納
		LPCSTR pszName = (LPCSTR)pInfo->m_strName;
		sqlite3_bind_text(pStmt, 9, pszName, -1, SQLITE_TRANSIENT);

		sqlite3_bind_int(pStmt, 10, (int)pInfo->m_dwDropSoundID);
		sqlite3_bind_int(pStmt, 11, (int)pInfo->m_wGrpIDMain);
		sqlite3_bind_int(pStmt, 12, (int)pInfo->m_wGrpIDSub);
		sqlite3_bind_int(pStmt, 13, (int)pInfo->m_dwUseEffectID);
		sqlite3_bind_int(pStmt, 14, (int)pInfo->m_dwUseSoundID);
		sqlite3_bind_int(pStmt, 15, (int)pInfo->m_dwWeaponInfoID);
		sqlite3_bind_int(pStmt, 16, (int)pInfo->m_dwValue);
		sqlite3_bind_int(pStmt, 17, (int)pInfo->m_dwMoveWait);
		sqlite3_bind_int(pStmt, 18, (int)pInfo->m_dwMoveCount);
		sqlite3_bind_int(pStmt, 19, (int)pInfo->m_dwValue2);

		sqlite3_step(pStmt);
		sqlite3_reset(pStmt);
	}

	sqlite3_finalize(pStmt);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
// ============================================================
BOOL CSaveLoadInfoItemType::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	const char* pszSql =
		"SELECT TypeID, DelAverage, Target, Area, PutOn,"
		"       ItemTypeID, GrpID, IconGrpID, Name, DropSoundID,"
		"       GrpIDMain, GrpIDSub, UseEffectID, UseSoundID,"
		"       WeaponInfoID, Value, MoveWait, MoveCount, Value2"
		" FROM sys_item_type;";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSql, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		// 新規 CInfoItemTypeBase を作成（GetNew() は ITEMTYPEID_NONE で確保する）
		PCInfoItemTypeBase pInfo = (PCInfoItemTypeBase)pDst->GetNew();
		if (pInfo == NULL) continue;

		// TypeID を先に設定（Add() 内で 0 なら GetNewID() が呼ばれるため先セット）
		pInfo->m_dwTypeID      = (DWORD)sqlite3_column_int(pStmt,  0);

		pInfo->m_byDelAverage  = (BYTE) sqlite3_column_int(pStmt,  1);
		pInfo->m_byTarget      = (BYTE) sqlite3_column_int(pStmt,  2);
		pInfo->m_byArea        = (BYTE) sqlite3_column_int(pStmt,  3);
		pInfo->m_bPutOn        = (BOOL) sqlite3_column_int(pStmt,  4);
		pInfo->m_dwItemTypeID  = (DWORD)sqlite3_column_int(pStmt,  5);
		pInfo->m_dwGrpID       = (DWORD)sqlite3_column_int(pStmt,  6);
		pInfo->m_dwIconGrpID   = (DWORD)sqlite3_column_int(pStmt,  7);

		// Name: SJIS バイト列として格納されているので LegacyAnsiToTString で変換
		const char* pszName = (const char*)sqlite3_column_text(pStmt, 8);
		if (pszName != NULL) {
			pInfo->m_strName = (LPCTSTR)LegacyAnsiToTString(pszName);
		}

		pInfo->m_dwDropSoundID = (DWORD)sqlite3_column_int(pStmt,  9);
		pInfo->m_wGrpIDMain    = (WORD) sqlite3_column_int(pStmt, 10);
		pInfo->m_wGrpIDSub     = (WORD) sqlite3_column_int(pStmt, 11);
		pInfo->m_dwUseEffectID = (DWORD)sqlite3_column_int(pStmt, 12);
		pInfo->m_dwUseSoundID  = (DWORD)sqlite3_column_int(pStmt, 13);
		pInfo->m_dwWeaponInfoID= (DWORD)sqlite3_column_int(pStmt, 14);
		pInfo->m_dwValue       = (DWORD)sqlite3_column_int(pStmt, 15);
		pInfo->m_dwMoveWait    = (DWORD)sqlite3_column_int(pStmt, 16);
		pInfo->m_dwMoveCount   = (DWORD)sqlite3_column_int(pStmt, 17);
		pInfo->m_dwValue2      = (DWORD)sqlite3_column_int(pStmt, 18);

		// ライブラリに追加（m_dwTypeID が 0 でないので GetNewID() は呼ばれない）
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
BOOL CSaveLoadInfoItemType::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'ItemType' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='ItemType';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoItemType: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoItemType::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoItemType::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoItemType: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoItemType: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
