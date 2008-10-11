/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_RENEWADMINLEVEL.h								 */
/* 内容			:コマンド(管理者系:管理者権限レベル更新) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_RENEWADMINLEVEL : public CPacketBase
{
public:
			CPacketADMIN_RENEWADMINLEVEL();				/* コンストラクタ */
	virtual ~CPacketADMIN_RENEWADMINLEVEL();			/* デストラクタ */

	void	Make	(DWORD dwAccountID, int nLevel);			/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwAccountID;					/* アカウントID */
	int		m_nAdminLevel;					/* 管理者レベル */
} CPacketADMIN_RENEWADMINLEVEL, *PCPacketADMIN_RENEWADMINLEVEL;

/* Copyright(C)URARA-works 2006 */
