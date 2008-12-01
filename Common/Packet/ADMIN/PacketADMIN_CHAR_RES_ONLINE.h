/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_RES_ONLINE.h								 */
/* 内容			:コマンド(管理者系:オンライン中キャラ一覧応答) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfoCharBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RES_ONLINE : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RES_ONLINE();			/* コンストラクタ */
	virtual ~CPacketADMIN_CHAR_RES_ONLINE();		/* デストラクタ */

	void	Make	(PCLibInfoCharBase pInfo);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);						/* パケットを設定 */


public:
	PCLibInfoCharBase	m_pLibInfo;				/* キャラ情報 */
} CPacketADMIN_CHAR_RES_ONLINE, *PCPacketADMIN_CHAR_RES_ONLINE;

/* Copyright(C)URARA-works 2008 */
