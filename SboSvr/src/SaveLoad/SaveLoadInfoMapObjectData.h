/// @file SaveLoadInfoMapObjectData.h
/// @brief データ保存・読み込みクラス(マップオブジェクト配置データ) 定義ファイル
///        各 CInfoMapBase の m_pLibInfoMapObjectData を SQLite 正規化テーブル
///        (sys_map_objectdata) に保存・復元する。
///        トップレベルの Lib ではなく per-map データのため CSaveLoadInfoBase は継承しない。
/// @author 年がら年中春うらら(URARA-works)
/// @date 2026/06/02
/// @copyright Copyright(C)URARA-works 2026

#pragma once

class CLibInfoMapBase;

// ============================================================
// CSaveLoadInfoMapObjectData
//   per-map オブジェクト配置データを sys_map_objectdata テーブルに永続化するクラス。
//   CSaveLoadInfoBase は継承せず、s_pDb だけ参照する独立クラス。
// ============================================================

typedef class CSaveLoadInfoMapObjectData
{
public:
			CSaveLoadInfoMapObjectData();	// コンストラクタ
	virtual ~CSaveLoadInfoMapObjectData();	// デストラクタ

	void	Save(CLibInfoMapBase *pLibInfoMap);	// 全マップの配置データを sys_map_objectdata に書き込み
	void	Load(CLibInfoMapBase *pLibInfoMap);	// sys_map_objectdata を読み、各マップの m_pLibInfoMapObjectData に投入

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	void	EnsureTable(void);
	// メモリ上の全マップ配置データを sys_map_objectdata に全行置換（DELETE + INSERT）
	void	SaveAllObjects(CLibInfoMapBase *pLibInfoMap);
	// sys_map_objectdata から全行読み込んで各マップの m_pLibInfoMapObjectData に復元（1行以上読めたら TRUE）
	BOOL	LoadAllObjects(CLibInfoMapBase *pLibInfoMap);
	// 配置データを sys_map_objectdata へ書き戻す（一回限りのマイグレーション用）
	void	MigrateData(CLibInfoMapBase *pLibInfoMap);
} CSaveLoadInfoMapObjectData, *PCSaveLoadInfoMapObjectData;
