/// @file PacketMAP_SETMAPSHADOW.h
/// @brief コマンド(マップ系:マップ影配置) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/08
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketMAP_SETMAPSHADOW : public CPacketBase
{
public:
			CPacketMAP_SETMAPSHADOW();	// コンストラクタ
	virtual ~CPacketMAP_SETMAPSHADOW();	// デストラクタ

	void	Make(DWORD dwMapID, int x, int y, DWORD dwShadowID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	POINT	m_ptPos;	// 座標
	DWORD	m_dwMapID,	// マップID
			m_dwShadowID;	// マップ影ID
} CPacketMAP_SETMAPSHADOW, *PCPacketMAP_SETMAPSHADOW;
