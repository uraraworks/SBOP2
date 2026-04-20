/// @file SaveLoadInfoChar.cpp
/// @brief データ保存・読み込みクラス(キャラ情報) 実装ファイル
///        Char ライブラリを SQLite 正規化テーブル (sys_char) に移行
///
///  テーブル構成:
///    sys_char — キャラ情報メインテーブル
///      主キー   : CharID (INTEGER)
///      個別カラム: MapID / MapX / MapY / Direction / MoveState / MoveType
///                  HP / MaxHP / SP / MaxSP / Exp / Level
///                  EquipItemIDCloth / EquipItemIDAcce1 / EquipItemIDAcce2
///                  EquipItemIDArmsRight / EquipItemIDArmsLeft / EquipItemIDHead
///                  Block / Push / bNPC / Sex / FamilyID / CharName (TEXT)
///      BLOB カラム:
///                  GrpIDData       — 外見 GrpID 群 (15 WORD: GrpIDNPC〜GrpIDInitSP)
///                  StatusData      — ステータス能力値 (20 WORD: Stamina〜AttrDark)
///                  MoveData        — 移動/検索関連スカラ (int×5 + DWORD×3)
///                  PutNpcData      — NPC発生情報 (dwPutCycle + int×3 + POINT)
///                  ItemData        — 所持アイテムID配列 (DWORD 可変長)
///                  SkillData       — 所持スキルID配列 (DWORD 可変長)
///
///  折衷 BLOB 方針:
///    外見 GrpID (WORD×15) / ステータス能力値 (WORD×20) / 移動関連スカラ /
///    NPC発生情報 / 所持アイテム配列 / 所持スキル配列 を BLOB 格納することで
///    カラム数を抑え実装コストを削減する。
///    CharID / MapID / HP / SP / Level / 装備アイテムID は SQLite で直接参照できるよう
///    個別カラムに展開する。
///
///  派生クラス対応:
///    CInfoCharSvr / CInfoCharATACKANIMESvr / CInfoCharMOVEATACKSvr /
///    CInfoCharBATTLE1Svr / CInfoCharBATTLE2Svr / CInfoCharPUTNPC は
///    全て CInfoCharBase の拡張であり保存フィールドは共通。
///    m_nMoveType カラムで型を識別し、読み込み時に GetNew(m_nMoveType) で
///    正しい派生クラスのインスタンスを生成する。
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "InfoCharBase.h"
#include "InfoCharSvr.h"
#include "LibInfoCharBase.h"
#include "SaveLoadInfoChar.h"

