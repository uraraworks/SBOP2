/// @file PacketVERSION_REQ_FILELISTCHECK.cpp
/// @brief コマンド(バージョン系:ファイルリストチェック要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/28
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_FILELISTCHECK.h"

CPacketVERSION_REQ_FILELISTCHECK::CPacketVERSION_REQ_FILELISTCHECK()
{
}

CPacketVERSION_REQ_FILELISTCHECK::~CPacketVERSION_REQ_FILELISTCHECK()
{
}

void CPacketVERSION_REQ_FILELISTCHECK::Make(
	LPCSTR pszFileListHash)	// [in] ファイルリストハッシュ
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 (strlen(pszFileListHash) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_FILELISTCHECK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strcpyRenew((LPSTR)pDataTmp, pszFileListHash, pDataTmp);	// ファイルリストハッシュ

	RenewPacket(pData, dwSize);
}

PBYTE CPacketVERSION_REQ_FILELISTCHECK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	StoreRenew(m_strFileListHash, (LPCSTR)pDataTmp, pDataTmp);	// ファイルリストハッシュ

	pRet = pDataTmp;
	return pRet;
}
