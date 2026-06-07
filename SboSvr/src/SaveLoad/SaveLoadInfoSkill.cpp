/// @file SaveLoadInfoSkill.cpp
/// @brief データ保存・読み込みクラス(スキル情報) 実装ファイル
///        Skill ライブラリを SQLite 正規化テーブル (sys_skill / sys_skill_heal / sys_skill_moveatack) に移行
///        派生クラス(HEAL, MOVEATACK)は各サブテーブルに展開。BLOB 折衷は使用しない。
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/04
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoSkillBase.h"
#include "InfoSkillHEAL.h"
#include "InfoSkillMOVEATACK.h"
#include "LibInfoSkill.h"
#include "SaveLoadInfoSkill.h"

// テーブル名
static const char* s_pszTableName         = "sys_skill";
static const char* s_pszTableNameHeal     = "sys_skill_heal";
static const char* s_pszTableNameMoveAtk  = "sys_skill_moveatack";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoSkill::CSaveLoadInfoSkill()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataSkill.dat");
	SetName("Skill");
}

CSaveLoadInfoSkill::~CSaveLoadInfoSkill()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   sys_skill       : 基底クラス共通フィールド
//   sys_skill_heal  : 回復スキル固有フィールド（SkillID が外部キー）
//   sys_skill_moveatack : 移動攻撃スキル固有フィールド
// ============================================================
void CSaveLoadInfoSkill::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// メインテーブル（基底クラス共通）
	const char* pszSqlMain =
		"CREATE TABLE IF NOT EXISTS sys_skill("
		"  SkillID   INTEGER PRIMARY KEY,"	// m_dwSkillID
		"  SP        INTEGER,"				// m_dwSP
		"  IconID    INTEGER,"				// m_dwIconID
		"  TypeMain  INTEGER,"				// m_nTypeMain
		"  TypeSub   INTEGER,"				// m_nTypeSub
		"  UseLimit  INTEGER,"				// m_nUse
		"  Name      TEXT"					// m_strName
		");";

	// 回復スキルサブテーブル
	const char* pszSqlHeal =
		"CREATE TABLE IF NOT EXISTS sys_skill_heal("
		"  SkillID      INTEGER PRIMARY KEY,"	// m_dwSkillID (メインテーブルへの参照)
		"  Area         INTEGER,"				// m_dwArea
		"  HealType     INTEGER,"				// m_dwHealType
		"  HitEffectID  INTEGER,"				// m_dwHitEffectID
		"  Value1       INTEGER,"				// m_dwValue1
		"  Value2       INTEGER,"				// m_dwValue2
		"  Distance     INTEGER"				// m_dwDistance
		");";

	// 移動攻撃スキルサブテーブル
	// m_adwEffectID[4] は固定長4要素なので 4 カラム展開（BLOB 不使用）
	const char* pszSqlMoveAtk =
		"CREATE TABLE IF NOT EXISTS sys_skill_moveatack("
		"  SkillID         INTEGER PRIMARY KEY,"	// m_dwSkillID
		"  TargetType      INTEGER,"				// m_dwTartgetType
		"  HitEffectID     INTEGER,"				// m_dwHitEffectID
		"  EffectID0       INTEGER,"				// m_adwEffectID[0]
		"  EffectID1       INTEGER,"				// m_adwEffectID[1]
		"  EffectID2       INTEGER,"				// m_adwEffectID[2]
		"  EffectID3       INTEGER,"				// m_adwEffectID[3]
		"  PutType         INTEGER,"				// m_dwPutType
		"  AliveTime       INTEGER,"				// m_dwAliveTime
		"  WaitTime        INTEGER,"				// m_dwWaitTime
		"  Value1          INTEGER,"				// m_dwValue1
		"  Value2          INTEGER,"				// m_dwValue2
		"  Distance        INTEGER,"				// m_dwDistance
		"  HitQuit         INTEGER,"				// m_bHitQuit
		"  DistanceDelete  INTEGER"				// m_bDistanceDelete
		");";

	sqlite3_exec(s_pDb, pszSqlMain,    NULL, NULL, NULL);
	sqlite3_exec(s_pDb, pszSqlHeal,    NULL, NULL, NULL);
	sqlite3_exec(s_pDb, pszSqlMoveAtk, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全行を正規化テーブルに全行置換
//   DELETE + prepared INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoSkill::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去（サブテーブルも含む）
	sqlite3_exec(s_pDb, "DELETE FROM sys_skill;",            NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_skill_heal;",       NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_skill_moveatack;",  NULL, NULL, NULL);

	// メインテーブル INSERT 用 prepared statement
	const char* pszInsertMain =
		"INSERT INTO sys_skill("
		"  SkillID, SP, IconID, TypeMain, TypeSub, UseLimit, Name"
		") VALUES(?,?,?,?,?,?,?);";

	// 回復スキルサブテーブル INSERT 用 prepared statement
	const char* pszInsertHeal =
		"INSERT INTO sys_skill_heal("
		"  SkillID, Area, HealType, HitEffectID, Value1, Value2, Distance"
		") VALUES(?,?,?,?,?,?,?);";

	// 移動攻撃スキルサブテーブル INSERT 用 prepared statement
	const char* pszInsertMoveAtk =
		"INSERT INTO sys_skill_moveatack("
		"  SkillID, TargetType, HitEffectID,"
		"  EffectID0, EffectID1, EffectID2, EffectID3,"
		"  PutType, AliveTime, WaitTime,"
		"  Value1, Value2, Distance, HitQuit, DistanceDelete"
		") VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";

	sqlite3_stmt* pStmtMain    = NULL;
	sqlite3_stmt* pStmtHeal    = NULL;
	sqlite3_stmt* pStmtMoveAtk = NULL;

	if (sqlite3_prepare_v2(s_pDb, pszInsertMain,    -1, &pStmtMain,    NULL) != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoSkill::SaveToNormalTable: prepare main failed\n");
		return;
	}
	if (sqlite3_prepare_v2(s_pDb, pszInsertHeal,    -1, &pStmtHeal,    NULL) != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoSkill::SaveToNormalTable: prepare heal failed\n");
		sqlite3_finalize(pStmtMain);
		return;
	}
	if (sqlite3_prepare_v2(s_pDb, pszInsertMoveAtk, -1, &pStmtMoveAtk, NULL) != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoSkill::SaveToNormalTable: prepare moveatack failed\n");
		sqlite3_finalize(pStmtMain);
		sqlite3_finalize(pStmtHeal);
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoSkillBase pBase = (PCInfoSkillBase)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pBase == NULL) continue;

		// ----- メインテーブル INSERT -----
		sqlite3_bind_int(pStmtMain, 1, (int)pBase->m_dwSkillID);
		sqlite3_bind_int(pStmtMain, 2, (int)pBase->m_dwSP);
		sqlite3_bind_int(pStmtMain, 3, (int)pBase->m_dwIconID);
		sqlite3_bind_int(pStmtMain, 4, (int)pBase->m_nTypeMain);
		sqlite3_bind_int(pStmtMain, 5, (int)pBase->m_nTypeSub);
		sqlite3_bind_int(pStmtMain, 6, (int)pBase->m_nUse);

		// m_strName: SJIS バイト列をそのまま TEXT として格納
		LPCSTR pszName = (LPCSTR)pBase->m_strName;
		sqlite3_bind_text(pStmtMain, 7, pszName, -1, SQLITE_TRANSIENT);

		sqlite3_step(pStmtMain);
		sqlite3_reset(pStmtMain);

		// ----- 派生クラス別サブテーブル INSERT -----
		if (pBase->m_nClassType == INFOSKILLTYPE_HEAL) {
			PCInfoSkillHEAL pHeal = (PCInfoSkillHEAL)pBase;
			sqlite3_bind_int(pStmtHeal, 1, (int)pHeal->m_dwSkillID);
			sqlite3_bind_int(pStmtHeal, 2, (int)pHeal->m_dwArea);
			sqlite3_bind_int(pStmtHeal, 3, (int)pHeal->m_dwHealType);
			sqlite3_bind_int(pStmtHeal, 4, (int)pHeal->m_dwHitEffectID);
			sqlite3_bind_int(pStmtHeal, 5, (int)pHeal->m_dwValue1);
			sqlite3_bind_int(pStmtHeal, 6, (int)pHeal->m_dwValue2);
			sqlite3_bind_int(pStmtHeal, 7, (int)pHeal->m_dwDistance);
			sqlite3_step(pStmtHeal);
			sqlite3_reset(pStmtHeal);
		}
		else if (pBase->m_nClassType == INFOSKILLTYPE_MOVEATACK) {
			PCInfoSkillMOVEATACK pMa = (PCInfoSkillMOVEATACK)pBase;
			sqlite3_bind_int(pStmtMoveAtk,  1, (int)pMa->m_dwSkillID);
			sqlite3_bind_int(pStmtMoveAtk,  2, (int)pMa->m_dwTartgetType);
			sqlite3_bind_int(pStmtMoveAtk,  3, (int)pMa->m_dwHitEffectID);
			sqlite3_bind_int(pStmtMoveAtk,  4, (int)pMa->m_adwEffectID[0]);
			sqlite3_bind_int(pStmtMoveAtk,  5, (int)pMa->m_adwEffectID[1]);
			sqlite3_bind_int(pStmtMoveAtk,  6, (int)pMa->m_adwEffectID[2]);
			sqlite3_bind_int(pStmtMoveAtk,  7, (int)pMa->m_adwEffectID[3]);
			sqlite3_bind_int(pStmtMoveAtk,  8, (int)pMa->m_dwPutType);
			sqlite3_bind_int(pStmtMoveAtk,  9, (int)pMa->m_dwAliveTime);
			sqlite3_bind_int(pStmtMoveAtk, 10, (int)pMa->m_dwWaitTime);
			sqlite3_bind_int(pStmtMoveAtk, 11, (int)pMa->m_dwValue1);
			sqlite3_bind_int(pStmtMoveAtk, 12, (int)pMa->m_dwValue2);
			sqlite3_bind_int(pStmtMoveAtk, 13, (int)pMa->m_dwDistance);
			sqlite3_bind_int(pStmtMoveAtk, 14, (int)pMa->m_bHitQuit);
			sqlite3_bind_int(pStmtMoveAtk, 15, (int)pMa->m_bDistanceDelete);
			sqlite3_step(pStmtMoveAtk);
			sqlite3_reset(pStmtMoveAtk);
		}
		// INFOSKILLTYPE_BASE（能力・生活など）はメインテーブルのみで完結
	}

	sqlite3_finalize(pStmtMain);
	sqlite3_finalize(pStmtHeal);
	sqlite3_finalize(pStmtMoveAtk);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
