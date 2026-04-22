/// @file SaveLoadInfoMapShadow.h
/// @brief データ保存・読み込みクラス(マップ影情報) 定義ファイル
///        MapShadow ライブラリを SQLite 正規化テーブル
///        (sys_map_shadow / sys_map_shadow_anime) に移行
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/06
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoMapShadow : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMapShadow();	// コンストラクタ
	virtual ~CSaveLoadInfoMapShadow();	// デストラクタ

	// Save/Load を正規化テーブル経路でオーバーライド
	virtual void	Save(PCLibInfoBase pSrc) override;	// 正規化テーブルに書き込み
	virtual void	Load(PCLibInfoBase pDst) override;	// 正規化テーブルから読み込み
	virtual void	SetHeaderInfo(PCInfoBase pInfo) override;	// BLOB/.dat 用ヘッダ情報を設定

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	void	EnsureTable(void);
	// メモリ上のマップ影情報を正規化テーブルに全行置換
	void	SaveToNormalTable(void);
	// 正規化テーブルからマップ影情報を読み込む（1行以上読めたら TRUE）
	BOOL	LoadFromNormalTable(PCLibInfoBase pDst);
	// BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
	BOOL	MigrateFromBlob(PCLibInfoBase pDst);
	// SQLite 化直後の壊れたアニメ行（全 grpID=0）を検出
	BOOL	HasBrokenAnimeRows(void);
} CSaveLoadInfoMapShadow, *PCSaveLoadInfoMapShadow;
