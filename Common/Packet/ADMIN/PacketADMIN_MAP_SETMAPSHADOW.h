/// @file PacketADMIN_MAP_SETMAPSHADOW.h
/// @brief コマンド(管理者系:マップ影配置) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/07
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_MAP_SETMAPSHADOW : public CPacketBase
{
public:
			CPacketADMIN_MAP_SETMAPSHADOW();	// コンストラクタ
	virtual ~CPacketADMIN_MAP_SETMAPSHADOW();	// デストラクタ

	void	Make(DWORD dwMapID, int x, int y, DWORD dwShadowID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	POINT	m_ptPos;	// 座標
	DWORD	m_dwMapID,	// マップID
			m_dwShadowID;	// マップ影ID
} CPacketADMIN_MAP_SETMAPSHADOW, *PCPacketADMIN_MAP_SETMAPSHADOW;
