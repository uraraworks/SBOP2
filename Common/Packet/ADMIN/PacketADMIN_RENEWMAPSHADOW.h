/// @file PacketADMIN_RENEWMAPSHADOW.h
/// @brief コマンド(管理者系:マップ影更新) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/06
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

class CInfoMapShadow;

// クラス宣言

typedef class CPacketADMIN_RENEWMAPSHADOW : public CPacketBase
{
public:
			CPacketADMIN_RENEWMAPSHADOW();	// コンストラクタ
	virtual ~CPacketADMIN_RENEWMAPSHADOW();	// デストラクタ

	void	Make(CInfoMapShadow *pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CInfoMapShadow	*m_pInfoMapShadow;	// マップ影情報
} CPacketADMIN_RENEWMAPSHADOW, *PCPacketADMIN_RENEWMAPSHADOW;
