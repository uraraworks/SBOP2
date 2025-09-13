/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_RES_ACCOUNT.h								 */
/* 内容			:コマンド(管理者系:アカウント情報応答) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/07													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoAccount;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RES_ACCOUNT : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RES_ACCOUNT();			/* コンストラクタ */
	virtual ~CPacketADMIN_CHAR_RES_ACCOUNT();			/* デストラクタ */

	void	Make	(CInfoAccount *pInfoAccount, DWORD dwIP, LPCSTR pszMacAddress);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD		m_dwAccountID,				/* アカウントID */
				m_dwIP;						/* IPアドレス */
	CmyString	m_strAccount,				/* アカウント */
				m_strPassword,				/* パスワード */
				m_strMacAddress;			/* MACアドレス */
} CPacketADMIN_RES_ACCOUNT, *PCPacketADMIN_RES_ACCOUNT;

/* Copyright(C)URARA-works 2008 */
