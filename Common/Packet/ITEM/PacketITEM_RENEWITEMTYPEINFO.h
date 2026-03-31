/// @file PacketITEM_RENEWITEMTYPEINFO.h
/// @brief コマンド(アイテム系:アイテム種別情報更新) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/03
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

class CInfoItemTypeBase;

// クラス宣言

typedef class CPacketITEM_RENEWITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_RENEWITEMTYPEINFO();	// コンストラクタ
	virtual ~CPacketITEM_RENEWITEMTYPEINFO();	// デストラクタ

	void	Make(CInfoItemTypeBase *pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CInfoItemTypeBase	*m_pInfo;	// アイテム種別情報
} CPacketITEM_RENEWITEMTYPEINFO, *PCPacketITEM_RENEWITEMTYPEINFO;
