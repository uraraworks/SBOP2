/// @file PacketCONNECT_REQ_LOGIN_TOKEN.h
/// @brief コマンド(接続系:端末トークンログイン要求) 定義ファイル
/// @details ログインコード方式(docs/login-code-auth-plan.md)で発行された
///          端末トークン(16進64文字)によるログイン要求。
///          MACアドレスは既存の CPacketCONNECT_REQ_LOGIN と同じくダミー値を送る。

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCONNECT_REQ_LOGIN_TOKEN : public CPacketBase
{
public:
			CPacketCONNECT_REQ_LOGIN_TOKEN();	// コンストラクタ
	virtual ~CPacketCONNECT_REQ_LOGIN_TOKEN();	// デストラクタ

	void	Make(LPCSTR pszDeviceToken, PBYTE pbyMacAddr);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	BYTE	m_byMacAddr[6];	// MACアドレス
	CmyString	m_strDeviceToken;	// 端末トークン(16進64文字)
} CPacketCONNECT_REQ_LOGIN_TOKEN, *PCPacketCONNECT_REQ_LOGIN_TOKEN;
