/// @file PacketADMIN_CHAR_RENEWSTATUS.h
/// @brief コマンド(管理者系:ステータス情報更新) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/12
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketCHAR_STATUS.h"

class CInfoCharBase;

// クラス宣言

typedef class CPacketADMIN_CHAR_RENEWSTATUS : public CPacketCHAR_STATUS
{
public:
			CPacketADMIN_CHAR_RENEWSTATUS();	// コンストラクタ
	virtual ~CPacketADMIN_CHAR_RENEWSTATUS();	// デストラクタ
} CPacketADMIN_CHAR_RENEWSTATUS, *PCPacketADMIN_CHAR_RENEWSTATUS;
