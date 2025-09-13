/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_RENEWITEMINFO.h									 */
/* 内容			:コマンド(アイテム系:アイテム情報更新) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/19													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItem;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_RENEWITEMINFO : public CPacketBase
{
public:
			CPacketITEM_RENEWITEMINFO();				/* コンストラクタ */
	virtual ~CPacketITEM_RENEWITEMINFO();				/* デストラクタ */

	void	Make	(CInfoItem *pInfo);							/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoItem	*m_pInfo;					/* アイテム情報 */
} CPacketITEM_RENEWITEMINFO, *PCPacketITEM_RENEWITEMINFO;

/* Copyright(C)URARA-works 2007 */
