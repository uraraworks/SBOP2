/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_BALLOONINFO.h									 */
/* 内容			:コマンド(エフェクト系:噴出し情報応答) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoEfcBalloon;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketEFFECT_BALLOONINFO : public CPacketBase
{
public:
			CPacketEFFECT_BALLOONINFO();						/* コンストラクタ */
	virtual ~CPacketEFFECT_BALLOONINFO();						/* デストラクタ */

	void	Make	(DWORD dwListID, CLibInfoEfcBalloon *pLibInfo);		/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);									/* パケットを設定 */


public:
	DWORD				m_dwListID;				/* 種別ID */
	CLibInfoEfcBalloon	*m_pLibInfo;			/* 噴出し情報 */
} CPacketEFFECT_BALLOONINFO, *PCPacketEFFECT_BALLOONINFO;

/* Copyright(C)URARA-works 2007 */
