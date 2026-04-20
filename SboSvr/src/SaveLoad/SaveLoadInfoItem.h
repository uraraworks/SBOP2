/// @file SaveLoadInfoItem.h
/// @brief データ保存・読み込みクラス(アイテム情報) 定義ファイル
///        Item ライブラリを SQLite 正規化テーブル (sys_item) に移行
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/19
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoItem : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoItem();	// コンストラクタ
	virtual ~CSaveLoadInfoItem();	// デストラクタ

	// Save/Load を正規化テーブル経路でオーバーライド
	virtual void	Save(PCLibInfoBase pSrc) override;	// 正規化テーブルに書き込み
	virtual void	Load(PCLibInfoBase pDst) override;	// 正規化テーブルから読み込み

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	void	EnsureTable(void);
	// メモリ上のアイテム情報を正規化テーブルに全行置換
	void	SaveToNormalTable(void);
	// 正規化テーブルからアイテム情報を読み込む（1行以上読めたら TRUE）
	BOOL	LoadFromNormalTable(PCLibInfoBase pDst);
	// BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
	BOOL	MigrateFromBlob(PCLibInfoBase pDst);
} CSaveLoadInfoItem, *PCSaveLoadInfoItem;
