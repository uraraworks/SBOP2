/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_REQ_MAPINFO.h									 */
/* 内容			:コマンド(マップ系:マップ情報要求) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_REQ_MAPINFO : public CPacketBase
{
public:
			CPacketMAP_REQ_MAPINFO();					/* コンストラクタ */
	virtual ~CPacketMAP_REQ_MAPINFO();					/* デストラクタ */

	void	Make	(DWORD dwMapID);							/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwMapID;						/* マップID */
} CPacketMAP_REQ_MAPINFO, *PCPacketMAP_REQ_MAPINFO;

/* Copyright(C)URARA-works 2007 */
