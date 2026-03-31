/// @file PacketMAP_RES_MAPINFO.h
/// @brief コマンド(マップ系:マップ情報応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/04
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

class CInfoMapBase;

// クラス宣言

typedef class CPacketMAP_RES_MAPINFO : public CPacketBase
{
public:
			CPacketMAP_RES_MAPINFO();	// コンストラクタ
	virtual ~CPacketMAP_RES_MAPINFO();	// デストラクタ

	void	Make(CInfoMapBase *pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CInfoMapBase	*m_pInfo;	// マップ情報
} CPacketMAP_RES_MAPINFO, *PCPacketMAP_RES_MAPINFO;
