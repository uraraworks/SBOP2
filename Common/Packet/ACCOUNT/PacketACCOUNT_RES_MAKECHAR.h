/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketACCOUNT_RES_MAKECHAR.h								 */
/* 内容			:コマンド(アカウント系:キャラ作成応答) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

enum {
	MAKECHARRES_NONE = 0,
	MAKECHARRES_OK,								/* 問題無し */
	MAKECHARRES_NG_USE,							/* 使用済み */
	MAKECHARRES_NG_SPACE,						/* 空白が使用されている */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketACCOUNT_RES_MAKECHAR : public CPacketBase
{
public:
			CPacketACCOUNT_RES_MAKECHAR();				/* コンストラクタ */
	virtual ~CPacketACCOUNT_RES_MAKECHAR();				/* デストラクタ */

	void	Make	(int nResult, DWORD dwCharID);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	int		m_nResult;					/* 結果 */
	DWORD	m_dwCharID;					/* キャラID */
} CPacketACCOUNT_RES_MAKECHAR, *PCPacketACCOUNT_RES_MAKECHAR;

/* Copyright(C)URARA-works 2006 */