// テーブル名
static const char* s_pszTableChar = "sys_char";

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
// 内部ヘルパー: テーブルが無ければ CREATE TABLE
//   メインテーブル sys_char を作成する
//   スカラ識別子は個別カラム、可変長・大量フィールドは BLOB 格納
// ============================================================
void CSaveLoadInfoChar::EnsureTable(void)
{
	if (s_pDb == NULL) return;

	// メインテーブル
	const char* pszSql =
		"CREATE TABLE IF NOT EXISTS sys_char("
		"  CharID              INTEGER PRIMARY KEY,"	// m_dwCharID
		"  MapID               INTEGER,"			// m_dwMapID
		"  MapX                INTEGER,"			// m_nMapX
		"  MapY                INTEGER,"			// m_nMapY
		"  Direction           INTEGER,"			// m_nDirection
		"  MoveState           INTEGER,"			// m_nMoveState
		"  MoveType            INTEGER,"			// m_nMoveType (派生クラス識別)
		"  HP                  INTEGER,"			// m_dwHP
		"  MaxHP               INTEGER,"			// m_dwMaxHP
		"  SP                  INTEGER,"			// m_dwSP
		"  MaxSP               INTEGER,"			// m_dwMaxSP
		"  Exp                 INTEGER,"			// m_dwExp
		"  Level               INTEGER,"			// m_wLevel
		"  EquipItemIDCloth    INTEGER,"			// m_dwEquipItemIDCloth
		"  EquipItemIDAcce1    INTEGER,"			// m_dwEquipItemIDAcce1
		"  EquipItemIDAcce2    INTEGER,"			// m_dwEquipItemIDAcce2
		"  EquipItemIDArmsRight INTEGER,"			// m_dwEquipItemIDArmsRight
		"  EquipItemIDArmsLeft  INTEGER,"			// m_dwEquipItemIDArmsLeft
		"  EquipItemIDHead     INTEGER,"			// m_dwEquipItemIDHead
		"  Block               INTEGER,"			// m_bBlock
		"  Push                INTEGER,"			// m_bPush
		"  IsNPC               INTEGER,"			// m_bNPC (NPC フラグ)
		"  Sex                 INTEGER,"			// m_nSex
		"  FamilyID            INTEGER,"			// m_wFamilyID
		"  GrpSize             INTEGER,"			// m_nGrpSize
		"  CharName            TEXT,"				// m_strCharName (SJIS バイト列)
		// ---- 折衷 BLOB カラム ----
		"  GrpIDData           BLOB,"				// 外見 GrpID 群 (WORD×15)
		"  StatusData          BLOB,"				// ステータス能力値 (WORD×20)
		"  MoveData            BLOB,"				// 移動/検索関連スカラ
		"  PutNpcData          BLOB,"				// NPC発生情報
		"  ItemData            BLOB,"				// 所持アイテムID配列 (DWORD 可変長)
		"  SkillData           BLOB"				// 所持スキルID配列 (DWORD 可変長)
		");";

	sqlite3_exec(s_pDb, pszSql, NULL, NULL, NULL);
}

