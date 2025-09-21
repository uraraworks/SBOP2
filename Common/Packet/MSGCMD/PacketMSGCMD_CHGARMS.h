/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMSGCMD_CHGARMS.h										 */
/* 内容			:コマンド(メッセージコマンド系:持ち物変更) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/21													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMSGCMD_CHGARMS : public CPacketBase
{
public:
			CPacketMSGCMD_CHGARMS();					/* コンストラクタ */
	virtual ~CPacketMSGCMD_CHGARMS();					/* デストラクタ */

	void	Make	(DWORD dwCharID, int nType);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwCharID;				/* キャラID */
	int		m_nType;				/* 種別 */
} CPacketMSGCMD_CHGARMS, *PCPacketMSGCMD_CHGARMS;

/* Copyright(C)URARA-works 2007 */
