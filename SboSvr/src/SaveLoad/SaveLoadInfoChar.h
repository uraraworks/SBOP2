/// @file SaveLoadInfoChar.h
/// @brief データ保存・読み込みクラス(キャラ情報) 定義ファイル
///        Char ライブラリを SQLite 完全正規化テーブルに移行
///
///  テーブル構成:
///    sys_char      — キャラ基本情報 (全スカラを個別カラム)
///    sys_char_item — 所持アイテムID サブテーブル (CharID, Slot, ItemID)
///    sys_char_skill— 所持スキルID  サブテーブル (CharID, Slot, SkillID)
///
///  旧 BLOB カラム (GrpIDData / StatusData / MoveData / PutNpcData /
///                  ItemData / SkillData) は廃止し、全フィールドを
///                  個別カラム / サブテーブルに展開する。
///
///  旧スキーマ検出時は全行をメモリに復元し DROP TABLE → 新 CREATE TABLE
///  → 再書き込みするマイグレーションを EnsureTable() 内で自動実行する。
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoChar : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoChar();	// コンストラクタ
	virtual ~CSaveLoadInfoChar();	// デストラクタ

	// Save/Load を正規化テーブル経路でオーバーライド
	virtual void	Save(PCLibInfoBase pSrc) override;	// 正規化テーブルに書き込み
	virtual void	Load(PCLibInfoBase pDst) override;	// 正規化テーブルから読み込み

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	// 旧 BLOB スキーマを検出した場合は自動マイグレーションする
	void	EnsureTable(void);
	// メモリ上のキャラ情報を正規化テーブルに全行置換
	void	SaveToNormalTable(void);
	// 正規化テーブルからキャラ情報を読み込む（1行以上読めたら TRUE）
	BOOL	LoadFromNormalTable(PCLibInfoBase pDst);
	// BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
	BOOL	MigrateFromBlob(PCLibInfoBase pDst);
} CSaveLoadInfoChar, *PCSaveLoadInfoChar;
