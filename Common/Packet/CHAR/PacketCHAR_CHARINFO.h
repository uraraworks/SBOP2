/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_CHARINFO.h										 */
/* 内容			:コマンド(キャラ系:キャラ情報通知) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/27													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfoCharBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_CHARINFO : public CPacketBase
{
public:
			CPacketCHAR_CHARINFO();					/* コンストラクタ */
	virtual ~CPacketCHAR_CHARINFO();				/* デストラクタ */

	void	Make	(PCLibInfoCharBase pInfo);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);						/* パケットを設定 */


public:
	PCLibInfoCharBase	m_pLibInfo;				/* キャラ情報 */
} CPacketCHAR_CHARINFO, *PCPacketCHAR_CHARINFO;

/* Copyright(C)URARA-works 2007 */
