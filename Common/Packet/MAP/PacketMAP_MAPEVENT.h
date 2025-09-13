/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_MAPEVENT.h										 */
/* 内容			:コマンド(マップ系:マップイベント情報通知) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/25													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMapEvent;
class CInfoMapEventBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_MAPEVENT : public CPacketBase
{
public:
			CPacketMAP_MAPEVENT();					/* コンストラクタ */
	virtual ~CPacketMAP_MAPEVENT();					/* デストラクタ */

	void	Make	(DWORD dwMapID, CLibInfoMapEvent *pLibInfo);	/* パケットを作成 */
	void	Make	(DWORD dwMapID, CInfoMapEventBase *pInfo);		/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwMapID;						/* マップID */
	CLibInfoMapEvent	*m_pLibInfo;		/* マップイベント情報 */
} CPacketMAP_MAPEVENT, *PCPacketMAP_MAPEVENT;

/* Copyright(C)URARA-works 2008 */
