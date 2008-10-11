/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_RES_FILELIST.h								 */
/* 内容			:コマンド(バージョン系:ファイルリスト応答) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/03/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoFileList;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketVERSION_RES_FILELIST : public CPacketBase
{
public:
			CPacketVERSION_RES_FILELIST();				/* コンストラクタ */
	virtual ~CPacketVERSION_RES_FILELIST();				/* デストラクタ */

	void	Make	(CInfoFileList *pSrc);						/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoFileList	*m_pInfo;					/* ファイルリスト情報 */
} CPacketVERSION_RES_FILELIST, *PCPacketVERSION_RES_FILELIST;

/* Copyright(C)URARA-works 2006 */
