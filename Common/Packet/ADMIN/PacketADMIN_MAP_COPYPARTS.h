/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_COPYPARTS.h								 */
/* 内容			:コマンド(管理者系:マップパーツコピー) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/14													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_COPYPARTS : public CPacketBase
{
public:
			CPacketADMIN_MAP_COPYPARTS();					/* コンストラクタ */
	virtual ~CPacketADMIN_MAP_COPYPARTS();					/* デストラクタ */

	void	Make	(DWORD dwSrcPartsID, POINT ptViewPos);			/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwSrcPartsID;			/* コピー元パーツID */
	POINT	m_ptViewPos;			/* 表示位置 */
} CPacketADMIN_MAP_COPYPARTS, *PCPacketADMIN_MAP_COPYPARTS;

/* Copyright(C)URARA-works 2007 */
