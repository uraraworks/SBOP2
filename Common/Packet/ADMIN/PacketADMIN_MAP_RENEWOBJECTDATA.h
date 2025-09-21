/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_RENEWOBJECTDATA.h							 */
/* 内容			:コマンド(管理者系:マップオブジェクト配置データ通知) 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoMapObjectData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_RENEWOBJECTDATA : public CPacketBase
{
public:
			CPacketADMIN_MAP_RENEWOBJECTDATA();			/* コンストラクタ */
	virtual ~CPacketADMIN_MAP_RENEWOBJECTDATA();		/* デストラクタ */

	void	Make	(DWORD dwMapID, CInfoMapObjectData *pInfo);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwMapID;							/* マップID */
	CInfoMapObjectData	*m_pInfo;				/* イベント情報 */
} CPacketADMIN_MAP_RENEWOBJECTDATA, *PCPacketADMIN_MAP_RENEWOBJECTDATA;

/* Copyright(C)URARA-works 2008 */
