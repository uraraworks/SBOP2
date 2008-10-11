/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_CHARINFO.h									 */
/* 内容			:コマンド(キャラ系:キャラ情報要求) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_CHARINFO : public CPacketBase
{
public:
			CPacketCHAR_REQ_CHARINFO();					/* コンストラクタ */
	virtual ~CPacketCHAR_REQ_CHARINFO();				/* デストラクタ */

	void	Make	(DWORD dwCharID);							/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwCharID;						/* キャラID */
} CPacketCHAR_REQ_CHARINFO, *PCPacketCHAR_REQ_CHARINFO;

/* Copyright(C)URARA-works 2006 */
