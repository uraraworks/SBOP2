/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_REQ_FILELISTCHECK.h							 */
/* 内容			:コマンド(バージョン系:ファイルリストチェック要求) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/28													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketVERSION_REQ_FILELISTCHECK : public CPacketBase
{
public:
			CPacketVERSION_REQ_FILELISTCHECK();			/* コンストラクタ */
	virtual ~CPacketVERSION_REQ_FILELISTCHECK();		/* デストラクタ */

	void	Make	(LPCSTR pszFileListHash);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CmyString	m_strFileListHash;				/* ファイルリストハッシュ */
} CPacketVERSION_REQ_FILELISTCHECK, *PCPacketVERSION_REQ_FILELISTCHECK;

/* Copyright(C)URARA-works 2006 */
