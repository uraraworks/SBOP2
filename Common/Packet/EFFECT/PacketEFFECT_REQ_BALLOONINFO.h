/// @file PacketEFFECT_REQ_BALLOONINFO.h
/// @brief コマンド(エフェクト系:噴出し情報要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/12/24
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketEFFECT_REQ_BALLOONINFO : public CPacketBase
{
public:
			CPacketEFFECT_REQ_BALLOONINFO();	// コンストラクタ
	virtual ~CPacketEFFECT_REQ_BALLOONINFO();	// デストラクタ

	void	Make(DWORD dwBalloonID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD m_dwBalloonID;	// 噴出しID
} CPacketEFFECT_REQ_BALLOONINFO, *PCPacketEFFECT_REQ_BALLOONINFO;
