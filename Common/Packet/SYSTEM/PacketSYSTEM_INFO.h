/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketSYSTEM_INFO.h										 */
/* 内容			:コマンド(システム系:システム情報通知) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/29													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoSystem;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketSYSTEM_INFO : public CPacketBase
{
public:
			CPacketSYSTEM_INFO();						/* コンストラクタ */
	virtual ~CPacketSYSTEM_INFO();						/* デストラクタ */

	void	Make	(CInfoSystem *pInfoSystem);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoSystem		*m_pInfoSystem;				/* システム情報 */
} CPacketSYSTEM_INFO, *PCPacketSYSTEM_INFO;

/* Copyright(C)URARA-works 2008 */
