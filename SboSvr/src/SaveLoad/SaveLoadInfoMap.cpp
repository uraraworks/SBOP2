/// @file SaveLoadInfoMap.cpp
/// @brief データ保存・読み込みクラス(マップ情報) 実装ファイル
///        Map ライブラリを SQLite 正規化テーブル (sys_map) に移行
///
///  テーブル構成:
///    sys_map ─ マップ基本情報（1マップ = 1行）
///      MapID        INTEGER PRIMARY KEY  (m_dwMapID)
///      MapCX        INTEGER              (m_sizeMap.cx)
///      MapCY        INTEGER              (m_sizeMap.cy)
///      BGMID        INTEGER              (m_dwBGMID)
///      WeatherType  INTEGER              (m_dwWeatherType)
///      EnableBattle INTEGER              (m_bEnableBattle)
///      Recovery     INTEGER              (m_bRecovery)
///      Level        INTEGER              (m_byLevel)
///      MapName      TEXT                 (m_strMapName、SJIS バイト列)
///      MapData      BLOB                 (m_pwMap      : cx*cy 個の WORD、折衷 BLOB)
///      MapPileData  BLOB                 (m_pwMapPile  : cx*cy 個の WORD、折衷 BLOB)
///      MapShadowData BLOB               (m_pwMapShadow: cx*cy 個の WORD、折衷 BLOB)
///
///  折衷 BLOB 方針:
///    m_pwMap / m_pwMapPile / m_pwMapShadow はマップサイズ (cx*cy) に依存する
///    可変長 WORD 配列。1タイル1行に展開すると大規模マップで膨大な行数になるため
///    バイナリのまま BLOB カラム 1 本に格納する。
///    cx, cy 自体はスカラーカラムに保持する。
///
///  マップイベント / オブジェクト配置データは各 SaveLoad クラスが個別に保存するため
///  本クラスでは読み書きしない（Init() 後の状態で記録し、ロード後に別クラスが復元する）。
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/11/08
/// @copyright Copyright(C)URARA-works 2005

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoMapBase.h"
#include "LibInfoMapBase.h"
#include "SaveLoadInfoMap.h"

// テーブル名
static const char* s_pszTableMain = "sys_map";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoMap::CSaveLoadInfoMap()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataMap.dat");
	SetName("Map");
}

CSaveLoadInfoMap::~CSaveLoadInfoMap()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   sys_map:
//     MapID を主キーとする基本情報テーブル。
//     タイル配列 3 本（Map / MapPile / MapShadow）は cx*cy 個の WORD を
//     BLOB カラムに格納する（折衷 BLOB 方式）。
// ============================================================
void CSaveLoadInfoMap::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	const char* pszSql =
		"CREATE TABLE IF NOT EXISTS sys_map("
		"  MapID         INTEGER PRIMARY KEY,"	// m_dwMapID
		"  MapCX         INTEGER,"				// m_sizeMap.cx
		"  MapCY         INTEGER,"				// m_sizeMap.cy
		"  BGMID         INTEGER,"				// m_dwBGMID
		"  WeatherType   INTEGER,"				// m_dwWeatherType
		"  EnableBattle  INTEGER,"				// m_bEnableBattle (戦闘許可)
		"  Recovery      INTEGER,"				// m_bRecovery (気絶後回復)
		"  Level         INTEGER,"				// m_byLevel (暗さレベル)
		"  MapName       TEXT,"					// m_strMapName (SJIS バイト列)
		"  MapData       BLOB,"					// m_pwMap       (cx*cy WORD、折衷 BLOB)
		"  MapPileData   BLOB,"					// m_pwMapPile   (cx*cy WORD、折衷 BLOB)
		"  MapShadowData BLOB"					// m_pwMapShadow (cx*cy WORD、折衷 BLOB)
		");";

	sqlite3_exec(s_pDb, pszSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全マップ情報を正規化テーブルに全行置換
//   DELETE + prepared INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoMap::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去
	sqlite3_exec(s_pDb, "DELETE FROM sys_map;", NULL, NULL, NULL);

	const char* pszInsert =
		"INSERT INTO sys_map("
		"  MapID, MapCX, MapCY, BGMID, WeatherType,"
		"  EnableBattle, Recovery, Level, MapName,"
		"  MapData, MapPileData, MapShadowData"
		") VALUES(?,?,?,?,?,?,?,?,?,?,?,?);";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsert, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMap::SaveToNormalTable: prepare failed\n");
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoMapBase pInfo = (PCInfoMapBase)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		int nMapCX = (int)pInfo->m_sizeMap.cx;
		int nMapCY = (int)pInfo->m_sizeMap.cy;
		int nTileCount = nMapCX * nMapCY;					// タイル総数
		int nBlobSize  = nTileCount * (int)sizeof(WORD);	// BLOB バイト数

		sqlite3_bind_int(pStmt, 1,  (int)pInfo->m_dwMapID);
		sqlite3_bind_int(pStmt, 2,  nMapCX);
		sqlite3_bind_int(pStmt, 3,  nMapCY);
		sqlite3_bind_int(pStmt, 4,  (int)pInfo->m_dwBGMID);
		sqlite3_bind_int(pStmt, 5,  (int)pInfo->m_dwWeatherType);
		sqlite3_bind_int(pStmt, 6,  (int)pInfo->m_bEnableBattle);
		sqlite3_bind_int(pStmt, 7,  (int)pInfo->m_bRecovery);
		sqlite3_bind_int(pStmt, 8,  (int)pInfo->m_byLevel);

		// m_strMapName: SJIS バイト列をそのまま TEXT としてバインド
		LPCSTR pszName = (LPCSTR)pInfo->m_strMapName;
		sqlite3_bind_text(pStmt, 9, pszName, -1, SQLITE_TRANSIENT);

		// m_pwMap: cx*cy 個の WORD を BLOB 格納（折衷 BLOB）
		if (pInfo->m_pwMap != NULL && nBlobSize > 0) {
			sqlite3_bind_blob(pStmt, 10, pInfo->m_pwMap, nBlobSize, SQLITE_TRANSIENT);
		} else {
			sqlite3_bind_null(pStmt, 10);
		}

		// m_pwMapPile: cx*cy 個の WORD を BLOB 格納（折衷 BLOB）
		if (pInfo->m_pwMapPile != NULL && nBlobSize > 0) {
			sqlite3_bind_blob(pStmt, 11, pInfo->m_pwMapPile, nBlobSize, SQLITE_TRANSIENT);
		} else {
			sqlite3_bind_null(pStmt, 11);
		}

		// m_pwMapShadow: cx*cy 個の WORD を BLOB 格納（折衷 BLOB）
		if (pInfo->m_pwMapShadow != NULL && nBlobSize > 0) {
			sqlite3_bind_blob(pStmt, 12, pInfo->m_pwMapShadow, nBlobSize, SQLITE_TRANSIENT);
		} else {
			sqlite3_bind_null(pStmt, 12);
		}

		sqlite3_step(pStmt);
		sqlite3_reset(pStmt);
	}

	sqlite3_finalize(pStmt);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
