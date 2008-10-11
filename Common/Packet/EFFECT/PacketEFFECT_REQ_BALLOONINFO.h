/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_REQ_BALLOONINFO.h								 */
/* 内容			:コマンド(エフェクト系:噴出し情報要求) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketEFFECT_REQ_BALLOONINFO : public CPacketBase
{
public:
			CPacketEFFECT_REQ_BALLOONINFO();				/* コンストラクタ */
	virtual ~CPacketEFFECT_REQ_BALLOONINFO();				/* デストラクタ */

	void	Make	(DWORD dwBalloonID);						/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwBalloonID;					/* 噴出しID */
} CPacketEFFECT_REQ_BALLOONINFO, *PCPacketEFFECT_REQ_BALLOONINFO;

/* Copyright(C)URARA-works 2007 */
