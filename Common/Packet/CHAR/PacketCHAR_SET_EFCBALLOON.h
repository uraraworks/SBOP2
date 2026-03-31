/// @file PacketCHAR_SET_EFCBALLOON.h
/// @brief コマンド(キャラ系:噴出しを指定) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/12/31
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCHAR_SET_EFCBALLOON : public CPacketBase
{
public:
			CPacketCHAR_SET_EFCBALLOON();	// コンストラクタ
	virtual ~CPacketCHAR_SET_EFCBALLOON();	// デストラクタ

	void	Make(DWORD dwCharID, DWORD dwBalloonID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID,	// キャラID
			m_dwBalloonID;	// 噴出しID
} CPacketCHAR_SET_EFCBALLOON, *PCPacketCHAR_SET_EFCBALLOON;
