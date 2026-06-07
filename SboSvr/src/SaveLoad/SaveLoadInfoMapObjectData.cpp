/// @file SaveLoadInfoMapObjectData.cpp
/// @brief データ保存・読み込みクラス(マップオブジェクト配置データ) 実装ファイル
///        各 CInfoMapBase の m_pLibInfoMapObjectData を SQLite 正規化テーブル
///        (sys_map_objectdata) に保存・復元する。
///
///  テーブル構成:
///    sys_map_objectdata ─ マップオブジェクト配置データ（1配置 = 1行）
///      (MapID, DataID) を複合主キーとする。
///
///  保存対象フィールド:
///    m_dwDataID   → DataID
///    m_dwObjectID → ObjectID
///    m_ptPos.x    → PosX
///    m_ptPos.y    → PosY
///
///  マイグレーション:
///    sys_map_objectdata が空（初回・既存DB移行直後など）の場合、
///    (a) メモリ上に配置データが存在すれば SaveAllObjects() で書き戻す
///        （新規DBで CSaveLoadInfoMap が .dat から配置込みで読込済みのケース）。
///    (b) メモリにも無い場合はレガシー SboDataMap.dat から一時読込して取り込む
///        （既存DBで dat が消去済みの場合の復旧）。
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2026/06/02
/// @copyright Copyright(C)URARA-works 2026

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoMapBase.h"
#include "LibInfoMapBase.h"
#include "LibInfoMapObjectData.h"
#include "InfoMapObjectData.h"
#include "SaveLoadInfoBase.h"
#include "SaveLoadInfoMap.h"
#include "SaveLoadInfoMapObjectData.h"

// テーブル名
static const char* s_pszTableObj = "sys_map_objectdata";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoMapObjectData::CSaveLoadInfoMapObjectData()
{
}

