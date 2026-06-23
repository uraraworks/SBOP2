/// @file SaveLoadInfoChar.cpp
/// @brief データ保存・読み込みクラス(キャラ情報) 実装ファイル
///        Char ライブラリを SQLite 完全正規化テーブルに移行
///
///  テーブル構成:
///    sys_char — キャラ情報メインテーブル
///      主キー   : CharID (INTEGER)
///      個別カラム(基本情報):
///                  MapID / MapX / MapY / Direction / MoveState / MoveType
///                  HP / MaxHP / SP / MaxSP / Exp / Level
///                  EquipItemIDCloth / EquipItemIDAcce1 / EquipItemIDAcce2
///                  EquipItemIDArmsRight / EquipItemIDArmsLeft / EquipItemIDHead
///                  Block / Push / IsNPC / Sex / FamilyID / GrpSize / CharName
///      個別カラム(外見 GrpID 群 ×15):
///                  GrpIDNPC / GrpIDCloth / GrpIDEye / GrpIDEyeColor
///                  GrpIDHairType / GrpIDHairColor / GrpIDSP
///                  GrpIDTmpMain / GrpIDTmpSub / GrpIDAcce
///                  GrpIDArmsMain / GrpIDArmsSub / GrpIDArmsLeftMain / GrpIDArmsLeftSub
///                  GrpIDInitNPC
///      個別カラム(ステータス能力値 ×20 + 初期 GrpID ×6 = ×26):
///                  Stamina / Power / Strength / Magic / Skillful
///                  AbillityAT / AbillityDF
///                  PAtack / PDefense / PMagic / PMagicDefense
///                  PHitAverage / PAvoidAverage / PCriticalAverage
///                  AttrFire / AttrWind / AttrWater / AttrEarth / AttrLight / AttrDark
///                  GrpIDInitCloth / GrpIDInitEye / GrpIDInitEyeColor
///                  GrpIDInitHairType / GrpIDInitHairColor / GrpIDInitSP
///      個別カラム(移動/検索スカラ ×9):
///                  MaxItemCount / DropItemAverage / MoveAverage / MoveAverageBattle
///                  MoveWait / MoveWaitBattle / SearchDistanceCX / SearchDistanceCY
///                  MotionTypeID
///      個別カラム(NPC発生情報 ×6):
///                  PutCycle / PutMoveType / MaxPutCount / PutAverage
///                  PutAreaX / PutAreaY
///
///    sys_char_item — 所持アイテムID サブテーブル
///      (CharID INTEGER, Slot INTEGER, ItemID INTEGER, PRIMARY KEY(CharID,Slot))
///
///    sys_char_skill — 所持スキルID サブテーブル
///      (CharID INTEGER, Slot INTEGER, SkillID INTEGER, PRIMARY KEY(CharID,Slot))
///
///  マイグレーション:
///    EnsureTable() 冒頭で PRAGMA table_info(sys_char) を確認し、
///    旧 BLOB カラム (GrpIDData) が存在する場合は旧スキーマで全行を読み込み
///    メモリに復元 → DROP TABLE sys_char → 新 CREATE TABLE → 再書き込みする。
///
///  派生クラス対応:
///    m_nMoveType カラムで型を識別し、読み込み時に GetNew(nMoveType) で
///    正しい派生クラスのインスタンスを生成する。
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include <map>
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoCharBase.h"
#include "InfoCharSvr.h"
#include "LibInfoCharBase.h"
#include "SaveLoadInfoChar.h"

// テーブル名
static const char* s_pszTableChar      = "sys_char";
static const char* s_pszTableCharItem  = "sys_char_item";
static const char* s_pszTableCharSkill = "sys_char_skill";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoChar::CSaveLoadInfoChar()
{
	// フォールバック .dat パスと論理名を設定（基底クラスの BLOB 経路マイグレーション用）
	SetFileName("SBODATA\\SboDataChar.dat");
	// SQLite PRIMARY KEY 用論理名
	SetName("Char");
}

CSaveLoadInfoChar::~CSaveLoadInfoChar()
{
}

