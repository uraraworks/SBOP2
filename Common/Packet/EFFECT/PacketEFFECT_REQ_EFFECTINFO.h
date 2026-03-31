/// @file PacketEFFECT_REQ_EFFECTINFO.h
/// @brief コマンド(エフェクト系:エフェクト情報要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketEFFECT_REQ_EFFECTINFO : public CPacketBase
{
public:
			CPacketEFFECT_REQ_EFFECTINFO();	// コンストラクタ
	virtual ~CPacketEFFECT_REQ_EFFECTINFO();	// デストラクタ

	void	Make(DWORD dwEffect);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD m_dwEffectID;	// エフェクトID
} CPacketEFFECT_REQ_EFFECTINFO, *PCPacketEFFECT_REQ_EFFECTINFO;
