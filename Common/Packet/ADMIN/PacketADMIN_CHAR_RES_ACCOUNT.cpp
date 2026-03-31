/// @file PacketADMIN_CHAR_RES_ACCOUNT.cpp
/// @brief コマンド(管理者系:アカウント情報応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/07
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "InfoAccount.h"
#include "PacketADMIN_CHAR_RES_ACCOUNT.h"

CPacketADMIN_CHAR_RES_ACCOUNT::CPacketADMIN_CHAR_RES_ACCOUNT()
{
	m_dwAccountID = 0;
	m_dwIP = 0;
}

CPacketADMIN_CHAR_RES_ACCOUNT::~CPacketADMIN_CHAR_RES_ACCOUNT()
{
}

void CPacketADMIN_CHAR_RES_ACCOUNT::Make(
	CInfoAccount *pInfoAccount,	// [in] アカウント情報
	DWORD dwIP,	// [in] IPアドレス
	LPCSTR pszMacAddress)	// [in] MACアドレス
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwIP) +
			 sizeof (pInfoAccount->m_dwAccountID) +
			 	(pInfoAccount->m_strAccount.GetLegacyStoreLength() + 1) +
			 	(pInfoAccount->m_strPassword.GetLegacyStoreLength() + 1) +
			(strlen(pszMacAddress) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ACCOUNT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &pInfoAccount->m_dwAccountID, sizeof (pInfoAccount->m_dwAccountID), pDataTmp);	// アカウントID
	CopyMemoryRenew(pDataTmp, &dwIP, sizeof (dwIP), pDataTmp);	// IPアドレス
	strcpyRenew((LPSTR)pDataTmp, pInfoAccount->m_strAccount.GetLegacyAnsiPointer(),  pDataTmp);	// アカウント
	strcpyRenew((LPSTR)pDataTmp, pInfoAccount->m_strPassword.GetLegacyAnsiPointer(), pDataTmp);	// パスワード
	strcpyRenew((LPSTR)pDataTmp, pszMacAddress, pDataTmp);	// MACアドレス

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_CHAR_RES_ACCOUNT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	// アカウントID
	CopyMemoryRenew(&m_dwIP, pDataTmp, sizeof (m_dwIP), pDataTmp);	// IPアドレス
	StoreRenew(m_strAccount,	(LPCSTR)pDataTmp, pDataTmp);	// アカウント
	StoreRenew(m_strPassword,	(LPCSTR)pDataTmp, pDataTmp);	// パスワード
	StoreRenew(m_strMacAddress,(LPCSTR)pDataTmp, pDataTmp);	// MACアドレス

	pRet = pDataTmp;
	return pRet;
}
