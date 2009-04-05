/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCONNECT_KEEPALIVE.h									 */
/* 内容			:コマンド(接続系:生存確認通知) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/21													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCONNECT_KEEPALIVE : public CPacketBase
{
public:
			CPacketCONNECT_KEEPALIVE();					/* コンストラクタ */
	virtual ~CPacketCONNECT_KEEPALIVE();				/* デストラクタ */

	void	Make	(DWORD dwData);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwData;				/* 返信用データ */
} CPacketCONNECT_KEEPALIVE, *PCPacketCONNECT_KEEPALIVE;

/* Copyright(C)URARA-works 2008 */
