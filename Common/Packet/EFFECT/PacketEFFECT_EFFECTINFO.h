/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_EFFECTINFO.h									 */
/* 内容			:コマンド(エフェクト系:エフェクト情報通知) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfo/LibInfoEffect.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketEFFECT_EFFECTINFO : public CPacketBase
{
public:
			CPacketEFFECT_EFFECTINFO();				/* コンストラクタ */
	virtual ~CPacketEFFECT_EFFECTINFO();			/* デストラクタ */

	void	Make	(PCLibInfoEffect pInfo);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);						/* パケットを設定 */


public:
	PCLibInfoEffect	m_pLibInfo;				/* エフェクト情報 */
} CPacketEFFECT_EFFECTINFO, *PCPacketEFFECT_EFFECTINFO;

/* Copyright(C)URARA-works 2007 */
