/// @file SaveLoadInfoMapObject.h
/// @brief データ保存・読み込みクラス(マップオブジェクト情報) 定義ファイル
///        MapObject ライブラリを SQLite 正規化テーブル (sys_map_object) に移行
///        サブ配列 m_aInfoAnime → sys_map_object_anime (ObjectID, Slot, Wait, Level, GrpIDData BLOB)
///        当たり判定データ m_pHit → HitData BLOB (cx*cy バイト、可変サイズのため折衷 BLOB)
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/01
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoMapObject : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMapObject();	// コンストラクタ
	virtual ~CSaveLoadInfoMapObject();	// デストラクタ

	// Save/Load を正規化テーブル経路でオーバーライド
	virtual void	Save(PCLibInfoBase pSrc) override;	// 正規化テーブルに書き込み
	virtual void	Load(PCLibInfoBase pDst) override;	// 正規化テーブルから読み込み

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	void	EnsureTable(void);
	// メモリ上のマップオブジェクト情報を正規化テーブルに全行置換
	void	SaveToNormalTable(void);
	// 正規化テーブルからマップオブジェクト情報を読み込む（1行以上読めたら TRUE）
	BOOL	LoadFromNormalTable(PCLibInfoBase pDst);
	// BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
	BOOL	MigrateFromBlob(PCLibInfoBase pDst);
} CSaveLoadInfoMapObject, *PCSaveLoadInfoMapObject;
