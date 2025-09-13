/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_SET_EFCBALLOON.h								 */
/* 内容			:コマンド(キャラ系:噴出しを指定) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_SET_EFCBALLOON : public CPacketBase
{
public:
			CPacketCHAR_SET_EFCBALLOON();					/* コンストラクタ */
	virtual ~CPacketCHAR_SET_EFCBALLOON();					/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwBalloonID);			/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwCharID,						/* キャラID */
			m_dwBalloonID;					/* 噴出しID */
} CPacketCHAR_SET_EFCBALLOON, *PCPacketCHAR_SET_EFCBALLOON;

/* Copyright(C)URARA-works 2007 */
