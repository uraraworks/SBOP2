/// @file PacketMAP_REQ_MAPINFO.h
/// @brief コマンド(マップ系:マップ情報要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/04
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketMAP_REQ_MAPINFO : public CPacketBase
{
public:
			CPacketMAP_REQ_MAPINFO();	// コンストラクタ
	virtual ~CPacketMAP_REQ_MAPINFO();	// デストラクタ

	void	Make(DWORD dwMapID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD m_dwMapID;	// マップID
} CPacketMAP_REQ_MAPINFO, *PCPacketMAP_REQ_MAPINFO;
