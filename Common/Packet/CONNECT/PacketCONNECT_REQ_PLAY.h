/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketCONNECT_REQ_PLAY.h									 */
/* 内容			:コマンド(接続系:ゲーム開始要求) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCONNECT_REQ_PLAY : public CPacketBase
{
public:
			CPacketCONNECT_REQ_PLAY();					/* コンストラクタ */
	virtual ~CPacketCONNECT_REQ_PLAY();					/* デストラクタ */

	void	Make	(DWORD dwAccountID, DWORD dwCharID);		/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwAccountID,						/* アカウントID */
			m_dwCharID;							/* キャラID */
} CPacketCONNECT_REQ_PLAY, *PCPacketCONNECT_REQ_PLAY;

/* Copyright(C)URARA-works 2006 */
