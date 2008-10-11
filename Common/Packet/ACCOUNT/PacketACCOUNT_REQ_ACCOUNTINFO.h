/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketACCOUNT_REQ_ACCOUNTINFO.h							 */
/* 内容			:コマンド(アカウント系:アカウント情報要求) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/06													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketACCOUNT_REQ_ACCOUNTINFO : public CPacketBase
{
public:
			CPacketACCOUNT_REQ_ACCOUNTINFO();			/* コンストラクタ */
	virtual ~CPacketACCOUNT_REQ_ACCOUNTINFO();			/* デストラクタ */

	void	Make	(DWORD dwAccountID);						/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwAccountID;						/* アカウントID */
} CPacketACCOUNT_REQ_ACCOUNTINFO, *PCPacketACCOUNT_REQ_ACCOUNTINFO;

/* Copyright(C)URARA-works 2006 */
