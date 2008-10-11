/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_EFC_RENEWBALLOON.h								 */
/* 内容			:コマンド(管理者系:噴出し情報の更新) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/30													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoEfcBalloon;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_EFC_RENEWBALLOON : public CPacketBase
{
public:
			CPacketADMIN_EFC_RENEWBALLOON();					/* コンストラクタ */
	virtual ~CPacketADMIN_EFC_RENEWBALLOON();					/* デストラクタ */

	void	Make	(DWORD dwListID, CLibInfoEfcBalloon *pSrc);			/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);									/* パケットを設定 */


public:
	DWORD				m_dwListID;			/* 種別ID */
	CLibInfoEfcBalloon	*m_pInfo;			/* 噴出し情報 */
} CPacketADMIN_EFC_RENEWBALLOON, *PCPacketADMIN_EFC_RENEWBALLOON;

/* Copyright(C)URARA-works 2007 */