// ============================================================
// 内部ヘルパー: メモリ上の全キャラ情報を正規化テーブルに全置換
//   DELETE + INSERT ループ（外側トランザクションに乗る）
// ============================================================
void CSaveLoadInfoChar::SaveToNormalTable(void)
{
	if (s_pDb == NULL || m_pLibInfoBase == NULL) return;

	// 既存行を全消去
	sqlite3_exec(s_pDb, "DELETE FROM sys_char;", NULL, NULL, NULL);

	// INSERT 文
	const char* pszInsert =
		"INSERT INTO sys_char("
		"  CharID, MapID, MapX, MapY, Direction, MoveState, MoveType,"
		"  HP, MaxHP, SP, MaxSP, Exp, Level,"
		"  EquipItemIDCloth, EquipItemIDAcce1, EquipItemIDAcce2,"
		"  EquipItemIDArmsRight, EquipItemIDArmsLeft, EquipItemIDHead,"
		"  Block, Push, IsNPC, Sex, FamilyID, GrpSize, CharName,"
		"  GrpIDData, StatusData, MoveData, PutNpcData, ItemData, SkillData"
		") VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";

	sqlite3_stmt* pStmt = NULL;
	int nRet = sqlite3_prepare_v2(s_pDb, pszInsert, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) {
		OutputDebugStringA("SaveLoadInfoChar::SaveToNormalTable: prepare failed\n");
		return;
	}

	// 保存対象インデックスを取得
	ARRAYINT anNo;
	m_pLibInfoBase->GetSaveNo(anNo);

	int nCount = (int)anNo.size();
	for (int i = 0; i < nCount; i++) {
		PCInfoCharBase pInfo = (PCInfoCharBase)m_pLibInfoBase->GetPtr(anNo[i]);
		if (pInfo == NULL) continue;

		// ---- 個別カラムのバインド ----
		sqlite3_bind_int(pStmt,  1, (int)pInfo->m_dwCharID);
		sqlite3_bind_int(pStmt,  2, (int)pInfo->m_dwMapID);
		sqlite3_bind_int(pStmt,  3, pInfo->m_nMapX);
		sqlite3_bind_int(pStmt,  4, pInfo->m_nMapY);
		sqlite3_bind_int(pStmt,  5, pInfo->m_nDirection);
		sqlite3_bind_int(pStmt,  6, pInfo->m_nMoveState);
		sqlite3_bind_int(pStmt,  7, pInfo->m_nMoveType);
		sqlite3_bind_int(pStmt,  8, (int)pInfo->m_dwHP);
		sqlite3_bind_int(pStmt,  9, (int)pInfo->m_dwMaxHP);
		sqlite3_bind_int(pStmt, 10, (int)pInfo->m_dwSP);
		sqlite3_bind_int(pStmt, 11, (int)pInfo->m_dwMaxSP);
		sqlite3_bind_int(pStmt, 12, (int)pInfo->m_dwExp);
		sqlite3_bind_int(pStmt, 13, (int)pInfo->m_wLevel);
		sqlite3_bind_int(pStmt, 14, (int)pInfo->m_dwEquipItemIDCloth);
		sqlite3_bind_int(pStmt, 15, (int)pInfo->m_dwEquipItemIDAcce1);
		sqlite3_bind_int(pStmt, 16, (int)pInfo->m_dwEquipItemIDAcce2);
		sqlite3_bind_int(pStmt, 17, (int)pInfo->m_dwEquipItemIDArmsRight);
		sqlite3_bind_int(pStmt, 18, (int)pInfo->m_dwEquipItemIDArmsLeft);
		sqlite3_bind_int(pStmt, 19, (int)pInfo->m_dwEquipItemIDHead);
		sqlite3_bind_int(pStmt, 20, (int)pInfo->m_bBlock);
		sqlite3_bind_int(pStmt, 21, (int)pInfo->m_bPush);
		sqlite3_bind_int(pStmt, 22, (int)pInfo->m_bNPC);
		sqlite3_bind_int(pStmt, 23, pInfo->m_nSex);
		sqlite3_bind_int(pStmt, 24, (int)pInfo->m_wFamilyID);
		sqlite3_bind_int(pStmt, 25, pInfo->m_nGrpSize);

		// キャラ名: SJIS バイト列をそのまま TEXT としてバインド
		LPCSTR pszName = (LPCSTR)pInfo->m_strCharName;
		sqlite3_bind_text(pStmt, 26, pszName, -1, SQLITE_TRANSIENT);

		// ---- BLOB: 外見 GrpID 群 (WORD×15) ----
		//   GrpIDNPC / Cloth / Eye / EyeColor / HairType / HairColor / SP /
		//   TmpMain / TmpSub / Acce / ArmsMain / ArmsSub / ArmsLeftMain / ArmsLeftSub / InitNPC
		//   ※ InitCloth / InitEye / InitEyeColor / InitHairType / InitHairColor / InitSP は StatusData に含める
		WORD awGrpID[15] = {
			pInfo->m_wGrpIDNPC,
			pInfo->m_wGrpIDCloth,
			pInfo->m_wGrpIDEye,
			pInfo->m_wGrpIDEyeColor,
			pInfo->m_wGrpIDHairType,
			pInfo->m_wGrpIDHairColor,
			pInfo->m_wGrpIDSP,
			pInfo->m_wGrpIDTmpMain,
			pInfo->m_wGrpIDTmpSub,
			pInfo->m_wGrpIDAcce,
			pInfo->m_wGrpIDArmsMain,
			pInfo->m_wGrpIDArmsSub,
			pInfo->m_wGrpIDArmsLeftMain,
			pInfo->m_wGrpIDArmsLeftSub,
			pInfo->m_wGrpIDInitNPC,
		};
		sqlite3_bind_blob(pStmt, 27, awGrpID, (int)sizeof(awGrpID), SQLITE_TRANSIENT);

		// ---- BLOB: ステータス能力値 (WORD×20) + 初期 GrpID 群 (WORD×6) ----
		//   Stamina / Power / Strength / Magic / Skillful /
		//   AbillityAT / AbillityDF /
		//   PAtack / PDefense / PMagic / PMagicDefense / PHitAverage / PAvoidAverage / PCriticalAverage /
		//   AttrFire / AttrWind / AttrWater / AttrEarth / AttrLight / AttrDark /
		//   InitCloth / InitEye / InitEyeColor / InitHairType / InitHairColor / InitSP
		WORD awStatus[26] = {
			pInfo->m_wStamina,
			pInfo->m_wPower,
			pInfo->m_wStrength,
			pInfo->m_wMagic,
			pInfo->m_wSkillful,
			pInfo->m_wAbillityAT,
			pInfo->m_wAbillityDF,
			pInfo->m_wPAtack,
			pInfo->m_wPDefense,
			pInfo->m_wPMagic,
			pInfo->m_wPMagicDefense,
			pInfo->m_wPHitAverage,
			pInfo->m_wPAvoidAverage,
			pInfo->m_wPCriticalAverage,
			pInfo->m_wAttrFire,
			pInfo->m_wAttrWind,
			pInfo->m_wAttrWater,
			pInfo->m_wAttrEarth,
			pInfo->m_wAttrLight,
			pInfo->m_wAttrDark,
			pInfo->m_wGrpIDInitCloth,
			pInfo->m_wGrpIDInitEye,
			pInfo->m_wGrpIDInitEyeColor,
			pInfo->m_wGrpIDInitHairType,
			pInfo->m_wGrpIDInitHairColor,
			pInfo->m_wGrpIDInitSP,
		};
		sqlite3_bind_blob(pStmt, 28, awStatus, (int)sizeof(awStatus), SQLITE_TRANSIENT);

		// ---- BLOB: 移動/検索関連スカラ ----
		//   nMaxItemCount / nDropItemAverage / nMoveAverage / nMoveAverageBattle /
		//   dwMoveWait / dwMoveWaitBattle / sizeSearchDistance (cx, cy)
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
		moveData.nMaxItemCount      = pInfo->m_nMaxItemCount;
		moveData.nDropItemAverage   = pInfo->m_nDropItemAverage;
		moveData.nMoveAverage       = pInfo->m_nMoveAverage;
		moveData.nMoveAverageBattle = pInfo->m_nMoveAverageBattle;
		moveData.dwMoveWait         = pInfo->m_dwMoveWait;
		moveData.dwMoveWaitBattle   = pInfo->m_dwMoveWaitBattle;
		moveData.nSearchDistanceCX  = (int)pInfo->m_sizeSearchDistance.cx;
		moveData.nSearchDistanceCY  = (int)pInfo->m_sizeSearchDistance.cy;
		moveData.dwMotionTypeID     = pInfo->m_dwMotionTypeID;
		sqlite3_bind_blob(pStmt, 29, &moveData, (int)sizeof(moveData), SQLITE_TRANSIENT);

		// ---- BLOB: NPC発生情報 ----
		struct {
			DWORD  dwPutCycle;
			int    nPutMoveType;
			int    nMaxPutCount;
			int    nPutAverage;
			int    nPutAreaX;
			int    nPutAreaY;
		} putNpcData;
		putNpcData.dwPutCycle    = pInfo->m_dwPutCycle;
		putNpcData.nPutMoveType  = pInfo->m_nPutMoveType;
		putNpcData.nMaxPutCount  = pInfo->m_nMaxPutCount;
		putNpcData.nPutAverage   = pInfo->m_nPutAverage;
		putNpcData.nPutAreaX     = pInfo->m_ptPutArea.x;
		putNpcData.nPutAreaY     = pInfo->m_ptPutArea.y;
		sqlite3_bind_blob(pStmt, 30, &putNpcData, (int)sizeof(putNpcData), SQLITE_TRANSIENT);

		// ---- BLOB: 所持アイテムID配列 (DWORD 可変長) ----
		int nItemCount = (int)pInfo->m_adwItemID.size();
		if (nItemCount > 0) {
			sqlite3_bind_blob(pStmt, 31,
				pInfo->m_adwItemID.data(),
				nItemCount * (int)sizeof(DWORD),
				SQLITE_TRANSIENT);
		} else {
			sqlite3_bind_null(pStmt, 31);
		}

		// ---- BLOB: 所持スキルID配列 (DWORD 可変長) ----
		int nSkillCount = (int)pInfo->m_adwSkillID.size();
		if (nSkillCount > 0) {
			sqlite3_bind_blob(pStmt, 32,
				pInfo->m_adwSkillID.data(),
				nSkillCount * (int)sizeof(DWORD),
				SQLITE_TRANSIENT);
		} else {
			sqlite3_bind_null(pStmt, 32);
		}

		sqlite3_step(pStmt);
		sqlite3_reset(pStmt);
	}

	sqlite3_finalize(pStmt);
}

