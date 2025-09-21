/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_MAPSHADOW.h										 */
/* 内容			:コマンド(マップ系:マップ影情報通知) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMapShadow;
class CInfoMapShadow;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_MAPSHADOW : public CPacketBase
{
public:
			CPacketMAP_MAPSHADOW();					/* コンストラクタ */
	virtual ~CPacketMAP_MAPSHADOW();				/* デストラクタ */

	void	Make	(CLibInfoMapShadow *pLibInfo);			/* パケットを作成 */
	void	Make	(CInfoMapShadow *pInfo);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);						/* パケットを設定 */


public:
	CLibInfoMapShadow	*m_pLibInfoMapShadow;		/* マップ影情報 */
} CPacketMAP_MAPSHADOW, *PCPacketMAP_MAPSHADOW;

/* Copyright(C)URARA-works 2007 */
