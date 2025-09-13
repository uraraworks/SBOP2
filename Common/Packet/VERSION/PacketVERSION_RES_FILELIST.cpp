/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_RES_FILELIST.cpp								 */
/* 内容			:コマンド(バージョン系:ファイルリスト応答) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/03/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoFileList.h"
#include "PacketVERSION_RES_FILELIST.h"

/* ========================================================================= */
/* 関数名	:CPacketVERSION_RES_FILELIST::CPacketVERSION_RES_FILELIST		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

CPacketVERSION_RES_FILELIST::CPacketVERSION_RES_FILELIST()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_RES_FILELIST::~CPacketVERSION_RES_FILELIST		 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

CPacketVERSION_RES_FILELIST::~CPacketVERSION_RES_FILELIST()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_RES_FILELIST::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

void CPacketVERSION_RES_FILELIST::Make(CInfoFileList *pSrc)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeInfo;
	PPACKETBASE pPacketBase;

	dwSizeInfo	= pSrc->GetSendDataSize ();
	pDataTmp	= pSrc->GetSendData ();
	dwSize		= sizeof (PACKETBASE) + dwSizeInfo;

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_RES_FILELIST;

	CopyMemory ((pPacketBase + 1), pDataTmp, dwSizeInfo);

	RenewPacket (pData, dwSize);
	SAFE_DELETE (pDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILE::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

PBYTE CPacketVERSION_RES_FILELIST::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo  = new CInfoFileList;
	pDataTmp = m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
