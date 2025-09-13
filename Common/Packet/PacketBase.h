/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketBase.h												 */
/* 内容			:コマンド基底クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/08/01													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* 構造体宣言																 */
/* ========================================================================= */

typedef struct _PACKETBASE {
	BYTE	byCmdMain,					/* コマンドID(メイン) */
			byCmdSub;					/* コマンドID(サブ) */
} PACKETBASE, *PPACKETBASE;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketBase
{
public:
			CPacketBase();								/* コンストラクタ */
	virtual ~CPacketBase();								/* デストラクタ */

	DWORD	GetSize		(void);									/* パケットサイズを取得 */
	PBYTE	Get			(void);									/* パケットを取得 */
	PBYTE	Set			(PBYTE pPacket);						/* パケットを設定 */


protected:
	void	RenewPacket		(PBYTE pPacket, DWORD dwSize);		/* パケットを更新 */


public:
	PBYTE	m_pPacket;							/* パケット */
	DWORD	m_dwPacketSize;						/* パケットサイズ */
	BYTE	m_byCmdMain,						/* コマンドID(メイン) */
			m_byCmdSub;							/* コマンドID(サブ) */
} CPacketBase, *PCPacketBase;

/* Copyright(C)URARA-works 2006 */
