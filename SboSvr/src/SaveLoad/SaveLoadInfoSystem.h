/// @file SaveLoadInfoSystem.h
/// @brief データ保存・読み込みクラス(システム情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/09/28
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoSystem : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoSystem();	// コンストラクタ
	virtual ~CSaveLoadInfoSystem();	// デストラクタ

	// Save/Load を正規化テーブル経路でオーバーライド
	virtual void	Save(PCLibInfoBase pSrc) override;	// 正規化テーブルに書き込み
	virtual void	Load(PCLibInfoBase pDst) override;	// 正規化テーブルから読み込み

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	void	EnsureTable(void);
	// メモリ上の STSYSTEM_INITCHARSTATUS を正規化テーブルに INSERT OR REPLACE
	void	SaveToNormalTable(void);
	// 正規化テーブルから STSYSTEM_INITCHARSTATUS を読み込む（成功時 TRUE）
	BOOL	LoadFromNormalTable(PCLibInfoBase pDst);
	// BLOB 経路（sbo_data）から読み込んでマイグレーション
	BOOL	MigrateFromBlob(PCLibInfoBase pDst);
} CSaveLoadInfoSystem, *PCSaveLoadInfoSystem;
