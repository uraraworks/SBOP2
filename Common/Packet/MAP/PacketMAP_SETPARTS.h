/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_SETPARTS.h										 */
/* 内容			:コマンド(マップ系:マップパーツ配置) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_SETPARTS : public CPacketBase
{
public:
			CPacketMAP_SETPARTS();							/* コンストラクタ */
	virtual ~CPacketMAP_SETPARTS();							/* デストラクタ */

	void	Make	(DWORD dwMapID, int x, int y, DWORD dwPartsID);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	POINT	m_ptPos;				/* 座標 */
	DWORD	m_dwMapID,				/* マップID */
			m_dwPartsID;			/* パーツID */
} CPacketMAP_SETPARTS, *PCPacketMAP_SETPARTS;

/* Copyright(C)URARA-works 2007 */
