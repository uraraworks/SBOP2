/// @file SaveLoadInfoMapObject.cpp
/// @brief データ保存・読み込みクラス(マップオブジェクト情報) 実装ファイル
///        MapObject ライブラリを SQLite 正規化テーブル (sys_map_object) に移行
///
///  テーブル構成:
///    sys_map_object      : 基本情報 + HitData BLOB (m_pHit: cx*cy バイト)
///    sys_map_object_anime: アニメコマ (ObjectID, Slot, Wait, Level, GrpIDData BLOB)
///
///  折衷 BLOB 方針:
///    m_pHit  (当たり判定) は画像サイズ (cx*cy) に依存する可変長バイト列のため BLOB 格納。
///    pwGrpID (アニメ画像ID) は (cx*cy) 個の WORD 配列のため BLOB 格納。
///    これ以外のスカラ・文字列フィールドはすべて個別カラムに展開する。
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/01
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoMapObject.h"
#include "LibInfoMapObject.h"
#include "SaveLoadInfoMapObject.h"

// テーブル名
static const char* s_pszTableMain  = "sys_map_object";
static const char* s_pszTableAnime = "sys_map_object_anime";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoMapObject::CSaveLoadInfoMapObject()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataMapObject.dat");
	SetName("MapObject");
}

CSaveLoadInfoMapObject::~CSaveLoadInfoMapObject()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   メインテーブル (sys_map_object) と
//   アニメコマ サブテーブル (sys_map_object_anime) を同時に作成する
//
//   折衷 BLOB:
//     HitData   = m_pHit (cx*cy バイト、NULL 可)
//     GrpIDData = pwGrpID (cx*cy 個の WORD バイト列)
// ============================================================
void CSaveLoadInfoMapObject::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// メインテーブル: マップオブジェクト基本情報
	const char* pszMainSql =
		"CREATE TABLE IF NOT EXISTS sys_map_object("
		"  ObjectID  INTEGER PRIMARY KEY,"	// m_dwObjectID
		"  Attr      INTEGER,"				// m_dwAttr (オブジェクトの属性)
		"  HideY     INTEGER,"				// m_nHideY (隠れる上からのマス数)
		"  GrpCX     INTEGER,"				// m_sizeGrp.cx
		"  GrpCY     INTEGER,"				// m_sizeGrp.cy
		"  Hit       INTEGER,"				// m_bHit (当たり判定フラグ)
		"  Name      TEXT,"					// m_strName (SJIS バイト列)
		"  HitData   BLOB"					// m_pHit (cx*cy バイト、折衷 BLOB)
		");";

	sqlite3_exec(s_pDb, pszMainSql, NULL, NULL, NULL);

	// サブテーブル: アニメーションコマ情報 (m_aInfoAnime の可変長配列)
	//   ObjectID + Slot を複合主キーとし、配列インデックスを Slot に格納する
	//   GrpIDData は pwGrpID (cx*cy 個の WORD) を BLOB 格納する（折衷 BLOB）
	const char* pszAnimeSql =
		"CREATE TABLE IF NOT EXISTS sys_map_object_anime("
		"  ObjectID  INTEGER NOT NULL,"
		"  Slot      INTEGER NOT NULL,"
		"  Wait      INTEGER,"				// byWait  (待ち時間 ×１０ミリ秒)
		"  Level     INTEGER,"				// byLevel (透明度)
		"  GrpIDData BLOB,"					// pwGrpID (cx*cy 個の WORD、折衷 BLOB)
		"  PRIMARY KEY (ObjectID, Slot)"
		");";

	sqlite3_exec(s_pDb, pszAnimeSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全マップオブジェクト情報を正規化テーブルに全行置換
//   DELETE + INSERT ループ（外側トランザクションに乗る）
//   アニメサブテーブルも同様に DELETE して再 INSERT する
// ============================================================
void CSaveLoadInfoMapObject::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去（サブテーブルも先に消す）
	sqlite3_exec(s_pDb, "DELETE FROM sys_map_object;",       NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_map_object_anime;", NULL, NULL, NULL);

	// メインテーブル INSERT 文
	const char* pszInsertMain =
		"INSERT INTO sys_map_object("
		"  ObjectID, Attr, HideY, GrpCX, GrpCY, Hit, Name, HitData"
		") VALUES(?, ?, ?, ?, ?, ?, ?, ?);";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsertMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMapObject::SaveToNormalTable: prepare(main) failed\n");
		return;
	}

	// アニメサブテーブル INSERT 文
	const char* pszInsertAnime =
		"INSERT INTO sys_map_object_anime(ObjectID, Slot, Wait, Level, GrpIDData) VALUES(?, ?, ?, ?, ?);";

	sqlite3_stmt* pStmtAnime = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszInsertAnime, -1, &pStmtAnime, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMapObject::SaveToNormalTable: prepare(anime) failed\n");
		sqlite3_finalize(pStmtMain);
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoMapObject pInfo = (PCInfoMapObject)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// ---- メインテーブルへの INSERT ----
		sqlite3_bind_int(pStmtMain, 1, (int)pInfo->m_dwObjectID);
		sqlite3_bind_int(pStmtMain, 2, (int)pInfo->m_dwAttr);
		sqlite3_bind_int(pStmtMain, 3, (int)pInfo->m_nHideY);
		sqlite3_bind_int(pStmtMain, 4, (int)pInfo->m_sizeGrp.cx);
		sqlite3_bind_int(pStmtMain, 5, (int)pInfo->m_sizeGrp.cy);
		sqlite3_bind_int(pStmtMain, 6, (int)pInfo->m_bHit);

		// m_strName: SJIS バイト列をそのまま TEXT としてバインド
		LPCSTR pszName = (LPCSTR)pInfo->m_strName;
		sqlite3_bind_text(pStmtMain, 7, pszName, -1, SQLITE_TRANSIENT);

		// m_pHit: 当たり判定データ (cx*cy バイト) を BLOB としてバインド
		int nHitSize = pInfo->m_sizeGrp.cx * pInfo->m_sizeGrp.cy;
		if (pInfo->m_pHit != NULL && nHitSize > 0) {
			sqlite3_bind_blob(pStmtMain, 8, pInfo->m_pHit, nHitSize, SQLITE_TRANSIENT);
		} else {
			sqlite3_bind_null(pStmtMain, 8);
		}

		sqlite3_step(pStmtMain);
		sqlite3_reset(pStmtMain);

		// ---- サブテーブルへの INSERT: アニメーションコマ ----
		//   Slot = 配列インデックス (0-based)
		//   GrpIDData = pwGrpID (cx*cy 個の WORD を BLOB 格納)
		int nAnimeCount = pInfo->GetAnimeCount();
		int nGrpIDSize  = sizeof(WORD) * pInfo->m_sizeGrp.cx * pInfo->m_sizeGrp.cy;

		for (int j = 0; j < nAnimeCount; j++) {
			PSTMAPOBJECTANIMEINFO pAnime = pInfo->GetAnimePtr(j);
			if (pAnime == NULL) continue;

			sqlite3_bind_int(pStmtAnime, 1, (int)pInfo->m_dwObjectID);
			sqlite3_bind_int(pStmtAnime, 2, j);
			sqlite3_bind_int(pStmtAnime, 3, (int)pAnime->byWait);
			sqlite3_bind_int(pStmtAnime, 4, (int)pAnime->byLevel);

			// pwGrpID: cx*cy 個の WORD 配列を BLOB 格納
			if (pAnime->pwGrpID != NULL && nGrpIDSize > 0) {
				sqlite3_bind_blob(pStmtAnime, 5, pAnime->pwGrpID, nGrpIDSize, SQLITE_TRANSIENT);
			} else {
				sqlite3_bind_null(pStmtAnime, 5);
			}

			sqlite3_step(pStmtAnime);
			sqlite3_reset(pStmtAnime);
		}
	}

	sqlite3_finalize(pStmtAnime);
	sqlite3_finalize(pStmtMain);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
