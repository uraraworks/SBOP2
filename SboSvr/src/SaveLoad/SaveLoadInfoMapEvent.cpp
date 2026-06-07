/// @file SaveLoadInfoMapEvent.cpp
/// @brief データ保存・読み込みクラス(マップイベント情報) 実装ファイル
///        各 CInfoMapBase の m_pLibInfoMapEvent を SQLite 正規化テーブル
///        (sys_map_event) に保存・復元する。
///
///  テーブル構成:
///    sys_map_event ─ マップイベント基本情報（1イベント = 1行）
///      (MapID, MapEventID) を複合主キーとする。
///      派生クラス固有フィールドは NULL 可の個別カラムに展開する。
///
///  保存対象フィールド:
///    基底 (CInfoMapEventBase):
///      Type, HitType, HitDirection, PosX1, PosY1, PosX2, PosY2, SoundID
///    MOVE / MAPMOVE 用:
///      DstMapID (MAPMOVE のみ使用), DstX, DstY, Direction
///    GRPIDTMP 用:
///      SetType, IDMain, IDSub
///    LIGHT 用:
///      LightOn, LightTime
///    INITSTATUS 用:
///      EffectID
///
///  マイグレーション:
///    sys_map_event が空（初回移行直後など）かつメモリ上にイベントが存在する場合、
///    MigrateFromMemory() を呼んで一回限りの書き戻しを行う。
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2026/04/25
/// @copyright Copyright(C)URARA-works 2026

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoMapBase.h"
#include "LibInfoMapBase.h"
#include "LibInfoMapEvent.h"
#include "InfoMapEventBase.h"
#include "InfoMapEventMOVE.h"
#include "InfoMapEventMAPMOVE.h"
#include "InfoMapEventGRPIDTMP.h"
#include "InfoMapEventLIGHT.h"
#include "InfoMapEventINITSTATUS.h"
#include "SaveLoadInfoBase.h"
#include "SaveLoadInfoMapEvent.h"

// テーブル名
static const char* s_pszTable = "sys_map_event";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoMapEvent::CSaveLoadInfoMapEvent()
{
}

