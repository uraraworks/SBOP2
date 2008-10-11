/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_RES_ITEMTYPEINFO.h								 */
/* 内容			:コマンド(アイテム系:アイテム種別情報応答) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_RES_ITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_RES_ITEMTYPEINFO();				/* コンストラクタ */
	virtual ~CPacketITEM_RES_ITEMTYPEINFO();			/* デストラクタ */

	void	Make	(CInfoItemTypeBase *pInfo);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoItemTypeBase	*m_pInfo;					/* アイテム種別情報 */
} CPacketITEM_RES_ITEMTYPEINFO, *PCPacketITEM_RES_ITEMTYPEINFO;

/* Copyright(C)URARA-works 2007 */
