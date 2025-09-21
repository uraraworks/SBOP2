/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_MAPPARTS.h										 */
/* 内容			:コマンド(マップ系:マップパーツ情報通知) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMapParts;
class CInfoMapParts;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_MAPPARTS : public CPacketBase
{
public:
			CPacketMAP_MAPPARTS();					/* コンストラクタ */
	virtual ~CPacketMAP_MAPPARTS();					/* デストラクタ */

	void	Make	(CLibInfoMapParts *pLibInfo);			/* パケットを作成 */
	void	Make	(CInfoMapParts *pInfo);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);						/* パケットを設定 */


public:
	CLibInfoMapParts	*m_pLibInfoMapParts;		/* マップパーツ情報 */
} CPacketMAP_MAPPARTS, *PCPacketMAP_MAPPARTS;

/* Copyright(C)URARA-works 2007 */
