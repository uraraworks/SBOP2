/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_PUSH.h										 */
/* 内容			:コマンド(キャラ系:押す要求) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/02													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_PUSH : public CPacketBase
{
public:
			CPacketCHAR_REQ_PUSH();						/* コンストラクタ */
	virtual ~CPacketCHAR_REQ_PUSH();					/* デストラクタ */

	void	Make	(DWORD dwCharID, int nDirection, int nPushCount = 1);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwCharID;				/* キャラID */
	int		m_nDirection,			/* 向き */
			m_nPushCount;			/* 押して進む数 */
} CPacketCHAR_REQ_PUSH, *PCPacketCHAR_REQ_PUSH;

/* Copyright(C)URARA-works 2007 */
