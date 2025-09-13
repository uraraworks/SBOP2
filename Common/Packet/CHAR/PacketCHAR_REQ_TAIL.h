/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_TAIL.h										 */
/* 内容			:コマンド(キャラ系:付いて行く要求) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_TAIL : public CPacketBase
{
public:
			CPacketCHAR_REQ_TAIL();						/* コンストラクタ */
	virtual ~CPacketCHAR_REQ_TAIL();					/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwTargetCharID, BOOL bTail);		/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);										/* パケットを設定 */


public:
	BOOL	m_bTail;						/* ついていくか解除か */
	DWORD	m_dwCharID,						/* キャラID */
			m_dwTargetCharID;				/* 付いて行く相手のキャラID */
} CPacketCHAR_REQ_TAIL, *PCPacketCHAR_REQ_TAIL;

/* Copyright(C)URARA-works 2007 */
