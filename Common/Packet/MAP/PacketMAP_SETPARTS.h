/// @file PacketMAP_SETPARTS.h
/// @brief コマンド(マップ系:マップパーツ配置) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketMAP_SETPARTS : public CPacketBase
{
public:
			CPacketMAP_SETPARTS();	// コンストラクタ
	virtual ~CPacketMAP_SETPARTS();	// デストラクタ

	void	Make(DWORD dwMapID, int x, int y, DWORD dwPartsID, BOOL bPile);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	BOOL	m_bPile;	// 重ね合わせ用
	POINT	m_ptPos;	// 座標
	DWORD	m_dwMapID,	// マップID
			m_dwPartsID;	// パーツID
} CPacketMAP_SETPARTS, *PCPacketMAP_SETPARTS;
