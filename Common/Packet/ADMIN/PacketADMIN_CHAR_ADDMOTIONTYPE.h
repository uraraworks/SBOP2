/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_ADDMOTIONTYPE.h							 */
/* 内容			:コマンド(管理者系:キャラモーション種別情報の追加) 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/09													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_ADDMOTIONTYPE : public CPacketBase
{
public:
			CPacketADMIN_CHAR_ADDMOTIONTYPE();			/* コンストラクタ */
	virtual ~CPacketADMIN_CHAR_ADDMOTIONTYPE();			/* デストラクタ */

	void	Make	(void);										/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
} CPacketADMIN_CHAR_ADDMOTIONTYPE, *PCPacketADMIN_CHAR_ADDMOTIONTYPE;

/* Copyright(C)URARA-works 2008 */
