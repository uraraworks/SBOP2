/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_RENEWMAPPARTS.h								 */
/* 内容			:コマンド(管理者系:マップパーツ更新) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/29													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoMapParts;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_RENEWMAPPARTS : public CPacketBase
{
public:
			CPacketADMIN_RENEWMAPPARTS();			/* コンストラクタ */
	virtual ~CPacketADMIN_RENEWMAPPARTS();			/* デストラクタ */

	void	Make	(CInfoMapParts *pInfo);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);						/* パケットを設定 */


public:
	CInfoMapParts	*m_pInfoMapParts;		/* マップパーツ情報 */
} CPacketADMIN_RENEWMAPPARTS, *PCPacketADMIN_RENEWMAPPARTS;

/* Copyright(C)URARA-works 2007 */
