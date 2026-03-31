/// @file PacketADMIN_CHAR_RENEW_ACCOUNT.h
/// @brief コマンド(管理者系:アカウント情報更新) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/07
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

class CInfoAccount;

// クラス宣言

typedef class CPacketADMIN_CHAR_RENEW_ACCOUNT : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RENEW_ACCOUNT();	// コンストラクタ
	virtual ~CPacketADMIN_CHAR_RENEW_ACCOUNT();	// デストラクタ

	void	Make(CInfoAccount *pInfoAccount, BOOL bDisable=FALSE, LPCSTR pszMacAddress=NULL);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	BOOL	m_bDisable;	// ログイン拒否
	DWORD	m_dwAccountID;	// アカウントID
	CmyString	m_strAccount,	// アカウント
				m_strPassword,	// パスワード
				m_strMacAddress;	// 拒否MACアドレス
} CPacketADMIN_CHAR_RENEW_ACCOUNT, *PCPacketADMIN_CHAR_RENEW_ACCOUNT;
