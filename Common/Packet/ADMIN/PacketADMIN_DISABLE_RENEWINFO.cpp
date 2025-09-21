/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_DISABLE_RENEWINFO.cpp							 */
/* 内容			:コマンド(管理者系:拒否情報の更新) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/11													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "LibInfoDisable.h"
#include "PacketADMIN_DISABLE_RENEWINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_DISABLE_RENEWINFO::CPacketADMIN_DISABLE_RENEWINFO	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

CPacketADMIN_DISABLE_RENEWINFO::CPacketADMIN_DISABLE_RENEWINFO()
{
	m_pLibInfoDisable = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_DISABLE_RENEWINFO::~CPacketADMIN_DISABLE_RENEWINFO */
/* 内容		:デストラクタ													 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

CPacketADMIN_DISABLE_RENEWINFO::~CPacketADMIN_DISABLE_RENEWINFO()
{
	SAFE_DELETE (m_pLibInfoDisable);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_DISABLE_RENEWINFO::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

void CPacketADMIN_DISABLE_RENEWINFO::Make(
	CLibInfoDisable *pLibInfoDisable)		/* [in] 拒否情報 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeLibInfoDisable;
	PBYTE pDataLibInfoDisable;
	PPACKETBASE pPacketBase;

	dwSizeLibInfoDisable = pLibInfoDisable->GetSendDataSize ();
	pDataLibInfoDisable	 = pLibInfoDisable->GetSendData ();

	dwSize = sizeof (PACKETBASE);
	dwSize += dwSizeLibInfoDisable;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_DISABLE_RENEWINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pDataLibInfoDisable, dwSizeLibInfoDisable, pDataTmp);	/* 拒否情報 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_DISABLE_RENEWINFO::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

void CPacketADMIN_DISABLE_RENEWINFO::Make(
	CInfoDisable *pInfoDisable)		/* [in] 拒否情報 */
{
	PCInfoDisable pInfoDisableTmp;
	CLibInfoDisable LibInfo;

	pInfoDisableTmp = (PCInfoDisable)LibInfo.GetNew ();
	pInfoDisableTmp->Copy (pInfoDisable);
	LibInfo.Create ();
	LibInfo.Add (pInfoDisableTmp);
	Make (&LibInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_DISABLE_RENEWINFO::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

PBYTE CPacketADMIN_DISABLE_RENEWINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	SAFE_DELETE (m_pLibInfoDisable);
	m_pLibInfoDisable = new CLibInfoDisable;
	m_pLibInfoDisable->Create ();

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);
	pDataTmp	= m_pLibInfoDisable->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2009 */
