/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名：	MacAddr.cpp													 */
/* 内容：		MACアドレス取得クラス 実装ファイル							 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2005/04/12													 */
/* ========================================================================= */

#include "StdAfx.h"
#include <Iphlpapi.h>
#include "MacAddr.h"

#pragma comment(lib, "Iphlpapi.lib")


/* ========================================================================= */
/* 関数名：	CMacAddr::CMacAddr												 */
/* 内容：	コンストラクタ													 */
/* 日付：	2005/04/12														 */
/* ========================================================================= */

CMacAddr::CMacAddr()
{
	m_nCount = GetNICCount ();
}


/* ========================================================================= */
/* 関数名：	CMacAddr::~CMacAddr												 */
/* 内容：	デストラクタ													 */
/* 日付：	2005/04/12														 */
/* ========================================================================= */

CMacAddr::~CMacAddr()
{
}


/* ========================================================================= */
/* 関数名：	CMacAddr::Get													 */
/* 内容：	MACアドレスを取得												 */
/* 日付：	2005/04/12														 */
/* ========================================================================= */

BOOL CMacAddr::Get(
	PBYTE pDst,		/* [ou] 取得したMACアドレス(バイナリ6バイト) */
	int nNo)		/* [in] 取得したいNIC番号(0が最初) */
{
	int		nCount;
	DWORD	dwResult, dwSize;
	BOOL	bRet;
	PBYTE	pBuff;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADDR_STRING pIPAddress;

	bRet	= FALSE;
	dwSize	= 0;

	/* 必要なサイズを求める */
	GetAdaptersInfo (NULL, &dwSize);

	pBuff			= new BYTE[dwSize];
	pAdapterInfo	= (PIP_ADAPTER_INFO)pBuff;

	/* 情報を取得 */
	dwResult = GetAdaptersInfo (pAdapterInfo, &dwSize);
	if (dwResult != ERROR_SUCCESS) {
		goto Exit;
	}

	BYTE abyNullMacAddress[6];
	ZeroMemory(abyNullMacAddress, sizeof(abyNullMacAddress));
	for (nCount = 0; ; nCount ++) {
		if (nCount == nNo) {
			pIPAddress = &pAdapterInfo->IpAddressList;
			CopyMemory (pDst, pAdapterInfo->Address, 6);
			if (memcmp(abyNullMacAddress, pDst, 6)) {
				bRet = TRUE;
				break;
			}
#if 0
			if (!((pIPAddress->Context == 0) ||
				 (strcmp (pIPAddress->IpAddress.String, "0.0.0.0") == 0) ||
				 (strcmp (pIPAddress->IpAddress.String, "127.0.0.1") == 0))) {
				CopyMemory (pDst, pAdapterInfo->Address, 6);
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


/* ========================================================================= */
/* 関数名：	CMacAddr::GetMACAddrStr											 */
/* 内容：	MACアドレスを取得(文字列)										 */
/* 日付：	2005/04/12														 */
/* ========================================================================= */

BOOL CMacAddr::GetStr(
	LPSTR pszDst,		/* [ou] 取得したMACアドレス */
	int nNo)			/* [in] 取得したいNIC番号(0が最初) */
{
	BOOL bResult;
	BYTE byMACAddr[6];

	ZeroMemory (byMACAddr, sizeof (byMACAddr));
	bResult = Get (byMACAddr, nNo);
	wsprintf (pszDst, "%02X-%02X-%02X-%02X-%02X-%02X",
			byMACAddr[0], byMACAddr[1], byMACAddr[2], 
			byMACAddr[3], byMACAddr[4], byMACAddr[5]);

	return bResult;
}


/* ========================================================================= */
/* 関数名：	CMacAddr::GetCount												 */
/* 内容：	NIC数を取得														 */
/* 日付：	2005/04/12														 */
/* ========================================================================= */

int CMacAddr::GetCount(void)
{
	return m_nCount;
}


/* ========================================================================= */
/* 関数名：	CMacAddr::GetDeviceName											 */
/* 内容：	デバイス名を取得												 */
/* 日付：	2005/04/12														 */
/* ========================================================================= */

BOOL CMacAddr::GetDeviceName(
	LPSTR pszDst,	/* [ou] 取得したデバイス名(十分なサイズが必要) */
	int nNo)		/* [in] 取得したいNIC番号(0が最初) */
{
	int		nCount;
	DWORD	dwResult, dwSize;
	BOOL	bRet;
	PBYTE	pBuff;
	LPSTR	pszTmp;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADDR_STRING pIPAddress;

	bRet	= FALSE;
	dwSize	= 0;

	/* 必要なサイズを求める */
	GetAdaptersInfo (NULL, &dwSize);

	pBuff			= new BYTE[dwSize];
	pAdapterInfo	= (PIP_ADAPTER_INFO)pBuff;

	/* 情報を取得 */
	dwResult = GetAdaptersInfo (pAdapterInfo, &dwSize);
	if (dwResult != ERROR_SUCCESS) {
		goto Exit;
	}

	for (nCount = 0; ; nCount ++) {
		if (nCount == nNo) {
			pIPAddress = &pAdapterInfo->IpAddressList;
			if (!((pIPAddress->Context == 0) ||
				 (strcmp (pIPAddress->IpAddress.String, "0.0.0.0") == 0)) ||
				 (strcmp (pIPAddress->IpAddress.String, "127.0.0.1") == 0)) {
				strcpy (pszDst, pAdapterInfo->Description);
				pszTmp = strstr (pszDst, "-");
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


/* ========================================================================= */
/* 関数名：	CMacAddr::GetNICCount											 */
/* 内容：	NIC数を取得														 */
/* 日付：	2005/04/12														 */
/* ========================================================================= */

int CMacAddr::GetNICCount(void)
{
	int		nRet;
	DWORD	dwResult, dwSize;
	PBYTE	pBuff;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADDR_STRING pIPAddress;

	nRet	= 0;
	dwSize	= 0;

	/* 必要なサイズを求める */
	GetAdaptersInfo (NULL, &dwSize);

	pBuff			= new BYTE[dwSize];
	pAdapterInfo	= (PIP_ADAPTER_INFO)pBuff;

	/* 情報を取得 */
	dwResult = GetAdaptersInfo (pAdapterInfo, &dwSize);
	if (dwResult != ERROR_SUCCESS) {
		goto Exit;
	}

	for (nRet = 1; ; nRet ++) {
		pIPAddress = &pAdapterInfo->IpAddressList;
		if (((pIPAddress->Context == 0) ||
			 (strcmp (pIPAddress->IpAddress.String, "0.0.0.0") == 0)) ||
			 (strcmp (pIPAddress->IpAddress.String, "127.0.0.1") == 0)) {
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

/* Copyright(C)URARA-works 2005 */
