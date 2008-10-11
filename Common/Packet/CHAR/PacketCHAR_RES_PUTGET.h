/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_RES_PUTGET.h									 */
/* 内容			:コマンド(キャラ系:アイテムを拾う置く応答) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

enum {
	RESULTID_CHAR_RES_PUTGET_NONE = 0,
	RESULTID_CHAR_RES_PUTGET_PUT,				/* 置いた */
	RESULTID_CHAR_RES_PUTGET_GET,				/* 取得した */
	RESULTID_CHAR_RES_PUTGET_DELETE,			/* 削除した */
};

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_RES_PUTGET : public CPacketBase
{
public:
			CPacketCHAR_RES_PUTGET();					/* コンストラクタ */
	virtual ~CPacketCHAR_RES_PUTGET();					/* デストラクタ */

	void	Make	(DWORD dwItemID, int nResult);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwItemID;					/* アイテムID */
	int		m_nResult;					/* 結果 */
} CPacketCHAR_RES_PUTGET, *PCPacketCHAR_RES_PUTGET;

/* Copyright(C)URARA-works 2007 */
