/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_RENEWMAPSIZE.h									 */
/* 内容			:コマンド(マップ系:マップサイズ更新) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_RENEWMAPSIZE : public CPacketBase
{
public:
			CPacketMAP_RENEWMAPSIZE();						/* コンストラクタ */
	virtual ~CPacketMAP_RENEWMAPSIZE();						/* デストラクタ */

	void	Make	(DWORD dwMapID, int nDirection, int nSize);		/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwMapID;				/* マップID */
	int		m_nDirection,			/* 増減する方向 */
			m_nSize;				/* 増減する距離 */
} CPacketMAP_RENEWMAPSIZE, *PCPacketMAP_RENEWMAPSIZE;

/* Copyright(C)URARA-works 2007 */
