/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_RENEWMAPOBJECT.h							 */
/* 内容			:コマンド(管理者系:マップオブジェクト更新) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoMapObject;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_RENEWMAPOBJECT : public CPacketBase
{
public:
			CPacketADMIN_MAP_RENEWMAPOBJECT();		/* コンストラクタ */
	virtual ~CPacketADMIN_MAP_RENEWMAPOBJECT();		/* デストラクタ */

	void	Make	(CInfoMapObject *pInfo);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);						/* パケットを設定 */


public:
	CInfoMapObject	*m_pInfoMapObject;		/* マップオブジェクト情報 */
} CPacketADMIN_MAP_RENEWMAPOBJECT, *PCPacketADMIN_MAP_RENEWMAPOBJECT;

/* Copyright(C)URARA-works 2008 */
