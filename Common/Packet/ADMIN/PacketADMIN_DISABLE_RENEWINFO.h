/// @file PacketADMIN_DISABLE_RENEWINFO.h
/// @brief コマンド(管理者系:拒否情報の更新) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/11
/// @copyright Copyright(C)URARA-works 2009

#pragma once

#include "PacketBase.h"

class CInfoDisable;
class CLibInfoDisable;

// クラス宣言

typedef class CPacketADMIN_DISABLE_RENEWINFO : public CPacketBase
{
public:
			CPacketADMIN_DISABLE_RENEWINFO();	// コンストラクタ
	virtual ~CPacketADMIN_DISABLE_RENEWINFO();	// デストラクタ

	void	Make(CLibInfoDisable *pLibInfoDisable);	// パケットを作成
	void	Make(CInfoDisable *pInfoDisable);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CLibInfoDisable	*m_pLibInfoDisable;	// 拒否情報
} CPacketADMIN_DISABLE_RENEWINFO, *PCPacketADMIN_DISABLE_RENEWINFO;
