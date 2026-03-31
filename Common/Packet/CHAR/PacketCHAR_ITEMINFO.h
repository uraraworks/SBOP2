/// @file PacketCHAR_ITEMINFO.h
/// @brief コマンド(キャラ系:アイテム情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/31
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCHAR_ITEMINFO : public CPacketBase
{
public:
			CPacketCHAR_ITEMINFO();	// コンストラクタ
	virtual ~CPacketCHAR_ITEMINFO();	// デストラクタ

	void	Make(DWORD dwCharID, PARRAYDWORD paSrc);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID;	// キャラID
	ARRAYDWORD	m_adwItemID;	// アイテムID
} CPacketCHAR_ITEMINFO, *PCPacketCHAR_ITEMINFO;
