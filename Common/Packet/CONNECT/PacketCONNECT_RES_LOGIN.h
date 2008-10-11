/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketCONNECT_RES_LOGIN.h									 */
/* 内容			:コマンド(接続系:ログイン応答) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

enum {
	LOGINRES_NONE = 0,
	LOGINRES_OK,							/* 問題無し */
	LOGINRES_NG_PASSWORD,					/* パスワード不一致 */
	LOGINRES_NG_LOGIN,						/* ログイン済み */
	LOGINRES_NG_MAC,						/* 作成済み */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCONNECT_RES_LOGIN : public CPacketBase
{
public:
			CPacketCONNECT_RES_LOGIN();					/* コンストラクタ */
	virtual ~CPacketCONNECT_RES_LOGIN();				/* デストラクタ */

	void	Make	(int nResult, DWORD dwAccountID);			/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	int		m_nResult;					/* 結果 */
	DWORD	m_dwAccountID;				/* アカウントID */
} CPacketCONNECT_RES_LOGIN, *PCPacketCONNECT_RES_LOGIN;

/* Copyright(C)URARA-works 2006 */
