/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_CHARINFO2.h									 */
/* 内容			:コマンド(キャラ系:キャラ情報要求(複数)) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/02/13													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_CHARINFO2 : public CPacketBase
{
public:
			CPacketCHAR_REQ_CHARINFO2();				/* コンストラクタ */
	virtual ~CPacketCHAR_REQ_CHARINFO2();				/* デストラクタ */

	void	Make	(PARRAYDWORD paSrc);						/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	ARRAYDWORD m_adwCharID;				/* キャラID */
} CPacketCHAR_REQ_CHARINFO2, *PCPacketCHAR_REQ_CHARINFO2;

/* Copyright(C)URARA-works 2007 */
