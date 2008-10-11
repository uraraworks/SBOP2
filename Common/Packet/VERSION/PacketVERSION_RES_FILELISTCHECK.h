/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_RES_FILELISTCHECK.h							 */
/* 内容			:コマンド(バージョン系:ファイルリストチェック応答) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/28													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

enum {
	FILELISTCHECKRES_NONE = 0,
	FILELISTCHECKRES_OK,					/* 問題無し */
	FILELISTCHECKRES_NG,					/* 不一致 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketVERSION_RES_FILELISTCHECK : public CPacketBase
{
public:
			CPacketVERSION_RES_FILELISTCHECK();			/* コンストラクタ */
	virtual ~CPacketVERSION_RES_FILELISTCHECK();		/* デストラクタ */

	void	Make	(int nResult);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	int		m_nResult;					/* 結果 */
} CPacketVERSION_RES_FILELISTCHECK, *PCPacketVERSION_RES_FILELISTCHECK;

/* Copyright(C)URARA-works 2007 */
