/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_ITEMTYPE_ADD.h									 */
/* 内容			:コマンド(管理者系:アイテム種別情報追加) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEMTYPE_ADD : public CPacketBase
{
public:
			CPacketADMIN_ITEMTYPE_ADD();				/* コンストラクタ */
	virtual ~CPacketADMIN_ITEMTYPE_ADD();				/* デストラクタ */

	void	Make	(CInfoItemTypeBase *pInfo);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoItemTypeBase	*m_pInfoItem;			/* 追加するアイテム種別情報 */
} CPacketADMIN_ITEMTYPE_ADD, *PCPacketADMIN_ITEMTYPE_ADD;

/* Copyright(C)URARA-works 2007 */
