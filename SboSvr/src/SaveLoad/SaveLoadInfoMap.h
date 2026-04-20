/// @file SaveLoadInfoMap.h
/// @brief データ保存・読み込みクラス(マップ情報) 定義ファイル
///        Map ライブラリを SQLite 正規化テーブル (sys_map) に移行
///
///  テーブル構成:
///    sys_map ─ マップ基本情報
///              スカラー: MapID(PK), MapCX, MapCY, BGMID, WeatherType,
///                        EnableBattle, Recovery, Level, MapName(TEXT)
///              折衷 BLOB: MapData(m_pwMap), MapPileData(m_pwMapPile),
///                         MapShadowData(m_pwMapShadow)
///                         ※ いずれも cx*cy 個の WORD のバイト列
///
///  マップイベント / オブジェクト配置データは
///  それぞれの SaveLoad クラスが個別に保存するため本クラスでは扱わない。
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/11/08
/// @copyright Copyright(C)URARA-works 2005

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoMap : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMap();	// コンストラクタ
	virtual ~CSaveLoadInfoMap();	// デストラクタ

	// Save/Load を正規化テーブル経路でオーバーライド
	virtual void	Save(PCLibInfoBase pSrc) override;	// 正規化テーブルに書き込み
	virtual void	Load(PCLibInfoBase pDst) override;	// 正規化テーブルから読み込み

private:
	// 正規化テーブルの CREATE TABLE を実行（なければ作成）
	void	EnsureTable(void);
	// メモリ上のマップ情報を正規化テーブルに全行置換
	void	SaveToNormalTable(void);
	// 正規化テーブルからマップ情報を読み込む（1行以上読めたら TRUE）
	BOOL	LoadFromNormalTable(PCLibInfoBase pDst);
	// BLOB 経路（sbo_data / .dat）から読み込んでマイグレーション
	BOOL	MigrateFromBlob(PCLibInfoBase pDst);
} CSaveLoadInfoMap, *PCSaveLoadInfoMap;
