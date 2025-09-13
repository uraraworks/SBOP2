/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_EFC_RENEWEFFECT.h								 */
/* 内容			:コマンド(管理者系:エフェクト情報の更新) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/06													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoEffect;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_EFC_RENEWEFFECT : public CPacketBase
{
public:
			CPacketADMIN_EFC_RENEWEFFECT();						/* コンストラクタ */
	virtual ~CPacketADMIN_EFC_RENEWEFFECT();					/* デストラクタ */

	void	Make	(DWORD dwEffectID, CInfoEffect *pSrc);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);									/* パケットを設定 */


public:
	DWORD		m_dwEffectID;		/* エフェクトID */
	CInfoEffect	*m_pInfo;			/* エフェクト情報 */
} CPacketADMIN_EFC_RENEWEFFECT, *PCPacketADMIN_EFC_RENEWEFFECT;

/* Copyright(C)URARA-works 2008 */
