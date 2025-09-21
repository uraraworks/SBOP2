/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_REQ_EFFECTINFO.h								 */
/* 内容			:コマンド(エフェクト系:エフェクト情報要求) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketEFFECT_REQ_EFFECTINFO : public CPacketBase
{
public:
			CPacketEFFECT_REQ_EFFECTINFO();				/* コンストラクタ */
	virtual ~CPacketEFFECT_REQ_EFFECTINFO();			/* デストラクタ */

	void	Make	(DWORD dwEffect);							/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwEffectID;						/* エフェクトID */
} CPacketEFFECT_REQ_EFFECTINFO, *PCPacketEFFECT_REQ_EFFECTINFO;

/* Copyright(C)URARA-works 2007 */
