/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_RES_TAIL.h										 */
/* 内容			:コマンド(キャラ系:付いて行く応答) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

enum {
	RES_TAIL_RESULT_NONE = 0,
	RES_TAIL_RESULT_OK,							/* ついていく */
	RES_TAIL_RESULT_PARGE,						/* ついていく解除 */
	RES_TAIL_RESULT_TAILPARGE,					/* つれていく解除 */
	RES_TAIL_RESULT_NG_TAIL,					/* 既についていっている */
	RES_TAIL_RESULT_NG_STATE,					/* ついていけない状態 */
	RES_TAIL_RESULT_NG_SWOON,					/* 気絶中なのでついていけない */
	RES_TAIL_RESULT_NG_PLACE,					/* ついていけない場所 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_RES_TAIL : public CPacketBase
{
public:
			CPacketCHAR_RES_TAIL();									/* コンストラクタ */
	virtual ~CPacketCHAR_RES_TAIL();								/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwTargetCharID, int nResult, DWORD dwPara=0);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);										/* パケットを設定 */


public:
	DWORD	m_dwCharID,						/* キャラID */
			m_dwTargetCharID,				/* 付いて行く相手のキャラID */
			m_dwPara;						/* パラメータ */
	int		m_nResult;						/* 結果 */
} CPacketCHAR_RES_TAIL, *PCPacketCHAR_RES_TAIL;

/* Copyright(C)URARA-works 2007 */
