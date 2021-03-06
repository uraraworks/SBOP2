/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_PLAYSOUND.h									 */
/* 内容			:コマンド(管理者系:効果音の再生) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/02													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_PLAYSOUND : public CPacketBase
{
public:
			CPacketADMIN_PLAYSOUND();					/* コンストラクタ */
	virtual ~CPacketADMIN_PLAYSOUND();					/* デストラクタ */

	void	Make	(DWORD dwSoundID);							/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwSoundID;					/* 効果音ID */
} CPacketADMIN_PLAYSOUND, *PCPacketADMIN_PLAYSOUND;

/* Copyright(C)URARA-works 2008 */