// ============================================================
BOOL CSaveLoadInfoMap::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	const char* pszSelect =
		"SELECT"
		"  MapID, MapCX, MapCY, BGMID, WeatherType,"
		"  EnableBattle, Recovery, Level, MapName,"
		"  MapData, MapPileData, MapShadowData"
		" FROM sys_map;";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSelect, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		// 新規 CInfoMapBase を作成（Create() は LibInfoMapBase::GetNew() 内で呼ばれる）
		PCInfoMapBase pInfo = (PCInfoMapBase)pDst->GetNew();
		if (pInfo == NULL) continue;

		// MapID を先にセットしてから Add() を呼ぶ
		//   ← Add() 内で m_dwMapID == 0 なら GetNewID() で上書きされてしまうため
		DWORD dwMapID = (DWORD)sqlite3_column_int(pStmt, 0);
		pInfo->m_dwMapID = dwMapID;

		int nMapCX = sqlite3_column_int(pStmt, 1);
		int nMapCY = sqlite3_column_int(pStmt, 2);

		pInfo->m_dwBGMID        = (DWORD)sqlite3_column_int(pStmt, 3);
		pInfo->m_dwWeatherType  = (DWORD)sqlite3_column_int(pStmt, 4);
		pInfo->m_bEnableBattle  = (BOOL) sqlite3_column_int(pStmt, 5);
		pInfo->m_bRecovery      = (BOOL) sqlite3_column_int(pStmt, 6);
		pInfo->m_byLevel        = (BYTE) sqlite3_column_int(pStmt, 7);

		// MapName: SJIS バイト列として格納されているので LegacyAnsiToTString で変換
		const char* pszName = (const char*)sqlite3_column_text(pStmt, 8);
		if (pszName != NULL) {
			pInfo->m_strMapName = (LPCTSTR)LegacyAnsiToTString(pszName);
		}

		// Init() でタイル配列を確保しマップイベント/HitTmp も初期化する
		//   ※ Init() は m_pLibInfoMapEvent / m_pLibInfoMapObjectData も new するため
		//      最初に呼んでからデータを復元する。
		//      Init() の wParts=0 は仮値（直後に BLOB で上書きするため問題なし）
		pInfo->Init(nMapCX, nMapCY, 0, TRUE);

		int nBlobSize = nMapCX * nMapCY * (int)sizeof(WORD);

		// MapData BLOB → m_pwMap 復元
		const void* pMapBlob     = sqlite3_column_blob(pStmt, 9);
		int         nMapBlobSize = sqlite3_column_bytes(pStmt, 9);
		if (pMapBlob != NULL && pInfo->m_pwMap != NULL && nMapBlobSize == nBlobSize) {
			CopyMemory(pInfo->m_pwMap, pMapBlob, nBlobSize);
		}

		// MapPileData BLOB → m_pwMapPile 復元
		const void* pPileBlob     = sqlite3_column_blob(pStmt, 10);
		int         nPileBlobSize = sqlite3_column_bytes(pStmt, 10);
		if (pPileBlob != NULL && pInfo->m_pwMapPile != NULL && nPileBlobSize == nBlobSize) {
			CopyMemory(pInfo->m_pwMapPile, pPileBlob, nBlobSize);
		}

		// MapShadowData BLOB → m_pwMapShadow 復元
		const void* pShadowBlob     = sqlite3_column_blob(pStmt, 11);
		int         nShadowBlobSize = sqlite3_column_bytes(pStmt, 11);
		if (pShadowBlob != NULL && pInfo->m_pwMapShadow != NULL && nShadowBlobSize == nBlobSize) {
			CopyMemory(pInfo->m_pwMapShadow, pShadowBlob, nBlobSize);
		}

		// m_pbyHitTmp を再構築（当たり判定は Init() 後に RenewHitTmp() で再計算する）
		pInfo->RenewHitTmp();

		// ライブラリに追加（m_dwMapID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmt);

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoMap::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'Map' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='Map';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoMap: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoMap::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoMap::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoMap: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoMap: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
