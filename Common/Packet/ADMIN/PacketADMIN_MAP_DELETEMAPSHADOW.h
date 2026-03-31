/// @file PacketADMIN_MAP_DELETEMAPSHADOW.h
/// @brief コマンド(管理者系:マップマップ影削除) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/07
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_MAP_DELETEMAPSHADOW : public CPacketBase
{
public:
			CPacketADMIN_MAP_DELETEMAPSHADOW();	// コンストラクタ
	virtual ~CPacketADMIN_MAP_DELETEMAPSHADOW();	// デストラクタ

	void	Make(DWORD dwShadowID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwShadowID;	// マップ影ID
} CPacketADMIN_MAP_DELETEMAPSHADOW, *PCPacketADMIN_MAP_DELETEMAPSHADOW;
