/// @file MacAddr.cpp
/// @brief MACアドレス取得クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/12
/// @copyright Copyright(C)URARA-works 2005

#include "StdAfx.h"
#include <cstdio>
#include <Iphlpapi.h>
#include "MacAddr.h"

#pragma comment(lib, "Iphlpapi.lib")

CMacAddr::CMacAddr()
{
	m_nCount = GetNICCount();
}

CMacAddr::~CMacAddr()
{
}

BOOL CMacAddr::Get(
	PBYTE pDst,	// [out] 取得したMACアドレス(バイナリ6バイト)
	int nNo)	// [in]  取得したいNIC番号(0が最初)
{
	int		nCount;
	DWORD	dwResult, dwSize;
	BOOL	bRet;
	PBYTE	pBuff;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADDR_STRING pIPAddress;

	bRet   = FALSE;
	dwSize = 0;

	// 必要なサイズを求める
	GetAdaptersInfo(NULL, &dwSize);

	pBuff        = new BYTE[dwSize];
	pAdapterInfo = (PIP_ADAPTER_INFO)pBuff;

	// 情報を取得
	dwResult = GetAdaptersInfo(pAdapterInfo, &dwSize);
	if (dwResult != ERROR_SUCCESS) {
		goto Exit;
	}

	BYTE abyNullMacAddress[6];
	ZeroMemory(abyNullMacAddress, sizeof(abyNullMacAddress));
	for (nCount = 0; ; nCount ++) {
		if (nCount == nNo) {
			pIPAddress = &pAdapterInfo->IpAddressList;
			CopyMemory(pDst, pAdapterInfo->Address, 6);
			if (memcmp(abyNullMacAddress, pDst, 6)) {
				bRet = TRUE;
				break;
			}
#if 0
			if (!((pIPAddress->Context == 0) ||
				 (strcmp(pIPAddress->IpAddress.String, "0.0.0.0") == 0) ||
				 (strcmp(pIPAddress->IpAddress.String, "127.0.0.1") == 0))) {
				CopyMemory(pDst, pAdapterInfo->Address, 6);
				bRet = TRUE;
				break;

			} else {
				nCount --;
			}
#endif
		}

		pAdapterInfo = pAdapterInfo->Next;
		if (pAdapterInfo == NULL) {
			break;
		}
	}

Exit:
	if (pBuff) {
		delete [] pBuff;
	}
	return bRet;
}

BOOL CMacAddr::GetStr(
	LPSTR pszDst,	// [out] 取得したMACアドレス
	int nNo)		// [in]  取得したいNIC番号(0が最初)
{
	BOOL bResult;
	BYTE byMACAddr[6];

	ZeroMemory(byMACAddr, sizeof(byMACAddr));
	bResult = Get(byMACAddr, nNo);
	sprintf_s(pszDst, 18, "%02X-%02X-%02X-%02X-%02X-%02X",
			byMACAddr[0], byMACAddr[1], byMACAddr[2],
			byMACAddr[3], byMACAddr[4], byMACAddr[5]);

	return bResult;
}

int CMacAddr::GetCount(void)
{
	return m_nCount;
}

BOOL CMacAddr::GetDeviceName(
	LPSTR pszDst,	// [out] 取得したデバイス名(十分なサイズが必要)
	int nNo)		// [in]  取得したいNIC番号(0が最初)
{
	int		nCount;
	DWORD	dwResult, dwSize;
	BOOL	bRet;
	PBYTE	pBuff;
	LPSTR	pszTmp;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADDR_STRING pIPAddress;

	bRet   = FALSE;
	dwSize = 0;

	// 必要なサイズを求める
	GetAdaptersInfo(NULL, &dwSize);

	pBuff        = new BYTE[dwSize];
	pAdapterInfo = (PIP_ADAPTER_INFO)pBuff;

	// 情報を取得
	dwResult = GetAdaptersInfo(pAdapterInfo, &dwSize);
	if (dwResult != ERROR_SUCCESS) {
		goto Exit;
	}

	for (nCount = 0; ; nCount ++) {
		if (nCount == nNo) {
			pIPAddress = &pAdapterInfo->IpAddressList;
			if (!((pIPAddress->Context == 0) ||
				 (strcmp(pIPAddress->IpAddress.String, "0.0.0.0") == 0)) ||
				 (strcmp(pIPAddress->IpAddress.String, "127.0.0.1") == 0)) {
				strcpy(pszDst, pAdapterInfo->Description);
				pszTmp = strstr(pszDst, "-");
				if (pszTmp) {
					*pszTmp = 0;
				}
				bRet = TRUE;
				break;

			} else {
				nCount --;
			}
		}

		pAdapterInfo = pAdapterInfo->Next;
		if (pAdapterInfo == NULL) {
			break;
		}
	}

Exit:
	if (pBuff) {
		delete [] pBuff;
	}
	return bRet;
}

int CMacAddr::GetNICCount(void)
{
	int		nRet;
	DWORD	dwResult, dwSize;
	PBYTE	pBuff;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADDR_STRING pIPAddress;

	nRet   = 0;
	dwSize = 0;

	// 必要なサイズを求める
	GetAdaptersInfo(NULL, &dwSize);

	pBuff        = new BYTE[dwSize];
	pAdapterInfo = (PIP_ADAPTER_INFO)pBuff;

	// 情報を取得
	dwResult = GetAdaptersInfo(pAdapterInfo, &dwSize);
	if (dwResult != ERROR_SUCCESS) {
		goto Exit;
	}

	for (nRet = 1; ; nRet ++) {
		pIPAddress = &pAdapterInfo->IpAddressList;
		if (((pIPAddress->Context == 0) ||
			 (strcmp(pIPAddress->IpAddress.String, "0.0.0.0") == 0)) ||
			 (strcmp(pIPAddress->IpAddress.String, "127.0.0.1") == 0)) {
			nRet --;
		}
		pAdapterInfo = pAdapterInfo->Next;
		if (pAdapterInfo == NULL) {
			break;
		}
	}

Exit:
	if (pBuff) {
		delete [] pBuff;
	}
	return nRet;
}
