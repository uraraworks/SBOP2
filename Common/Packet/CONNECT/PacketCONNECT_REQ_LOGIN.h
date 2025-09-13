/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketCONNECT_REQ_LOGIN.h									 */
/* 内容			:コマンド(接続系:ログイン要求) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCONNECT_REQ_LOGIN : public CPacketBase
{
public:
			CPacketCONNECT_REQ_LOGIN();					/* コンストラクタ */
	virtual ~CPacketCONNECT_REQ_LOGIN();				/* デストラクタ */

	void	Make	(LPCSTR pszAccount, LPCSTR pszPassword, PBYTE pbyMacAddr);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	BYTE		m_byMacAddr[6];					/* MACアドレス */
	CmyString	m_strAccount,					/* アカウント */
				m_strPassword;					/* パスワード */
} CPacketCONNECT_REQ_LOGIN, *PCPacketCONNECT_REQ_LOGIN;

/* Copyright(C)URARA-works 2006 */
