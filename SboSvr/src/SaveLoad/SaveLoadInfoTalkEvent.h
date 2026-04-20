/// @file SaveLoadInfoTalkEvent.h
/// @brief データ保存・読み込みクラス(会話イベント情報) 定義ファイル
///        TalkEvent ライブラリを SQLite 正規化テーブル (sys_talk_event) に移行
///        イベント配列 m_apTalkEvent → sys_talk_event_event (TalkEventID, Slot, ...)
///        MENU 選択肢配列 m_aMenuInfo → sys_talk_event_menu (TalkEventID, Slot, ItemNo, ...)
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/27
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoTalkEvent : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoTalkEvent();	// コンストラクタ
	virtual ~CSaveLoadInfoTalkEvent();	// デストラクタ

	// Save/Load を正規化テーブル経路でオーバーライド
	virtual void	Save(PCLibInfoBase pSrc) override;	// 正規化テーブルに書き込み
	virtual void	Load(PCLibInfoBase pDst) override;	// 正規化テーブルから読み込み

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	void	EnsureTable(void);
	// メモリ上の会話イベント情報を正規化テーブルに全行置換
	void	SaveToNormalTable(void);
	// 正規化テーブルから会話イベント情報を読み込む（1行以上読めたら TRUE）
	BOOL	LoadFromNormalTable(PCLibInfoBase pDst);
	// BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
	BOOL	MigrateFromBlob(PCLibInfoBase pDst);
} CSaveLoadInfoTalkEvent, *PCSaveLoadInfoTalkEvent;
