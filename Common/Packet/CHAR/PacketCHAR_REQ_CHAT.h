/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_CHAT.h										 */
/* 内容			:コマンド(キャラ系:チャット要求) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/02/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

enum {
	CHATTYPE_NORMAL = 0,							/* 通常 */
	CHATTYPE_PARTY,									/* パーティチャット */
	CHATTYPE_MAP,									/* マップ内放送 */
	CHATTYPE_ADMIN,									/* 一斉放送 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_CHAT : public CPacketBase
{
public:
			CPacketCHAR_REQ_CHAT();							/* コンストラクタ */
	virtual ~CPacketCHAR_REQ_CHAT();						/* デストラクタ */

	void	Make	(int nType, DWORD dwCharID, LPCSTR pszChat);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	int			m_nType;					/* チャット種別 */
	DWORD		m_dwCharID;					/* キャラID */
	CmyString	m_strChat;					/* 発言内容 */
} CPacketCHAR_REQ_CHAT, *PCPacketCHAR_REQ_CHAT;

/* Copyright(C)URARA-works 2007 */
