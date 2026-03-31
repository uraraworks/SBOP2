/// @file PacketVERSION_REQ_VERSIONCHECK.h
/// @brief コマンド(バージョン系:バージョンチェック要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketVERSION_REQ_VERSIONCHECK : public CPacketBase
{
public:
			CPacketVERSION_REQ_VERSIONCHECK();	// コンストラクタ
	virtual ~CPacketVERSION_REQ_VERSIONCHECK();	// デストラクタ

	void	Make(DWORD dwVersion);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwVersion;	// バージョンコード
} CPacketVERSION_REQ_VERSIONCHECK, *PCPacketVERSION_REQ_VERSIONCHECK;
