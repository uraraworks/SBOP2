/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ƒtƒ@ƒCƒ‹–¼F	MacAddr.cpp													 */
/* “à—eF		MACƒAƒhƒŒƒXŽæ“¾ƒNƒ‰ƒX ŽÀ‘•ƒtƒ@ƒCƒ‹							 */
/* ì¬F		”N‚ª‚ç”N’†t‚¤‚ç‚ç(URARA-works)								 */
/* ì¬ŠJŽn“úF	2005/04/12													 */
/* ========================================================================= */

#include "StdAfx.h"
#include <Iphlpapi.h>
#include "MacAddr.h"

#pragma comment(lib, "Iphlpapi.lib")


/* ========================================================================= */
/* ŠÖ”–¼F	CMacAddr::CMacAddr												 */
/* “à—eF	ƒRƒ“ƒXƒgƒ‰ƒNƒ^													 */
/* “ú•tF	2005/04/12														 */
/* ========================================================================= */

CMacAddr::CMacAddr()
{
	m_nCount = GetNICCount ();
}


/* ========================================================================= */
/* ŠÖ”–¼F	CMacAddr::~CMacAddr												 */
/* “à—eF	ƒfƒXƒgƒ‰ƒNƒ^													 */
/* “ú•tF	2005/04/12														 */
/* ========================================================================= */

CMacAddr::~CMacAddr()
{
}


/* ========================================================================= */
/* ŠÖ”–¼F	CMacAddr::Get													 */
/* “à—eF	MACƒAƒhƒŒƒX‚ðŽæ“¾												 */
/* “ú•tF	2005/04/12														 */
/* ========================================================================= */

BOOL CMacAddr::Get(
	PBYTE pDst,		/* [ou] Žæ“¾‚µ‚½MACƒAƒhƒŒƒX(ƒoƒCƒiƒŠ6ƒoƒCƒg) */
	int nNo)		/* [in] Žæ“¾‚µ‚½‚¢NIC”Ô†(0‚ªÅ‰) */
{
	int		nCount;
	DWORD	dwResult, dwSize;
	BOOL	bRet;
	PBYTE	pBuff;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADDR_STRING pIPAddress;

	bRet	= FALSE;
	dwSize	= 0;

	/* •K—v‚ÈƒTƒCƒY‚ð‹‚ß‚é */
	GetAdaptersInfo (NULL, &dwSize);

	pBuff			= new BYTE[dwSize];
	pAdapterInfo	= (PIP_ADAPTER_INFO)pBuff;

	/* î•ñ‚ðŽæ“¾ */
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
/* ŠÖ”–¼F	CMacAddr::GetMACAddrStr											 */
/* “à—eF	MACƒAƒhƒŒƒX‚ðŽæ“¾(•¶Žš—ñ)										 */
/* “ú•tF	2005/04/12														 */
/* ========================================================================= */

BOOL CMacAddr::GetStr(
	LPSTR pszDst,		/* [ou] Žæ“¾‚µ‚½MACƒAƒhƒŒƒX */
	int nNo)			/* [in] Žæ“¾‚µ‚½‚¢NIC”Ô†(0‚ªÅ‰) */
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
/* ŠÖ”–¼F	CMacAddr::GetCount												 */
/* “à—eF	NIC”‚ðŽæ“¾														 */
/* “ú•tF	2005/04/12														 */
/* ========================================================================= */

int CMacAddr::GetCount(void)
{
	return m_nCount;
}


/* ========================================================================= */
/* ŠÖ”–¼F	CMacAddr::GetDeviceName											 */
/* “à—eF	ƒfƒoƒCƒX–¼‚ðŽæ“¾												 */
/* “ú•tF	2005/04/12														 */
/* ========================================================================= */

BOOL CMacAddr::GetDeviceName(
	LPSTR pszDst,	/* [ou] Žæ“¾‚µ‚½ƒfƒoƒCƒX–¼(\•ª‚ÈƒTƒCƒY‚ª•K—v) */
	int nNo)		/* [in] Žæ“¾‚µ‚½‚¢NIC”Ô†(0‚ªÅ‰) */
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

	/* •K—v‚ÈƒTƒCƒY‚ð‹‚ß‚é */
	GetAdaptersInfo (NULL, &dwSize);

	pBuff			= new BYTE[dwSize];
	pAdapterInfo	= (PIP_ADAPTER_INFO)pBuff;

	/* î•ñ‚ðŽæ“¾ */
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
/* ŠÖ”–¼F	CMacAddr::GetNICCount											 */
/* “à—eF	NIC”‚ðŽæ“¾														 */
/* “ú•tF	2005/04/12														 */
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

	/* •K—v‚ÈƒTƒCƒY‚ð‹‚ß‚é */
	GetAdaptersInfo (NULL, &dwSize);

	pBuff			= new BYTE[dwSize];
	pAdapterInfo	= (PIP_ADAPTER_INFO)pBuff;

	/* î•ñ‚ðŽæ“¾ */
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
