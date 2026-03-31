/// @file PacketEFFECT_EFFECTINFO.h
/// @brief コマンド(エフェクト系:エフェクト情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"
#include "LibInfo/LibInfoEffect.h"

// クラス宣言

typedef class CPacketEFFECT_EFFECTINFO : public CPacketBase
{
public:
			CPacketEFFECT_EFFECTINFO();	// コンストラクタ
	virtual ~CPacketEFFECT_EFFECTINFO();	// デストラクタ

	void	Make(PCLibInfoEffect pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	PCLibInfoEffect	m_pLibInfo;	// エフェクト情報
} CPacketEFFECT_EFFECTINFO, *PCPacketEFFECT_EFFECTINFO;
