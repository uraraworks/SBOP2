/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_DELETEMAPSHADOW.h							 */
/* 内容			:コマンド(管理者系:マップマップ影削除) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/07													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_DELETEMAPSHADOW : public CPacketBase
{
public:
			CPacketADMIN_MAP_DELETEMAPSHADOW();				/* コンストラクタ */
	virtual ~CPacketADMIN_MAP_DELETEMAPSHADOW();			/* デストラクタ */

	void	Make	(DWORD dwShadowID);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwShadowID;			/* マップ影ID */
} CPacketADMIN_MAP_DELETEMAPSHADOW, *PCPacketADMIN_MAP_DELETEMAPSHADOW;

/* Copyright(C)URARA-works 2007 */
