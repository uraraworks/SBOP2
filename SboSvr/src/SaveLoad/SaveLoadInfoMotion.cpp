/// @file SaveLoadInfoMotion.cpp
/// @brief データ保存・読み込みクラス(モーション情報) 実装ファイル
///        Motion ライブラリを SQLite 正規化テーブル (sys_motion / sys_motion_draw) に移行
///
/// テーブル構成:
///   sys_motion       ─ モーション基本情報（スカラフィールド + POINT 展開）
///   sys_motion_draw  ─ 描画順リスト m_anDrawList の可変長配列
///                       (MotionID, Slot, DrawNo) の 3 カラム構成
///
/// BLOB 折衷なし: m_anDrawList は単純な int 配列なので完全正規化可能
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/11/23
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoMotion.h"
#include "LibInfoMotion.h"
#include "SaveLoadInfoMotion.h"

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoMotion::CSaveLoadInfoMotion()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataMotion.dat");
	SetName("Motion");
}

CSaveLoadInfoMotion::~CSaveLoadInfoMotion()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   sys_motion:      モーション基本情報
//     POINT は X/Y の 2 カラムに展開（Pile0X/Pile0Y 等）
//   sys_motion_draw: 描画順リスト (m_anDrawList の可変長配列)
//     (MotionID, Slot, DrawNo) の複合主キー
// ============================================================
void CSaveLoadInfoMotion::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// メインテーブル
	const char* pszMainSql =
		"CREATE TABLE IF NOT EXISTS sys_motion("
		"  MotionID      INTEGER PRIMARY KEY,"	// m_dwMotionID
		"  MotionTypeID  INTEGER,"				// m_dwMotionTypeID
		"  MotionListID  INTEGER,"				// m_dwMotionListID
		"  Wait          INTEGER,"				// m_byWait  (×１０ミリ秒)
		"  Level1        INTEGER,"				// m_byLevel1 (透明度1)
		"  Level2        INTEGER,"				// m_byLevel2 (透明度2)
		"  Level3        INTEGER,"				// m_byLevel3 (透明度3)
		"  Pile          INTEGER,"				// m_bPile
		"  RedrawHand    INTEGER,"				// m_bRedrawHand
		"  Loop          INTEGER,"				// m_bLoop
		"  GrpIDMainBase INTEGER,"				// m_wGrpIDMainBase
		"  GrpIDMainPile1 INTEGER,"				// m_wGrpIDMainPile1
		"  GrpIDMainPile2 INTEGER,"				// m_wGrpIDMainPile2
		"  GrpIDMainPile3 INTEGER,"				// m_wGrpIDMainPile3
		"  GrpIDSubBase  INTEGER,"				// m_wGrpIDSubBase
		"  GrpIDSubPile1 INTEGER,"				// m_wGrpIDSubPile1
		"  GrpIDSubPile2 INTEGER,"				// m_wGrpIDSubPile2
		"  GrpIDSubPile3 INTEGER,"				// m_wGrpIDSubPile3
		"  SoundID       INTEGER,"				// m_dwSoundID
		"  ProcID        INTEGER,"				// m_dwProcID
		"  DrawPos0X     INTEGER,"				// m_ptDrawPosPile0.x
		"  DrawPos0Y     INTEGER,"				// m_ptDrawPosPile0.y
		"  DrawPos1X     INTEGER,"				// m_ptDrawPosPile1.x
		"  DrawPos1Y     INTEGER,"				// m_ptDrawPosPile1.y
		"  DrawPos2X     INTEGER,"				// m_ptDrawPosPile2.x
		"  DrawPos2Y     INTEGER,"				// m_ptDrawPosPile2.y
		"  DrawPos3X     INTEGER,"				// m_ptDrawPosPile3.x
		"  DrawPos3Y     INTEGER"				// m_ptDrawPosPile3.y
		");";

	sqlite3_exec(s_pDb, pszMainSql, NULL, NULL, NULL);

	// サブテーブル: 描画順リスト (m_anDrawList の可変長配列)
	//   MotionID + Slot を複合主キーとし、配列インデックスをそのまま Slot に格納
	const char* pszSubSql =
		"CREATE TABLE IF NOT EXISTS sys_motion_draw("
		"  MotionID INTEGER NOT NULL,"
		"  Slot     INTEGER NOT NULL,"
		"  DrawNo   INTEGER,"
		"  PRIMARY KEY (MotionID, Slot)"
		");";

	sqlite3_exec(s_pDb, pszSubSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全モーションを正規化テーブルに全置換
//   DELETE + INSERT ループ（外側トランザクションに乗る）
//   サブテーブル (sys_motion_draw) も同様に DELETE して再 INSERT する
// ============================================================
void CSaveLoadInfoMotion::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去（サブテーブルも先に消す）
	sqlite3_exec(s_pDb, "DELETE FROM sys_motion;",      NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_motion_draw;", NULL, NULL, NULL);

	// メインテーブル INSERT 文
	const char* pszInsertMain =
		"INSERT INTO sys_motion("
		"  MotionID, MotionTypeID, MotionListID,"
		"  Wait, Level1, Level2, Level3,"
		"  Pile, RedrawHand, Loop,"
		"  GrpIDMainBase, GrpIDMainPile1, GrpIDMainPile2, GrpIDMainPile3,"
		"  GrpIDSubBase, GrpIDSubPile1, GrpIDSubPile2, GrpIDSubPile3,"
		"  SoundID, ProcID,"
		"  DrawPos0X, DrawPos0Y, DrawPos1X, DrawPos1Y,"
		"  DrawPos2X, DrawPos2Y, DrawPos3X, DrawPos3Y"
		") VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsertMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMotion::SaveToNormalTable: prepare(main) failed\n");
		return;
	}

	// サブテーブル INSERT 文（モーションごとに複数回使い回す）
	const char* pszInsertSub =
		"INSERT INTO sys_motion_draw(MotionID, Slot, DrawNo) VALUES(?,?,?);";

	sqlite3_stmt* pStmtSub = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszInsertSub, -1, &pStmtSub, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMotion::SaveToNormalTable: prepare(sub) failed\n");
		sqlite3_finalize(pStmtMain);
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoMotion pInfo = (PCInfoMotion)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// ---- メインテーブルへの INSERT ----
		sqlite3_bind_int(pStmtMain,  1, (int)pInfo->m_dwMotionID);
		sqlite3_bind_int(pStmtMain,  2, (int)pInfo->m_dwMotionTypeID);
		sqlite3_bind_int(pStmtMain,  3, (int)pInfo->m_dwMotionListID);
		sqlite3_bind_int(pStmtMain,  4, (int)pInfo->m_byWait);
		sqlite3_bind_int(pStmtMain,  5, (int)pInfo->m_byLevel1);
		sqlite3_bind_int(pStmtMain,  6, (int)pInfo->m_byLevel2);
		sqlite3_bind_int(pStmtMain,  7, (int)pInfo->m_byLevel3);
		sqlite3_bind_int(pStmtMain,  8, (int)pInfo->m_bPile);
		sqlite3_bind_int(pStmtMain,  9, (int)pInfo->m_bRedrawHand);
		sqlite3_bind_int(pStmtMain, 10, (int)pInfo->m_bLoop);
		sqlite3_bind_int(pStmtMain, 11, (int)pInfo->m_wGrpIDMainBase);
		sqlite3_bind_int(pStmtMain, 12, (int)pInfo->m_wGrpIDMainPile1);
		sqlite3_bind_int(pStmtMain, 13, (int)pInfo->m_wGrpIDMainPile2);
		sqlite3_bind_int(pStmtMain, 14, (int)pInfo->m_wGrpIDMainPile3);
		sqlite3_bind_int(pStmtMain, 15, (int)pInfo->m_wGrpIDSubBase);
		sqlite3_bind_int(pStmtMain, 16, (int)pInfo->m_wGrpIDSubPile1);
		sqlite3_bind_int(pStmtMain, 17, (int)pInfo->m_wGrpIDSubPile2);
		sqlite3_bind_int(pStmtMain, 18, (int)pInfo->m_wGrpIDSubPile3);
		sqlite3_bind_int(pStmtMain, 19, (int)pInfo->m_dwSoundID);
		sqlite3_bind_int(pStmtMain, 20, (int)pInfo->m_dwProcID);
		sqlite3_bind_int(pStmtMain, 21, (int)pInfo->m_ptDrawPosPile0.x);
		sqlite3_bind_int(pStmtMain, 22, (int)pInfo->m_ptDrawPosPile0.y);
		sqlite3_bind_int(pStmtMain, 23, (int)pInfo->m_ptDrawPosPile1.x);
		sqlite3_bind_int(pStmtMain, 24, (int)pInfo->m_ptDrawPosPile1.y);
		sqlite3_bind_int(pStmtMain, 25, (int)pInfo->m_ptDrawPosPile2.x);
		sqlite3_bind_int(pStmtMain, 26, (int)pInfo->m_ptDrawPosPile2.y);
		sqlite3_bind_int(pStmtMain, 27, (int)pInfo->m_ptDrawPosPile3.x);
		sqlite3_bind_int(pStmtMain, 28, (int)pInfo->m_ptDrawPosPile3.y);

		sqlite3_step(pStmtMain);
		sqlite3_reset(pStmtMain);

		// ---- サブテーブルへの INSERT (m_anDrawList ループ) ----
		//   Slot = 配列インデックス (0-based) をそのまま使う
		int nDrawCount = (int)pInfo->m_anDrawList.size();
		for (int j = 0; j < nDrawCount; j++) {
			sqlite3_bind_int(pStmtSub, 1, (int)pInfo->m_dwMotionID);
			sqlite3_bind_int(pStmtSub, 2, j);
			sqlite3_bind_int(pStmtSub, 3, (int)pInfo->m_anDrawList[j]);

			sqlite3_step(pStmtSub);
			sqlite3_reset(pStmtSub);	// reset で再利用（finalize より高効率）
		}
	}

	sqlite3_finalize(pStmtSub);
	sqlite3_finalize(pStmtMain);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
