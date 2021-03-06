/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMSGCMD_CHGACCE.h										 */
/* 内容			:コマンド(メッセージコマンド系:アクセサリ変更) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMSGCMD_CHGACCE : public CPacketBase
{
public:
			CPacketMSGCMD_CHGACCE();					/* コンストラクタ */
	virtual ~CPacketMSGCMD_CHGACCE();					/* デストラクタ */

	void	Make	(DWORD dwCharID, int nType);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwCharID;				/* キャラID */
	int		m_nType;				/* 種別 */
} CPacketMSGCMD_CHGACCE, *PCPacketMSGCMD_CHGACCE;

/* Copyright(C)URARA-works 2007 */
