/// @file PacketCHAR_CHAT.h
/// @brief コマンド(キャラ系:チャット通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/02/04
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCHAR_CHAT : public CPacketBase
{
public:
			CPacketCHAR_CHAT();	// コンストラクタ
	virtual ~CPacketCHAR_CHAT();	// デストラクタ

	void	Make(DWORD dwCharID, LPCSTR pszChat);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID;	// キャラID
	CmyString	m_strChat;	// 発言内容
} CPacketCHAR_CHAT, *PCPacketCHAR_CHAT;