//   メインテーブルを全行 SELECT し、各オブジェクトごとにサブテーブルを
//   prepared statement を reset して再利用しながら取得する
// ============================================================
BOOL CSaveLoadInfoMapObject::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	// メインテーブル SELECT
	const char* pszSelectMain =
		"SELECT ObjectID, Attr, HideY, GrpCX, GrpCY, Hit, Name, HitData"
		" FROM sys_map_object;";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSelectMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	// アニメサブテーブル SELECT
	//   ORDER BY Slot を必ず付けて配列インデックス順を保証する
	const char* pszSelectAnime =
		"SELECT Slot, Wait, Level, GrpIDData"
		" FROM sys_map_object_anime WHERE ObjectID=? ORDER BY Slot;";

	sqlite3_stmt* pStmtAnime = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszSelectAnime, -1, &pStmtAnime, NULL);
	if (nRet != SQLITE_OK) {
		sqlite3_finalize(pStmtMain);
		return FALSE;
	}

	int nRowCount = 0;
	while (sqlite3_step(pStmtMain) == SQLITE_ROW) {
		// 新規 CInfoMapObject を作成
		PCInfoMapObject pInfo = (PCInfoMapObject)pDst->GetNew();
		if (pInfo == NULL) continue;

		// ObjectID を事前にセットしてから Add() を呼ぶ
		//   ← Add() 内で m_dwObjectID == 0 なら GetNewID() で上書きされてしまうため
		DWORD dwObjectID    = (DWORD)sqlite3_column_int(pStmtMain, 0);
		pInfo->m_dwObjectID = dwObjectID;

		pInfo->m_dwAttr  = (DWORD)sqlite3_column_int(pStmtMain, 1);
		pInfo->m_nHideY  = (int)  sqlite3_column_int(pStmtMain, 2);

		// 画像サイズを先に復元する（AddAnime() / m_pHit の確保サイズが依存するため）
		pInfo->m_sizeGrp.cx = (LONG)sqlite3_column_int(pStmtMain, 3);
		pInfo->m_sizeGrp.cy = (LONG)sqlite3_column_int(pStmtMain, 4);

		pInfo->m_bHit = (BOOL)sqlite3_column_int(pStmtMain, 5);

		// Name: SJIS バイト列として格納されているので LegacyAnsiToTString で変換
		const char* pszName = (const char*)sqlite3_column_text(pStmtMain, 6);
		if (pszName != NULL) {
			pInfo->m_strName = (LPCTSTR)LegacyAnsiToTString(pszName);
		}

		// HitData: 当たり判定データ BLOB の復元
		const void* pHitBlob = sqlite3_column_blob(pStmtMain, 7);
		int nHitSize         = sqlite3_column_bytes(pStmtMain, 7);
		if (pHitBlob != NULL && nHitSize > 0) {
			SAFE_DELETE_ARRAY(pInfo->m_pHit);
			pInfo->m_pHit = new BYTE[nHitSize];
			CopyMemory(pInfo->m_pHit, pHitBlob, nHitSize);
		}

		// ---- サブテーブルからアニメーションコマを復元 ----
		//   prepared statement を bind + reset で再利用
		pInfo->DeleteAllAnime();
		sqlite3_bind_int(pStmtAnime, 1, (int)dwObjectID);

		int nGrpIDExpected = sizeof(WORD) * pInfo->m_sizeGrp.cx * pInfo->m_sizeGrp.cy;

		while (sqlite3_step(pStmtAnime) == SQLITE_ROW) {
			// AddAnime() で空コマを追加してからフィールドを上書き
			//   AddAnime() 内で pwGrpID = new WORD[cx*cy] が行われる
			pInfo->AddAnime();
			int nSlot = pInfo->GetAnimeCount() - 1;
			PSTMAPOBJECTANIMEINFO pAnime = pInfo->GetAnimePtr(nSlot);
			if (pAnime == NULL) continue;

			pAnime->byWait  = (BYTE)sqlite3_column_int(pStmtAnime, 1);
			pAnime->byLevel = (BYTE)sqlite3_column_int(pStmtAnime, 2);

			// GrpIDData BLOB → pwGrpID に復元
			const void* pGrpBlob = sqlite3_column_blob(pStmtAnime, 3);
			int nGrpSize         = sqlite3_column_bytes(pStmtAnime, 3);
			if (pGrpBlob != NULL && pAnime->pwGrpID != NULL && nGrpSize == nGrpIDExpected) {
				CopyMemory(pAnime->pwGrpID, pGrpBlob, nGrpSize);
			}
		}
		sqlite3_reset(pStmtAnime);	// 次のオブジェクトのために reset

		// ライブラリに追加（m_dwObjectID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmtAnime);
	sqlite3_finalize(pStmtMain);

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoMapObject::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'MapObject' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='MapObject';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoMapObject: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoMapObject::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoMapObject::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoMapObject: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoMapObject: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
