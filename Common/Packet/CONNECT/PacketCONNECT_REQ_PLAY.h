/// @file PacketCONNECT_REQ_PLAY.h
/// @brief コマンド(接続系:ゲーム開始要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/12/31
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCONNECT_REQ_PLAY : public CPacketBase
{
public:
			CPacketCONNECT_REQ_PLAY();	// コンストラクタ
	virtual ~CPacketCONNECT_REQ_PLAY();	// デストラクタ

	void	Make(DWORD dwAccountID, DWORD dwCharID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwAccountID,	// アカウントID
			m_dwCharID;	// キャラID
} CPacketCONNECT_REQ_PLAY, *PCPacketCONNECT_REQ_PLAY;
