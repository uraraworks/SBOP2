/// @file SaveLoadInfoItemWeapon.cpp
/// @brief データ保存・読み込みクラス(武器情報) 実装ファイル
///        ItemWeapon ライブラリを SQLite 正規化テーブル (sys_item_weapon) に移行
///        サブ配列 m_adwEffectIDAtack   → sys_item_weapon_effect_atk  (WeaponInfoID, Slot, EffectID)
///        サブ配列 m_adwEffectIDCritical → sys_item_weapon_effect_crit (WeaponInfoID, Slot, EffectID)
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/11
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoItemWeapon.h"
#include "LibInfoItemWeapon.h"
#include "SaveLoadInfoItemWeapon.h"

// テーブル名
static const char* s_pszTableName    = "sys_item_weapon";
static const char* s_pszTableAtk     = "sys_item_weapon_effect_atk";
static const char* s_pszTableCrit    = "sys_item_weapon_effect_crit";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoItemWeapon::CSaveLoadInfoItemWeapon()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataItemWeapon.dat");
	SetName("ItemWeapon");
}

CSaveLoadInfoItemWeapon::~CSaveLoadInfoItemWeapon()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   メインテーブル (sys_item_weapon) と
//   エフェクトID サブテーブル 2 本を同時に作成する
// ============================================================
void CSaveLoadInfoItemWeapon::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// メインテーブル: 武器基本情報（サブ配列以外）
	const char* pszMainSql =
		"CREATE TABLE IF NOT EXISTS sys_item_weapon("
		"  WeaponInfoID        INTEGER PRIMARY KEY,"	// m_dwWeaponInfoID
		"  MotionType          INTEGER,"				// m_dwMotionType
		"  MotionTypeStand     INTEGER,"				// m_dwMotionTypeStand
		"  MotionTypeWalk      INTEGER,"				// m_dwMotionTypeWalk
		"  Name                TEXT"					// m_strName (SJIS バイト列)
		");";

	sqlite3_exec(s_pDb, pszMainSql, NULL, NULL, NULL);

	// サブテーブル: 通常攻撃エフェクトID (m_adwEffectIDAtack の可変長配列)
	//   WeaponInfoID + Slot を複合主キーとし、配列インデックスを Slot に格納する
	const char* pszAtkSql =
		"CREATE TABLE IF NOT EXISTS sys_item_weapon_effect_atk("
		"  WeaponInfoID INTEGER NOT NULL,"
		"  Slot         INTEGER NOT NULL,"
		"  EffectID     INTEGER,"
		"  PRIMARY KEY (WeaponInfoID, Slot)"
		");";

	sqlite3_exec(s_pDb, pszAtkSql, NULL, NULL, NULL);

	// サブテーブル: クリティカルエフェクトID (m_adwEffectIDCritical の可変長配列)
	const char* pszCritSql =
		"CREATE TABLE IF NOT EXISTS sys_item_weapon_effect_crit("
		"  WeaponInfoID INTEGER NOT NULL,"
		"  Slot         INTEGER NOT NULL,"
		"  EffectID     INTEGER,"
		"  PRIMARY KEY (WeaponInfoID, Slot)"
		");";

	sqlite3_exec(s_pDb, pszCritSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全武器情報を正規化テーブルに全行置換
//   DELETE + INSERT ループ（外側トランザクションに乗る）
//   サブテーブル 2 本も同様に DELETE して再 INSERT する
// ============================================================
void CSaveLoadInfoItemWeapon::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去（サブテーブルも先に消す）
	sqlite3_exec(s_pDb, "DELETE FROM sys_item_weapon;",              NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_item_weapon_effect_atk;",   NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_item_weapon_effect_crit;",  NULL, NULL, NULL);

	// メインテーブル INSERT 文
	const char* pszInsertMain =
		"INSERT INTO sys_item_weapon("
		"  WeaponInfoID, MotionType, MotionTypeStand, MotionTypeWalk, Name"
		") VALUES(?, ?, ?, ?, ?);";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsertMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoItemWeapon::SaveToNormalTable: prepare(main) failed\n");
		return;
	}

	// 通常攻撃エフェクトサブテーブル INSERT 文
	const char* pszInsertAtk =
		"INSERT INTO sys_item_weapon_effect_atk(WeaponInfoID, Slot, EffectID) VALUES(?, ?, ?);";

	sqlite3_stmt* pStmtAtk = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszInsertAtk, -1, &pStmtAtk, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoItemWeapon::SaveToNormalTable: prepare(atk) failed\n");
		sqlite3_finalize(pStmtMain);
		return;
	}

	// クリティカルエフェクトサブテーブル INSERT 文
	const char* pszInsertCrit =
		"INSERT INTO sys_item_weapon_effect_crit(WeaponInfoID, Slot, EffectID) VALUES(?, ?, ?);";

	sqlite3_stmt* pStmtCrit = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszInsertCrit, -1, &pStmtCrit, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoItemWeapon::SaveToNormalTable: prepare(crit) failed\n");
		sqlite3_finalize(pStmtAtk);
		sqlite3_finalize(pStmtMain);
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoItemWeapon pInfo = (PCInfoItemWeapon)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// ---- メインテーブルへの INSERT ----
		sqlite3_bind_int(pStmtMain, 1, (int)pInfo->m_dwWeaponInfoID);
		sqlite3_bind_int(pStmtMain, 2, (int)pInfo->m_dwMotionType);
		sqlite3_bind_int(pStmtMain, 3, (int)pInfo->m_dwMotionTypeStand);
		sqlite3_bind_int(pStmtMain, 4, (int)pInfo->m_dwMotionTypeWalk);

		// m_strName: SJIS バイト列をそのまま TEXT としてバインド
		LPCSTR pszName = (LPCSTR)pInfo->m_strName;
		sqlite3_bind_text(pStmtMain, 5, pszName, -1, SQLITE_TRANSIENT);

		sqlite3_step(pStmtMain);
		sqlite3_reset(pStmtMain);

		// ---- サブテーブルへの INSERT: 通常攻撃エフェクトID ----
		//   Slot = 配列インデックス (0-based) をそのまま使う
		int nAtkCount = (int)pInfo->m_adwEffectIDAtack.size();
		for (int j = 0; j < nAtkCount; j++) {
			sqlite3_bind_int(pStmtAtk, 1, (int)pInfo->m_dwWeaponInfoID);
			sqlite3_bind_int(pStmtAtk, 2, j);
			sqlite3_bind_int(pStmtAtk, 3, (int)pInfo->m_adwEffectIDAtack[j]);

			sqlite3_step(pStmtAtk);
			sqlite3_reset(pStmtAtk);	// reset で再利用
		}

		// ---- サブテーブルへの INSERT: クリティカルエフェクトID ----
		int nCritCount = (int)pInfo->m_adwEffectIDCritical.size();
		for (int j = 0; j < nCritCount; j++) {
			sqlite3_bind_int(pStmtCrit, 1, (int)pInfo->m_dwWeaponInfoID);
			sqlite3_bind_int(pStmtCrit, 2, j);
			sqlite3_bind_int(pStmtCrit, 3, (int)pInfo->m_adwEffectIDCritical[j]);

			sqlite3_step(pStmtCrit);
			sqlite3_reset(pStmtCrit);
		}
	}

	sqlite3_finalize(pStmtCrit);
	sqlite3_finalize(pStmtAtk);
	sqlite3_finalize(pStmtMain);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