// ============================================================
// 内部ヘルパー: 正規化テーブルから読み込む（1行以上読めたら TRUE）
// ============================================================
BOOL CSaveLoadInfoChar::LoadFromNormalTable(PCLibInfoBase pDst)
{
	if (s_pDb == NULL || pDst == NULL) return FALSE;

	// メインテーブル SELECT
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
	int nRet = sqlite3_prepare_v2(s_pDb, pszSelect, -1, &pStmt, NULL);
	if (nRet != SQLITE_OK) return FALSE;

	// pDst は実体が CLibInfoCharBase (CLibInfoCharSvr 等) なので GetNew(int) を呼ぶためキャスト
	CLibInfoCharBase* pLibChar = (CLibInfoCharBase*)pDst;

	int nRowCount = 0;
	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		// MoveType で正しい派生クラスのインスタンスを生成
		int nMoveType = sqlite3_column_int(pStmt, 6);
		PCInfoCharBase pInfo = (PCInfoCharBase)pLibChar->GetNew(nMoveType);
		if (pInfo == NULL) continue;

		// ---- 個別カラムの読み込み ----
		// m_dwCharID を事前にセットしてから Add() を呼ぶ
		//   ← Add() 内で m_dwCharID == 0 なら GetNewID() で上書きされてしまうため
		pInfo->m_dwCharID             = (DWORD)sqlite3_column_int(pStmt,  0);
		pInfo->m_dwMapID              = (DWORD)sqlite3_column_int(pStmt,  1);
		pInfo->m_nMapX                = sqlite3_column_int(pStmt,  2);
		pInfo->m_nMapY                = sqlite3_column_int(pStmt,  3);
		pInfo->m_nDirection           = sqlite3_column_int(pStmt,  4);
		pInfo->m_nMoveState           = sqlite3_column_int(pStmt,  5);
		pInfo->m_nMoveType            = sqlite3_column_int(pStmt,  6);
		pInfo->m_dwHP                 = (DWORD)sqlite3_column_int(pStmt,  7);
		pInfo->m_dwMaxHP              = (DWORD)sqlite3_column_int(pStmt,  8);
		pInfo->m_dwSP                 = (DWORD)sqlite3_column_int(pStmt,  9);
		pInfo->m_dwMaxSP              = (DWORD)sqlite3_column_int(pStmt, 10);
		pInfo->m_dwExp                = (DWORD)sqlite3_column_int(pStmt, 11);
		pInfo->m_wLevel               = (WORD)sqlite3_column_int(pStmt,  12);
		pInfo->m_dwEquipItemIDCloth   = (DWORD)sqlite3_column_int(pStmt, 13);
		pInfo->m_dwEquipItemIDAcce1   = (DWORD)sqlite3_column_int(pStmt, 14);
		pInfo->m_dwEquipItemIDAcce2   = (DWORD)sqlite3_column_int(pStmt, 15);
		pInfo->m_dwEquipItemIDArmsRight = (DWORD)sqlite3_column_int(pStmt, 16);
		pInfo->m_dwEquipItemIDArmsLeft  = (DWORD)sqlite3_column_int(pStmt, 17);
		pInfo->m_dwEquipItemIDHead    = (DWORD)sqlite3_column_int(pStmt, 18);
		pInfo->m_bBlock               = (BOOL)sqlite3_column_int(pStmt,  19);
		pInfo->m_bPush                = (BOOL)sqlite3_column_int(pStmt,  20);
		pInfo->m_bNPC                 = (BOOL)sqlite3_column_int(pStmt,  21);
		pInfo->m_nSex                 = sqlite3_column_int(pStmt,  22);
		pInfo->m_wFamilyID            = (WORD)sqlite3_column_int(pStmt,  23);
		pInfo->m_nGrpSize             = sqlite3_column_int(pStmt,  24);

		// キャラ名: SJIS バイト列として格納されているので LPCSTR で取得し TCHAR 変換
		const char* pszName = (const char*)sqlite3_column_text(pStmt, 25);
		if (pszName != NULL) pInfo->m_strCharName = (LPCTSTR)LegacyAnsiToTString(pszName);

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
				pInfo->m_wStamina          = pw[0];
				pInfo->m_wPower            = pw[1];
				pInfo->m_wStrength         = pw[2];
				pInfo->m_wMagic            = pw[3];
				pInfo->m_wSkillful         = pw[4];
				pInfo->m_wAbillityAT       = pw[5];
				pInfo->m_wAbillityDF       = pw[6];
				pInfo->m_wPAtack           = pw[7];
				pInfo->m_wPDefense         = pw[8];
				pInfo->m_wPMagic           = pw[9];
				pInfo->m_wPMagicDefense    = pw[10];
				pInfo->m_wPHitAverage      = pw[11];
				pInfo->m_wPAvoidAverage    = pw[12];
				pInfo->m_wPCriticalAverage = pw[13];
				pInfo->m_wAttrFire         = pw[14];
				pInfo->m_wAttrWind         = pw[15];
				pInfo->m_wAttrWater        = pw[16];
				pInfo->m_wAttrEarth        = pw[17];
				pInfo->m_wAttrLight        = pw[18];
				pInfo->m_wAttrDark         = pw[19];
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
				pInfo->m_nMaxItemCount      = moveData.nMaxItemCount;
				pInfo->m_nDropItemAverage   = moveData.nDropItemAverage;
				pInfo->m_nMoveAverage       = moveData.nMoveAverage;
				pInfo->m_nMoveAverageBattle = moveData.nMoveAverageBattle;
				pInfo->m_dwMoveWait         = moveData.dwMoveWait;
				pInfo->m_dwMoveWaitBattle   = moveData.dwMoveWaitBattle;
				pInfo->m_sizeSearchDistance.cx = (LONG)moveData.nSearchDistanceCX;
				pInfo->m_sizeSearchDistance.cy = (LONG)moveData.nSearchDistanceCY;
				pInfo->m_dwMotionTypeID     = moveData.dwMotionTypeID;
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
				for (int j = 0; j < nItemCount; j++) {
					pInfo->m_adwItemID.push_back(pdw[j]);
				}
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
				for (int j = 0; j < nSkillCount; j++) {
					pInfo->m_adwSkillID.push_back(pdw[j]);
				}
			}
		}

		// ライブラリに追加（m_dwCharID が 0 でないので GetNewID() は呼ばれない）
		pDst->Add(pInfo);
		nRowCount++;
	}

	sqlite3_finalize(pStmt);

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
		const char* pszDelSql =
			"DELETE FROM sbo_data WHERE name='Char';";
		sqlite3_exec(s_pDb, pszDelSql, NULL, NULL, NULL);
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

	// テーブルが無ければ作成
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
