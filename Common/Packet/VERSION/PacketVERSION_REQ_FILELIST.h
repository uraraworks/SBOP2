/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_REQ_FILELIST.h								 */
/* 内容			:コマンド(バージョン系:ファイルリスト要求) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/03/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketVERSION_REQ_FILELIST : public CPacketBase
{
public:
			CPacketVERSION_REQ_FILELIST();				/* コンストラクタ */
	virtual ~CPacketVERSION_REQ_FILELIST();				/* デストラクタ */

	void	Make	(void);										/* パケットを作成 */
} CPacketVERSION_REQ_FILELIST, *PCPacketVERSION_REQ_FILELIST;

/* Copyright(C)URARA-works 2006 */
