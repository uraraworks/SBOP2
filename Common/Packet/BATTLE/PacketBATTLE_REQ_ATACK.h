/// @file PacketBATTLE_REQ_ATACK.h
/// @brief コマンド(戦闘系:攻撃要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketBATTLE_REQ_ATACK : public CPacketBase
{
public:
			CPacketBATTLE_REQ_ATACK();	// コンストラクタ
	virtual ~CPacketBATTLE_REQ_ATACK();	// デストラクタ

	void	Make(DWORD dwCharID, DWORD dwTargetCharID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID,	// キャラID
			m_dwTargetCharID;	// 攻撃先キャラID
} CPacketBATTLE_REQ_ATACK, *PCPacketBATTLE_REQ_ATACK;
