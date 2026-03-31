/// @file PacketVERSION_RES_FILELIST.cpp
/// @brief コマンド(バージョン系:ファイルリスト応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/03/08
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "InfoFileList.h"
#include "PacketVERSION_RES_FILELIST.h"

CPacketVERSION_RES_FILELIST::CPacketVERSION_RES_FILELIST()
{
	m_pInfo = NULL;
}

CPacketVERSION_RES_FILELIST::~CPacketVERSION_RES_FILELIST()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketVERSION_RES_FILELIST::Make(CInfoFileList *pSrc)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeInfo;
	PPACKETBASE pPacketBase;

	dwSizeInfo	= pSrc->GetSendDataSize();
	pDataTmp	= pSrc->GetSendData();
	dwSize	= sizeof (PACKETBASE) + dwSizeInfo;

	pData = ZeroNew(dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_RES_FILELIST;

	CopyMemory((pPacketBase + 1), pDataTmp, dwSizeInfo);

	RenewPacket(pData, dwSize);
	SAFE_DELETE(pDataTmp);
}

PBYTE CPacketVERSION_RES_FILELIST::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfo);
	m_pInfo  = new CInfoFileList;
	pDataTmp = m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