//   メインテーブルを全行 SELECT し、各モーションごとにサブテーブルを
//   prepared statement を reset して再利用しながら取得する
// ============================================================
BOOL CSaveLoadInfoMotion::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	// メインテーブル SELECT
	const char* pszSelectMain =
		"SELECT"
		"  MotionID, MotionTypeID, MotionListID,"
		"  Wait, Level1, Level2, Level3,"
		"  Pile, RedrawHand, Loop,"
		"  GrpIDMainBase, GrpIDMainPile1, GrpIDMainPile2, GrpIDMainPile3,"
		"  GrpIDSubBase, GrpIDSubPile1, GrpIDSubPile2, GrpIDSubPile3,"
		"  SoundID, ProcID,"
		"  DrawPos0X, DrawPos0Y, DrawPos1X, DrawPos1Y,"
		"  DrawPos2X, DrawPos2Y, DrawPos3X, DrawPos3Y"
		" FROM sys_motion;";

	sqlite3_stmt* pStmtMain = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszSelectMain, -1, &pStmtMain, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	// サブテーブル SELECT（外側で一度 prepare し、各モーションごとに bind+reset で使い回す）
	//   ORDER BY Slot を必ず付けて配列インデックス順を保証する
	const char* pszSelectSub =
		"SELECT Slot, DrawNo FROM sys_motion_draw WHERE MotionID=? ORDER BY Slot;";

	sqlite3_stmt* pStmtSub = NULL;
	nRet = sqlite3_prepare_v2(s_pDb, pszSelectSub, -1, &pStmtSub, NULL);
	if (nRet != SQLITE_OK) {
		sqlite3_finalize(pStmtMain);
		return FALSE;
	}

	int nRowCount = 0;
	while (sqlite3_step(pStmtMain) == SQLITE_ROW) {
		// 新規 CInfoMotion を作成
		PCInfoMotion pInfo = (PCInfoMotion)pDst->GetNew();
		if (pInfo == NULL) continue;

		// m_dwMotionID を先にセットしてから Add() を呼ぶ
		//   ← Add() 内で 0 なら GetNewID() が呼ばれてしまうため
		DWORD dwMotionID = (DWORD)sqlite3_column_int(pStmtMain, 0);
		pInfo->m_dwMotionID     = dwMotionID;
		pInfo->m_dwMotionTypeID = (DWORD)sqlite3_column_int(pStmtMain, 1);
		pInfo->m_dwMotionListID = (DWORD)sqlite3_column_int(pStmtMain, 2);
		pInfo->m_byWait         = (BYTE) sqlite3_column_int(pStmtMain, 3);
		pInfo->m_byLevel1       = (BYTE) sqlite3_column_int(pStmtMain, 4);
		pInfo->m_byLevel2       = (BYTE) sqlite3_column_int(pStmtMain, 5);
		pInfo->m_byLevel3       = (BYTE) sqlite3_column_int(pStmtMain, 6);
		pInfo->m_bPile          = (BOOL) sqlite3_column_int(pStmtMain, 7);
		pInfo->m_bRedrawHand    = (BOOL) sqlite3_column_int(pStmtMain, 8);
		pInfo->m_bLoop          = (BOOL) sqlite3_column_int(pStmtMain, 9);
		pInfo->m_wGrpIDMainBase  = (WORD)sqlite3_column_int(pStmtMain, 10);
		pInfo->m_wGrpIDMainPile1 = (WORD)sqlite3_column_int(pStmtMain, 11);
		pInfo->m_wGrpIDMainPile2 = (WORD)sqlite3_column_int(pStmtMain, 12);
		pInfo->m_wGrpIDMainPile3 = (WORD)sqlite3_column_int(pStmtMain, 13);
		pInfo->m_wGrpIDSubBase   = (WORD)sqlite3_column_int(pStmtMain, 14);
		pInfo->m_wGrpIDSubPile1  = (WORD)sqlite3_column_int(pStmtMain, 15);
		pInfo->m_wGrpIDSubPile2  = (WORD)sqlite3_column_int(pStmtMain, 16);
		pInfo->m_wGrpIDSubPile3  = (WORD)sqlite3_column_int(pStmtMain, 17);
		pInfo->m_dwSoundID       = (DWORD)sqlite3_column_int(pStmtMain, 18);
		pInfo->m_dwProcID        = (DWORD)sqlite3_column_int(pStmtMain, 19);
		pInfo->m_ptDrawPosPile0.x = (LONG)sqlite3_column_int(pStmtMain, 20);
		pInfo->m_ptDrawPosPile0.y = (LONG)sqlite3_column_int(pStmtMain, 21);
		pInfo->m_ptDrawPosPile1.x = (LONG)sqlite3_column_int(pStmtMain, 22);
		pInfo->m_ptDrawPosPile1.y = (LONG)sqlite3_column_int(pStmtMain, 23);
		pInfo->m_ptDrawPosPile2.x = (LONG)sqlite3_column_int(pStmtMain, 24);
		pInfo->m_ptDrawPosPile2.y = (LONG)sqlite3_column_int(pStmtMain, 25);
		pInfo->m_ptDrawPosPile3.x = (LONG)sqlite3_column_int(pStmtMain, 26);
		pInfo->m_ptDrawPosPile3.y = (LONG)sqlite3_column_int(pStmtMain, 27);

		// ---- サブテーブルから描画順リストを復元 ----
		//   prepared statement を bind + reset で再利用
		pInfo->m_anDrawList.clear();
		sqlite3_bind_int(pStmtSub, 1, (int)dwMotionID);
		while (sqlite3_step(pStmtSub) == SQLITE_ROW) {
			// Slot は連番のはずだが ORDER BY Slot で取得するので push_back で順序保証
			int nDrawNo = sqlite3_column_int(pStmtSub, 1);
			pInfo->m_anDrawList.push_back(nDrawNo);
		}
		sqlite3_reset(pStmtSub);	// 次のモーションのために reset

		// ライブラリに追加（m_dwMotionID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmtSub);
	sqlite3_finalize(pStmtMain);

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoMotion::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'Motion' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='Motion';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoMotion: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoMotion::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoMotion::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoMotion: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoMotion: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
