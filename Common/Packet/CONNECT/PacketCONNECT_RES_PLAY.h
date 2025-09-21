/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketCONNECT_RES_PLAY.h									 */
/* 内容			:コマンド(接続系:ゲーム開始応答) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

enum {
	PLAYRES_NONE = 0,
	PLAYRES_OK,						/* 問題無し */
	PLAYRES_ADMINLEVEL_ALL,			/* 管理者(全権限) */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCONNECT_RES_PLAY : public CPacketBase
{
public:
			CPacketCONNECT_RES_PLAY();					/* コンストラクタ */
	virtual ~CPacketCONNECT_RES_PLAY();					/* デストラクタ */

	void	Make	(int nResult);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	int		m_nResult;					/* 結果 */
} CPacketCONNECT_RES_PLAY, *PCPacketCONNECT_RES_PLAY;

/* Copyright(C)URARA-works 2006 */
