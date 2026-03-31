/// @file PacketADMIN_RENEWMAPPARTS.h
/// @brief コマンド(管理者系:マップパーツ更新) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/29
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

class CInfoMapParts;

// クラス宣言

typedef class CPacketADMIN_RENEWMAPPARTS : public CPacketBase
{
public:
			CPacketADMIN_RENEWMAPPARTS();	// コンストラクタ
	virtual ~CPacketADMIN_RENEWMAPPARTS();	// デストラクタ

	void	Make(CInfoMapParts *pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CInfoMapParts	*m_pInfoMapParts;	// マップパーツ情報
} CPacketADMIN_RENEWMAPPARTS, *PCPacketADMIN_RENEWMAPPARTS;
