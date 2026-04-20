/// @file SaveLoadInfoSystem.cpp
/// @brief データ保存・読み込みクラス(システム情報) 実装ファイル
///        System ライブラリのみ SQLite 正規化テーブル (sys_init_char_status) に移行
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/09/28
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoSystem.h"
#include "LibInfoSystem.h"
#include "SaveLoadInfoSystem.h"

// ============================================================
// 正規化テーブル用スキーマ定義
//   カラム名 / SQLite 型 / 構造体内バイトサイズ / オフセット
//   POINT (ptInitPos) は x / y に 2 分割する
// ============================================================
struct SQL_COL_DEF {
	const char*	pszName;	// カラム名
	const char*	pszType;	// SQLite 型文字列
	int		nByteSize;	// バインド時に読むバイト数
	size_t		nOffset;	// STSYSTEM_INITCHARSTATUS 内オフセット
};

// STSYSTEM_INITCHARSTATUS 内の POINT::x, POINT::y のオフセットを個別に計算するための
// ダミー変数（offsetof は POD 型にのみ確実に使えるので static 変数経由で取得）
static STSYSTEM_INITCHARSTATUS s_dummy;

// 25 カラム分の定義（インデックス 0〜22: 通常フィールド、23: ptInitPos_x、24: ptInitPos_y）
static const SQL_COL_DEF s_aColDef[] = {
	// name                  type       bytes   offsetof
	{ "wStamina",            "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wStamina)           },
	{ "wPower",              "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wPower)             },
	{ "wStrength",           "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wStrength)         },
	{ "wMagic",              "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wMagic)             },
	{ "wSkillful",           "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wSkillful)         },
	{ "wAbillityAT",         "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wAbillityAT)       },
	{ "wAbillityDF",         "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wAbillityDF)       },
	{ "wPAtack",             "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wPAtack)           },
	{ "wPDefense",           "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wPDefense)         },
	{ "wPMagic",             "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wPMagic)           },
	{ "wPMagicDefense",      "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wPMagicDefense)   },
	{ "wPHitAverage",        "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wPHitAverage)     },
	{ "wPAvoidAverage",      "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wPAvoidAverage)   },
	{ "wPCriticalAverage",   "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wPCriticalAverage)},
	{ "wAttrFire",           "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wAttrFire)         },
	{ "wAttrWind",           "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wAttrWind)         },
	{ "wAttrWater",          "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wAttrWater)        },
	{ "wAttrEarth",          "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wAttrEarth)        },
	{ "wAttrLight",          "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wAttrLight)        },
	{ "wAttrDark",           "INTEGER", 2, offsetof(STSYSTEM_INITCHARSTATUS, wAttrDark)         },
	{ "dwMaxHP",             "INTEGER", 4, offsetof(STSYSTEM_INITCHARSTATUS, dwMaxHP)           },
	{ "dwMaxSP",             "INTEGER", 4, offsetof(STSYSTEM_INITCHARSTATUS, dwMaxSP)           },
	{ "dwInitPosMapID",      "INTEGER", 4, offsetof(STSYSTEM_INITCHARSTATUS, dwInitPosMapID)   },
	// POINT を x / y に分解（LONG = 4byte）
	{ "ptInitPos_x",         "INTEGER", 4, offsetof(STSYSTEM_INITCHARSTATUS, ptInitPos.x)      },
	{ "ptInitPos_y",         "INTEGER", 4, offsetof(STSYSTEM_INITCHARSTATUS, ptInitPos.y)      },
};
static const int s_nColCount = (int)(sizeof(s_aColDef) / sizeof(s_aColDef[0]));

// テーブル名
static const char* s_pszTableName = "sys_init_char_status";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoSystem::CSaveLoadInfoSystem()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataSystemInfo.dat");
	SetName("System");
}

CSaveLoadInfoSystem::~CSaveLoadInfoSystem()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
// ============================================================
void CSaveLoadInfoSystem::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// CREATE TABLE IF NOT EXISTS を動的に組み立てる
	char szSql[2048];
	int nLen = 0;
	nLen += sprintf_s(szSql + nLen, sizeof(szSql) - nLen,
		"CREATE TABLE IF NOT EXISTS %s("
		"id INTEGER PRIMARY KEY",
		s_pszTableName);

	for (int i = 0; i < s_nColCount; i++) {
		nLen += sprintf_s(szSql + nLen, sizeof(szSql) - nLen,
			", %s %s", s_aColDef[i].pszName, s_aColDef[i].pszType);
	}
	nLen += sprintf_s(szSql + nLen, sizeof(szSql) - nLen, ");");

	sqlite3_exec(s_pDb, szSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ値を正規化テーブルに INSERT OR REPLACE（id=1 固定）
// ============================================================
void CSaveLoadInfoSystem::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// LibInfoSystem から STSYSTEM_INITCHARSTATUS ポインタを取得
	PCInfoSystem pInfoSys = (PCInfoSystem)m_pLibInfoBase->GetPtr(0);
	if (pInfoSys == NULL || pInfoSys->m_pInitCharStatus == NULL) return;

	const BYTE* pBase = (const BYTE*)pInfoSys->m_pInitCharStatus;

	// INSERT OR REPLACE INTO sys_init_char_status(id, col0, col1, ...) VALUES(?, ?, ...)
	char szSql[2048];
	int nLen = 0;
	nLen += sprintf_s(szSql + nLen, sizeof(szSql) - nLen,
		"INSERT OR REPLACE INTO %s(id", s_pszTableName);
	for (int i = 0; i < s_nColCount; i++) {
		nLen += sprintf_s(szSql + nLen, sizeof(szSql) - nLen,
			", %s", s_aColDef[i].pszName);
	}
	nLen += sprintf_s(szSql + nLen, sizeof(szSql) - nLen, ") VALUES(?");
	for (int i = 0; i < s_nColCount; i++) {
		nLen += sprintf_s(szSql + nLen, sizeof(szSql) - nLen, ", ?");
	}
	nLen += sprintf_s(szSql + nLen, sizeof(szSql) - nLen, ");");

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, szSql, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoSystem::SaveToNormalTable: prepare failed\n");
		return;
	}

	// id = 1 を固定バインド
	sqlite3_bind_int(pStmt, 1, 1);

	// 各カラムを INTEGER としてバインド
	for (int i = 0; i < s_nColCount; i++) {
		const SQL_COL_DEF& col = s_aColDef[i];
		const BYTE* pField = pBase + col.nOffset;
		sqlite3_int64 nVal = 0;

		if (col.nByteSize == 2) {
			// WORD (符号なし 16bit) → sqlite3_int64
			WORD w;
			CopyMemory(&w, pField, 2);
			nVal = (sqlite3_int64)w;
		} else {
			// DWORD / LONG (4byte)
			DWORD dw;
			CopyMemory(&dw, pField, 4);
			nVal = (sqlite3_int64)dw;
		}
		sqlite3_bind_int64(pStmt, i + 2, nVal);	// パラメータインデックスは 1 始まり（id が 1 なので +2）
	}

	sqlite3_step(pStmt);
	sqlite3_finalize(pStmt);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（成功時 TRUE）
// ============================================================
BOOL CSaveLoadInfoSystem::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	char szSql[256];
	sprintf_s(szSql, "SELECT * FROM %s WHERE id=1;", s_pszTableName);

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, szSql, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	BOOL bSuccess = FALSE;
	if (sqlite3_step(pStmt) == SQLITE_ROW) {
		// 空のデータを 1 行追加してから取得
		PCInfoBase pInfoBase = pDst->GetNew();
		pDst->Add(pInfoBase);

		PCInfoSystem pInfoSys = (PCInfoSystem)pDst->GetPtr(0);
		if (pInfoSys != NULL && pInfoSys->m_pInitCharStatus != NULL) {
			BYTE* pBase = (BYTE*)pInfoSys->m_pInitCharStatus;

			// SELECT * の結果: 列 0 = id, 列 1〜25 = データカラム
			for (int i = 0; i < s_nColCount; i++) {
				const SQL_COL_DEF& col = s_aColDef[i];
				BYTE* pField = pBase + col.nOffset;
				sqlite3_int64 nVal = sqlite3_column_int64(pStmt, i + 1);

				if (col.nByteSize == 2) {
					WORD w = (WORD)(nVal & 0xFFFF);
					CopyMemory(pField, &w, 2);
				} else {
					DWORD dw = (DWORD)(nVal & 0xFFFFFFFF);
					CopyMemory(pField, &dw, 4);
				}
			}
			bSuccess = TRUE;
		}
	}

	sqlite3_finalize(pStmt);
	pDst->RenewIDPtr();
	return bSuccess;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data）から読み込んでマイグレーション
//   - sbo_data に 'System' 行があれば BLOB 経路で読み、
//     正規化テーブルに INSERT して sbo_data から削除する
//   - .dat フォールバックも同様に処理する
// ============================================================
BOOL CSaveLoadInfoSystem::MigrateFromBlob(PCLibInfoBase pDst)
{
	// 親クラスの Load() を呼んで BLOB / .dat を読む
	CSaveLoadInfoBase::Load(pDst);

	// pDst に要素が追加されていれば読み込み成功 → 正規化テーブルに移行
	PCInfoSystem pInfoSys = (PCInfoSystem)pDst->GetPtr(0);
	if (pInfoSys == NULL || pInfoSys->m_pInitCharStatus == NULL) {
		return FALSE;
	}

	// 正規化テーブルへ書き込み
	m_pLibInfoBase = pDst;
	SaveToNormalTable();

	// sbo_data から 'System' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='System';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoSystem: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoSystem::Save(PCLibInfoBase pSrc)
{
	if (pSrc == NULL) return;

	m_pLibInfoBase = pSrc;

	// テーブルが無ければ作成
	EnsureTable();

	// 正規化テーブルへ保存
	SaveToNormalTable();
	// sbo_data の 'System' 行には書かない
}

// ============================================================
// Load のオーバーライド: 正規化テーブルから読む（マイグレーション付き）
// ============================================================
void CSaveLoadInfoSystem::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoSystem: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoSystem: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
