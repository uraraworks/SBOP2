/// @file PacketADMIN_SERVER_SAVEINFO.h
/// @brief コマンド(管理者系:サーバー情報保存) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/08
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_SERVER_SAVEINFO : public CPacketBase
{
public:
			CPacketADMIN_SERVER_SAVEINFO();	// コンストラクタ
	virtual ~CPacketADMIN_SERVER_SAVEINFO();	// デストラクタ

	void	Make(void);	// パケットを作成
} CPacketADMIN_SERVER_SAVEINFO, *PCPacketADMIN_SERVER_SAVEINFO;