CSaveLoadInfoMapEvent::~CSaveLoadInfoMapEvent()
{
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
// ============================================================
void CSaveLoadInfoMapEvent::EnsureTable(void)
{
	if (CSaveLoadInfoBase::s_pDb == NULL) return;

	const char* pszSql =
		"CREATE TABLE IF NOT EXISTS sys_map_event("
		"  MapID         INTEGER NOT NULL,"		// 所属マップID
		"  MapEventID    INTEGER NOT NULL,"		// m_dwMapEventID
		"  Type          INTEGER,"				// m_nType (MAPEVENTTYPE_*)
		"  HitType       INTEGER,"				// m_nHitType
		"  HitDirection  INTEGER,"				// m_nHitDirection
		"  PosX1         INTEGER,"				// m_ptPos.x
		"  PosY1         INTEGER,"				// m_ptPos.y
		"  PosX2         INTEGER,"				// m_ptPos2.x
		"  PosY2         INTEGER,"				// m_ptPos2.y
		"  SoundID       INTEGER,"				// m_dwSoundID
		"  DstMapID      INTEGER,"				// MAPMOVE: m_dwMapID（MOVE は 0）
		"  DstX          INTEGER,"				// MOVE/MAPMOVE: m_ptDst.x
		"  DstY          INTEGER,"				// MOVE/MAPMOVE: m_ptDst.y
		"  Direction     INTEGER,"				// MOVE/MAPMOVE: m_nDirection
		"  SetType       INTEGER,"				// GRPIDTMP: m_nSetType
		"  IDMain        INTEGER,"				// GRPIDTMP: m_dwIDMain
		"  IDSub         INTEGER,"				// GRPIDTMP: m_dwIDSub
		"  LightOn       INTEGER,"				// LIGHT: m_bLightOn
		"  LightTime     INTEGER,"				// LIGHT: m_dwTime
		"  EffectID      INTEGER,"				// INITSTATUS: m_dwEffectID
		"  PRIMARY KEY (MapID, MapEventID)"
		");";

	sqlite3_exec(CSaveLoadInfoBase::s_pDb, pszSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全マップイベントを全行置換
//   DELETE + prepared INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoMapEvent::SaveAllEvents(CLibInfoMapBase *pLibInfoMap)
{
	if (CSaveLoadInfoBase::s_pDb == NULL || pLibInfoMap == NULL) return;

	// 既存行を全消去
	sqlite3_exec(CSaveLoadInfoBase::s_pDb, "DELETE FROM sys_map_event;", NULL, NULL, NULL);

	// INSERT 文を準備
	const char* pszInsert =
		"INSERT INTO sys_map_event("
		"  MapID, MapEventID, Type, HitType, HitDirection,"
		"  PosX1, PosY1, PosX2, PosY2, SoundID,"
		"  DstMapID, DstX, DstY, Direction,"
		"  SetType, IDMain, IDSub,"
		"  LightOn, LightTime,"
		"  EffectID"
		") VALUES(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?, ?,?,?, ?,?, ?);";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(CSaveLoadInfoBase::s_pDb, pszInsert, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoMapEvent::SaveAllEvents: prepare failed\n");
		return;
	}

	int nMapCount = pLibInfoMap->GetCount();
	for (int i = 0; i < nMapCount; i++) {
		PCInfoMapBase pInfoMap = (PCInfoMapBase)pLibInfoMap->GetPtr(i);
		if (pInfoMap == NULL) continue;
		if (pInfoMap->m_pLibInfoMapEvent == NULL) continue;

		int nEvtCount = pInfoMap->m_pLibInfoMapEvent->GetCount();
		DWORD dwMapID = pInfoMap->m_dwMapID;

		for (int j = 0; j < nEvtCount; j++) {
			PCInfoMapEventBase pEvt = (PCInfoMapEventBase)pInfoMap->m_pLibInfoMapEvent->GetPtr(j);
			if (pEvt == NULL) continue;

			// ---- 基底フィールド ----
			sqlite3_bind_int(pStmt,  1, (int)dwMapID);
			sqlite3_bind_int(pStmt,  2, (int)pEvt->m_dwMapEventID);
			sqlite3_bind_int(pStmt,  3, (int)pEvt->m_nType);
			sqlite3_bind_int(pStmt,  4, (int)pEvt->m_nHitType);
			sqlite3_bind_int(pStmt,  5, (int)pEvt->m_nHitDirection);
			sqlite3_bind_int(pStmt,  6, (int)pEvt->m_ptPos.x);
			sqlite3_bind_int(pStmt,  7, (int)pEvt->m_ptPos.y);
			sqlite3_bind_int(pStmt,  8, (int)pEvt->m_ptPos2.x);
			sqlite3_bind_int(pStmt,  9, (int)pEvt->m_ptPos2.y);
			sqlite3_bind_int(pStmt, 10, (int)pEvt->m_dwSoundID);

			// ---- 派生クラス固有フィールド（種別ごとに分岐） ----
			switch (pEvt->m_nType) {
			case MAPEVENTTYPE_MOVE:
				{
					PCInfoMapEventMOVE pMove = static_cast<PCInfoMapEventMOVE>(pEvt);
					sqlite3_bind_int(pStmt, 11, 0);						// DstMapID（MOVE は 0）
					sqlite3_bind_int(pStmt, 12, (int)pMove->m_ptDst.x);	// DstX
					sqlite3_bind_int(pStmt, 13, (int)pMove->m_ptDst.y);	// DstY
					sqlite3_bind_int(pStmt, 14, (int)pMove->m_nDirection);	// Direction
				}
				break;
			case MAPEVENTTYPE_MAPMOVE:
				{
					PCInfoMapEventMAPMOVE pMapMove = static_cast<PCInfoMapEventMAPMOVE>(pEvt);
					sqlite3_bind_int(pStmt, 11, (int)pMapMove->m_dwMapID);	// DstMapID
					sqlite3_bind_int(pStmt, 12, (int)pMapMove->m_ptDst.x);	// DstX
					sqlite3_bind_int(pStmt, 13, (int)pMapMove->m_ptDst.y);	// DstY
					sqlite3_bind_int(pStmt, 14, (int)pMapMove->m_nDirection);// Direction
				}
				break;
			default:
				sqlite3_bind_null(pStmt, 11);
				sqlite3_bind_null(pStmt, 12);
				sqlite3_bind_null(pStmt, 13);
				sqlite3_bind_null(pStmt, 14);
				break;
			}

			// GRPIDTMP
			if (pEvt->m_nType == MAPEVENTTYPE_GRPIDTMP) {
				PCInfoMapEventGRPIDTMP pGrp = static_cast<PCInfoMapEventGRPIDTMP>(pEvt);
				sqlite3_bind_int(pStmt, 15, (int)pGrp->m_nSetType);	// SetType
				sqlite3_bind_int(pStmt, 16, (int)pGrp->m_dwIDMain);	// IDMain
				sqlite3_bind_int(pStmt, 17, (int)pGrp->m_dwIDSub);		// IDSub
			} else {
				sqlite3_bind_null(pStmt, 15);
				sqlite3_bind_null(pStmt, 16);
				sqlite3_bind_null(pStmt, 17);
			}

			// LIGHT
			if (pEvt->m_nType == MAPEVENTTYPE_LIGHT) {
				PCInfoMapEventLIGHT pLight = static_cast<PCInfoMapEventLIGHT>(pEvt);
				sqlite3_bind_int(pStmt, 18, (int)pLight->m_bLightOn);	// LightOn
				sqlite3_bind_int(pStmt, 19, (int)pLight->m_dwTime);		// LightTime
			} else {
				sqlite3_bind_null(pStmt, 18);
				sqlite3_bind_null(pStmt, 19);
			}

			// INITSTATUS
			if (pEvt->m_nType == MAPEVENTTYPE_INITSTATUS) {
				PCInfoMapEventINITSTATUS pInit = static_cast<PCInfoMapEventINITSTATUS>(pEvt);
				sqlite3_bind_int(pStmt, 20, (int)pInit->m_dwEffectID);	// EffectID
			} else {
				sqlite3_bind_null(pStmt, 20);
			}

			sqlite3_step(pStmt);
			sqlite3_reset(pStmt);
		}
	}

	sqlite3_finalize(pStmt);
	OutputDebugStringA("SaveLoadInfoMapEvent: sys_map_event に保存完了\n");
}

// ============================================================
// 内部ヘルパー: sys_map_event から全行読み込んで各マップに復元
//   1行以上読めたら TRUE を返す
// ============================================================
BOOL CSaveLoadInfoMapEvent::LoadAllEvents(CLibInfoMapBase *pLibInfoMap)
{
	if (CSaveLoadInfoBase::s_pDb == NULL || pLibInfoMap == NULL) return FALSE;

	const char* pszSelect =
		"SELECT"
		"  MapID, MapEventID, Type, HitType, HitDirection,"
		"  PosX1, PosY1, PosX2, PosY2, SoundID,"
		"  DstMapID, DstX, DstY, Direction,"
		"  SetType, IDMain, IDSub,"
		"  LightOn, LightTime,"
		"  EffectID"
		" FROM sys_map_event"
		" ORDER BY MapID, MapEventID;";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(CSaveLoadInfoBase::s_pDb, pszSelect, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		DWORD dwMapID      = (DWORD)sqlite3_column_int(pStmt, 0);
		DWORD dwMapEventID = (DWORD)sqlite3_column_int(pStmt, 1);
		int   nType        = sqlite3_column_int(pStmt, 2);

		// 対象マップを取得
		PCInfoMapBase pInfoMap = (PCInfoMapBase)pLibInfoMap->GetPtr(dwMapID);
		if (pInfoMap == NULL || pInfoMap->m_pLibInfoMapEvent == NULL) continue;

		// 種別に応じて新規インスタンスを生成
		PCInfoMapEventBase pEvt = (PCInfoMapEventBase)pInfoMap->m_pLibInfoMapEvent->GetNew(nType);
		if (pEvt == NULL) continue;

		// ---- MapEventID を Add() より前にセット ----
		//   Add() 内で m_dwMapEventID == 0 だと GetNewID() で上書きされてしまうため
		pEvt->m_dwMapEventID  = dwMapEventID;

		// ---- 基底フィールドを復元 ----
		pEvt->m_nType         = nType;
		pEvt->m_nHitType      = sqlite3_column_int(pStmt, 3);
		pEvt->m_nHitDirection = sqlite3_column_int(pStmt, 4);
		pEvt->m_ptPos.x       = (LONG)sqlite3_column_int(pStmt, 5);
		pEvt->m_ptPos.y       = (LONG)sqlite3_column_int(pStmt, 6);
		pEvt->m_ptPos2.x      = (LONG)sqlite3_column_int(pStmt, 7);
		pEvt->m_ptPos2.y      = (LONG)sqlite3_column_int(pStmt, 8);
		pEvt->m_dwSoundID     = (DWORD)sqlite3_column_int(pStmt, 9);

		// ---- 派生クラス固有フィールドを復元 ----
		switch (nType) {
		case MAPEVENTTYPE_MOVE:
			{
				PCInfoMapEventMOVE pMove = static_cast<PCInfoMapEventMOVE>(pEvt);
				pMove->m_ptDst.x   = (LONG)sqlite3_column_int(pStmt, 11);	// DstX
				pMove->m_ptDst.y   = (LONG)sqlite3_column_int(pStmt, 12);	// DstY
				pMove->m_nDirection = sqlite3_column_int(pStmt, 13);		// Direction
			}
			break;
		case MAPEVENTTYPE_MAPMOVE:
			{
				PCInfoMapEventMAPMOVE pMapMove = static_cast<PCInfoMapEventMAPMOVE>(pEvt);
				pMapMove->m_dwMapID    = (DWORD)sqlite3_column_int(pStmt, 10);	// DstMapID
				pMapMove->m_ptDst.x    = (LONG)sqlite3_column_int(pStmt, 11);	// DstX
				pMapMove->m_ptDst.y    = (LONG)sqlite3_column_int(pStmt, 12);	// DstY
				pMapMove->m_nDirection = sqlite3_column_int(pStmt, 13);			// Direction
			}
			break;
		default:
			break;
		}

		if (nType == MAPEVENTTYPE_GRPIDTMP) {
			PCInfoMapEventGRPIDTMP pGrp = static_cast<PCInfoMapEventGRPIDTMP>(pEvt);
			pGrp->m_nSetType = sqlite3_column_int(pStmt, 14);			// SetType
			pGrp->m_dwIDMain = (DWORD)sqlite3_column_int(pStmt, 15);	// IDMain
			pGrp->m_dwIDSub  = (DWORD)sqlite3_column_int(pStmt, 16);	// IDSub
		}

		if (nType == MAPEVENTTYPE_LIGHT) {
			PCInfoMapEventLIGHT pLight = static_cast<PCInfoMapEventLIGHT>(pEvt);
			pLight->m_bLightOn = (BOOL)sqlite3_column_int(pStmt, 17);	// LightOn
			pLight->m_dwTime   = (DWORD)sqlite3_column_int(pStmt, 18);	// LightTime
		}

		if (nType == MAPEVENTTYPE_INITSTATUS) {
			PCInfoMapEventINITSTATUS pInit = static_cast<PCInfoMapEventINITSTATUS>(pEvt);
			pInit->m_dwEffectID = (DWORD)sqlite3_column_int(pStmt, 19);	// EffectID
		}

		// ライブラリに追加（m_dwMapEventID が 0 でないので GetNewID() は呼ばれない）
		pInfoMap->m_pLibInfoMapEvent->Add(pEvt);
		nRowCount++;
	}

	sqlite3_finalize(pStmt);

	if (nRowCount > 0) {
		OutputDebugStringA("SaveLoadInfoMapEvent: sys_map_event から読み込み成功\n");
	}
	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: メモリ上のイベントを sys_map_event へ書き戻す
//   旧 BLOB からマップロード済みの場合、events は既にメモリ上にあるため
//   そのまま SaveAllEvents() を呼んで正規化テーブルへ移行する（一回限り）。
// ============================================================
void CSaveLoadInfoMapEvent::MigrateFromMemory(CLibInfoMapBase *pLibInfoMap)
{
	if (pLibInfoMap == NULL) return;

	// イベントが1件でも存在するか確認
	int nTotalEvt = 0;
	int nMapCount = pLibInfoMap->GetCount();
	for (int i = 0; i < nMapCount; i++) {
		PCInfoMapBase pInfoMap = (PCInfoMapBase)pLibInfoMap->GetPtr(i);
		if (pInfoMap == NULL || pInfoMap->m_pLibInfoMapEvent == NULL) continue;
		nTotalEvt += pInfoMap->m_pLibInfoMapEvent->GetCount();
	}

	if (nTotalEvt <= 0) {
		// メモリ上にもイベントなし → マイグレーション不要
		return;
	}

	// メモリ → sys_map_event へ書き戻し
	SaveAllEvents(pLibInfoMap);
	OutputDebugStringA("SaveLoadInfoMapEvent: メモリ → sys_map_event へマイグレーション完了\n");
}

// ============================================================
// Save: 全マップのイベントを sys_map_event に書き込み
//   トランザクションは MgrData 側で BEGIN/COMMIT している
// ============================================================
void CSaveLoadInfoMapEvent::Save(CLibInfoMapBase *pLibInfoMap)
{
	if (pLibInfoMap == NULL) return;

	// テーブルが無ければ作成
	EnsureTable();

	// 全行置換して保存
	SaveAllEvents(pLibInfoMap);
}

// ============================================================
// Load: sys_map_event を読み込んで各マップの m_pLibInfoMapEvent に投入
//   テーブルが空かつメモリ上にイベントが乗っている場合はマイグレーションを行う。
// ============================================================
void CSaveLoadInfoMapEvent::Load(CLibInfoMapBase *pLibInfoMap)
{
	if (pLibInfoMap == NULL) return;

	// テーブルが無ければ作成
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadAllEvents(pLibInfoMap)) {
		return;
	}

	// 2. テーブルが空 → メモリ上のデータからマイグレーション
	OutputDebugStringA("SaveLoadInfoMapEvent: sys_map_event が空 → メモリからマイグレーション\n");
	MigrateFromMemory(pLibInfoMap);
}
