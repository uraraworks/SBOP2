/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_ONLINE.h											 */
/* 内容			:コマンド(マップ系:オンライン数通知) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/02/10													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_ONLINE : public CPacketBase
{
public:
			CPacketMAP_ONLINE();						/* コンストラクタ */
	virtual ~CPacketMAP_ONLINE();						/* デストラクタ */

	void	Make	(int nCount);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	int m_nCount;							/* オンライン数 */
} CPacketMAP_ONLINE, *PCPacketMAP_ONLINE;

/* Copyright(C)URARA-works 2007 */
