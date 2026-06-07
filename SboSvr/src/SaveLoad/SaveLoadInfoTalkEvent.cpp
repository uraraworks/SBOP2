/// @file SaveLoadInfoTalkEvent.cpp
/// @brief データ保存・読み込みクラス(会話イベント情報) 実装ファイル
///        TalkEvent ライブラリを SQLite 正規化テーブル (sys_talk_event) に移行
///
///        【テーブル構成】
///          sys_talk_event       : 親レコード (TalkEventID のみ)
///          sys_talk_event_event : イベント行 (TalkEventID, Slot, EventType, Page, Data, Text,
///                                             PageChgCondition, PageJump)
///                                  ※ PAGE 派生フィールド (PageChgCondition, PageJump) は
///                                     非 PAGE イベントでは NULL を格納
///          sys_talk_event_menu  : MENU 選択肢行 (TalkEventID, Slot, ItemNo, ItemPage, ItemName)
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/27
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoTalkEvent.h"
#include "InfoTalkEventPAGE.h"
#include "InfoTalkEventMENU.h"
#include "LibInfoTalkEvent.h"
#include "SaveLoadInfoTalkEvent.h"

// テーブル名
static const char* s_pszTableMain  = "sys_talk_event";
static const char* s_pszTableEvent = "sys_talk_event_event";
static const char* s_pszTableMenu  = "sys_talk_event_menu";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoTalkEvent::CSaveLoadInfoTalkEvent()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataTalkEvent.dat");
	SetName("TalkEvent");
}

