/// @file PacketADMIN_RENEW_CLIENTVERSION.h
/// @brief コマンド(管理者系:クライアントバージョン更新) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/07
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

class CInfoAccount;

// クラス宣言

typedef class CPacketADMIN_RENEW_CLIENTVERSION : public CPacketBase
{
public:
			CPacketADMIN_RENEW_CLIENTVERSION();	// コンストラクタ
	virtual ~CPacketADMIN_RENEW_CLIENTVERSION();	// デストラクタ

	void	Make(LPCSTR pszClientVersion);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CmyString	m_strClientVersion;	// クライアントバージョン
} CPacketADMIN_RENEW_CLIENTVERSION, *PCPacketADMIN_RENEW_CLIENTVERSION;
