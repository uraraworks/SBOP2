/// @file PacketADMIN_REQ_PLAYSOUND.h
/// @brief コマンド(管理者系:効果音の再生要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/02
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_REQ_PLAYSOUND : public CPacketBase
{
public:
			CPacketADMIN_REQ_PLAYSOUND();	// コンストラクタ
	virtual ~CPacketADMIN_REQ_PLAYSOUND();	// デストラクタ

	void	Make(DWORD dwSoundID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD m_dwSoundID;	// 効果音ID
} CPacketADMIN_REQ_PLAYSOUND, *PCPacketADMIN_REQ_PLAYSOUND;
