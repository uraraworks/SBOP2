/// @file SaveLoadInfoDisable.h
/// @brief データ保存・読み込みクラス(拒否情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/11
/// @copyright Copyright(C)URARA-works 2009

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoDisable : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoDisable();	// コンストラクタ
	virtual ~CSaveLoadInfoDisable();	// デストラクタ

	// Save/Load を正規化テーブル経路でオーバーライド
	virtual void	Save(PCLibInfoBase pSrc) override;	// 正規化テーブルに書き込み
	virtual void	Load(PCLibInfoBase pDst) override;	// 正規化テーブルから読み込み

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	void	EnsureTable(void);
	// メモリ上の拒否情報を正規化テーブルに全行置換
	void	SaveToNormalTable(void);
	// 正規化テーブルから拒否情報を読み込む（1行以上読めたら TRUE）
	BOOL	LoadFromNormalTable(PCLibInfoBase pDst);
	// BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
	BOOL	MigrateFromBlob(PCLibInfoBase pDst);
} CSaveLoadInfoDisable, *PCSaveLoadInfoDisable;
