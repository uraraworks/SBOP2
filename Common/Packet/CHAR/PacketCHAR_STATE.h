/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_STATE.h											 */
/* 内容			:コマンド(キャラ系:状態通知) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/24													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_STATE : public CPacketBase
{
public:
			CPacketCHAR_STATE();							/* コンストラクタ */
	virtual ~CPacketCHAR_STATE();							/* デストラクタ */

	void	Make	(DWORD dwCharID, int nState);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwCharID;						/* キャラID */
	int		m_nState;						/* 状態 */
} CPacketCHAR_STATE, *PCPacketCHAR_STATE;

/* Copyright(C)URARA-works 2007 */
