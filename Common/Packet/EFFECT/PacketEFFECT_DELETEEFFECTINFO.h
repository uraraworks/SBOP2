/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_DELETEEFFECTINFO.h							 */
/* 内容			:コマンド(エフェクト系:エフェクト情報削除) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketEFFECT_DELETEEFFECTINFO : public CPacketBase
{
public:
			CPacketEFFECT_DELETEEFFECTINFO();				/* コンストラクタ */
	virtual ~CPacketEFFECT_DELETEEFFECTINFO();				/* デストラクタ */

	void	Make	(DWORD dwEffectID);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwEffectID;			/* エフェクトID */
} CPacketEFFECT_DELETEEFFECTINFO, *PCPacketEFFECT_DELETEEFFECTINFO;

/* Copyright(C)URARA-works 2007 */