// ============================================================
// 内部ヘルパー: 旧 BLOB スキーマを全行読み込んでメモリに復元する
//   LoadFromNormalTable() より先に呼び、旧スキーマ専用で実行する
// ============================================================
static BOOL s_LoadLegacyBlobRows(sqlite3* pDb, CLibInfoCharBase* pLibChar)
{
	// 旧スキーマ: GrpIDData BLOB カラムで読み込む
	const char* pszSelect =
		"SELECT"
		"  CharID, MapID, MapX, MapY, Direction, MoveState, MoveType,"
		"  HP, MaxHP, SP, MaxSP, Exp, Level,"
		"  EquipItemIDCloth, EquipItemIDAcce1, EquipItemIDAcce2,"
		"  EquipItemIDArmsRight, EquipItemIDArmsLeft, EquipItemIDHead,"
		"  Block, Push, IsNPC, Sex, FamilyID, GrpSize, CharName,"
		"  GrpIDData, StatusData, MoveData, PutNpcData, ItemData, SkillData"
		" FROM sys_char;";

	sqlite3_stmt* pStmt = NULL;
	if (sqlite3_prepare_v2(pDb, pszSelect, -1, &pStmt, NULL) != SQLITE_OK) return FALSE;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		int nMoveType = sqlite3_column_int(pStmt, 6);
		PCInfoCharBase pInfo = (PCInfoCharBase)pLibChar->GetNew(nMoveType);
		if (pInfo == NULL) continue;

		// ---- 個別カラム ----
		pInfo->m_dwCharID               = (DWORD)sqlite3_column_int(pStmt,  0);
		pInfo->m_dwMapID                = (DWORD)sqlite3_column_int(pStmt,  1);
		pInfo->m_nMapX                  = sqlite3_column_int(pStmt,  2);
		pInfo->m_nMapY                  = sqlite3_column_int(pStmt,  3);
		pInfo->m_nDirection             = sqlite3_column_int(pStmt,  4);
		pInfo->m_nMoveState             = sqlite3_column_int(pStmt,  5);
		pInfo->m_nMoveType              = sqlite3_column_int(pStmt,  6);
		pInfo->m_dwHP                   = (DWORD)sqlite3_column_int(pStmt,  7);
		pInfo->m_dwMaxHP                = (DWORD)sqlite3_column_int(pStmt,  8);
		pInfo->m_dwSP                   = (DWORD)sqlite3_column_int(pStmt,  9);
		pInfo->m_dwMaxSP                = (DWORD)sqlite3_column_int(pStmt, 10);
		pInfo->m_dwExp                  = (DWORD)sqlite3_column_int(pStmt, 11);
		pInfo->m_wLevel                 = (WORD)sqlite3_column_int(pStmt,  12);
		pInfo->m_dwEquipItemIDCloth     = (DWORD)sqlite3_column_int(pStmt, 13);
		pInfo->m_dwEquipItemIDAcce1     = (DWORD)sqlite3_column_int(pStmt, 14);
		pInfo->m_dwEquipItemIDAcce2     = (DWORD)sqlite3_column_int(pStmt, 15);
		pInfo->m_dwEquipItemIDArmsRight = (DWORD)sqlite3_column_int(pStmt, 16);
		pInfo->m_dwEquipItemIDArmsLeft  = (DWORD)sqlite3_column_int(pStmt, 17);
		pInfo->m_dwEquipItemIDHead      = (DWORD)sqlite3_column_int(pStmt, 18);
		pInfo->m_bBlock                 = (BOOL)sqlite3_column_int(pStmt,  19);
		pInfo->m_bPush                  = (BOOL)sqlite3_column_int(pStmt,  20);
		pInfo->m_bNPC                   = (BOOL)sqlite3_column_int(pStmt,  21);
		pInfo->m_nSex                   = sqlite3_column_int(pStmt,  22);
		pInfo->m_wFamilyID              = (WORD)sqlite3_column_int(pStmt,  23);
		pInfo->m_nGrpSize               = sqlite3_column_int(pStmt,  24);

		const char* pszName = (const char*)sqlite3_column_text(pStmt, 25);
		if (pszName != NULL) pInfo->m_strCharName = (LPCTSTR)Utf8ToTString(pszName);

		// ---- BLOB: 外見 GrpID 群 (WORD×15) ----
		{
			const void* pBlob = sqlite3_column_blob(pStmt, 26);
			int nBytes = sqlite3_column_bytes(pStmt, 26);
			if (pBlob != NULL && nBytes == 15 * (int)sizeof(WORD)) {
				const WORD* pw = (const WORD*)pBlob;
				pInfo->m_wGrpIDNPC          = pw[0];
				pInfo->m_wGrpIDCloth        = pw[1];
				pInfo->m_wGrpIDEye          = pw[2];
				pInfo->m_wGrpIDEyeColor     = pw[3];
				pInfo->m_wGrpIDHairType     = pw[4];
				pInfo->m_wGrpIDHairColor    = pw[5];
				pInfo->m_wGrpIDSP           = pw[6];
				pInfo->m_wGrpIDTmpMain      = pw[7];
				pInfo->m_wGrpIDTmpSub       = pw[8];
				pInfo->m_wGrpIDAcce         = pw[9];
				pInfo->m_wGrpIDArmsMain     = pw[10];
				pInfo->m_wGrpIDArmsSub      = pw[11];
				pInfo->m_wGrpIDArmsLeftMain = pw[12];
				pInfo->m_wGrpIDArmsLeftSub  = pw[13];
				pInfo->m_wGrpIDInitNPC      = pw[14];
			}
		}

		// ---- BLOB: ステータス能力値 (WORD×26) ----
		{
			const void* pBlob = sqlite3_column_blob(pStmt, 27);
			int nBytes = sqlite3_column_bytes(pStmt, 27);
			if (pBlob != NULL && nBytes == 26 * (int)sizeof(WORD)) {
				const WORD* pw = (const WORD*)pBlob;
				pInfo->m_wStamina           = pw[0];
				pInfo->m_wPower             = pw[1];
				pInfo->m_wStrength          = pw[2];
				pInfo->m_wMagic             = pw[3];
				pInfo->m_wSkillful          = pw[4];
				pInfo->m_wAbillityAT        = pw[5];
				pInfo->m_wAbillityDF        = pw[6];
				pInfo->m_wPAtack            = pw[7];
				pInfo->m_wPDefense          = pw[8];
				pInfo->m_wPMagic            = pw[9];
				pInfo->m_wPMagicDefense     = pw[10];
				pInfo->m_wPHitAverage       = pw[11];
				pInfo->m_wPAvoidAverage     = pw[12];
				pInfo->m_wPCriticalAverage  = pw[13];
				pInfo->m_wAttrFire          = pw[14];
				pInfo->m_wAttrWind          = pw[15];
				pInfo->m_wAttrWater         = pw[16];
				pInfo->m_wAttrEarth         = pw[17];
				pInfo->m_wAttrLight         = pw[18];
				pInfo->m_wAttrDark          = pw[19];
				pInfo->m_wGrpIDInitCloth    = pw[20];
				pInfo->m_wGrpIDInitEye      = pw[21];
				pInfo->m_wGrpIDInitEyeColor = pw[22];
				pInfo->m_wGrpIDInitHairType = pw[23];
				pInfo->m_wGrpIDInitHairColor= pw[24];
				pInfo->m_wGrpIDInitSP       = pw[25];
			}
		}

		// ---- BLOB: 移動/検索関連スカラ ----
		{
			const void* pBlob = sqlite3_column_blob(pStmt, 28);
			int nBytes = sqlite3_column_bytes(pStmt, 28);
			struct {
				int    nMaxItemCount;
				int    nDropItemAverage;
				int    nMoveAverage;
				int    nMoveAverageBattle;
				DWORD  dwMoveWait;
				DWORD  dwMoveWaitBattle;
				int    nSearchDistanceCX;
				int    nSearchDistanceCY;
				DWORD  dwMotionTypeID;
			} moveData;
			if (pBlob != NULL && nBytes == (int)sizeof(moveData)) {
				memcpy(&moveData, pBlob, sizeof(moveData));
				pInfo->m_nMaxItemCount         = moveData.nMaxItemCount;
				pInfo->m_nDropItemAverage      = moveData.nDropItemAverage;
				pInfo->m_nMoveAverage          = moveData.nMoveAverage;
				pInfo->m_nMoveAverageBattle    = moveData.nMoveAverageBattle;
				pInfo->m_dwMoveWait            = moveData.dwMoveWait;
				pInfo->m_dwMoveWaitBattle      = moveData.dwMoveWaitBattle;
				pInfo->m_sizeSearchDistance.cx = (LONG)moveData.nSearchDistanceCX;
				pInfo->m_sizeSearchDistance.cy = (LONG)moveData.nSearchDistanceCY;
				pInfo->m_dwMotionTypeID        = moveData.dwMotionTypeID;
			}
		}

		// ---- BLOB: NPC発生情報 ----
		{
			const void* pBlob = sqlite3_column_blob(pStmt, 29);
			int nBytes = sqlite3_column_bytes(pStmt, 29);
			struct {
				DWORD  dwPutCycle;
				int    nPutMoveType;
				int    nMaxPutCount;
				int    nPutAverage;
				int    nPutAreaX;
				int    nPutAreaY;
			} putNpcData;
			if (pBlob != NULL && nBytes == (int)sizeof(putNpcData)) {
				memcpy(&putNpcData, pBlob, sizeof(putNpcData));
				pInfo->m_dwPutCycle   = putNpcData.dwPutCycle;
				pInfo->m_nPutMoveType = putNpcData.nPutMoveType;
				pInfo->m_nMaxPutCount = putNpcData.nMaxPutCount;
				pInfo->m_nPutAverage  = putNpcData.nPutAverage;
				pInfo->m_ptPutArea.x  = putNpcData.nPutAreaX;
				pInfo->m_ptPutArea.y  = putNpcData.nPutAreaY;
			}
		}

		// ---- BLOB: 所持アイテムID配列 (DWORD 可変長) ----
		{
			const void* pBlob = sqlite3_column_blob(pStmt, 30);
			int nBytes = sqlite3_column_bytes(pStmt, 30);
			pInfo->m_adwItemID.clear();
			if (pBlob != NULL && nBytes > 0) {
				int nItemCount = nBytes / (int)sizeof(DWORD);
				const DWORD* pdw = (const DWORD*)pBlob;
				for (int j = 0; j < nItemCount; j++) pInfo->m_adwItemID.push_back(pdw[j]);
			}
		}

		// ---- BLOB: 所持スキルID配列 (DWORD 可変長) ----
		{
			const void* pBlob = sqlite3_column_blob(pStmt, 31);
			int nBytes = sqlite3_column_bytes(pStmt, 31);
			pInfo->m_adwSkillID.clear();
			if (pBlob != NULL && nBytes > 0) {
				int nSkillCount = nBytes / (int)sizeof(DWORD);
				const DWORD* pdw = (const DWORD*)pBlob;
				for (int j = 0; j < nSkillCount; j++) pInfo->m_adwSkillID.push_back(pdw[j]);
			}
		}

		pLibChar->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmt);
	pLibChar->RenewIDPtr();
	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   旧 BLOB スキーマ検出時は自動マイグレーション実行
