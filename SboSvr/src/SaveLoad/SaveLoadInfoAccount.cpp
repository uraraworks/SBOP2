/// @file SaveLoadInfoAccount.cpp
/// @brief データ保存・読み込みクラス(アカウント情報) 実装ファイル
///        Account ライブラリを SQLite 正規化テーブル (sys_account / sys_account_char) に移行
///        サブ配列 (m_adwCharID) をサブテーブル sys_account_char に (AccountID, Slot) で格納する
///        初のサブテーブルパターン ─ 将来の Char / Item / Map 等でも同じ構造を流用すること
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoAccount.h"
#include "LibInfoAccount.h"
#include "SaveLoadInfoAccount.h"

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoAccount::CSaveLoadInfoAccount()
{
	// ファイル名の設定（フォールバック用 .dat パス）
	SetFileName("SBODATA\\SboDataAccount.dat");
	// SQLite PRIMARY KEY 用論理名
	SetName("Account");
}

CSaveLoadInfoAccount::~CSaveLoadInfoAccount()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   メインテーブル (sys_account) とサブテーブル (sys_account_char) を両方作成する
// ============================================================
void CSaveLoadInfoAccount::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// メインテーブル: アカウント基本情報（m_adwCharID 以外）
	const char* pszMainSql =
		"CREATE TABLE IF NOT EXISTS sys_account("
		"  AccountID       INTEGER PRIMARY KEY,"
		"  Disable         INTEGER,"
		"  CharID          INTEGER,"
		"  TimeLastLogin   INTEGER,"
		"  TimeMakeAccount INTEGER,"
		"  LoginCount      INTEGER,"
		"  AdminLevel      INTEGER,"
		"  Account         TEXT,"
		"  Password        TEXT,"
		"  MacAddr         TEXT"
		");";

	sqlite3_exec(s_pDb, pszMainSql, NULL, NULL, NULL);

	// サブテーブル: キャラIDリスト (m_adwCharID の可変長配列)
	//   AccountID + Slot を複合主キーとし、配列インデックスをそのまま Slot に格納する
	//   【流用ポイント】Item テーブル等でも同じパターン (親ID + Slot + 値) が使える
	const char* pszSubSql =
		"CREATE TABLE IF NOT EXISTS sys_account_char("
		"  AccountID INTEGER NOT NULL,"
		"  Slot      INTEGER NOT NULL,"
		"  CharID    INTEGER,"
		"  PRIMARY KEY (AccountID, Slot)"
		");";

	sqlite3_exec(s_pDb, pszSubSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全アカウントを正規化テーブルに全置換
//   DELETE + INSERT ループ（外側トランザクションに乗る）
//   サブテーブルも同様に DELETE して再 INSERT する
// ============================================================
void CSaveLoadInfoAccount::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去（サブテーブルも先に消す）
	sqlite3_exec(s_pDb, "DELETE FROM sys_account;",      NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_account_char;", NULL, NULL, NULL);

	// メインテーブル INSERT 文
	const char* pszInsertMain =
		"INSERT INTO sys_account("
		"  AccountID, Disable, CharID, TimeLastLogin, TimeMakeAccount,"
		"  LoginCount, AdminLevel, Account, Password, MacAddr"
		") VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsertMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoAccount::SaveToNormalTable: prepare(main) failed\n");
		return;
	}

	// サブテーブル INSERT 文（アカウントごとに複数回使い回すので prepare/reset パターン推奨）
	const char* pszInsertSub =
		"INSERT INTO sys_account_char(AccountID, Slot, CharID) VALUES(?, ?, ?);";

	sqlite3_stmt* pStmtSub = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszInsertSub, -1, &pStmtSub, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoAccount::SaveToNormalTable: prepare(sub) failed\n");
		sqlite3_finalize(pStmtMain);
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoAccount pInfo = (PCInfoAccount)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// ---- メインテーブルへの INSERT ----
		sqlite3_bind_int(pStmtMain, 1, (int)pInfo->m_dwAccountID);
		sqlite3_bind_int(pStmtMain, 2, (int)pInfo->m_bDisable);
		sqlite3_bind_int(pStmtMain, 3, (int)pInfo->m_dwCharID);
		sqlite3_bind_int(pStmtMain, 4, (int)pInfo->m_dwTimeLastLogin);
		sqlite3_bind_int(pStmtMain, 5, (int)pInfo->m_dwTimeMakeAccount);
		sqlite3_bind_int(pStmtMain, 6, (int)pInfo->m_dwLoginCount);
		sqlite3_bind_int(pStmtMain, 7, (int)pInfo->m_nAdminLevel);

		// 文字列は SJIS バイト列をそのまま TEXT としてバインド（Disable と同じ方針）
		LPCSTR pszAccount  = (LPCSTR)pInfo->m_strAccount;
		LPCSTR pszPassword = (LPCSTR)pInfo->m_strPassword;
		LPCSTR pszMacAddr  = (LPCSTR)pInfo->m_strMacAddr;
		sqlite3_bind_text(pStmtMain, 8, pszAccount,  -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStmtMain, 9, pszPassword, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStmtMain, 10, pszMacAddr, -1, SQLITE_TRANSIENT);

		sqlite3_step(pStmtMain);
		sqlite3_reset(pStmtMain);

		// ---- サブテーブルへの INSERT (m_adwCharID ループ) ----
		//   Slot = 配列インデックス (0-based) をそのまま使う
		//   【流用ポイント】Item 等の可変長サブ配列でも (親ID, Slot, 値) の 3 列で同じ実装になる
		int nCharCount = (int)pInfo->m_adwCharID.size();
		for (int j = 0; j < nCharCount; j++) {
			sqlite3_bind_int(pStmtSub, 1, (int)pInfo->m_dwAccountID);
			sqlite3_bind_int(pStmtSub, 2, j);
			sqlite3_bind_int(pStmtSub, 3, (int)pInfo->m_adwCharID[j]);

			sqlite3_step(pStmtSub);
			sqlite3_reset(pStmtSub);	// reset で再利用（finalize より高効率）
		}
	}

	sqlite3_finalize(pStmtSub);
	sqlite3_finalize(pStmtMain);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
