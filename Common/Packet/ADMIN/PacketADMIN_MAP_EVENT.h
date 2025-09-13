/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_EVENT.h									 */
/* 内容			:コマンド(管理者系:マップイベント情報通知) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/25													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoMapEventBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_EVENT : public CPacketBase
{
public:
			CPacketADMIN_MAP_EVENT();					/* コンストラクタ */
	virtual ~CPacketADMIN_MAP_EVENT();					/* デストラクタ */

	void	Make	(DWORD dwMapID, CInfoMapEventBase *pInfo);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwMapID;							/* マップID */
	CInfoMapEventBase	*m_pInfo;				/* イベント情報 */
} CPacketADMIN_MAP_EVENT, *PCPacketADMIN_MAP_EVENT;

/* Copyright(C)URARA-works 2008 */
