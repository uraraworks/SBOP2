/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_SETPARTS.h									 */
/* 内容			:コマンド(管理者系:マップパーツ配置) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_SETPARTS : public CPacketBase
{
public:
			CPacketADMIN_MAP_SETPARTS();					/* コンストラクタ */
	virtual ~CPacketADMIN_MAP_SETPARTS();					/* デストラクタ */

	void	Make	(DWORD dwMapID, int x, int y, DWORD dwPartsID, BOOL bPile);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	BOOL	m_bPile;				/* TRUE:重ね合わせ用マップ */
	POINT	m_ptPos;				/* 座標 */
	DWORD	m_dwMapID,				/* マップID */
			m_dwPartsID;			/* パーツID */
} CPacketADMIN_MAP_SETPARTS, *PCPacketADMIN_MAP_SETPARTS;

/* Copyright(C)URARA-works 2007 */
