/// @file SaveLoadInfoEffect.cpp
/// @brief データ保存・読み込みクラス(エフェクト情報) 実装ファイル
///        Effect ライブラリを SQLite 正規化テーブル (sys_effect) に移行
///        サブ配列 m_paAnimeInfo → sys_effect_anime (EffectID, Slot, Wait, Level, GrpIDBase, GrpIDPile)
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoEffect.h"
#include "InfoAnime.h"
#include "LibInfoEffect.h"
#include "SaveLoadInfoEffect.h"

// テーブル名
static const char* s_pszTableMain  = "sys_effect";
static const char* s_pszTableAnime = "sys_effect_anime";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoEffect::CSaveLoadInfoEffect()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataEffect.dat");
	SetName("Effect");
}

CSaveLoadInfoEffect::~CSaveLoadInfoEffect()
{
}

void CSaveLoadInfoEffect::SetHeaderInfo(PCInfoBase pInfo)
{
	int i;
	LPCSTR pszName;
	CStringA strHeader;
	CInfoAnime AnimeTmp;

	CSaveLoadInfoBase::SetHeaderInfo(pInfo);

	for (i = 0; ; i++) {
		pszName = AnimeTmp.GetName(i);
		if (pszName == NULL) {
			break;
		}
		strHeader.Format("%s%s", PREFIX_INFOANIME, pszName);
		AddHeaderInfo(strHeader);
	}
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   メインテーブル (sys_effect) と
//   アニメコマ サブテーブル (sys_effect_anime) を同時に作成する
// ============================================================
void CSaveLoadInfoEffect::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// メインテーブル: エフェクト基本情報（アニメコマ以外）
	//   カラム順は InfoEffect.cpp の s_aszName[] に準拠
	//   (byAnimeCount, dwEffectID, dwSoundID, bLoop, m_bLoopSound, m_dwGrpIDMain, m_strName)
	const char* pszMainSql =
		"CREATE TABLE IF NOT EXISTS sys_effect("
		"  EffectID    INTEGER PRIMARY KEY,"	// m_dwEffectID
		"  AnimeCount  INTEGER,"				// m_byAnimeCount
		"  SoundID     INTEGER,"				// m_dwSoundID
		"  Loop        INTEGER,"				// m_bLoop
		"  LoopSound   INTEGER,"				// m_bLoopSound
		"  GrpIDMain   INTEGER,"				// m_dwGrpIDMain
		"  Name        TEXT"					// m_strName (SJIS バイト列)
		");";

	sqlite3_exec(s_pDb, pszMainSql, NULL, NULL, NULL);

	// サブテーブル: アニメーションコマ情報 (m_paAnimeInfo の可変長配列)
	//   EffectID + Slot を複合主キーとし、配列インデックスを Slot に格納する
	const char* pszAnimeSql =
		"CREATE TABLE IF NOT EXISTS sys_effect_anime("
		"  EffectID   INTEGER NOT NULL,"
		"  Slot       INTEGER NOT NULL,"
		"  Wait       INTEGER,"				// m_byWait  (×１０ミリ秒)
		"  Level      INTEGER,"				// m_byLevel (透明度)
		"  GrpIDBase  INTEGER,"				// m_wGrpIDBase
		"  GrpIDPile  INTEGER,"				// m_wGrpIDPile
		"  PRIMARY KEY (EffectID, Slot)"
		");";

	sqlite3_exec(s_pDb, pszAnimeSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全エフェクト情報を正規化テーブルに全行置換
//   DELETE + INSERT ループ（外側トランザクションに乗る）
//   アニメサブテーブルも同様に DELETE して再 INSERT する
// ============================================================
void CSaveLoadInfoEffect::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去（サブテーブルも先に消す）
	sqlite3_exec(s_pDb, "DELETE FROM sys_effect;",       NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_effect_anime;", NULL, NULL, NULL);

	// メインテーブル INSERT 文
	const char* pszInsertMain =
		"INSERT INTO sys_effect("
		"  EffectID, AnimeCount, SoundID, Loop, LoopSound, GrpIDMain, Name"
		") VALUES(?, ?, ?, ?, ?, ?, ?);";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsertMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoEffect::SaveToNormalTable: prepare(main) failed\n");
		return;
	}

	// アニメサブテーブル INSERT 文
	const char* pszInsertAnime =
		"INSERT INTO sys_effect_anime(EffectID, Slot, Wait, Level, GrpIDBase, GrpIDPile) VALUES(?, ?, ?, ?, ?, ?);";

	sqlite3_stmt* pStmtAnime = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszInsertAnime, -1, &pStmtAnime, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoEffect::SaveToNormalTable: prepare(anime) failed\n");
		sqlite3_finalize(pStmtMain);
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoEffect pInfo = (PCInfoEffect)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// ---- メインテーブルへの INSERT ----
		sqlite3_bind_int(pStmtMain, 1, (int)pInfo->m_dwEffectID);
		sqlite3_bind_int(pStmtMain, 2, (int)pInfo->m_byAnimeCount);
		sqlite3_bind_int(pStmtMain, 3, (int)pInfo->m_dwSoundID);
		sqlite3_bind_int(pStmtMain, 4, (int)pInfo->m_bLoop);
		sqlite3_bind_int(pStmtMain, 5, (int)pInfo->m_bLoopSound);
		sqlite3_bind_int(pStmtMain, 6, (int)pInfo->m_dwGrpIDMain);

		// m_strName: SJIS バイト列をそのまま TEXT としてバインド
		LPCSTR pszName = (LPCSTR)pInfo->m_strName;
		sqlite3_bind_text(pStmtMain, 7, pszName, -1, SQLITE_TRANSIENT);

		sqlite3_step(pStmtMain);
		sqlite3_reset(pStmtMain);

		// ---- サブテーブルへの INSERT: アニメーションコマ ----
		//   Slot = 配列インデックス (0-based) をそのまま使う
		int nAnimeCount = pInfo->GetAnimeCount();
		for (int j = 0; j < nAnimeCount; j++) {
			PCInfoAnime pAnime = pInfo->GetAnimePtr(j);
			if (pAnime == NULL) continue;

			sqlite3_bind_int(pStmtAnime, 1, (int)pInfo->m_dwEffectID);
			sqlite3_bind_int(pStmtAnime, 2, j);
			sqlite3_bind_int(pStmtAnime, 3, (int)pAnime->m_byWait);
			sqlite3_bind_int(pStmtAnime, 4, (int)pAnime->m_byLevel);
			sqlite3_bind_int(pStmtAnime, 5, (int)pAnime->m_wGrpIDBase);
			sqlite3_bind_int(pStmtAnime, 6, (int)pAnime->m_wGrpIDPile);

			sqlite3_step(pStmtAnime);
			sqlite3_reset(pStmtAnime);
		}
	}

	sqlite3_finalize(pStmtAnime);
	sqlite3_finalize(pStmtMain);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
//   メインテーブルを全行 SELECT し、各エフェクトごとにサブテーブルを
//   prepared statement を reset して再利用しながら取得する
// ============================================================
BOOL CSaveLoadInfoEffect::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	// メインテーブル SELECT
	const char* pszSelectMain =
		"SELECT EffectID, AnimeCount, SoundID, Loop, LoopSound, GrpIDMain, Name"
		" FROM sys_effect;";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSelectMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	// アニメサブテーブル SELECT
	//   ORDER BY Slot を必ず付けて配列インデックス順を保証する
	const char* pszSelectAnime =
		"SELECT Slot, Wait, Level, GrpIDBase, GrpIDPile"
		" FROM sys_effect_anime WHERE EffectID=? ORDER BY Slot;";

	sqlite3_stmt* pStmtAnime = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszSelectAnime, -1, &pStmtAnime, NULL);
	if (nRet != SQLITE_OK) {
		sqlite3_finalize(pStmtMain);
		return FALSE;
	}

	int nRowCount = 0;
	while (sqlite3_step(pStmtMain) == SQLITE_ROW) {
		// 新規 CInfoEffect を作成
		PCInfoEffect pInfo = (PCInfoEffect)pDst->GetNew();
		if (pInfo == NULL) continue;

		// EffectID を事前にセットしてから Add() を呼ぶ
		//   ← Add() 内で m_dwEffectID == 0 なら GetNewID() で上書きされてしまうため
		DWORD dwEffectID = (DWORD)sqlite3_column_int(pStmtMain, 0);
		pInfo->m_dwEffectID  = dwEffectID;

		// AnimeCount は後でアニメを AddAnime() で追加するため直接セットしない
		// （m_byAnimeCount は AddAnime() 内で自動更新される）
		pInfo->m_dwSoundID   = (DWORD)sqlite3_column_int(pStmtMain, 2);
		pInfo->m_bLoop       = (BOOL) sqlite3_column_int(pStmtMain, 3);
		pInfo->m_bLoopSound  = (BOOL) sqlite3_column_int(pStmtMain, 4);
		pInfo->m_dwGrpIDMain = (DWORD)sqlite3_column_int(pStmtMain, 5);

		// Name: SJIS バイト列として格納されているので LegacyAnsiToTString で変換
		const char* pszName = (const char*)sqlite3_column_text(pStmtMain, 6);
		if (pszName != NULL) {
			pInfo->m_strName = (LPCTSTR)LegacyAnsiToTString(pszName);
		}

		// ---- サブテーブルからアニメーションコマを復元 ----
		//   prepared statement を bind + reset で再利用
		pInfo->DeleteAllAnime();
		sqlite3_bind_int(pStmtAnime, 1, (int)dwEffectID);
		while (sqlite3_step(pStmtAnime) == SQLITE_ROW) {
			// AddAnime() で空コマを追加してからフィールドを上書き
			pInfo->AddAnime();
			int nSlot = pInfo->GetAnimeCount() - 1;
			PCInfoAnime pAnime = pInfo->GetAnimePtr(nSlot);
			if (pAnime == NULL) continue;

			pAnime->m_byWait    = (BYTE) sqlite3_column_int(pStmtAnime, 1);
			pAnime->m_byLevel   = (BYTE) sqlite3_column_int(pStmtAnime, 2);
			pAnime->m_wGrpIDBase = (WORD)sqlite3_column_int(pStmtAnime, 3);
			pAnime->m_wGrpIDPile = (WORD)sqlite3_column_int(pStmtAnime, 4);
		}
		sqlite3_reset(pStmtAnime);	// 次のエフェクトのために reset

		// ライブラリに追加（m_dwEffectID が 0 でないので GetNewID() は呼ばれない）
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
BOOL CSaveLoadInfoEffect::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'Effect' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='Effect';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoEffect: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

BOOL CSaveLoadInfoEffect::HasBrokenAnimeRows(void)
{
	const char* pszSql =
		"SELECT CASE"
		"  WHEN EXISTS (SELECT 1 FROM sys_effect_anime)"
		"   AND NOT EXISTS ("
		"     SELECT 1 FROM sys_effect_anime"
		"      WHERE IFNULL(GrpIDBase, 0) <> 0 OR IFNULL(GrpIDPile, 0) <> 0"
		"   )"
		"  THEN 1"
		"  ELSE 0"
		"END;";
	sqlite3_stmt* pStmt = NULL;
	BOOL bRet = FALSE;

	if (s_pDb == NULL) {
		return FALSE;
	}

	if (sqlite3_prepare_v2(s_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
		return FALSE;
	}

	if (sqlite3_step(pStmt) == SQLITE_ROW) {
		bRet = sqlite3_column_int(pStmt, 0) ? TRUE : FALSE;
	}
	sqlite3_finalize(pStmt);

	return bRet;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoEffect::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoEffect::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 旧 .dat からの移行時にアニメコマの grpID だけ欠落した DB を自動修復する
	if (HasBrokenAnimeRows()) {
		OutputDebugStringA("SaveLoadInfoEffect: 破損した正規化アニメ行を検出 → BLOB/.dat から再マイグレーション\n");
		MigrateFromBlob(pDst);
		return;
	}

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoEffect: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoEffect: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
