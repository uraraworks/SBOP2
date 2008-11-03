/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_MAPOBJECTDATA.h									 */
/* 内容			:コマンド(マップ系:マップオブジェクト配置データ通知) 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMapObjectData;
class CInfoMapObjectData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_MAPOBJECTDATA : public CPacketBase
{
public:
			CPacketMAP_MAPOBJECTDATA();							/* コンストラクタ */
	virtual ~CPacketMAP_MAPOBJECTDATA();						/* デストラクタ */

	void	Make	(DWORD dwMapID, CLibInfoMapObjectData *pLibInfo);	/* パケットを作成 */
	void	Make	(DWORD dwMapID, CInfoMapObjectData *pInfo);			/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);									/* パケットを設定 */


public:
	DWORD					m_dwMapID;			/* マップID */
	CLibInfoMapObjectData	*m_pLibInfo;		/* マップオブジェクト配置データ */
} CPacketMAP_MAPOBJECTDATA, *PCPacketMAP_MAPOBJECTDATA;

/* Copyright(C)URARA-works 2008 */
