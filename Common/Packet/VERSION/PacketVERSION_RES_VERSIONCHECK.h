/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_RES_VERSIONCHECK.h							 */
/* 内容			:コマンド(バージョン系:バージョンチェック応答) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

enum {
	VERSIONCHECKRES_NONE = 0,
	VERSIONCHECKRES_OK,							/* 問題無し */
	VERSIONCHECKRES_NG_VERSION,					/* バージョン不一致 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketVERSION_RES_VERSIONCHECK : public CPacketBase
{
public:
			CPacketVERSION_RES_VERSIONCHECK();			/* コンストラクタ */
	virtual ~CPacketVERSION_RES_VERSIONCHECK();			/* デストラクタ */

	void	Make	(int nResult);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	int		m_nResult;					/* 結果 */
} CPacketVERSION_RES_VERSIONCHECK, *PCPacketVERSION_RES_VERSIONCHECK;

/* Copyright(C)URARA-works 2006 */
