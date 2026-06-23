/// @file SaveLoadInfoMotionType.cpp
/// @brief データ保存・読み込みクラス(モーション種別情報) 実装ファイル
///        MotionType ライブラリを SQLite 正規化テーブル (sys_motion_type) に移行
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/05/31
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoMotionType.h"
#include "LibInfoMotionType.h"
#include "SaveLoadInfoMotionType.h"

// テーブル名
static const char* s_pszTableName = "sys_motion_type";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoMotionType::CSaveLoadInfoMotionType()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataMotionType.dat");
	SetName("MotionType");
}

CSaveLoadInfoMotionType::~CSaveLoadInfoMotionType()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   カラム順は InfoMotionType.cpp の s_aszName[] に準拠
//   (MotionTypeID, GrpIDSub, Name)
// ============================================================
void CSaveLoadInfoMotionType::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// MotionTypeID を主キーとする（GetNewID() で重複なく払い出し済み）
	const char* pszSql =
		"CREATE TABLE IF NOT EXISTS sys_motion_type("
		"  MotionTypeID INTEGER PRIMARY KEY,"	// m_dwMotionTypeID
		"  GrpIDSub    INTEGER,"				// m_wGrpIDSub (プレビュー用グラフィックIDサブ)
		"  Name        TEXT"					// m_strName
		");";

	sqlite3_exec(s_pDb, pszSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全行を正規化テーブルに全行置換
//   DELETE + prepared INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoMotionType::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去
	sqlite3_exec(s_pDb, "DELETE FROM sys_motion_type;", NULL, NULL, NULL);

	// prepared statement を使い回す
	const char* pszInsert =
		"INSERT INTO sys_motion_type("
		"  MotionTypeID, GrpIDSub, Name"
		") VALUES(?,?,?);";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsert, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMotionType::SaveToNormalTable: prepare failed\n");
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoMotionType pInfo = (PCInfoMotionType)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// バインド（カラム順に合わせて）
		sqlite3_bind_int(pStmt, 1, (int)pInfo->m_dwMotionTypeID);
		sqlite3_bind_int(pStmt, 2, (int)pInfo->m_wGrpIDSub);

		// m_strName: UTF-8 バイト列としてバインド
		LPCSTR pszName = pInfo->m_strName.GetUtf8Pointer();
		sqlite3_bind_text(pStmt, 3, pszName, -1, SQLITE_TRANSIENT);

		sqlite3_step(pStmt);
		sqlite3_reset(pStmt);
	}

	sqlite3_finalize(pStmt);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
// ============================================================
BOOL CSaveLoadInfoMotionType::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	const char* pszSql =
		"SELECT MotionTypeID, GrpIDSub, Name"
		" FROM sys_motion_type;";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSql, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		// 新規 CInfoMotionType を作成
		PCInfoMotionType pInfo = (PCInfoMotionType)pDst->GetNew();
		if (pInfo == NULL) continue;

		// MotionTypeID を先に設定（Add() 内で 0 なら GetNewID() が呼ばれるため先セット）
		pInfo->m_dwMotionTypeID = (DWORD)sqlite3_column_int(pStmt, 0);

		pInfo->m_wGrpIDSub = (WORD)sqlite3_column_int(pStmt, 1);

		// Name: UTF-8 バイト列として格納されているので Utf8ToTString で変換
		const char* pszName = (const char*)sqlite3_column_text(pStmt, 2);
		if (pszName != NULL) {
			pInfo->m_strName = (LPCTSTR)Utf8ToTString(pszName);
		}

		// ライブラリに追加（m_dwMotionTypeID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmt);

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoMotionType::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'MotionType' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='MotionType';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoMotionType: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoMotionType::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoMotionType::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoMotionType: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoMotionType: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