CSaveLoadInfoTalkEvent::~CSaveLoadInfoTalkEvent()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   sys_talk_event       : 親テーブル (TalkEventID)
//   sys_talk_event_event : イベント行サブテーブル
//   sys_talk_event_menu  : MENU 選択肢サブテーブル
// ============================================================
void CSaveLoadInfoTalkEvent::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// 親テーブル: 会話イベントIDのみ保持
	const char* pszSqlMain =
		"CREATE TABLE IF NOT EXISTS sys_talk_event("
		"  TalkEventID  INTEGER PRIMARY KEY"	// m_dwTalkEventID
		");";

	// イベント行サブテーブル:
	//   Slot = m_apTalkEvent 内の配列インデックス (0-based)
	//   PageChgCondition / PageJump は TALKEVENTTYPE_PAGE 以外では NULL
	const char* pszSqlEvent =
		"CREATE TABLE IF NOT EXISTS sys_talk_event_event("
		"  TalkEventID        INTEGER NOT NULL,"	// 親 TalkEventID
		"  Slot               INTEGER NOT NULL,"	// 配列インデックス
		"  EventType          INTEGER,"			// m_nEventType
		"  Page               INTEGER,"			// m_nPage (所属ページ番号)
		"  Data               INTEGER,"			// m_dwData (バイナリデータ)
		"  Text               TEXT,"			// m_strText (SJIS バイト列)
		"  PageChgCondition   INTEGER,"			// m_nPageChgCondition (PAGE 派生のみ)
		"  PageJump           INTEGER,"			// m_nPageJump (PAGE 派生のみ)
		"  PRIMARY KEY (TalkEventID, Slot)"
		");";

	// MENU 選択肢サブテーブル:
	//   Slot = 親イベント行の Slot、ItemNo = 選択肢インデックス (0-based)
	const char* pszSqlMenu =
		"CREATE TABLE IF NOT EXISTS sys_talk_event_menu("
		"  TalkEventID  INTEGER NOT NULL,"	// 親 TalkEventID
		"  Slot         INTEGER NOT NULL,"	// イベント行 Slot
		"  ItemNo       INTEGER NOT NULL,"	// 選択肢インデックス (0-based)
		"  ItemPage     INTEGER,"			// STTALKEVENTMENUINFO::nPage (ジャンプ先ページ)
		"  ItemName     TEXT,"			// STTALKEVENTMENUINFO::strName (SJIS)
		"  PRIMARY KEY (TalkEventID, Slot, ItemNo)"
		");";

	sqlite3_exec(s_pDb, pszSqlMain,  NULL, NULL, NULL);
	sqlite3_exec(s_pDb, pszSqlEvent, NULL, NULL, NULL);
	sqlite3_exec(s_pDb, pszSqlMenu,  NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全会話イベント情報を正規化テーブルに全行置換
//   DELETE + INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoTalkEvent::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去（サブテーブルを先に消す）
	sqlite3_exec(s_pDb, "DELETE FROM sys_talk_event_menu;",  NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_talk_event_event;", NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_talk_event;",       NULL, NULL, NULL);

	// 親テーブル INSERT 用 prepared statement
	const char* pszInsertMain =
		"INSERT INTO sys_talk_event(TalkEventID) VALUES(?);";

	// イベント行サブテーブル INSERT 用 prepared statement
	const char* pszInsertEvent =
		"INSERT INTO sys_talk_event_event("
		"  TalkEventID, Slot, EventType, Page, Data, Text, PageChgCondition, PageJump"
		") VALUES(?,?,?,?,?,?,?,?);";

	// MENU 選択肢サブテーブル INSERT 用 prepared statement
	const char* pszInsertMenu =
		"INSERT INTO sys_talk_event_menu("
		"  TalkEventID, Slot, ItemNo, ItemPage, ItemName"
		") VALUES(?,?,?,?,?);";

	sqlite3_stmt* pStmtMain  = NULL;
	sqlite3_stmt* pStmtEvent = NULL;
	sqlite3_stmt* pStmtMenu  = NULL;

	if (sqlite3_prepare_v2(s_pDb, pszInsertMain,  -1, &pStmtMain,  NULL) != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoTalkEvent::SaveToNormalTable: prepare(main) failed\n");
		return;
	}
	if (sqlite3_prepare_v2(s_pDb, pszInsertEvent, -1, &pStmtEvent, NULL) != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoTalkEvent::SaveToNormalTable: prepare(event) failed\n");
		sqlite3_finalize(pStmtMain);
		return;
	}
	if (sqlite3_prepare_v2(s_pDb, pszInsertMenu,  -1, &pStmtMenu,  NULL) != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoTalkEvent::SaveToNormalTable: prepare(menu) failed\n");
		sqlite3_finalize(pStmtMain);
		sqlite3_finalize(pStmtEvent);
		return;
	}

	// 保存対象のインデックス配列を取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoTalkEvent pInfo = (PCInfoTalkEvent)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// ---- 親テーブルへの INSERT ----
		sqlite3_bind_int(pStmtMain, 1, (int)pInfo->m_dwTalkEventID);
		sqlite3_step(pStmtMain);
		sqlite3_reset(pStmtMain);

		// ---- イベント行サブテーブルへの INSERT ----
		int nEventCount = (int)pInfo->m_apTalkEvent.size();
		for (int j = 0; j < nEventCount; j++) {
			PCInfoTalkEventBase pEvt = pInfo->m_apTalkEvent[j];
			if (pEvt == NULL) continue;

			sqlite3_bind_int(pStmtEvent, 1, (int)pInfo->m_dwTalkEventID);
			sqlite3_bind_int(pStmtEvent, 2, j);
			sqlite3_bind_int(pStmtEvent, 3, (int)pEvt->m_nEventType);
			sqlite3_bind_int(pStmtEvent, 4, (int)pEvt->m_nPage);
			sqlite3_bind_int(pStmtEvent, 5, (int)pEvt->m_dwData);

			// m_strText: SJIS バイト列をそのまま TEXT として格納
			LPCSTR pszText = (LPCSTR)pEvt->m_strText;
			sqlite3_bind_text(pStmtEvent, 6, pszText, -1, SQLITE_TRANSIENT);

			// PAGE 派生フィールド（非 PAGE は NULL）
			if (pEvt->m_nEventType == TALKEVENTTYPE_PAGE) {
				CInfoTalkEventPAGE* pPage = static_cast<CInfoTalkEventPAGE*>(pEvt);
				sqlite3_bind_int(pStmtEvent, 7, (int)pPage->m_nPageChgCondition);
				sqlite3_bind_int(pStmtEvent, 8, (int)pPage->m_nPageJump);
			} else {
				sqlite3_bind_null(pStmtEvent, 7);
				sqlite3_bind_null(pStmtEvent, 8);
			}

			sqlite3_step(pStmtEvent);
			sqlite3_reset(pStmtEvent);

			// ---- MENU 選択肢サブテーブルへの INSERT ----
			if (pEvt->m_nEventType == TALKEVENTTYPE_MENU) {
				CInfoTalkEventMENU* pMenu = static_cast<CInfoTalkEventMENU*>(pEvt);
				int nMenuCount = pMenu->GetMenuInfoCount();
				for (int k = 0; k < nMenuCount; k++) {
					PSTTALKEVENTMENUINFO pMenuItem = pMenu->GetPtr(k);
					if (pMenuItem == NULL) continue;

					sqlite3_bind_int(pStmtMenu, 1, (int)pInfo->m_dwTalkEventID);
					sqlite3_bind_int(pStmtMenu, 2, j);
					sqlite3_bind_int(pStmtMenu, 3, k);
					sqlite3_bind_int(pStmtMenu, 4, (int)pMenuItem->nPage);

					// strName: SJIS バイト列をそのまま TEXT として格納
					LPCSTR pszItemName = (LPCSTR)pMenuItem->strName;
					sqlite3_bind_text(pStmtMenu, 5, pszItemName, -1, SQLITE_TRANSIENT);

					sqlite3_step(pStmtMenu);
					sqlite3_reset(pStmtMenu);
				}
			}
		}
	}

	sqlite3_finalize(pStmtMenu);
	sqlite3_finalize(pStmtEvent);
	sqlite3_finalize(pStmtMain);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