//   メインテーブルを全行 SELECT し、各武器ごとにサブテーブルを
//   prepared statement を reset して再利用しながら取得する
// ============================================================
BOOL CSaveLoadInfoItemWeapon::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	// メインテーブル SELECT
	const char* pszSelectMain =
		"SELECT WeaponInfoID, MotionType, MotionTypeStand, MotionTypeWalk, Name"
		" FROM sys_item_weapon;";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSelectMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	// 通常攻撃エフェクトサブテーブル SELECT
	//   ORDER BY Slot を必ず付けて配列インデックス順を保証する
	const char* pszSelectAtk =
		"SELECT Slot, EffectID FROM sys_item_weapon_effect_atk WHERE WeaponInfoID=? ORDER BY Slot;";

	sqlite3_stmt* pStmtAtk = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszSelectAtk, -1, &pStmtAtk, NULL);
	if (nRet != SQLITE_OK) {
		sqlite3_finalize(pStmtMain);
		return FALSE;
	}

	// クリティカルエフェクトサブテーブル SELECT
	const char* pszSelectCrit =
		"SELECT Slot, EffectID FROM sys_item_weapon_effect_crit WHERE WeaponInfoID=? ORDER BY Slot;";

	sqlite3_stmt* pStmtCrit = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszSelectCrit, -1, &pStmtCrit, NULL);
	if (nRet != SQLITE_OK) {
		sqlite3_finalize(pStmtAtk);
		sqlite3_finalize(pStmtMain);
		return FALSE;
	}

	int nRowCount = 0;
	while (sqlite3_step(pStmtMain) == SQLITE_ROW) {
		// 新規 CInfoItemWeapon を作成
		PCInfoItemWeapon pInfo = (PCInfoItemWeapon)pDst->GetNew();
		if (pInfo == NULL) continue;

		// WeaponInfoID を事前にセットしてから Add() を呼ぶ
		//   ← Add() 内で m_dwWeaponInfoID == 0 なら GetNewID() で上書きされてしまうため
		DWORD dwWeaponInfoID = (DWORD)sqlite3_column_int(pStmtMain, 0);
		pInfo->m_dwWeaponInfoID    = dwWeaponInfoID;

		pInfo->m_dwMotionType      = (DWORD)sqlite3_column_int(pStmtMain, 1);
		pInfo->m_dwMotionTypeStand = (DWORD)sqlite3_column_int(pStmtMain, 2);
		pInfo->m_dwMotionTypeWalk  = (DWORD)sqlite3_column_int(pStmtMain, 3);

		// Name: SJIS バイト列として格納されているので LegacyAnsiToTString で変換
		const char* pszName = (const char*)sqlite3_column_text(pStmtMain, 4);
		if (pszName != NULL) {
			pInfo->m_strName = (LPCTSTR)LegacyAnsiToTString(pszName);
		}

		// ---- サブテーブルから通常攻撃エフェクトIDリストを復元 ----
		//   prepared statement を bind + reset で再利用
		pInfo->m_adwEffectIDAtack.clear();
		sqlite3_bind_int(pStmtAtk, 1, (int)dwWeaponInfoID);
		while (sqlite3_step(pStmtAtk) == SQLITE_ROW) {
			DWORD dwEffectID = (DWORD)sqlite3_column_int(pStmtAtk, 1);
			pInfo->m_adwEffectIDAtack.push_back(dwEffectID);
		}
		sqlite3_reset(pStmtAtk);	// 次の武器のために reset

		// ---- サブテーブルからクリティカルエフェクトIDリストを復元 ----
		pInfo->m_adwEffectIDCritical.clear();
		sqlite3_bind_int(pStmtCrit, 1, (int)dwWeaponInfoID);
		while (sqlite3_step(pStmtCrit) == SQLITE_ROW) {
			DWORD dwEffectID = (DWORD)sqlite3_column_int(pStmtCrit, 1);
			pInfo->m_adwEffectIDCritical.push_back(dwEffectID);
		}
		sqlite3_reset(pStmtCrit);	// 次の武器のために reset

		// ライブラリに追加（m_dwWeaponInfoID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmtCrit);
	sqlite3_finalize(pStmtAtk);
	sqlite3_finalize(pStmtMain);

	// ID 検索マップを更新
	pDst->RenewIDPtr();

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoItemWeapon::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'ItemWeapon' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='ItemWeapon';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoItemWeapon: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoItemWeapon::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoItemWeapon::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoItemWeapon: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoItemWeapon: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
