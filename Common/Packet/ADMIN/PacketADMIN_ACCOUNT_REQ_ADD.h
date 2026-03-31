/// @file PacketADMIN_ACCOUNT_REQ_ADD.h
/// @brief コマンド(管理者系:アカウントの追加要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/01/14
/// @copyright Copyright(C)URARA-works 2009

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_ACCOUNT_REQ_ADD : public CPacketBase
{
public:
			CPacketADMIN_ACCOUNT_REQ_ADD();	// コンストラクタ
	virtual ~CPacketADMIN_ACCOUNT_REQ_ADD();	// デストラクタ

	void	Make(LPCSTR pszAccount, LPCSTR pszPassword);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CmyString	m_strAccount,	// アカウント
				m_strPassword;	// パスワード
} CPacketADMIN_ACCOUNT_REQ_ADD, *PCPacketADMIN_ACCOUNT_REQ_ADD;
