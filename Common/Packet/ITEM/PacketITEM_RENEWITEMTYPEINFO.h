/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_RENEWITEMTYPEINFO.h								 */
/* 内容			:コマンド(アイテム系:アイテム種別情報更新) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_RENEWITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_RENEWITEMTYPEINFO();			/* コンストラクタ */
	virtual ~CPacketITEM_RENEWITEMTYPEINFO();			/* デストラクタ */

	void	Make	(CInfoItemTypeBase *pInfo);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoItemTypeBase	*m_pInfo;					/* アイテム種別情報 */
} CPacketITEM_RENEWITEMTYPEINFO, *PCPacketITEM_RENEWITEMTYPEINFO;

/* Copyright(C)URARA-works 2007 */
