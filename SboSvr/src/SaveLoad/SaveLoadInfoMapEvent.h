/// @file SaveLoadInfoMapEvent.h
/// @brief データ保存・読み込みクラス(マップイベント情報) 定義ファイル
///        各 CInfoMapBase の m_pLibInfoMapEvent を SQLite 正規化テーブル
///        (sys_map_event) に保存・復元する。
///        トップレベルの Lib ではなく per-map データのため CSaveLoadInfoBase は継承しない。
/// @author 年がら年中春うらら(URARA-works)
/// @date 2026/04/25
/// @copyright Copyright(C)URARA-works 2026

#pragma once

class CLibInfoMapBase;

// ============================================================
// CSaveLoadInfoMapEvent
//   per-map イベント情報を sys_map_event テーブルに永続化するクラス。
//   CSaveLoadInfoBase は継承せず、s_pDb だけ参照する独立クラス。
// ============================================================

typedef class CSaveLoadInfoMapEvent
{
public:
			CSaveLoadInfoMapEvent();	// コンストラクタ
	virtual ~CSaveLoadInfoMapEvent();	// デストラクタ

	void	Save(CLibInfoMapBase *pLibInfoMap);	// 全マップのイベントを sys_map_event に書き込み
	void	Load(CLibInfoMapBase *pLibInfoMap);	// sys_map_event を読み、各マップの m_pLibInfoMapEvent に投入

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	void	EnsureTable(void);
	// メモリ上の全マップイベントを sys_map_event に全行置換（DELETE + INSERT）
	void	SaveAllEvents(CLibInfoMapBase *pLibInfoMap);
	// sys_map_event から全行読み込んで各マップの m_pLibInfoMapEvent に復元（1行以上読めたら TRUE）
	BOOL	LoadAllEvents(CLibInfoMapBase *pLibInfoMap);
	// メモリ上のイベントデータを sys_map_event へ書き戻す（一回限りのマイグレーション用）
	void	MigrateFromMemory(CLibInfoMapBase *pLibInfoMap);
} CSaveLoadInfoMapEvent, *PCSaveLoadInfoMapEvent;
