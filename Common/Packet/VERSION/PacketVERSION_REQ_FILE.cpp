/// @file PacketVERSION_REQ_FILE.cpp
/// @brief コマンド(バージョン系:ファイル要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/20
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_FILE.h"

CPacketVERSION_REQ_FILE::CPacketVERSION_REQ_FILE()
{
	m_dwOffset	= 0;
	m_dwReqSize	= 0;
}

CPacketVERSION_REQ_FILE::~CPacketVERSION_REQ_FILE()
{
}

void CPacketVERSION_REQ_FILE::Make(
	DWORD dwOffset,	// [in] 開始オフセット
	DWORD dwReqSize,	// [in] 要求サイズ
	LPCSTR pszFileName)	// [in] バージョンコード
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwOffset)	+
			 sizeof (dwReqSize)	+
			 (strlen(pszFileName) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_FILE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwOffset,	sizeof (dwOffset),	pDataTmp);	// 開始オフセット
	CopyMemoryRenew(pDataTmp, &dwReqSize,	sizeof (dwReqSize),	pDataTmp);	// 要求サイズ
	strcpyRenew((LPSTR)pDataTmp, pszFileName, pDataTmp);	// 相対パス付きファイル名

	RenewPacket(pData, dwSize);
}

PBYTE CPacketVERSION_REQ_FILE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwOffset,	pDataTmp, sizeof (m_dwOffset),	pDataTmp);	// 開始オフセット
	CopyMemoryRenew(&m_dwReqSize,	pDataTmp, sizeof (m_dwReqSize),	pDataTmp);	// 要求サイズ
	StoreRenew(m_strFileName,	(LPCSTR)pDataTmp, pDataTmp);	// 相対パス付きファイル名

	pRet = pDataTmp;
	return pRet;
}