//   sys_talk_event を全行 SELECT し、各 TalkEvent ごとに
//   sys_talk_event_event / sys_talk_event_menu を個別 SELECT で取得する
// ============================================================
BOOL CSaveLoadInfoTalkEvent::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	// 親テーブル SELECT
	const char* pszSelectMain =
		"SELECT TalkEventID FROM sys_talk_event;";

	sqlite3_stmt* pStmtMain = NULL;
	if (sqlite3_prepare_v2(s_pDb, pszSelectMain, -1, &pStmtMain, NULL) != SQLITE_OK) return FALSE;

	// イベント行サブテーブル SELECT（TalkEventID で引く、ORDER BY Slot で順序保証）
	const char* pszSelectEvent =
		"SELECT Slot, EventType, Page, Data, Text, PageChgCondition, PageJump"
		" FROM sys_talk_event_event WHERE TalkEventID=? ORDER BY Slot;";

	sqlite3_stmt* pStmtEvent = NULL;
	if (sqlite3_prepare_v2(s_pDb, pszSelectEvent, -1, &pStmtEvent, NULL) != SQLITE_OK) {
		sqlite3_finalize(pStmtMain);
		return FALSE;
	}

	// MENU 選択肢サブテーブル SELECT（TalkEventID + Slot で引く、ORDER BY ItemNo で順序保証）
	const char* pszSelectMenu =
		"SELECT ItemNo, ItemPage, ItemName"
		" FROM sys_talk_event_menu WHERE TalkEventID=? AND Slot=? ORDER BY ItemNo;";

	sqlite3_stmt* pStmtMenu = NULL;
	if (sqlite3_prepare_v2(s_pDb, pszSelectMenu, -1, &pStmtMenu, NULL) != SQLITE_OK) {
		sqlite3_finalize(pStmtEvent);
		sqlite3_finalize(pStmtMain);
		return FALSE;
	}

	int nRowCount = 0;
	while (sqlite3_step(pStmtMain) == SQLITE_ROW) {
		DWORD dwTalkEventID = (DWORD)sqlite3_column_int(pStmtMain, 0);

		// 新規 CInfoTalkEvent を作成
		PCInfoTalkEvent pInfo = (PCInfoTalkEvent)pDst->GetNew();
		if (pInfo == NULL) continue;

		// TalkEventID を事前にセットしてから Add() を呼ぶ
		//   ← Add() 内で m_dwTalkEventID == 0 なら GetNewID() で上書きされてしまうため
		pInfo->m_dwTalkEventID = dwTalkEventID;

		// ---- イベント行を復元 ----
		pInfo->DeleteAllTalkEvent();
		sqlite3_bind_int(pStmtEvent, 1, (int)dwTalkEventID);
		while (sqlite3_step(pStmtEvent) == SQLITE_ROW) {
			int nSlot      = sqlite3_column_int(pStmtEvent, 0);
			int nEventType = sqlite3_column_int(pStmtEvent, 1);
			int nPage      = sqlite3_column_int(pStmtEvent, 2);
			int nData      = sqlite3_column_int(pStmtEvent, 3);

			// Text: SJIS バイト列として格納されているので LegacyAnsiToTString で変換
			const char* pszText = (const char*)sqlite3_column_text(pStmtEvent, 4);

			// EventType に応じた派生クラスを生成
			PCInfoTalkEventBase pEvt = pInfo->GetNew(nEventType);
			if (pEvt == NULL) continue;

			pEvt->m_nEventType = nEventType;
			pEvt->m_nPage      = nPage;
			pEvt->m_dwData     = (DWORD)nData;

			if (pszText != NULL) {
				pEvt->m_strText = (LPCTSTR)LegacyAnsiToTString(pszText);
			}

			// PAGE 派生フィールドの復元
			if (nEventType == TALKEVENTTYPE_PAGE) {
				CInfoTalkEventPAGE* pPage = static_cast<CInfoTalkEventPAGE*>(pEvt);
				// NULL チェック: sqlite3_column_type で判定
				if (sqlite3_column_type(pStmtEvent, 5) != SQLITE_NULL) {
					pPage->m_nPageChgCondition = sqlite3_column_int(pStmtEvent, 5);
				}
				if (sqlite3_column_type(pStmtEvent, 6) != SQLITE_NULL) {
					pPage->m_nPageJump = sqlite3_column_int(pStmtEvent, 6);
				}
			}

			// MENU 選択肢の復元
			if (nEventType == TALKEVENTTYPE_MENU) {
				CInfoTalkEventMENU* pMenu = static_cast<CInfoTalkEventMENU*>(pEvt);
				pMenu->DeleteAllMenuInfo();

				sqlite3_bind_int(pStmtMenu, 1, (int)dwTalkEventID);
				sqlite3_bind_int(pStmtMenu, 2, nSlot);
				while (sqlite3_step(pStmtMenu) == SQLITE_ROW) {
					int nItemPage = sqlite3_column_int(pStmtMenu, 1);
					const char* pszItemName = (const char*)sqlite3_column_text(pStmtMenu, 2);

					// AddMenuInfo は LPCSTR (SJIS) を受け取る
					LPCSTR pszItemNameMB = (pszItemName != NULL) ? pszItemName : "";
					pMenu->AddMenuInfo(nItemPage, pszItemNameMB);
				}
				sqlite3_reset(pStmtMenu);	// 次のイベントのために reset
			}

			pInfo->AddTalkEvent(pEvt);
		}
		sqlite3_reset(pStmtEvent);	// 次の TalkEvent のために reset

		// ライブラリに追加（m_dwTalkEventID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmtMenu);
	sqlite3_finalize(pStmtEvent);
	sqlite3_finalize(pStmtMain);

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoTalkEvent::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'TalkEvent' 行を削除（あれば）
	if (s_pDb != NULL) {
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='TalkEvent';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoTalkEvent: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoTalkEvent::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoTalkEvent::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoTalkEvent: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoTalkEvent: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
