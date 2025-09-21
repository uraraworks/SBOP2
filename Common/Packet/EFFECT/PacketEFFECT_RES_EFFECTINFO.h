/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_RES_EFFECTINFO.h								 */
/* 内容			:コマンド(エフェクト系:エフェクト情報応答) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoEffect;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketEFFECT_RES_EFFECTINFO : public CPacketBase
{
public:
			CPacketEFFECT_RES_EFFECTINFO();				/* コンストラクタ */
	virtual ~CPacketEFFECT_RES_EFFECTINFO();			/* デストラクタ */

	void	Make	(CInfoEffect *pInfo);						/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoEffect	*m_pInfo;					/* エフェクト情報 */
} CPacketEFFECT_RES_EFFECTINFO, *PCPacketEFFECT_RES_EFFECTINFO;

/* Copyright(C)URARA-works 2007 */
