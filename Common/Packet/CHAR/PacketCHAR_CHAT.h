/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_CHAT.h											 */
/* 内容			:コマンド(キャラ系:チャット通知) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/02/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_CHAT : public CPacketBase
{
public:
			CPacketCHAR_CHAT();								/* コンストラクタ */
	virtual ~CPacketCHAR_CHAT();							/* デストラクタ */

	void	Make	(DWORD dwCharID, LPCSTR pszChat);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD		m_dwCharID;					/* キャラID */
	CmyString	m_strChat;					/* 発言内容 */
} CPacketCHAR_CHAT, *PCPacketCHAR_CHAT;

/* Copyright(C)URARA-works 2007 */