//   sys_skill を全行取得し、TypeMain/TypeSub で派生クラスを決定。
//   サブテーブルから派生フィールドを JOIN せず個別 SELECT で取得する。
// ============================================================
BOOL CSaveLoadInfoSkill::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	CLibInfoSkill* pLib = (CLibInfoSkill*)pDst;

	// メインテーブル SELECT
	const char* pszSqlMain =
		"SELECT SkillID, SP, IconID, TypeMain, TypeSub, UseLimit, Name"
		" FROM sys_skill;";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSqlMain, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	// HEAL サブテーブル SELECT（SkillID で引く）
	const char* pszSqlHeal =
		"SELECT Area, HealType, HitEffectID, Value1, Value2, Distance"
		" FROM sys_skill_heal WHERE SkillID=?;";

	// MOVEATACK サブテーブル SELECT（SkillID で引く）
	const char* pszSqlMoveAtk =
		"SELECT TargetType, HitEffectID,"
		"  EffectID0, EffectID1, EffectID2, EffectID3,"
		"  PutType, AliveTime, WaitTime,"
		"  Value1, Value2, Distance, HitQuit, DistanceDelete"
		" FROM sys_skill_moveatack WHERE SkillID=?;";

	sqlite3_stmt* pStmtHeal    = NULL;
	sqlite3_stmt* pStmtMoveAtk = NULL;
	sqlite3_prepare_v2(s_pDb, pszSqlHeal,    -1, &pStmtHeal,    NULL);
	sqlite3_prepare_v2(s_pDb, pszSqlMoveAtk, -1, &pStmtMoveAtk, NULL);

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		DWORD dwSkillID  = (DWORD)sqlite3_column_int(pStmt, 0);
		DWORD dwSP       = (DWORD)sqlite3_column_int(pStmt, 1);
		DWORD dwIconID   = (DWORD)sqlite3_column_int(pStmt, 2);
		int   nTypeMain  = sqlite3_column_int(pStmt, 3);
		int   nTypeSub   = sqlite3_column_int(pStmt, 4);
		int   nUse       = sqlite3_column_int(pStmt, 5);

		// TypeMain/TypeSub で適切な派生クラスを生成
		PCInfoSkillBase pBase = (PCInfoSkillBase)pLib->GetNew(nTypeMain, nTypeSub);
		if (pBase == NULL) continue;

		// 基底フィールドを設定（Add() 前に SkillID をセット）
		pBase->m_dwSkillID  = dwSkillID;
		pBase->m_dwSP       = dwSP;
		pBase->m_dwIconID   = dwIconID;
		pBase->m_nTypeMain  = nTypeMain;
		pBase->m_nTypeSub   = nTypeSub;
		pBase->m_nUse       = nUse;

		// Name: SJIS バイト列として格納されているので LegacyAnsiToTString で変換
		const char* pszName = (const char*)sqlite3_column_text(pStmt, 6);
		if (pszName != NULL) {
			pBase->m_strName = (LPCTSTR)LegacyAnsiToTString(pszName);
		}

		// ----- 派生フィールドをサブテーブルから取得 -----
		if (pBase->m_nClassType == INFOSKILLTYPE_HEAL && pStmtHeal != NULL) {
			PCInfoSkillHEAL pHeal = (PCInfoSkillHEAL)pBase;
			sqlite3_bind_int(pStmtHeal, 1, (int)dwSkillID);
			if (sqlite3_step(pStmtHeal) == SQLITE_ROW) {
				pHeal->m_dwArea         = (DWORD)sqlite3_column_int(pStmtHeal, 0);
				pHeal->m_dwHealType     = (DWORD)sqlite3_column_int(pStmtHeal, 1);
				pHeal->m_dwHitEffectID  = (DWORD)sqlite3_column_int(pStmtHeal, 2);
				pHeal->m_dwValue1       = (DWORD)sqlite3_column_int(pStmtHeal, 3);
				pHeal->m_dwValue2       = (DWORD)sqlite3_column_int(pStmtHeal, 4);
				pHeal->m_dwDistance     = (DWORD)sqlite3_column_int(pStmtHeal, 5);
			}
			sqlite3_reset(pStmtHeal);
		}
		else if (pBase->m_nClassType == INFOSKILLTYPE_MOVEATACK && pStmtMoveAtk != NULL) {
			PCInfoSkillMOVEATACK pMa = (PCInfoSkillMOVEATACK)pBase;
			sqlite3_bind_int(pStmtMoveAtk, 1, (int)dwSkillID);
			if (sqlite3_step(pStmtMoveAtk) == SQLITE_ROW) {
				pMa->m_dwTartgetType     = (DWORD)sqlite3_column_int(pStmtMoveAtk,  0);
				pMa->m_dwHitEffectID     = (DWORD)sqlite3_column_int(pStmtMoveAtk,  1);
				pMa->m_adwEffectID[0]    = (DWORD)sqlite3_column_int(pStmtMoveAtk,  2);
				pMa->m_adwEffectID[1]    = (DWORD)sqlite3_column_int(pStmtMoveAtk,  3);
				pMa->m_adwEffectID[2]    = (DWORD)sqlite3_column_int(pStmtMoveAtk,  4);
				pMa->m_adwEffectID[3]    = (DWORD)sqlite3_column_int(pStmtMoveAtk,  5);
				pMa->m_dwPutType         = (DWORD)sqlite3_column_int(pStmtMoveAtk,  6);
				pMa->m_dwAliveTime       = (DWORD)sqlite3_column_int(pStmtMoveAtk,  7);
				pMa->m_dwWaitTime        = (DWORD)sqlite3_column_int(pStmtMoveAtk,  8);
				pMa->m_dwValue1          = (DWORD)sqlite3_column_int(pStmtMoveAtk,  9);
				pMa->m_dwValue2          = (DWORD)sqlite3_column_int(pStmtMoveAtk, 10);
				pMa->m_dwDistance        = (DWORD)sqlite3_column_int(pStmtMoveAtk, 11);
				pMa->m_bHitQuit          = (BOOL) sqlite3_column_int(pStmtMoveAtk, 12);
				pMa->m_bDistanceDelete   = (BOOL) sqlite3_column_int(pStmtMoveAtk, 13);
			}
			sqlite3_reset(pStmtMoveAtk);
		}

		// ライブラリに追加（m_dwSkillID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pBase);
		nRowCount++;
	}

	sqlite3_finalize(pStmt);
	if (pStmtHeal    != NULL) sqlite3_finalize(pStmtHeal);
	if (pStmtMoveAtk != NULL) sqlite3_finalize(pStmtMoveAtk);

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoSkill::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'Skill' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='Skill';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoSkill: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoSkill::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoSkill::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoSkill: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoSkill: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
