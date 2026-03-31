/// @file PacketITEM_RENEWITEMINFO.h
/// @brief コマンド(アイテム系:アイテム情報更新) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/19
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

class CInfoItem;

// クラス宣言

typedef class CPacketITEM_RENEWITEMINFO : public CPacketBase
{
public:
			CPacketITEM_RENEWITEMINFO();	// コンストラクタ
	virtual ~CPacketITEM_RENEWITEMINFO();	// デストラクタ

	void	Make(CInfoItem *pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CInfoItem	*m_pInfo;	// アイテム情報
} CPacketITEM_RENEWITEMINFO, *PCPacketITEM_RENEWITEMINFO;
