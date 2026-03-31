/// @file PacketMAP_DELETEMAPSHADOW.h
/// @brief コマンド(マップ系:マップ影削除) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/08
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketMAP_DELETEMAPSHADOW : public CPacketBase
{
public:
			CPacketMAP_DELETEMAPSHADOW();	// コンストラクタ
	virtual ~CPacketMAP_DELETEMAPSHADOW();	// デストラクタ

	void	Make(DWORD dwShadowID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwShadowID;	// マップ影ID
} CPacketMAP_DELETEMAPSHADOW, *PCPacketMAP_DELETEMAPSHADOW;
