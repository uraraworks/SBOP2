/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_REQ_ACCOUNT.h								 */
/* 内容			:コマンド(管理者系:アカウント情報要求) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/07													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_REQ_ACCOUNT : public CPacketBase
{
public:
			CPacketADMIN_CHAR_REQ_ACCOUNT();			/* コンストラクタ */
	virtual ~CPacketADMIN_CHAR_REQ_ACCOUNT();			/* デストラクタ */

	void	Make	(DWORD dwAccountID);						/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwAccountID;					/* アカウントID */
} CPacketADMIN_CHAR_REQ_ACCOUNT, *PCPacketADMIN_CHAR_REQ_ACCOUNT;

/* Copyright(C)URARA-works 2008 */
