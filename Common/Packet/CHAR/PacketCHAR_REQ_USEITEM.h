/// @file PacketCHAR_REQ_USEITEM.h
/// @brief コマンド(キャラ系:アイテムを使う要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/05
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCHAR_REQ_USEITEM : public CPacketBase
{
public:
			CPacketCHAR_REQ_USEITEM();	// コンストラクタ
	virtual ~CPacketCHAR_REQ_USEITEM();	// デストラクタ

	void	Make(DWORD dwCharID, DWORD dwItemID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID,	// キャラID
			m_dwItemID;	// アイテムID
} CPacketCHAR_REQ_USEITEM, *PCPacketCHAR_REQ_USEITEM;
