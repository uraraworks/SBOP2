/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_MOVEPOS.h										 */
/* 内容			:コマンド(キャラ系:移動通知) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/14													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_MOVEPOS : public CPacketBase
{
public:
			CPacketCHAR_MOVEPOS();							/* コンストラクタ */
	virtual ~CPacketCHAR_MOVEPOS();							/* デストラクタ */

	void	Make	(DWORD dwMapID, DWORD dwCharID, int nDirection, int x, int y, BOOL bUpdate);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	BOOL	m_bUpdate;					/* 自キャラの座標を更新する */
	DWORD	m_dwMapID,					/* マップID */
			m_dwCharID;					/* キャラID */
	int		m_nDirection;				/* 向き */
	POINT	m_pos;						/* 座標 */
} CPacketCHAR_MOVEPOS, *PCPacketCHAR_MOVEPOS;

/* Copyright(C)URARA-works 2007 */
