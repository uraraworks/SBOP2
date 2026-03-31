/// @file PacketCHAR_REQ_CHARINFO2.h
/// @brief コマンド(キャラ系:キャラ情報要求(複数)) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/02/13
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCHAR_REQ_CHARINFO2 : public CPacketBase
{
public:
			CPacketCHAR_REQ_CHARINFO2();	// コンストラクタ
	virtual ~CPacketCHAR_REQ_CHARINFO2();	// デストラクタ

	void	Make(PARRAYDWORD paSrc);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	ARRAYDWORD m_adwCharID;	// キャラID
} CPacketCHAR_REQ_CHARINFO2, *PCPacketCHAR_REQ_CHARINFO2;
