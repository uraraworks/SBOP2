/// @file PacketACCOUNT_RES_ACCOUNTINFO.h
/// @brief コマンド(アカウント系:アカウント情報応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/07
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

class CInfoAccount;

// クラス宣言

typedef class CPacketACCOUNT_RES_ACCOUNTINFO : public CPacketBase
{
public:
			CPacketACCOUNT_RES_ACCOUNTINFO();	// コンストラクタ
	virtual ~CPacketACCOUNT_RES_ACCOUNTINFO();	// デストラクタ

	void	Make(CInfoAccount *pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CInfoAccount	*m_pInfo;	// アカウント情報
} CPacketACCOUNT_RES_ACCOUNTINFO, *PCPacketACCOUNT_RES_ACCOUNTINFO;
