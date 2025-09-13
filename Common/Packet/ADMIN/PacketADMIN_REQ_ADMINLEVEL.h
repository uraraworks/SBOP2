/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_REQ_ADMINLEVEL.h								 */
/* 内容			:コマンド(管理者系:管理者権限レベル要求) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_REQ_ADMINLEVEL : public CPacketBase
{
public:
			CPacketADMIN_REQ_ADMINLEVEL();				/* コンストラクタ */
	virtual ~CPacketADMIN_REQ_ADMINLEVEL();				/* デストラクタ */

	void	Make	(DWORD dwAccountID);						/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwAccountID;					/* アカウントID */
} CPacketADMIN_REQ_ADMINLEVEL, *PCPacketADMIN_REQ_ADMINLEVEL;

/* Copyright(C)URARA-works 2006 */