// ============================================================
void CSaveLoadInfoChar::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// ---- 旧 BLOB スキーマ検出 ----
	// PRAGMA table_info で sys_char に GrpIDData カラムが存在するか調べる
	{
		bool bHasLegacyBlob = false;
		sqlite3_stmt* pStmt = NULL;
		if (sqlite3_prepare_v2(s_pDb, "PRAGMA table_info(sys_char);", -1, &pStmt, NULL) == SQLITE_OK) {
			while (sqlite3_step(pStmt) == SQLITE_ROW) {
				const char* pszColName = (const char*)sqlite3_column_text(pStmt, 1);
				if (pszColName != NULL && strcmp(pszColName, "GrpIDData") == 0) {
					bHasLegacyBlob = true;
					break;
				}
			}
			sqlite3_finalize(pStmt);
		}

		if (bHasLegacyBlob) {
			OutputDebugStringA("SaveLoadInfoChar: 旧 BLOB スキーマ検出 → マイグレーション開始\n");

			// 旧スキーマで全行をメモリに復元する一時ライブラリを使う
			// m_pLibInfoBase が設定済みであれば直接利用できる
			if (m_pLibInfoBase != NULL) {
				CLibInfoCharBase* pLibChar = (CLibInfoCharBase*)m_pLibInfoBase;
				s_LoadLegacyBlobRows(s_pDb, pLibChar);
			}

			// 旧テーブルを削除
			sqlite3_exec(s_pDb, "DROP TABLE IF EXISTS sys_char;", NULL, NULL, NULL);
			OutputDebugStringA("SaveLoadInfoChar: 旧 sys_char を DROP\n");
		}
	}

	// ---- メインテーブル CREATE ----
	const char* pszSql =
		"CREATE TABLE IF NOT EXISTS sys_char("
		"  CharID                 INTEGER PRIMARY KEY,"	// m_dwCharID
		"  MapID                  INTEGER,"				// m_dwMapID
		"  MapX                   INTEGER,"				// m_nMapX
		"  MapY                   INTEGER,"				// m_nMapY
		"  Direction              INTEGER,"				// m_nDirection
		"  MoveState              INTEGER,"				// m_nMoveState
		"  MoveType               INTEGER,"				// m_nMoveType (派生クラス識別)
		"  HP                     INTEGER,"				// m_dwHP
		"  MaxHP                  INTEGER,"				// m_dwMaxHP
		"  SP                     INTEGER,"				// m_dwSP
		"  MaxSP                  INTEGER,"				// m_dwMaxSP
		"  Exp                    INTEGER,"				// m_dwExp
		"  Level                  INTEGER,"				// m_wLevel
		"  EquipItemIDCloth       INTEGER,"				// m_dwEquipItemIDCloth
		"  EquipItemIDAcce1       INTEGER,"				// m_dwEquipItemIDAcce1
		"  EquipItemIDAcce2       INTEGER,"				// m_dwEquipItemIDAcce2
		"  EquipItemIDArmsRight   INTEGER,"				// m_dwEquipItemIDArmsRight
		"  EquipItemIDArmsLeft    INTEGER,"				// m_dwEquipItemIDArmsLeft
		"  EquipItemIDHead        INTEGER,"				// m_dwEquipItemIDHead
		"  Block                  INTEGER,"				// m_bBlock
		"  Push                   INTEGER,"				// m_bPush
		"  IsNPC                  INTEGER,"				// m_bNPC
		"  Sex                    INTEGER,"				// m_nSex
		"  FamilyID               INTEGER,"				// m_wFamilyID
		"  GrpSize                INTEGER,"				// m_nGrpSize
		"  CharName               TEXT,"				// m_strCharName
		// ---- 外見 GrpID 群 (×15) ----
		"  GrpIDNPC               INTEGER,"				// m_wGrpIDNPC
		"  GrpIDCloth             INTEGER,"				// m_wGrpIDCloth
		"  GrpIDEye               INTEGER,"				// m_wGrpIDEye
		"  GrpIDEyeColor          INTEGER,"				// m_wGrpIDEyeColor
		"  GrpIDHairType          INTEGER,"				// m_wGrpIDHairType
		"  GrpIDHairColor         INTEGER,"				// m_wGrpIDHairColor
		"  GrpIDSP                INTEGER,"				// m_wGrpIDSP
		"  GrpIDTmpMain           INTEGER,"				// m_wGrpIDTmpMain
		"  GrpIDTmpSub            INTEGER,"				// m_wGrpIDTmpSub
		"  GrpIDAcce              INTEGER,"				// m_wGrpIDAcce
		"  GrpIDArmsMain          INTEGER,"				// m_wGrpIDArmsMain
		"  GrpIDArmsSub           INTEGER,"				// m_wGrpIDArmsSub
		"  GrpIDArmsLeftMain      INTEGER,"				// m_wGrpIDArmsLeftMain
		"  GrpIDArmsLeftSub       INTEGER,"				// m_wGrpIDArmsLeftSub
		"  GrpIDInitNPC           INTEGER,"				// m_wGrpIDInitNPC
		// ---- ステータス能力値 (×20) ----
		"  Stamina                INTEGER,"				// m_wStamina
		"  Power                  INTEGER,"				// m_wPower
		"  Strength               INTEGER,"				// m_wStrength
		"  Magic                  INTEGER,"				// m_wMagic
		"  Skillful               INTEGER,"				// m_wSkillful
		"  AbillityAT             INTEGER,"				// m_wAbillityAT
		"  AbillityDF             INTEGER,"				// m_wAbillityDF
		"  PAtack                 INTEGER,"				// m_wPAtack
		"  PDefense               INTEGER,"				// m_wPDefense
		"  PMagic                 INTEGER,"				// m_wPMagic
		"  PMagicDefense          INTEGER,"				// m_wPMagicDefense
		"  PHitAverage            INTEGER,"				// m_wPHitAverage
		"  PAvoidAverage          INTEGER,"				// m_wPAvoidAverage
		"  PCriticalAverage       INTEGER,"				// m_wPCriticalAverage
		"  AttrFire               INTEGER,"				// m_wAttrFire
		"  AttrWind               INTEGER,"				// m_wAttrWind
		"  AttrWater              INTEGER,"				// m_wAttrWater
		"  AttrEarth              INTEGER,"				// m_wAttrEarth
		"  AttrLight              INTEGER,"				// m_wAttrLight
		"  AttrDark               INTEGER,"				// m_wAttrDark
		// ---- 初期外見 GrpID (×6) ----
		"  GrpIDInitCloth         INTEGER,"				// m_wGrpIDInitCloth
		"  GrpIDInitEye           INTEGER,"				// m_wGrpIDInitEye
		"  GrpIDInitEyeColor      INTEGER,"				// m_wGrpIDInitEyeColor
		"  GrpIDInitHairType      INTEGER,"				// m_wGrpIDInitHairType
		"  GrpIDInitHairColor     INTEGER,"				// m_wGrpIDInitHairColor
		"  GrpIDInitSP            INTEGER,"				// m_wGrpIDInitSP
		// ---- 移動/検索スカラ (×9) ----
		"  MaxItemCount           INTEGER,"				// m_nMaxItemCount
		"  DropItemAverage        INTEGER,"				// m_nDropItemAverage
		"  MoveAverage            INTEGER,"				// m_nMoveAverage
		"  MoveAverageBattle      INTEGER,"				// m_nMoveAverageBattle
		"  MoveWait               INTEGER,"				// m_dwMoveWait
		"  MoveWaitBattle         INTEGER,"				// m_dwMoveWaitBattle
		"  SearchDistanceCX       INTEGER,"				// m_sizeSearchDistance.cx
		"  SearchDistanceCY       INTEGER,"				// m_sizeSearchDistance.cy
		"  MotionTypeID           INTEGER,"				// m_dwMotionTypeID
		// ---- NPC発生情報 (×6) ----
		"  PutCycle               INTEGER,"				// m_dwPutCycle
		"  PutMoveType            INTEGER,"				// m_nPutMoveType
		"  MaxPutCount            INTEGER,"				// m_nMaxPutCount
		"  PutAverage             INTEGER,"				// m_nPutAverage
		"  PutAreaX               INTEGER,"				// m_ptPutArea.x
		"  PutAreaY               INTEGER"				// m_ptPutArea.y
		");";

	sqlite3_exec(s_pDb, pszSql, NULL, NULL, NULL);

	// ---- サブテーブル: 所持アイテム ----
	sqlite3_exec(s_pDb,
		"CREATE TABLE IF NOT EXISTS sys_char_item("
		"  CharID  INTEGER NOT NULL,"
		"  Slot    INTEGER NOT NULL,"
		"  ItemID  INTEGER,"
		"  PRIMARY KEY (CharID, Slot)"
		");",
		NULL, NULL, NULL);

	// ---- サブテーブル: 所持スキル ----
	sqlite3_exec(s_pDb,
		"CREATE TABLE IF NOT EXISTS sys_char_skill("
		"  CharID  INTEGER NOT NULL,"
		"  Slot    INTEGER NOT NULL,"
		"  SkillID INTEGER,"
		"  PRIMARY KEY (CharID, Slot)"
		");",
		NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全キャラ情報を正規化テーブルに全置換
//   DELETE + INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoChar::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// ---- sys_char 全消去 ----
	sqlite3_exec(s_pDb, "DELETE FROM sys_char;", NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_char_item;", NULL, NULL, NULL);
	sqlite3_exec(s_pDb, "DELETE FROM sys_char_skill;", NULL, NULL, NULL);

	// ---- sys_char INSERT 文 ----
	const char* pszInsert =
		"INSERT INTO sys_char("
		"  CharID, MapID, MapX, MapY, Direction, MoveState, MoveType,"
		"  HP, MaxHP, SP, MaxSP, Exp, Level,"
		"  EquipItemIDCloth, EquipItemIDAcce1, EquipItemIDAcce2,"
		"  EquipItemIDArmsRight, EquipItemIDArmsLeft, EquipItemIDHead,"
		"  Block, Push, IsNPC, Sex, FamilyID, GrpSize, CharName,"
		// 外見 GrpID 群 ×15
		"  GrpIDNPC, GrpIDCloth, GrpIDEye, GrpIDEyeColor,"
		"  GrpIDHairType, GrpIDHairColor, GrpIDSP,"
		"  GrpIDTmpMain, GrpIDTmpSub, GrpIDAcce,"
		"  GrpIDArmsMain, GrpIDArmsSub, GrpIDArmsLeftMain, GrpIDArmsLeftSub,"
		"  GrpIDInitNPC,"
		// ステータス能力値 ×20
		"  Stamina, Power, Strength, Magic, Skillful,"
		"  AbillityAT, AbillityDF,"
		"  PAtack, PDefense, PMagic, PMagicDefense,"
		"  PHitAverage, PAvoidAverage, PCriticalAverage,"
		"  AttrFire, AttrWind, AttrWater, AttrEarth, AttrLight, AttrDark,"
		// 初期 GrpID ×6
		"  GrpIDInitCloth, GrpIDInitEye, GrpIDInitEyeColor,"
		"  GrpIDInitHairType, GrpIDInitHairColor, GrpIDInitSP,"
		// 移動/検索スカラ ×9
		"  MaxItemCount, DropItemAverage, MoveAverage, MoveAverageBattle,"
		"  MoveWait, MoveWaitBattle, SearchDistanceCX, SearchDistanceCY, MotionTypeID,"
		// NPC発生情報 ×6
		"  PutCycle, PutMoveType, MaxPutCount, PutAverage, PutAreaX, PutAreaY"
		") VALUES("
		"  ?,?,?,?,?,?,?,"	//  1- 7: CharID〜MoveType
		"  ?,?,?,?,?,?,"	//  8-13: HP〜Level
		"  ?,?,?,?,?,?,"	// 14-19: EquipItemID 6個
		"  ?,?,?,?,?,?,?,"	// 20-26: Block〜CharName
		// 外見 GrpID ×15
		"  ?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,"	// 27-41
		// ステータス ×20
		"  ?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,"	// 42-61
		// 初期 GrpID ×6
		"  ?,?,?,?,?,?,"	// 62-67
		// 移動/検索 ×9
		"  ?,?,?,?,?,?,?,?,?,"	// 68-76
		// NPC発生 ×6
		"  ?,?,?,?,?,?"		// 77-82
		");";

	sqlite3_stmt* pStmtChar = NULL;
	if (sqlite3_prepare_v2(s_pDb, pszInsert, -1, &pStmtChar, NULL) != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoChar::SaveToNormalTable: sys_char prepare failed\n");
		return;
	}

	// ---- sys_char_item INSERT 文 ----
	sqlite3_stmt* pStmtItem = NULL;
	sqlite3_prepare_v2(s_pDb,
		"INSERT INTO sys_char_item(CharID, Slot, ItemID) VALUES(?,?,?);",
		-1, &pStmtItem, NULL);

	// ---- sys_char_skill INSERT 文 ----
	sqlite3_stmt* pStmtSkill = NULL;
	sqlite3_prepare_v2(s_pDb,
		"INSERT INTO sys_char_skill(CharID, Slot, SkillID) VALUES(?,?,?);",
		-1, &pStmtSkill, NULL);

	// 保存対象インデックスを取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoCharBase pInfo = (PCInfoCharBase)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		int nCharID = (int)pInfo->m_dwCharID;

		// ==== sys_char バインド ====

		// 基本情報 (1-26)
		sqlite3_bind_int(pStmtChar,  1, nCharID);
		sqlite3_bind_int(pStmtChar,  2, (int)pInfo->m_dwMapID);
		sqlite3_bind_int(pStmtChar,  3, pInfo->m_nMapX);
		sqlite3_bind_int(pStmtChar,  4, pInfo->m_nMapY);
		sqlite3_bind_int(pStmtChar,  5, pInfo->m_nDirection);
		sqlite3_bind_int(pStmtChar,  6, pInfo->m_nMoveState);
		sqlite3_bind_int(pStmtChar,  7, pInfo->m_nMoveType);
		sqlite3_bind_int(pStmtChar,  8, (int)pInfo->m_dwHP);
		sqlite3_bind_int(pStmtChar,  9, (int)pInfo->m_dwMaxHP);
		sqlite3_bind_int(pStmtChar, 10, (int)pInfo->m_dwSP);
		sqlite3_bind_int(pStmtChar, 11, (int)pInfo->m_dwMaxSP);
		sqlite3_bind_int(pStmtChar, 12, (int)pInfo->m_dwExp);
		sqlite3_bind_int(pStmtChar, 13, (int)pInfo->m_wLevel);
		sqlite3_bind_int(pStmtChar, 14, (int)pInfo->m_dwEquipItemIDCloth);
		sqlite3_bind_int(pStmtChar, 15, (int)pInfo->m_dwEquipItemIDAcce1);
		sqlite3_bind_int(pStmtChar, 16, (int)pInfo->m_dwEquipItemIDAcce2);
		sqlite3_bind_int(pStmtChar, 17, (int)pInfo->m_dwEquipItemIDArmsRight);
		sqlite3_bind_int(pStmtChar, 18, (int)pInfo->m_dwEquipItemIDArmsLeft);
		sqlite3_bind_int(pStmtChar, 19, (int)pInfo->m_dwEquipItemIDHead);
		sqlite3_bind_int(pStmtChar, 20, (int)pInfo->m_bBlock);
		sqlite3_bind_int(pStmtChar, 21, (int)pInfo->m_bPush);
		sqlite3_bind_int(pStmtChar, 22, (int)pInfo->m_bNPC);
		sqlite3_bind_int(pStmtChar, 23, pInfo->m_nSex);
		sqlite3_bind_int(pStmtChar, 24, (int)pInfo->m_wFamilyID);
		sqlite3_bind_int(pStmtChar, 25, pInfo->m_nGrpSize);

		// キャラ名 (26)
		LPCSTR pszName = pInfo->m_strCharName.GetUtf8Pointer();
		sqlite3_bind_text(pStmtChar, 26, pszName, -1, SQLITE_TRANSIENT);

		// 外見 GrpID 群 (27-41)
		sqlite3_bind_int(pStmtChar, 27, (int)pInfo->m_wGrpIDNPC);
		sqlite3_bind_int(pStmtChar, 28, (int)pInfo->m_wGrpIDCloth);
		sqlite3_bind_int(pStmtChar, 29, (int)pInfo->m_wGrpIDEye);
		sqlite3_bind_int(pStmtChar, 30, (int)pInfo->m_wGrpIDEyeColor);
		sqlite3_bind_int(pStmtChar, 31, (int)pInfo->m_wGrpIDHairType);
		sqlite3_bind_int(pStmtChar, 32, (int)pInfo->m_wGrpIDHairColor);
		sqlite3_bind_int(pStmtChar, 33, (int)pInfo->m_wGrpIDSP);
		sqlite3_bind_int(pStmtChar, 34, (int)pInfo->m_wGrpIDTmpMain);
		sqlite3_bind_int(pStmtChar, 35, (int)pInfo->m_wGrpIDTmpSub);
		sqlite3_bind_int(pStmtChar, 36, (int)pInfo->m_wGrpIDAcce);
		sqlite3_bind_int(pStmtChar, 37, (int)pInfo->m_wGrpIDArmsMain);
		sqlite3_bind_int(pStmtChar, 38, (int)pInfo->m_wGrpIDArmsSub);
		sqlite3_bind_int(pStmtChar, 39, (int)pInfo->m_wGrpIDArmsLeftMain);
		sqlite3_bind_int(pStmtChar, 40, (int)pInfo->m_wGrpIDArmsLeftSub);
		sqlite3_bind_int(pStmtChar, 41, (int)pInfo->m_wGrpIDInitNPC);

		// ステータス能力値 (42-61)
		sqlite3_bind_int(pStmtChar, 42, (int)pInfo->m_wStamina);
		sqlite3_bind_int(pStmtChar, 43, (int)pInfo->m_wPower);
		sqlite3_bind_int(pStmtChar, 44, (int)pInfo->m_wStrength);
		sqlite3_bind_int(pStmtChar, 45, (int)pInfo->m_wMagic);
		sqlite3_bind_int(pStmtChar, 46, (int)pInfo->m_wSkillful);
		sqlite3_bind_int(pStmtChar, 47, (int)pInfo->m_wAbillityAT);
		sqlite3_bind_int(pStmtChar, 48, (int)pInfo->m_wAbillityDF);
		sqlite3_bind_int(pStmtChar, 49, (int)pInfo->m_wPAtack);
		sqlite3_bind_int(pStmtChar, 50, (int)pInfo->m_wPDefense);
		sqlite3_bind_int(pStmtChar, 51, (int)pInfo->m_wPMagic);
		sqlite3_bind_int(pStmtChar, 52, (int)pInfo->m_wPMagicDefense);
		sqlite3_bind_int(pStmtChar, 53, (int)pInfo->m_wPHitAverage);
		sqlite3_bind_int(pStmtChar, 54, (int)pInfo->m_wPAvoidAverage);
		sqlite3_bind_int(pStmtChar, 55, (int)pInfo->m_wPCriticalAverage);
		sqlite3_bind_int(pStmtChar, 56, (int)pInfo->m_wAttrFire);
		sqlite3_bind_int(pStmtChar, 57, (int)pInfo->m_wAttrWind);
		sqlite3_bind_int(pStmtChar, 58, (int)pInfo->m_wAttrWater);
		sqlite3_bind_int(pStmtChar, 59, (int)pInfo->m_wAttrEarth);
		sqlite3_bind_int(pStmtChar, 60, (int)pInfo->m_wAttrLight);
		sqlite3_bind_int(pStmtChar, 61, (int)pInfo->m_wAttrDark);

		// 初期外見 GrpID (62-67)
		sqlite3_bind_int(pStmtChar, 62, (int)pInfo->m_wGrpIDInitCloth);
		sqlite3_bind_int(pStmtChar, 63, (int)pInfo->m_wGrpIDInitEye);
		sqlite3_bind_int(pStmtChar, 64, (int)pInfo->m_wGrpIDInitEyeColor);
		sqlite3_bind_int(pStmtChar, 65, (int)pInfo->m_wGrpIDInitHairType);
		sqlite3_bind_int(pStmtChar, 66, (int)pInfo->m_wGrpIDInitHairColor);
		sqlite3_bind_int(pStmtChar, 67, (int)pInfo->m_wGrpIDInitSP);

		// 移動/検索スカラ (68-76)
		sqlite3_bind_int(pStmtChar, 68, pInfo->m_nMaxItemCount);
		sqlite3_bind_int(pStmtChar, 69, pInfo->m_nDropItemAverage);
		sqlite3_bind_int(pStmtChar, 70, pInfo->m_nMoveAverage);
		sqlite3_bind_int(pStmtChar, 71, pInfo->m_nMoveAverageBattle);
		sqlite3_bind_int(pStmtChar, 72, (int)pInfo->m_dwMoveWait);
		sqlite3_bind_int(pStmtChar, 73, (int)pInfo->m_dwMoveWaitBattle);
		sqlite3_bind_int(pStmtChar, 74, (int)pInfo->m_sizeSearchDistance.cx);
		sqlite3_bind_int(pStmtChar, 75, (int)pInfo->m_sizeSearchDistance.cy);
		sqlite3_bind_int(pStmtChar, 76, (int)pInfo->m_dwMotionTypeID);

		// NPC発生情報 (77-82)
		sqlite3_bind_int(pStmtChar, 77, (int)pInfo->m_dwPutCycle);
		sqlite3_bind_int(pStmtChar, 78, pInfo->m_nPutMoveType);
		sqlite3_bind_int(pStmtChar, 79, pInfo->m_nMaxPutCount);
		sqlite3_bind_int(pStmtChar, 80, pInfo->m_nPutAverage);
		sqlite3_bind_int(pStmtChar, 81, pInfo->m_ptPutArea.x);
		sqlite3_bind_int(pStmtChar, 82, pInfo->m_ptPutArea.y);

		sqlite3_step(pStmtChar);
		sqlite3_reset(pStmtChar);

		// ==== sys_char_item バインド ====
		if (pStmtItem != NULL) {
			int nItemCount = (int)pInfo->m_adwItemID.size();
			for (int j = 0; j < nItemCount; j++) {
				sqlite3_bind_int(pStmtItem, 1, nCharID);
				sqlite3_bind_int(pStmtItem, 2, j);
				sqlite3_bind_int(pStmtItem, 3, (int)pInfo->m_adwItemID[j]);
				sqlite3_step(pStmtItem);
				sqlite3_reset(pStmtItem);
			}
		}

		// ==== sys_char_skill バインド ====
		if (pStmtSkill != NULL) {
			int nSkillCount = (int)pInfo->m_adwSkillID.size();
			for (int j = 0; j < nSkillCount; j++) {
				sqlite3_bind_int(pStmtSkill, 1, nCharID);
				sqlite3_bind_int(pStmtSkill, 2, j);
				sqlite3_bind_int(pStmtSkill, 3, (int)pInfo->m_adwSkillID[j]);
				sqlite3_step(pStmtSkill);
				sqlite3_reset(pStmtSkill);
			}
		}
	}

	sqlite3_finalize(pStmtChar);
	if (pStmtItem  != NULL) sqlite3_finalize(pStmtItem);
	if (pStmtSkill != NULL) sqlite3_finalize(pStmtSkill);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
// ============================================================
BOOL CSaveLoadInfoChar::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	// ---- sys_char SELECT ----
	const char* pszSelect =
		"SELECT"
		"  CharID, MapID, MapX, MapY, Direction, MoveState, MoveType,"		// 0-6
		"  HP, MaxHP, SP, MaxSP, Exp, Level,"								// 7-12
		"  EquipItemIDCloth, EquipItemIDAcce1, EquipItemIDAcce2,"			// 13-15
		"  EquipItemIDArmsRight, EquipItemIDArmsLeft, EquipItemIDHead,"		// 16-18
		"  Block, Push, IsNPC, Sex, FamilyID, GrpSize, CharName,"			// 19-25
		// 外見 GrpID 群 ×15 (26-40)
		"  GrpIDNPC, GrpIDCloth, GrpIDEye, GrpIDEyeColor,"
		"  GrpIDHairType, GrpIDHairColor, GrpIDSP,"
		"  GrpIDTmpMain, GrpIDTmpSub, GrpIDAcce,"
		"  GrpIDArmsMain, GrpIDArmsSub, GrpIDArmsLeftMain, GrpIDArmsLeftSub,"
		"  GrpIDInitNPC,"
		// ステータス能力値 ×20 (41-60)
		"  Stamina, Power, Strength, Magic, Skillful,"
		"  AbillityAT, AbillityDF,"
		"  PAtack, PDefense, PMagic, PMagicDefense,"
		"  PHitAverage, PAvoidAverage, PCriticalAverage,"
		"  AttrFire, AttrWind, AttrWater, AttrEarth, AttrLight, AttrDark,"
		// 初期 GrpID ×6 (61-66)
		"  GrpIDInitCloth, GrpIDInitEye, GrpIDInitEyeColor,"
		"  GrpIDInitHairType, GrpIDInitHairColor, GrpIDInitSP,"
		// 移動/検索スカラ ×9 (67-75)
		"  MaxItemCount, DropItemAverage, MoveAverage, MoveAverageBattle,"
		"  MoveWait, MoveWaitBattle, SearchDistanceCX, SearchDistanceCY, MotionTypeID,"
		// NPC発生情報 ×6 (76-81)
		"  PutCycle, PutMoveType, MaxPutCount, PutAverage, PutAreaX, PutAreaY"
		" FROM sys_char;";

	sqlite3_stmt* pStmt = NULL;
	if (sqlite3_prepare_v2(s_pDb, pszSelect, -1, &pStmt, NULL) != SQLITE_OK) return FALSE;

	CLibInfoCharBase* pLibChar = (CLibInfoCharBase*)pDst;

	// CharID -> ポインタの一時マップ (サブテーブル読込時の ID 検索用)
	// ※ pDst->GetPtr(n) は配列インデックスを要求するため、CharID での検索には使えない。
	//    RenewIDPtr() は末尾で呼ぶので、ここでは自前マップを作る。
	std::map<DWORD, PCInfoCharBase> mapCharByID;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		int nMoveType = sqlite3_column_int(pStmt, 6);
		PCInfoCharBase pInfo = (PCInfoCharBase)pLibChar->GetNew(nMoveType);
		if (pInfo == NULL) continue;

		// 基本情報 (0-25)
		pInfo->m_dwCharID               = (DWORD)sqlite3_column_int(pStmt,  0);
		pInfo->m_dwMapID                = (DWORD)sqlite3_column_int(pStmt,  1);
		pInfo->m_nMapX                  = sqlite3_column_int(pStmt,  2);
		pInfo->m_nMapY                  = sqlite3_column_int(pStmt,  3);
		pInfo->m_nDirection             = sqlite3_column_int(pStmt,  4);
		pInfo->m_nMoveState             = sqlite3_column_int(pStmt,  5);
		pInfo->m_nMoveType              = sqlite3_column_int(pStmt,  6);
		pInfo->m_dwHP                   = (DWORD)sqlite3_column_int(pStmt,  7);
		pInfo->m_dwMaxHP                = (DWORD)sqlite3_column_int(pStmt,  8);
		pInfo->m_dwSP                   = (DWORD)sqlite3_column_int(pStmt,  9);
		pInfo->m_dwMaxSP                = (DWORD)sqlite3_column_int(pStmt, 10);
		pInfo->m_dwExp                  = (DWORD)sqlite3_column_int(pStmt, 11);
		pInfo->m_wLevel                 = (WORD)sqlite3_column_int(pStmt,  12);
		pInfo->m_dwEquipItemIDCloth     = (DWORD)sqlite3_column_int(pStmt, 13);
		pInfo->m_dwEquipItemIDAcce1     = (DWORD)sqlite3_column_int(pStmt, 14);
		pInfo->m_dwEquipItemIDAcce2     = (DWORD)sqlite3_column_int(pStmt, 15);
		pInfo->m_dwEquipItemIDArmsRight = (DWORD)sqlite3_column_int(pStmt, 16);
		pInfo->m_dwEquipItemIDArmsLeft  = (DWORD)sqlite3_column_int(pStmt, 17);
		pInfo->m_dwEquipItemIDHead      = (DWORD)sqlite3_column_int(pStmt, 18);
		pInfo->m_bBlock                 = (BOOL)sqlite3_column_int(pStmt,  19);
		pInfo->m_bPush                  = (BOOL)sqlite3_column_int(pStmt,  20);
		pInfo->m_bNPC                   = (BOOL)sqlite3_column_int(pStmt,  21);
		pInfo->m_nSex                   = sqlite3_column_int(pStmt,  22);
		pInfo->m_wFamilyID              = (WORD)sqlite3_column_int(pStmt,  23);
		pInfo->m_nGrpSize               = sqlite3_column_int(pStmt,  24);

		const char* pszName = (const char*)sqlite3_column_text(pStmt, 25);
		if (pszName != NULL) pInfo->m_strCharName = (LPCTSTR)Utf8ToTString(pszName);

		// 外見 GrpID 群 (26-40)
		pInfo->m_wGrpIDNPC          = (WORD)sqlite3_column_int(pStmt, 26);
		pInfo->m_wGrpIDCloth        = (WORD)sqlite3_column_int(pStmt, 27);
		pInfo->m_wGrpIDEye          = (WORD)sqlite3_column_int(pStmt, 28);
		pInfo->m_wGrpIDEyeColor     = (WORD)sqlite3_column_int(pStmt, 29);
		pInfo->m_wGrpIDHairType     = (WORD)sqlite3_column_int(pStmt, 30);
		pInfo->m_wGrpIDHairColor    = (WORD)sqlite3_column_int(pStmt, 31);
		pInfo->m_wGrpIDSP           = (WORD)sqlite3_column_int(pStmt, 32);
		pInfo->m_wGrpIDTmpMain      = (WORD)sqlite3_column_int(pStmt, 33);
		pInfo->m_wGrpIDTmpSub       = (WORD)sqlite3_column_int(pStmt, 34);
		pInfo->m_wGrpIDAcce         = (WORD)sqlite3_column_int(pStmt, 35);
		pInfo->m_wGrpIDArmsMain     = (WORD)sqlite3_column_int(pStmt, 36);
		pInfo->m_wGrpIDArmsSub      = (WORD)sqlite3_column_int(pStmt, 37);
		pInfo->m_wGrpIDArmsLeftMain = (WORD)sqlite3_column_int(pStmt, 38);
		pInfo->m_wGrpIDArmsLeftSub  = (WORD)sqlite3_column_int(pStmt, 39);
		pInfo->m_wGrpIDInitNPC      = (WORD)sqlite3_column_int(pStmt, 40);

		// ステータス能力値 (41-60)
		pInfo->m_wStamina           = (WORD)sqlite3_column_int(pStmt, 41);
		pInfo->m_wPower             = (WORD)sqlite3_column_int(pStmt, 42);
		pInfo->m_wStrength          = (WORD)sqlite3_column_int(pStmt, 43);
		pInfo->m_wMagic             = (WORD)sqlite3_column_int(pStmt, 44);
		pInfo->m_wSkillful          = (WORD)sqlite3_column_int(pStmt, 45);
		pInfo->m_wAbillityAT        = (WORD)sqlite3_column_int(pStmt, 46);
		pInfo->m_wAbillityDF        = (WORD)sqlite3_column_int(pStmt, 47);
		pInfo->m_wPAtack            = (WORD)sqlite3_column_int(pStmt, 48);
		pInfo->m_wPDefense          = (WORD)sqlite3_column_int(pStmt, 49);
		pInfo->m_wPMagic            = (WORD)sqlite3_column_int(pStmt, 50);
		pInfo->m_wPMagicDefense     = (WORD)sqlite3_column_int(pStmt, 51);
		pInfo->m_wPHitAverage       = (WORD)sqlite3_column_int(pStmt, 52);
		pInfo->m_wPAvoidAverage     = (WORD)sqlite3_column_int(pStmt, 53);
		pInfo->m_wPCriticalAverage  = (WORD)sqlite3_column_int(pStmt, 54);
		pInfo->m_wAttrFire          = (WORD)sqlite3_column_int(pStmt, 55);
		pInfo->m_wAttrWind          = (WORD)sqlite3_column_int(pStmt, 56);
		pInfo->m_wAttrWater         = (WORD)sqlite3_column_int(pStmt, 57);
		pInfo->m_wAttrEarth         = (WORD)sqlite3_column_int(pStmt, 58);
		pInfo->m_wAttrLight         = (WORD)sqlite3_column_int(pStmt, 59);
		pInfo->m_wAttrDark          = (WORD)sqlite3_column_int(pStmt, 60);

		// 初期外見 GrpID (61-66)
		pInfo->m_wGrpIDInitCloth    = (WORD)sqlite3_column_int(pStmt, 61);
		pInfo->m_wGrpIDInitEye      = (WORD)sqlite3_column_int(pStmt, 62);
		pInfo->m_wGrpIDInitEyeColor = (WORD)sqlite3_column_int(pStmt, 63);
		pInfo->m_wGrpIDInitHairType = (WORD)sqlite3_column_int(pStmt, 64);
		pInfo->m_wGrpIDInitHairColor= (WORD)sqlite3_column_int(pStmt, 65);
		pInfo->m_wGrpIDInitSP       = (WORD)sqlite3_column_int(pStmt, 66);

		// 移動/検索スカラ (67-75)
		pInfo->m_nMaxItemCount         = sqlite3_column_int(pStmt, 67);
		pInfo->m_nDropItemAverage      = sqlite3_column_int(pStmt, 68);
		pInfo->m_nMoveAverage          = sqlite3_column_int(pStmt, 69);
		pInfo->m_nMoveAverageBattle    = sqlite3_column_int(pStmt, 70);
		pInfo->m_dwMoveWait            = (DWORD)sqlite3_column_int(pStmt, 71);
		pInfo->m_dwMoveWaitBattle      = (DWORD)sqlite3_column_int(pStmt, 72);
		pInfo->m_sizeSearchDistance.cx = (LONG)sqlite3_column_int(pStmt, 73);
		pInfo->m_sizeSearchDistance.cy = (LONG)sqlite3_column_int(pStmt, 74);
		pInfo->m_dwMotionTypeID        = (DWORD)sqlite3_column_int(pStmt, 75);

		// NPC発生情報 (76-81)
		pInfo->m_dwPutCycle   = (DWORD)sqlite3_column_int(pStmt, 76);
		pInfo->m_nPutMoveType = sqlite3_column_int(pStmt, 77);
		pInfo->m_nMaxPutCount = sqlite3_column_int(pStmt, 78);
		pInfo->m_nPutAverage  = sqlite3_column_int(pStmt, 79);
		pInfo->m_ptPutArea.x  = sqlite3_column_int(pStmt, 80);
		pInfo->m_ptPutArea.y  = sqlite3_column_int(pStmt, 81);

		// ライブラリに追加（m_dwCharID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		mapCharByID[pInfo->m_dwCharID] = pInfo;
		nRowCount++;
	}
	sqlite3_finalize(pStmt);

	// ---- sys_char_item から所持アイテム読み込み ----
	{
		sqlite3_stmt* pStmtItem = NULL;
		const char* pszSelItem =
			"SELECT CharID, Slot, ItemID FROM sys_char_item ORDER BY CharID, Slot;";
		if (sqlite3_prepare_v2(s_pDb, pszSelItem, -1, &pStmtItem, NULL) == SQLITE_OK) {
			while (sqlite3_step(pStmtItem) == SQLITE_ROW) {
				DWORD dwCharID = (DWORD)sqlite3_column_int(pStmtItem, 0);
				// int nSlot   = sqlite3_column_int(pStmtItem, 1); // Slot はシーケンシャルに順序保証
				DWORD dwItemID = (DWORD)sqlite3_column_int(pStmtItem, 2);

				auto it = mapCharByID.find(dwCharID);
				if (it != mapCharByID.end()) {
					it->second->m_adwItemID.push_back(dwItemID);
				}
			}
			sqlite3_finalize(pStmtItem);
		}
	}

	// ---- sys_char_skill から所持スキル読み込み ----
	{
		sqlite3_stmt* pStmtSkill = NULL;
		const char* pszSelSkill =
			"SELECT CharID, Slot, SkillID FROM sys_char_skill ORDER BY CharID, Slot;";
		if (sqlite3_prepare_v2(s_pDb, pszSelSkill, -1, &pStmtSkill, NULL) == SQLITE_OK) {
			while (sqlite3_step(pStmtSkill) == SQLITE_ROW) {
				DWORD dwCharID  = (DWORD)sqlite3_column_int(pStmtSkill, 0);
				DWORD dwSkillID = (DWORD)sqlite3_column_int(pStmtSkill, 2);

				auto it = mapCharByID.find(dwCharID);
				if (it != mapCharByID.end()) {
					it->second->m_adwSkillID.push_back(dwSkillID);
				}
			}
			sqlite3_finalize(pStmtSkill);
		}
	}

	// ID 検索マップを更新
	pDst->RenewIDPtr();

	return (nRowCount > 0) ? TRUE : FALSE;
}

