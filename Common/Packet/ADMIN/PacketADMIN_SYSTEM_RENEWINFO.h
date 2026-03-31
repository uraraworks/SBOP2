/// @file PacketADMIN_SYSTEM_RENEWINFO.h
/// @brief コマンド(管理者系:システム情報の更新) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/04
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

class CInfoSystem;

// クラス宣言

typedef class CPacketADMIN_SYSTEM_RENEWINFO : public CPacketBase
{
public:
			CPacketADMIN_SYSTEM_RENEWINFO();	// コンストラクタ
	virtual ~CPacketADMIN_SYSTEM_RENEWINFO();	// デストラクタ

	void	Make(CInfoSystem *pInfoSystem);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CInfoSystem	*m_pInfoSystem;	// システム情報
} CPacketADMIN_SYSTEM_RENEWINFO, *PCPacketADMIN_SYSTEM_RENEWINFO;
