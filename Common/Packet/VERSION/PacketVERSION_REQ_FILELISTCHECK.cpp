/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_REQ_FILELISTCHECK.cpp						 */
/* 内容			:コマンド(バージョン系:ファイルリストチェック要求) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/28													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_FILELISTCHECK.h"

/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILELISTCHECK::CPacketVERSION_REQ_FILELISTCHECK */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/08/28														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILELISTCHECK::CPacketVERSION_REQ_FILELISTCHECK()
{
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILELISTCHECK::~CPacketVERSION_REQ_FILELISTCHECK */
/* 内容		:デストラクタ													 */
/* 日付		:2007/08/28														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILELISTCHECK::~CPacketVERSION_REQ_FILELISTCHECK()
{
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILELISTCHECK::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/08/28														 */
/* ========================================================================= */

void CPacketVERSION_REQ_FILELISTCHECK::Make(
	LPCSTR pszFileListHash)		/* [in] ファイルリストハッシュ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 (strlen (pszFileListHash) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_FILELISTCHECK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strcpyRenew ((LPSTR)pDataTmp, pszFileListHash, pDataTmp);		/* ファイルリストハッシュ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILELISTCHECK::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/08/28														 */
/* ========================================================================= */

PBYTE CPacketVERSION_REQ_FILELISTCHECK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	StoreRenew (m_strFileListHash, (LPCSTR)pDataTmp, pDataTmp);		/* ファイルリストハッシュ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
