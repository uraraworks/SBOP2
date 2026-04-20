/// @file SaveLoadInfoChar.h
/// @brief データ保存・読み込みクラス(キャラ情報) 定義ファイル
///        Char ライブラリを SQLite 正規化テーブル (sys_char) に移行
///
///  テーブル構成:
///    sys_char: キャラ基本情報 + 各種 BLOB カラム
///
///  方針:
///    スカラ識別子 (CharID/MapID/HP/SP/Level/装備アイテムID等) は個別カラムに展開。
///    外見 GrpID 群・ステータス能力値・NPC 発生情報・所持アイテム配列・所持スキル配列は
///    フィールド数が多いため BLOB 格納（折衷 BLOB 方針）。
///    派生クラス (CInfoCharSvr, CInfoCharBATTLE1Svr 等) は全て CInfoCharBase として
///    m_nMoveType カラムで識別する。読み込み時は GetNew(m_nMoveType) で正しい型を生成する。
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
	void	EnsureTable(void);
	// メモリ上のキャラ情報を正規化テーブルに全行置換
	void	SaveToNormalTable(void);
	// 正規化テーブルからキャラ情報を読み込む（1行以上読めたら TRUE）
	BOOL	LoadFromNormalTable(PCLibInfoBase pDst);
	// BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
	BOOL	MigrateFromBlob(PCLibInfoBase pDst);
} CSaveLoadInfoChar, *PCSaveLoadInfoChar;