CSaveLoadInfoMapObjectData::~CSaveLoadInfoMapObjectData()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
// ============================================================
void CSaveLoadInfoMapObjectData::EnsureTable(void)
{
	if (CSaveLoadInfoBase::s_pDb == NULL) return;

	const char* pszSql =
		"CREATE TABLE IF NOT EXISTS sys_map_objectdata("
		"  MapID    INTEGER NOT NULL,"	// 所属マップID
		"  DataID   INTEGER NOT NULL,"	// m_dwDataID（配置データID）
		"  ObjectID INTEGER,"			// m_dwObjectID（オブジェクトID）
		"  PosX     INTEGER,"			// m_ptPos.x
		"  PosY     INTEGER,"			// m_ptPos.y
		"  PRIMARY KEY (MapID, DataID)"
		");";

	sqlite3_exec(CSaveLoadInfoBase::s_pDb, pszSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全マップ配置データを全行置換
//   DELETE + prepared INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoMapObjectData::SaveAllObjects(CLibInfoMapBase *pLibInfoMap)
{
	if (CSaveLoadInfoBase::s_pDb == NULL || pLibInfoMap == NULL) return;

	// 既存行を全消去
	sqlite3_exec(CSaveLoadInfoBase::s_pDb, "DELETE FROM sys_map_objectdata;", NULL, NULL, NULL);

	// INSERT 文を準備
	const char* pszInsert =
		"INSERT INTO sys_map_objectdata("
		"  MapID, DataID, ObjectID, PosX, PosY"
		") VALUES(?,?,?,?,?);";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(CSaveLoadInfoBase::s_pDb, pszInsert, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMapObjectData::SaveAllObjects: prepare failed\n");
		return;
	}

	int nMapCount = pLibInfoMap->GetCount();
	for (int i = 0; i < nMapCount; i++) {
		PCInfoMapBase pInfoMap = (PCInfoMapBase)pLibInfoMap->GetPtr(i);
		if (pInfoMap == NULL) continue;
		if (pInfoMap->m_pLibInfoMapObjectData == NULL) continue;

		int nObjCount = pInfoMap->m_pLibInfoMapObjectData->GetCount();
		DWORD dwMapID = pInfoMap->m_dwMapID;

		for (int j = 0; j < nObjCount; j++) {
			PCInfoMapObjectData pData = (PCInfoMapObjectData)pInfoMap->m_pLibInfoMapObjectData->GetPtr(j);
			if (pData == NULL) continue;

			sqlite3_bind_int(pStmt, 1, (int)dwMapID);
			sqlite3_bind_int(pStmt, 2, (int)pData->m_dwDataID);
			sqlite3_bind_int(pStmt, 3, (int)pData->m_dwObjectID);
			sqlite3_bind_int(pStmt, 4, (int)pData->m_ptPos.x);
			sqlite3_bind_int(pStmt, 5, (int)pData->m_ptPos.y);

			sqlite3_step(pStmt);
			sqlite3_reset(pStmt);
		}
	}

	sqlite3_finalize(pStmt);
	OutputDebugStringA("SaveLoadInfoMapObjectData: sys_map_objectdata に保存完了\n");
}

// ============================================================
// 内部ヘルパー: sys_map_objectdata から全行読み込んで各マップに復元
//   1行以上読めたら TRUE を返す
// ============================================================
BOOL CSaveLoadInfoMapObjectData::LoadAllObjects(CLibInfoMapBase *pLibInfoMap)
{
	if (CSaveLoadInfoBase::s_pDb == NULL || pLibInfoMap == NULL) return FALSE;

	const char* pszSelect =
		"SELECT MapID, DataID, ObjectID, PosX, PosY"
		" FROM sys_map_objectdata"
		" ORDER BY MapID, DataID;";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(CSaveLoadInfoBase::s_pDb, pszSelect, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		DWORD dwMapID   = (DWORD)sqlite3_column_int(pStmt, 0);
		DWORD dwDataID  = (DWORD)sqlite3_column_int(pStmt, 1);

		// 対象マップを取得（DWORD 版 GetPtr でマップIDから検索）
		PCInfoMapBase pInfoMap = (PCInfoMapBase)pLibInfoMap->GetPtr(dwMapID);
		if (pInfoMap == NULL || pInfoMap->m_pLibInfoMapObjectData == NULL) continue;

		// 新規配置データを生成
		PCInfoMapObjectData pData = (PCInfoMapObjectData)pInfoMap->m_pLibInfoMapObjectData->GetNew();
		if (pData == NULL) continue;

		// ---- DataID を Add() より前にセット ----
		//   Add() 内で m_dwDataID == 0 だと GetNewID() で上書きされてしまうため
		pData->m_dwDataID  = dwDataID;

		// ---- 各フィールドを復元 ----
		pData->m_dwObjectID = (DWORD)sqlite3_column_int(pStmt, 2);
		pData->m_ptPos.x    = (LONG)sqlite3_column_int(pStmt, 3);
		pData->m_ptPos.y    = (LONG)sqlite3_column_int(pStmt, 4);

		// ライブラリに追加（m_dwDataID が 0 でないので GetNewID() は呼ばれない）
		pInfoMap->m_pLibInfoMapObjectData->Add(pData);
		nRowCount++;
	}

	sqlite3_finalize(pStmt);

	if (nRowCount > 0) {
		OutputDebugStringA("SaveLoadInfoMapObjectData: sys_map_objectdata から読み込み成功\n");
	}
	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: 配置データを sys_map_objectdata へ書き戻す
//   (a) メモリ上に配置がある場合: そのまま SaveAllObjects() で書き戻す。
//   (b) メモリにも無い場合: レガシー .dat から一時読込して取り込む（既存DB復旧）。
// ============================================================
void CSaveLoadInfoMapObjectData::MigrateData(CLibInfoMapBase *pLibInfoMap)
{
	if (pLibInfoMap == NULL) return;

	// (a) メモリ上に配置データが存在するか確認
	int nTotalObj = 0;
	int nMapCount = pLibInfoMap->GetCount();
	for (int i = 0; i < nMapCount; i++) {
		PCInfoMapBase pInfoMap = (PCInfoMapBase)pLibInfoMap->GetPtr(i);
		if (pInfoMap == NULL || pInfoMap->m_pLibInfoMapObjectData == NULL) continue;
		nTotalObj += pInfoMap->m_pLibInfoMapObjectData->GetCount();
	}

	if (nTotalObj > 0) {
		// メモリ → sys_map_objectdata へ書き戻し（新規DB、.dat から配置込みで読込済みのケース）
		SaveAllObjects(pLibInfoMap);
		OutputDebugStringA("SaveLoadInfoMapObjectData: メモリ → sys_map_objectdata へマイグレーション完了\n");
		return;
	}

	// (b) メモリにも無い場合: レガシー .dat から一時読込して本番マップに取り込む
	//   CSaveLoadInfoMap のコンストラクタが SetFileName/SetName 済みのため、
	//   基底 CSaveLoadInfoBase::Load() を明示修飾で呼ぶと
	//   sbo_data 'Map' → なければ SboDataMap.dat フォールバックで配置込みの全マップを読む。
	OutputDebugStringA("SaveLoadInfoMapObjectData: .dat からレガシー復旧を試みます\n");

	CLibInfoMapBase tempLib;
	tempLib.Create(NULL);	// MapParts なしで作成（配置データ取得のみが目的）

	CSaveLoadInfoMap tempSL;	// コンストラクタで SetFileName/SetName 済み
	// sys_map 経路の override を回避して sbo_data / .dat フォールバックを使う
	tempSL.CSaveLoadInfoBase::Load((PCLibInfoBase)&tempLib);

	if (tempLib.GetCount() <= 0) {
		// .dat も存在しない（またはマップ0件） → 現状維持（ログのみ）
		OutputDebugStringA("SaveLoadInfoMapObjectData: .dat からのマップ読込なし、マイグレーションをスキップ\n");
		tempLib.Destroy();
		return;
	}

	// 一時 lib の各マップから本番マップへ配置データを Merge
	int nTempMapCount = tempLib.GetCount();
	for (int i = 0; i < nTempMapCount; i++) {
		PCInfoMapBase pTempMap = (PCInfoMapBase)tempLib.GetPtr(i);
		if (pTempMap == NULL || pTempMap->m_pLibInfoMapObjectData == NULL) continue;
		if (pTempMap->m_pLibInfoMapObjectData->GetCount() <= 0) continue;

		// 同 MapID の本番マップを取得
		PCInfoMapBase pDstMap = (PCInfoMapBase)pLibInfoMap->GetPtr(pTempMap->m_dwMapID);
		if (pDstMap == NULL || pDstMap->m_pLibInfoMapObjectData == NULL) continue;

		// 本番マップへ配置データを取り込む
		pDstMap->m_pLibInfoMapObjectData->Merge(pTempMap->m_pLibInfoMapObjectData);
	}

	// 本番マップに取り込んだ配置データを永続化
	SaveAllObjects(pLibInfoMap);
	OutputDebugStringA("SaveLoadInfoMapObjectData: .dat → sys_map_objectdata へマイグレーション完了\n");

	tempLib.Destroy();
}

// ============================================================
// Save: 全マップの配置データを sys_map_objectdata に書き込み
//   トランザクションは MgrData 側で BEGIN/COMMIT している
// ============================================================
void CSaveLoadInfoMapObjectData::Save(CLibInfoMapBase *pLibInfoMap)
{
	if (pLibInfoMap == NULL) return;

	// テーブルが無ければ作成
	EnsureTable();

	// 全行置換して保存
	SaveAllObjects(pLibInfoMap);
}

// ============================================================
// Load: sys_map_objectdata を読み込んで各マップの m_pLibInfoMapObjectData に投入
//   テーブルが空かつデータが未投入の場合はマイグレーションを行う。
// ============================================================
void CSaveLoadInfoMapObjectData::Load(CLibInfoMapBase *pLibInfoMap)
{
	if (pLibInfoMap == NULL) return;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadAllObjects(pLibInfoMap)) {
		return;
	}

	// 2. テーブルが空 → メモリ上のデータまたは .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoMapObjectData: sys_map_objectdata が空 → メモリ/.dat からマイグレーション\n");
	MigrateData(pLibInfoMap);
}
