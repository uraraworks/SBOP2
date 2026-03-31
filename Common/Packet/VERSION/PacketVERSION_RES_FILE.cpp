/// @file PacketVERSION_RES_FILE.cpp
/// @brief コマンド(バージョン系:ファイル要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/20
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_RES_FILE.h"

CPacketVERSION_RES_FILE::CPacketVERSION_RES_FILE()
{
	m_dwOffset	= 0;
	m_dwSize	= 0;
	m_pFileData	= NULL;
}

CPacketVERSION_RES_FILE::~CPacketVERSION_RES_FILE()
{
	SAFE_DELETE_ARRAY(m_pFileData);
}

void CPacketVERSION_RES_FILE::Make(
	DWORD dwOffset,	// [in] 開始オフセット
	DWORD dwDataSize,	// [in] サイズ
	LPCSTR pszFileName,	// [in] バージョンコード
	PBYTE pFileData)	// [in] ファイルデータ
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwOffset)	+
			 sizeof (dwDataSize)	+
			 (strlen(pszFileName) + 1)	+
			 dwDataSize;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_RES_FILE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwOffset,	sizeof (dwOffset),	pDataTmp);	// 開始オフセット
	CopyMemoryRenew(pDataTmp, &dwDataSize,	sizeof (dwSize),	pDataTmp);	// サイズ
	strcpyRenew((LPSTR)pDataTmp, pszFileName, pDataTmp);	// 相対パス付きファイル名
	CopyMemoryRenew(pDataTmp, pFileData,	dwDataSize,	pDataTmp);	// ファイルデータ

	RenewPacket(pData, dwSize);
}

PBYTE CPacketVERSION_RES_FILE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE_ARRAY(m_pFileData);
	CopyMemoryRenew(&m_dwOffset,	pDataTmp, sizeof (m_dwOffset),	pDataTmp);	// 開始オフセット
	CopyMemoryRenew(&m_dwSize,	pDataTmp, sizeof (m_dwSize),	pDataTmp);	// サイズ
	StoreRenew(m_strFileName,	(LPCSTR)pDataTmp, pDataTmp);	// 相対パス付きファイル名

	m_pFileData = new BYTE[m_dwSize];
	CopyMemoryRenew(m_pFileData, pDataTmp, m_dwSize, pDataTmp);	// ファイルデータ

	pRet = pDataTmp;
	return pRet;
}
