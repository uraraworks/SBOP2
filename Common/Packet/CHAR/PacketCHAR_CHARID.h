/// @file PacketCHAR_CHARID.h
/// @brief コマンド(キャラ系:キャラID通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/02/12
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCHAR_CHARID : public CPacketBase
{
public:
			CPacketCHAR_CHARID();	// コンストラクタ
	virtual ~CPacketCHAR_CHARID();	// デストラクタ

	void	Make(PARRAYDWORD paSrc);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	ARRAYDWORD m_adwCharID;	// キャラID
} CPacketCHAR_CHARID, *PCPacketCHAR_CHARID;