// ============================================================
// 内部ヘルパー: BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
// ============================================================
BOOL CSaveLoadInfoChar::MigrateFromBlob(PCLibInfoBase pDst)
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

	// sbo_data から 'Char' 行を削除（あれば）
	if (s_pDb != NULL) {
		sqlite3_exec(s_pDb, "DELETE FROM sbo_data WHERE name='Char';", NULL, NULL, NULL);
		OutputDebugStringA("SaveLoadInfoChar: BLOB → 正規化テーブルへマイグレーション完了\n");
	}

	return TRUE;
}

// ============================================================
// Save のオーバーライド: 正規化テーブルにのみ書く
// ============================================================
void CSaveLoadInfoChar::Save(PCLibInfoBase pSrc)
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
void CSaveLoadInfoChar::Load(PCLibInfoBase pDst)
{
	if (pDst == NULL) return;

	m_pLibInfoBase = pDst;

	// テーブルが無ければ作成（旧 BLOB スキーマ検出時は自動マイグレーション）
	EnsureTable();

	// 1. 正規化テーブルに行があれば読み込んで完了
	if (LoadFromNormalTable(pDst)) {
		OutputDebugStringA("SaveLoadInfoChar: 正規化テーブルから読み込み成功\n");
		return;
	}

	// 2. 行がなければ BLOB / .dat からマイグレーション
	OutputDebugStringA("SaveLoadInfoChar: 正規化テーブルが空 → BLOB/.dat からマイグレーション\n");
	MigrateFromBlob(pDst);
}
