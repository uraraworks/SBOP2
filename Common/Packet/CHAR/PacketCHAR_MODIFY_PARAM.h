/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_MODIFY_PARAM.h									 */
/* 内容			:コマンド(キャラ系:パラメータ変更通知) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/09													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

enum {
	PARAMID_CHAR_MODIFY_NONE = 0,
	PARAMID_CHAR_MODIFY_ANIME,					/* アニメーション番号の変更 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_MODIFY_PARAM : public CPacketBase
{
public:
			CPacketCHAR_MODIFY_PARAM();					/* コンストラクタ */
	virtual ~CPacketCHAR_MODIFY_PARAM();				/* デストラクタ */

	void	Make	(DWORD dwCharID, int nType, int nParam);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwCharID;					/* キャラID */
	int		m_nType,					/* 種別 */
			m_nParam;					/* パラメータ */
} CPacketCHAR_MODIFY_PARAM, *PCPacketCHAR_MODIFY_PARAM;

/* Copyright(C)URARA-works 2007 */
