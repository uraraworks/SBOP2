/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_SET_EFFECT.h									 */
/* 内容			:コマンド(キャラ系:エフェクトを指定) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/27													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_SET_EFFECT : public CPacketBase
{
public:
			CPacketCHAR_SET_EFFECT();						/* コンストラクタ */
	virtual ~CPacketCHAR_SET_EFFECT();						/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwEffectID);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwCharID,						/* キャラID */
			m_dwEffectID;					/* エフェクトID */
} CPacketCHAR_SET_EFFECT, *PCPacketCHAR_SET_EFFECT;

/* Copyright(C)URARA-works 2007 */
