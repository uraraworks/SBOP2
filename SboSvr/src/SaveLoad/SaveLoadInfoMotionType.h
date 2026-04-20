/// @file SaveLoadInfoMotionType.h
/// @brief データ保存・読み込みクラス(モーション種別情報) 定義ファイル
///        MotionType ライブラリを SQLite 正規化テーブル (sys_motion_type) に移行
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/05/31
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoMotionType : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMotionType();	// コンストラクタ
	virtual ~CSaveLoadInfoMotionType();	// デストラクタ

	// Save/Load を正規化テーブル経路でオーバーライド
	virtual void	Save(PCLibInfoBase pSrc) override;	// 正規化テーブルに書き込み
	virtual void	Load(PCLibInfoBase pDst) override;	// 正規化テーブルから読み込み

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	void	EnsureTable(void);
	// メモリ上のモーション種別情報を正規化テーブルに全行置換
	void	SaveToNormalTable(void);
	// 正規化テーブルからモーション種別情報を読み込む（1行以上読めたら TRUE）
	BOOL	LoadFromNormalTable(PCLibInfoBase pDst);
	// BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
	BOOL	MigrateFromBlob(PCLibInfoBase pDst);
} CSaveLoadInfoMotionType, *PCSaveLoadInfoMotionType;
