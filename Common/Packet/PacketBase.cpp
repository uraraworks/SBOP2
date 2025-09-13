/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketBase.cpp												 */
/* 内容			:コマンド基底クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/08/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "PacketBase.h"

/* ========================================================================= */
/* 関数名	:CPacketBase::CPacketBase										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/08/01														 */
/* ========================================================================= */

CPacketBase::CPacketBase()
{
	m_pPacket		= NULL;
	m_dwPacketSize	= 0;
	m_byCmdMain		= 0;
	m_byCmdSub		= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketBase::~CPacketBase										 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/08/01														 */
/* ========================================================================= */

CPacketBase::~CPacketBase()
{
	SAFE_DELETE_ARRAY (m_pPacket);
}


/* ========================================================================= */
/* 関数名	:CPacketBase::GetSize											 */
/* 内容		:パケットサイズを取得											 */
/* 日付		:2006/08/01														 */
/* ========================================================================= */

DWORD CPacketBase::GetSize(void)
{
	return m_dwPacketSize;
}


/* ========================================================================= */
/* 関数名	:CPacketBase::Get												 */
/* 内容		:パケットを取得													 */
/* 日付		:2006/08/01														 */
/* ========================================================================= */

PBYTE CPacketBase::Get(void)
{
	return m_pPacket;
}


/* ========================================================================= */
/* 関数名	:CPacketBase::Set												 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

PBYTE CPacketBase::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	PACKETBASE stPacketBase;

	pRet = pDataTmp = pPacket;

	CopyMemoryRenew ((PBYTE)&stPacketBase, pDataTmp, sizeof (stPacketBase), pDataTmp);

	m_byCmdMain	= stPacketBase.byCmdMain;		/* コマンドID(メイン) */
	m_byCmdSub	= stPacketBase.byCmdSub;		/* コマンドID(サブ) */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CPacketBase::RenewPacket										 */
/* 内容		:パケットを更新													 */
/* 日付		:2006/08/01														 */
/* ========================================================================= */

void CPacketBase::RenewPacket(PBYTE pPacket, DWORD dwSize)
{
	SAFE_DELETE_ARRAY (m_pPacket);

	m_pPacket		= pPacket;
	m_dwPacketSize	= dwSize;
}

/* Copyright(C)URARA-works 2006 */
