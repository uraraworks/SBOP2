/// @file PacketADMIN_DISABLE_RES_INFO.h
/// @brief コマンド(管理者系:拒否情報応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/11
/// @copyright Copyright(C)URARA-works 2009

#pragma once

#include "PacketBase.h"

class CLibInfoDisable;

// クラス宣言

typedef class CPacketADMIN_DISABLE_RES_INFO : public CPacketBase
{
public:
			CPacketADMIN_DISABLE_RES_INFO();	// コンストラクタ
	virtual ~CPacketADMIN_DISABLE_RES_INFO();	// デストラクタ

	void	Make(CLibInfoDisable *pLibInfoDisable);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CLibInfoDisable	*m_pLibInfoDisable;	// 拒否情報
} CPacketADMIN_DISABLE_RES_INFO, *PCPacketADMIN_DISABLE_RES_INFO;
