/// @file PacketADMIN_MAP_SELECTPICK.h
/// @brief コマンド(管理者系:マップ選択ピック) 定義ファイル

#pragma once

#include "PacketBase.h"

// 選択対象の種別
enum {
	SELECTPICK_TYPE_MAPCELL   = 0,	// マップセル
	SELECTPICK_TYPE_PLACEMENT = 1,	// 配置オブジェクト
	SELECTPICK_TYPE_EVENT     = 2,	// イベント
	SELECTPICK_TYPE_CHAR      = 3,	// キャラクター
};

// クラス宣言

typedef class CPacketADMIN_MAP_SELECTPICK : public CPacketBase
{
public:
			CPacketADMIN_MAP_SELECTPICK();	// コンストラクタ
	virtual ~CPacketADMIN_MAP_SELECTPICK();	// デストラクタ

	void	Make(BYTE byType, DWORD dwMapID, WORD wX, WORD wY, DWORD dwRefID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	BYTE	m_byType;	// 選択種別（SELECTPICK_TYPE_*）
	DWORD	m_dwMapID;	// マップID
	WORD	m_wX;		// タイルX座標
	WORD	m_wY;		// タイルY座標
	DWORD	m_dwRefID;	// 参照ID（未使用時 0）
} CPacketADMIN_MAP_SELECTPICK, *PCPacketADMIN_MAP_SELECTPICK;