//   メインテーブルを全行 SELECT し、各アカウントごとにサブテーブルを
//   prepared statement を reset して再利用しながら取得する
// ============================================================
BOOL CSaveLoadInfoAccount::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	// メインテーブル SELECT
	const char* pszSelectMain =
		"SELECT AccountID, Disable, CharID, TimeLastLogin, TimeMakeAccount,"
		"       LoginCount, AdminLevel, Account, Password, MacAddr"
		" FROM sys_account;";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSelectMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	// サブテーブル SELECT（外側で一度 prepare し、各アカウントごとに bind+reset で使い回す）
	//   【流用ポイント】ORDER BY Slot を必ず付けて配列インデックス順を保証する
	const char* pszSelectSub =
		"SELECT Slot, CharID FROM sys_account_char WHERE AccountID=? ORDER BY Slot;";

	sqlite3_stmt* pStmtSub = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszSelectSub, -1, &pStmtSub, NULL);
	if (nRet != SQLITE_OK) {
		sqlite3_finalize(pStmtMain);
		return FALSE;
	}

	int nRowCount = 0;
	while (sqlite3_step(pStmtMain) == SQLITE_ROW) {
		// 新規 CInfoAccount を作成
		PCInfoAccount pInfo = (PCInfoAccount)pDst->GetNew();
		if (pInfo == NULL) continue;

		// メインテーブルの各列をメンバに設定
		DWORD dwAccountID = (DWORD)sqlite3_column_int(pStmtMain, 0);

		// m_dwAccountID を事前にセットしてから Add() を呼ぶ
		//   ← Add() 内で m_dwAccountID == 0 なら GetNewID() で上書きされてしまうため
		pInfo->m_dwAccountID     = dwAccountID;
		pInfo->m_bDisable        = (BOOL)sqlite3_column_int(pStmtMain, 1);
		pInfo->m_dwCharID        = (DWORD)sqlite3_column_int(pStmtMain, 2);
		pInfo->m_dwTimeLastLogin = (DWORD)sqlite3_column_int(pStmtMain, 3);
		pInfo->m_dwTimeMakeAccount = (DWORD)sqlite3_column_int(pStmtMain, 4);
		pInfo->m_dwLoginCount    = (DWORD)sqlite3_column_int(pStmtMain, 5);
		pInfo->m_nAdminLevel     = (int)sqlite3_column_int(pStmtMain, 6);

		// 文字列列: SJIS バイト列として格納されているので LPCSTR で取得し TCHAR 変換
		const char* pszAccount  = (const char*)sqlite3_column_text(pStmtMain, 7);
		const char* pszPassword = (const char*)sqlite3_column_text(pStmtMain, 8);
		const char* pszMacAddr  = (const char*)sqlite3_column_text(pStmtMain, 9);
		if (pszAccount  != NULL) pInfo->m_strAccount  = (LPCTSTR)LegacyAnsiToTString(pszAccount);
		if (pszPassword != NULL) pInfo->m_strPassword = (LPCTSTR)LegacyAnsiToTString(pszPassword);
		if (pszMacAddr  != NULL) pInfo->m_strMacAddr  = (LPCTSTR)LegacyAnsiToTString(pszMacAddr);

		// ---- サブテーブルからキャラIDリストを復元 ----
		//   prepared statement を bind + reset で再利用
		pInfo->m_adwCharID.clear();
		sqlite3_bind_int(pStmtSub, 1, (int)dwAccountID);
		while (sqlite3_step(pStmtSub) == SQLITE_ROW) {
			// Slot は連番のはずだが ORDER BY Slot で取得するので push_back で順序保証
			DWORD dwCharID = (DWORD)sqlite3_column_int(pStmtSub, 1);
			pInfo->m_adwCharID.push_back(dwCharID);
		}
		sqlite3_reset(pStmtSub);	// 次のアカウントのために reset

		// ライブラリに追加（m_dwAccountID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmtSub);
	sqlite3_finalize(pStmtMain);

	// ID 検索マップを更新（LibInfoAccount は基底の空実装でよい）
	pDst->RenewIDPtr();

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoAccount::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'Account' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='Account';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoAccount: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoAccount::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoAccount::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoAccount: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoAccount: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
