/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketSKILL_PARA2.cpp										 */
/* 内容			:コマンド(スキル系:汎用[パラメータ2個]) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/06													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketSKILL_PARA2.h"

/* ========================================================================= */
/* 関数名	:CPacketSKILL_PARA2::CPacketSKILL_PARA2							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

CPacketSKILL_PARA2::CPacketSKILL_PARA2()
{
	m_dwPara1	= 0;
	m_dwPara2	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketSKILL_PARA2::~CPacketSKILL_PARA2						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

CPacketSKILL_PARA2::~CPacketSKILL_PARA2()
{
}


/* ========================================================================= */
/* 関数名	:CPacketSKILL_PARA2::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

void CPacketSKILL_PARA2::Make(
	BYTE byCmdSub,			/* [in] コマンドIDサブ */
	DWORD dwPara1/*0*/,		/* [in] パラメータ1 */
	DWORD dwPara2/*0*/)		/* [in] パラメータ2 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwPara1)		+
			 sizeof (dwPara2);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_SKILL;
	pPacketBase->byCmdSub	= byCmdSub;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwPara1, sizeof (dwPara1), pDataTmp);	/* パラメータ1 */
	CopyMemoryRenew (pDataTmp, &dwPara2, sizeof (dwPara2), pDataTmp);	/* パラメータ2 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketSKILL_PARA2::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

PBYTE CPacketSKILL_PARA2::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwPara1, pDataTmp, sizeof (m_dwPara1), pDataTmp);	/* パラメータ1 */
	CopyMemoryRenew (&m_dwPara2, pDataTmp, sizeof (m_dwPara2), pDataTmp);	/* パラメータ2 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
